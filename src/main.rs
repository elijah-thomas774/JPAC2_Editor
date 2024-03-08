#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]
#![feature(slice_as_chunks)]

use std::{
    fs::File,
    io::{Cursor, Read},
};

use crate::jpa::JPAC;
use binrw::{BinRead, BinWrite};
use eframe::egui;
use egui::{menu, Context, Ui};
use jpa::{ResouceSelector, Selected};

pub mod bti;
mod jpa;
pub mod ui_helpers;

fn main() -> Result<(), eframe::Error> {
    env_logger::init(); // Log to stderr (if you run with `RUST_LOG=debug`).
    let options = eframe::NativeOptions {
        ..Default::default()
    };
    eframe::run_native(
        "JPAC Editor",
        options,
        Box::new(|_cc| Box::<MyApp>::default()),
    )
}

#[derive(Debug, Default, PartialEq, Clone, Copy)]
enum FilterType {
    #[default]
    None,
    Texture,
    Resource,
    Name,
}

#[derive(Default)]
enum MainWindowState {
    #[default]
    None,
    Exporter,
    Editor,
    ErrorMessage(String),
    Viewer, // NYI: Probably for a bit
}

#[derive(Default)]
struct FilterSettings {
    text:        String,
    filter_type: FilterType,
}

#[derive(Default)]
struct MyApp {
    jpac:               Option<JPAC>,
    tree_filter:        FilterSettings,
    main_window:        MainWindowState,
    selected_resources: ResouceSelector,
    selected:           Selected,
}

/// Menu Bar
impl MyApp {
    fn menu_bar(&mut self, ui: &mut Ui, ctx: &Context) {
        menu::bar(ui, |ui| {
            ui.menu_button("File", |ui| {
                self.jpc_open(ui, ctx);
                self.jpc_save(ui);
            });
            ui.menu_button("Resource", |ui| {
                self.resource_import(ui);
                self.resource_export(ui);
            });
            ui.menu_button("Alias", |ui| {
                self.alias_import(ui);
                self.alias_export(ui);
            });
        });
    }

    fn jpc_open(&mut self, ui: &mut Ui, ctx: &Context) {
        if ui.button("Open JPC").clicked() {
            if let Some(path) = rfd::FileDialog::new()
                .add_filter("JPC", &["jpc"])
                .pick_file()
            {
                // Clear the existing JPC
                // TODO: Check Dirty and confirm?
                self.jpac = None;

                let mut file_contents: Vec<u8> = Vec::new();
                File::open(&path)
                    .expect("File Could not be opened")
                    .read_to_end(&mut file_contents)
                    .expect("Could not Read File Contents");
                match JPAC::read(&mut Cursor::new(file_contents)) {
                    Ok(mut jpac) => {
                        jpac.load_textures(ctx);
                        self.jpac = Some(jpac);
                        self.main_window = MainWindowState::Editor;
                    },
                    Err(msg) => {
                        self.main_window = MainWindowState::ErrorMessage(msg.to_string());
                    },
                }
            }
            ui.close_menu();
        }
    }

    fn jpc_save(&mut self, ui: &mut Ui) {
        if ui.button("Save").clicked() {
            if let Some(jpac) = &mut self.jpac {
                if let Some(path) = &rfd::FileDialog::new()
                    .set_file_name("new_jpc.jpc")
                    .add_filter("JPC", &["jpc"])
                    .save_file()
                {
                    let mut f = File::create(path).expect("File Could not be opened");
                    jpac.write(&mut f).expect("File could not be read");
                }
                ui.close_menu();
            } else {
                self.main_window =
                    MainWindowState::ErrorMessage("Please Open a JPC to Save".to_string());
            }
        }
    }

    fn alias_export(&mut self, ui: &mut Ui) {
        if ui.button("Save Alias").clicked() {
            if let Some(jpac) = &mut self.jpac {
                if let Some(path) = &rfd::FileDialog::new()
                    .set_file_name("resource_alias.json")
                    .add_filter("Resource Alias", &["json"])
                    .save_file()
                {
                    let mut f = File::create(path).expect("Unable to open/create file");
                    serde_json::to_writer_pretty(&mut f, &jpac.export_alias()).unwrap();
                }
            } else {
                self.main_window = MainWindowState::ErrorMessage(
                    "Please Open a JPC to Export an Alias".to_string(),
                );
            }
            ui.close_menu();
        }
    }
    fn alias_import(&mut self, ui: &mut Ui) {
        if ui.button("Open Alias").clicked() {
            if let Some(jpac) = &mut self.jpac {
                if let Some(path) = &rfd::FileDialog::new()
                    .set_file_name("resource_alias.json")
                    .add_filter("Resource Alias", &["json"])
                    .pick_file()
                {
                    let mut f = File::open(path).expect("Unable to open file");
                    jpac.apply_alias(&serde_json::from_reader(&mut f).unwrap());
                }
            } else {
                self.main_window = MainWindowState::ErrorMessage(
                    "Please Open a JPC to import an Alias".to_string(),
                );
            }
            ui.close_menu();
        }
    }

    fn resource_export(&mut self, ui: &mut Ui) {
        if ui.button("Export Resources").clicked() {
            self.main_window = MainWindowState::Exporter;
            ui.close_menu();
        }
    }

    fn resource_import(&mut self, ui: &mut Ui) {
        if ui.button("Import Resources").clicked() {
            if let Some(jpac) = &mut self.jpac {
                if let Some(path) = rfd::FileDialog::new()
                    .add_filter("JPA Resource", &["jpares"])
                    .pick_file()
                {
                    let path = path.display().to_string();
                    let mut f = File::open(&path).expect("File Could not be opened");
                    print!("{:?}", jpac.import_resources(&mut f));
                }
            } else {
                self.main_window = MainWindowState::ErrorMessage(
                    "Please Open a JPC to import a resource".to_string(),
                );
            }
            ui.close_menu();
        }
    }
}

/// Main View
impl MyApp {
    fn main_window(&mut self, ctx: &Context) {
        egui::CentralPanel::default().show(ctx, |ui| {
            match &self.main_window {
                MainWindowState::None => self.show_into_text(ui),
                MainWindowState::Exporter => self.show_exporter(ui),
                MainWindowState::Editor => self.show_editor(self.selected, ui),
                MainWindowState::Viewer => self.show_viewer(ui),
                MainWindowState::ErrorMessage(errormsg) => {
                    self.show_error_message(&errormsg.clone(), ui)
                },
            }
        });
    }

    fn show_into_text(&mut self, ui: &mut Ui) {
        ui.label("Welcome to the JPAC Editor! \n To get started, go to the `File` tab and select the .jpc file. \n Note: Currently the program only support JPAC2-11 (Version for Skyward Sword)");
    }

    fn show_error_message(&mut self, message: &String, ui: &mut Ui) {
        ui.label(message);
    }

    fn show_editor(&mut self, selected: Selected, ui: &mut Ui) {
        if let Some(jpac) = &mut self.jpac {
            jpac.show_editor(selected, ui);
        } else {
            self.main_window = MainWindowState::ErrorMessage(
                "To Show the Editor, please open a resource file".to_string(),
            );
        }
    }

    /// Interface for showing the resource exporter
    fn show_exporter(&mut self, ui: &mut Ui) {
        if let Some(jpac) = &mut self.jpac {
            ui.horizontal(|ui| {
                if ui.button("Export").clicked() {
                    if let Some(path) = rfd::FileDialog::new()
                        .add_filter("JPA Resource", &["jpares"])
                        .save_file()
                    {
                        let path = path.display().to_string();
                        let mut f = File::create(&path).expect("File Could not be opened");
                        let _ = jpac.export_resources(&self.selected_resources.selected, &mut f);
                    }
                    self.main_window = MainWindowState::Editor;
                }
                if ui.button("Cancel").clicked() {
                    self.main_window = MainWindowState::Editor;
                }
            });
            ui.label(format!(
                "Selected Resources: {}",
                self.selected_resources.filter_subtext
            ));
            jpac.resource_selector(&mut self.selected_resources, ui);
        } else {
            self.main_window = MainWindowState::ErrorMessage(
                "To Show the Exporter, please open a resource file".to_string(),
            );
        }
    }

    fn show_viewer(&mut self, ui: &mut Ui) {
        todo!("Implement Particle Viewing")
    }
}

impl MyApp {
    fn show_ribbon(&mut self, ctx: &egui::Context) {
        egui::TopBottomPanel::top("top_ribbon")
            .resizable(false)
            .show(ctx, |ui| self.menu_bar(ui, ctx));
    }

    fn show_tree_view(&mut self, ctx: &egui::Context) {
        egui::SidePanel::left("side_panel")
            .resizable(true)
            .default_width(250.0)
            .show(ctx, |ui| {
                if let Some(jpac) = &mut self.jpac {
                    jpac.show_tree_ui(&mut self.selected, &mut self.tree_filter, ui);
                } else {
                    ui.label("Pick a File First");
                }
            });
    }
}

impl eframe::App for MyApp {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        self.show_ribbon(ctx);
        self.show_tree_view(ctx);
        self.main_window(ctx);
    }
}

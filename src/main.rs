#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]
#![feature(slice_as_chunks)]

use std::{
    fs::File,
    io::{Cursor, Read},
};

use crate::jpa::JPAC;
use binrw::{BinRead, BinWrite};
use eframe::egui;
use egui::{menu, Ui};
use jpa::ResouceSelector;


mod jpa;
pub mod ui_helpers;
pub mod bti;

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

#[derive(Debug, Default, PartialEq)]
enum FilterType {
    #[default]
    None,
    Texture,
    Resource,
    Name,
}

#[derive(Debug, Default)]
struct FilterSettings {
    text: String,
    filter_type: FilterType,
}

#[derive(Default)]
struct MyApp {
    picked_path: Option<String>,
    jpac:        Option<JPAC>,
    edit:        jpa::Selected,
    filter:      FilterSettings,
    selected_resources: ResouceSelector,
    show_exporter: bool,
}

impl MyApp {
    fn show_menu(&mut self, ui: &mut Ui) {
        menu::bar(ui, |ui| {
            ui.menu_button("File", |ui| {
                if ui.button("Open JPC").clicked() {
                    if let Some(path) = rfd::FileDialog::new()
                    .add_filter("JPC", &["jpc"])
                    .pick_file()
                    {
                        self.picked_path = Some(path.display().to_string());
                        self.jpac = None;
                    }
                    ui.close_menu();
                }
                if ui.button("Open Alias").clicked() {if let Some(path) = &rfd::FileDialog::new()
                    .set_file_name("resource_alias.json")
                    .add_filter("Resource Alias", &["json"])
                    .pick_file()
                    {
                        let mut f = File::open(path).expect("Unable to open file");
                        if let Some(jpac) = &mut self.jpac {
                            jpac.apply_alias(&serde_json::from_reader(&mut f).unwrap());
                        }
                    }
                    ui.close_menu();
                }
                if ui.button("Save Alias").clicked() {
                    if let Some(path) = &rfd::FileDialog::new()
                    .set_file_name("resource_alias.json")
                    .add_filter("Resource Alias", &["json"])
                    .save_file()
                    {
                        let mut f = File::create(path).expect("Unable to open/create file");
                        if let Some(jpac) = &self.jpac {
                            serde_json::to_writer_pretty(&mut f, &jpac.export_alias()).unwrap();
                        }
                    }
                    ui.close_menu();
                }
                if ui.button("Import Resources").clicked() {
                    if let Some(path) = rfd::FileDialog::new()
                        .add_filter("JPA Resource", &["jpares"])
                        .pick_file()
                    {
                        let path = path.display().to_string();
                        let mut f = File::open(&path)
                            .expect("File Could not be opened");
                        if let Some(jpac) = &mut self.jpac {
                            print!("{:?}", jpac.import_resources(&mut f));
                        }
                    }
                    ui.close_menu();
                }
                if ui.button("Export Resources").clicked() {
                    self.show_exporter = true;
                }
                if ui.button("Save").clicked() {
                    if let Some(path) = &rfd::FileDialog::new()
                        .set_file_name("new_jpc.jpc")
                        .add_filter("JPC", &["jpc"])
                        .save_file()
                    {
                        let mut f = File::create(path).expect("File Could not be opened");
                        self.jpac.write(&mut f).expect("File could not be read");
                    }
                    ui.close_menu();
                }
            });
        });
    }
    fn show_ribbon(&mut self, ctx: &egui::Context) {
        egui::TopBottomPanel::top("top_ribbon")
            .resizable(false)
            .show(ctx, |ui| {
                self.show_menu(ui);
            });
    }

    fn show_tree_view(&mut self, ctx: &egui::Context) {
        egui::SidePanel::left("side_panel")
            .resizable(true)
            .default_width(250.0)
            .show(ctx, |ui| {
                if let Some(jpac) = &mut self.jpac {
                    jpac.show_tree_ui(&mut self.edit, &mut self.filter, ui);
                } else {
                    ui.label("Pick a File First");
                }
            });
    }

    fn show_edit_view(&mut self, ctx: &egui::Context) {
        egui::CentralPanel::default().show(ctx, |ui| {
            // The Main Edit Panel
             if let Some(jpac) = &mut self.jpac {
                if self.show_exporter {
                    ui.horizontal(|ui| {
                        if ui.button("Export").clicked() {
                            if let Some(path) = rfd::FileDialog::new()
                                .add_filter("JPA Resource", &["jpares"])
                                .save_file()
                            {
                                let path = path.display().to_string();
                                let mut f = File::create(&path)
                                    .expect("File Could not be opened");
                                let _ = jpac.export_resources(&self.selected_resources.selected, &mut f);
                            }
                            self.show_exporter = false;
                        }
                        if ui.button("Cancel").clicked() {
                            self.show_exporter = false;
                        }
                    });
                    ui.label(format!("Selected Resources: {}", self.selected_resources.filter_subtext));
                    jpac.resource_selector(&mut self.selected_resources, ui);
                } else {
                    jpac.show_editor(self.edit, ui);
                }
            } 
            // There isnt anything to edit if there isnt a file
            else {
                ui.label("Welcome to the JPAC Editor! \n To get started, go to the `File` tab and select the .jpc file. \n Note: Currently the program only support JPAC2-11 (Version for Skyward Sword)");
                if let Some(picked_path) = &self.picked_path {
                    let mut file_contents: Vec<u8> = Vec::new();
                    File::open(&picked_path)
                        .expect("File Could not be opened")
                        .read_to_end(&mut file_contents)
                        .expect("Could not Read File Contents");
                    match JPAC::read(&mut Cursor::new(file_contents)) {
                        Ok(mut jpac) => {
                            jpac.load_textures(ctx);
                            self.jpac = Some(jpac);
                        },
                        Err(msg) => {
                            println!("{msg}");
                            self.picked_path = None;
                        },
                    }
                }
            }
        });
    }
}

impl eframe::App for MyApp {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        self.show_ribbon(ctx);
        self.show_tree_view(ctx);
        self.show_edit_view(ctx);
    }
}

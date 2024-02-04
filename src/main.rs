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
use jpa::jpac2_11::resource::JPAResource;

mod jpa;
mod ui_helpers;

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

#[derive(Debug, Default)]
struct MyApp {
    picked_path: Option<String>,
    jpac:        Option<JPAC>,
    edit:        jpa::Selected,
    filter:      FilterSettings,
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
                // TODO: Make this version dependent
                if ui.button("Import Resource").clicked() {
                    if let Some(path) = rfd::FileDialog::new()
                        .add_filter("JPC2-11 Resource", &["jpc2-11"])
                        .pick_file()
                    {
                        let path = path.display().to_string();
                        let mut file_contents: Vec<u8> = Vec::new();
                        File::open(&path)
                            .expect("File Could not be opened")
                            .read_to_end(&mut file_contents)
                            .expect("Could not Read File Contents");
                        match JPAResource::read(&mut Cursor::new(file_contents)) {
                            Ok(in_res) => {
                                if let Some(JPAC::JPAC2_11(jpac)) = &mut self.jpac {
                                    if let Some(res) = jpac.resources.iter_mut().find(|res| res.res_id == in_res.res_id) {
                                        *res = in_res;
                                    }
                                }
                            },
                            Err(msg) => {
                                println!("{msg}");
                            },
                        }
                    }
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
                ctx.settings_ui(ui);
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
                jpac.show_editor(self.edit, ui);
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

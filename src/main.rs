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

mod jpa;

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

#[derive(Debug, Default)]
struct MyApp {
    picked_path: Option<String>,
    jpac:        Option<JPAC>,
    edit:        jpa::Selected,
    filter_text: String,
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
                if ui.button("Open JPN").clicked() {
                    ui.close_menu();
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
        if egui::SidePanel::left("side_panel")
            .resizable(true)
            .default_width(250.0)
            .show(ctx, |ui| {
                if let Some(jpac) = &mut self.jpac {
                    jpac.show_tree_ui(&mut self.edit, &mut self.filter_text, ui);
                } else {
                    ui.label("Pick a File First");
                }
            })
            .response
            .hovered()
        {
            let down_pressed = ctx.input(|i| i.key_pressed(egui::Key::ArrowDown));
            let up_pressed = ctx.input(|i| i.key_pressed(egui::Key::ArrowUp));
            match self.edit {
                jpa::Selected::Resource(idx) => {
                    if up_pressed && idx != 0 {
                        self.edit = jpa::Selected::Resource(idx - 1);
                    } else if down_pressed {
                        if let Some(jpac) = &self.jpac {
                            if idx < jpac.get_num_resources() {
                                self.edit = jpa::Selected::Resource(idx + 1);
                            }
                        }
                    }
                },
                jpa::Selected::Texture(idx) => {
                    if up_pressed && idx != 0 {
                        self.edit = jpa::Selected::Texture(idx - 1);
                    } else if down_pressed {
                        if let Some(jpac) = &self.jpac {
                            if idx < jpac.get_num_textures() {
                                self.edit = jpa::Selected::Texture(idx + 1);
                            }
                        }
                    }
                },
                jpa::Selected::None => {},
            };
        }
    }

    fn show_edit_view(&mut self, ctx: &egui::Context) {
        egui::CentralPanel::default().show(ctx, |ui| {
            if let Some(jpac) = &mut self.jpac {
                jpac.show_editor(self.edit, ui);
            } else {
                if let Some(picked_path) = &self.picked_path {
                    let mut file_contents: Vec<u8> = Vec::new();
                    File::open(&picked_path)
                        .expect("File Could not be opened")
                        .read_to_end(&mut file_contents)
                        .expect("Could not Read File Contents");
                    match JPAC::read(&mut Cursor::new(file_contents)) {
                        Ok(mut jpac) => {
                            jpac.correct_parsing();
                            self.jpac = Some(jpac);
                        },
                        Err(msg) => {
                            ui.label("The file is either an incorrect Version or incorrect format");
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

use binrw::{binrw, BinWrite};
use egui::{CollapsingHeader, Image, Response, Ui, Vec2};
use std::{
    fs::File,
    io::{self},
};

pub mod bsp;
pub mod etx;
pub mod resource;
use crate::jpa::tex::JPATexture;
use resource::JPAResource;

use crate::{FilterSettings, FilterType};

use super::Selected;

#[binrw]
#[brw(big, stream = s)]
pub struct JPAC {
    #[br(temp)]
    #[bw(calc = resources.len() as u16)]
    pub resource_count:   u16,
    #[br(temp)]
    #[bw(calc = textures.len() as u16)]
    pub texture_count:    u16,
    #[br(temp)]
    #[bw(calc = 0)] // Will return to write after
    pub texture_offset: u32,
    #[br(count = resource_count)]
    #[bw(align_after = 0x20)]
    pub resources:        Vec<JPAResource>,
    #[br(ignore)]
    #[bw(restore_position, calc = {let t = s.stream_position()? as u32; s.seek(io::SeekFrom::Start(0xC))?; t})]
    tex_offset_corrected: u32,
    #[br(seek_before = io::SeekFrom::Start(texture_offset as _))]
    #[br(count = texture_count)]
    pub textures:         Vec<JPATexture>,

    #[brw(ignore)]
    res_filter_val: Option<String>,
    #[brw(ignore)]
    res_filter_idx: Vec<usize>,
    #[brw(ignore)]
    tex_filter_val: Option<String>,
    #[brw(ignore)]
    tex_filter_idx: Vec<(usize, String)>,
}

impl JPAC {
    pub fn show_tree_ui(
        &mut self,
        selected: &mut Selected,
        filter: &mut FilterSettings,
        force_filter: bool,
        ui: &mut Ui,
    ) {
        ui.label("Version: JPAC2_11");
        let mut res_responses: Vec<Response> = vec![];
        let mut tex_responses: Vec<Response> = vec![];
        CollapsingHeader::new("Resources")
            .default_open(false)
            .show(ui, |ui| {
                let filter_val = &mut self.res_filter_val;
                let filter_idx = &mut self.res_filter_idx;
                // filter condition
                let change_filter = if (*filter_val).is_none() {
                    *filter_val = Some(filter.text.clone());
                    true
                } else if let Some(prev_fitler) = filter_val {
                    *prev_fitler != filter.text
                } else {
                    force_filter
                };

                // Change based on filter
                if change_filter {
                    let filter_conds: Vec<&str> =
                        filter.text.split(",").map(|s| s.trim()).collect();
                    filter_idx.clear();
                    *filter_idx = self
                        .resources
                        .iter()
                        .enumerate()
                        .filter_map(|(idx, res)| -> Option<usize> {
                            // If no filters
                            if filter_conds.is_empty() || filter.text.is_empty() {
                                return Some(idx);
                            }
                            // Custom status per filter type
                            match filter.filter_type {
                                // Remove all filtered Values
                                FilterType::None => Some(idx),
                                // Scan the Texture lookup and match it to desired texture names
                                FilterType::Texture => {
                                    let names =
                                        res.get_textures().get_texture_names(&self.textures);
                                    filter_conds.iter().find_map(|str| -> Option<usize> {
                                        names
                                            .iter()
                                            .find_map(|name| name.contains(str).then(|| idx))
                                    })
                                },
                                // Matching resource ids
                                FilterType::Resource => {
                                    filter_conds.iter().find_map(|str| -> Option<usize> {
                                        str.parse::<u16>()
                                            .is_ok_and(|id| res.res_id == id)
                                            .then(|| idx)
                                    })
                                },
                                // Matching Resource Names
                                FilterType::Name => {
                                    filter_conds
                                        .iter()
                                        .find_map(|str| res.alias.contains(str).then(|| idx))
                                },
                            }
                        })
                        .collect();
                }

                // List all resources
                for &idx in filter_idx.iter() {
                    let checked = if let Selected::Resource(sel) = *selected {
                        sel == idx
                    } else {
                        false
                    };
                    let response = ui.selectable_label(checked, &self.resources[idx].alias);
                    if response.clicked() {
                        *selected = Selected::Resource(idx);
                    }
                    res_responses.push(response);
                }
            });
        CollapsingHeader::new("Textures")
            .default_open(false)
            .show(ui, |ui| {
                let filter_val = &mut self.tex_filter_val;
                let filter_idx = &mut self.tex_filter_idx;
                // filter condition
                let change_filter = if (*filter_val).is_none() {
                    *filter_val = Some(filter.text.clone());
                    true
                } else if let Some(prev_fitler) = filter_val {
                    *prev_fitler != filter.text
                } else {
                    false
                };

                if change_filter {
                    filter_idx.clear();
                    *filter_idx = self
                        .textures
                        .iter()
                        .enumerate()
                        .filter_map(|(idx, tex)| {
                            let name = std::str::from_utf8(&tex.name).unwrap();
                            match filter.filter_type {
                                crate::FilterType::Texture => {
                                    for filt in filter
                                        .text
                                        .split(",")
                                        .map(|s| s.trim())
                                        .collect::<Vec<&str>>()
                                    {
                                        if name.contains(filt) {
                                            return Some((idx, String::from(name)));
                                        }
                                    }
                                },
                                _ => {
                                    return Some((idx, String::from(name)));
                                },
                            }
                            None
                        })
                        .collect();
                }

                // List all Textures
                for (idx, name) in filter_idx.iter() {
                    let checked = if let Selected::Texture(sel) = *selected {
                        sel == *idx
                    } else {
                        false
                    };
                    let response = ui.selectable_label(checked, name);
                    if response.clicked() {
                        *selected = Selected::Texture(*idx);
                    }
                    tex_responses.push(response);
                }
            });
        if ui.input(|i| i.key_pressed(egui::Key::ArrowUp)) {
            *selected = match *selected {
                Selected::Resource(idx) => {
                    let filter_idx = &self.res_filter_idx;
                    let idx =
                        if let Some(i) = filter_idx.iter().enumerate().find(|(_, &e)| e == idx) {
                            i.0
                        } else {
                            0
                        };
                    let idx = idx.wrapping_sub(1).clamp(0, filter_idx.len() - 1);
                    res_responses[idx].scroll_to_me(None);
                    Selected::Resource(filter_idx[idx])
                },
                Selected::Texture(idx) => {
                    let filter_idx = &self.tex_filter_idx;
                    let idx =
                        if let Some(i) = filter_idx.iter().enumerate().find(|(_, e)| e.0 == idx) {
                            i.0
                        } else {
                            0
                        };
                    let idx = idx.wrapping_sub(1).clamp(0, filter_idx.len() - 1);
                    tex_responses[idx].scroll_to_me(None);
                    Selected::Texture(filter_idx[idx].0)
                },
                Selected::None => *selected,
            };
        } else if ui.input(|i| i.key_pressed(egui::Key::ArrowDown)) {
            *selected = match *selected {
                Selected::Resource(idx) => {
                    let filter_idx = &self.res_filter_idx;
                    let idx =
                        if let Some(i) = filter_idx.iter().enumerate().find(|(_, &e)| e == idx) {
                            i.0
                        } else {
                            0
                        };
                    let mut idx = (idx + 1).clamp(0, filter_idx.len());
                    if idx == filter_idx.len() {
                        idx = 0;
                    }
                    res_responses[idx].scroll_to_me(None);
                    Selected::Resource(filter_idx[idx])
                },
                Selected::Texture(idx) => {
                    let filter_idx = &self.tex_filter_idx;
                    let idx =
                        if let Some(i) = filter_idx.iter().enumerate().find(|(_, e)| e.0 == idx) {
                            i.0
                        } else {
                            0
                        };
                    let mut idx = (idx + 1).clamp(0, filter_idx.len());
                    if idx == filter_idx.len() {
                        idx = 0;
                    }
                    tex_responses[idx].scroll_to_me(None);
                    Selected::Texture(filter_idx[idx].0)
                },
                Selected::None => *selected,
            };
        }
    }
    pub fn show_edit_ui(&mut self, selected: Selected, ui: &mut Ui) {
        match selected {
            Selected::Resource(idx) => {
                if let Some(resource) = self.resources.get_mut(idx) {
                    ui.horizontal(|ui| {
                        if ui.button("Export Resource").clicked() {
                            if let Some(path) = rfd::FileDialog::new()
                                .add_filter("JPC2-11", &["jpc2-11"])
                                .set_file_name(format!("{}", resource.alias))
                                .save_file()
                            {
                                let path = path.display().to_string();
                                let mut f = File::create(&path).expect("File Could not be opened");
                                resource
                                    .write(&mut f)
                                    .expect("Could not write Resource to file");
                            }
                        }
                    });
                    resource.show_editor(&self.textures, ui);
                } else {
                    ui.label(format!("The Resource at index {idx} does not exist."));
                }
            },
            Selected::Texture(idx) => {
                if let Some(tex) = self.textures.get_mut(idx) {
                    if let Some(texture) = tex.get_handle() {
                        ui.add(Image::from_texture(texture).shrink_to_fit().max_size(Vec2 {
                            x: 480f32,
                            y: 480f32,
                        }));
                    }
                } else {
                    ui.label(format!("The Texture at index {idx} does not exist"));
                }
            },
            Selected::None => {
                ui.label("To get started:\n\tSelect a Resource or Texture from the left side");
            },
        }
    }
}

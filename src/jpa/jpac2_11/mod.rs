use binrw::binrw;
use egui::{Align, CollapsingHeader, Response, Ui};
use std::{fmt::Debug, io, num, process::id};

pub mod bem;
pub mod bsp;
pub mod esp;
pub mod etx;
pub mod fld;
pub mod kfa;
pub mod resource;
pub mod ssp;
pub mod tdb;
pub mod tex;

use resource::JPAResource;
use tex::JPATexture;

use crate::FilterSettings;

use self::resource::DataBlock;

use super::Selected;

#[binrw]
#[brw(big, stream = s)]
#[derive(Debug)]
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
}

static mut RES_FILTER_VAL: Option<String> = None;
static mut RES_FILTER_IDX: Vec<(usize, String)> = Vec::new();

static mut TEX_FILTER_VAL: Option<String> = None;
static mut TEX_FILTER_IDX: Vec<(usize, String)> = Vec::new();

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
                let filter_val = unsafe { &mut RES_FILTER_VAL };
                let filter_idx = unsafe { &mut RES_FILTER_IDX };
                // filter condition
                let change_filter = if (*filter_val).is_none() {
                    *filter_val = Some(filter.filter_text.clone());
                    true
                } else if let Some(prev_fitler) = filter_val {
                    *prev_fitler != filter.filter_text
                } else {
                    force_filter
                };

                // Change based on filter
                if change_filter {
                    let filter_conds: Vec<&str> =
                        filter.filter_text.split(",").map(|s| s.trim()).collect();
                    filter_idx.clear();
                    *filter_idx = self
                        .resources
                        .iter()
                        .enumerate()
                        .filter_map(|(idx, res)| {
                            let name = format!("Resource {}", res.res_id);
                            for filt in filter_conds.iter() {
                                match filter.fitler_type {
                                    crate::FilterType::Texture => {
                                        // Grab Tex
                                        let textures = res
                                            .blocks
                                            .iter()
                                            .find(|&blk| {
                                                match blk {
                                                    DataBlock::TextureDataBase(_) => true,
                                                    _ => false,
                                                }
                                            })
                                            .and_then(|tdb| {
                                                if let DataBlock::TextureDataBase(tdb) = tdb {
                                                    Some(&tdb.textures)
                                                } else {
                                                    None
                                                }
                                            })
                                            .expect("Could not Find Texture Block");
                                        // Search for it
                                        for tex_idx in textures {
                                            if std::str::from_utf8(
                                                &self.textures[*tex_idx as usize].name,
                                            )
                                            .unwrap()
                                            .contains(filt)
                                            {
                                                return Some((idx, name));
                                            }
                                        }
                                    },
                                    crate::FilterType::Resource => {
                                        match filt.parse::<u16>() {
                                            Ok(id) => {
                                                if id == res.res_id {
                                                    return Some((idx, name));
                                                }
                                            },
                                            Err(_) => {}, // Fail Silently
                                        };
                                    },
                                    _ => return Some((idx, name)),
                                }
                            }
                            None
                        })
                        .collect();
                }

                // List all resources
                for (idx, name) in filter_idx.iter() {
                    let checked = if let Selected::Resource(sel) = *selected {
                        sel == *idx
                    } else {
                        false
                    };
                    let response = ui.selectable_label(checked, name);
                    if response.clicked() {
                        *selected = Selected::Resource(*idx);
                    }
                    res_responses.push(response);
                }
            });
        CollapsingHeader::new("Textures")
            .default_open(false)
            .show(ui, |ui| {
                let filter_val = unsafe { &mut TEX_FILTER_VAL };
                let filter_idx = unsafe { &mut TEX_FILTER_IDX };
                // filter condition
                let change_filter = if (*filter_val).is_none() {
                    *filter_val = Some(filter.filter_text.clone());
                    true
                } else if let Some(prev_fitler) = filter_val {
                    *prev_fitler != filter.filter_text
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
                            match filter.fitler_type {
                                crate::FilterType::Texture => {
                                    for filt in filter
                                        .filter_text
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
                    let filter_idx = unsafe { &mut RES_FILTER_IDX };
                    let idx =
                        if let Some(i) = filter_idx.iter().enumerate().find(|(_, e)| e.0 == idx) {
                            i.0
                        } else {
                            0
                        };
                    let idx = idx.wrapping_sub(1).clamp(0, filter_idx.len() - 1);
                    res_responses[idx].scroll_to_me(None);
                    Selected::Resource(filter_idx[idx].0)
                },
                Selected::Texture(idx) => {
                    let filter_idx = unsafe { &mut TEX_FILTER_IDX };
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
                    let filter_idx = unsafe { &mut RES_FILTER_IDX };
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
                    res_responses[idx].scroll_to_me(None);
                    Selected::Resource(filter_idx[idx].0)
                },
                Selected::Texture(idx) => {
                    let filter_idx = unsafe { &mut TEX_FILTER_IDX };
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
                let res = self.resources.get_mut(idx).unwrap();
                let mut num_kfa = 0;
                let mut num_fld = 0;
                for block in &mut res.blocks {
                    match block {
                        DataBlock::Dynamics(bem) => bem.show_editor(ui),
                        DataBlock::BaseShape(bsp) => bsp.show_editor(ui),
                        DataBlock::ExTex(etx) => etx.show_editor(ui),
                        DataBlock::ExtraShape(esp) => esp.show_editor(ui),
                        DataBlock::ChildShape(ssp) => ssp.show_editor(ui),
                        DataBlock::TextureDataBase(tdb) => {
                            tdb.show_editor(&mut res.tdb_count, &self.textures, ui)
                        },
                        DataBlock::Field(fld) => {
                            fld.show_editor(num_fld, ui);
                            num_fld += 1;
                        },
                        DataBlock::KeyFrame(kfa) => {
                            kfa.show_editor(num_kfa, ui);
                            num_kfa += 1;
                        },
                    }
                }
                // ui.label(format!("{:#?}", &res));
            },
            Selected::Texture(idx) => {
                if let Some(tex) = self.textures.get_mut(idx) {
                    ui.label(format!("{:#?}", tex));
                } else {
                    ui.label("Somehow an out of bounds texture was selected");
                }
            },
            Selected::None => {
                ui.label("Select a Resource or Texture from the left side");
            },
        }
    }

    pub fn correct_parsing(&mut self) {
        for res in &mut self.resources {
            for block in &mut res.blocks {
                match block {
                    // Couldnt figure out arg parsing, but this should work
                    DataBlock::TextureDataBase(tdb) => tdb.textures.truncate(res.tdb_count.into()),
                    _ => {},
                }
            }
        }
    }
}

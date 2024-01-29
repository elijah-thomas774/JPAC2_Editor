use binrw::binrw;
use egui::{CollapsingHeader, Ui};
use std::{fmt::Debug, io};

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

impl JPAC {
    pub fn show_tree_ui(&mut self, selected: &mut Selected, filter: &mut String, ui: &mut Ui) {
        ui.label("Version: JPAC2_11");
        CollapsingHeader::new("Resources")
            .default_open(false)
            .show(ui, |ui| {
                // List all resources
                for n in 0..self.resources.len() {
                    let checked: bool = match *selected {
                        Selected::Resource(idx) => n == idx,
                        _ => false,
                    };
                    // Show the label "Resource #"
                    let name = format!("Resource {}", self.resources[n].res_id);
                    if name.contains(filter.as_str()) {
                        if ui.selectable_label(checked, name).clicked() {
                            *selected = Selected::Resource(n);
                        }
                    }
                }
            });
        CollapsingHeader::new("Textures")
            .default_open(false)
            .show(ui, |ui| {
                // List all Textures
                for n in 0..self.textures.len() {
                    let checked: bool = match *selected {
                        Selected::Texture(idx) => n == idx,
                        _ => false,
                    };
                    // Show the Texture name
                    let name = std::str::from_utf8(&self.textures[n].name)
                        .expect("Could not read texture name")
                        .trim();
                    if name.contains(filter.as_str()) {
                        if ui.selectable_label(checked, name).clicked() {
                            *selected = Selected::Texture(n);
                        }
                    }
                }
            });
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

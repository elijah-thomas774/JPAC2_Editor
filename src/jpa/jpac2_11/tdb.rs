use binrw::binrw;
use egui::{Image, Ui, Vec2};
use std::fmt::Debug;

use super::tex::JPATexture;

#[binrw]
#[brw(big)]
#[br(import(tdb_count : u8))]
pub struct TDB1 {
    #[br(temp)]
    #[bw(calc = 8 + (((textures.len()*2) + 3)&!3) as u32)]
    _size:        u32,
    #[br(count = tdb_count)]
    #[brw(align_after = 4)]
    pub textures: Vec<u16>,
}

impl Debug for TDB1 {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("TDB1")
            .field("textures", &format!("{:04X?}", &self.textures))
            .finish()
    }
}

impl TDB1 {
    /// Shows the Editor for the Texture Database block
    pub fn show_editor(&mut self, textures: &Vec<JPATexture>, ui: &mut Ui) {
        for (idx, &tex_idx) in self.textures.iter().enumerate() {
            match textures.get(tex_idx as usize) {
                Some(tex) => {
                    ui.label(format!(
                        "{}: {}",
                        idx,
                        std::str::from_utf8(&tex.name).unwrap().trim_matches('\0')
                    ))
                    .on_hover_ui(|ui| {
                        if let Some(texture) = &tex.texture {
                            ui.add(
                                Image::from_texture(texture)
                                    .max_size(Vec2 { x: 200.0, y: 200.0 })
                                    .shrink_to_fit(),
                            );
                        }
                    });
                },
                None => {
                    ui.label(format!("{}: Invalid {}", idx, tex_idx));
                },
            }
        }
    }

    // Grabs all the texture names from a list of indecies
    #[allow(dead_code)]
    pub fn get_texture_names(&self, textures: &Vec<JPATexture>) -> Vec<String> {
        self.textures
            .iter()
            .map(|&tex_idx| {
                String::from_utf8(
                    textures
                        .get(tex_idx as usize)
                        .expect(
                            format!("ERROR: Texture at index [{tex_idx}] does not exist").as_str(),
                        )
                        .name
                        .to_vec(),
                )
                .expect(format!("Texture at {tex_idx} had invalid name!!").as_str())
            })
            .collect()
    }
}

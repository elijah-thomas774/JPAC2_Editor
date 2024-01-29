use binrw::binrw;
use egui::{CollapsingHeader, Ui};
use std::fmt::Debug;

use super::tex::JPATexture;

#[binrw]
#[brw(big)]
pub struct TDB1 {
    #[br(temp)]
    #[bw(calc = 8 + (((textures.len()*2) + 3)&!3) as u32)]
    size:         u32,
    #[br(count = (size-8)/2)]
    #[bw(align_after = 4)]
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
    pub fn show_editor(&mut self, num_textures: &mut u8, textures: &Vec<JPATexture>, ui: &mut Ui) {
        CollapsingHeader::new("Textures").show(ui, |ui| {
            for n in 0..*num_textures as usize {
                match textures.get(self.textures[n] as usize) {
                    Some(tex) => {
                        ui.label(format!(
                            "{}: {}",
                            n,
                            std::str::from_utf8(&tex.name).unwrap().trim_matches('\0')
                        ));
                    },
                    None => {
                        ui.label(format!("{}: Invalid", n));
                    },
                }
            }
        });
    }
}

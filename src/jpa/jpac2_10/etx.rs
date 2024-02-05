use binrw::binrw;
use egui::{RichText, Ui};
use std::fmt::Debug;

#[binrw]
#[brw(big)]
#[derive(Debug)]
pub struct ETX1 {
    #[br(temp, assert(size == 0x28))]
    #[bw(calc = 0x28)]
    size:             u32, // 0x04
    pub flags:        u32,      // 0x08
    pub values:       [f32; 6], // 0x0C
    pub scale:        i8,       // 0x24
    pub ind_tex_id:   u8,       // 0x25
    #[bw(if(flags & 0x100 != 0, 0))]
    #[brw(pad_after(1))]
    pub ind_tex_id_2: u8, // 0x26
}

impl ETX1 {
    pub fn show_editor(&mut self, ui: &mut Ui) {
        ui.label(RichText::new(format!("Flags: {:08X}", self.flags)).monospace());
        ui.label(format!(
            "Values:\n\t{:.3?},\n\t{:.3?}",
            &self.values[0..3],
            &self.values[4..6],
        ));
        ui.label(format!("Scale: {}", self.scale));
        ui.label(format!("Indirect Texture Index: {}", self.ind_tex_id));
        ui.label(format!("Indirect Texture Index 2: {}", self.ind_tex_id_2));
    }
}

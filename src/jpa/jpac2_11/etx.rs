use binrw::binrw;
use egui::{ RichText, Ui};
use std::fmt::Debug;

#[binrw]
#[brw(big)]
#[derive(Debug)]
pub struct ETX1 {
    #[br(temp, assert(size == 0x50))]
    #[bw(calc = 0x50)]
    size:             u32, // 0x04
    pub flags:        u32,       // 0x08
    pub values:       [f32; 16], // 0x0C
    pub unk:          u8,        // 0x4C
    pub scale:        i8,        // 0x4D
    pub ind_tex_id:   u8,        // 0x4E
    #[bw(if(flags & 0x100 != 0, 0))]
    pub ind_tex_id_2: u8, // 0x4F
}

impl ETX1 {
    pub fn show_editor(&mut self, ui: &mut Ui) {
        ui.label(RichText::new(format!("Flags: {:08X}", self.flags)).monospace());
        ui.label(format!(
            "Values:\n\t{:.3?},\n\t{:.3?},\n\t{:.3?},\n\t{:.3?}",
            &self.values[0..4],
            &self.values[4..8],
            &self.values[8..12],
            &self.values[12..16]
        ));
        ui.label(format!("Unk: {:#02X}", self.unk));
        ui.label(format!("Scale: {}", self.scale));
        ui.label(format!("Indirect Texture Index: {}", self.ind_tex_id));
        ui.label(format!("Indirect Texture Index 2: {}", self.ind_tex_id_2));
    }
}

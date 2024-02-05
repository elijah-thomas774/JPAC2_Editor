use binrw::binrw;
use egui::{Checkbox, ComboBox, Ui};
use std::fmt::Debug;

#[binrw]
#[brw(big, repr = u8)]
#[derive(Debug, PartialEq)]
pub enum JPAKeyType {
    None           = -1,
    Rate           = 0x00,
    VolumeSize     = 0x01,
    VolumeSweep    = 0x02,
    VolumeMinRad   = 0x03,
    LifeTime       = 0x04,
    Moment         = 0x05,
    InitialVelOmni = 0x06,
    InitialVelAxis = 0x07,
    InitialVelDir  = 0x08,
    Spread         = 0x09,
    Scale          = 0x0A,
}

#[binrw]
#[brw(big)]
#[derive(Debug)]
pub struct KFA1 {
    #[br(temp)]
    #[bw(calc = (0xC + ((key_values.len()*4)+3)&!3) as u32)]
    _size:              u32,
    pub key_type:       JPAKeyType,
    #[br(temp)]
    #[bw(calc = (key_values.len()/4) as u8)]
    pub key_count:      u8,
    pub unk_a:          u8,
    #[br(temp)]
    #[bw(calc = if *is_loop_enable {1u8} else {0u8})]
    temp_en:            u8,
    #[br(calc = temp_en != 0)]
    #[bw(ignore)]
    pub is_loop_enable: bool,
    #[br(count = key_count*4)]
    pub key_values:     Vec<f32>,
}

impl KFA1 {
    pub fn show_editor(&mut self, ui: &mut Ui) {
        ui.horizontal(|ui| {
            ui.label("Key Type:");
            ComboBox::from_id_source(std::ptr::addr_of!(self))
                .selected_text(format!("{:?}", self.key_type))
                .show_ui(ui, |ui| {
                    ui.selectable_value(&mut self.key_type, JPAKeyType::None, "None");
                    ui.selectable_value(&mut self.key_type, JPAKeyType::Rate, "Rate");
                    ui.selectable_value(&mut self.key_type, JPAKeyType::VolumeSize, "VolumeSize");
                    ui.selectable_value(&mut self.key_type, JPAKeyType::VolumeSweep, "VolumeSweep");
                    ui.selectable_value(
                        &mut self.key_type,
                        JPAKeyType::VolumeMinRad,
                        "VolumeMinRad",
                    );
                    ui.selectable_value(&mut self.key_type, JPAKeyType::LifeTime, "LifeTime");
                    ui.selectable_value(&mut self.key_type, JPAKeyType::Moment, "Moment");
                    ui.selectable_value(
                        &mut self.key_type,
                        JPAKeyType::InitialVelOmni,
                        "InitialVelOmni",
                    );
                    ui.selectable_value(
                        &mut self.key_type,
                        JPAKeyType::InitialVelAxis,
                        "InitialVelAxis",
                    );
                    ui.selectable_value(
                        &mut self.key_type,
                        JPAKeyType::InitialVelDir,
                        "InitialVelDir",
                    );
                    ui.selectable_value(&mut self.key_type, JPAKeyType::Spread, "Spread");
                    ui.selectable_value(&mut self.key_type, JPAKeyType::Scale, "Scale");
                });
        });
        ui.label(format!("Unknown 0x0A: {:#02X}", self.unk_a));
        ui.horizontal(|ui| {
            ui.label("Loop: ");
            ui.add(Checkbox::without_text(&mut self.is_loop_enable));
        });
        let chunks = self.key_values.as_chunks::<4>().0;
        ui.label("Key Values: ");
        for chunk in chunks {
            ui.label(format!("\t{:.3?},", chunk));
        }
    }
}

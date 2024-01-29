use binrw::binrw;
use egui::{CollapsingHeader, Color32, DragValue, RichText, Ui};
use std::fmt::Debug;

#[binrw]
#[brw(big)]
#[derive(Debug)]
pub struct SSP1 {
    #[br(temp, assert(size==0x48))]
    #[bw(calc = 0x48)]
    size:                     u32, // 0x04
    pub flags:                u32,      // 0x08
    pub position_random:      f32,      // 0x0C
    pub base_velocity:        f32,      // 0x10
    pub base_velocity_random: f32,      // 0x14
    pub velocity_inf_rate:    f32,      // 0x18
    pub gravity:              f32,      // 0x1C
    pub scale_2d:             [f32; 2], // 0x20
    pub inherit_scale:        f32,      // 0x28
    pub inherit_alpha:        f32,      // 0x2C
    pub inherit_rgb:          f32,      // 0x30
    #[bw(calc = [color_prm.r(), color_prm.g(), color_prm.b(), color_prm.a()])]
    color_temp:               [u8; 4], // 0x34
    #[bw(ignore)]
    #[br(calc = Color32::from_rgba_premultiplied(color_temp[0], color_temp[1], color_temp[2], color_temp[3]))]
    pub color_prm:            Color32, // 0x34
    #[bw(calc = [color_env.r(), color_env.g(), color_env.b(), color_env.a()])]
    color_temp:               [u8; 4], // 0x38
    #[bw(ignore)]
    #[br(calc = Color32::from_rgba_premultiplied(color_temp[0], color_temp[1], color_temp[2], color_temp[3]))]
    pub color_env:            Color32, // 0x38
    pub timing:               f32,      // 0x3C
    pub life:                 u16,      // 0x40
    pub rate:                 u16,      // 0x42
    pub step:                 u8,       // 0x44
    pub texture_idx:          u8,       // 0x45
    pub rotate_speed:         u16,      // 0x46
}

impl SSP1 {
    pub fn show_editor(&mut self, ui: &mut Ui) {
        CollapsingHeader::new("Child Shape").show(ui, |ui| {
            ui.label(RichText::new(format!("Flags: {:08X}", self.flags)).monospace());
            ui.horizontal(|ui| {
                ui.label("Position Random: ");
                ui.add(DragValue::new(&mut self.position_random).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Base Velocity: ");
                ui.add(DragValue::new(&mut self.base_velocity).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Base Velocity Random: ");
                ui.add(DragValue::new(&mut self.base_velocity_random).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Velocity Inf Rate: ");
                ui.add(DragValue::new(&mut self.velocity_inf_rate).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Global Scale 2D: ");
                ui.add(DragValue::new(&mut self.scale_2d[0]).speed(1).prefix("x: "));
                ui.add(DragValue::new(&mut self.scale_2d[1]).speed(1).prefix("y: "));
            });
            ui.horizontal(|ui| {
                ui.label("Inherit Scale: ");
                ui.add(DragValue::new(&mut self.inherit_scale).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Inherit Alpha: ");
                ui.add(DragValue::new(&mut self.inherit_alpha).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Inherit RGB: ");
                ui.add(DragValue::new(&mut self.inherit_rgb).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label(
                    RichText::new(format!("Color Prm: {}", self.color_prm.to_hex())).monospace(),
                );
                ui.color_edit_button_srgba(&mut self.color_prm);
            });
            ui.horizontal(|ui| {
                ui.label(
                    RichText::new(format!("Color Env: {}", self.color_env.to_hex())).monospace(),
                );
                ui.color_edit_button_srgba(&mut self.color_env);
            });
            ui.horizontal(|ui| {
                ui.label("Timing: ");
                ui.add(DragValue::new(&mut self.timing).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Life: ");
                ui.add(DragValue::new(&mut self.life).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Rate: ");
                ui.add(DragValue::new(&mut self.rate).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Step: ");
                ui.add(DragValue::new(&mut self.step).speed(1));
            });
            ui.label(format!("Texture Indx: {}", self.texture_idx));
            ui.horizontal(|ui| {
                ui.label("Rotate Speed: ");
                ui.add(DragValue::new(&mut self.rotate_speed).speed(1));
            });
        });
    }
}

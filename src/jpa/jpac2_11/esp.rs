use binrw::binrw;
use egui::{CollapsingHeader, DragValue, RichText, Ui};
use std::fmt::Debug;

#[binrw]
#[brw(big)]
#[derive(Debug)]
pub struct ESP1 {
    #[br(temp, assert(size == 0x60))]
    #[bw(calc = 0x60)]
    size:                      u32, // 0x04
    pub flags:                 u32, // 0x08
    pub scale_in_timing:       f32, // 0x0C
    pub scale_out_timing:      f32, // 0x10
    pub scale_in_value_x:      f32, // 0x14
    pub scale_out_value_x:     f32, // 0x18
    pub scale_in_value_y:      f32, // 0x1C
    pub scale_out_value_y:     f32, // 0x20
    pub scale_out_random:      f32, // 0x24
    pub scale_anm_max_frame_x: u16, // 0x28
    pub scale_anm_max_frame_y: u16, // 0x2A
    pub alpha_in_timing:       f32, // 0x2C
    pub alpha_out_timing:      f32, // 0x30
    pub alpha_in_value:        f32, // 0x34
    pub alpha_base_value:      f32, // 0x38
    pub alpha_out_value:       f32, // 0x3C
    pub alpha_wave_random:     f32, // 0x44
    pub alpha_wave_frequency:  f32, // 0x40
    pub alpha_wave_amplitude:  f32, // 0x48
    pub rotate_angle:          f32, // 0x4C
    pub rotate_angle_random:   f32, // 0x50
    pub rotate_speed:          f32, // 0x54
    pub rotate_speed_random:   f32, // 0x58
    pub rotate_direction:      f32, // 0x5C
}

impl ESP1 {
    pub fn show_editor(&mut self, ui: &mut Ui) {
        CollapsingHeader::new("Extra Shape").show(ui, |ui| {
            ui.label(RichText::new(format!("Flags: {:08X}", self.flags)).monospace());
            ui.horizontal(|ui| {
                ui.label("Scale In Timing: ");
                ui.add(DragValue::new(&mut self.scale_in_timing).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Scale Out Timing: ");
                ui.add(DragValue::new(&mut self.scale_out_timing).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Scale In X: ");
                ui.add(DragValue::new(&mut self.scale_in_value_x).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Scale Out X: ");
                ui.add(DragValue::new(&mut self.scale_out_value_x).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Scale In Y: ");
                ui.add(DragValue::new(&mut self.scale_in_value_y).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Scale Out Y ");
                ui.add(DragValue::new(&mut self.scale_out_value_y).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Scale Out Random: ");
                ui.add(DragValue::new(&mut self.scale_out_random).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Scale Animation Frames X: ");
                ui.add(DragValue::new(&mut self.scale_anm_max_frame_x).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Scale Animation Frames Y: ");
                ui.add(DragValue::new(&mut self.scale_anm_max_frame_y).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Alpha In Timing: ");
                ui.add(DragValue::new(&mut self.alpha_in_timing).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Alpha Out Timing: ");
                ui.add(DragValue::new(&mut self.alpha_out_timing).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Alpha In: ");
                ui.add(DragValue::new(&mut self.alpha_in_value).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Alpha Base: ");
                ui.add(DragValue::new(&mut self.alpha_base_value).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Alpha Out: ");
                ui.add(DragValue::new(&mut self.alpha_out_value).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Alpha Wave Random: ");
                ui.add(DragValue::new(&mut self.alpha_wave_random).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Alpha Wave Frequency: ");
                ui.add(DragValue::new(&mut self.alpha_wave_frequency).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Alpha Wave Amplitude: ");
                ui.add(DragValue::new(&mut self.alpha_wave_amplitude).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Rotate Angle: ");
                ui.add(DragValue::new(&mut self.rotate_angle).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Rotate Angle Random: ");
                ui.add(DragValue::new(&mut self.rotate_angle_random).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Rotate Speed: ");
                ui.add(DragValue::new(&mut self.rotate_speed).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Rotate Speed Random: ");
                ui.add(DragValue::new(&mut self.rotate_speed_random).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Rotate Direction: ");
                ui.add(DragValue::new(&mut self.rotate_direction).speed(1));
            });
        });
    }
}

use binrw::binrw;
use egui::{RichText, Ui};
use std::fmt::Debug;

use crate::ui_helpers::num_edit;

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
        ui.label(RichText::new(format!("Flags: {:08X}", self.flags)).monospace());
        num_edit(ui, &mut self.scale_in_timing, "Scale In Timing: ");
        num_edit(ui, &mut self.scale_out_timing, "Scale Out Timing: ");
        num_edit(ui, &mut self.scale_in_value_x, "Scale In X: ");
        num_edit(ui, &mut self.scale_out_value_x, "Scale Out X: ");
        num_edit(ui, &mut self.scale_in_value_y, "Scale In Y: ");
        num_edit(ui, &mut self.scale_out_value_y, "Scale Out Y ");
        num_edit(ui, &mut self.scale_out_random, "Scale Out Random: ");
        num_edit(ui, &mut self.scale_anm_max_frame_x, "Scale Anim Frames X: ");
        num_edit(ui, &mut self.scale_anm_max_frame_y, "Scale Anim Frames Y: ");
        num_edit(ui, &mut self.alpha_in_timing, "Alpha In Timing: ");
        num_edit(ui, &mut self.alpha_out_timing, "Alpha Out Timing: ");
        num_edit(ui, &mut self.alpha_in_value, "Alpha In: ");
        num_edit(ui, &mut self.alpha_base_value, "Alpha Base: ");
        num_edit(ui, &mut self.alpha_out_value, "Alpha Out: ");
        num_edit(ui, &mut self.alpha_wave_random, "Alpha Wave Random: ");
        num_edit(ui, &mut self.alpha_wave_frequency, "Alpha Wave Frequency: ");
        num_edit(ui, &mut self.alpha_wave_amplitude, "Alpha Wave Amplitude: ");
        num_edit(ui, &mut self.rotate_angle, "Rotate Angle: ");
        num_edit(ui, &mut self.rotate_angle_random, "Rotate Angle Random: ");
        num_edit(ui, &mut self.rotate_speed, "Rotate Speed: ");
        num_edit(ui, &mut self.rotate_speed_random, "Rotate Speed Random: ");
        num_edit(ui, &mut self.rotate_direction, "Rotate Direction: ");
    }
}

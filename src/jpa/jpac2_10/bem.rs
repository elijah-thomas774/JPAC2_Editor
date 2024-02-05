use binrw::binrw;
use egui::{RichText, Ui};
use std::fmt::Debug;

use crate::ui_helpers::{num_edit, nums_edit};

#[binrw]
#[brw(big)]
#[derive(Debug)]
pub struct BEM1 {
    #[br(temp, assert(size == 0x7C))]
    #[bw(calc = 0x7C)]
    size:                  u32, // 0x04
    pub emit_flags:        u32,      // 0x08
    pub unk_flags:         u32,      // 0x0C
    pub scale:             [f32; 3], // 0x10
    pub trans:             [f32; 3], // 0x1C
    pub direction:         [f32; 3], // 0x28
    pub initial_vel_omni:  f32,      // 0x34
    pub initial_vel_axis:  f32,      // 0x38
    pub initial_vel_rndm:  f32,      // 0x3C
    pub initial_vel_dir:   f32,      // 0x40
    pub spread:            f32,      // 0x44
    pub initial_vel_ratio: f32,      // 0x48
    pub rate:              f32,      // 0x4C
    pub rate_rndm:         f32,      // 0x50
    pub life_time_rndm:    f32,      // 0x54
    pub volume_sweep:      f32,      // 0x58
    pub volume_min_rad:    f32,      // 0x5C
    pub air_resist:        f32,      // 0x60
    pub moment_rndm:       f32,      // 0x64
    pub emitter_rot:       [i16; 3], // 0x68
    pub max_frame:         i16,      // 0x6E
    pub start_frame:       i16,      // 0x70
    pub life_time:         i16,      // 0x72
    pub volume_size:       i16,      // 0x74
    pub div_number:        i16,      // 0x76
    #[brw(pad_after = 3)]
    pub rate_step:         u8, // 0x78
}

impl BEM1 {
    pub fn show_editor(&mut self, ui: &mut Ui) {
        ui.label(RichText::new(format!("Emitter Flags: {:08X}", self.emit_flags)).monospace()); // TODO(Dropdown of Flags)
        ui.label(RichText::new(format!("Unknown Flags: {:08X}", self.unk_flags)).monospace()); // TODO(Dropdown of Flags)
        nums_edit(ui, &mut self.scale, "Scale: ");
        nums_edit(ui, &mut self.trans, "Translation: ");
        nums_edit(ui, &mut self.direction, "Direction: ");
        num_edit(ui, &mut self.initial_vel_omni, "Velocity Omni: ");
        num_edit(ui, &mut self.initial_vel_axis, "Velocity Axis: ");
        num_edit(ui, &mut self.initial_vel_rndm, "Velocity Random: ");
        num_edit(ui, &mut self.initial_vel_dir, "Velocity Direction: ");
        num_edit(ui, &mut self.spread, "Spread: ");
        num_edit(ui, &mut self.initial_vel_ratio, "Velocity Ratio: ");
        num_edit(ui, &mut self.rate, "Rate: ");
        num_edit(ui, &mut self.rate_rndm, "Rate Random: ");
        num_edit(ui, &mut self.life_time_rndm, "Life Time Random: ");
        num_edit(ui, &mut self.volume_sweep, "Volume Sweep: ");
        num_edit(ui, &mut self.volume_min_rad, "Volume Min Radius: ");
        num_edit(ui, &mut self.air_resist, "Air Resistance: ");
        num_edit(ui, &mut self.moment_rndm, "Moment Random: ");
        nums_edit(ui, &mut self.emitter_rot, "Emitter Rotation: ");
        num_edit(ui, &mut self.max_frame, "Max Frame");
        num_edit(ui, &mut self.start_frame, "Start Frame");
        num_edit(ui, &mut self.life_time, "Life Time");
        num_edit(ui, &mut self.volume_size, "Volume Size");
        num_edit(ui, &mut self.div_number, "Division Number");
        num_edit(ui, &mut self.rate_step, "Rate Step");
    }
}

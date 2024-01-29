use binrw::binrw;
use egui::{CollapsingHeader, DragValue, RichText, Ui};
use std::fmt::Debug;

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

fn add_f32_edit_ui(name: &str, val: &mut f32, ui: &mut Ui) {
    ui.horizontal(|ui| {
        ui.label(name);
        ui.add(DragValue::new(val).speed(1))
    });
}
fn add_i16_edit_ui(name: &str, val: &mut i16, ui: &mut Ui) {
    ui.horizontal(|ui| {
        ui.label(name);
        ui.add(DragValue::new(val).speed(1))
    });
}

fn add_f32_arr3_ui(name: &str, arr: &mut [f32; 3], ui: &mut Ui) {
    ui.horizontal(|ui| {
        ui.label(name);
        ui.add(DragValue::new(&mut arr[0]).speed(1).prefix("x: "));
        ui.add(DragValue::new(&mut arr[1]).speed(1).prefix("y: "));
        ui.add(DragValue::new(&mut arr[2]).speed(1).prefix("z: "));
    });
}
fn add_i16_arr3_ui(name: &str, arr: &mut [i16; 3], ui: &mut Ui) {
    ui.horizontal(|ui| {
        ui.label(name);
        ui.add(DragValue::new(&mut arr[0]).speed(1).prefix("x: "));
        ui.add(DragValue::new(&mut arr[1]).speed(1).prefix("y: "));
        ui.add(DragValue::new(&mut arr[2]).speed(1).prefix("z: "));
    });
}

impl BEM1 {
    pub fn show_editor(&mut self, ui: &mut Ui) {
        CollapsingHeader::new("Dynamics").show(ui, |ui| {
            ui.label(RichText::new(format!("Emitter Flags: {:08X}", self.emit_flags)).monospace());
            ui.label(RichText::new(format!("Unknown Flags: {:08X}", self.unk_flags)).monospace());
            add_f32_arr3_ui("Scale: ", &mut self.scale, ui);
            add_f32_arr3_ui("Translation: ", &mut self.trans, ui);
            add_f32_arr3_ui("Direction: ", &mut self.direction, ui);
            add_f32_edit_ui("Velocity Omni: ", &mut self.initial_vel_omni, ui);
            add_f32_edit_ui("Velocity Axis: ", &mut self.initial_vel_axis, ui);
            add_f32_edit_ui("Velocity Random: ", &mut self.initial_vel_rndm, ui);
            add_f32_edit_ui("Velocity Direction: ", &mut self.initial_vel_dir, ui);
            add_f32_edit_ui("Spread: ", &mut self.spread, ui);
            add_f32_edit_ui("Velocity Ratio: ", &mut self.initial_vel_ratio, ui);
            add_f32_edit_ui("Rate: ", &mut self.rate, ui);
            add_f32_edit_ui("Rate Random: ", &mut self.rate_rndm, ui);
            add_f32_edit_ui("Life Time Random: ", &mut self.life_time_rndm, ui);
            add_f32_edit_ui("Volume Sweep: ", &mut self.volume_sweep, ui);
            add_f32_edit_ui("Volume Min Radius: ", &mut self.volume_min_rad, ui);
            add_f32_edit_ui("Air Resistance: ", &mut self.air_resist, ui);
            add_f32_edit_ui("Moment Random: ", &mut self.moment_rndm, ui);
            add_i16_arr3_ui("Emitter Rotation: ", &mut self.emitter_rot, ui);
            add_i16_edit_ui("Max Frame", &mut self.max_frame, ui);
            add_i16_edit_ui("Start Frame", &mut self.start_frame, ui);
            add_i16_edit_ui("Life Time", &mut self.life_time, ui);
            add_i16_edit_ui("Volume Size", &mut self.volume_size, ui);
            add_i16_edit_ui("Division Number", &mut self.div_number, ui);
            ui.horizontal(|ui| {
                ui.label("Rate Step: ");
                ui.add(
                    DragValue::new(&mut self.rate_step)
                        .speed(1)
                        .clamp_range(u8::MIN..=u8::MAX),
                )
            });
        });
    }
}

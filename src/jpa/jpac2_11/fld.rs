use binrw::binrw;
use egui::{CollapsingHeader, DragValue, Ui};
use std::fmt::Debug;

#[binrw]
#[brw(big)]
#[derive(Debug)]
pub struct FLD1 {
    #[br(temp, assert(size==0x44))]
    #[bw(calc = 0x44)]
    size:         u32, // 0x04
    pub flags:    u32,      // 0x08
    pub position: [f32; 3], // 0x0C
    pub distance: [f32; 3], // 0x18
    pub param1:   f32,      // 0x24
    pub param2:   f32,      // 0x28
    pub param3:   f32,      // 0x2C
    pub fade_in:  f32,      // 0x30
    pub fade_out: f32,      // 0x34
    pub en_time:  f32,      // 0x38
    pub dis_time: f32,      // 0x3C
    pub cycle:    f32,      // 0x40
}

impl FLD1 {
    pub fn show_editor(&mut self, block_num: usize, ui: &mut Ui) {
        CollapsingHeader::new(format!("Field {block_num}")).show(ui, |ui| {
            ui.label(format!("Flags: {:08X}", self.flags));

            ui.horizontal(|ui| {
                ui.label("Position: ");
                ui.add(
                    DragValue::new(&mut self.position[0])
                        .speed(1)
                        .clamp_range(f32::MIN..=f32::MAX)
                        .prefix("x: "),
                );
                ui.add(
                    DragValue::new(&mut self.position[1])
                        .speed(1)
                        .clamp_range(f32::MIN..=f32::MAX)
                        .prefix("y: "),
                );
                ui.add(
                    DragValue::new(&mut self.position[2])
                        .speed(1)
                        .clamp_range(f32::MIN..=f32::MAX)
                        .prefix("z: "),
                );
            });

            ui.horizontal(|ui| {
                ui.label("Distance: ");
                ui.add(DragValue::new(&mut self.distance[0]).speed(1).prefix("x: "));
                ui.add(DragValue::new(&mut self.distance[1]).speed(1).prefix("y: "));
                ui.add(DragValue::new(&mut self.distance[2]).speed(1).prefix("z: "));
            });

            ui.horizontal(|ui| {
                ui.label("Param 1: ");
                ui.add(DragValue::new(&mut self.param1).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Param 2: ");
                ui.add(DragValue::new(&mut self.param2).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Param 3: ");
                ui.add(DragValue::new(&mut self.param3).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Fade In: ");
                ui.add(DragValue::new(&mut self.fade_in).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Fade Out: ");
                ui.add(DragValue::new(&mut self.fade_out).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("en_time: ");
                ui.add(DragValue::new(&mut self.en_time).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("dis_time: ");
                ui.add(DragValue::new(&mut self.dis_time).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Cycle: ");
                ui.add(DragValue::new(&mut self.cycle).speed(1));
            });
        });
    }
}

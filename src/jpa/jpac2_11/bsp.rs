use binrw::binrw;
use byteorder::ReadBytesExt;
use egui::{CollapsingHeader, Color32, DragValue, Response, RichText, Ui};
use std::fmt::Debug;

use crate::jpa::{color_edit, write_color32};

#[binrw]
#[brw(big)]
#[derive(Debug)]
pub struct BSP1TexScrollAnmData {
    pub initial_trans:   [f32; 2],
    pub initial_scale:   [f32; 2],
    pub initial_rot:     f32,
    pub increment_trans: [f32; 2],
    pub increment_scale: [f32; 2],
    pub increment_rot:   f32,
}

impl BSP1TexScrollAnmData {
    pub fn show_editor(&mut self, ui: &mut Ui) {
        CollapsingHeader::new("Texture Animation Data").show(ui, |ui| {
            ui.horizontal(|ui| {
                ui.label("Initial Translation: ");
                ui.add(
                    DragValue::new(&mut self.initial_trans[0])
                        .speed(1)
                        .prefix("x: "),
                );
                ui.add(
                    DragValue::new(&mut self.initial_trans[1])
                        .speed(1)
                        .prefix("y: "),
                );
            });
            ui.horizontal(|ui| {
                ui.label("Initial Scale: ");
                ui.add(
                    DragValue::new(&mut self.initial_scale[0])
                        .speed(1)
                        .prefix("x: "),
                );
                ui.add(
                    DragValue::new(&mut self.initial_scale[1])
                        .speed(1)
                        .prefix("y: "),
                );
            });
            ui.horizontal(|ui| {
                ui.label("Initial Rotation: ");
                ui.add(DragValue::new(&mut self.initial_rot).speed(1));
            });
            ui.horizontal(|ui| {
                ui.label("Increment Translation: ");
                ui.add(
                    DragValue::new(&mut self.increment_trans[0])
                        .speed(1)
                        .prefix("x: "),
                );
                ui.add(
                    DragValue::new(&mut self.increment_trans[1])
                        .speed(1)
                        .prefix("y: "),
                );
            });
            ui.horizontal(|ui| {
                ui.label("Increment Scale: ");
                ui.add(
                    DragValue::new(&mut self.increment_scale[0])
                        .speed(1)
                        .prefix("x: "),
                );
                ui.add(
                    DragValue::new(&mut self.increment_scale[1])
                        .speed(1)
                        .prefix("y: "),
                );
            });
            ui.horizontal(|ui| {
                ui.label("Increment Rotation: ");
                ui.add(DragValue::new(&mut self.increment_rot).speed(1));
            });
        });
    }
}

#[binrw]
#[brw(big)]
#[derive(Default)]
pub struct FrameColor {
    frame:      u16,
    #[br(temp)]
    #[bw(calc = [color.r(), color.g(), color.b(), color.a()])]
    color_temp: [u8; 4],
    #[bw(ignore)]
    #[br(calc = Color32::from_rgba_premultiplied(color_temp[0], color_temp[1], color_temp[2], color_temp[3]))]
    color:      Color32,
    #[br(calc = color.to_hex())]
    #[bw(ignore)]
    color_str:  String,
}

impl FrameColor {
    fn show_editor(&mut self, ui: &mut Ui) -> Response {
        ui.horizontal(|ui| {
            ui.label("Frame: ");
            ui.add(
                DragValue::new(&mut self.frame)
                    .speed(1)
                    .clamp_range(0..=u16::MAX),
            );
            color_edit("Color:", &mut self.color, &mut self.color_str, ui);
            ui.button(RichText::new("X").color(Color32::RED))
        })
        .inner
    }
}

impl Debug for FrameColor {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("FrameColor")
            .field("frame", &self.frame)
            .field(
                "color",
                &format!(
                    "{:08X?}",
                    &[self.color[0], self.color[1], self.color[2], self.color[3]]
                ),
            )
            .finish()
    }
}

#[binrw]
#[brw(big, stream = s)]
#[derive(Debug)]
pub struct BSP1 {
    #[br(temp)]
    #[bw(calc = (0x34 // Base Size
        + if flags & 0x0400_0000 != 0 { 0x28 } else { 0x00 } // Texture Scroll Anim Data
        + ((tex_idx_anim_data.len() + 3) & !3) // Texture Anim Data
        + if color_flags & 0x02 != 0 {(color_prm_data.len()*6 + 3) & !3} else { 0 } // Color Prm Table
        + if color_flags & 0x08 != 0 {(color_env_data.len()*6 + 3) & !3} else { 0 } // Color Env Table
    ) as u32)]
    _size:                u32, // 0x04
    flags:                u32, // 0x08
    #[bw(calc = if color_flags & 0x02 != 0 { 0x34 + if flags & 0x0400_0000 != 0 {0x28} else {0} + ((tex_idx_anim_data.len() +3 ) & !3)} else {0} as u16)]
    _color_prm_offs:      u16, // 0x0C
    #[bw(calc = if color_flags & 0x08 != 0 {0x34 + if flags & 0x0400_0000 != 0 {0x28} else {0} + ((tex_idx_anim_data.len() + 3) & !3) + ((color_prm_data.len()*6 + 3) & !3)} else {0} as u16)]
    _color_env_offs:      u16, // 0x0E
    base_size:            [f32; 2], // 0x10
    blend_mode_flags:     u16, // 0x18
    alpha_cmpr_flags:     u8,  // 0x1A
    alpha_ref_0:          u8,  // 0x1B
    alpha_ref_1:          u8,  // 0x1C
    z_mode_flags:         u8,  // 0x1D
    tex_flags:            u8,  // 0x1E
    #[br(temp)]
    #[bw(calc = tex_idx_anim_data.len() as u8)]
    tex_idx_anm_count:    u8, // 0x1F
    tex_idx:              u8,  // 0x20
    color_flags:          u8,  // 0x21
    #[br(temp)]
    #[bw(calc = color_prm_data.len() as u8)]
    color_prm_data_count: u8, // 0x22
    #[br(temp)]
    #[bw(calc = color_env_data.len() as u8)]
    color_env_data_count: u8, // 0x23
    color_anm_max_frame:  u16, // 0x24

    #[br(calc = Color32::from_rgba_premultiplied(s.read_u8().unwrap(), s.read_u8().unwrap(), s.read_u8().unwrap(),s.read_u8().unwrap()))]
    #[bw(write_with = write_color32)]
    color_prm:     Color32, // 0x 26
    #[br(calc = color_prm.to_hex())]
    #[bw(ignore)]
    color_prm_str: String,

    #[br(calc = Color32::from_rgba_premultiplied(s.read_u8().unwrap(), s.read_u8().unwrap(), s.read_u8().unwrap(),s.read_u8().unwrap()))]
    #[bw(write_with = write_color32)]
    color_env:     Color32, // 0x2A
    #[br(calc = color_env.to_hex())]
    #[bw(ignore)]
    color_env_str: String,

    anm_rndm:                 u8, // 0x2E
    color_loop_offset_mask:   u8, // 0x2F
    #[brw(pad_after = 3)]
    tex_idx_loop_offset_mask: u8, // 0x30
    #[brw(if((flags & 0x0400_0000) != 0))]
    tex_scroll_anm_data:      Option<BSP1TexScrollAnmData>,
    #[br(count = tex_idx_anm_count)]
    #[brw(align_after = 0x4)]
    tex_idx_anim_data:        Vec<u8>, // 0x34 + 0x28?
    #[brw(if(color_flags & 0x02 != 0))]
    #[br(count = color_prm_data_count)]
    #[brw(align_after = 4)]
    color_prm_data:           Vec<FrameColor>,
    #[brw(if(color_flags & 0x08 != 0))]
    #[brw(align_after = 4)]
    #[br(count = color_env_data_count)]
    color_env_data:           Vec<FrameColor>,
}

impl BSP1 {
    pub fn show_editor(&mut self, ui: &mut Ui) {
        CollapsingHeader::new("Base Shape").show(ui, |ui| {
            ui.label(RichText::new(format!("Flags: {:08X}", self.flags)).monospace());
            ui.horizontal(|ui| {
                ui.label("Size: ");
                ui.add(
                    DragValue::new(&mut self.base_size[0])
                        .speed(1)
                        .prefix("x: "),
                );
                ui.add(
                    DragValue::new(&mut self.base_size[1])
                        .speed(1)
                        .prefix("y: "),
                );
            });
            ui.label(
                RichText::new(format!("Blend Mode Flags: {:#04X}", self.blend_mode_flags))
                    .monospace(),
            );
            ui.label(format!(
                "Alpha Compare Flags: {:#02X}",
                self.alpha_cmpr_flags
            ));
            ui.label(
                RichText::new(format!("Z Mode Flags: {:#02X}", self.z_mode_flags)).monospace(),
            );
            ui.label(RichText::new(format!("Texture Flags: {:#02X}", self.tex_flags)).monospace());
            ui.horizontal(|ui| {
                ui.label("Alpha Ref values: ");
                ui.add(DragValue::new(&mut self.alpha_ref_0).speed(1).prefix("0: "));
                ui.add(DragValue::new(&mut self.alpha_ref_0).speed(1).prefix("1: "));
            });
            ui.label(format!("Textures Index: {}   ", self.tex_idx));
            ui.label(RichText::new(format!("Color Flags: {:#02X}", self.color_flags)).monospace());
            ui.horizontal(|ui| {
                ui.label("Color Animation Frame Count: ");
                ui.add(DragValue::new(&mut self.color_anm_max_frame).speed(1));
            });
            ui.label(format!("Anim Random: {}", self.anm_rndm));
            color_edit(
                "Color Prm:",
                &mut self.color_prm,
                &mut self.color_prm_str,
                ui,
            );
            color_edit(
                "Color Env:",
                &mut self.color_env,
                &mut self.color_env_str,
                ui,
            );
            if let Some(extra_data) = &mut self.tex_scroll_anm_data {
                extra_data.show_editor(ui);
            }

            ui.label(format!(
                "Texture Index:  {:?}",
                self.tex_idx_anim_data.as_slice()
            ));

            CollapsingHeader::new("Color Prm Table").show(ui, |ui| {
                let mut delete_pressed: Option<usize> = None;
                for n in 0..self.color_prm_data.len() {
                    if self.color_prm_data[n].show_editor(ui).clicked() {
                        delete_pressed = Some(n);
                    }
                }
                if let Some(idx) = delete_pressed {
                    self.color_prm_data.remove(idx);
                }
                if ui
                    .button(RichText::new("+").color(Color32::GREEN))
                    .clicked()
                {
                    self.color_prm_data.push(FrameColor::default());
                }
            });
            CollapsingHeader::new("Color Env Table").show(ui, |ui| {
                let mut delete_pressed: Option<usize> = None;
                for n in 0..self.color_env_data.len() {
                    if self.color_env_data[n].show_editor(ui).clicked() {
                        delete_pressed = Some(n);
                    }
                }
                if let Some(idx) = delete_pressed {
                    self.color_env_data.remove(idx);
                }
                if ui
                    .button(RichText::new("+").color(Color32::GREEN))
                    .clicked()
                {
                    self.color_env_data.push(FrameColor::default());
                }
            });
        });
    }
}

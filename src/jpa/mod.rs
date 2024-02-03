use std::fmt::Debug;

use egui::{Color32, ComboBox, ScrollArea, TextEdit, TextStyle, Ui};

use binrw::{binrw, BinResult, BinWrite};

use crate::{FilterSettings, FilterType};

pub mod jpac2_11;

#[binrw::writer(writer, endian)]
pub fn write_color32(clr: &Color32) -> BinResult<()> {
    clr.r().write_options(writer, endian, ())?;
    clr.g().write_options(writer, endian, ())?;
    clr.b().write_options(writer, endian, ())?;
    clr.a().write_options(writer, endian, ())?;
    Ok(())
}

pub fn color_edit(label: &str, color: &mut Color32, color_str: &mut String, ui: &mut Ui) {
    ui.horizontal(|ui| {
        ui.label(label);
        if TextEdit::singleline(color_str)
            .font(TextStyle::Monospace)
            .desired_width(0.0)
            .clip_text(false)
            .show(ui)
            .response
            .lost_focus()
        {
            match Color32::from_hex(color_str.as_str()) {
                Ok(clr) => *color = clr,
                Err(_) => *color_str = color.to_hex(),
            };
        };
        if ui.color_edit_button_srgba(color).changed() {
            *color_str = color.to_hex();
        };
    });
}

#[derive(Clone, Copy, PartialEq, Debug)]
pub enum Selected {
    None,
    Resource(usize),
    Texture(usize),
}
impl Default for Selected {
    fn default() -> Self {
        Self::None
    }
}

#[binrw]
#[brw(big)]
#[derive(Debug)]
pub enum JPAC {
    #[brw(magic = b"JPAC2-11")]
    JPAC2_11(jpac2_11::JPAC),
}

impl JPAC {
    pub fn correct_parsing(&mut self) {
        match self {
            JPAC::JPAC2_11(jpac) => jpac.correct_parsing(),
        }
    }

    pub fn get_num_resources(&self) -> usize {
        match self {
            JPAC::JPAC2_11(jpac) => jpac.resources.len(),
        }
    }
    pub fn get_num_textures(&self) -> usize {
        match self {
            JPAC::JPAC2_11(jpac) => jpac.textures.len(),
        }
    }

    pub fn show_tree_ui(
        &mut self,
        selected: &mut Selected,
        filter: &mut FilterSettings,
        ui: &mut Ui,
    ) {
        // Filter Arrea
        ui.horizontal(|ui| {
            ui.label("Filter: ");
            ui.text_edit_singleline(&mut filter.filter_text);
        });
        let force_filter = ComboBox::from_label("Filter Type")
            .selected_text(format!("{:?}", filter.fitler_type))
            .show_ui(ui, |ui| {
                ui.selectable_value(&mut filter.fitler_type, FilterType::None, "None");
                ui.selectable_value(&mut filter.fitler_type, FilterType::Resource, "Resource ID");
                ui.selectable_value(&mut filter.fitler_type, FilterType::Texture, "Texture");
            })
            .response
            .changed();

        // The Scroll Area for navigating the tree
        ScrollArea::new([false, true])
            .auto_shrink([false, false])
            .show(ui, |ui| {
                match self {
                    Self::JPAC2_11(jpac) => {
                        jpac.show_tree_ui(selected, filter, force_filter, ui);
                    },
                }
            });
    }

    pub fn show_editor(&mut self, selected: Selected, ui: &mut Ui) {
        ScrollArea::new([false, true])
            .auto_shrink(false)
            .show(ui, |ui| {
                match self {
                    Self::JPAC2_11(jpac) => jpac.show_edit_ui(selected, ui),
                }
            });
    }
}

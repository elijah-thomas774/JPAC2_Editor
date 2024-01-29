use std::fmt::Debug;

use egui::{ScrollArea, Ui};

use binrw::binrw;

pub mod jpac2_11;

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

    pub fn show_tree_ui(&mut self, selected: &mut Selected, filter: &mut String, ui: &mut Ui) {
        // Filter Arrea
        ui.horizontal(|ui| {
            ui.label("Filter: ");
            ui.text_edit_singleline(filter);
        });

        // The Scroll Area for navigating the tree
        ScrollArea::new([false, true])
            .auto_shrink([false, false])
            .show(ui, |ui| {
                match self {
                    Self::JPAC2_11(jpac) => {
                        jpac.show_tree_ui(selected, filter, ui);
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

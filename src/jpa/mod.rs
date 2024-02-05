use std::fmt::Debug;

use egui::{ComboBox, Context, ScrollArea, Ui};

use binrw::binrw;

use crate::{FilterSettings, FilterType};

pub mod jpac2_10;
pub mod jpac2_11;

#[derive(serde::Serialize, serde::Deserialize, Debug)]
pub struct ResAlias {
    pub res_id: u16,
    pub name:   String,
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
    #[brw(magic = b"JPAC2-10")]
    JPAC2_10(jpac2_10::JPAC),
    #[brw(magic = b"JPAC2-11")]
    JPAC2_11(jpac2_11::JPAC),
}

impl JPAC {
    pub fn load_textures(&mut self, ctx: &Context) {
        match self {
            JPAC::JPAC2_11(jpac) => jpac.textures.iter_mut().for_each(|e| e.load_image(ctx)),
            JPAC::JPAC2_10(jpac) => jpac.textures.iter_mut().for_each(|e| e.load_image(ctx)),
        }
    }

    pub fn export_alias(&self) -> Vec<ResAlias> {
        match self {
            JPAC::JPAC2_11(jpac) => {
                jpac.resources
                    .iter()
                    .filter(|res| res.alias != format!("Resource {}", res.res_id))
                    .map(|res| {
                        ResAlias {
                            res_id: res.res_id,
                            name:   res.alias.clone(),
                        }
                    })
                    .collect()
            },
            JPAC::JPAC2_10(jpac) => {
                jpac.resources
                    .iter()
                    .filter(|res| res.alias != format!("Resource {}", res.res_id))
                    .map(|res| {
                        ResAlias {
                            res_id: res.res_id,
                            name:   res.alias.clone(),
                        }
                    })
                    .collect()
            },
        }
    }

    pub fn apply_alias(&mut self, alias: &Vec<ResAlias>) {
        match self {
            JPAC::JPAC2_11(jpac) => {
                for ResAlias { res_id: id, name } in alias {
                    for res in &mut jpac.resources {
                        if res.res_id == *id {
                            res.alias = name.clone();
                            break;
                        }
                    }
                }
            },
            JPAC::JPAC2_10(jpac) => {
                for ResAlias { res_id: id, name } in alias {
                    for res in &mut jpac.resources {
                        if res.res_id == *id {
                            res.alias = name.clone();
                            break;
                        }
                    }
                }
            },
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
            ui.text_edit_singleline(&mut filter.text);
        });
        let force_filter = ComboBox::from_label("Filter Type")
            .selected_text(format!("{:?}", filter.filter_type))
            .show_ui(ui, |ui| {
                ui.selectable_value(&mut filter.filter_type, FilterType::None, "None");
                ui.selectable_value(&mut filter.filter_type, FilterType::Resource, "Resource ID");
                ui.selectable_value(&mut filter.filter_type, FilterType::Texture, "Texture");
                ui.selectable_value(&mut filter.filter_type, FilterType::Name, "Name");
            })
            .response
            .changed();

        // The Scroll Area for navigating the tree
        ScrollArea::new([false, true])
            .auto_shrink([false, false])
            .show(ui, |ui| {
                match self {
                    Self::JPAC2_11(jpac) => jpac.show_tree_ui(selected, filter, force_filter, ui),
                    Self::JPAC2_10(jpac) => jpac.show_tree_ui(selected, filter, force_filter, ui),
                }
            });
    }

    pub fn show_editor(&mut self, selected: Selected, ui: &mut Ui) {
        ScrollArea::new([false, true])
            .auto_shrink(false)
            .show(ui, |ui| {
                match self {
                    Self::JPAC2_11(jpac) => jpac.show_edit_ui(selected, ui),
                    Self::JPAC2_10(jpac) => jpac.show_edit_ui(selected, ui),
                }
            });
    }
}

use std::{
    fmt::Debug,
    fs::File,
    io::{self, Read, Write},
};

use byteorder::{BigEndian, ReadBytesExt, WriteBytesExt};
use egui::{ComboBox, Context, ScrollArea, TextBuffer, Ui};

use binrw::{binrw, BinRead, BinWrite};

use crate::{FilterSettings, FilterType};

pub mod jpac2_10;
pub mod jpac2_11;
pub mod tex;

#[derive(serde::Serialize, serde::Deserialize, Debug)]
pub struct ResAlias {
    pub res_id: u16,
    pub name:   String,
}

#[derive(Clone, Copy, PartialEq, Default)]
pub enum Selected {
    #[default]
    None,
    Resource(usize),
    Texture(usize),
}

#[binrw]
#[brw(big)]
pub enum JPAC {
    #[brw(magic = b"JPAC2-10")]
    JPAC2_10(jpac2_10::JPAC),
    #[brw(magic = b"JPAC2-11")]
    JPAC2_11(jpac2_11::JPAC),
}

#[derive(thiserror::Error, Debug)]
pub enum ResourceError {
    #[error("Invalid Resource Version. Read `{0}`, but expected `{1}`")]
    InvalidResourceVersion(String, String),
    #[error("Could not read File")]
    FileError(#[from] io::Error),
    #[error("Could not write [binrw]")]
    BinrwError(#[from] binrw::Error),
}
impl JPAC {
    /// Exports a resource
    pub fn export_resources(
        &self,
        res_idxs: &[usize],
        file: &mut File,
    ) -> Result<(), ResourceError> {
        match self {
            JPAC::JPAC2_10(jpac) => {
                file.write(b"JPAC2-10")?; // Version
                file.write_u32::<BigEndian>(res_idxs.len() as u32)?; // count
                for &res_idx in res_idxs {
                    if let Some(res) = jpac.resources.get(res_idx) {
                        res.write(file)?; // [Resource]
                    }
                }
            },
            JPAC::JPAC2_11(jpac) => {
                file.write(b"JPAC2-11")?; // Version
                file.write_u32::<BigEndian>(res_idxs.len() as u32)?; // count
                for &res_idx in res_idxs {
                    if let Some(res) = jpac.resources.get(res_idx) {
                        res.write(file)?; // [Resource]
                    }
                }
            },
        }
        Ok(())
    }

    /// Reads in replacment resources
    pub fn import_resources(&mut self, file: &mut File) -> Result<(), ResourceError> {
        let mut version = [0u8; 8];
        file.read_exact(&mut version)?;
        let count = file.read_u32::<BigEndian>()?;

        let version = std::str::from_utf8(&version).unwrap();
        match self {
            JPAC::JPAC2_10(jpac) => {
                if version.eq("JPAC2-10") {
                    use jpac2_10::resource::JPAResource;
                    for _ in 0..count {
                        let new_res = JPAResource::read(file)?;
                        if let Some(res) = jpac
                            .resources
                            .iter_mut()
                            .find(|res| res.res_id == new_res.res_id)
                        {
                            *res = new_res;
                        }
                    }
                } else {
                    return Err(ResourceError::InvalidResourceVersion(
                        String::from(version),
                        String::from("JPAC2-10"),
                    ));
                }
            },
            JPAC::JPAC2_11(jpac) => {
                if version.eq("JPAC2-11") {
                    use jpac2_11::resource::JPAResource;
                    for _ in 0..count {
                        let new_res = JPAResource::read(file)?;
                        if let Some(res) = jpac
                            .resources
                            .iter_mut()
                            .find(|res| res.res_id == new_res.res_id)
                        {
                            *res = new_res;
                        }
                    }
                } else {
                    return Err(ResourceError::InvalidResourceVersion(
                        String::from(version),
                        String::from("JPAC2-11"),
                    ));
                }
            },
        }
        Ok(())
    }

    /// Loads Textures from the embedded images
    pub fn load_textures(&mut self, ctx: &Context) {
        match self {
            JPAC::JPAC2_11(jpac) => jpac.textures.iter_mut().for_each(|e| e.load_image(ctx)),
            JPAC::JPAC2_10(jpac) => jpac.textures.iter_mut().for_each(|e| e.load_image(ctx)),
        }
    }

    /// Exports names given for each resource
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

    /// Applies the names for each resource
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

    /// Displays the JPAC in the left tree view
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

    /// Shows the JPAC in the main editor
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

#[derive(Default)]
pub struct ResouceSelector {
    /// Filter string
    pub filter:         String,
    /// Status of Filter
    pub filter_type:    FilterType,
    /// String of values that are filter
    pub filter_subtext: String,
    /// What Indecies to show in the filter menu
    pub show:           Vec<usize>,
    /// What Resources were selected based on filter
    pub selected:       Vec<usize>,
}

/// Save Selection
impl JPAC {
    /// Show list of resources as save selection
    pub fn resource_selector(&mut self, selections: &mut ResouceSelector, ui: &mut Ui) {
        // Filter Text Entry
        let filter_res = ui.text_edit_singleline(&mut selections.filter);
        let filter_changed = filter_res.changed();
        let filter_type = selections.filter_type;
        let type_changed = ComboBox::from_label("Filter Type")
            .selected_text(format!("{:?}", selections.filter_type))
            .show_ui(ui, |ui| {
                let is_none = ui
                    .selectable_value(&mut selections.filter_type, FilterType::None, "None")
                    .clicked();
                let is_id = ui
                    .selectable_value(
                        &mut selections.filter_type,
                        FilterType::Resource,
                        "Resource ID",
                    )
                    .clicked();
                let is_name = ui
                    .selectable_value(&mut selections.filter_type, FilterType::Name, "Name")
                    .clicked();
            });

        // Since the Filter changed, the show list needs to be updated
        if filter_changed
            || (filter_type != selections.filter_type)
            || (selections.filter_type == FilterType::None && selections.show.is_empty())
        {
            selections.show.clear();
            // If filter changed to be empty or is none, show everything
            if selections.filter.is_empty() || matches!(selections.filter_type, FilterType::None) {
                selections.show = match self {
                    JPAC::JPAC2_10(jpac) => {
                        jpac.resources
                            .iter()
                            .enumerate()
                            .map(|(idx, _)| idx)
                            .collect()
                    },
                    JPAC::JPAC2_11(jpac) => {
                        jpac.resources
                            .iter()
                            .enumerate()
                            .map(|(idx, _)| idx)
                            .collect()
                    },
                };
            }
            // filter the results
            else {
                // Parse the filter
                let mut entries: Vec<&str> = selections.filter.split(',').collect::<Vec<&str>>();
                // Trim whitespace from each entry
                entries.iter_mut().for_each(|e| *e = e.trim());
                // Apply the filter
                for entry in entries {
                    match selections.filter_type {
                        FilterType::Resource => {
                            if let Ok(id) = entry.parse::<u16>() {
                                if let Some(res_idx) = match self {
                                    JPAC::JPAC2_10(jpac) => {
                                        jpac.resources.iter().enumerate().find_map(|(idx, res)| {
                                            (res.res_id == id).then_some(idx)
                                        })
                                    },
                                    JPAC::JPAC2_11(jpac) => {
                                        jpac.resources.iter().enumerate().find_map(|(idx, res)| {
                                            (res.res_id == id).then_some(idx)
                                        })
                                    },
                                } {
                                    selections.show.push(res_idx);
                                }
                            }
                        },
                        FilterType::Name => {
                            if let Some(res_idx) = match self {
                                JPAC::JPAC2_10(jpac) => {
                                    jpac.resources.iter().enumerate().find_map(|(idx, res)| {
                                        (res.alias.contains(entry)).then_some(idx)
                                    })
                                },
                                JPAC::JPAC2_11(jpac) => {
                                    jpac.resources.iter().enumerate().find_map(|(idx, res)| {
                                        (res.alias.contains(entry)).then_some(idx)
                                    })
                                },
                            } {
                                selections.show.push(res_idx);
                            }
                        },
                        _ => {},
                    }
                }
            }
        }

        // Watch for is selection was updated
        let mut selection_changed: bool = false;
        // Show resources
        ScrollArea::new([false, true])
            .auto_shrink(false)
            .show(ui, |ui| {
                for &idx in &selections.show {
                    match self {
                        JPAC::JPAC2_10(jpac) => {
                            if ui
                                .selectable_label(
                                    selections.selected.contains(&idx),
                                    jpac.resources[idx].alias.as_str(),
                                )
                                .clicked()
                            {
                                if let Some(remove_idx) = selections
                                    .selected
                                    .iter()
                                    .enumerate()
                                    .find_map(|(i, &val)| (val == idx).then_some(i))
                                {
                                    selections.selected.remove(remove_idx);
                                } else {
                                    selections.selected.push(idx);
                                }
                                selection_changed = true;
                            }
                        },
                        JPAC::JPAC2_11(jpac) => {
                            if ui
                                .selectable_label(
                                    selections.selected.contains(&idx),
                                    jpac.resources[idx].alias.as_str(),
                                )
                                .clicked()
                            {
                                if let Some(remove_idx) = selections
                                    .selected
                                    .iter()
                                    .enumerate()
                                    .find_map(|(i, &val)| (val == idx).then_some(i))
                                {
                                    selections.selected.remove(remove_idx);
                                } else {
                                    selections.selected.push(idx);
                                }
                                selection_changed = true;
                            }
                        },
                    }
                }
            });
        if selection_changed {
            // Change Filter subtext
            selections.filter_subtext.clear();
            match self {
                JPAC::JPAC2_10(jpac) => {
                    for res in
                        jpac.resources.iter().enumerate().filter_map(|(idx, res)| {
                            selections.selected.contains(&idx).then_some(res)
                        })
                    {
                        selections.filter_subtext += res.alias.as_str();
                        selections.filter_subtext += ", ";
                    }
                },
                JPAC::JPAC2_11(jpac) => {
                    for res in
                        jpac.resources.iter().enumerate().filter_map(|(idx, res)| {
                            selections.selected.contains(&idx).then_some(res)
                        })
                    {
                        selections.filter_subtext += res.alias.as_str();
                        selections.filter_subtext += ", ";
                    }
                },
            }
        }
    }
}

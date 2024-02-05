use super::{
    bem::BEM1, bsp::BSP1, esp::ESP1, etx::ETX1, fld::FLD1, kfa::KFA1, ssp::SSP1, tdb::TDB1,
};

use crate::jpa::tex::JPATexture;
use binrw::binrw;
use egui::{CollapsingHeader, RichText, Ui};
use std::fmt::Debug;

/// Describes the various types the data blocks can contain. Rust's pattern
/// matching here is brilliant for containing all this information
#[binrw]
#[brw(big)]
#[br(import(tdb_count : u8))]
#[derive(Debug)]
pub enum DataBlock {
    #[brw(magic = b"BEM1")]
    DynamicsBlock(BEM1),
    #[brw(magic = b"BSP1")]
    BaseShape(BSP1),
    #[brw(magic = b"ESP1")]
    ExtraShape(ESP1),
    #[brw(magic = b"ETX1")]
    ExTexShape(ETX1),
    #[brw(magic = b"SSP1")]
    ChildShape(SSP1),
    #[brw(magic = b"FLD1")]
    FieldBlock(FLD1),
    #[brw(magic = b"KFA1")]
    KeyBlock(KFA1),
    #[brw(magic = b"TDB1")]
    TextureDataBase(#[br(args(tdb_count))] TDB1),
}

/// Describes the Resource Block, a single particle.
#[binrw]
#[brw(big)]
#[derive(Debug, Default)]
pub struct JPAResource {
    /// Resource ID
    pub res_id:  u16,
    #[bw(ignore)]
    #[br(calc = format!("Resource {res_id}"))]
    pub alias:   String,
    #[br(temp)]
    #[bw(calc = blocks.len() as u16)]
    block_count: u16,
    #[br(temp)]
    #[bw(calc = blocks.iter().filter(|&block| matches!(block, DataBlock::FieldBlock(_))).count() as u8)]
    _fld_count:  u8,
    #[br(temp)]
    #[bw(calc = blocks.iter().filter(|&block| matches!(block, DataBlock::KeyBlock(_))).count() as u8)]
    _kfa_count:  u8,
    #[br(temp)]
    #[bw(calc = self.get_textures().textures.len() as u8)]
    tdb_count:   u8,
    #[brw(pad_before = 1)]
    #[br(count = block_count, args { inner: (tdb_count,) })]
    /// Resource Data to form the Particle
    pub blocks:  Vec<DataBlock>,
}

/// Helper Functions
impl JPAResource {
    /// Shows the resource editor
    pub fn show_editor(&mut self, textures: &Vec<JPATexture>, ui: &mut Ui) {
        ui.label(RichText::new(format!(
            "Editing Resource: {0:} [{0:#06X}]: {1:}",
            self.res_id, self.alias
        )));
        // Resource Name
        ui.horizontal(|ui| {
            ui.label("Name");
            ui.text_edit_singleline(&mut self.alias);
        });

        // Dynamics Block
        CollapsingHeader::new(RichText::new("Dynamics Block (BEM1)").strong())
            .show(ui, |ui| self.get_dynamics_mut().show_editor(ui));
        // Base Shape
        CollapsingHeader::new(RichText::new("Base Shape Block (BSP1)").strong())
            .show(ui, |ui| self.get_base_shape_mut().show_editor(ui));
        // Child Shape
        CollapsingHeader::new(RichText::new("Child Shape Block (SSP1)").strong()).show(ui, |ui| {
            if let Some(blk) = self.get_child_shape_mut() {
                blk.show_editor(ui);
            }
        });
        // Extra Shape
        CollapsingHeader::new(RichText::new("Extra Shape Block (ESP1)").strong()).show(ui, |ui| {
            if let Some(blk) = self.get_extra_shape_mut() {
                blk.show_editor(ui);
            }
        });
        // ExTex
        CollapsingHeader::new(RichText::new("ExTex Shape Block (ETX1)").strong()).show(ui, |ui| {
            if let Some(blk) = self.get_ex_tex_shape_mut() {
                blk.show_editor(ui);
            }
        });
        // Textures
        CollapsingHeader::new(RichText::new("Textures (TDB1)").strong())
            .show(ui, |ui| self.get_textures_mut().show_editor(textures, ui));
        // Key Blocks
        CollapsingHeader::new(RichText::new("Key Blocks (KFA1)").strong()).show(ui, |ui| {
            self.get_key_blocks_mut()
                .iter_mut()
                .enumerate()
                .for_each(|(n, kfa)| {
                    CollapsingHeader::new(format!("{n}")).show(ui, |ui| kfa.show_editor(ui));
                });
            // TODO: (Option to add Block)
        });
        // Field Blocks
        CollapsingHeader::new(RichText::new("Field Blocks (FLD1)").strong()).show(ui, |ui| {
            self.get_field_blocks_mut()
                .iter_mut()
                .enumerate()
                .for_each(|(n, fld)| {
                    CollapsingHeader::new(format!("{n}")).show(ui, |ui| fld.show_editor(ui));
                });
            // TODO: (Option to add Block)
        });
    }
}

/// Data Block Accessor Functions
#[allow(dead_code)]
impl JPAResource {
    // ------------ DynamicsBlock ---------------------------------------------
    /// Grabs the Dynamics (BEM1) block. Each Resouce is garunteed to have one.
    pub fn get_dynamics(&self) -> &BEM1 {
        for block in &self.blocks {
            match block {
                DataBlock::DynamicsBlock(bem1) => return bem1,
                _ => {},
            }
        }
        panic!("No BEM Block Found!!");
    }

    /// Grabs the Dynamics (BEM1) block as mutable. Each Resource is garunteed
    /// to have one.
    pub fn get_dynamics_mut(&mut self) -> &mut BEM1 {
        for block in &mut self.blocks {
            match block {
                DataBlock::DynamicsBlock(bem1) => return bem1,
                _ => {},
            }
        }
        panic!("No BEM Block Found!!");
    }

    // ------------ BaseShape -------------------------------------------------
    /// Grabs the Base Shape block (BSP1). Each Resource is garunteed to have
    /// one.
    pub fn get_base_shape(&self) -> &BSP1 {
        for block in &self.blocks {
            match block {
                DataBlock::BaseShape(bsp1) => return bsp1,
                _ => {},
            }
        }
        panic!("No BSP Block Found!!");
    }

    /// Grabs the Base Shape block (BSP1) as mutable. Each Resource is
    /// garunteed to have one.
    pub fn get_base_shape_mut(&mut self) -> &mut BSP1 {
        for block in &mut self.blocks {
            match block {
                DataBlock::BaseShape(bsp1) => return bsp1,
                _ => {},
            }
        }
        panic!("No BSP Block Found!!");
    }

    // ------------ ExtraShape ------------------------------------------------
    /// Grabs the Extra Shape Data (ESP1). May not exist.
    pub fn get_extra_shape(&self) -> Option<&ESP1> {
        for block in &self.blocks {
            match block {
                DataBlock::ExtraShape(esp1) => return Some(esp1),
                _ => {},
            }
        }
        None
    }

    /// Grabs the Extra Shape Data (ESP1) as mutable. May not exist.
    pub fn get_extra_shape_mut(&mut self) -> Option<&mut ESP1> {
        for block in &mut self.blocks {
            match block {
                DataBlock::ExtraShape(esp1) => return Some(esp1),
                _ => {},
            }
        }
        None
    }

    // ------------ ExTexShape ------------------------------------------------
    /// Grabs the Child Shape Data (SSP1). May not exist.
    pub fn get_ex_tex_shape(&self) -> Option<&ETX1> {
        for block in &self.blocks {
            match block {
                DataBlock::ExTexShape(etx1) => return Some(etx1),
                _ => {},
            }
        }
        None
    }

    /// Grabs the Child Shape Data (SSP1) as mutable. May not exist.
    pub fn get_ex_tex_shape_mut(&mut self) -> Option<&mut ETX1> {
        for block in &mut self.blocks {
            match block {
                DataBlock::ExTexShape(etx1) => return Some(etx1),
                _ => {},
            }
        }
        None
    }

    // ------------ ChildShape ------------------------------------------------
    /// Grabs the Child Shape Data (SSP1). May not exist.
    pub fn get_child_shape(&self) -> Option<&SSP1> {
        for block in &self.blocks {
            match block {
                DataBlock::ChildShape(ssp1) => return Some(ssp1),
                _ => {},
            }
        }
        None
    }

    /// Grabs the Child Shape Data (SSP1) as mutable. May not exist.
    pub fn get_child_shape_mut(&mut self) -> Option<&mut SSP1> {
        for block in &mut self.blocks {
            match block {
                DataBlock::ChildShape(ssp1) => return Some(ssp1),
                _ => {},
            }
        }
        None
    }

    // ------------ FieldBlock ------------------------------------------------
    /// Grabs all Field Blocks. There could be none, so check the size.
    pub fn get_field_blocks(&self) -> Vec<&FLD1> {
        self.blocks
            .iter()
            .filter_map(|block| {
                match block {
                    DataBlock::FieldBlock(fld1) => Some(fld1),
                    _ => None,
                }
            })
            .collect()
    }

    /// Grabs all Field Blocks as mutable. There could be none, so check the
    /// size.
    pub fn get_field_blocks_mut(&mut self) -> Vec<&mut FLD1> {
        self.blocks
            .iter_mut()
            .filter_map(|block| {
                match block {
                    DataBlock::FieldBlock(fld1) => Some(fld1),
                    _ => None,
                }
            })
            .collect()
    }

    // ------------ KeyBlock --------------------------------------------------
    /// Grabs all Key Blocks (KFA1) Blocks. There could be none, so check the
    /// size.
    pub fn get_key_blocks(&self) -> Vec<&KFA1> {
        self.blocks
            .iter()
            .filter_map(|block| {
                match block {
                    DataBlock::KeyBlock(kfa1) => Some(kfa1),
                    _ => None,
                }
            })
            .collect()
    }

    /// Grabs all Key Blocks (KFA1) Blocks as mutable. There could be none,
    /// so check the size.
    pub fn get_key_blocks_mut(&mut self) -> Vec<&mut KFA1> {
        self.blocks
            .iter_mut()
            .filter_map(|block| {
                match block {
                    DataBlock::KeyBlock(kfa1) => Some(kfa1),
                    _ => None,
                }
            })
            .collect()
    }

    // ------------ TextureDataBase -------------------------------------------
    /// Grabs the Texture Database (TDB1) block. Each Resource is garunteed to
    /// have one.
    pub fn get_textures(&self) -> &TDB1 {
        for block in &self.blocks {
            match block {
                DataBlock::TextureDataBase(tdb1) => return &tdb1,
                _ => {},
            }
        }
        panic!("No TDB Block Found!!");
    }

    /// Grabs the Texture Database (TDB1) block as mutable. Each Resource is
    /// garunteed to have one.
    pub fn get_textures_mut(&mut self) -> &mut TDB1 {
        for block in &mut self.blocks {
            match block {
                DataBlock::TextureDataBase(tdb1) => return tdb1,
                _ => {},
            }
        }
        panic!("No TDB Block Found!!");
    }
}

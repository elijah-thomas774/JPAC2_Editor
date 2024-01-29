use super::{bem, bsp, esp, etx, fld, kfa, ssp, tdb};
use binrw::binrw;
use std::fmt::Debug;

#[binrw]
#[brw(big)]
#[derive(Debug)]
pub enum DataBlock {
    #[brw(magic = b"BEM1")]
    Dynamics(bem::BEM1),
    #[brw(magic = b"BSP1")]
    BaseShape(bsp::BSP1),
    #[brw(magic = b"ESP1")]
    ExtraShape(esp::ESP1),
    #[brw(magic = b"ETX1")]
    ExTex(etx::ETX1),
    #[brw(magic = b"SSP1")]
    ChildShape(ssp::SSP1),
    #[brw(magic = b"FLD1")]
    Field(fld::FLD1),
    #[brw(magic = b"KFA1")]
    KeyFrame(kfa::KFA1),
    #[brw(magic = b"TDB1")]
    TextureDataBase(tdb::TDB1),
}

#[binrw]
#[brw(big)]
#[derive(Debug, Default)]
pub struct JPAResource {
    pub res_id:    u16,
    #[br(temp)]
    #[bw(calc = blocks.len() as u16)]
    block_count:   u16,
    #[br(temp)]
    #[bw(calc = blocks.iter().filter(|&block| match block {DataBlock::Field(_) => true, _=> false}).count() as u8)]
    _fld_count:    u8,
    #[br(temp)]
    #[bw(calc = blocks.iter().filter(|&block| match block {DataBlock::KeyFrame(_) => true, _=> false}).count() as u8)]
    _kfa_count:    u8,
    pub tdb_count: u8,
    #[brw(pad_before = 1)]
    #[br(count = block_count)]
    pub blocks:    Vec<DataBlock>,
}

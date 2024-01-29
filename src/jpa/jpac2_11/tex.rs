use binrw::binrw;
use std::fmt::Debug;

#[binrw]
#[brw(big, repr = u8)]
#[derive(Debug)]
pub enum BTIFormat {
    I4     = 0x0,
    I8     = 0x1,
    IA4    = 0x2,
    IA8    = 0x3,
    RGB565 = 0x4,
    RGB5A3 = 0x5,
    RGBA8  = 0x6,
    C4     = 0x8,
    C8     = 0x9,
    C14X2  = 0xA,
    CMPR   = 0xE,
}

#[binrw]
#[brw(big)]
#[derive(Debug)]
pub struct BTIHeader {
    format:            BTIFormat, // 0x00
    enable_alpha:      u8,        // 0x01
    width:             u16,       // 0x02
    height:            u16,       // 0x04
    wrap_s:            u8,        // 0x06
    wrap_t:            u8,        // 0x07
    pallet_fmt:        u16,       // 0x08
    num_pallet_etries: u16,       // 0x0A
    pallet_data_offs:  u32,       // 0x0C
    unk1:              u32,       // 0x10
    mag_filter_type:   u8,        // 0x14
    min_filter_type:   u8,        // 0x15
    unk2:              u16,       // 0x16
    num_images:        u8,        // 0x18
    unk3:              u8,        // 0x19
    unk4:              u16,       // 0x1A
    image_data_offs:   u32,       // 0x1C // Will always be 0x20
}

#[binrw]
#[brw(big)]
#[brw(magic = b"TEX1")]
pub struct JPATexture {
    #[br(temp)]
    #[bw(calc = (0x20 + 0x20 + image_data.len()) as u32)]
    pub size:       u32,
    pub unk:        u32,
    pub name:       [u8; 0x14],
    pub header:     BTIHeader,
    #[br(count = size-0x40)]
    pub image_data: Vec<u8>,
}

impl Debug for JPATexture {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("JPATexture")
            .field("unk", &self.unk)
            .field(
                "name",
                &std::str::from_utf8(&self.name)
                    .unwrap()
                    .trim_end_matches(['\0']),
            )
            .field("header", &self.header)
            .field(
                "image_data",
                &format!("{:02X?}", &self.image_data.as_slice()),
            )
            .finish()
    }
}

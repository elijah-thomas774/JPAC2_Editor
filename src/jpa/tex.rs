use crate::bti::BTIHeader;
use binrw::{binrw, NullString};
use egui::{Context, TextureHandle};

#[binrw]
#[brw(big)]
#[brw(magic = b"TEX1")]
pub struct JPATexture {
    #[br(temp)]
    #[bw(calc = (0x20 + 0x20 + image_data.len()) as u32)]
    size:           u32,
    version:        u32,
    #[bw(assert(name.len() < 0x14, "Name cannot exceed 32 characters!!"))]
    #[brw(pad_size_to = 0x14)]
    pub name:       NullString,
    pub header:     BTIHeader,
    #[br(count = size-0x40)]
    pub image_data: Vec<u8>,
}

impl JPATexture {
    pub fn get_handle(&self) -> &Option<TextureHandle> {
        &self.header.texture_handle
    }
    pub fn load_image(&mut self, ctx: &Context) {
        self.header.read_image_data(
            &String::from_utf8(self.name.to_vec()).unwrap(),
            ctx,
            &self.image_data,
        );
    }
}

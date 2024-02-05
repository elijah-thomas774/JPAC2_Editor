use std::io::Cursor;

use binrw::binrw;
use byteorder::{BigEndian, ReadBytesExt};
use egui::{Color32, ColorImage, Context, ImageData, TextureHandle, TextureOptions};

#[binrw]
#[brw(big, repr = u8)]
#[derive(Debug, Clone, Copy)]
pub enum BTIFormat {
    I4     = 0x0,
    I8     = 0x1,
    IA4    = 0x2,
    IA8    = 0x3,
    RGB565 = 0x4,
    RGB5A3 = 0x5,
    RGBA32 = 0x6,
    C4     = 0x8,
    C8     = 0x9,
    C14X2  = 0xA,
    CMPR   = 0xE,
}

#[binrw]
#[brw(big)]
#[allow(non_snake_case)]
pub struct BTIHeader {
    pub format:          BTIFormat, // 0x00
    pub alpha_enable:    u8,        // 0x01
    pub width:           u16,       // 0x02
    pub height:          u16,       // 0x04
    pub wrap_s:          u8,        // 0x06
    pub wrap_t:          u8,        // 0x07
    pub index_texture:   u8,        // 0x08
    pub color_fmt:       u8,        // 0x08
    pub num_colors:      u16,       // 0x0A
    pub pallet_offset:   u32,       // 0x0C
    pub mipmap_enable:   u8,        // 0x10
    pub do_edge_LOD:     u8,        // 0x11
    pub bias_clamp:      u8,        // 0x12
    pub max_anisotropy:  u8,        // 0x13
    pub min_filter:      u8,        // 0x14
    pub mag_filter:      u8,        // 0x15
    pub min_LOD:         i8,        // 0x16
    pub max_LOD:         i8,        // 0x17
    pub mipmap_count:    u8,        // 0x18
    pub unk3:            u8,        // 0x19
    pub LOD_bias:        i16,       // 0x1A
    pub image_data_offs: u32,       // 0x1C // Will always be 0x20
    #[brw(ignore)]
    pub texture_handle:  Option<TextureHandle>,
}

impl BTIFormat {
    pub fn read_data(&self, width: usize, height: usize, image_data: &Vec<u8>) -> ColorImage {
        let mut pixels: Vec<Color32> = Vec::new();
        let mut reader = Cursor::new(image_data);
        let (block_width, block_height) = match self {
            BTIFormat::I4 => (8, 8),
            BTIFormat::I8 => (8, 4),
            BTIFormat::IA4 => (8, 4),
            BTIFormat::IA8 => (4, 4),
            BTIFormat::RGB565 => (4, 4),
            BTIFormat::RGB5A3 => (4, 4),
            BTIFormat::RGBA32 => (8, 8),
            BTIFormat::C4 => (8, 8),
            BTIFormat::C8 => (8, 4),
            BTIFormat::C14X2 => (4, 4),
            BTIFormat::CMPR => (8, 8),
        };

        let (block_num_w, block_num_h) =
            (width.div_ceil(block_width), height.div_ceil(block_height));

        let block_num = block_num_h * block_num_w;

        let mut blocks = vec![vec![vec![Color32::default(); block_width]; block_height]; block_num];

        // Read the data
        for n in 0..block_num {
            for r in 0..block_height {
                for c in 0..block_width {
                    match self {
                        // Read the block as I4 format
                        BTIFormat::I4 => {
                            if (c % 2) == 0 {
                                let pixels = reader.read_u8().unwrap();
                                let (pix1, pix2) =
                                    (((pixels >> 4) & 0xF) * 0x11, ((pixels >> 0) & 0xF) * 0x11);

                                blocks[n][r][c + 0] = Color32::from_gray(pix1);
                                blocks[n][r][c + 1] = Color32::from_gray(pix2);
                            }
                        },
                        // Read the block as I8 format
                        BTIFormat::I8 => {
                            let byte = reader.read_u8().unwrap();
                            blocks[n][r][c] = Color32::from_gray(byte);
                        },
                        BTIFormat::IA4 => {
                            let byte = reader.read_u8().unwrap();
                            let gray = ((byte >> 0) & 0xF) * 0x11;
                            let alpha = ((byte >> 4) & 0xF) * 0x11;
                            blocks[n][r][c] =
                                Color32::from_rgba_premultiplied(gray, gray, gray, alpha);
                        },
                        BTIFormat::IA8 => {
                            let alpha = reader.read_u8().unwrap();
                            let gray = reader.read_u8().unwrap();
                            blocks[n][r][c] =
                                Color32::from_rgba_unmultiplied(gray, gray, gray, alpha);
                        },
                        BTIFormat::RGB565 => {
                            let data = reader.read_u16::<BigEndian>().unwrap();
                            let red = (((data >> 11) & 0x1F) * 8) as u8;
                            let green = (((data >> 5) & 0x3F) * 4) as u8;
                            let blue = (((data >> 0) & 0x1F) * 8) as u8;
                            blocks[n][r][c] =
                                Color32::from_rgba_unmultiplied(red, green, blue, 0xFF);
                        },
                        BTIFormat::RGB5A3 => {
                            let data = reader.read_u16::<BigEndian>().unwrap();
                            let (red, green, blue, alpha) = if (data & 0x8000) != 0 {
                                (
                                    (((data >> 10) & 0x1F) * 8) as u8,
                                    (((data >> 5) & 0x1F) * 8) as u8,
                                    (((data >> 0) & 0x1F) * 8) as u8,
                                    0xFFu8,
                                )
                            } else {
                                (
                                    (((data >> 8) & 0xF) * 17) as u8,
                                    (((data >> 4) & 0xF) * 17) as u8,
                                    (((data >> 0) & 0xF) * 17) as u8,
                                    (((data >> 12) & 0x7) * 32) as u8,
                                )
                            };
                            blocks[n][r][c] =
                                Color32::from_rgba_unmultiplied(red, green, blue, alpha);
                        },
                        BTIFormat::CMPR => blocks[n][r][c] = Color32::WHITE, // TODO: NYI
                        BTIFormat::RGBA32 => blocks[n][r][c] = Color32::WHITE, // TODO: NYI
                        BTIFormat::C4 => blocks[n][r][c] = Color32::WHITE,   // TODO: NYI
                        BTIFormat::C8 => blocks[n][r][c] = Color32::WHITE,   // TODO: NYI
                        BTIFormat::C14X2 => blocks[n][r][c] = Color32::WHITE, // TODO: NYI
                    }
                }
            }
        }

        // Reorder pixels
        for r in 0..height {
            for c in 0..width {
                let block_idx = (c / (block_width)) + (r / (block_height)) * block_num_w;
                pixels.push(blocks[block_idx][r % block_height][c % block_width]);
            }
        }
        assert_eq!(pixels.len(), height * width);

        return ColorImage {
            size:   [width, height],
            pixels: pixels.clone(),
        };
    }
}

impl BTIHeader {
    /// Reads the BTI and puts it into the context
    pub fn read_image_data(&mut self, name: &String, ctx: &Context, image_data: &Vec<u8>) {
        let (width, height) = (self.width as _, self.height as _);
        let filter_option = TextureOptions {
            magnification: match self.mag_filter {
                0 => egui::TextureFilter::Nearest,
                _ => egui::TextureFilter::Linear,
            },
            minification:  match self.min_filter {
                0 => egui::TextureFilter::Nearest,
                _ => egui::TextureFilter::Linear,
            },
        };

        self.texture_handle = Some(ctx.load_texture(
            name,
            ImageData::Color(self.format.read_data(width, height, image_data).into()),
            filter_option,
        ));
    }
}

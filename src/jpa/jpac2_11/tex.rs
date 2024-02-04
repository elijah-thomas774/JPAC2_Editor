use binrw::{binrw, NullString};
use byteorder::{BigEndian, ReadBytesExt};
use egui::{Color32, ColorImage, Context, ImageData, TextureHandle};
use std::{fmt::Debug, io::Cursor};

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
#[derive(Debug)]
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
}

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
    #[brw(ignore)]
    pub texture:    Option<TextureHandle>,
}

impl Debug for JPATexture {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("JPATexture")
            .field("Version Number", &self.version)
            .field(
                "name",
                &String::from_utf8(self.name.to_vec()).unwrap().as_str(),
            )
            .field("header", &self.header)
            .field(
                "image_data",
                &format!("{:02X?}", &self.image_data.as_slice()),
            )
            .finish()
    }
}

impl JPATexture {
    pub fn load_image(&mut self, ctx: &Context) {
        // Load Data bytes
        let data_bytes = self.image_data.as_slice();
        let (width, height) = (self.header.width as _, self.header.height as _);

        // Convert to RGBA
        self.texture = Some(
            ctx.load_texture(
                &String::from_utf8(self.name.to_vec()).unwrap(),
                ImageData::Color(
                    ColorImage {
                        size:   [width, height],
                        pixels: match self.header.format {
                            BTIFormat::I4 => {
                                let mut pixels = Vec::<Color32>::new();
                                pixels.reserve_exact(height * width);
                                const BLOCK_WIDTH: usize = 8; // pixels
                                const BLOCK_HEIGHT: usize = 8; // pixels
                                type Block = [[Color32; BLOCK_WIDTH]; BLOCK_HEIGHT];

                                let num_blocks_w = width.div_ceil(BLOCK_WIDTH);
                                let num_blocks_h = height.div_ceil(BLOCK_HEIGHT);
                                let num_blocks = num_blocks_w * num_blocks_h;
                                let mut blocks = Vec::<Block>::new();

                                let mut reader = Cursor::new(data_bytes);
                                // Read Blocks
                                for _ in 0..num_blocks {
                                    let mut block: Block = Block::default();
                                    // Read one block and append before moving
                                    for r in 0..BLOCK_HEIGHT {
                                        // div 2 due to each byte being two pixels
                                        for c in 0..(BLOCK_WIDTH / 2) {
                                            // Read and split byte into pixels
                                            let pixels = reader.read_u8().unwrap();
                                            let (pix1, pix2) = (
                                                ((pixels >> 4) & 0xF) * 0x11,
                                                ((pixels >> 0) & 0xF) * 0x11,
                                            );

                                            block[r][c * 2 + 0] = Color32::from_gray(pix1);
                                            block[r][c * 2 + 1] = Color32::from_gray(pix2);
                                        }
                                    }
                                    blocks.push(block);
                                }
                                // Reorder Blocks into Vec
                                for r in 0..height {
                                    for c in 0..width {
                                        let block_idx = (c / (BLOCK_WIDTH))
                                            + (r / (BLOCK_HEIGHT)) * num_blocks_w;
                                        pixels.push(
                                            blocks[block_idx][r % BLOCK_HEIGHT][c % BLOCK_WIDTH],
                                        );
                                    }
                                }
                                assert_eq!(pixels.len(), height * width);
                                pixels.clone()
                            }, // 27
                            BTIFormat::I8 => {
                                let mut pixels = Vec::<Color32>::new();
                                pixels.reserve_exact(height * width);
                                const BLOCK_WIDTH: usize = 8; // pixels
                                const BLOCK_HEIGHT: usize = 4; // pixels
                                type Block = [[Color32; BLOCK_WIDTH]; BLOCK_HEIGHT];

                                let num_blocks_w = width.div_ceil(BLOCK_WIDTH);
                                let num_blocks_h = height.div_ceil(BLOCK_HEIGHT);
                                let num_blocks = num_blocks_w * num_blocks_h;
                                let mut blocks = Vec::<Block>::new();

                                let mut reader = Cursor::new(data_bytes);
                                // Read Blocks
                                for _ in 0..num_blocks {
                                    let mut block: Block = Block::default();
                                    // Read one block and append before moving
                                    for r in 0..BLOCK_HEIGHT {
                                        for c in 0..BLOCK_WIDTH {
                                            let byte = reader.read_u8().unwrap();
                                            block[r][c] = Color32::from_gray(byte);
                                        }
                                    }
                                    blocks.push(block);
                                }
                                // Reorder Blocks into Vec
                                for r in 0..height {
                                    for c in 0..width {
                                        let block_idx = (c / (BLOCK_WIDTH))
                                            + (r / (BLOCK_HEIGHT)) * num_blocks_w;
                                        pixels.push(
                                            blocks[block_idx][r % BLOCK_HEIGHT][c % BLOCK_WIDTH],
                                        );
                                    }
                                }
                                assert_eq!(pixels.len(), height * width);
                                pixels.clone()
                            }, // 92
                            BTIFormat::IA4 => {
                                let mut pixels = Vec::<Color32>::new();
                                pixels.reserve_exact(height * width);
                                const BLOCK_WIDTH: usize = 8; // pixels
                                const BLOCK_HEIGHT: usize = 4; // pixels
                                type Block = [[Color32; BLOCK_WIDTH]; BLOCK_HEIGHT];

                                let num_blocks_w = width.div_ceil(BLOCK_WIDTH);
                                let num_blocks_h = height.div_ceil(BLOCK_HEIGHT);
                                let num_blocks = num_blocks_w * num_blocks_h;
                                let mut blocks = Vec::<Block>::new();

                                let mut reader = Cursor::new(data_bytes);
                                // Read Blocks
                                for _ in 0..num_blocks {
                                    let mut block: Block = Block::default();
                                    // Read one block and append before moving
                                    for r in 0..BLOCK_HEIGHT {
                                        for c in 0..BLOCK_WIDTH {
                                            let byte = reader.read_u8().unwrap();
                                            let gray = ((byte >> 0) & 0xF) * 0x11;
                                            let alpha = ((byte >> 4) & 0xF) * 0x11;
                                            block[r][c] = Color32::from_rgba_premultiplied(
                                                gray, gray, gray, alpha,
                                            );
                                        }
                                    }
                                    blocks.push(block);
                                }
                                // Reorder Blocks into Vec
                                for r in 0..height {
                                    for c in 0..width {
                                        let block_idx = (c / (BLOCK_WIDTH))
                                            + (r / (BLOCK_HEIGHT)) * num_blocks_w;
                                        pixels.push(
                                            blocks[block_idx][r % BLOCK_HEIGHT][c % BLOCK_WIDTH],
                                        );
                                    }
                                }
                                assert_eq!(pixels.len(), height * width);
                                pixels.clone()
                            }, // 52
                            BTIFormat::IA8 => {
                                let mut pixels = Vec::<Color32>::new();
                                pixels.reserve_exact(height * width);
                                const BLOCK_WIDTH: usize = 4; // pixels
                                const BLOCK_HEIGHT: usize = 4; // pixels
                                type Block = [[Color32; BLOCK_WIDTH]; BLOCK_HEIGHT];

                                let num_blocks_w = width.div_ceil(BLOCK_WIDTH);
                                let num_blocks_h = height.div_ceil(BLOCK_HEIGHT);
                                let num_blocks = num_blocks_w * num_blocks_h;
                                let mut blocks = Vec::<Block>::new();

                                let mut reader = Cursor::new(data_bytes);
                                // Read Blocks
                                for _ in 0..num_blocks {
                                    let mut block: Block = Block::default();
                                    // Read one block and append before moving
                                    for r in 0..BLOCK_HEIGHT {
                                        for c in 0..BLOCK_WIDTH {
                                            let alpha = reader.read_u8().unwrap();
                                            let gray = reader.read_u8().unwrap();
                                            block[r][c] = Color32::from_rgba_unmultiplied(
                                                gray, gray, gray, alpha,
                                            );
                                        }
                                    }
                                    blocks.push(block);
                                }
                                // Reorder Blocks into Vec
                                for r in 0..height {
                                    for c in 0..width {
                                        let block_idx = (c / (BLOCK_WIDTH))
                                            + (r / (BLOCK_HEIGHT)) * num_blocks_w;
                                        pixels.push(
                                            blocks[block_idx][r % BLOCK_HEIGHT][c % BLOCK_WIDTH],
                                        );
                                    }
                                }
                                assert_eq!(pixels.len(), height * width);
                                pixels.clone()
                            }, // 103
                            BTIFormat::RGB565 => {
                                let mut pixels = Vec::<Color32>::new();
                                pixels.reserve_exact(height * width);
                                const BLOCK_WIDTH: usize = 4; // pixels
                                const BLOCK_HEIGHT: usize = 4; // pixels
                                type Block = [[Color32; BLOCK_WIDTH]; BLOCK_HEIGHT];

                                let num_blocks_w = width.div_ceil(BLOCK_WIDTH);
                                let num_blocks_h = height.div_ceil(BLOCK_HEIGHT);
                                let num_blocks = num_blocks_w * num_blocks_h;
                                let mut blocks = Vec::<Block>::new();

                                let mut reader = Cursor::new(data_bytes);
                                // Read Blocks
                                for _ in 0..num_blocks {
                                    let mut block: Block = Block::default();
                                    // Read one block and append before moving
                                    for r in 0..BLOCK_HEIGHT {
                                        for c in 0..BLOCK_WIDTH {
                                            let data = reader.read_u16::<BigEndian>().unwrap();
                                            let red = (((data >> 11) & 0x1F) * 8) as u8;
                                            let green = (((data >> 5) & 0x3F) * 4) as u8;
                                            let blue = (((data >> 0) & 0x1F) * 8) as u8;
                                            block[r][c] = Color32::from_rgba_unmultiplied(
                                                red, green, blue, 0xFF,
                                            );
                                        }
                                    }
                                    blocks.push(block);
                                }
                                // Reorder Blocks into Vec
                                for r in 0..height {
                                    for c in 0..width {
                                        let block_idx = (c / (BLOCK_WIDTH))
                                            + (r / (BLOCK_HEIGHT)) * num_blocks_w;
                                        pixels.push(
                                            blocks[block_idx][r % BLOCK_HEIGHT][c % BLOCK_WIDTH],
                                        );
                                    }
                                }
                                assert_eq!(pixels.len(), height * width);
                                pixels.clone()
                            }, // 7
                            BTIFormat::RGB5A3 => {
                                let mut pixels = Vec::<Color32>::new();
                                pixels.reserve_exact(height * width);
                                const BLOCK_WIDTH: usize = 4; // pixels
                                const BLOCK_HEIGHT: usize = 4; // pixels
                                type Block = [[Color32; BLOCK_WIDTH]; BLOCK_HEIGHT];

                                let num_blocks_w = width.div_ceil(BLOCK_WIDTH);
                                let num_blocks_h = height.div_ceil(BLOCK_HEIGHT);
                                let num_blocks = num_blocks_w * num_blocks_h;
                                let mut blocks = Vec::<Block>::new();

                                let mut reader = Cursor::new(data_bytes);
                                // Read Blocks
                                for _ in 0..num_blocks {
                                    let mut block: Block = Block::default();
                                    // Read one block and append before moving
                                    for r in 0..BLOCK_HEIGHT {
                                        for c in 0..BLOCK_WIDTH {
                                            let data = reader.read_u16::<BigEndian>().unwrap();
                                            if (data & 0x8000) != 0 {
                                                let red = (((data >> 10) & 0x1F) * 8) as u8;
                                                let green = (((data >> 5) & 0x1F) * 8) as u8;
                                                let blue = (((data >> 0) & 0x1F) * 8) as u8;
                                                block[r][c] = Color32::from_rgba_unmultiplied(
                                                    red, green, blue, 0xFF,
                                                );
                                            } else {
                                                let alpha = (((data >> 12) & 0x7) * 32) as u8;
                                                let red = (((data >> 8) & 0xF) * 17) as u8;
                                                let green = (((data >> 4) & 0xF) * 17) as u8;
                                                let blue = (((data >> 0) & 0xF) * 17) as u8;
                                                block[r][c] = Color32::from_rgba_unmultiplied(
                                                    red, green, blue, alpha,
                                                );
                                            }
                                        }
                                    }
                                    blocks.push(block);
                                }
                                // Reorder Blocks into Vec
                                for r in 0..height {
                                    for c in 0..width {
                                        let block_idx = (c / (BLOCK_WIDTH))
                                            + (r / (BLOCK_HEIGHT)) * num_blocks_w;
                                        pixels.push(
                                            blocks[block_idx][r % BLOCK_HEIGHT][c % BLOCK_WIDTH],
                                        );
                                    }
                                }
                                assert_eq!(pixels.len(), height * width);
                                pixels.clone()
                            }, // 23
                            BTIFormat::CMPR => {
                                vec![Color32::from_hex("#ffffffff").unwrap(); width * height]
                            }, // 22
                            BTIFormat::RGBA32
                            | BTIFormat::C4
                            | BTIFormat::C8
                            | BTIFormat::C14X2 => {
                                println!("Format [{:?}] not yet supported", self.header.format);
                                vec![Color32::from_hex("#ffffffff").unwrap(); width * height]
                            },
                        },
                    }
                    .into(),
                ),
                Default::default(),
            ),
        );
    }
}

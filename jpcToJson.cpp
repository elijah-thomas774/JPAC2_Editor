#include "jpcToJson.hpp"

using namespace std;

JPA_BEM1 read_BEM1_JPAC2_10(Buffer& block);
JPA_BSP1 read_BSP1_JPAC2_10(Buffer& block){
    string magic = block.read_string(0, 4); 
    u32 size = block.read_u32(4);
    assert(magic.compare("BSP1") == 0);
    assert(size == block.data.size());
    JPA_BSP1 bsp1;
    bsp1.flags = block.read_u32(0x08);
    bsp1.texFlags = block.read_u8(0x1E);
    bsp1.colorFlags = block.read_u8(0x21);
    bsp1.parse_flags_JPAC2_10();
    bsp1.baseSize = block.read_vec2f(0x10);
    bsp1.blendModeFlags = block.read_u16(0x18);
    bsp1.alphaCompareFlags = block.read_u8(0x1A);
    bsp1.alphaRef0 = block.read_u8(0x1B);
    bsp1.alphaRef1 = block.read_u8(0x1C);
    bsp1.zModeFlags = block.read_u8(0x1D);
    bsp1.anmRndm = block.read_u8(0x2E);
    // Texture Palette Animation
    bsp1.texIdx = block.read_u8(0x20);
    bsp1.texIdxLoopOfstMask = block.read_u8(0x30);
    u8 texIdxAnmCount = block.read_u8(0x1F);
    bsp1.texInitTransX = 0;
    bsp1.texInitTransY = 0;
    bsp1.texInitScaleX = 0;
    bsp1.texInitScaleY = 0;
    bsp1.texInitRot = 0;
    bsp1.texIncTransX = 0;
    bsp1.texIncTransY = 0;
    bsp1.texIncScaleX = 0;
    bsp1.texIncScaleY = 0;
    bsp1.texIncRot = 0;
    i32 extraOff = 0x34;
    if (bsp1.isEnableTexScrollAnm)
    {
        bsp1.texInitTransX = block.read_f32(extraOff + 0x00);
        bsp1.texInitTransY = block.read_f32(extraOff + 0x04);
        bsp1.texInitScaleX = block.read_f32(extraOff + 0x08);
        bsp1.texInitScaleY = block.read_f32(extraOff + 0x0C);
        bsp1.texInitRot    = block.read_f32(extraOff + 0x10);
        bsp1.texIncTransX  = block.read_f32(extraOff + 0x14);
        bsp1.texIncTransY  = block.read_f32(extraOff + 0x18);
        bsp1.texIncScaleX  = block.read_f32(extraOff + 0x1C);
        bsp1.texIncScaleY  = block.read_f32(extraOff + 0x20);
        bsp1.texIncRot     = block.read_f32(extraOff + 0x24);
        extraOff += 0x28;
    }
    if (!!(bsp1.texFlags & 0x1))
        bsp1.texIdxAnimData = block.read_vec_u8(extraOff, texIdxAnmCount);
    // Color Animation Settings
    bsp1.colorLoopOfstMask = block.read_u8(0x2F);
    bsp1.colorPrm.color_string = block.read_u32(0x26);
    bsp1.colorEnv.color_string = block.read_u32(0x2A);
    bsp1.colorAnimMaxFrm = block.read_u16(0x24);
    if (!!(bsp1.colorFlags & 0x02))
    {
        u16 colorPrmDataOffs = block.read_u16(0xC);
        u8 colorPrmDataCount = block.read_u8(0x22);
        for(int i = 0; i < colorPrmDataCount; i++)
        {
            u16 frameTime;
            Color color;
            frameTime = block.read_u16(colorPrmDataOffs + 6*i);
            color.color_string = block.read_u32(colorPrmDataOffs + 6*i + 2);
            bsp1.colorPrmAnimData.push_back({frameTime, color});
        }
    }   
    if (!!(bsp1.colorFlags & 0x08))
    {
        u16 colorEnvDataOffs = block.read_u16(0xE);
        u8 colorEnvDataCount = block.read_u8(0x23); 
        for(int i = 0; i < colorEnvDataCount; i++)
        {
            u16 frameTime;
            Color color;
            frameTime = block.read_u16(colorEnvDataOffs + 6*i);
            color.color_string = block.read_u32(colorEnvDataOffs + 6*i + 2);
            bsp1.colorPrmAnimData.push_back({frameTime, color});
        }
    }
    return bsp1;
}
JPA_ESP1 read_ESP1_JPAC2_10(Buffer& block);
JPA_ETX1 read_ETX1_JPAC2_10(Buffer& block);
JPA_SSP1 read_SSP1_JPAC2_10(Buffer& block);
JPA_FLD1 read_FLD1_JPAC2_10(Buffer& block);
JPA_KFA1 read_KFA1_JPAC2_10(Buffer& block);
JPA_TDB1 read_TDB1_JPAC2_10(Buffer& block);

JPA_BEM1 read_BEM1_JPAC2_11(Buffer& block){
   return read_BEM1_JPAC2_10(block);
}
JPA_BSP1 read_BSP1_JPAC2_11(Buffer& block){
    string magic = block.read_string(0, 4); 
    u32 size = block.read_u32(4);
    assert(magic.compare("BSP1") == 0);
    assert(size == block.data.size());
    JPA_BSP1 bsp1;
    bsp1.flags = block.read_u32(0x08);
    bsp1.texFlags = block.read_u8(0x1E);
    bsp1.colorFlags = block.read_u8(0x21);
    bsp1.parse_flags_JPAC2_11();
    bsp1.baseSize = block.read_vec2f(0x10);
    bsp1.blendModeFlags = block.read_u16(0x18);
    bsp1.alphaCompareFlags = block.read_u8(0x1A);
    bsp1.alphaRef0 = block.read_u8(0x1B);
    bsp1.alphaRef1 = block.read_u8(0x1C);
    bsp1.zModeFlags = block.read_u8(0x1D);
    bsp1.anmRndm = block.read_u8(0x2E);
    // Texture Palette Animation
    bsp1.texIdx = block.read_u8(0x20);
    bsp1.texIdxLoopOfstMask = block.read_u8(0x30);
    u8 texIdxAnmCount = block.read_u8(0x1F);
    bsp1.texInitTransX = 0;
    bsp1.texInitTransY = 0;
    bsp1.texInitScaleX = 0;
    bsp1.texInitScaleY = 0;
    bsp1.texInitRot = 0;
    bsp1.texIncTransX = 0;
    bsp1.texIncTransY = 0;
    bsp1.texIncScaleX = 0;
    bsp1.texIncScaleY = 0;
    bsp1.texIncRot = 0;
    i32 extraOff = 0x34;
    if (bsp1.isEnableTexScrollAnm)
    {
        bsp1.texInitTransX = block.read_f32(extraOff + 0x00);
        bsp1.texInitTransY = block.read_f32(extraOff + 0x04);
        bsp1.texInitScaleX = block.read_f32(extraOff + 0x08);
        bsp1.texInitScaleY = block.read_f32(extraOff + 0x0C);
        bsp1.texInitRot    = block.read_f32(extraOff + 0x10);
        bsp1.texIncTransX  = block.read_f32(extraOff + 0x14);
        bsp1.texIncTransY  = block.read_f32(extraOff + 0x18);
        bsp1.texIncScaleX  = block.read_f32(extraOff + 0x1C);
        bsp1.texIncScaleY  = block.read_f32(extraOff + 0x20);
        bsp1.texIncRot     = block.read_f32(extraOff + 0x24);
        extraOff += 0x28;
    }
    if (!!(bsp1.texFlags & 0x1))
        bsp1.texIdxAnimData = block.read_vec_u8(extraOff, texIdxAnmCount);
    // Color Animation Settings
    bsp1.colorLoopOfstMask = block.read_u8(0x2F);
    bsp1.colorPrm.color_string = block.read_u32(0x26);
    bsp1.colorEnv.color_string = block.read_u32(0x2A);
    bsp1.colorAnimMaxFrm = block.read_u16(0x24);
    if (!!(bsp1.colorFlags & 0x02))
    {
        u16 colorPrmDataOffs = block.read_u16(0xC);
        u8 colorPrmDataCount = block.read_u8(0x22);
        for(int i = 0; i < colorPrmDataCount; i++)
        {
            u16 frameTime;
            Color color;
            frameTime = block.read_u16(colorPrmDataOffs + 6*i);
            color.color_string = block.read_u32(colorPrmDataOffs + 6*i + 2);
            bsp1.colorPrmAnimData.push_back({frameTime, color});
        }
    }   
    if (!!(bsp1.colorFlags & 0x08))
    {
        u16 colorEnvDataOffs = block.read_u16(0xE);
        u8 colorEnvDataCount = block.read_u8(0x23); 
        for(int i = 0; i < colorEnvDataCount; i++)
        {
            u16 frameTime;
            Color color;
            frameTime = block.read_u16(colorEnvDataOffs + 6*i);
            color.color_string = block.read_u32(colorEnvDataOffs + 6*i + 2);
            bsp1.colorPrmAnimData.push_back({frameTime, color});
        }
    }
    return bsp1;
}
JPA_ESP1 read_ESP1_JPAC2_11(Buffer& block){

}
JPA_ETX1 read_ETX1_JPAC2_11(Buffer& block){

}
JPA_SSP1 read_SSP1_JPAC2_11(Buffer& block){

}
JPA_FLD1 read_FLD1_JPAC2_11(Buffer& block){

}
JPA_KFA1 read_KFA1_JPAC2_11(Buffer& block){

}
JPA_TDB1 read_TDB1_JPAC2_11(Buffer& block){

}

JPA_Resource read_resouce_JPAC2_10(Buffer& rawRes);
JPA_Resource read_resouce_JPAC2_11(Buffer& rawRes);

JPA_Texture read_texture(Buffer& rawTexure);

JPAC read_JPAC(Buffer& jpc);

void read_jpc(std::string &in_file, std::string out_file, std::string texture_folder);
void write_json(std::string &out_file, JPAC &jpc);
void dump_textures(std::string &texture_folder, JPA_Texture &textures);
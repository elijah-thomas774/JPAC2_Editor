#include "jpcToJson.hpp"

using namespace std;

JPA_BEM1 read_BEM1_JPAC2_10(Buffer& block)
{    
    string magic = block.read_string(0, 4); 
    u32 size = block.read_u32(4);
    assert(magic.compare("BEM1")==0);
    assert(block.data.size() == 0x7C);

    JPA_BEM1 bem1;
    bem1.emitFlags        = block.read_u32(0x08);
    bem1.parse_flags();
    bem1.unkFlags         = block.read_u32(0x0C);
    bem1.emitterScl       = block.read_vec3f(0x10);
    bem1.emitterTrs       = block.read_vec3f(0x1C);
    bem1.emitterDir       = block.read_vec3f(0x28);
    bem1.initialVelOmni   = block.read_f32(0x34);
    bem1.initialVelAxis   = block.read_f32(0x38);
    bem1.initialVelRndm   = block.read_f32(0x3C);
    bem1.initialVelDir    = block.read_f32(0x40); 
    bem1.spread           = block.read_f32(0x44);
    bem1.initialVelRatio  = block.read_f32(0x48);
    bem1.rate             = block.read_f32(0x4C);   
    bem1.rateRndm         = block.read_f32(0x50);
    bem1.lifeTimeRndm     = block.read_f32(0x54);
    bem1.volumeSweep      = block.read_f32(0x58);
    bem1.volumeMinRad     = block.read_f32(0x5C);
    bem1.airResist        = block.read_f32(0x60);
    bem1.momentRndm       = block.read_f32(0x64);
    bem1.emitterRot       = block.read_vec_i16(0x68, 3);
    bem1.maxFrame         = block.read_i16(0x6E);
    bem1.startFrame       = block.read_i16(0x70);
    bem1.lifeTime         = block.read_i16(0x72);
    bem1.volumeSize       = block.read_i16(0x74);
    bem1.divNumber        = block.read_i16(0x76);
    bem1.rateStep         = block.read_u8(0x78);
    return bem1;
}
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
            bsp1.colorEnvAnimData.push_back({frameTime, color});
        }
    }
    return bsp1;
}
JPA_ESP1 read_ESP1_JPAC2_10(Buffer& block){
    JPA_ESP1 esp1;
    esp1.flags              = block.read_u32(0x08);
    esp1.parse_flags();
    esp1.scaleInTiming      = block.read_f32(0xC);
    esp1.scaleOutTiming     = block.read_f32(0x10);
    esp1.scaleInValueX      = block.read_f32(0x14);
    esp1.scaleOutValueX     = block.read_f32(0x18);
    esp1.scaleInValueY      = block.read_f32(0x1C);
    esp1.scaleOutValueY     = block.read_f32(0x20);
    esp1.scaleOutRandom     = block.read_f32(0x24);
    esp1.scaleAnmMaxFrameX  = block.read_u16(0x28);
    esp1.scaleAnmMaxFrameY  = block.read_u16(0x2A);
    esp1.alphaInTiming      = block.read_f32(0x2C);
    esp1.alphaOutTiming     = block.read_f32(0x30);
    esp1.alphaInValue       = block.read_f32(0x34);
    esp1.alphaBaseValue     = block.read_f32(0x38);
    esp1.alphaOutValue      = block.read_f32(0x3C);
    esp1.alphaWaveRandom    = block.read_f32(0x44);
    esp1.alphaWaveFrequency = block.read_f32(0x40);
    esp1.alphaWaveAmplitude = block.read_f32(0x48);
    esp1.rotateAngle        = block.read_f32(0x4C);
    esp1.rotateAngleRandom  = block.read_f32(0x50);
    esp1.rotateSpeed        = block.read_f32(0x54);
    esp1.rotateSpeedRandom  = block.read_f32(0x58);
    esp1.rotateDirection    = block.read_f32(0x5C);
    return esp1;
}
JPA_ETX1 read_ETX1_JPAC2_10(Buffer& block){
    string magic = block.read_string(0, 4); 
    u32 size = block.read_u32(4);
    assert(magic.compare("ETX1") == 0);
    assert(0x28 == block.data.size());
    JPA_ETX1 etx1;
    u32 flags            = block.read_u32(0x8);        
    etx1.indTextureMode  = (flags & 0x1);
    etx1.floats          = block.read_vec_f32(0xC, 6);
    etx1.unk             = 0;
    etx1.indTextureID    = block.read_u8(0x25);
    etx1.scale           = block.read_i8(0x24);
    etx1.secondTextureID = !!(flags & 0x100) ? block.read_u8(0x26) : -1;
    return etx1;
}
JPA_SSP1 read_SSP1_JPAC2_10(Buffer& block){
    string magic = block.read_string(0, 4); 
    u32 size = block.read_u32(4);
    assert(magic.compare("SSP1") == 0);
    assert(0x48 == block.data.size());
    JPA_SSP1 ssp1;
    ssp1.flags          = block.read_u32(0x08);
    ssp1.parse_flags();
    ssp1.posRndm        = block.read_f32(0xC);
    ssp1.baseVel        = block.read_f32(0x10);
    ssp1.baseVelRndm    = block.read_f32(0x14);
    ssp1.velInfRate     = block.read_f32(0x18);
    ssp1.gravity        = block.read_f32(0x1C);
    ssp1.globalScale2D  = block.read_vec_f32(0x20, 2);
    ssp1.inheritScale   = block.read_f32(0x28);
    ssp1.inheritAlpha   = block.read_f32(0x2C);
    ssp1.inheritRGB     = block.read_f32(0x30);
    ssp1.colorPrm.color_string = block.read_u32(0x34);
    ssp1.colorEnv.color_string = block.read_u32(0x38);
    ssp1.timing         = block.read_f32(0x3C);
    ssp1.life           = block.read_u16(0x40);
    ssp1.rate           = block.read_u16(0x42);
    ssp1.step           = block.read_u8(0x44);
    ssp1.texIdx         = block.read_u8(0x45);
    ssp1.rotateSpeed    = block.read_u16(0x46);
    return ssp1;
}
JPA_FLD1 read_FLD1_JPAC2_10(Buffer& block){
    string magic = block.read_string(0, 4); 
    u32 size = block.read_u32(4);
    assert(magic.compare("FLD1") == 0);
    assert(0x44 == block.data.size());

    JPA_FLD1 fld1;
    fld1.flags   = block.read_u32(0x8);
    fld1.parse_flags();
    fld1.pos     = block.read_vec_f32(0x0C, 3);
    fld1.dis     = block.read_vec_f32(0x18, 3);
    fld1.param1  = block.read_f32(0x24);
    fld1.param2  = block.read_f32(0x28);
    fld1.param3  = block.read_f32(0x2C);
    fld1.fadeIn  = block.read_f32(0x30);
    fld1.fadeOut = block.read_f32(0x34);
    fld1.enTime  = block.read_f32(0x38);
    fld1.disTime = block.read_f32(0x3C);
    fld1.cycle   = block.read_u8(0x40);
    return fld1;
}
JPA_KFA1 read_KFA1_JPAC2_10(Buffer& block){
    string magic = block.read_string(0, 4); 
    u32 size = block.read_u32(4);
    assert(magic.compare("KFA1") == 0);
    assert(size == block.data.size());
    JPA_KFA1 kfa1;
    kfa1.keyType        =    block.read_u8(0x08);
    kfa1.keyCount       =    block.read_u8(0x09);
    kfa1.unk0xA         =    block.read_u8(0xA);
    kfa1.isLoopEnable   = !!(block.read_u8(0x0B));
    kfa1.keyValues      =    block.read_vec_f32(0x0C, kfa1.keyCount*4);
    return kfa1;
}
JPA_TDB1 read_TDB1_JPAC2_10(Buffer& block, u32 num){
    string magic = block.read_string(0, 4); 
    u32 size = block.read_u32(4);
    assert(magic.compare("TDB1") == 0);
    assert(size == block.data.size());
    JPA_TDB1 tdb1;
    tdb1.textureIdx = block.read_vec_u16(0x8, num);
    return tdb1;
}

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
            bsp1.colorEnvAnimData.push_back({frameTime, color});
        }
    }
    return bsp1;
}
JPA_ESP1 read_ESP1_JPAC2_11(Buffer& block){
    return read_ESP1_JPAC2_10(block);
}
JPA_ETX1 read_ETX1_JPAC2_11(Buffer& block){
    string magic = block.read_string(0, 4); 
    u32 size = block.read_u32(4);
    assert(magic.compare("ETX1") == 0);
    assert(0x50 == block.data.size());

    JPA_ETX1 etx1;
    u32 flags            = block.read_u32(0x8);        
    etx1.indTextureMode  = (flags & 0x1);
    etx1.floats          = block.read_vec_f32(0xC, 16);
    etx1.unk             = block.read_u8(0x4C);
    etx1.indTextureID    = block.read_u8(0x4E);
    etx1.scale           = block.read_i8(0x4D);
    etx1.secondTextureID = !!(flags & 0x100) ? block.read_u8(0x4F) : -1;
    return etx1;
}
JPA_SSP1 read_SSP1_JPAC2_11(Buffer& block){
    return read_SSP1_JPAC2_10(block);
}
JPA_FLD1 read_FLD1_JPAC2_11(Buffer& block){
    return read_FLD1_JPAC2_10(block);
}
JPA_KFA1 read_KFA1_JPAC2_11(Buffer& block){
    return read_KFA1_JPAC2_10(block);
}
JPA_TDB1 read_TDB1_JPAC2_11(Buffer& block, u32 num){
    return read_TDB1_JPAC2_10(block, num);
}

JPA_Resource read_resouce_JPAC2_11(Buffer& rawRes){
    JPA_Resource resource;
    resource.resourceId = rawRes.read_u16(0x0);
    resource.blockCount = rawRes.read_u16(0x2);
    resource.fieldBlockCount = rawRes.read_u8(0x4);
    resource.keyBlockCount = rawRes.read_u8(0x5);
    resource.tdb1Count = rawRes.read_u8(0x6);
    u32 currOffset = 0x8;
    for (int i = 0; i < resource.blockCount; i++)
    {
        string magic = rawRes.read_string(currOffset, 4);
        u32 blockSize = rawRes.read_u32(currOffset+0x04);
        Buffer block = rawRes.read_slice_amount(currOffset, blockSize);
        if (magic.compare("BEM1") == 0)
            resource.bem1.push_back(read_BEM1_JPAC2_11(block));
        else if (magic.compare("BSP1") == 0)
            resource.bsp1.push_back(read_BSP1_JPAC2_11(block));
        else if (magic.compare("ESP1") == 0)
            resource.esp1.push_back(read_ESP1_JPAC2_11(block));
        else if (magic.compare("ETX1") == 0)
            resource.etx1.push_back(read_ETX1_JPAC2_11(block));
        else if (magic.compare("SSP1") == 0)
            resource.ssp1.push_back(read_SSP1_JPAC2_11(block));
        else if (magic.compare("FLD1") == 0)
            resource.fld1.push_back(read_FLD1_JPAC2_11(block));
        else if (magic.compare("KFA1") == 0)
            resource.kfa1.push_back(read_KFA1_JPAC2_11(block));
        else if (magic.compare("TDB1") == 0)
            resource.tdb1.push_back(read_TDB1_JPAC2_11(block, resource.tdb1Count));
        else
            assert(!"Unknown Block Type");
        currOffset += blockSize;
    }
    return resource;
}
JPA_Resource read_resouce_JPAC2_10(Buffer& rawRes){
    JPA_Resource resource;
    resource.resourceId = rawRes.read_u16(0x0);
    resource.blockCount = rawRes.read_u16(0x2);
    resource.fieldBlockCount = rawRes.read_u8(0x4);
    resource.keyBlockCount = rawRes.read_u8(0x5);
    resource.tdb1Count = rawRes.read_u8(0x6);
    u32 currOffset = 0x8;
    for (i32 k = 0; k < resource.blockCount; k+=1)
    {
        string magic = rawRes.read_string(currOffset, 4);
        u32 blockSize = rawRes.read_u32(currOffset+0x04);
        Buffer block = rawRes.read_slice_amount(currOffset, blockSize);
        if (magic.compare("BEM1") == 0)
            resource.bem1.push_back(read_BEM1_JPAC2_10(block));
        else if (magic.compare("BSP1") == 0)
            resource.bsp1.push_back(read_BSP1_JPAC2_10(block));
        else if (magic.compare("ESP1") == 0)
            resource.esp1.push_back(read_ESP1_JPAC2_10(block));
        else if (magic.compare("ETX1") == 0)
            resource.etx1.push_back(read_ETX1_JPAC2_10(block));
        else if (magic.compare("SSP1") == 0)
            resource.ssp1.push_back(read_SSP1_JPAC2_10(block));
        else if (magic.compare("FLD1") == 0)
            resource.fld1.push_back(read_FLD1_JPAC2_10(block));
        else if (magic.compare("KFA1") == 0)
            resource.kfa1.push_back(read_KFA1_JPAC2_10(block));
        else if (magic.compare("TDB1") == 0)
            resource.tdb1.push_back(read_TDB1_JPAC2_10(block, resource.tdb1Count));
        else
            assert(!"Unknown Block Type");
        currOffset += blockSize;
    }
    return resource;
}

JPA_Texture read_texture(Buffer& rawTexure){
    string magic = rawTexure.read_string(0, 4); 
    u32 size = rawTexure.read_u32(4);
    assert(magic.compare("TEX1") == 0);
    assert(size == rawTexure.data.size());    
    JPA_Texture texture;
    texture.name = rawTexure.read_string(12, 20);
    texture.data = rawTexure.read_slice(0x20, size).data;
    return texture;
}

JPAC read_JPAC(Buffer& rawJpc){
    JPAC jpc;
    jpc.version = rawJpc.read_string(0, 8);
    jpc.resource_count = rawJpc.read_u16(0x8);
    jpc.texture_count = rawJpc.read_u16(0xA);
    jpc.texture_offset = rawJpc.read_u32(0xC);
    u32 currOffset = 0x10;
    for (i32 i = 0; i < jpc.resource_count; i++)
    {
        u32 resStart = currOffset;
        u16 blockCnt = rawJpc.read_u16(resStart+0x2);
        currOffset += 8;
        for(i32 j = 0; j < blockCnt; j++)
            currOffset+=rawJpc.read_u32(currOffset+0x4);
        Buffer resource = rawJpc.read_slice(resStart, currOffset);
        if (jpc.version.compare("JPAC2-10") == 0)
            jpc.resources.push_back(read_resouce_JPAC2_10(resource));
        else if (jpc.version.compare("JPAC2-11") == 0)
            jpc.resources.push_back(read_resouce_JPAC2_11(resource));
    }
    currOffset = jpc.texture_offset;
    for(i32 i = 0; i < jpc.texture_count; i++)
    {
        u32 textureStart = currOffset;
        currOffset += rawJpc.read_u32(textureStart+0x4); // size
        Buffer tempp = rawJpc.read_slice(textureStart, currOffset);
        jpc.textures.push_back(read_texture(tempp));        
    }
    for (auto& resource : jpc.resources)
        resource.tdb1.at(0).map_to_texture(jpc.textures);
    return jpc;
}

void read_jpc(std::string in_file, std::string out_file, std::string texture_dump_folder){
    Buffer data(in_file); // reads in the file
    
    JPAC jpc = read_JPAC(data);
    write_json(out_file, jpc);
    if (texture_dump_folder != "")
        dump_textures(texture_dump_folder, jpc.textures);
}
JPAC read_jpc(std::string in_file)
{
    Buffer data(in_file); // reads in the file
    string version = data.read_string(0, 8);
    JPAC jpc;
    if (version.compare("JPAC2-10") != 0 && version.compare("JPAC2-11") != 0){
        std::cout << "Version is not compatible. Expects JPAC2-10 or JPAC2-11, but got " << version << std::endl;
        jpc.version = "BAD";
        return jpc;
    }
    jpc = read_JPAC(data);
    return jpc;
}
void write_json(std::string &out_file, JPAC &jpc){
    ordered_json j = to_json(jpc);
    std::ofstream dest;
    dest.open(out_file, std::ios::binary | std::ios::out);
    dest << std::setw(4) << j;
    dest.close();
}
void dump_textures(std::string &texture_folder, std::vector<JPA_Texture> &textures){
    for (auto& texture : textures)
    {
        std::string textureName = texture_folder;
        textureName.append(texture.name);
        textureName.append(".bti");
        std::ofstream texFile(textureName, std::ios::binary | std::ios::out);
        if(texFile.is_open())
        {
            texFile.write((char*)&texture.data.at(0), texture.data.size());
        }
        else{
            std::cout << "Could not open texture folder" << std::endl;
            break;
        }
        texFile.close();
    }
}
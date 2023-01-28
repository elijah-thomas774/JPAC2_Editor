#include "jpc.hpp"
#include <assert.h>
#include <iostream>
void JPA_BEM1::parse_flags(){
    volumeType = (emitFlags >> 8) & 0x7;
    unkEmitFlags = (emitFlags & 0x1F);
}
void JPA_BEM1::build_flags(){
    emitFlags = 0;
    emitFlags |= (volumeType & 0x7)<<8;
    emitFlags |= (unkEmitFlags & 0x1F);
}

void JPA_BSP1::parse_flags_JPAC2_10(){
    shapeType =              (flags & 0xF);        // 0x0000000F -
    dirType =                (flags >>  4) & 0x7;  // 0x0000007F -
    rotType =                (flags >>  7) & 0x7;  // 0x000003FF -
    planeType =              (flags >> 10) & 0x1;  // 0x000007FF -
    tilingS =              !!(flags & 0x02000000); // 0x020007FF
    tilingT =              !!(flags & 0x04000000); // 0x060007FF
    isNoDrawParent  =      !!(flags & 0x08000000); // 0x0E0007FF
    isNoDrawChild   =      !!(flags & 0x10000000); // 0x1E0007FF
    colorInSelect =          (flags >> 15) & 0x07; // 0x1E0387FF
    alphaInSelect =        !!(flags & 0x00040000); // 0x1E0787FF
    isEnableTexScrollAnm = !!(flags & 0x01000000); // 0x1F0787FF
    isDrawFwdAhead  =      !!(flags & 0x00200000); // 0x1F2787FF
    isDrawPrntAhead =      !!(flags & 0x00400000); // 0x1F6787FF
    isEnableProjection   = !!(flags & 0x00100000); // 0x1F7787FF
    isGlblTexAnm  =        !!(flags & 0x00004000); // 0x1F77C7FF
    isGlblClrAnm  =        !!(flags & 0x00001000); // 0x1F77D7FF
    unkVar1       =        !!(flags & 0x00000800); // unkVar
    unkVar2       =        !!(flags & 0x00002000); // unkVar
    unkVar3       =        !!(flags & 0x00080000); // unkVar
    unkVar4       =        !!(flags & 0x00800000); // unkVar
    unkVar5       =        0;                      // unused for JPAC2_10
    
    texCalcIdxType = (texFlags >> 2) & 0x7; 

    colorCalcIdxType = (colorFlags >> 4) & 0x7;
    unkColorFlag1 = !!(colorFlags & 4);
    unkColorFlag2 = !!(colorFlags & 1);
}
void JPA_BSP1::parse_flags_JPAC2_11(){
    shapeType =              (flags & 0xF);        // 0x0000000F
    dirType =                (flags >>  4) & 0x7;  // 0x0000007F
    rotType =                (flags >>  7) & 0x7;  // 0x000003FF
    planeType =              (flags >> 10) & 0x1;  // 0x000007FF
    tilingS =              !!(flags & 0x08000000); // 0x080007FF
    tilingT =              !!(flags & 0x10000000); // 0x180007FF
    isNoDrawParent  =      !!(flags & 0x20000000); // 0x380007FF
    isNoDrawChild   =      !!(flags & 0x40000000); // 0x780007FF
    colorInSelect =          (flags >> 15) & 0x07; // 0x780387FF
    alphaInSelect =        !!(flags & 0x00040000); // 0x780787FF
    isEnableTexScrollAnm = !!(flags & 0x04000000); // 0x7C0787FF
    isDrawFwdAhead  =      !!(flags & 0x00800000); // 0x7C8787FF
    isDrawPrntAhead =      !!(flags & 0x01000000); // 0x7D8787FF
    isEnableProjection   = !!(flags & 0x00400000); // 0x7DC787FF
    isGlblTexAnm  =        !!(flags & 0x00004000); // 0x7DC7C7FF
    isGlblClrAnm  =        !!(flags & 0x00001000); // 0x7DC7D7FF
    unkVar1       =        !!(flags & 0x00000800); // unkVar
    unkVar2       =        !!(flags & 0x00002000); // unkVar
    unkVar3       =        !!(flags & 0x00080000); // unkVar
    unkVar4       =        !!(flags & 0x00100000); // unkVar
    unkVar5       =        !!(flags & 0x02000000); // unkVar

    texCalcIdxType = (texFlags >> 2) & 0x7; 

    colorCalcIdxType = (colorFlags >> 4) & 0x7;
    unkColorFlag1 = !!(colorFlags & 4);
    unkColorFlag2 = !!(colorFlags & 1);
}
void JPA_BSP1::build_flags_JPAC2_10(){
    flags = 0;
    flags |= (shapeType & 0xF);
    flags |= (dirType   & 0x7) << 4;
    flags |= tilingS                ? 0x02000000 : 0;
    flags |= tilingT                ? 0x04000000 : 0;
    flags |= isNoDrawParent         ? 0x08000000 : 0;
    flags |= isNoDrawChild          ? 0x10000000 : 0;
    flags |= (colorInSelect & 0x7) << 15;
    flags |= alphaInSelect          ? 0x00040000 : 0;
    flags |= isEnableTexScrollAnm   ? 0x01000000 : 0;
    flags |= isDrawFwdAhead         ? 0x00200000 : 0;
    flags |= isDrawPrntAhead        ? 0x00400000 : 0;
    flags |= isEnableProjection     ? 0x00100000 : 0;
    flags |= isGlblTexAnm           ? 0x00004000 : 0;
    flags |= isGlblClrAnm           ? 0x00001000 : 0;
    flags |= unkVar1                ? 0x00000800 : 0;
    flags |= unkVar2                ? 0x00002000 : 0;
    flags |= unkVar3                ? 0x00080000 : 0;
    flags |= unkVar4                ? 0x00800000 : 0;
    flags |= unkVar5                ? 0x00000000 : 0;

    texFlags = 0x2;
    texFlags |= (texIdxAnimData.size() != 0) ? 1 : 0;
    texFlags |= (texCalcIdxType & 0x7) << 2;

    colorFlags = 0;
    colorFlags |= colorPrmAnimData.size() != 0 ? 0x2 : 0;
    colorFlags |= colorEnvAnimData.size() != 0 ? 0x8 : 0;
    colorFlags |= (colorCalcIdxType & 0x7) << 4;
    colorFlags |= unkColorFlag1 ? 0x4 : 0;
    colorFlags |= unkColorFlag2 ? 0x1 : 0;
}
void JPA_BSP1::build_flags_JPAC2_11(){
    flags = 0;
    flags |= (shapeType & 0xF);
    flags |= (dirType   & 0x7) << 4;
    flags |= tilingS                ? 0x08000000 : 0;
    flags |= tilingT                ? 0x10000000 : 0;
    flags |= isNoDrawParent         ? 0x20000000 : 0;
    flags |= isNoDrawChild          ? 0x40000000 : 0;
    flags |= (colorInSelect & 0x7) << 15;
    flags |= alphaInSelect          ? 0x00040000 : 0;
    flags |= isEnableTexScrollAnm   ? 0x04000000 : 0;
    flags |= isDrawFwdAhead         ? 0x00800000 : 0;
    flags |= isDrawPrntAhead        ? 0x01000000 : 0;
    flags |= isEnableProjection     ? 0x00400000 : 0;
    flags |= isGlblTexAnm           ? 0x00004000 : 0;
    flags |= isGlblClrAnm           ? 0x00001000 : 0;
    flags |= unkVar1                ? 0x00000800 : 0;
    flags |= unkVar2                ? 0x00002000 : 0;
    flags |= unkVar3                ? 0x00080000 : 0;
    flags |= unkVar4                ? 0x00100000 : 0;
    flags |= unkVar5                ? 0x02000000 : 0;
    
    texFlags = 0x2;
    texFlags |= (texIdxAnimData.size() != 0) ? 1 : 0;
    texFlags |= (texCalcIdxType & 0x7) << 2;

    colorFlags = 0;
    colorFlags |= colorPrmAnimData.size() != 0 ? 0x2 : 0;
    colorFlags |= colorEnvAnimData.size() != 0 ? 0x8 : 0;
    colorFlags |= (colorCalcIdxType & 0x7) << 4;
    colorFlags |= unkColorFlag1 ? 0x4 : 0;
    colorFlags |= unkColorFlag2 ? 0x1 : 0;
}

void JPA_ESP1::parse_flags(){
    isEnableScale =   !!(flags & 0x1);          // 0x00000001
    isDiffXY =        !!(flags & 0x2);          // 0x00000003
    scaleAnmTypeX =     (flags >> 0x08) & 0x03; // 0x00000303
    scaleAnmTypeY =     (flags >> 0x0A) & 0x03; // 0x00000F03
    isEnableAlpha  =  !!(flags & 0x00010000);   // 0x00010F03
    isEnableSinWave = !!(flags & 0x00020000);   // 0x00030F03
    isEnableRotate =  !!(flags & 0x01000000);   // 0x01030F03
    pivotX =            (flags >> 0x0C) & 0x03; // 0x01033F03
    pivotY =            (flags >> 0x0E) & 0x03; // 0x0103FF03
    unkFlags  =         (flags >> 2) & 0x3;     // 0x0103FF0F
}
void JPA_ESP1::build_flags(){
    flags = 0;
    flags |= isEnableScale ? 0x1 : 0;
    flags |= isDiffXY ? 0x2 : 0;
    flags |= (scaleAnmTypeX & 0x3)<<8;
    flags |= (scaleAnmTypeY & 0x3)<<10;
    flags |= isEnableAlpha   ? 0x00010000 : 0;
    flags |= isEnableSinWave ? 0x00020000 : 0;
    flags |= isEnableRotate  ? 0x01000000 : 0;
    flags |= (pivotX & 0x3) << 12;
    flags |= (pivotY & 0x3) << 14;
    flags |= (unkFlags & 0x3) << 2;
}

void JPA_SSP1::parse_flags(){
    shapeType =           flags & 0xF;          // 0x0000000F
    dirType =            (flags >> 4) & 0x7;    // 0x0000007F
    rotType =            (flags >> 7) & 0x7;    // 0x000003FF
    planeType =          (flags >> 10) & 0x1;   // 0x000007FF
    isInheritedScale = !!(flags & 0x00010000);  // 0x000107FF
    isInheritedAlpha = !!(flags & 0x00020000);  // 0x000307FF
    isInheritedRGB =   !!(flags & 0x00040000);  // 0x000707FF
    isEnableField =    !!(flags & 0x00200000);  // 0x002707FF
    isEnableScaleOut = !!(flags & 0x00400000);  // 0x006707FF
    isEnableAlphaOut = !!(flags & 0x00800000);  // 0x00E707FF
    isEnableRotate =   !!(flags & 0x01000000);  // 0x01E707FF
    unkFlags1      =     (flags >> 19) & 0x3;   // 0x01FF07FF
}
void JPA_SSP1::build_flags(){
    flags = 0;
    flags |= (shapeType & 0xFF);
    flags |= (dirType & 0x7) << 4;
    flags |= (rotType & 0x7) << 7;
    flags |= (planeType & 0x1) << 10; 
    flags |= (isInheritedScale) ? 0x00010000 : 0;
    flags |= (isInheritedAlpha) ? 0x00020000 : 0;
    flags |= (isInheritedRGB)   ? 0x00040000 : 0;
    flags |= (isEnableField)    ? 0x00200000 : 0;
    flags |= (isEnableScaleOut) ? 0x00400000 : 0;
    flags |= (isEnableAlphaOut) ? 0x00800000 : 0;
    flags |= (isEnableRotate)   ? 0x01000000 : 0;
    flags |= (unkFlags1 & 0x3) << 19;

}

void JPA_FLD1::parse_flags(){
    sttFlag =  flags >> 16;       // 0xFFFF0000
    type    =  flags & 0xF;       // 0xFFFF000F
    addType = (flags >> 8) & 0x3; // 0xFFFF030F
}
void JPA_FLD1::build_flags(){
    flags = 0;
    flags |= (sttFlag & 0xFFFF)<<16;
    flags |= (type & 0xF);
    flags |= (addType & 0x3) << 8;
}

void JPA_TDB1::map_to_texture(std::vector<JPA_Texture> &textures){
    for (u16 idx : textureIdx)
    {
        this->textures.push_back(textures.at(idx).name);
    }
}
void JPA_TDB1::remap_index(std::vector<JPA_Texture> &textures){
    textureIdx = std::vector<u16>();
    for (std::string name : this->textures)
    {
        for (u16 i =  0; i < textures.size(); i++)
        {
            if (name.compare(textures.at(i).name) == 0){
                textureIdx.push_back(i);
                break;
            }
            if (i == textures.size()-1){
                std::cout << "Error with: " << name << std::endl;
                assert(!"Could Not Find Texture");    
            }
        }
    }
}

void JPA_Resource::update_resource_info(std::string version, std::vector<JPA_Texture> &textures){
    u16 blkCnt = 0;
    blockCount += bem1.size();
    blockCount += bsp1.size();
    blockCount += esp1.size();
    blockCount += etx1.size();
    blockCount += ssp1.size();
    blockCount += fld1.size();
    blockCount += kfa1.size();
    blockCount += tdb1.size();
    blockCount = blkCnt;
    fieldBlockCount = fld1.size();
    keyBlockCount = kfa1.size();
    tdb1.at(0).map_to_texture(textures);
    tdb1.at(0).remap_index(textures);
    tdb1Count = tdb1.at(0).textureIdx.size();
    if (bem1.size() != 0) bem1.at(0).build_flags();
    if (esp1.size() != 0) esp1.at(0).build_flags();
    if (ssp1.size() != 0) ssp1.at(0).build_flags();
    for (auto& fld : fld1) fld.build_flags();
    if (version.compare("JPAC2-10") == 0){
        if (bsp1.size() != 0)
            bsp1.at(0).build_flags_JPAC2_10();    
    }
    if (version.compare("JPAC2-11") == 0){
        if (bsp1.size() != 0)
            bsp1.at(0).build_flags_JPAC2_11();   
    }
}
void JPAC::add_texture(std::string &texture_name){
    // texture name is the file path
    // appends it to the list
    // then updates
}
void JPAC::append_textures(std::string &path){
    // add textures from folder
    // check matching names - replace
    // else add it
    update();
}
void JPAC::add_texture_data(std::string &path){
    
}
void JPAC::replace_texture(std::string &texture_name){
    // folder
}
i32  JPAC::get_resource_index(u16 id){
    for (i32 i = 0; i < resources.size(); i++)
    {
        if (resources.at(i).resourceId == id)
            return i;
    }
    std::cout << "Could Not Find Resource with ID: " << std::hex << id << std::endl;
    assert(!"Resource Id Error");
    return 0;
}
void JPAC::add_resource(JPA_Resource &resource){
    resources.push_back(resource);
    update();
}
void JPAC::update(){
    for (auto & res : resources)
        res.update_resource_info(version, textures);
    resource_count = resources.size();
    texture_count = textures.size();
    // texture offset will be generated upon resouce creation
}
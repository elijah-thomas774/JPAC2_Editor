#include "jpc.hpp"

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
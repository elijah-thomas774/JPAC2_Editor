#ifndef JPC_HPP
#define JPC_HPP

#include "common_types.hpp"

struct BTI_header {
   /* 0x00 */ u8  format;
   /* 0x01 */ u8  enAlpha;
   /* 0x02 */ u16 width;
   /* 0x04 */ u16 height;
   /* 0x06 */ u8  wrapS;
   /* 0x07 */ u8  wrapT;
   /* 0x08 */ u16 palletFormat;
   /* 0x0A */ u16 numPalletEntries;
   /* 0x0C */ u32 palletDataOffset;
   /* 0x10 */ u32 unk1;
   /* 0x14 */ u8  magnificationFilterType;
   /* 0x15 */ u8  minificationFilterType;
   /* 0x16 */ u16 unk2;
   /* 0x18 */ u8  totalNumberOfImages;
   /* 0x19 */ u8  unk3;
   /* 0x1A */ u16 unk4;
   /* 0x1C */ u32 imageDataOffset;
};

class JPA_BEM1 { 
    public:
    u32 emitFlags;
    u16 unkEmitFlags;
    u32 unkFlags;
    u8 volumeType;
    vec3f emitterScl;
    std::vector<i16> emitterRot; // in degrees
    vec3f emitterTrs;
    vec3f emitterDir;

    f32 initialVelOmni;
    f32 initialVelAxis;
    f32 initialVelRndm;
    f32 initialVelDir; 

    f32 spread;
    f32 initialVelRatio;
    f32 rate;
    f32 rateRndm;
    f32 lifeTimeRndm;
    f32 volumeSweep;
    f32 volumeMinRad;
    f32 airResist;
    f32 momentRndm;
    i16 maxFrame;
    i16 startFrame;
    i16 lifeTime;
    i16 volumeSize;
    i16 divNumber;
    u8 rateStep;

    void parse_flags();
    void build_flags();
    JPA_BEM1 copy();
};
class JPA_BSP1 {
    public:
    // u32 flags; // Controls some commented out options TODO
    u32 flags;
    bool unkVar1;
    bool unkVar2;
    bool unkVar3;
    bool unkVar4;
    bool unkVar5;
    u8 texFlags;
    u8 colorFlags;
    bool unkColorFlag1;
    bool unkColorFlag2;
    u8 shapeType;
    u8 dirType;
    u8 rotType;
    u8 planeType;
    std::vector<f32> baseSize;
    bool tilingS;
    bool tilingT;
    bool isDrawFwdAhead;
    bool isDrawPrntAhead;
    bool isNoDrawParent;
    bool isNoDrawChild;
    u8 colorInSelect;
    u8 alphaInSelect;
    u16 blendModeFlags;
    u8 alphaCompareFlags;
    u8 alphaRef0;
    u8 alphaRef1;
    u8 zModeFlags;
    u8 anmRndm;
    bool isGlblTexAnm;
    u8 texCalcIdxType;
    u8 texIdx;
    std::vector<u8> texIdxAnimData;
    u8 texIdxLoopOfstMask;
    bool isEnableProjection;
    bool isEnableTexScrollAnm;
    f32 texInitTransX;
    f32 texInitTransY;
    f32 texInitScaleX;
    f32 texInitScaleY;
    f32 texInitRot;
    f32 texIncTransX;
    f32 texIncTransY;
    f32 texIncScaleX;
    f32 texIncScaleY;
    f32 texIncRot;
    bool isGlblClrAnm;
    u8 colorCalcIdxType;
    Color colorPrm;
    Color colorEnv;
    std::vector<std::pair<u16, Color>> colorPrmAnimData;
    std::vector<std::pair<u16, Color>> colorEnvAnimData;
    u16 colorAnimMaxFrm;
    u8 colorLoopOfstMask;
    void parse_flags_JPAC2_10();
    void parse_flags_JPAC2_11();
    void build_flags_JPAC2_10();
    void build_flags_JPAC2_11();
    JPA_BSP1 copy();
};
class JPA_ESP1 {
    public:
    u32 flags;
    u8 unkFlags;
    bool isEnableScale;
    bool isDiffXY;
    u8 scaleAnmTypeX;
    u8 scaleAnmTypeY;
    bool isEnableRotate;
    bool isEnableAlpha;
    bool isEnableSinWave;
    u8 pivotX;
    u8 pivotY;
    f32 scaleInTiming;
    f32 scaleOutTiming;
    f32 scaleInValueX;
    f32 scaleOutValueX;
    f32 scaleInValueY;
    f32 scaleOutValueY;
    f32 scaleOutRandom;
    u16 scaleAnmMaxFrameX;
    u16 scaleAnmMaxFrameY;
    f32 alphaInTiming;
    f32 alphaOutTiming;
    f32 alphaInValue;
    f32 alphaBaseValue;
    f32 alphaOutValue;
    f32 alphaWaveRandom;
    f32 alphaWaveFrequency;
    f32 alphaWaveAmplitude;
    f32 rotateAngle;
    f32 rotateAngleRandom;
    f32 rotateSpeed;
    f32 rotateSpeedRandom;
    f32 rotateDirection;
    void parse_flags();
    void build_flags();
    JPA_ESP1 copy();
};
class JPA_ETX1 {
    public:
    u8 indTextureMode;
    std::vector<f32> floats;
    u8 indTextureID;
    u8 unk;
    i8 scale;
    u8 secondTextureID;
    u32 build_flags();
    JPA_ETX1 copy();
};
class JPA_SSP1 {
    public:
    u32 flags;
    u8 unkFlags1;
    bool isInheritedScale;
    bool isInheritedRGB;
    bool isInheritedAlpha;
    bool isEnableAlphaOut;
    bool isEnableField;
    bool isEnableRotate;
    bool isEnableScaleOut;
    u8 shapeType;
    u8 dirType;
    u8 rotType;
    u8 planeType;
    f32 posRndm;
    f32 baseVel;
    f32 baseVelRndm;
    f32 velInfRate;
    f32 gravity;
    vec2f globalScale2D;
    f32 inheritScale;
    f32 inheritAlpha;
    f32 inheritRGB;
    Color colorPrm;
    Color colorEnv;
    f32 timing;
    u16 life;
    u16 rate;
    u8 step;
    u8 texIdx;
    u16 rotateSpeed;

    void build_flags();
    void parse_flags();
    JPA_SSP1 copy();
};
class JPA_FLD1 {
    public:
    u32 flags;
    u16 sttFlag;
    u8 type;
    u8 addType;
    vec3f pos;
    vec3f dis;
    f32 param1;
    f32 param2;
    f32 param3;
    f32 fadeIn;
    f32 fadeOut;
    f32 enTime;
    f32 disTime;
    u8 cycle;

    void parse_flags();
    void build_flags();
    JPA_FLD1 copy();
};
class JPA_KFA1 {
    public:
    u8 keyType;
    u8 keyCount;
    u8 unk0xA;
    std::vector<f32> keyValues;
    bool isLoopEnable;
    JPA_KFA1 copy();
};
class JPA_Texture {
    public:
        std::string name;
        std::vector<u8> data;
    void add_data(std::string& filename);
};
class JPA_TDB1 { // Maps to the texture names (index)
    public:
        std::vector<u16> textureIdx;
        std::vector<std::string> textures;

        void map_to_texture(std::vector<JPA_Texture> &textures);
        void remap_index(std::vector<JPA_Texture> &textures);
    JPA_TDB1 copy();
};
class JPA_Resource {
    public:
    u16 resourceId;
    u16 blockCount;
    u8 fieldBlockCount;
    u8 keyBlockCount;
    u8 tdb1Count;

    std::vector<JPA_BEM1> bem1; // Dynamics Block
    std::vector<JPA_BSP1> bsp1; // Base Shape Block
    std::vector<JPA_ESP1> esp1; // Extra Shape Block
    std::vector<JPA_ETX1> etx1; // Extra Texture Block
    std::vector<JPA_SSP1> ssp1; // Child Shape Block
    std::vector<JPA_FLD1> fld1; // Field Blocks
    std::vector<JPA_KFA1> kfa1; // Key Blocks
    std::vector<JPA_TDB1> tdb1; // Texture Id to Texture Map

    void update_resource_info(std::string version, std::vector<JPA_Texture> &textures);
};
class JPAC {
    public:
    std::string version;
    u16 resource_count;
    u16 texture_count;
    u32 texture_offset;

    std::vector<JPA_Resource> resources;
    std::vector<JPA_Texture> textures;

    void add_texture(std::string &texture_name);
    void append_textures(std::string &path);
    void add_texture_data(JPA_Texture &texture);
    i32 get_resource_index(u16 id);
    void add_resource(JPA_Resource &resource);
    void apply_edits(JPAC&changes);
    void update();
};

#endif

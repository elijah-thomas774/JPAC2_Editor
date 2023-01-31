#include "../include/jsonBase.hpp"
bool exists(json &j, std::string entry)
{
    return (j.find(entry) != j.end());
}
std::string get_if_exists(json &j, std::string entry, std::string default_val){
    if (exists(j, entry)){
        std::string tempVal = j[entry];
        return tempVal;}
    else
        return default_val;
}
template<typename T>
T get_if_exists(json &j, std::string entry, T default_val){
    if (exists(j, entry))
        return j[entry].get<T>();
    else
        return default_val;
}


std::string to_volume_type(u8 val)
{
    switch (val)
    {
        case 0: return "Cube";
        case 1: return "Sphere";
        case 2: return "Cylinder";
        case 3: return "Torus";
        case 4: return "Point";
        case 5: return "Circle";
        case 6: return "Line";
        default:
            return std::to_string(val);
    }
}
std::string to_shape_type(u8 val){
    switch (val)
    {
        case  0: return "Point";
        case  1: return "Line";
        case  2: return "Billboard";
        case  3: return "Direction";
        case  4: return "DirectionCross";
        case  5: return "Stripe";
        case  6: return "StripeCross";
        case  7: return "Rotation";
        case  8: return "RotationCross";
        case  9: return "DirBillboard";
        case 10: return "YBillboard";
        default:
            return std::to_string(val);
    }
}
std::string to_dir_type(u8 val){
    switch (val)
    {
        case 0: return "Vel";
        case 1: return "Pos";
        case 2: return "PosInv";
        case 3: return "EmtrDir";
        case 4: return "PrevPctl";
        case 5: return "Unk5";
        default:
            return std::to_string(val);
    }
}
std::string to_rot_type(u8 val){
    switch (val)
    {
        case 0: return "Y";
        case 1: return "X";
        case 2: return "Z";
        case 3: return "XYZ";
        case 4: return "YJiggle";
        default:
            return std::to_string(val);
    }
}
std::string to_plane_type(u8 val){
    switch (val)
    {
        case 0: return "XY";
        case 1: return "XZ";
        case 2: return "X";
        default:
            return std::to_string(val);
    }
}
std::string to_calc_idx_type(u8 val){
    switch(val)
    {
        case 0: return "Normal";
        case 1: return "Repeat";
        case 2: return "Reverse";
        case 3: return "Merge";
        case 4: return "Random";
        default:
            return std::to_string(val);
    }
}
std::string to_calc_scale_anm_type(u8 val){
    switch(val)
    {
        case 0: return "Normal";
        case 1: return "Repeat";
        case 2: return "Reverse";
        default:
            return std::to_string(val);
    }
}
std::string to_ind_texture_mode(u8 val){
    switch (val)
    {
        case 0: return "Off";
        case 1: return "Normal";
        case 2: return "Sub";
        default:
            return std::to_string(val);
    }
}
std::string to_field_type(u8 val){
    switch (val)
    {
        case 0: return "Gravity";
        case 1: return "Air";
        case 2: return "Magnet";
        case 3: return "Newton";
        case 4: return "Vortex";
        case 5: return "Random";
        case 6: return "Drag";
        case 7: return "Convection";
        case 8: return "Spin";
        default:
            return std::to_string(val);
    }
}
std::string to_field_add_type(u8 val){
    switch (val)
    {
        case 0: return "FieldAccel";
        case 1: return "BaseVelocity";
        case 2: return "FieldVelocity";
        default:
            return std::to_string(val);
    }
}
std::string to_key_type(u8 val){
    switch (val)
    {
        case  0: return "Rate";
        case  1: return "VolumeSize";
        case  2: return "VolumeSweep";
        case  3: return "VolumeMinRad";
        case  4: return "LifeTime";
        case  5: return "Moment";
        case  6: return "InitialVelOmni";
        case  7: return "InitialVelAxis";
        case  8: return "InitialVelDir";
        case  9: return "Spread";
        case 10: return "Scale";
        default:
            return std::to_string(val);
    }
}

u8 from_volume_type(json &j, std::string field, u8 orig){
    if (!exists(j, field))
        return orig;
    std::string val = j[field].get<std::string>();
    if(val.compare("Cube") == 0)          return 0;
    else if(val.compare("Sphere") == 0)   return 1;
    else if(val.compare("Cylinder") == 0) return 2;
    else if(val.compare("Torus") == 0)    return 3;
    else if(val.compare("Point") == 0)    return 4;
    else if(val.compare("Circle") == 0)   return 5;
    else if(val.compare("Line") == 0)     return 6;
    return 0;
}
u8 from_shape_type(json &j, std::string field, u8 orig){
    if (!exists(j, field))
        return orig;
    std::string val = j[field].get<std::string>();
    if (val.compare("Point") == 0) return 0;
    else if (val.compare("Line") == 0) return 1;
    else if (val.compare("Billboard") == 0) return 2;
    else if (val.compare("Direction") == 0) return 3;
    else if (val.compare("DirectionCross") == 0) return 4;
    else if (val.compare("Stripe") == 0) return 5;
    else if (val.compare("StripeCross") == 0) return 6;
    else if (val.compare("Rotation") == 0) return 7;
    else if (val.compare("RotationCross") == 0) return 8;
    else if (val.compare("DirBillboard") == 0) return 9;
    else if (val.compare("YBillboard") == 0) return 10;
    return 0;
}
u8 from_dir_type(json &j, std::string field, u8 orig){
    if (!exists(j, field))
        return orig;
    std::string val = j[field].get<std::string>();
    if (val.compare("Vel") == 0) return 0;
    else if (val.compare("Pos") == 0) return 1;
    else if (val.compare("PosInv") == 0) return 2;
    else if (val.compare("EmtrDir") == 0) return 3;
    else if (val.compare("PrevPctl") == 0) return 4;
    else if (val.compare("Unk5") == 0) return 5;
    return 0;
}
u8 from_rot_type(json &j, std::string field, u8 orig){
    if (!exists(j, field))
        return orig;
    std::string val = j[field].get<std::string>();
    if (val.compare("Y") == 0) return 0;
    else if (val.compare("X") == 0) return 1;
    else if (val.compare("Z") == 0) return 2;
    else if (val.compare("XYZ") == 0) return 3;
    else if (val.compare("YJiggle") == 0) return 4;
    return 0;
}
u8 from_plane_type(json &j, std::string field, u8 orig){
    if (!exists(j, field))
        return orig;
    std::string val = j[field].get<std::string>();
    if (val.compare("XY") == 0) return 0;
    else if (val.compare("XZ") == 0) return 1;
    else if (val.compare("X") == 0) return 2;
    return 0;
}
u8 from_calc_idx_type(json &j, std::string field, u8 orig){
    if (!exists(j, field))
        return orig;
    std::string val = j[field].get<std::string>();
    if (val.compare("Normal") == 0) return 0;
    else if (val.compare("Repeat") == 0) return 1;
    else if (val.compare("Reverse") == 0) return 2;
    else if (val.compare("Merge") == 0) return 3;
    else if (val.compare("Random") == 0) return 4;
    return 0;
}
u8 from_calc_scale_anm_type(json &j, std::string field, u8 orig){
    if (!exists(j, field))
        return orig;
    std::string val = j[field].get<std::string>();
    if (val.compare("Normal") == 0) return 0;
    else if (val.compare("Repeat") == 0) return 1;
    else if (val.compare("Reverse") == 0) return 2;
    return 0;
}
u8 from_ind_texture_mode(json &j, std::string field, u8 orig){
    if (!exists(j, field))
        return orig;
    std::string val = j[field].get<std::string>();
    if (val.compare("Off") == 0) return 0;
    else if (val.compare("Normal") == 0) return 1;
    else if (val.compare("Sub") == 0) return 2;
    return 0;
}
u8 from_field_type(json &j, std::string field, u8 orig){
    if (!exists(j, field))
        return orig;
    std::string val = j[field].get<std::string>();
    if (val.compare("Gravity") == 0) return 0;
    else if (val.compare("Air") == 0) return 1;
    else if (val.compare("Magnet") == 0) return 2;
    else if (val.compare("Newton") == 0) return 3;
    else if (val.compare("Vortex") == 0) return 4;
    else if (val.compare("Random") == 0) return 5;
    else if (val.compare("Drag") == 0) return 6;
    else if (val.compare("Convection") == 0) return 7;
    else if (val.compare("Spin") == 0) return 8;
    return 0;
}
u8 from_field_add_type(json &j, std::string field, u8 orig){
    if (!exists(j, field))
        return orig;
    std::string val = j[field].get<std::string>();
    if (val.compare("FieldAccel") == 0) return 0;
    else if (val.compare("BaseVelocity") == 0) return 1;
    else if (val.compare("FieldVelocity") == 0) return 2;
    return 0;
}
u8 from_key_type(json &j, std::string field, u8 orig){
    if (!exists(j, field))
        return orig;
    std::string val = j[field].get<std::string>();
    if (val.compare("Rate") == 0) return  0;
    else if (val.compare("VolumeSize") == 0) return  1;
    else if (val.compare("VolumeSweep") == 0) return  2;
    else if (val.compare("VolumeMinRad") == 0) return  3;
    else if (val.compare("LifeTime") == 0) return  4;
    else if (val.compare("Moment") == 0) return  5;
    else if (val.compare("InitialVelOmni") == 0) return  6;
    else if (val.compare("InitialVelAxis") == 0) return  7;
    else if (val.compare("InitialVelDir") == 0) return  8;
    else if (val.compare("Spread") == 0) return  9;
    else if (val.compare("Scale") == 0) return 10;
    return 0;
}


std::string to_color(Color color){
    std::stringstream colorStr;
    colorStr << "#" << std::setw(8) << std::setfill('0') << std::hex << color.color_string;
    return colorStr.str();
}
Color from_color(std::string& str){
    Color color;
    std::string colorHex = str.substr(1, str.length());
    if(colorHex.length() < 8) colorHex.append("FF");
    color.color_string = std::stoul(colorHex, nullptr, 16);
    return color; 
}
ordered_json to_color_table(std::vector<std::pair<u16, Color>> data){
    ordered_json j;
    for (int i = 0; i < data.size(); i++)
    {
        j[i]["time"] = data.at(i).first;
        j[i]["color"] = to_color(data.at(i).second);
    }
    return j;
}
Color get_color_from_json(json& j, std::string field, Color orig){
    if (!exists(j, field))
        return orig;
    
    std::string color_str = j[field].get<std::string>();
    return from_color(color_str);
}
std::vector<std::pair<u16, Color>> from_color_table(json &j, std::string field, std::vector<std::pair<u16, Color>> orig){
    if (!exists(j, field))
        return orig;
    std::vector<std::pair<u16, Color>> table;
    u32 num_entries = j[field].size();
    json &colors = j[field];
    for(i32 i = 0; i < num_entries; i++)
    {
        assert(exists(colors[i], "time"));
        assert(exists(colors[i], "color"));
        Color tempColor;
        tempColor.color_string = 0xFFFFFFFF;
        Color currColor = get_color_from_json(colors[i], "color", tempColor);
        u16 time = colors[i]["time"].get<u16>();
        table.push_back({time, currColor});
    }
    return table;
}

ordered_json to_json(JPA_BEM1& bem1){
    // output all fields (includes enum -> text)
    // u32 emitFlags; // built with others
    ordered_json j;
    j["unkFlags"] = bem1.unkFlags;
    j["emitFlags"] = bem1.unkEmitFlags;
    // DOC : emitFlags
    //   FixedDensity        = 0x01,
    //   FixedInterval       = 0x02,
    //   InheritScale        = 0x04,
    //   FollowEmitter       = 0x08,
    //   FollowEmitterChild  = 0x10
    j["unkFlags"] = bem1.unkFlags;
    j["volumeType"] = to_volume_type(bem1.volumeType);
    j["emitterScl"] = bem1.emitterScl;
    j["emitterRot"] = bem1.emitterRot;
    j["emitterTrs"] = bem1.emitterTrs;
    j["emitterDir"] = bem1.emitterDir;
    j["initialVelOmni"] = bem1.initialVelOmni;
    j["initialVelAxis"] = bem1.initialVelAxis;
    j["initialVelRndm"] = bem1.initialVelRndm;
    j["initialVelDir"] = bem1.initialVelDir; 
    j["spread"] = bem1.spread;
    j["initialVelRatio"] = bem1.initialVelRatio;
    j["rate"] = bem1.rate;
    j["rateRndm"] = bem1.rateRndm;
    j["lifeTimeRndm"] = bem1.lifeTimeRndm;
    j["volumeSweep"] = bem1.volumeSweep;
    j["volumeMinRad"] = bem1.volumeMinRad;
    j["airResist"] = bem1.airResist;
    j["momentRndm"] = bem1.momentRndm;
    j["maxFrame"] = bem1.maxFrame;
    j["startFrame"] = bem1.startFrame;
    j["lifeTime"] = bem1.lifeTime;
    j["volumeSize"] = bem1.volumeSize;
    j["divNumber"] = bem1.divNumber;
    j["rateStep"] = bem1.rateStep;
    return j;
}
ordered_json to_json(JPA_BSP1& bsp1){
    ordered_json j;
    j["unkVar1"]                = bsp1.unkVar1;
    j["unkVar2"]                = bsp1.unkVar2;
    j["unkVar3"]                = bsp1.unkVar3;
    j["unkVar4"]                = bsp1.unkVar4;
    j["unkVar5"]                = bsp1.unkVar5;
    j["unkColorFlag1"]          = bsp1.unkColorFlag1;
    j["unkColorFlag2"]          = bsp1.unkColorFlag2;
    j["shapeType"]              = to_shape_type (bsp1.shapeType);
    j["dirType"]                = to_dir_type   (bsp1.dirType);
    j["rotType"]                = to_rot_type   (bsp1.rotType);
    j["planeType"]              = to_plane_type (bsp1.planeType);
    j["baseSize"]               = bsp1.baseSize;
    j["tilingS"]                = bsp1.tilingS;
    j["tilingT"]                = bsp1.tilingT;
    j["isDrawFwdAhead"]         = bsp1.isDrawFwdAhead;
    j["isDrawPrntAhead"]        = bsp1.isDrawPrntAhead;
    j["isNoDrawParent"]         = bsp1.isNoDrawParent;
    j["isNoDrawChild"]          = bsp1.isNoDrawChild;
    j["colorInSelect"]          = bsp1.colorInSelect;
    j["alphaInSelect"]          = bsp1.alphaInSelect;
    j["blendModeFlags"]         = bsp1.blendModeFlags;
    j["alphaCompareFlags"]      = bsp1.alphaCompareFlags;
    j["alphaRef0"]              = bsp1.alphaRef0;
    j["alphaRef1"]              = bsp1.alphaRef1;
    j["zModeFlags"]             = bsp1.zModeFlags;
    j["anmRndm"]                = bsp1.anmRndm;
    j["isGlblTexAnm"]           = bsp1.isGlblTexAnm;
    j["texCalcIdxType"]         = to_calc_idx_type(bsp1.texCalcIdxType);
    j["texIdx"]                 = bsp1.texIdx;
    j["texIdxAnimData"]         = bsp1.texIdxAnimData;
    j["texIdxLoopOfstMask"]     = bsp1.texIdxLoopOfstMask;
    j["isEnableProjection"]     = bsp1.isEnableProjection;
    j["isEnableTexScrollAnm"]   = bsp1.isEnableTexScrollAnm;
    j["texInitTransX"]          = bsp1.texInitTransX;
    j["texInitTransY"]          = bsp1.texInitTransY;
    j["texInitScaleX"]          = bsp1.texInitScaleX;
    j["texInitScaleY"]          = bsp1.texInitScaleY;
    j["texInitRot"]             = bsp1.texInitRot;
    j["texIncTransX"]           = bsp1.texIncTransX;
    j["texIncTransY"]           = bsp1.texIncTransY;
    j["texIncScaleX"]           = bsp1.texIncScaleX;
    j["texIncScaleY"]           = bsp1.texIncScaleY;
    j["texIncRot"]              = bsp1.texIncRot;
    j["isGlblClrAnm"]           = bsp1.isGlblClrAnm;
    j["colorCalcIdxType"]       = bsp1.colorCalcIdxType;
    j["colorPrm"]               = to_color(bsp1.colorPrm);
    j["colorEnv"]               = to_color(bsp1.colorEnv);
    j["colorPrmAnimData"]       = to_color_table(bsp1.colorPrmAnimData);
    j["colorEnvAnimData"]       = to_color_table(bsp1.colorEnvAnimData);
    j["colorAnimMaxFrm"]        = bsp1.colorAnimMaxFrm;
    j["colorLoopOfstMask"]      = bsp1.colorLoopOfstMask;
    return j;
}
ordered_json to_json(JPA_ESP1& esp1){
    ordered_json j;
    j["unkFlags"] = esp1.unkFlags;
    j["isEnableScale"] = esp1.isEnableScale;
    j["isDiffXY"] = esp1.isDiffXY;
    j["scaleAnmTypeX"] = to_calc_scale_anm_type(esp1.scaleAnmTypeX);
    j["scaleAnmTypeY"] = to_calc_scale_anm_type(esp1.scaleAnmTypeY);
    j["isEnableRotate"] = esp1.isEnableRotate;
    j["isEnableAlpha"] = esp1.isEnableAlpha;
    j["isEnableSinWave"] = esp1.isEnableSinWave;
    j["pivotX"] = esp1.pivotX;
    j["pivotY"] = esp1.pivotY;
    j["scaleInTiming"] = esp1.scaleInTiming;
    j["scaleOutTiming"] = esp1.scaleOutTiming;
    j["scaleInValueX"] = esp1.scaleInValueX;
    j["scaleOutValueX"] = esp1.scaleOutValueX;
    j["scaleInValueY"] = esp1.scaleInValueY;
    j["scaleOutValueY"] = esp1.scaleOutValueY;
    j["scaleOutRandom"] = esp1.scaleOutRandom;
    j["scaleAnmMaxFrameX"] = esp1.scaleAnmMaxFrameX;
    j["scaleAnmMaxFrameY"] = esp1.scaleAnmMaxFrameY;
    j["alphaInTiming"] = esp1.alphaInTiming;
    j["alphaOutTiming"] = esp1.alphaOutTiming;
    j["alphaInValue"] = esp1.alphaInValue;
    j["alphaBaseValue"] = esp1.alphaBaseValue;
    j["alphaOutValue"] = esp1.alphaOutValue;
    j["alphaWaveRandom"] = esp1.alphaWaveRandom;
    j["alphaWaveFrequency"] = esp1.alphaWaveFrequency;
    j["alphaWaveAmplitude"] = esp1.alphaWaveAmplitude;
    j["rotateAngle"] = esp1.rotateAngle;
    j["rotateAngleRandom"] = esp1.rotateAngleRandom;
    j["rotateSpeed"] = esp1.rotateSpeed;
    j["rotateSpeedRandom"] = esp1.rotateSpeedRandom;
    j["rotateDirection"] = esp1.rotateDirection;
    return j;
}
ordered_json to_json(JPA_ETX1& etx1){
    ordered_json j;
    j["indTextureMode"] = to_ind_texture_mode(etx1.indTextureMode);
    j["floats"] = etx1.floats;
    j["indTextureID"] = etx1.indTextureID;
    j["unk"] = etx1.unk;
    j["scale"] = etx1.scale;
    j["secondTextureID"] = etx1.secondTextureID;
    return j;
}
ordered_json to_json(JPA_SSP1& ssp1){
    ordered_json j;
    j["unkFlags1"] = ssp1.unkFlags1;
    j["isInheritedScale"] = ssp1.isInheritedScale;
    j["isInheritedRGB"] = ssp1.isInheritedRGB;
    j["isInheritedAlpha"] = ssp1.isInheritedAlpha;
    j["isEnableAlphaOut"] = ssp1.isEnableAlphaOut;
    j["isEnableField"] = ssp1.isEnableField;
    j["isEnableRotate"] = ssp1.isEnableRotate;
    j["isEnableScaleOut"] = ssp1.isEnableScaleOut;
    j["shapeType"] = to_shape_type(ssp1.shapeType);
    j["dirType"] =   to_dir_type(ssp1.dirType);
    j["rotType"] =   to_rot_type(ssp1.rotType);
    j["planeType"] = to_plane_type(ssp1.planeType);
    j["posRndm"] = ssp1.posRndm;
    j["baseVel"] = ssp1.baseVel;
    j["baseVelRndm"] = ssp1.baseVelRndm;
    j["velInfRate"] = ssp1.velInfRate;
    j["gravity"] = ssp1.gravity;
    j["globalScale2D"] = ssp1.globalScale2D;
    j["inheritScale"] = ssp1.inheritScale;
    j["inheritAlpha"] = ssp1.inheritAlpha;
    j["inheritRGB"] = ssp1.inheritRGB;
    j["colorPrm"] = to_color(ssp1.colorPrm);
    j["colorEnv"] = to_color(ssp1.colorEnv);
    j["timing"] = ssp1.timing;
    j["life"] = ssp1.life;
    j["rate"] = ssp1.rate;
    j["step"] = ssp1.step;
    j["texIdx"] = ssp1.texIdx;
    j["rotateSpeed"] = ssp1.rotateSpeed;
    return j;
}
ordered_json to_json(JPA_FLD1& fld1){
    ordered_json j;
    j["sttFlag"] = fld1.sttFlag;
    j["type"] = to_field_type(fld1.type);
    j["addType"] = to_field_add_type(fld1.addType);
    j["pos"] = fld1.pos;
    j["dis"] = fld1.dis;
    j["param1"] = fld1.param1;
    j["param2"] = fld1.param2;
    j["param3"] = fld1.param3;
    j["fadeIn"] = fld1.fadeIn;
    j["fadeOut"] = fld1.fadeOut;
    j["enTime"] = fld1.enTime;
    j["disTime"] = fld1.disTime;
    j["cycle"] = fld1.cycle;
    return j;
}
ordered_json to_json(JPA_KFA1& kfa1){
    ordered_json j;
    j["keyType"] = to_key_type(kfa1.keyType);
    j["unk0xA"] = kfa1.unk0xA;
    j["keyValues"] = kfa1.keyValues;
    j["isLoopEnable"] = kfa1.isLoopEnable;
    return j;
}
ordered_json to_json(JPA_TDB1& tdb1){
    ordered_json j;
    j["textures"] = tdb1.textures;
    return j;
}
ordered_json to_json(JPA_Texture& texture){
    ordered_json j;
    j["name"] = texture.name;
    return j;
}
ordered_json to_json(JPA_Resource& resource){
    ordered_json j;
    j["resourceId"] = resource.resourceId;
    j["BEM1"] = to_json(resource.bem1.at(0));
    j["BSP1"] = to_json(resource.bsp1.at(0));
    j["ESP1"] = {};
    j["ETX1"] = {};
    j["SSP1"] = {};
    if (resource.esp1.size() != 0) j["ESP1"] = to_json(resource.esp1.at(0));
    if (resource.etx1.size() != 0) j["ETX1"] = to_json(resource.etx1.at(0));
    if (resource.ssp1.size() != 0) j["SSP1"] = to_json(resource.ssp1.at(0));
    j["FLD1"] = {};
    for(i32 i = 0; i < resource.fld1.size(); i++)
        j["FLD1"][i] = to_json(resource.fld1.at(i));
    j["KFA1"] = {};
    for(i32 i = 0; i < resource.kfa1.size(); i++)
        j["KFA1"][i] = to_json(resource.kfa1.at(i));
    j["TDB1"] = to_json(resource.tdb1.at(0));
    return j;
}
ordered_json to_json(JPAC& jpc){
    ordered_json j;
    j["version"] = jpc.version;
    j["resources"] = {};
    for (int i = 0; i < jpc.resources.size(); i++)
        j["resources"][i] = to_json(jpc.resources.at(i));
    j["textures"] = {};
    for (int i = 0; i < jpc.textures.size(); i++)
        j["textures"][i] = to_json(jpc.textures.at(i));
    return j;
}
JPA_BEM1 read_BEM1_from_json(json& j, JPA_BEM1 &orig){
    JPA_BEM1 bem1;
    bem1.unkFlags          = get_if_exists(j, "unkFlags", orig.unkFlags);
    bem1.unkEmitFlags      = get_if_exists(j, "emitFlags", orig.unkFlags);
    bem1.volumeType        = from_volume_type(j, "volumeType", orig.volumeType);
    bem1.emitterScl        = get_if_exists(j, "emitterScl", orig.emitterScl);
    bem1.emitterRot        = get_if_exists(j, "emitterRot", orig.emitterRot);
    bem1.emitterTrs        = get_if_exists(j, "emitterTrs", orig.emitterTrs);
    bem1.emitterDir        = get_if_exists(j, "emitterDir", orig.emitterDir);
    bem1.initialVelOmni    = get_if_exists(j, "initialVelOmni", orig.initialVelOmni);
    bem1.initialVelAxis    = get_if_exists(j, "initialVelAxis", orig.initialVelAxis);
    bem1.initialVelRndm    = get_if_exists(j, "initialVelRndm", orig.initialVelRndm);
    bem1.initialVelDir     = get_if_exists(j, "initialVelDir", orig.initialVelDir);  
    bem1.spread            = get_if_exists(j, "spread", orig.spread);
    bem1.initialVelRatio   = get_if_exists(j, "initialVelRatio", orig.initialVelRatio);
    bem1.rate              = get_if_exists(j, "rate", orig.rate);
    bem1.rateRndm          = get_if_exists(j, "rateRndm", orig.rateRndm);
    bem1.lifeTimeRndm      = get_if_exists(j, "lifeTimeRndm", orig.lifeTimeRndm);
    bem1.volumeSweep       = get_if_exists(j, "volumeSweep", orig.volumeSweep);
    bem1.volumeMinRad      = get_if_exists(j, "volumeMinRad", orig.volumeMinRad);
    bem1.airResist         = get_if_exists(j, "airResist", orig.airResist);
    bem1.momentRndm        = get_if_exists(j, "momentRndm", orig.momentRndm);
    bem1.maxFrame          = get_if_exists(j, "maxFrame", orig.maxFrame);
    bem1.startFrame        = get_if_exists(j, "startFrame", orig.startFrame);
    bem1.lifeTime          = get_if_exists(j, "lifeTime", orig.lifeTime);
    bem1.volumeSize        = get_if_exists(j, "volumeSize", orig.volumeSize);
    bem1.divNumber         = get_if_exists(j, "divNumber", orig.divNumber);
    bem1.rateStep          = get_if_exists(j, "rateStep", orig.rateStep);
    bem1.build_flags();
    return bem1;
}
JPA_BSP1 read_BSP1_from_json(json& j, JPA_BSP1 &orig){
    JPA_BSP1 bsp1;
    bsp1.unkVar1 = get_if_exists(j, "unkVar1", orig.unkVar1);                 
    bsp1.unkVar2 = get_if_exists(j, "unkVar2", orig.unkVar2);                 
    bsp1.unkVar3 = get_if_exists(j, "unkVar3", orig.unkVar3);                 
    bsp1.unkVar4 = get_if_exists(j, "unkVar4", orig.unkVar4);                 
    bsp1.unkVar5 = get_if_exists(j, "unkVar5", orig.unkVar5);                 
    bsp1.unkColorFlag1 = get_if_exists(j, "unkColorFlag1", orig.unkColorFlag1);           
    bsp1.unkColorFlag2 = get_if_exists(j, "unkColorFlag2", orig.unkColorFlag2);           
    bsp1.shapeType  = from_shape_type(j, "shapeType", orig.shapeType);
    bsp1.dirType    = from_dir_type(j, "dirType",   orig.dirType);
    bsp1.rotType    = from_rot_type(j, "rotType",   orig.rotType);
    bsp1.planeType  = from_plane_type(j, "planeType", orig.planeType);
    bsp1.baseSize = get_if_exists(j, "baseSize", orig.baseSize);
    bsp1.tilingS = get_if_exists(j, "tilingS", orig.tilingS);  
    bsp1.tilingT = get_if_exists(j, "tilingT", orig.tilingT);  
    bsp1.isDrawFwdAhead = get_if_exists(j, "isDrawFwdAhead", orig.isDrawFwdAhead);
    bsp1.isDrawPrntAhead = get_if_exists(j, "isDrawPrntAhead", orig.isDrawPrntAhead);
    bsp1.isNoDrawParent = get_if_exists(j, "isNoDrawParent", orig.isNoDrawParent);
    bsp1.isNoDrawChild = get_if_exists(j, "isNoDrawChild", orig.isNoDrawChild);
    bsp1.colorInSelect = get_if_exists(j, "colorInSelect", orig.colorInSelect);
    bsp1.alphaInSelect = get_if_exists(j, "alphaInSelect", orig.alphaInSelect);
    bsp1.blendModeFlags = get_if_exists(j, "blendModeFlags", orig.blendModeFlags);
    bsp1.alphaCompareFlags = get_if_exists(j, "alphaCompareFlags", orig.alphaCompareFlags);
    bsp1.alphaRef0 = get_if_exists(j, "alphaRef0", orig.alphaRef0);               
    bsp1.alphaRef1 = get_if_exists(j, "alphaRef1", orig.alphaRef1);               
    bsp1.zModeFlags = get_if_exists(j, "zModeFlags", orig.zModeFlags);              
    bsp1.anmRndm = get_if_exists(j, "anmRndm", orig.anmRndm);                 
    bsp1.isGlblTexAnm = get_if_exists(j, "isGlblTexAnm", orig.isGlblTexAnm);            
    bsp1.texCalcIdxType = from_calc_idx_type(j, "texCalcIdxType", orig.texCalcIdxType);          
    bsp1.texIdx = get_if_exists(j, "texIdx", orig.texIdx);                  
    bsp1.texIdxAnimData = get_if_exists(j, "texIdxAnimData", orig.texIdxAnimData);          
    bsp1.texIdxLoopOfstMask = get_if_exists(j, "texIdxLoopOfstMask", orig.texIdxLoopOfstMask);      
    bsp1.isEnableProjection = get_if_exists(j, "isEnableProjection", orig.isEnableProjection);      
    bsp1.isEnableTexScrollAnm = get_if_exists(j, "isEnableTexScrollAnm", orig.isEnableTexScrollAnm);    
    bsp1.texInitTransX = get_if_exists(j, "texInitTransX", orig.texInitTransX);           
    bsp1.texInitTransY = get_if_exists(j, "texInitTransY", orig.texInitTransY);           
    bsp1.texInitScaleX = get_if_exists(j, "texInitScaleX", orig.texInitScaleX);           
    bsp1.texInitScaleY = get_if_exists(j, "texInitScaleY", orig.texInitScaleY);           
    bsp1.texInitRot = get_if_exists(j, "texInitRot", orig.texInitRot);              
    bsp1.texIncTransX = get_if_exists(j, "texIncTransX", orig.texIncTransX);            
    bsp1.texIncTransY = get_if_exists(j, "texIncTransY", orig.texIncTransY);            
    bsp1.texIncScaleX = get_if_exists(j, "texIncScaleX", orig.texIncScaleX);            
    bsp1.texIncScaleY = get_if_exists(j, "texIncScaleY", orig.texIncScaleY);            
    bsp1.texIncRot = get_if_exists(j, "texIncRot", orig.texIncRot);               
    bsp1.isGlblClrAnm = get_if_exists(j, "isGlblClrAnm", orig.isGlblClrAnm);            
    bsp1.colorCalcIdxType = get_if_exists(j, "colorCalcIdxType", orig.colorCalcIdxType);     
    bsp1.colorPrm = get_color_from_json(j, "colorPrm", orig.colorPrm);
    bsp1.colorEnv = get_color_from_json(j, "colorEnv", orig.colorEnv);
    bsp1.colorPrmAnimData = from_color_table(j, "colorPrmAnimData", orig.colorPrmAnimData);
    bsp1.colorEnvAnimData = from_color_table(j, "colorEnvAnimData", orig.colorEnvAnimData);
    bsp1.colorAnimMaxFrm = get_if_exists(j, "colorAnimMaxFrm", orig.colorAnimMaxFrm);         
    bsp1.colorLoopOfstMask = get_if_exists(j, "colorLoopOfstMask", orig.colorLoopOfstMask);   
    return bsp1;    
}
JPA_ESP1 read_ESP1_from_json(json& j, JPA_ESP1 &orig){
    JPA_ESP1 esp1;
    esp1.unkFlags = get_if_exists(j, "unkFlags", orig.unkFlags);
    esp1.isEnableScale = get_if_exists(j, "isEnableScale", orig.isEnableScale);
    esp1.isDiffXY = get_if_exists(j, "isDiffXY", orig.isDiffXY);
    esp1.scaleAnmTypeX = from_calc_scale_anm_type(j, "scaleAnmTypeX", orig.scaleAnmTypeX);
    esp1.scaleAnmTypeY = from_calc_scale_anm_type(j, "scaleAnmTypeY", orig.scaleAnmTypeY);
    esp1.isEnableRotate = get_if_exists(j, "isEnableRotate", orig.isEnableRotate);
    esp1.isEnableAlpha = get_if_exists(j, "isEnableAlpha", orig.isEnableAlpha);
    esp1.isEnableSinWave = get_if_exists(j, "isEnableSinWave", orig.isEnableSinWave);
    esp1.pivotX = get_if_exists(j, "pivotX", orig.pivotX);
    esp1.pivotY = get_if_exists(j, "pivotY", orig.pivotY);
    esp1.scaleInTiming = get_if_exists(j, "scaleInTiming", orig.scaleInTiming);
    esp1.scaleOutTiming = get_if_exists(j, "scaleOutTiming", orig.scaleOutTiming);
    esp1.scaleInValueX = get_if_exists(j, "scaleInValueX", orig.scaleInValueX);
    esp1.scaleOutValueX = get_if_exists(j, "scaleOutValueX", orig.scaleOutValueX);
    esp1.scaleInValueY = get_if_exists(j, "scaleInValueY", orig.scaleInValueY);
    esp1.scaleOutValueY = get_if_exists(j, "scaleOutValueY", orig.scaleOutValueY);
    esp1.scaleOutRandom = get_if_exists(j, "scaleOutRandom", orig.scaleOutRandom);
    esp1.scaleAnmMaxFrameX = get_if_exists(j, "scaleAnmMaxFrameX", orig.scaleAnmMaxFrameX);
    esp1.scaleAnmMaxFrameY = get_if_exists(j, "scaleAnmMaxFrameY", orig.scaleAnmMaxFrameY);
    esp1.alphaInTiming = get_if_exists(j, "alphaInTiming", orig.alphaInTiming);
    esp1.alphaOutTiming = get_if_exists(j, "alphaOutTiming", orig.alphaOutTiming);
    esp1.alphaInValue = get_if_exists(j, "alphaInValue", orig.alphaInValue);
    esp1.alphaBaseValue = get_if_exists(j, "alphaBaseValue", orig.alphaBaseValue);
    esp1.alphaOutValue = get_if_exists(j, "alphaOutValue", orig.alphaOutValue);
    esp1.alphaWaveRandom = get_if_exists(j, "alphaWaveRandom", orig.alphaWaveRandom);
    esp1.alphaWaveFrequency = get_if_exists(j, "alphaWaveFrequency", orig.alphaWaveFrequency);
    esp1.alphaWaveAmplitude = get_if_exists(j, "alphaWaveAmplitude", orig.alphaWaveAmplitude);
    esp1.rotateAngle = get_if_exists(j, "rotateAngle", orig.rotateAngle);
    esp1.rotateAngleRandom = get_if_exists(j, "rotateAngleRandom", orig.rotateAngleRandom);
    esp1.rotateSpeed = get_if_exists(j, "rotateSpeed", orig.rotateSpeed);
    esp1.rotateSpeedRandom = get_if_exists(j, "rotateSpeedRandom", orig.rotateSpeedRandom);
    esp1.rotateDirection = get_if_exists(j, "rotateDirection", orig.rotateDirection);
    return esp1;
}
JPA_ETX1 read_ETX1_from_json(json& j, JPA_ETX1 &orig){
    JPA_ETX1 etx1;
    etx1.indTextureMode = from_ind_texture_mode(j, "indTextureMode", orig.indTextureMode);
    etx1.floats = get_if_exists(j, "floats", orig.floats);
    etx1.indTextureID = get_if_exists(j, "indTextureID", orig.indTextureID);
    etx1.unk = get_if_exists(j, "unk", orig.unk);
    etx1.scale = get_if_exists(j, "scale", orig.scale);
    etx1.secondTextureID = get_if_exists(j, "secondTextureID", orig.secondTextureID);
    return etx1;
}
JPA_SSP1 read_SSP1_from_json(json& j, JPA_SSP1 &orig){
    JPA_SSP1 ssp1;
    ssp1.unkFlags1 =  get_if_exists(j, "unkFlags1", orig.unkFlags1);
    ssp1.isInheritedScale =  get_if_exists(j, "isInheritedScale", orig.isInheritedScale);
    ssp1.isInheritedRGB =  get_if_exists(j, "isInheritedRGB", orig.isInheritedRGB);
    ssp1.isInheritedAlpha =  get_if_exists(j, "isInheritedAlpha", orig.isInheritedAlpha);
    ssp1.isEnableAlphaOut =  get_if_exists(j, "isEnableAlphaOut", orig.isEnableAlphaOut);
    ssp1.isEnableField =  get_if_exists(j, "isEnableField", orig.isEnableField);
    ssp1.isEnableRotate =  get_if_exists(j, "isEnableRotate", orig.isEnableRotate);
    ssp1.isEnableScaleOut =  get_if_exists(j, "isEnableScaleOut", orig.isEnableScaleOut);
    ssp1.shapeType =  from_shape_type(j, "shapeType", orig.shapeType);
    ssp1.dirType =  from_dir_type(j, "dirType",   orig.dirType);
    ssp1.rotType =  from_rot_type(j, "rotType",   orig.rotType);
    ssp1.planeType =  from_plane_type(j, "planeType", orig.planeType);
    ssp1.posRndm =  get_if_exists(j, "posRndm", orig.posRndm);
    ssp1.baseVel =  get_if_exists(j, "baseVel", orig.baseVel);
    ssp1.baseVelRndm =  get_if_exists(j, "baseVelRndm", orig.baseVelRndm);
    ssp1.velInfRate =  get_if_exists(j, "velInfRate", orig.velInfRate);
    ssp1.gravity =  get_if_exists(j, "gravity", orig.gravity);
    ssp1.globalScale2D =  get_if_exists(j, "globalScale2D", orig.globalScale2D);
    ssp1.inheritScale =  get_if_exists(j, "inheritScale", orig.inheritScale);
    ssp1.inheritAlpha =  get_if_exists(j, "inheritAlpha", orig.inheritAlpha);
    ssp1.inheritRGB =  get_if_exists(j, "inheritRGB", orig.inheritRGB);
    ssp1.colorPrm =  get_color_from_json(j, "colorPrm", orig.colorPrm);
    ssp1.colorEnv =  get_color_from_json(j, "colorEnv", orig.colorEnv);
    ssp1.timing =  get_if_exists(j, "timing", orig.timing);
    ssp1.life =  get_if_exists(j, "life", orig.life);
    ssp1.rate =  get_if_exists(j, "rate", orig.rate);
    ssp1.step =  get_if_exists(j, "step", orig.step);
    ssp1.texIdx =  get_if_exists(j, "texIdx", orig.texIdx);
    ssp1.rotateSpeed =  get_if_exists(j, "rotateSpeed", orig.rotateSpeed);

    return ssp1;
}
JPA_FLD1 read_FLD1_from_json(json& j, JPA_FLD1 &orig){
    JPA_FLD1 fld1;
    fld1.sttFlag = get_if_exists(j, "sttFlag", orig.sttFlag);
    fld1.type = from_field_type(j, "type", orig.type);
    fld1.addType = from_field_add_type(j, "addType", orig.addType);
    fld1.pos = get_if_exists(j, "pos", orig.pos);
    fld1.dis = get_if_exists(j, "dis", orig.dis);
    fld1.param1 = get_if_exists(j, "param1", orig.param1);
    fld1.param2 = get_if_exists(j, "param2", orig.param2);
    fld1.param3 = get_if_exists(j, "param3", orig.param3);
    fld1.fadeIn = get_if_exists(j, "fadeIn", orig.fadeIn);
    fld1.fadeOut = get_if_exists(j, "fadeOut", orig.fadeOut);
    fld1.enTime = get_if_exists(j, "enTime", orig.enTime);
    fld1.disTime = get_if_exists(j, "disTime", orig.disTime);
    fld1.cycle = get_if_exists(j, "cycle", orig.cycle);
    return fld1;
}
JPA_KFA1 read_KFA1_from_json(json& j, JPA_KFA1 &orig){
    JPA_KFA1 kfa1;
    kfa1.keyType = from_key_type(j, "keyType", orig.keyType);
    kfa1.unk0xA = get_if_exists(j, "unk0xA", orig.unk0xA);
    kfa1.keyValues = get_if_exists(j, "keyValues", orig.keyValues);
    kfa1.isLoopEnable = get_if_exists(j, "isLoopEnable", orig.isLoopEnable);
    return kfa1;
}
JPA_TDB1 read_TDB1_from_json(json& j, JPA_TDB1 &orig){
    JPA_TDB1 tdb1;
    tdb1.textures = get_if_exists(j, "textures", orig.textures);
    return tdb1;
}
JPA_Texture read_Texture_from_json(json& j){
    JPA_Texture tex1;
    tex1.name = get_if_exists(j, "name", (std::string)"dummy");
    return tex1;
}
JPA_Resource read_Resource_from_json(json& j, JPA_Resource &orig){
    JPA_Resource res;
    res.resourceId = get_if_exists(j, "resourceId", orig.resourceId);
    if (exists(j, "BEM1") && j["BEM1"].size() != 0) {
        res.bem1.push_back(read_BEM1_from_json(j["BEM1"], orig.bem1.at(0)));
    } else 
        res.bem1 = orig.bem1;


    if (exists(j, "BSP1") && j["BSP1"].size() != 0) {
        res.bsp1.push_back(read_BSP1_from_json(j["BSP1"], orig.bsp1.at(0)));
    } else 
        res.bsp1 = orig.bsp1;


    if (exists(j, "ESP1") && j["ESP1"].size() != 0) {
        JPA_ESP1 temp;
        if (orig.esp1.size() != 0)
            res.esp1.push_back(read_ESP1_from_json(j["ESP1"], orig.esp1.at(0)));
        else
            res.esp1.push_back(read_ESP1_from_json(j["ESP1"], temp));
    } else 
        res.esp1 = orig.esp1;


    if (exists(j, "ETX1") && j["ETX1"].size() != 0) {
        JPA_ETX1 temp;
        if (orig.etx1.size() != 0)
            res.etx1.push_back(read_ETX1_from_json(j["ETX1"], orig.etx1.at(0)));
        else
            res.etx1.push_back(read_ETX1_from_json(j["ETX1"], temp));
    } else 
        res.etx1 = orig.etx1;


    if (exists(j, "SSP1") && j["SSP1"].size() != 0) {
        JPA_SSP1 temp;
        if (orig.ssp1.size() != 0)
            res.ssp1.push_back(read_SSP1_from_json(j["SSP1"], orig.ssp1.at(0)));
        else
            res.ssp1.push_back(read_SSP1_from_json(j["SSP1"], temp));
    } else 
        res.ssp1 = orig.ssp1;


    if (exists(j, "FLD1") && j["FLD1"].size() != 0) {
        JPA_FLD1 temp;
        for (int i = 0; i < j["FLD1"].size(); i++)
        {
            if (i < orig.fld1.size())
                res.fld1.push_back(read_FLD1_from_json(j["FLD1"].at(i), orig.fld1.at(i)));
            else
                res.fld1.push_back(read_FLD1_from_json(j["FLD1"].at(i), temp));
        }
    } else 
        res.fld1 = orig.fld1;


    if (exists(j, "KFA1") && j["KFA1"].size() != 0) {
        JPA_KFA1 temp;
        for (int i = 0; i < j["KFA1"].size(); i++)
        {
            if (i < orig.kfa1.size())
                res.kfa1.push_back(read_KFA1_from_json(j["KFA1"].at(i), orig.kfa1.at(i)));
            else
                res.kfa1.push_back(read_KFA1_from_json(j["KFA1"].at(i), temp));
        }
    } else 
        res.kfa1 = orig.kfa1;
    

    if (exists(j, "TDB1") && j["TDB1"].size() != 0) {
        res.tdb1.push_back(read_TDB1_from_json(j["TDB1"], orig.tdb1.at(0)));
    } else 
        res.tdb1 = orig.tdb1;
    return res;
}
JPAC read_JPAC_from_json(json& j, JPAC &orig){
    JPAC jpc;
    jpc.version = get_if_exists(j, "version", orig.version);
    if (exists(j, "resources"))
    {
        json j1 = j["resources"];
        for(i32 i = 0; i < j1.size(); i++){
            assert(exists(j1[i], "resourceId"));
            u16 resId = j1[i]["resourceId"].get<u16>();
            i32 idx = orig.get_resource_index(resId);
            if (i < orig.resources.size() && idx == i){
                jpc.resources.push_back(read_Resource_from_json(j1[i], orig.resources.at(idx)));
            } else if (idx != -1)
            {
                jpc.resources.push_back(read_Resource_from_json(j1[i], orig.resources.at(idx)));
            } 
            else {
                JPA_Resource res;
                jpc.resources.push_back(read_Resource_from_json(j1[i], res));
            }
        }
        
    }
    if (exists(j, "textures"))
    {
        json j2 = j["textures"];
        for(int i = 0; i < j2.size(); i++)
        {
            if (i < orig.textures.size())
            {
                jpc.textures.push_back(read_Texture_from_json(j2[i]));
            } else {
                jpc.textures.push_back(read_Texture_from_json(j2[i]));
            }
        }
    }
    else{
        jpc.textures = orig.textures;
    }
    for (auto & res : jpc.resources)
        res.update_resource_info(jpc.version, jpc.textures);
    return jpc;
}
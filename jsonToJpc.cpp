#include "jsonToJpc.hpp"

Buffer toBuffer(JPA_BEM1& bem1){
    Buffer block;
    block.write((u32)'BEM1');
    block.write((u32)0x7C); // size
    block.write(bem1.emitFlags);
    block.write(bem1.unkFlags);
    block.write(bem1.emitterScl);
    block.write(bem1.emitterTrs);
    block.write(bem1.emitterDir);
    block.write(bem1.initialVelOmni);
    block.write(bem1.initialVelAxis);
    block.write(bem1.initialVelRndm);
    block.write(bem1.initialVelDir);
    block.write(bem1.spread);
    block.write(bem1.initialVelRatio);
    block.write(bem1.rate);
    block.write(bem1.rateRndm);
    block.write(bem1.lifeTimeRndm);
    block.write(bem1.volumeSweep);
    block.write(bem1.volumeMinRad);
    block.write(bem1.airResist);
    block.write(bem1.momentRndm);
    block.write(bem1.emitterRot); // TODO, add conversion for deg to rad
    block.write(bem1.maxFrame);
    block.write(bem1.startFrame);
    block.write(bem1.lifeTime);
    block.write(bem1.volumeSize);
    block.write(bem1.divNumber);
    block.write(bem1.rateStep);
    block.write(std::vector<u8>(3,0));
    assert(0x7C == block.data.size());
    return block;
}
Buffer toBuffer(JPA_BSP1& bsp1){
    Buffer block;
    block.write((u32)'BSP1');
    u32 size = 0x34;
    size += bsp1.isEnableTexScrollAnm ? 0x28 : 0;
    u32 texAnmSize = bsp1.texIdxAnimData.size();
    texAnmSize += (texAnmSize&3 != 0) ? (4 - texAnmSize&3) : 0;
    size += texAnmSize;
    size += 6*bsp1.colorEnvAnimData.size();
    size += (size&3 != 0) ? (4-size&3) : 0;
    size += 6*bsp1.colorPrmAnimData.size();
    size += (size&3 != 0) ? (4-size&3) : 0;
    block.write(size);
    block.write(bsp1.flags);
    block.write((u32)0);
    block.write(bsp1.baseSize);
    block.write(bsp1.alphaRef0);
    block.write(bsp1.alphaRef1);
    block.write(bsp1.zModeFlags);
    block.write(bsp1.texFlags);
    block.write(bsp1.colorFlags);
    block.write((u8)bsp1.colorPrmAnimData.size());
    block.write((u8)bsp1.colorEnvAnimData.size());
    block.write(bsp1.colorAnimMaxFrm);
    block.write(bsp1.colorPrm.color_string);
    block.write(bsp1.colorEnv.color_string);
    block.write(bsp1.anmRndm);
    block.write(bsp1.colorLoopOfstMask);
    block.write(bsp1.texIdxLoopOfstMask);
    block.write(std::vector<u8>(3,0));
    if (bsp1.isEnableTexScrollAnm){
        block.write(bsp1.texInitTransX);
        block.write(bsp1.texInitTransY);
        block.write(bsp1.texInitScaleX);
        block.write(bsp1.texInitScaleY);
        block.write(bsp1.texInitRot);
        block.write(bsp1.texIncTransX);
        block.write(bsp1.texIncTransY);
        block.write(bsp1.texIncScaleX);
        block.write(bsp1.texIncScaleY);
        block.write(bsp1.texIncRot);
    }
    if(bsp1.texIdxAnimData.size() != 0){
        block.write(bsp1.texIdxAnimData);
        if (bsp1.texIdxAnimData.size()&3 != 0)
            block.write(std::vector<u8>(4-bsp1.texIdxAnimData.size()&3));
    }    
    if(bsp1.colorPrmAnimData.size() != 0){
        block.data.at(0xD) = block.data.size();
        for(auto& temp : bsp1.colorPrmAnimData){
            block.write(temp.first);
            block.write(temp.second.color_string);
        }
        if (block.data.size()&3 != 0)
            block.write(std::vector<u8>(4-block.data.size()&3,0));
    }
    if(bsp1.colorEnvAnimData.size() != 0){
        block.data.at(0xF) = block.data.size();
        for(auto& temp : bsp1.colorEnvAnimData){
            block.write(temp.first);
            block.write(temp.second.color_string);
        }
        if (block.data.size()&3 != 0)
            block.write(std::vector<u8>(4-block.data.size()&3,0));
    }
    return block;
}
Buffer toBuffer(JPA_ESP1& esp1){
    Buffer block;
    block.write((u32)'ESP1');
    block.write((u32)0x60);
    block.write(esp1.flags);
    block.write(esp1.scaleInTiming);
    block.write(esp1.scaleOutTiming);
    block.write(esp1.scaleInValueX);
    block.write(esp1.scaleOutValueX);
    block.write(esp1.scaleInValueY);
    block.write(esp1.scaleOutValueY);
    block.write(esp1.scaleOutRandom);
    block.write(esp1.scaleAnmMaxFrameX);
    block.write(esp1.scaleAnmMaxFrameY);
    block.write(esp1.alphaInTiming);
    block.write(esp1.alphaOutTiming);
    block.write(esp1.alphaInValue);
    block.write(esp1.alphaBaseValue);
    block.write(esp1.alphaOutValue);
    block.write(esp1.alphaWaveFrequency);
    block.write(esp1.alphaWaveRandom);
    block.write(esp1.alphaWaveAmplitude);
    block.write(esp1.rotateAngle);
    block.write(esp1.rotateAngleRandom);
    block.write(esp1.rotateSpeed);
    block.write(esp1.rotateSpeedRandom);
    block.write(esp1.rotateDirection);
    assert(block.data.size() == 0x60);
    return block;
}
Buffer toBuffer(JPA_ETX1& etx1){
    Buffer block;
    block.write((u32)'ETX1');
    if (etx1.floats.size() != 6 && etx1.floats.size() != 16)
        assert(!"The floats parameter do not match its required size. 6 for JPAC2-10, 16 for 2-11");
    if (etx1.floats.size() == 16) block.write((u32)0x50); // 2-11
    if (etx1.floats.size() == 6) block.write((u32)0x28); // 2-10
    block.write((u16)0);
    block.write(etx1.secondTextureID == 0xFF ? 0 : 1);
    block.write(etx1.indTextureMode&0x1);
    block.write(etx1.floats); // TODO: Add verification for this
    if (etx1.floats.size() == 16) block.write(etx1.unk);
    block.write(etx1.scale);
    block.write(etx1.indTextureID);
    block.write(etx1.secondTextureID == 0xFF ? 0 : etx1.secondTextureID);
    if (etx1.floats.size() == 6) block.write((u8)0);
    return block;
}
Buffer toBuffer(JPA_SSP1& ssp1){
    Buffer block;
    block.write((u32)'SSP1');
    block.write((u32)0x48);
    block.write(ssp1.flags);
    block.write(ssp1.posRndm);
    block.write(ssp1.baseVel);
    block.write(ssp1.baseVelRndm);
    block.write(ssp1.velInfRate);
    block.write(ssp1.gravity);
    block.write(ssp1.globalScale2D);
    block.write(ssp1.inheritScale);
    block.write(ssp1.inheritAlpha);
    block.write(ssp1.inheritRGB);    
    block.write(ssp1.colorPrm.color_string);
    block.write(ssp1.colorEnv.color_string);
    block.write(ssp1.timing);
    block.write(ssp1.life);
    block.write(ssp1.rate);
    block.write(ssp1.step);
    block.write(ssp1.texIdx);
    block.write(ssp1.rotateSpeed);
    assert(block.data.size() == 0x48);
    return block;
}
Buffer toBuffer(JPA_FLD1& fld1){
    Buffer block;
    block.write((u32)'FLD1');
    block.write(0x44);
    block.write(fld1.flags);
    block.write(fld1.pos);
    block.write(fld1.dis);
    block.write(fld1.param1);
    block.write(fld1.param2);
    block.write(fld1.param3);
    block.write(fld1.fadeIn);
    block.write(fld1.fadeOut);
    block.write(fld1.enTime);
    block.write(fld1.disTime);
    block.write(fld1.cycle);
    block.write(std::vector<u8>(3,0));
    assert(0x44 == block.data.size());
    return block;
}
Buffer toBuffer(JPA_KFA1& kfa1){
    Buffer block;
    block.write((u32)'KFA1');
    u32 size = kfa1.keyValues.size();
    u32 block_size = size*4 + 0xC; // key value array size + header
    block.write(block_size); 
    block.write(kfa1.keyType);
    block.write((u8)(size/4)); // number of keys is div by 4 due to each key being 4 floats
    block.write(kfa1.unk0xA);
    block.write((u8)(kfa1.isLoopEnable ? 0x01 : 0x00));
    block.write(kfa1.keyValues);
    assert(block_size == block.data.size());
    return block;
}
Buffer toBuffer(JPA_TDB1& tdb1){
    Buffer block;
    block.write((u32)'TDB1');
    u32 len = tdb1.textures.size() * 2;
    if (len % 4 != 0)
        len += (4 - (len%4)); // Buffers it already.
    len+=0x8;
    block.write(len);
    block.write(tdb1.textureIdx);
    if (block.data.size()&3 != 0)
        block.write(std::vector<u8>(4-block.data.size()&3, 0));
    return block;
}
Buffer toBuffer(JPA_Texture& texture);
Buffer toBuffer(JPA_Resource& resource);
Buffer toBuffer(JPAC& jpc);
#include "jsonToJpc.hpp"

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
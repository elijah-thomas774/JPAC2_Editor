#ifndef JPC_TO_JSON_HPP
#define JPC_TO_JSON_HPP

#include "buffer.hpp"
#include "jpc.hpp"
#include "jsonBase.hpp"
//Functions i want to do here

// read file
// check verion
//  do stuff
// write file

JPA_BEM1 read_BEM1_JPAC2_10(Buffer& block); // done
JPA_BSP1 read_BSP1_JPAC2_10(Buffer& block); // done
JPA_ESP1 read_ESP1_JPAC2_10(Buffer& block); // done
JPA_ETX1 read_ETX1_JPAC2_10(Buffer& block); // done
JPA_SSP1 read_SSP1_JPAC2_10(Buffer& block); // done
JPA_FLD1 read_FLD1_JPAC2_10(Buffer& block); // done
JPA_KFA1 read_KFA1_JPAC2_10(Buffer& block); // done
JPA_TDB1 read_TDB1_JPAC2_10(Buffer& block, u32 num); // done

JPA_BEM1 read_BEM1_JPAC2_11(Buffer& block); // done
JPA_BSP1 read_BSP1_JPAC2_11(Buffer& block); // done
JPA_ESP1 read_ESP1_JPAC2_11(Buffer& block); // done
JPA_ETX1 read_ETX1_JPAC2_11(Buffer& block); // done
JPA_SSP1 read_SSP1_JPAC2_11(Buffer& block); // done
JPA_FLD1 read_FLD1_JPAC2_11(Buffer& block); // done
JPA_KFA1 read_KFA1_JPAC2_11(Buffer& block); // done
JPA_TDB1 read_TDB1_JPAC2_11(Buffer& block, u32 num); // done

JPA_Resource read_resouce_JPAC2_10(Buffer& rawRes); // done
JPA_Resource read_resouce_JPAC2_11(Buffer& rawRes); // done

JPA_Texture read_texture(Buffer& rawTexure); // done

JPAC read_JPAC(Buffer& jpc); // done

void read_jpc(std::string in_file, std::string out_file, std::string texture_folder); // done
void write_json(std::string &out_file, JPAC &jpc); // done
void dump_textures(std::string &texture_folder, std::vector<JPA_Texture> &textures); // done
JPAC read_jpc(std::string in_file);
#endif
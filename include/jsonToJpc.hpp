#ifndef JSON_TO_JPC_HPP
#define JSON_TO_JPC_HPP

#include "buffer.hpp"
#include "jpc.hpp"
#include "jsonBase.hpp"
//Functions i want to do here

Buffer toBuffer(JPA_BEM1&); // done
Buffer toBuffer(JPA_BSP1&); // done
Buffer toBuffer(JPA_ESP1&); // done
Buffer toBuffer(JPA_ETX1&); // done
Buffer toBuffer(JPA_SSP1&); // done
Buffer toBuffer(JPA_FLD1&); // done
Buffer toBuffer(JPA_KFA1&); // done
Buffer toBuffer(JPA_TDB1&); // done
Buffer toBuffer(JPA_Texture&); // done
Buffer toBuffer(JPA_Resource&); // done 
Buffer toBuffer(JPAC&); // done

void write_buffer(std::string&, Buffer&); // done

void edit_from_file(JPAC &jpc, std::string edits);
JPAC load_from_file(std::string source_jpc, std::string texture_folder, JPAC& src);
void write_to_file(JPAC &jpc, std::string dest_file);


#endif
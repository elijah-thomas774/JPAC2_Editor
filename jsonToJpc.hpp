#ifndef JSON_TO_JPC_HPP
#define JSON_TO_JPC_HPP

#include "buffer.hpp"
#include "jpc.hpp"
#include "jsonBase.hpp"
//Functions i want to do here



Buffer toBuffer(JPA_BEM1&);
Buffer toBuffer(JPA_BSP1&); // done
Buffer toBuffer(JPA_ESP1&);
Buffer toBuffer(JPA_ETX1&);
Buffer toBuffer(JPA_SSP1&);
Buffer toBuffer(JPA_FLD1&);
Buffer toBuffer(JPA_KFA1&);
Buffer toBuffer(JPA_TDB1&);
Buffer toBuffer(JPA_Texture&);
Buffer toBuffer(JPA_Resource&);
Buffer toBuffer(JPAC&);

void write_buffer(std::string&, Buffer&);

void edit_from_file(); // This will handle version differences by calling the respectice build func
void load_from_file();
void write_to_file();


#endif
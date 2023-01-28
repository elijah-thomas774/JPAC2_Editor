#ifndef JSON_BASE_HPP
#define JSON_BASE_HPP

#include "json.hpp"
#include "jpc.hpp"
#include <iomanip>
#include <sstream>

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

ordered_json to_json(JPA_BEM1& bem1);
ordered_json to_json(JPA_BSP1& bsp1);
ordered_json to_json(JPA_ESP1& esp1);
ordered_json to_json(JPA_ETX1& etx1);
ordered_json to_json(JPA_SSP1& ssp1);
ordered_json to_json(JPA_FLD1& fld1);
ordered_json to_json(JPA_KFA1& kfa1);
ordered_json to_json(JPA_TDB1& tdb1);
ordered_json to_json(JPA_Texture& texture);
ordered_json to_json(JPA_Resource& resource);
ordered_json to_json(JPAC& jpc);
JPA_BEM1 read_BEM1_from_json(json& j, JPA_BEM1 &orig);
JPA_BSP1 read_BSP1_from_json(json& j, JPA_BSP1 &orig);
JPA_ESP1 read_ESP1_from_json(json& j, JPA_ESP1 &orig);
JPA_ETX1 read_ETX1_from_json(json& j, JPA_ETX1 &orig);
JPA_SSP1 read_SSP1_from_json(json& j, JPA_SSP1 &orig);
JPA_FLD1 read_FLD1_from_json(json& j, JPA_FLD1 &orig);
JPA_KFA1 read_KFA1_from_json(json& j, JPA_KFA1 &orig);
JPA_TDB1 read_TDB1_from_json(json& j, JPA_TDB1 &orig);
JPA_Texture read_Texture_from_json(json& j);
JPA_Resource read_Resource_from_json(json& j, JPA_Resource &res);
JPAC read_JPAC_from_json(json& j, JPAC &orig);
#endif
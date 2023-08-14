#include <cstring>
#include "principals_dto.hpp"
#include "title_dto.hpp"
#include "imdb_bdb_keys.hpp"

KEY_EXTRACTOR_FCT_TYPE Imdb_bdb_key_extractor::key_extractor_fct(const char *key_extractor_name) {
  if (!strcmp(key_extractor_name, "get_principals_name_id"))
    return Principals_DTO::get_principals_name_id;
  if (!strcmp(key_extractor_name, "get_principals_title_id"))
    return Principals_DTO::get_principals_title_id;
  if (!strcmp(key_extractor_name, "get_title_primary_name"))
    return Title_DTO::get_title_primary_name;
  else return nullptr;
}



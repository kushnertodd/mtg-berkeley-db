#include <cstring>
#include "example_keys.hpp"
#include "inventory.hpp"

KEY_EXTRACTOR_FCT_TYPE Example_key_extractor::key_extractor_fct(const char *key_extractor_name) {
  if (!strcmp(key_extractor_name, "get_item_name"))
    return Inventory_DTO::get_item_name;
  else
    return nullptr;
}



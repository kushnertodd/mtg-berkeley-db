#include <cstring>
#include "deck_card_dto.hpp"
#include "mtg_bdb_keys.hpp"

KEY_EXTRACTOR_FCT_TYPE Mtg_bdb_key_extractor::key_extractor_fct(const char *key_extractor_name) {
  if (!strcmp(key_extractor_name, "get_deck_card_deck_id"))
    return Deck_card_DTO::get_deck_card_deck_id;
  if (!strcmp(key_extractor_name, "get_deck_card_card_id"))
    return Deck_card_DTO::get_deck_card_card_id;
  else return nullptr;
}



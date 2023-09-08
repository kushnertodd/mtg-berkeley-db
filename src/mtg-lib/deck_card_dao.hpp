#pragma once

#include <map>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases.hpp"

#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "card_dto.hpp"
#include "deck_dto.hpp"
#include "deck_card_dto.hpp"
#include "bdb_tokens.hpp"

class Deck_card_DAO {
 public:
  static int load(Bdb_dbp &deck_card_db,
                  const std::string &text_file,
                  Bdb_errors &errors,
                  char delimiter = tab);
  static void lookup(Bdb_dbp &deck_card_db,
                     const std::string &deck_card_id,
                     Deck_card_DTO &deck_card_dto,
                     Bdb_errors &errors);
  static void save(Bdb_dbp &deck_card_db,
                   Deck_card_DTO &deck_card_dto,
                   Deck_card_DTO &deck_card_dto_with_key,
                   Bdb_errors &errors);
  static void select_by_card_sdb(Bdb_dbp &deck_card_card_id_sdb,
                                 const std::string &card_id,
                                 Deck_card_DTO_key_list &deck_card_dto_key_list,
                                 Bdb_errors &errors);
  static void select_by_deck_sdb(Bdb_dbp &deck_card_deck_id_sdb,
                                 const std::string &deck_id,
                                 Deck_card_DTO_key_list &deck_card_dto_key_list,
                                 Bdb_errors &errors);
  static void select_by_key_list(Bdb_dbp &deck_card_db,
                                 Deck_card_DTO_key_list &deck_card_dto_key_list,
                                 Deck_card_DTO_list &deck_card_dto_list,
                                 Bdb_errors &errors);
  static void select_cards_for_deck(Bdb_dbp &deck_card_deck_id_sdb,
                                    Bdb_dbp &deck_card_db,
                                    Bdb_dbp &card_db,
                                    const std::string &deck_id,
                                    Card_DTO_list &card_dto_list,
                                    Bdb_errors &errors);
  static void select_card_list(Bdb_dbp &card_db,
                               Deck_card_DTO_list &deck_card_dto_list,
                               Card_DTO_list &card_dto_list,
                               Bdb_errors &errors);
  static void select_decks_for_card(Bdb_dbp &deck_card_card_id_sdb,
                                    Bdb_dbp &deck_card_db,
                                    Bdb_dbp &deck_db,
                                    const std::string &card_id,
                                    Deck_DTO_list &deck_dto_list,
                                    Bdb_errors &errors);
  static void select_deck_list(Bdb_dbp &deck_db,
                               Deck_card_DTO_list &deck_card_key_list,
                               Deck_DTO_list &deck_dto_list,
                               Bdb_errors &errors);
  static void select_other_cards(Bdb_dbp &deck_card_deck_id_sdb,
                                 Bdb_dbp &deck_card_db,
                                 Bdb_dbp &card_db,
                                 const std::string &deck_id,
                                 Card_DTO_list &card_dto_list,
                                 Bdb_errors &errors);
};
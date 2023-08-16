#pragma once

#include <map>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "deck_cards_dto.hpp"
#include "bdb_tokens.hpp"

class Deck_cards_DAO {
 public:
  static int load(Bdb_dbp &deck_cards_db,
                  Bdb_dbp &deck_cards_tripthong_bdb_db,
                  const std::string &text_file,
                  Bdb_errors &errors,
                  char delimiter = tab);

  static void lookup(Bdb_dbp &deck_cards_db,
                     const std::string &deck_cards_id,
                     Deck_cards_DTO &deck_cards_dto,
                     Bdb_errors &errors);

  static void save(Bdb_dbp &deck_cards_db,
                   Deck_cards_DTO &deck_cards_dto,
                   Bdb_errors &errors);

  static void select_all(Bdb_dbp &deck_cards_db,
                         Deck_cards_DTO_list &deck_cards_dto_list,
                         Bdb_errors &errors);

  static void select_all_key(Bdb_dbp &deck_cards_db,
                             Deck_cards_DTO_key &deck_cards_dto_key,
                             Deck_cards_DTO_list &deck_cards_dto_list,
                             Bdb_errors &errors);

  static void select_by_key_list(Bdb_dbp &deck_cards_db,
                                 Deck_cards_DTO_key_list &deck_cards_DTO_key_list,
                                 Deck_cards_DTO_list &deck_cards_DTO_list,
                                 Bdb_errors &errors);

  static void update(Bdb_dbp &deck_cards_db,
                     const std::string &deck_cards_id,
                     const std::string &user_ratings,
                     const std::string &priority,
                     Deck_cards_DTO &deck_cards_dto,
                     Bdb_errors &errors);
};
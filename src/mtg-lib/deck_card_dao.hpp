#pragma once

#include <map>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "deck_card_dto.hpp"
#include "bdb_tokens.hpp"

class Deck_card_DAO {
 public:

  static void lookup(Bdb_dbp &deck_card_db,
                     const std::string &deck_card_id,
                     Deck_card_DTO &deck_card_dto,
                     Bdb_errors &errors);

  static void save(Bdb_dbp &deck_card_db,
                   Deck_card_DTO &deck_card_dto,
                   Bdb_errors &errors);

  static void select_all(Bdb_dbp &deck_card_db,
                         Deck_card_DTO_list &deck_card_dto_list,
                         Bdb_errors &errors);

  static void select_all_key(Bdb_dbp &deck_card_db,
                             Deck_card_DTO_key &deck_card_dto_key,
                             Deck_card_DTO_list &deck_card_dto_list,
                             Bdb_errors &errors);

  static void select_by_key_list(Bdb_dbp &deck_card_db,
                                 Deck_card_DTO_key_list &deck_card_DTO_key_list,
                                 Deck_card_DTO_list &deck_card_DTO_list,
                                 Bdb_errors &errors);
};
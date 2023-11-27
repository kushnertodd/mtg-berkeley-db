#pragma once

#include <map>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "bdb_tokens.hpp"
#include "card_dto.hpp"
#include "deck_dto.hpp"

class Card_DAO {
 public:
  static int load(Bdb_dbp &card_db,
                  Bdb_dbp &card_bdb_triplet_db,
                  const std::string &text_file,
                  Bdb_errors &errors,
                  char delimiter = tab);
  static void lookup(Bdb_dbp &card_db,
                     const std::string &card_id,
                     Card_DTO &card_dto,
                     Bdb_errors &errors);
  static void save(Bdb_dbp &card_db,
                   Card_DTO &card_dto,
                   Bdb_errors &errors);
  static void select_all(Bdb_dbp &card_db,
                         Card_DTO_list &card_dto_list,
                         Bdb_errors &errors);
  static void select_all_for_type_id(Bdb_dbp &card_type_id_sdb,
                                     Bdb_dbp &card_db,
                                     const std::string &type_id,
                                     Card_DTO_list &card_dto_list,
                                     Bdb_errors &errors);
  static void select_all_for_name(Bdb_dbp &card_name_sdb,
                                  Bdb_dbp &card_db,
                                  const std::string &name,
                                  Card_DTO_list &card_dto_list,
                                  Bdb_errors &errors);
  static void update(Bdb_dbp &card_db,
                     Card_DTO &card_dto,
                     Bdb_errors &errors);
};
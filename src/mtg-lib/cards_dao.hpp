#pragma once

#include <map>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "cards_dto.hpp"
#include "bdb_tokens.hpp"

class Cards_DAO {
 public:
  static int load(Bdb_dbp &cards_db,
                  Bdb_dbp &cards_tripthong_bdb_db,
                  const std::string &text_file,
                  Bdb_errors &errors,
                  char delimiter = tab);

  static void lookup(Bdb_dbp &cards_db,
                     const std::string &cards_id,
                     Cards_DTO &cards_dto,
                     Bdb_errors &errors);

  static void save(Bdb_dbp &cards_db,
                   Cards_DTO &cards_dto,
                   Bdb_errors &errors);

  static void select_all(Bdb_dbp &cards_db,
                         Cards_DTO_list &cards_dto_list,
                         Bdb_errors &errors);

  static void select_all_key(Bdb_dbp &cards_db,
                             Cards_DTO_key &cards_dto_key,
                             Cards_DTO_list &cards_dto_list,
                             Bdb_errors &errors);

  static void select_by_key_list(Bdb_dbp &cards_db,
                                 Cards_DTO_key_list &cards_DTO_key_list,
                                 Cards_DTO_list &cards_DTO_list,
                                 Bdb_errors &errors);

  static void update(Bdb_dbp &cards_db,
                     const std::string &cards_id,
                     const std::string &name,
                     const std::string &type_id,
                     Cards_DTO &cards_dto,
                     Bdb_errors &errors);
};
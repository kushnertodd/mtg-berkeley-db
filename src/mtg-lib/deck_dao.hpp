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
#include "deck_dto.hpp"

class Deck_DAO {
 public:
  static int load(Bdb_dbp &deck_db,
                  Bdb_dbp &deck_bdb_triplet_db,
                  const std::string &text_file,
                  Bdb_errors &errors,
                  char delimiter = tab);
  static void lookup(Bdb_dbp &deck_db,
                     const std::string &deck_id,
                     Deck_DTO &deck_dto,
                     Bdb_errors &errors);
  static void save(Bdb_dbp &deck_db,
                   Deck_DTO &deck_dto,
                   Bdb_errors &errors);
  static void select_all(Bdb_dbp &deck_db,
                         Deck_DTO_list &deck_dto_list,
                         Bdb_errors &errors);
  static void select_all_for_account_id(Bdb_dbp &deck_account_id_sdb,
                                        Bdb_dbp &deck_db,
                                        const std::string &account_id,
                                        Deck_DTO_list &deck_dto_list,
                                        Bdb_errors &errors);
  static void select_all_for_name(Bdb_dbp &deck_name_sdb,
                                  Bdb_dbp &deck_db,
                                  const std::string &name,
                                  Deck_DTO_list &deck_dto_list,
                                  Bdb_errors &errors);
  static void update(Bdb_dbp &deck_db,
                     Deck_DTO &deck_dto,
                     Bdb_errors &errors);
};

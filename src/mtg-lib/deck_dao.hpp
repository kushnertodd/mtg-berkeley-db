#pragma once

#include <map>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "deck_dto.hpp"
#include "bdb_tokens.hpp"

class Deck_DAO {
 public:
  static int load(Bdb_dbp &deck_db,
                  Bdb_dbp &deck_triplet_bdb_db,
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

  static void select_all_account_id(Bdb_dbp &deck_db,
                                    Bdb_dbp &deckemail_sdb,
                                    const std::string &account_id,
                                    Deck_DTO_list &deck_dto_list,
                                    Bdb_errors &errors);

  static void update(Bdb_dbp &deck_db,
                     const std::string &deck_id,
                     const std::string &account_id,
                     const std::string &name,
                     Deck_DTO &deck_dto,
                     Bdb_errors &errors);
};

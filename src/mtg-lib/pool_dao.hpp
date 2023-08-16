#pragma once

#include <map>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "pool_dto.hpp"
#include "bdb_tokens.hpp"

class Pool_DAO {
 public:
  static int load(Bdb_dbp &pool_db,
                  Bdb_dbp &pool_tripthong_bdb_db,
                  const std::string &text_file,
                  Bdb_errors &errors,
                  char delimiter = tab);

  static void lookup(Bdb_dbp &pool_db,
                     const std::string &pool_id,
                     Pool_DTO &pool_dto,
                     Bdb_errors &errors);

  static void save(Bdb_dbp &pool_db,
                   Pool_DTO &pool_dto,
                   Bdb_errors &errors);

  static void select_all(Bdb_dbp &pool_db,
                         Pool_DTO_list &pool_dto_list,
                         Bdb_errors &errors);

  static void select_all_key(Bdb_dbp &pool_db,
                             Pool_DTO_key &pool_dto_key,
                             Pool_DTO_list &pool_dto_list,
                             Bdb_errors &errors);

  static void select_by_key_list(Bdb_dbp &pool_db,
                                 Pool_DTO_key_list &pool_DTO_key_list,
                                 Pool_DTO_list &pool_DTO_list,
                                 Bdb_errors &errors);

  static void update(Bdb_dbp &pool_db,
                     const std::string &pool_id,
                     const std::string &name,
                     Pool_DTO &pool_dto,
                     Bdb_errors &errors);
};
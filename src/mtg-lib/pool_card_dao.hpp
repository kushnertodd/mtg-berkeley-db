#pragma once

#include <map>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "pool_card_dto.hpp"
#include "bdb_tokens.hpp"

class Pool_card_DAO {
 public:

  static void lookup(Bdb_dbp &pool_card_db,
                     const std::string &pool_card_id,
                     Pool_card_DTO &pool_card_dto,
                     Bdb_errors &errors);

  static void save(Bdb_dbp &pool_card_db,
                   Pool_card_DTO &pool_card_dto,
                   Bdb_errors &errors);

  static void select_all(Bdb_dbp &pool_card_db,
                         Pool_card_DTO_list &pool_card_dto_list,
                         Bdb_errors &errors);

  static void select_all_key(Bdb_dbp &pool_card_db,
                             Pool_card_DTO_key &pool_card_dto_key,
                             Pool_card_DTO_list &pool_card_dto_list,
                             Bdb_errors &errors);

  static void select_by_key_list(Bdb_dbp &pool_card_db,
                                 Pool_card_DTO_key_list &pool_card_DTO_key_list,
                                 Pool_card_DTO_list &pool_card_DTO_list,
                                 Bdb_errors &errors);
};
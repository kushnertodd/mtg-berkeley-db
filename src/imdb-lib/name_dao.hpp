#pragma once

#include <map>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "name_dto.hpp"
#include "bdb_tokens.hpp"

class Name_DAO {
 public:
  static int load(Bdb_dbp &name_db,
                  Bdb_dbp &name_tripthong_bdb_db,
                  const std::string &text_file,
                  Bdb_errors &errors,
                  char delimiter = tab);

  static void lookup(Bdb_dbp &name_db,
                     const std::string &name_id,
                     Name_DTO &name_dto,
                     Bdb_errors &errors);

  static void save(Bdb_dbp &name_db,
                   Name_DTO &name_dto,
                   Bdb_errors &errors);

  static void select_all(Bdb_dbp &name_db,
                         Name_DTO_list &name_dto_list,
                         Bdb_errors &errors);

  static void select_all_key(Bdb_dbp &name_db,
                             Name_DTO_key &name_dto_key,
                             Name_DTO_list &name_dto_list,
                             Bdb_errors &errors);

  static void select_by_key_list(Bdb_dbp &name_db,
                                 Name_DTO_key_list &name_DTO_key_list,
                                 Name_DTO_list &name_DTO_list,
                                 Bdb_errors &errors);

  static void update(Bdb_dbp &name_db,
                     const std::string &name_id,
                     const std::string &user_ratings,
                     const std::string &priority,
                     Name_DTO &name_dto,
                     Bdb_errors &errors);
};
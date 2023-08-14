#pragma once

#include <map>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "title_dto.hpp"
#include "bdb_tokens.hpp"

class Title_DAO {
 public:
  static int load(Bdb_dbp &title_db,
                  Bdb_dbp &title_tripthong_bdb_db,
                  const std::string &text_file,
                  Bdb_errors &errors,
                  char delimiter = tab);

  static void lookup(Bdb_dbp &title_db,
                     Bdb_dbp &ratings_db,
                     const std::string &title_id,
                     Title_DTO &title_dto,
                     Bdb_errors &errors);

  static void save(Bdb_dbp &title_db,
                   Title_DTO &title_dto,
                   Bdb_errors &errors);

  static void select_all(Bdb_dbp &title_db,
                         Bdb_dbp &ratings_db,
                         Title_DTO_list &title_dto_list,
                         Bdb_errors &errors);

  static void select_all_key(Bdb_dbp &title_db,
                             Title_DTO_key &title_dto_key,
                             Title_DTO_list &title_dto_list,
                             Bdb_errors &errors);

  static void select_by_key_list(Bdb_dbp &title_db,
                                 Title_DTO_key_list &title_dto_key_list,
                                 Title_DTO_list &title_dto_list,
                                 Title_DTO &title_dto,
                                 Bdb_errors &errors);
  static void update(Bdb_dbp &title_db,
                     Bdb_dbp &ratings_db,
                     const std::string &title_id,
                     const std::string &user_ratings,
                     const std::string &priority,
                     Title_DTO &title_dto,
                     Bdb_errors &errors);
};
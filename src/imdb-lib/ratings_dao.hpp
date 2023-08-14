#pragma once

#include <map>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "ratings_dto.hpp"
#include "bdb_tokens.hpp"

class Ratings_DAO {
 public:
  static int load(Bdb_dbp &ratings_db, const std::string &text_file, Bdb_errors &errors, char delimiter = tab);

  static void lookup(Bdb_dbp &ratings_db, const std::string &ratings_id, Ratings_DTO &ratings_dto, Bdb_errors &errors);

  static void save(Bdb_dbp &ratings_db, Ratings_DTO &ratings_dto, Bdb_errors &errors);

  static void select_all(Bdb_dbp &ratings_db, Ratings_DTO_list &ratings_dto_list, Bdb_errors &errors);
};
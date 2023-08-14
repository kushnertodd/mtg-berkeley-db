#pragma once

#include <map>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases.hpp"

#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "principals_dto.hpp"
#include "title_dto.hpp"
#include "bdb_tokens.hpp"

class Principals_DAO {
 public:
  static int load(Bdb_dbp &principals_db,
                  const std::string &text_file,
                  Bdb_errors &errors, char delimiter = tab);

  static void lookup(Bdb_dbp &principals_db,
                     const std::string &principals_id,
                     Principals_DTO &principals_dto,
                     Bdb_errors &errors);

  static void save(Bdb_dbp &principals_db,
                   Principals_DTO &principals_dto,
                   Bdb_errors &errors);

  static void search_by_name(Bdb_dbp &principals_name_id_sdb,
                             Bdb_dbp &principals_db,
                             Bdb_dbp &title_db,
                             Bdb_dbp &ratings_db,
                             const std::string &name_id,
                             Title_DTO_list &title_dto_list,
                             Bdb_errors &errors);

  static void search_by_name_sdb(Bdb_dbp &principals_name_id_sdb,
                                 const std::string &name_id,
                                 Principals_DTO_key_list &principals_dto_key_list,
                                 Bdb_errors &errors);

  static void search_by_title(Bdb_dbp &principals_title_id_sdb,
                              Bdb_dbp &principals_db,
                              Bdb_dbp &name_db,
                              const std::string &title_id,
                              Name_DTO_list &name_dto_list,
                              Bdb_errors &errors);

  static void search_by_title_sdb(Bdb_dbp &principals_title_id_sdb,
                                  const std::string &title_id,
                                  Principals_DTO_key_list &principals_DTO_key_list,
                                  Bdb_errors &errors);

  static void select_all(Bdb_dbp &principals_db,
                         Principals_DTO_list &principals_dto_list,
                         Bdb_errors &errors);

  static void select_all_key(Bdb_dbp &title_db,
                             Principals_DTO_key &principals_dto_key,
                             Principals_DTO_list &principals_dto_list,
                             Bdb_errors &errors);

  static void select_by_key_list(Bdb_dbp &principals_db,
                                 Principals_DTO_key_list &principals_DTO_key_list,
                                 Principals_DTO_list &principals_DTO_list,
                                 Bdb_errors &errors);

  static void select_name_list(Bdb_dbp &name_db,
                               Principals_DTO_list &principals_key_list,
                               Name_DTO_list &name_DTO_list,
                               Bdb_errors &errors);

  static void select_title_list(Bdb_dbp &title_db,
                                Bdb_dbp &ratings_db,
                                Principals_DTO_list &principals_dto_list,
                                Title_DTO_list &title_dto_list,
                                Bdb_errors &errors);
};
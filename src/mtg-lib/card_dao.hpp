#pragma once

#include <map>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "card_dto.hpp"
#include "bdb_tokens.hpp"

class Card_DAO {
 public:
  static int load(Bdb_dbp &card_db,
                  Bdb_dbp &card_tripthong_bdb_db,
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

  static void select_all_type_id(Bdb_dbp &account_db,
                                 Bdb_dbp &account_email_sdb,
                                 const std::string &type_id,
                                 Card_DTO_list &card_dto_list,
                                 Bdb_errors &errors);

  static void update(Bdb_dbp &card_db,
                     const std::string &card_id,
                     const std::string &name,
                     const std::string &type_id,
                     Card_DTO &card_dto,
                     Bdb_errors &errors);
};
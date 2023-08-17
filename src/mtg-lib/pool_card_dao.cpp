#include "bdb_dao.hpp"
#include "pool_card_dto.hpp"
#include "pool_card_dao.hpp"

/*!
 * @brief load pool_card database from delimited record file
 * @param pool_card_db pool_card database to which to save
 * @param text_file pool_card records with fields corresponding to dto
 * @param errors if save fails
 * @param delimiter record delimiter
 * @return count of records saved
 */
int Pool_card_DAO::load(Bdb_dbp &pool_card_db,
                   Bdb_dbp &pool_card_tripthong_bdb_db,
                   const std::string &text_file,
                   Bdb_errors &errors,
                   char delimiter) {
  return Bdb_DAO::load<Pool_card_DTO_key,
                       Pool_card_DTO>(pool_card_db, text_file, errors, delimiter);
}

/*!
 * @brief select pool_card from database based on pool_card id
 * @param pool_card_db pool_card database to which to save
 * @param pool_card_id pool_card record key
 * @param pool_card_dto pool_card dto to save
 * @param errors if save fails
 */
void Pool_card_DAO::lookup(Bdb_dbp &pool_card_db,
                      const std::string &pool_card_id,
                      Pool_card_DTO &pool_card_dto,
                      Bdb_errors &errors) {
  Pool_card_DTO_key pool_card_dto_key(pool_card_id);
  Bdb_DAO::lookup<Pool_card_DTO_key,
                  Pool_card_DTO>(pool_card_db, pool_card_dto_key, pool_card_dto, errors);
}

/*!
 * @brief save pool_card dto to pool_card database
 * @param pool_card_db pool_card database to which to save
 * @param pool_card_dto prinicpals dto to save
 * @param errors if save fails
 */
void Pool_card_DAO::save(Bdb_dbp &pool_card_db, Pool_card_DTO &pool_card_dto, Bdb_errors &errors) {
  Pool_card_DTO_key pool_card_dto_key(pool_card_dto);
  Bdb_DAO::save<Pool_card_DTO_key,
                Pool_card_DTO>(pool_card_db, pool_card_dto_key, pool_card_dto, errors);
}

/*!
 * @brief select all pool_card dto records from pool_card database
 * @param pool_card_db pool_card database to select from
 * @param pool_card_dto_list all pool_card dtos in database
 * @param errors if select fails
 */
void Pool_card_DAO::select_all(Bdb_dbp &pool_card_db, Pool_card_DTO_list &pool_card_dto_list, Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(pool_card_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_all<Pool_card_DTO_key,
                                Pool_card_DTO,
                                Pool_card_DTO_list>(pool_card_dto_list, errors);
}

/*!
 * @brief select all pool_card dto records from database from key
 * @param pool_card_db pool_card database to select from
 * @param pool_card_dto_list all pool_card dtos in database matching key
 * @param errors if select fails
 * @details selects all duplicate records from key if duplicates allowed
 */
void Pool_card_DAO::select_all_key(Bdb_dbp &pool_card_db,
                              Pool_card_DTO_key &pool_card_dto_key,
                              Pool_card_DTO_list &pool_card_dto_list,
                              Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(pool_card_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_key<Pool_card_DTO_key,
                                Pool_card_DTO,
                                Pool_card_DTO_list>(pool_card_dto_key, pool_card_dto_list, errors);
}

/*!
 * @brief select pool_card key list using pool_card id to search pool_card id->pool_card key secondary database
 * @param pool_card_pool_card_id_sdb pool_card id->pool_card key secondary database
 * @param pool_card_id secondary database search key
 * @param pool_card_DTO_key_list selected pool_card key list
 * @param errors if pool_card key not found
 */
void Pool_card_DAO::select_by_key_list(Bdb_dbp &pool_card_db,
                                  Pool_card_DTO_key_list &pool_card_DTO_key_list,
                                  Pool_card_DTO_list &pool_card_DTO_list,
                                  Bdb_errors &errors) {
  for (Pool_card_DTO_key &pool_card_DTO_key: pool_card_DTO_key_list.list) {
    Pool_card_DTO pool_card_dto;
    Bdb_DAO::lookup<Pool_card_DTO_key,
                    Pool_card_DTO>(pool_card_db,
                              pool_card_DTO_key,
                              pool_card_dto,
                              errors);
    if (!errors.has())
      pool_card_DTO_list.add(pool_card_dto);
    else
      break;
  }
}

void Pool_card_DAO::update(Bdb_dbp &pool_card_db,
                      const std::string &pool_card_id,
                      const std::string &user_ratings,
                      const std::string &priority,
                      Pool_card_DTO &pool_card_dto,
                      Bdb_errors &errors) {
  Pool_card_DTO_key pool_card_dto_key(pool_card_id);
  Bdb_DAO::lookup<Pool_card_DTO_key,
                  Pool_card_DTO>(pool_card_db, pool_card_dto_key, pool_card_dto, errors);
  pool_card_dto.user_rating = user_ratings;
  pool_card_dto.priority = priority;
  Bdb_DAO::save<Pool_card_DTO_key,
                Pool_card_DTO>(pool_card_db, pool_card_dto_key, pool_card_dto, errors);
}
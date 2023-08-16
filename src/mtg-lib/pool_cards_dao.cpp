#include "bdb_dao.hpp"
#include "pool_cards_dto.hpp"
#include "pool_cards_dao.hpp"

/*!
 * @brief load pool_cards database from delimited record file
 * @param pool_cards_db pool_cards database to which to save
 * @param text_file pool_cards records with fields corresponding to dto
 * @param errors if save fails
 * @param delimiter record delimiter
 * @return count of records saved
 */
int Pool_cards_DAO::load(Bdb_dbp &pool_cards_db,
                   Bdb_dbp &pool_cards_tripthong_bdb_db,
                   const std::string &text_file,
                   Bdb_errors &errors,
                   char delimiter) {
  return Bdb_DAO::load<Pool_cards_DTO_key,
                       Pool_cards_DTO>(pool_cards_db, text_file, errors, delimiter);
}

/*!
 * @brief select pool_cards from database based on pool_cards id
 * @param pool_cards_db pool_cards database to which to save
 * @param pool_cards_id pool_cards record key
 * @param pool_cards_dto pool_cards dto to save
 * @param errors if save fails
 */
void Pool_cards_DAO::lookup(Bdb_dbp &pool_cards_db,
                      const std::string &pool_cards_id,
                      Pool_cards_DTO &pool_cards_dto,
                      Bdb_errors &errors) {
  Pool_cards_DTO_key pool_cards_dto_key(pool_cards_id);
  Bdb_DAO::lookup<Pool_cards_DTO_key,
                  Pool_cards_DTO>(pool_cards_db, pool_cards_dto_key, pool_cards_dto, errors);
}

/*!
 * @brief save pool_cards dto to pool_cards database
 * @param pool_cards_db pool_cards database to which to save
 * @param pool_cards_dto prinicpals dto to save
 * @param errors if save fails
 */
void Pool_cards_DAO::save(Bdb_dbp &pool_cards_db, Pool_cards_DTO &pool_cards_dto, Bdb_errors &errors) {
  Pool_cards_DTO_key pool_cards_dto_key(pool_cards_dto);
  Bdb_DAO::save<Pool_cards_DTO_key,
                Pool_cards_DTO>(pool_cards_db, pool_cards_dto_key, pool_cards_dto, errors);
}

/*!
 * @brief select all pool_cards dto records from pool_cards database
 * @param pool_cards_db pool_cards database to select from
 * @param pool_cards_dto_list all pool_cards dtos in database
 * @param errors if select fails
 */
void Pool_cards_DAO::select_all(Bdb_dbp &pool_cards_db, Pool_cards_DTO_list &pool_cards_dto_list, Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(pool_cards_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_all<Pool_cards_DTO_key,
                                Pool_cards_DTO,
                                Pool_cards_DTO_list>(pool_cards_dto_list, errors);
}

/*!
 * @brief select all pool_cards dto records from database from key
 * @param pool_cards_db pool_cards database to select from
 * @param pool_cards_dto_list all pool_cards dtos in database matching key
 * @param errors if select fails
 * @details selects all duplicate records from key if duplicates allowed
 */
void Pool_cards_DAO::select_all_key(Bdb_dbp &pool_cards_db,
                              Pool_cards_DTO_key &pool_cards_dto_key,
                              Pool_cards_DTO_list &pool_cards_dto_list,
                              Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(pool_cards_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_key<Pool_cards_DTO_key,
                                Pool_cards_DTO,
                                Pool_cards_DTO_list>(pool_cards_dto_key, pool_cards_dto_list, errors);
}

/*!
 * @brief select pool_cards key list using pool_cards id to search pool_cards id->pool_cards key secondary database
 * @param pool_cards_pool_cards_id_sdb pool_cards id->pool_cards key secondary database
 * @param pool_cards_id secondary database search key
 * @param pool_cards_DTO_key_list selected pool_cards key list
 * @param errors if pool_cards key not found
 */
void Pool_cards_DAO::select_by_key_list(Bdb_dbp &pool_cards_db,
                                  Pool_cards_DTO_key_list &pool_cards_DTO_key_list,
                                  Pool_cards_DTO_list &pool_cards_DTO_list,
                                  Bdb_errors &errors) {
  for (Pool_cards_DTO_key &pool_cards_DTO_key: pool_cards_DTO_key_list.list) {
    Pool_cards_DTO pool_cards_dto;
    Bdb_DAO::lookup<Pool_cards_DTO_key,
                    Pool_cards_DTO>(pool_cards_db,
                              pool_cards_DTO_key,
                              pool_cards_dto,
                              errors);
    if (!errors.has())
      pool_cards_DTO_list.add(pool_cards_dto);
    else
      break;
  }
}

void Pool_cards_DAO::update(Bdb_dbp &pool_cards_db,
                      const std::string &pool_cards_id,
                      const std::string &user_ratings,
                      const std::string &priority,
                      Pool_cards_DTO &pool_cards_dto,
                      Bdb_errors &errors) {
  Pool_cards_DTO_key pool_cards_dto_key(pool_cards_id);
  Bdb_DAO::lookup<Pool_cards_DTO_key,
                  Pool_cards_DTO>(pool_cards_db, pool_cards_dto_key, pool_cards_dto, errors);
  pool_cards_dto.user_rating = user_ratings;
  pool_cards_dto.priority = priority;
  Bdb_DAO::save<Pool_cards_DTO_key,
                Pool_cards_DTO>(pool_cards_db, pool_cards_dto_key, pool_cards_dto, errors);
}
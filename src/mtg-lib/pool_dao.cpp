#include "bdb_dao.hpp"
#include "pool_dto.hpp"
#include "pool_dao.hpp"

/*!
 * @brief load pool database from delimited record file
 * @param pool_db pool database to which to save
 * @param text_file pool records with fields corresponding to dto
 * @param errors if save fails
 * @param delimiter record delimiter
 * @return count of records saved
 */
int Pool_DAO::load(Bdb_dbp &pool_db,
                   Bdb_dbp &pool_tripthong_bdb_db,
                   const std::string &text_file,
                   Bdb_errors &errors,
                   char delimiter) {
  return Bdb_DAO::load<Pool_DTO_key,
                       Pool_DTO>(pool_db, text_file, errors, delimiter);
}

/*!
 * @brief select pool from database based on pool id
 * @param pool_db pool database to which to save
 * @param pool_id pool record key
 * @param pool_dto pool dto to save
 * @param errors if save fails
 */
void Pool_DAO::lookup(Bdb_dbp &pool_db,
                      const std::string &pool_id,
                      Pool_DTO &pool_dto,
                      Bdb_errors &errors) {
  Pool_DTO_key pool_dto_key(pool_id);
  Bdb_DAO::lookup<Pool_DTO_key,
                  Pool_DTO>(pool_db, pool_dto_key, pool_dto, errors);
}

/*!
 * @brief save pool dto to pool database
 * @param pool_db pool database to which to save
 * @param pool_dto prinicpals dto to save
 * @param errors if save fails
 */
void Pool_DAO::save(Bdb_dbp &pool_db, Pool_DTO &pool_dto, Bdb_errors &errors) {
  Pool_DTO_key pool_dto_key(pool_dto);
  Bdb_DAO::save<Pool_DTO_key,
                Pool_DTO>(pool_db, pool_dto_key, pool_dto, errors);
}

/*!
 * @brief select all pool dto records from pool database
 * @param pool_db pool database to select from
 * @param pool_dto_list all pool dtos in database
 * @param errors if select fails
 */
void Pool_DAO::select_all(Bdb_dbp &pool_db, Pool_DTO_list &pool_dto_list, Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(pool_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_all<Pool_DTO_key,
                                Pool_DTO,
                                Pool_DTO_list>(pool_dto_list, errors);
}

/*!
 * @brief select all pool dto records from database from key
 * @param pool_db pool database to select from
 * @param pool_dto_list all pool dtos in database matching key
 * @param errors if select fails
 * @details selects all duplicate records from key if duplicates allowed
 */
void Pool_DAO::select_all_key(Bdb_dbp &pool_db,
                              Pool_DTO_key &pool_dto_key,
                              Pool_DTO_list &pool_dto_list,
                              Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(pool_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_key<Pool_DTO_key,
                                Pool_DTO,
                                Pool_DTO_list>(pool_dto_key, pool_dto_list, errors);
}

/*!
 * @brief select pool key list using pool id to search pool id->pool key secondary database
 * @param pool_pool_id_sdb pool id->pool key secondary database
 * @param pool_id secondary database search key
 * @param pool_DTO_key_list selected pool key list
 * @param errors if pool key not found
 */
void Pool_DAO::select_by_key_list(Bdb_dbp &pool_db,
                                  Pool_DTO_key_list &pool_DTO_key_list,
                                  Pool_DTO_list &pool_DTO_list,
                                  Bdb_errors &errors) {
  for (Pool_DTO_key &pool_DTO_key: pool_DTO_key_list.list) {
    Pool_DTO pool_dto;
    Bdb_DAO::lookup<Pool_DTO_key,
                    Pool_DTO>(pool_db,
                              pool_DTO_key,
                              pool_dto,
                              errors);
    if (!errors.has())
      pool_DTO_list.add(pool_dto);
    else
      break;
  }
}

void Pool_DAO::update(Bdb_dbp &pool_db,
                      const std::string &pool_id,
                      const std::string &name,
                      Pool_DTO &pool_dto,
                      Bdb_errors &errors) {
  Pool_DTO_key pool_dto_key(pool_id);
  Bdb_DAO::lookup<Pool_DTO_key,
                  Pool_DTO>(pool_db, pool_dto_key, pool_dto, errors);
  pool_dto.name = name;
  Bdb_DAO::save<Pool_DTO_key,
                Pool_DTO>(pool_db, pool_dto_key, pool_dto, errors);
}
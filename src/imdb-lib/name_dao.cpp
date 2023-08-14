#include "bdb_dao.hpp"
#include "name_dto.hpp"
#include "name_dao.hpp"

/*!
 * @brief load name database from delimited record file
 * @param name_db name database to which to save
 * @param text_file name records with fields corresponding to dto
 * @param errors if save fails
 * @param delimiter record delimiter
 * @return count of records saved
 */
int Name_DAO::load(Bdb_dbp &name_db,
                   Bdb_dbp &name_tripthong_bdb_db,
                   const std::string &text_file,
                   Bdb_errors &errors,
                   char delimiter) {
  return Bdb_DAO::load_tripthongs<Name_DTO_key,
                       Name_DTO>(name_db,
                                 name_tripthong_bdb_db,
                                 text_file,
                                 errors,
                                 delimiter);
}

/*!
 * @brief select name from database based on name id
 * @param name_db name database to which to save
 * @param name_id name record key
 * @param name_dto name dto to save
 * @param errors if save fails
 */
void Name_DAO::lookup(Bdb_dbp &name_db,
                      const std::string &name_id,
                      Name_DTO &name_dto,
                      Bdb_errors &errors) {
  Name_DTO_key name_dto_key(name_id);
  Bdb_DAO::lookup<Name_DTO_key,
                  Name_DTO>(name_db, name_dto_key, name_dto, errors);
}

/*!
 * @brief save name dto to name database
 * @param name_db name database to which to save
 * @param name_dto prinicpals dto to save
 * @param errors if save fails
 */
void Name_DAO::save(Bdb_dbp &name_db, Name_DTO &name_dto, Bdb_errors &errors) {
  Name_DTO_key name_dto_key(name_dto);
  Bdb_DAO::save<Name_DTO_key,
                Name_DTO>(name_db, name_dto_key, name_dto, errors);
}

/*!
 * @brief select all name dto records from name database
 * @param name_db name database to select from
 * @param name_dto_list all name dtos in database
 * @param errors if select fails
 */
void Name_DAO::select_all(Bdb_dbp &name_db, Name_DTO_list &name_dto_list, Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(name_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_all<Name_DTO_key,
                                Name_DTO,
                                Name_DTO_list>(name_dto_list, errors);
}

/*!
 * @brief select all name dto records from database from key
 * @param name_db name database to select from
 * @param name_dto_list all name dtos in database matching key
 * @param errors if select fails
 * @details selects all duplicate records from key if duplicates allowed
 */
void Name_DAO::select_all_key(Bdb_dbp &name_db,
                              Name_DTO_key &name_dto_key,
                              Name_DTO_list &name_dto_list,
                              Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(name_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_key<Name_DTO_key,
                                Name_DTO,
                                Name_DTO_list>(name_dto_key, name_dto_list, errors);
}

/*!
 * @brief select name key list using name id to search name id->name key secondary database
 * @param name_name_id_sdb name id->name key secondary database
 * @param name_id secondary database search key
 * @param name_DTO_key_list selected name key list
 * @param errors if name key not found
 */
void Name_DAO::select_by_key_list(Bdb_dbp &name_db,
                                  Name_DTO_key_list &name_DTO_key_list,
                                  Name_DTO_list &name_DTO_list,
                                  Bdb_errors &errors) {
  for (Name_DTO_key &name_DTO_key: name_DTO_key_list.list) {
    Name_DTO name_dto;
    Bdb_DAO::lookup<Name_DTO_key,
                    Name_DTO>(name_db,
                              name_DTO_key,
                              name_dto,
                              errors);
    if (!errors.has())
      name_DTO_list.add(name_dto);
    else
      break;
  }
}

void Name_DAO::update(Bdb_dbp &name_db,
                      const std::string &name_id,
                      const std::string &user_ratings,
                      const std::string &priority,
                      Name_DTO &name_dto,
                      Bdb_errors &errors) {
  Name_DTO_key name_dto_key(name_id);
  Bdb_DAO::lookup<Name_DTO_key,
                  Name_DTO>(name_db, name_dto_key, name_dto, errors);
  name_dto.user_rating = user_ratings;
  name_dto.priority = priority;
  Bdb_DAO::save<Name_DTO_key,
                Name_DTO>(name_db, name_dto_key, name_dto, errors);
}
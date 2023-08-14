#include "bdb_dao.hpp"
#include "name_dto.hpp"
#include "principals_dto.hpp"
#include "title_dao.hpp"
#include "title_dto.hpp"
#include "principals_dao.hpp"

/*!
 * @brief load principals database from delimited record file
 * @param principals_db principals database to which to save
 * @param text_file principals records with fields corresponding to dto
 * @param errors if save fails
 * @param delimiter record delimiter
 * @return count of records saved
 */
int Principals_DAO::load(Bdb_dbp &principals_db, const std::string &text_file, Bdb_errors &errors, char delimiter) {
  return Bdb_DAO::load<Principals_DTO_key,
                       Principals_DTO>(principals_db, text_file, errors, delimiter);
}

/*!
 * @brief select principals from database based on principals id
 * @param principals_db principals database to which to save
 * @param principals_id principals record key
 * @param principals_dto principals dto to save
 * @param errors if save fails
 */
void Principals_DAO::lookup(Bdb_dbp &principals_db,
                            const std::string &principals_id,
                            Principals_DTO &principals_dto,
                            Bdb_errors &errors) {
  Principals_DTO_key principals_DTO_key(principals_id);
  Bdb_DAO::lookup<Principals_DTO_key,
                  Principals_DTO>(principals_db, principals_DTO_key, principals_dto, errors);
}

/*!
 * @brief save principals dto to principals database
 * @param principals_db principals database to which to save
 * @param principals_dto prinicpals dto to save
 * @param errors if save fails
 */
void Principals_DAO::save(Bdb_dbp &principals_db, Principals_DTO &principals_dto, Bdb_errors &errors) {
  Principals_DTO_key principals_dto_key(principals_dto);
  Bdb_DAO::save<Principals_DTO_key,
                Principals_DTO>(principals_db, principals_dto_key, principals_dto, errors);
}

/*!
 * @brief select all title records corresponding to name id via name id->principals key secondary database
 * @param principals_name_id_sdb name id->principals key secondary database
 * @param principals_db principals database
 * @param title_db titles database
 * @param name_id secondary database search key
 * @param title_dto_list selected titles list
 * @param errors if name key not found
 */
void Principals_DAO::search_by_name(Bdb_dbp &principals_name_id_sdb,
                                    Bdb_dbp &principals_db,
                                    Bdb_dbp &title_db,
                                    Bdb_dbp &ratings_db,
                                    const std::string &name_id,
                                    Title_DTO_list &title_dto_list,
                                    Bdb_errors &errors) {
  Principals_DTO_key_list principals_dto_key_list;
  search_by_name_sdb(principals_name_id_sdb, name_id, principals_dto_key_list, errors);
  Principals_DTO_list principals_dto_list;
  select_by_key_list(principals_db, principals_dto_key_list, principals_dto_list, errors);
  select_title_list(title_db, ratings_db, principals_dto_list, title_dto_list, errors);
}

/*!
 * @brief select principals key list using name id to search name id->principals key secondary database
 * @param principals_name_id_sdb name id->principals key secondary database
 * @param name_id secondary database search key
 * @param principals_DTO_key_list selected principals key list
 * @param errors if name key not found
 */
void Principals_DAO::search_by_name_sdb(Bdb_dbp &principals_name_id_sdb,
                                        const std::string &name_id,
                                        Principals_DTO_key_list &principals_DTO_key_list,
                                        Bdb_errors &errors) {
  Name_DTO_key name_dto_key(name_id);
  Bdb_cursor bdb_cursor(principals_name_id_sdb, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_key<Name_DTO_key,
                                Principals_DTO_key,
                                Principals_DTO_key_list>(name_dto_key,
                                                         principals_DTO_key_list,
                                                         errors);
}

/*!
 * @brief select all name records corresponding to title id via title id->principals key secondary database
 * @param principals_title_id_sdb title id->principals key secondary database
 * @param principals_db principals database
 * @param name_db names database
 * @param title_id secondary database search key
 * @param name_dto_list selected names list
 * @param errors if title key not found
 */
void Principals_DAO::search_by_title(Bdb_dbp &principals_title_id_sdb,
                                     Bdb_dbp &principals_db,
                                     Bdb_dbp &name_db,
                                     const std::string &title_id,
                                     Name_DTO_list &name_dto_list,
                                     Bdb_errors &errors) {
  Principals_DTO_key_list principals_dto_key_list;
  search_by_title_sdb(principals_title_id_sdb, title_id, principals_dto_key_list, errors);
  Principals_DTO_list principals_dto_list;
  select_by_key_list(principals_db, principals_dto_key_list, principals_dto_list, errors);
  select_name_list(name_db, principals_dto_list, name_dto_list, errors);
}

/*!
 * @brief select principals key list using name id to search name id->principals key secondary database
 * @param principals_name_id_sdb name id->principals key secondary database
 * @param name_id secondary database search key
 * @param principals_DTO_key_list selected principals key list
 * @param errors if name key not found
 */
void Principals_DAO::search_by_title_sdb(Bdb_dbp &principals_title_id_sdb,
                                         const std::string &title_id,
                                         Principals_DTO_key_list &principals_DTO_key_list,
                                         Bdb_errors &errors) {
  Title_DTO_key title_dto_key(title_id);
  Bdb_cursor bdb_cursor(principals_title_id_sdb, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_key<Title_DTO_key,
                                Principals_DTO_key,
                                Principals_DTO_key_list>(title_dto_key,
                                                         principals_DTO_key_list,
                                                         errors);
}

/*!
 * @brief select all principals dto records from principals database
 * @param principals_db principals database to select from
 * @param principals_dto_list all principals dtos in database matching key
 * @param errors if select fails
 */
void Principals_DAO::select_all(Bdb_dbp &principals_db,
                                Principals_DTO_list &principals_dto_list,
                                Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(principals_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_all<Principals_DTO_key,
                                Principals_DTO,
                                Principals_DTO_list>(principals_dto_list, errors);
}

/*!
 * @brief select all name dto records from database from key
 * @param principals_db principals database to select from
 * @param principals_dto_list all principals dtos in database matching key
 * @param errors if select fails
 * @details selects all duplicate records from key if duplicates allowed
 */
void Principals_DAO::select_all_key(Bdb_dbp &principals_db,
                                    Principals_DTO_key &principals_dto_key,
                                    Principals_DTO_list &principals_dto_list,
                                    Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(principals_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_key<Principals_DTO_key,
                                Principals_DTO,
                                Principals_DTO_list>(principals_dto_key,
                                                     principals_dto_list,
                                                     errors);
}

/*!
 * @brief select principals key list using name id to search name id->principals key secondary database
 * @param principals_name_id_sdb name id->principals key secondary database
 * @param name_id secondary database search key
 * @param principals_DTO_key_list selected principals key list
 * @param errors if name key not found
 */
void Principals_DAO::select_by_key_list(Bdb_dbp &principals_db,
                                        Principals_DTO_key_list &principals_DTO_key_list,
                                        Principals_DTO_list &principals_DTO_list,
                                        Bdb_errors &errors) {
  for (Principals_DTO_key &principals_DTO_key: principals_DTO_key_list.list) {
    Principals_DTO principals_dto;
    Bdb_DAO::lookup<Principals_DTO_key,
                    Principals_DTO>(principals_db,
                                    principals_DTO_key,
                                    principals_dto,
                                    errors);
    if (!errors.has())
      principals_DTO_list.add(principals_dto);
    if (errors.has())
      break;
  }
}

/*!
 * @brief select name dto list using principals dto list
 * @param name_db name primary database
 * @param principals_dto_list select one name dto per principals dto
 * @param name_dto_list selected name dto list
 * @param errors if principals key not found
 */
void Principals_DAO::select_name_list(Bdb_dbp &name_db,
                                      Principals_DTO_list &principals_dto_list,
                                      Name_DTO_list &name_dto_list,
                                      Bdb_errors &errors) {
  for (Principals_DTO &principals_dto: principals_dto_list.list) {
    Name_DTO_key name_dto_key(principals_dto);
    Name_DTO name_dto;
    Bdb_DAO::lookup<Name_DTO_key,
                    Name_DTO>(name_db, name_dto_key, name_dto, errors);
    if (!errors.has())
      name_dto_list.add(name_dto);
    if (errors.has())
      break;
  }
}

/*!
 * @brief select title dto list using principals dto list
 * @param title_db title primary database
 * @param principals_dto_list select one title dto per principals dto
 * @param title_dto_list selected title dto list
 * @param errors if principals key not found
 */
void Principals_DAO::select_title_list(Bdb_dbp &title_db,
                                       Bdb_dbp &ratings_db,
                                       Principals_DTO_list &principals_dto_list,
                                       Title_DTO_list &title_dto_list,
                                       Bdb_errors &errors) {
  for (Principals_DTO &principals_dto: principals_dto_list.list) {
    Title_DTO_key title_dto_key(principals_dto);
    Title_DTO title_dto;
    Title_DAO::lookup(title_db, ratings_db, title_dto_key.title_id, title_dto, errors);
    if (!errors.has())
      title_dto_list.add(title_dto);
    if (errors.has())
      break;
  }
}


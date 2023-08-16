#include "bdb_dao.hpp"
#include "cards_dto.hpp"
#include "cards_dao.hpp"

/*!
 * @brief load cards database from delimited record file
 * @param cards_db cards database to which to save
 * @param text_file cards records with fields corresponding to dto
 * @param errors if save fails
 * @param delimiter record delimiter
 * @return count of records saved
 */
int Cards_DAO::load(Bdb_dbp &cards_db,
                   Bdb_dbp &cards_tripthong_bdb_db,
                   const std::string &text_file,
                   Bdb_errors &errors,
                   char delimiter) {
  return Bdb_DAO::load<Cards_DTO_key,
                       Cards_DTO>(cards_db, text_file, errors, delimiter);
}

/*!
 * @brief select cards from database based on cards id
 * @param cards_db cards database to which to save
 * @param cards_id cards record key
 * @param cards_dto cards dto to save
 * @param errors if save fails
 */
void Cards_DAO::lookup(Bdb_dbp &cards_db,
                      const std::string &cards_id,
                      Cards_DTO &cards_dto,
                      Bdb_errors &errors) {
  Cards_DTO_key cards_dto_key(cards_id);
  Bdb_DAO::lookup<Cards_DTO_key,
                  Cards_DTO>(cards_db, cards_dto_key, cards_dto, errors);
}

/*!
 * @brief save cards dto to cards database
 * @param cards_db cards database to which to save
 * @param cards_dto prinicpals dto to save
 * @param errors if save fails
 */
void Cards_DAO::save(Bdb_dbp &cards_db, Cards_DTO &cards_dto, Bdb_errors &errors) {
  Cards_DTO_key cards_dto_key(cards_dto);
  Bdb_DAO::save<Cards_DTO_key,
                Cards_DTO>(cards_db, cards_dto_key, cards_dto, errors);
}

/*!
 * @brief select all cards dto records from cards database
 * @param cards_db cards database to select from
 * @param cards_dto_list all cards dtos in database
 * @param errors if select fails
 */
void Cards_DAO::select_all(Bdb_dbp &cards_db, Cards_DTO_list &cards_dto_list, Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(cards_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_all<Cards_DTO_key,
                                Cards_DTO,
                                Cards_DTO_list>(cards_dto_list, errors);
}

/*!
 * @brief select all cards dto records from database from key
 * @param cards_db cards database to select from
 * @param cards_dto_list all cards dtos in database matching key
 * @param errors if select fails
 * @details selects all duplicate records from key if duplicates allowed
 */
void Cards_DAO::select_all_key(Bdb_dbp &cards_db,
                              Cards_DTO_key &cards_dto_key,
                              Cards_DTO_list &cards_dto_list,
                              Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(cards_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_key<Cards_DTO_key,
                                Cards_DTO,
                                Cards_DTO_list>(cards_dto_key, cards_dto_list, errors);
}

/*!
 * @brief select cards key list using cards id to search cards id->cards key secondary database
 * @param cards_cards_id_sdb cards id->cards key secondary database
 * @param cards_id secondary database search key
 * @param cards_DTO_key_list selected cards key list
 * @param errors if cards key not found
 */
void Cards_DAO::select_by_key_list(Bdb_dbp &cards_db,
                                  Cards_DTO_key_list &cards_DTO_key_list,
                                  Cards_DTO_list &cards_DTO_list,
                                  Bdb_errors &errors) {
  for (Cards_DTO_key &cards_DTO_key: cards_DTO_key_list.list) {
    Cards_DTO cards_dto;
    Bdb_DAO::lookup<Cards_DTO_key,
                    Cards_DTO>(cards_db,
                              cards_DTO_key,
                              cards_dto,
                              errors);
    if (!errors.has())
      cards_DTO_list.add(cards_dto);
    else
      break;
  }
}

void Cards_DAO::update(Bdb_dbp &cards_db,
                      const std::string &cards_id,
                      const std::string &name,
                      const std::string &type_id,
                      Cards_DTO &cards_dto,
                      Bdb_errors &errors) {
  Cards_DTO_key cards_dto_key(cards_id);
  Bdb_DAO::lookup<Cards_DTO_key,
                  Cards_DTO>(cards_db, cards_dto_key, cards_dto, errors);
  cards_dto.name = name;
  cards_dto.type_id = type_id;
  Bdb_DAO::save<Cards_DTO_key,
                Cards_DTO>(cards_db, cards_dto_key, cards_dto, errors);
}
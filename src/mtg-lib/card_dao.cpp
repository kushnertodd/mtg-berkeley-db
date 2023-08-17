#include "bdb_dao.hpp"
#include "card_dto.hpp"
#include "card_dao.hpp"

/*!
 * @brief load card database from delimited record file
 * @param card_db card database to which to save
 * @param text_file card records with fields corresponding to dto
 * @param errors if save fails
 * @param delimiter record delimiter
 * @return count of records saved
 */
int Card_DAO::load(Bdb_dbp &card_db,
                   Bdb_dbp &card_tripthong_bdb_db,
                   const std::string &text_file,
                   Bdb_errors &errors,
                   char delimiter) {
  return Bdb_DAO::load<Card_DTO_key,
                       Card_DTO>(card_db, text_file, errors, delimiter);
}

/*!
 * @brief select card from database based on card id
 * @param card_db card database to which to save
 * @param card_id card record key
 * @param card_dto card dto to save
 * @param errors if save fails
 */
void Card_DAO::lookup(Bdb_dbp &card_db,
                      const std::string &card_id,
                      Card_DTO &card_dto,
                      Bdb_errors &errors) {
  Card_DTO_key card_dto_key(card_id);
  Bdb_DAO::lookup<Card_DTO_key,
                  Card_DTO>(card_db, card_dto_key, card_dto, errors);
}

/*!
 * @brief save card dto to card database
 * @param card_db card database to which to save
 * @param card_dto prinicpals dto to save
 * @param errors if save fails
 */
void Card_DAO::save(Bdb_dbp &card_db, Card_DTO &card_dto, Bdb_errors &errors) {
  Card_DTO_key card_dto_key(card_dto);
  Bdb_DAO::save<Card_DTO_key,
                Card_DTO>(card_db, card_dto_key, card_dto, errors);
}

/*!
 * @brief select all card dto records from card database
 * @param card_db card database to select from
 * @param card_dto_list all card dtos in database
 * @param errors if select fails
 */
void Card_DAO::select_all(Bdb_dbp &card_db, Card_DTO_list &card_dto_list, Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(card_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_all<Card_DTO_key,
                                Card_DTO,
                                Card_DTO_list>(card_dto_list, errors);
}

/*!
 * @brief select all card dto records from database from key
 * @param card_db card database to select from
 * @param card_dto_list all card dtos in database matching key
 * @param errors if select fails
 * @details selects all duplicate records from key if duplicates allowed
 */
void Card_DAO::select_all_key(Bdb_dbp &card_db,
                              Card_DTO_key &card_dto_key,
                              Card_DTO_list &card_dto_list,
                              Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(card_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_key<Card_DTO_key,
                                Card_DTO,
                                Card_DTO_list>(card_dto_key, card_dto_list, errors);
}

/*!
 * @brief select card key list using card id to search card id->card key secondary database
 * @param card_card_id_sdb card id->card key secondary database
 * @param card_id secondary database search key
 * @param card_DTO_key_list selected card key list
 * @param errors if card key not found
 */
void Card_DAO::select_by_key_list(Bdb_dbp &card_db,
                                  Card_DTO_key_list &card_DTO_key_list,
                                  Card_DTO_list &card_DTO_list,
                                  Bdb_errors &errors) {
  for (Card_DTO_key &card_DTO_key: card_DTO_key_list.list) {
    Card_DTO card_dto;
    Bdb_DAO::lookup<Card_DTO_key,
                    Card_DTO>(card_db,
                              card_DTO_key,
                              card_dto,
                              errors);
    if (!errors.has())
      card_DTO_list.add(card_dto);
    else
      break;
  }
}

void Card_DAO::update(Bdb_dbp &card_db,
                      const std::string &card_id,
                      const std::string &name,
                      const std::string &type_id,
                      Card_DTO &card_dto,
                      Bdb_errors &errors) {
  Card_DTO_key card_dto_key(card_id);
  Bdb_DAO::lookup<Card_DTO_key,
                  Card_DTO>(card_db, card_dto_key, card_dto, errors);
  card_dto.name = name;
  card_dto.type_id = type_id;
  Bdb_DAO::save<Card_DTO_key,
                Card_DTO>(card_db, card_dto_key, card_dto, errors);
}
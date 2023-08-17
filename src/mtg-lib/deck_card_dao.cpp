#include "bdb_dao.hpp"
#include "deck_card_dto.hpp"
#include "deck_card_dao.hpp"

/*!
 * @brief load deck_card database from delimited record file
 * @param deck_card_db deck_card database to which to save
 * @param text_file deck_card records with fields corresponding to dto
 * @param errors if save fails
 * @param delimiter record delimiter
 * @return count of records saved
 */
int Deck_card_DAO::load(Bdb_dbp &deck_card_db,
                   Bdb_dbp &deck_card_tripthong_bdb_db,
                   const std::string &text_file,
                   Bdb_errors &errors,
                   char delimiter) {
  return Bdb_DAO::load<Deck_card_DTO_key,
                       Deck_card_DTO>(deck_card_db, text_file, errors, delimiter);
}

/*!
 * @brief select deck_card from database based on deck_card id
 * @param deck_card_db deck_card database to which to save
 * @param deck_card_id deck_card record key
 * @param deck_card_dto deck_card dto to save
 * @param errors if save fails
 */
void Deck_card_DAO::lookup(Bdb_dbp &deck_card_db,
                      const std::string &deck_card_id,
                      Deck_card_DTO &deck_card_dto,
                      Bdb_errors &errors) {
  Deck_card_DTO_key deck_card_dto_key(deck_card_id);
  Bdb_DAO::lookup<Deck_card_DTO_key,
                  Deck_card_DTO>(deck_card_db, deck_card_dto_key, deck_card_dto, errors);
}

/*!
 * @brief save deck_card dto to deck_card database
 * @param deck_card_db deck_card database to which to save
 * @param deck_card_dto prinicpals dto to save
 * @param errors if save fails
 */
void Deck_card_DAO::save(Bdb_dbp &deck_card_db, Deck_card_DTO &deck_card_dto, Bdb_errors &errors) {
  Deck_card_DTO_key deck_card_dto_key(deck_card_dto);
  Bdb_DAO::save<Deck_card_DTO_key,
                Deck_card_DTO>(deck_card_db, deck_card_dto_key, deck_card_dto, errors);
}

/*!
 * @brief select all deck_card dto records from deck_card database
 * @param deck_card_db deck_card database to select from
 * @param deck_card_dto_list all deck_card dtos in database
 * @param errors if select fails
 */
void Deck_card_DAO::select_all(Bdb_dbp &deck_card_db, Deck_card_DTO_list &deck_card_dto_list, Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(deck_card_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_all<Deck_card_DTO_key,
                                Deck_card_DTO,
                                Deck_card_DTO_list>(deck_card_dto_list, errors);
}

/*!
 * @brief select all deck_card dto records from database from key
 * @param deck_card_db deck_card database to select from
 * @param deck_card_dto_list all deck_card dtos in database matching key
 * @param errors if select fails
 * @details selects all duplicate records from key if duplicates allowed
 */
void Deck_card_DAO::select_all_key(Bdb_dbp &deck_card_db,
                              Deck_card_DTO_key &deck_card_dto_key,
                              Deck_card_DTO_list &deck_card_dto_list,
                              Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(deck_card_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_key<Deck_card_DTO_key,
                                Deck_card_DTO,
                                Deck_card_DTO_list>(deck_card_dto_key, deck_card_dto_list, errors);
}

/*!
 * @brief select deck_card key list using deck_card id to search deck_card id->deck_card key secondary database
 * @param deck_card_deck_card_id_sdb deck_card id->deck_card key secondary database
 * @param deck_card_id secondary database search key
 * @param deck_card_DTO_key_list selected deck_card key list
 * @param errors if deck_card key not found
 */
void Deck_card_DAO::select_by_key_list(Bdb_dbp &deck_card_db,
                                  Deck_card_DTO_key_list &deck_card_DTO_key_list,
                                  Deck_card_DTO_list &deck_card_DTO_list,
                                  Bdb_errors &errors) {
  for (Deck_card_DTO_key &deck_card_DTO_key: deck_card_DTO_key_list.list) {
    Deck_card_DTO deck_card_dto;
    Bdb_DAO::lookup<Deck_card_DTO_key,
                    Deck_card_DTO>(deck_card_db,
                              deck_card_DTO_key,
                              deck_card_dto,
                              errors);
    if (!errors.has())
      deck_card_DTO_list.add(deck_card_dto);
    else
      break;
  }
}

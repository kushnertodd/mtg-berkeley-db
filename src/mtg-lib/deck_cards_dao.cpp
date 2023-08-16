#include "bdb_dao.hpp"
#include "deck_cards_dto.hpp"
#include "deck_cards_dao.hpp"

/*!
 * @brief load deck_cards database from delimited record file
 * @param deck_cards_db deck_cards database to which to save
 * @param text_file deck_cards records with fields corresponding to dto
 * @param errors if save fails
 * @param delimiter record delimiter
 * @return count of records saved
 */
int Deck_cards_DAO::load(Bdb_dbp &deck_cards_db,
                   Bdb_dbp &deck_cards_tripthong_bdb_db,
                   const std::string &text_file,
                   Bdb_errors &errors,
                   char delimiter) {
  return Bdb_DAO::load<Deck_cards_DTO_key,
                       Deck_cards_DTO>(deck_cards_db, text_file, errors, delimiter);
}

/*!
 * @brief select deck_cards from database based on deck_cards id
 * @param deck_cards_db deck_cards database to which to save
 * @param deck_cards_id deck_cards record key
 * @param deck_cards_dto deck_cards dto to save
 * @param errors if save fails
 */
void Deck_cards_DAO::lookup(Bdb_dbp &deck_cards_db,
                      const std::string &deck_cards_id,
                      Deck_cards_DTO &deck_cards_dto,
                      Bdb_errors &errors) {
  Deck_cards_DTO_key deck_cards_dto_key(deck_cards_id);
  Bdb_DAO::lookup<Deck_cards_DTO_key,
                  Deck_cards_DTO>(deck_cards_db, deck_cards_dto_key, deck_cards_dto, errors);
}

/*!
 * @brief save deck_cards dto to deck_cards database
 * @param deck_cards_db deck_cards database to which to save
 * @param deck_cards_dto prinicpals dto to save
 * @param errors if save fails
 */
void Deck_cards_DAO::save(Bdb_dbp &deck_cards_db, Deck_cards_DTO &deck_cards_dto, Bdb_errors &errors) {
  Deck_cards_DTO_key deck_cards_dto_key(deck_cards_dto);
  Bdb_DAO::save<Deck_cards_DTO_key,
                Deck_cards_DTO>(deck_cards_db, deck_cards_dto_key, deck_cards_dto, errors);
}

/*!
 * @brief select all deck_cards dto records from deck_cards database
 * @param deck_cards_db deck_cards database to select from
 * @param deck_cards_dto_list all deck_cards dtos in database
 * @param errors if select fails
 */
void Deck_cards_DAO::select_all(Bdb_dbp &deck_cards_db, Deck_cards_DTO_list &deck_cards_dto_list, Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(deck_cards_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_all<Deck_cards_DTO_key,
                                Deck_cards_DTO,
                                Deck_cards_DTO_list>(deck_cards_dto_list, errors);
}

/*!
 * @brief select all deck_cards dto records from database from key
 * @param deck_cards_db deck_cards database to select from
 * @param deck_cards_dto_list all deck_cards dtos in database matching key
 * @param errors if select fails
 * @details selects all duplicate records from key if duplicates allowed
 */
void Deck_cards_DAO::select_all_key(Bdb_dbp &deck_cards_db,
                              Deck_cards_DTO_key &deck_cards_dto_key,
                              Deck_cards_DTO_list &deck_cards_dto_list,
                              Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(deck_cards_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_key<Deck_cards_DTO_key,
                                Deck_cards_DTO,
                                Deck_cards_DTO_list>(deck_cards_dto_key, deck_cards_dto_list, errors);
}

/*!
 * @brief select deck_cards key list using deck_cards id to search deck_cards id->deck_cards key secondary database
 * @param deck_cards_deck_cards_id_sdb deck_cards id->deck_cards key secondary database
 * @param deck_cards_id secondary database search key
 * @param deck_cards_DTO_key_list selected deck_cards key list
 * @param errors if deck_cards key not found
 */
void Deck_cards_DAO::select_by_key_list(Bdb_dbp &deck_cards_db,
                                  Deck_cards_DTO_key_list &deck_cards_DTO_key_list,
                                  Deck_cards_DTO_list &deck_cards_DTO_list,
                                  Bdb_errors &errors) {
  for (Deck_cards_DTO_key &deck_cards_DTO_key: deck_cards_DTO_key_list.list) {
    Deck_cards_DTO deck_cards_dto;
    Bdb_DAO::lookup<Deck_cards_DTO_key,
                    Deck_cards_DTO>(deck_cards_db,
                              deck_cards_DTO_key,
                              deck_cards_dto,
                              errors);
    if (!errors.has())
      deck_cards_DTO_list.add(deck_cards_dto);
    else
      break;
  }
}

void Deck_cards_DAO::update(Bdb_dbp &deck_cards_db,
                      const std::string &deck_cards_id,
                      const std::string &user_ratings,
                      const std::string &priority,
                      Deck_cards_DTO &deck_cards_dto,
                      Bdb_errors &errors) {
  Deck_cards_DTO_key deck_cards_dto_key(deck_cards_id);
  Bdb_DAO::lookup<Deck_cards_DTO_key,
                  Deck_cards_DTO>(deck_cards_db, deck_cards_dto_key, deck_cards_dto, errors);
  deck_cards_dto.user_rating = user_ratings;
  deck_cards_dto.priority = priority;
  Bdb_DAO::save<Deck_cards_DTO_key,
                Deck_cards_DTO>(deck_cards_db, deck_cards_dto_key, deck_cards_dto, errors);
}
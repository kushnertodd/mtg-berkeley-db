#include "bdb_dao.hpp"
#include "deck_dto.hpp"
#include "deck_dao.hpp"

/*!
 * @brief load deck database from delimited record file
 * @param deck_db deck database to which to save
 * @param text_file deck records with fields corresponding to dto
 * @param errors if save fails
 * @param delimiter record delimiter
 * @return count of records saved
 */
int Deck_DAO::load(Bdb_dbp &deck_db,
                   Bdb_dbp &deck_tripthong_bdb_db,
                   const std::string &text_file,
                   Bdb_errors &errors,
                   char delimiter) {
  return Bdb_DAO::load_tripthongs<Deck_DTO_key,
                                  Deck_DTO>(deck_db,
                                            deck_tripthong_bdb_db,
                                            text_file,
                                            errors,
                                            delimiter);

}

/*!
 * @brief select deck from database based on deck id
 * @param deck_db deck database to which to save
 * @param deck_id deck record key
 * @param deck_dto deck dto to save
 * @param errors if save fails
 */
void Deck_DAO::lookup(Bdb_dbp &deck_db,
                      const std::string &deck_id,
                      Deck_DTO &deck_dto,
                      Bdb_errors &errors) {
  Deck_DTO_key deck_dto_key(deck_id);
  Bdb_DAO::lookup<Deck_DTO_key,
                  Deck_DTO>(deck_db, deck_dto_key, deck_dto, errors);
}

/*!
 * @brief save deck dto to deck database
 * @param deck_db deck database to which to save
 * @param deck_dto prinicpals dto to save
 * @param errors if save fails
 */
void Deck_DAO::save(Bdb_dbp &deck_db, Deck_DTO &deck_dto, Bdb_errors &errors) {
  Deck_DTO_key deck_dto_key(deck_dto);
  Bdb_DAO::save<Deck_DTO_key,
                Deck_DTO>(deck_db, deck_dto_key, deck_dto, errors);
}

/*!
 * @brief select all deck dto records from deck database
 * @param deck_db deck database to select from
 * @param deck_dto_list all deck dtos in database
 * @param errors if select fails
 */
void Deck_DAO::select_all(Bdb_dbp &deck_db, Deck_DTO_list &deck_dto_list, Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(deck_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_all<Deck_DTO_key,
                                Deck_DTO,
                                Deck_DTO_list>(deck_dto_list, errors);
}

/*!
 * @brief select deck key list using deck id to search deck id->deck key secondary database
 * @param deck_deck_id_sdb deck id->deck key secondary database
 * @param deck_id secondary database search key
 * @param deck_DTO_key_list selected deck key list
 * @param errors if deck key not found
 */
void Deck_DAO::select_by_key_list(Bdb_dbp &deck_db,
                                  Deck_DTO_key_list &deck_DTO_key_list,
                                  Deck_DTO_list &deck_DTO_list,
                                  Bdb_errors &errors) {
  Bdb_DAO::select_by_key_list<Deck_DTO_key,
                              Deck_DTO_key_list,
                              Deck_DTO,
                              Deck_DTO_list>(deck_db,
                                             deck_DTO_key_list,
                                             deck_DTO_list,
                                             errors);
}

void Deck_DAO::update(Bdb_dbp &deck_db,
                      const std::string &deck_id,
                      const std::string &account_id,
                      const std::string &name,
                      Deck_DTO &deck_dto,
                      Bdb_errors &errors) {
  Deck_DTO_key deck_dto_key(deck_id);
  Bdb_DAO::lookup<Deck_DTO_key,
                  Deck_DTO>(deck_db, deck_dto_key, deck_dto, errors);
  deck_dto.account_id = account_id;
  deck_dto.name = name;
  Bdb_DAO::save<Deck_DTO_key,
                Deck_DTO>(deck_db, deck_dto_key, deck_dto, errors);
}

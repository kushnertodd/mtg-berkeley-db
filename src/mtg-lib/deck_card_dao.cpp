#include <algorithm>
#include "bdb_dao.hpp"
#include "deck_dto.hpp"
#include "deck_card_dto.hpp"
#include "card_dao.hpp"
#include "card_dto.hpp"
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
                        const std::string &text_file,
                        Bdb_errors &errors,
                        char delimiter) {
  return Bdb_DAO::load<Deck_card_DTO_key, Deck_card_DTO>
      (deck_card_db, text_file, errors, delimiter);
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
  Bdb_DAO::lookup<Deck_card_DTO_key, Deck_card_DTO>
      (deck_card_db, deck_card_dto_key, deck_card_dto, errors);
}

/*!
 * @brief save deck_card dto to deck_card database
 * @param deck_card_db deck_card database to which to save
 * @param deck_card_dto prinicpals dto to save
 * @param errors if save fails
 */
void Deck_card_DAO::save(Bdb_dbp &deck_card_db,
                         Deck_card_DTO &deck_card_dto,
                         Bdb_errors &errors) {
  Deck_card_DTO_key deck_card_dto_key(deck_card_dto);
  Bdb_DAO::save<Deck_card_DTO_key, Deck_card_DTO>
      (deck_card_db, deck_card_dto_key, deck_card_dto, errors);
}

/*!
 * @brief select deck_card key list using deck id to search deck id->deck_card key secondary database
 * @param deck_card_deck_id_sdb deck id->deck_card key secondary database
 * @param deck_id secondary database search key
 * @param deck_card_dto_key_list selected deck_card key list
 * @param errors if deck key not found
 */
void Deck_card_DAO::select_by_card_sdb(Bdb_dbp &deck_card_card_id_sdb,
                                       const std::string &card_id,
                                       Deck_card_DTO_key_list &deck_card_dto_key_list,
                                       Bdb_errors &errors) {
  Card_DTO_key card_dto_key(card_id);
  Bdb_cursor bdb_cursor(deck_card_card_id_sdb, errors);
  if (!errors.has())
    bdb_cursor.dto_get_duplicate_list<Card_DTO_key,
                                      Deck_card_DTO_key,
                                      Deck_card_DTO_key_list>
        (card_dto_key,
         deck_card_dto_key_list,
         errors);
}

/*!
 * @brief select deck_card key list using deck id to search deck id->deck_card key secondary database
 * @param deck_card_deck_id_sdb deck id->deck_card key secondary database
 * @param deck_id secondary database search key
 * @param deck_card_dto_key_list selected deck_card key list
 * @param errors if deck key not found
 */
void Deck_card_DAO::select_by_deck_sdb(Bdb_dbp &deck_card_deck_id_sdb,
                                       const std::string &deck_id,
                                       Deck_card_DTO_key_list &deck_card_dto_key_list,
                                       Bdb_errors &errors) {
  Deck_DTO_key deck_dto_key(deck_id);
  Bdb_cursor bdb_cursor(deck_card_deck_id_sdb, errors);
  if (!errors.has())
    bdb_cursor.dto_get_duplicate_list<Deck_DTO_key,
                                      Deck_card_DTO_key,
                                      Deck_card_DTO_key_list>
        (deck_dto_key,
         deck_card_dto_key_list,
         errors);
}

/*!
 * @brief select deck_card key list using deck id to search deck id->deck_card key secondary database
 * @param deck_card_deck_id_sdb deck id->deck_card key secondary database
 * @param deck_id secondary database search key
 * @param deck_card_dto_key_list selected deck_card key list
 * @param errors if deck key not found
 */
void Deck_card_DAO::select_by_key_list(Bdb_dbp &deck_card_db,
                                       Deck_card_DTO_key_list &deck_card_dto_key_list,
                                       Deck_card_DTO_list &deck_card_dto_list,
                                       Bdb_errors &errors) {
  Bdb_DAO::select_by_key_list<Deck_card_DTO_key,
                              Deck_card_DTO_key_list,
                              Deck_card_DTO,
                              Deck_card_DTO_list>
      (deck_card_db,
       deck_card_dto_key_list,
       deck_card_dto_list,
       errors);
}

/*!
 * @brief select all card records corresponding to deck id via deck id->deck_card key secondary database
 * @param deck_card_deck_id_sdb deck id->deck_card key secondary database
 * @param deck_card_db deck_card database
 * @param card_db cards database
 * @param deck_id secondary database search key
 * @param card_dto_list selected cards list
 * @param errors if deck key not found
 */
void Deck_card_DAO::select_cards_for_deck(Bdb_dbp &deck_card_deck_id_sdb,
                                          Bdb_dbp &deck_card_db,
                                          Bdb_dbp &card_db,
                                          const std::string &deck_id,
                                          Card_DTO_list &card_dto_list,
                                          Bdb_errors &errors) {
  Deck_card_DTO_key_list deck_card_dto_key_list;
  select_by_deck_sdb(deck_card_deck_id_sdb, deck_id, deck_card_dto_key_list, errors);
  Deck_card_DTO_list deck_card_dto_list;
  select_by_key_list(deck_card_db, deck_card_dto_key_list, deck_card_dto_list, errors);
  select_card_list(card_db, deck_card_dto_list, card_dto_list, errors);
}

/*!
 * @brief select card dto list using deck_card dto list
 * @param card_db card primary database
 * @param deck_card_dto_list select one card dto per deck_card dto
 * @param card_dto_list selected card dto list
 * @param errors if deck_card key not found
 */
void Deck_card_DAO::select_card_list(Bdb_dbp &card_db,
                                     Deck_card_DTO_list &deck_card_dto_list,
                                     Card_DTO_list &card_dto_list,
                                     Bdb_errors &errors) {
  for (Deck_card_DTO &deck_card_dto: deck_card_dto_list.list) {
    Card_DTO_key card_dto_key(deck_card_dto);
    Card_DTO card_dto;
    Card_DAO::lookup(card_db, card_dto_key.card_id, card_dto, errors);
    if (!errors.has())
      card_dto_list.add(card_dto);
    if (errors.has())
      break;
  }
}

/*!
 * @brief select all deck records corresponding to card id via card id->deck_card key secondary database
 * @param deck_card_card_id_sdb card id->deck_card key secondary database
 * @param deck_card_db deck_card database
 * @param deck_db decks database
 * @param card_id secondary database search key
 * @param deck_dto_list selected decks list
 * @param errors if card key not found
 */
void Deck_card_DAO::select_decks_for_card(Bdb_dbp &deck_card_card_id_sdb,
                                          Bdb_dbp &deck_card_db,
                                          Bdb_dbp &deck_db,
                                          const std::string &card_id,
                                          Deck_DTO_list &deck_dto_list,
                                          Bdb_errors &errors) {
  Deck_card_DTO_key_list deck_card_dto_key_list;
  select_by_card_sdb(deck_card_card_id_sdb, card_id, deck_card_dto_key_list, errors);
  Deck_card_DTO_list deck_card_dto_list;
  select_by_key_list(deck_card_db, deck_card_dto_key_list, deck_card_dto_list, errors);
  select_deck_list(deck_db, deck_card_dto_list, deck_dto_list, errors);
}

/*!
 * @brief select deck dto list using deck_card dto list
 * @param deck_db deck primary database
 * @param deck_card_dto_list select one deck dto per deck_card dto
 * @param deck_dto_list selected deck dto list
 * @param errors if deck_card key not found
 */
void Deck_card_DAO::select_deck_list(Bdb_dbp &deck_db,
                                     Deck_card_DTO_list &deck_card_dto_list,
                                     Deck_DTO_list &deck_dto_list,
                                     Bdb_errors &errors) {
  for (Deck_card_DTO &deck_card_dto: deck_card_dto_list.list) {
    Deck_DTO_key deck_dto_key(deck_card_dto);
    Deck_DTO deck_dto;
    Bdb_DAO::lookup<Deck_DTO_key, Deck_DTO>
        (deck_db, deck_dto_key, deck_dto, errors);
    if (!errors.has())
      deck_dto_list.add(deck_dto);
    if (errors.has())
      break;
  }
}

/*!
 * @brief select deck dto list using deck_card dto list
 * @param deck_db deck primary database
 * @param deck_card_dto_list select one deck dto per deck_card dto
 * @param deck_dto_list selected deck dto list
 * @param errors if deck_card key not found
 */
void Deck_card_DAO::select_other_cards(Bdb_dbp &deck_card_deck_id_sdb,
                                       Bdb_dbp &deck_card_db,
                                       Bdb_dbp &card_db,
                                       const std::string &deck_id,
                                       Card_DTO_list &card_dto_list,
                                       Bdb_errors &errors) {
  Card_DTO_list cards_in_deck_dto_list;
  Deck_card_DAO::select_cards_for_deck(deck_card_deck_id_sdb,
                                       deck_card_db,
                                       card_db,
                                       deck_id,
                                       cards_in_deck_dto_list,
                                       errors);
  Card_DAO::select_all(card_db, card_dto_list, errors);
  for (auto &card_dto: cards_in_deck_dto_list.list) {
    auto it = std::remove_if(card_dto_list.list.begin(),
                             card_dto_list.list.end(),
                             [&card_dto](Card_DTO &dto) {
                               return card_dto.card_id == dto.card_id;
                             });
    card_dto_list.list.erase(it, card_dto_list.list.end());
  }
}


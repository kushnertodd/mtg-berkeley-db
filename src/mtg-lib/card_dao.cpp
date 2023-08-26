#include "bdb_dao.hpp"
#include "card_dto.hpp"
#include "card_dao.hpp"
#include "deck_dto.hpp"

/*!
 * @brief load card database from delimited record file
 * @param card_db card database to which to save
 * @param text_file card records with fields corresponding to dto
 * @param errors if save fails
 * @param delimiter record delimiter
 * @return count of records saved
 */
int Card_DAO::load(Bdb_dbp &card_db,
                   Bdb_dbp &card_bdb_triplet_db,
                   const std::string &text_file,
                   Bdb_errors &errors,
                   char delimiter) {
  return Bdb_DAO::load_triplets<Card_DTO_key,
                                Card_DTO>(card_db,
                                          card_bdb_triplet_db,
                                          text_file,
                                          errors,
                                          delimiter);
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
void Card_DAO::select_all(Bdb_dbp &card_db,
                          Card_DTO_list &card_dto_list,
                          Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(card_db, errors);
  if (!errors.has())
    bdb_cursor.dto_get_list<Card_DTO_key,
                            Card_DTO,
                            Card_DTO_list>(card_dto_list, errors);
}

//void Card_DAO::select_all_decks(Bdb_dbp &deck_card_deck_id_sdb,
//                                Bdb_dbp &deck_card_db,
//                                Bdb_dbp &card_db,
//                                const std::string &deck_id,
//                                Deck_DTO_list &deck_dto_list,
//                                Bdb_errors &errors) {
//  Deck_DTO_key deck_dto_key(deck_id);
//  Deck_card_DTO_list deck_card_dto_list;
//    Bdb_DAO::select_by_secondary_db_key
//        <Deck_card_DTO_key,
//         Deck_card_DTO,
//         Deck_card_DTO_key_list,
//         Deck_card_DTO_list,
//         Deck_DTO_key>
//        (deck_card_deck_id_sdb,
//         deck_card_db,
//         deck_dto_key,
//         deck_card_dto_list,
//         errors);
//  if (!errors.has())
//    Bdb_DAO::select_by_join_dto_list<Deck_card_DTO,
//                                     Deck_card_DTO_list,
//                                     Deck_DTO_key,
//                                     Deck_DTO,
//                                     Deck_DTO_list>
//        (card_db,
//         deck_card_dto_list,
//         deck_dto_list,
//         errors);
//}
//
/*!
 * @brief select card key list using card id to search card id->card key secondary database
 * @param card_card_id_sdb card id->card key secondary database
 * @param card_id secondary database search key
 * @param card_DTO_key_list selected card key list
 * @param errors if card key not found
 */
void Card_DAO::select_all_for_type_id(Bdb_dbp &card_type_id_sdb,
                                      Bdb_dbp &card_db,
                                      const std::string &type_id,
                                      Card_DTO_list &card_dto_list,
                                      Bdb_errors &errors) {
  Card_DTO_type_id_key card_DTO_type_id_key(type_id);
  Card_DTO_key_list card_dto_key_list;
  Bdb_cursor bdb_cursor(card_type_id_sdb, errors);
  if (!errors.has())
    bdb_cursor.dto_get_duplicate_list<Card_DTO_type_id_key,
                                      Card_DTO_key,
                                      Card_DTO_key_list>
        (card_DTO_type_id_key,
         card_dto_key_list,
         errors);
  if (!errors.has())
    Bdb_DAO::select_by_key_list<Card_DTO_key,
                                Card_DTO_key_list,
                                Card_DTO,
                                Card_DTO_list>
        (card_db,
         card_dto_key_list,
         card_dto_list,
         errors);
}

/*!
 * @brief select card key list using card id to search card id->card key secondary database
 * @param card_card_id_sdb card id->card key secondary database
 * @param card_id secondary database search key
 * @param card_DTO_key_list selected card key list
 * @param errors if card key not found
 */
void Card_DAO::select_all_for_name(Bdb_dbp &card_name_sdb,
                                   Bdb_dbp &card_db,
                                   const std::string &name,
                                   Card_DTO_list &card_dto_list,
                                   Bdb_errors &errors) {
  Card_DTO_name_key card_DTO_name_key(name);
  Card_DTO_key_list card_dto_key_list;
  Bdb_cursor bdb_cursor(card_name_sdb, errors);
  if (!errors.has())
    bdb_cursor.dto_get_duplicate_list<Card_DTO_name_key,
                                      Card_DTO_key,
                                      Card_DTO_key_list>
        (card_DTO_name_key,
         card_dto_key_list,
         errors);
  if (!errors.has())
    Bdb_DAO::select_by_key_list<Card_DTO_key,
                                Card_DTO_key_list,
                                Card_DTO,
                                Card_DTO_list>
        (card_db,
         card_dto_key_list,
         card_dto_list,
         errors);
}

void Card_DAO::update(Bdb_dbp &card_db,
                      Card_DTO &card_dto,
                      Bdb_errors &errors) {
  Card_DTO_key card_dto_key(card_dto);
  Bdb_DAO::save<Card_DTO_key, Card_DTO>
      (card_db, card_dto_key, card_dto, errors);
}
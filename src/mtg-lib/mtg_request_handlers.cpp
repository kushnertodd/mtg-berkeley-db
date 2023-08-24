#include "bdb_common.hpp"
#include "bdb_json_utils.hpp"
#include "account_dao.hpp"
#include "account_dto.hpp"
#include "card_dao.hpp"
#include "card_dto.hpp"
#include "deck_card_dao.hpp"
#include "deck_dao.hpp"
#include "deck_dto.hpp"
#include "mtg_inet_app_init.hpp"
#include "mtg_request.hpp"
#include "mtg_request_handlers.hpp"

void Mtg_request_handler::handle(Mtg_inet_app_init &mtg_inet_app_init,
                                 const Mtg_request &mtg_request,
                                 Mtg_request_response &mtg_request_response,
                                 Bdb_errors &errors) {
  mtg_request_response.add_response(mtg_request.request_json);
  if (!Mtg_account_request_handler::handle(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_card_request_handler::handle(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_deck_request_handler::handle(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_deck_card_request_handler::handle(mtg_inet_app_init, mtg_request, mtg_request_response, errors))
    errors.add("Mtg_request_handler::handle", "", "invalid request: " + mtg_request.request);
}

// Mtg_account_request_handler methods

bool Mtg_account_request_handler::handle(Mtg_inet_app_init &mtg_inet_app_init,
                                         const Mtg_request &mtg_request,
                                         Mtg_request_response &mtg_request_response,
                                         Bdb_errors &errors) {
  if (!Mtg_account_request_handler::load(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_account_request_handler::lookup(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_account_request_handler::match_username(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_account_request_handler::select_all_email(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_account_request_handler::select_all(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_account_request_handler::update(mtg_inet_app_init, mtg_request, mtg_request_response, errors))
    return false;
  return true;
}

bool Mtg_account_request_handler::load(Mtg_inet_app_init &mtg_inet_app_init,
                                       const Mtg_request &mtg_request,
                                       Mtg_request_response &mtg_request_response,
                                       Bdb_errors &errors) {
  if (mtg_request.request != "account_load")
    return false;
  Mtg_request_handler::load<Account_DAO>(mtg_inet_app_init,
                                         mtg_request,
                                         "account",
                                         "account_triplets",
                                         mtg_request_response,
                                         errors);
  return true;
}

bool Mtg_account_request_handler::lookup(Mtg_inet_app_init &mtg_inet_app_init,
                                         const Mtg_request &mtg_request,
                                         Mtg_request_response &mtg_request_response,
                                         Bdb_errors &errors) {
  if (mtg_request.request != "account_lookup")
    return false;
  Mtg_request_handler::lookup<Account_DTO, Account_DAO>
      (mtg_inet_app_init,
       mtg_request,
       "account",
       mtg_request_response,
       errors);
  return true;
}

bool Mtg_account_request_handler::match_username(Mtg_inet_app_init &mtg_inet_app_init,
                                                 const Mtg_request &mtg_request,
                                                 Mtg_request_response &mtg_request_response,
                                                 Bdb_errors &errors) {
  if (mtg_request.request != "account_match_username")
    return false;
  Mtg_request_handler::match_text<Account_DTO, Account_DAO, Account_DTO_list>
      (mtg_inet_app_init,
       mtg_request,
       "account",
       "account_username_triplets",
       mtg_request_response,
       errors);
  return true;
}

bool Mtg_account_request_handler::select_all_email(Mtg_inet_app_init &mtg_inet_app_init,
                                                   const Mtg_request &mtg_request,
                                                   Mtg_request_response &mtg_request_response,
                                                   Bdb_errors &errors) {
  if (mtg_request.request != "account_select_all_for_email")
    return false;
  if (mtg_request.arguments.empty())
    errors.add("Mtg_request::select_accounts_for_email", "1", "missing email");
  std::string email;
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor = std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database_config account_primary_database_config;
    mtg_inet_app_init.bdb_databases_config.select("account", account_primary_database_config, errors);
    Secondary_database_config account_email_secondary_database_config;
    if (!errors.has())
      account_primary_database_config.select("account_email",
                                             account_email_secondary_database_config,
                                             errors);
    if (!errors.has()) {
      Primary_database account_db(account_primary_database_config,
                                  mtg_bdb_key_extractor.get(),
                                  mtg_inet_app_init.db_home,
                                  errors);
      if (!errors.has()) {
        Secondary_database account_email_sdb(account_email_secondary_database_config,
                                             mtg_inet_app_init.db_home,
                                             errors);
        Account_DTO_list account_dto_list;
        if (!errors.has()) {
          email = mtg_request.arguments.at(0);
          Account_DAO::select_accounts_for_email(account_email_sdb.bdb_db,
                                                 account_db.bdb_db,
                                                 email,
                                                 account_dto_list,
                                                 errors);
        }
        if (!errors.has()) {
          json_object *account_dto_list_json = account_dto_list.to_json(errors);
          if (!errors.has()) {
            mtg_request_response.add_response(account_dto_list_json);
          }
        }
      }
    }
  }
  return true;
}

bool Mtg_account_request_handler::select_all(Mtg_inet_app_init &mtg_inet_app_init,
                                             const Mtg_request &mtg_request,
                                             Mtg_request_response &mtg_request_response,
                                             Bdb_errors &errors) {
  if (mtg_request.request != "account_select_all")
    return false;
  Mtg_request_handler::select_all<Account_DTO_list, Account_DAO>(mtg_inet_app_init,
                                                                 "account",
                                                                 mtg_request_response,
                                                                 errors);
  return true;
}

bool Mtg_account_request_handler::update(Mtg_inet_app_init &mtg_inet_app_init,
                                         const Mtg_request &mtg_request,
                                         Mtg_request_response &mtg_request_response,
                                         Bdb_errors &errors) {
  if (mtg_request.request != "account_update")
    return false;
  if (mtg_request.arguments.size() < 4)
    errors.add("Mtg_account_request_handler::process_update_account_request", "1",
               "missing account id, username, email, or created date");
  Primary_database_config primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("account", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
        std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database
        account_db(primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home, errors);
    if (!errors.has()) {
      std::string account_id = mtg_request.arguments.at(0);
      std::string username = mtg_request.arguments.at(1);
      std::string email = mtg_request.arguments.at(2);
      std::string created = mtg_request.arguments.at(3);
      Account_DTO account_dto(account_id, username, email, created);
      Account_DAO::update(account_db.bdb_db, account_dto, errors);
      if (!errors.has()) {
        json_object *account_json = account_dto.to_json(errors);
        if (!errors.has()) {
          mtg_request_response.add_response(account_json);
        }
      }
    }
  }
  return true;
}

// Mtg_card_request_handler methods

bool Mtg_card_request_handler::handle(Mtg_inet_app_init &mtg_inet_app_init,
                                      const Mtg_request &mtg_request,
                                      Mtg_request_response &mtg_request_response,
                                      Bdb_errors &errors) {
  if (!Mtg_card_request_handler::load(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_card_request_handler::lookup(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_card_request_handler::match_name(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_card_request_handler::select_all(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_card_request_handler::select_cards_for_deck(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_card_request_handler::select_cards_for_name(mtg_inet_app_init,
                                                          mtg_request,
                                                          mtg_request_response,
                                                          errors)
      && !Mtg_card_request_handler::update(mtg_inet_app_init, mtg_request, mtg_request_response, errors))
    return false;
  return true;
}

bool Mtg_card_request_handler::load(Mtg_inet_app_init &mtg_inet_app_init,
                                    const Mtg_request &mtg_request,
                                    Mtg_request_response &mtg_request_response,
                                    Bdb_errors &errors) {
  if (mtg_request.request != "card_load")
    return false;
  Mtg_request_handler::load<Card_DAO>(mtg_inet_app_init,
                                      mtg_request,
                                      "card",
                                      "card_triplets",
                                      mtg_request_response,
                                      errors);
  return true;
}

bool Mtg_card_request_handler::lookup(Mtg_inet_app_init &mtg_inet_app_init,
                                      const Mtg_request &mtg_request,
                                      Mtg_request_response &mtg_request_response,
                                      Bdb_errors &errors) {
  if (mtg_request.request != "lookup_card")
    return false;
  Mtg_request_handler::lookup<Card_DTO, Card_DAO>(mtg_inet_app_init,
                                                  mtg_request,
                                                  "card",
                                                  mtg_request_response,
                                                  errors);
  return true;
}

bool Mtg_card_request_handler::match_name(Mtg_inet_app_init &mtg_inet_app_init,
                                          const Mtg_request &mtg_request,
                                          Mtg_request_response &mtg_request_response,
                                          Bdb_errors &errors) {
  if (mtg_request.request != "card_match_name")
    return false;
  Mtg_request_handler::match_text<Account_DTO, Account_DAO, Account_DTO_list>
      (mtg_inet_app_init,
       mtg_request,
       "card",
       "card_name_triplets",
       mtg_request_response,
       errors);
  return true;
}

bool Mtg_card_request_handler::select_all(Mtg_inet_app_init &mtg_inet_app_init,
                                          const Mtg_request &mtg_request,
                                          Mtg_request_response &mtg_request_response,
                                          Bdb_errors &errors) {
  if (mtg_request.request != "card_select_all")
    return false;
  Mtg_request_handler::select_all<Card_DTO_list, Card_DAO>(mtg_inet_app_init,
                                                           "card",
                                                           mtg_request_response,
                                                           errors);
  return true;
}

bool Mtg_card_request_handler::select_cards_for_deck(Mtg_inet_app_init &mtg_inet_app_init,
                                                     const Mtg_request &mtg_request,
                                                     Mtg_request_response &mtg_request_response,
                                                     Bdb_errors &errors) {
  if (mtg_request.request != "card_select_all_for_deck")
    return false;
  if (mtg_request.arguments.empty())
    errors.add("Mtg_card_request_handler::select_cards_for_deck", "1", "missing card_id");
  Primary_database_config deck_card_primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("deck_card", deck_card_primary_database_config, errors);
  Secondary_database_config deck_card_deck_id_secondary_database_config;
  deck_card_primary_database_config.select("deck_card_deck_id",
                                           deck_card_deck_id_secondary_database_config,
                                           errors);
  Primary_database_config card_primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("card", card_primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
        std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database card_db(card_primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home,
                             errors);
    Primary_database
        deck_card_db(deck_card_primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home,
                     errors);
    Secondary_database
        deck_card_deck_id_sdb(deck_card_deck_id_secondary_database_config, mtg_inet_app_init.db_home, errors);
    if (!errors.has()) {
      std::string deck_id = mtg_request.arguments.at(0);
      Deck_DTO_list deck_dto_list;
      Deck_card_DAO::select_by_card(deck_card_deck_id_sdb.bdb_db,
                                    deck_card_db.bdb_db,
                                    card_db.bdb_db,
                                    deck_id,
                                    deck_dto_list,
                                    errors);
      if (!errors.has()) {
        json_object *card_dto_list_json = deck_dto_list.to_json(errors);
        if (!errors.has()) {
          mtg_request_response.add_response(card_dto_list_json);
        }
      }
    }
  }
  return true;
}

bool Mtg_card_request_handler::select_cards_for_name(Mtg_inet_app_init &mtg_inet_app_init,
                                                     const Mtg_request &mtg_request,
                                                     Mtg_request_response &mtg_request_response,
                                                     Bdb_errors &errors) {
  if (mtg_request.request != "card_select_all_for_type_id")
    return false;
  if (mtg_request.arguments.empty())
    errors.add("Mtg_card_request_handler::select_cards_for_name",
               "1",
               "parameters: type_id");
  std::string type_id;
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor = std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database_config card_primary_database_config;
    mtg_inet_app_init.bdb_databases_config.select("card", card_primary_database_config, errors);
    Secondary_database_config card_type_id_secondary_database_config;
    if (!errors.has())
      card_primary_database_config.select("card_type_id",
                                          card_type_id_secondary_database_config,
                                          errors);
    if (!errors.has()) {
      Primary_database card_db(card_primary_database_config,
                               mtg_bdb_key_extractor.get(),
                               mtg_inet_app_init.db_home,
                               errors);
      if (!errors.has()) {
        Secondary_database card_type_id_sdb(card_type_id_secondary_database_config,
                                            mtg_inet_app_init.db_home,
                                            errors);
        Card_DTO_list card_dto_list;
        if (!errors.has()) {
          type_id = mtg_request.arguments.at(0);
          Card_DAO::select_cards_for_type_id(card_type_id_sdb.bdb_db,
                                             card_db.bdb_db,
                                             type_id,
                                             card_dto_list,
                                             errors);
        }
        if (!errors.has()) {
          json_object *card_dto_list_json = card_dto_list.to_json(errors);
          if (!errors.has()) {
            mtg_request_response.add_response(card_dto_list_json);
          }
        }
      }
    }
  }
  return true;
}

bool Mtg_card_request_handler::update(Mtg_inet_app_init &mtg_inet_app_init,
                                      const Mtg_request &mtg_request,
                                      Mtg_request_response &mtg_request_response,
                                      Bdb_errors &errors) {
  if (mtg_request.request != "card_update")
    return false;
  if (mtg_request.arguments.size() < 4)
    errors.add("Mtg_card_request_handler::process_update_card_request", "1", "missing card_id");
  Primary_database_config primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("card", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
        std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database card_db(primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home, errors);
    if (!errors.has()) {
      std::string card_id = mtg_request.arguments.at(0);
      std::string name = mtg_request.arguments.at(1);
      std::string email = mtg_request.arguments.at(2);
      std::string type_id = mtg_request.arguments.at(3);
      Card_DTO card_dto(card_id, name, type_id);
      Card_DAO::update(card_db.bdb_db, card_dto, errors);
      if (!errors.has()) {
        json_object *card_json = card_dto.to_json(errors);
        if (!errors.has()) {
          mtg_request_response.add_response(card_json);
        }
      }
    }
  }
  return true;
}

// Mtg_deck_request_handler methods

bool Mtg_deck_request_handler::handle(Mtg_inet_app_init &mtg_inet_app_init,
                                      const Mtg_request &mtg_request,
                                      Mtg_request_response &mtg_request_response,
                                      Bdb_errors &errors) {
  if (!Mtg_deck_request_handler::load(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_deck_request_handler::lookup(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_deck_request_handler::match_name(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_deck_request_handler::select_all(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_deck_request_handler::select_decks_for_account_id(mtg_inet_app_init,
                                                                mtg_request,
                                                                mtg_request_response,
                                                                errors)
      && !Mtg_deck_request_handler::select_all_cards(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_deck_request_handler::select_other_cards(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_deck_request_handler::update(mtg_inet_app_init, mtg_request, mtg_request_response, errors))
    return false;
  return true;
}

bool Mtg_deck_request_handler::load(Mtg_inet_app_init &mtg_inet_app_init,
                                    const Mtg_request &mtg_request,
                                    Mtg_request_response &mtg_request_response,
                                    Bdb_errors &errors) {
  if (mtg_request.request != "deck_load")
    return false;
  Mtg_request_handler::load<Deck_DAO>(mtg_inet_app_init,
                                      mtg_request,
                                      "deck",
                                      "deck_triplets",
                                      mtg_request_response,
                                      errors);
  return true;
}

bool Mtg_deck_request_handler::lookup(Mtg_inet_app_init &mtg_inet_app_init,
                                      const Mtg_request &mtg_request,
                                      Mtg_request_response &mtg_request_response,
                                      Bdb_errors &errors) {
  if (mtg_request.request == "lookup_deck")
    return false;
  Mtg_request_handler::lookup<Deck_DTO, Deck_DAO>(mtg_inet_app_init,
                                                  mtg_request,
                                                  "deck",
                                                  mtg_request_response,
                                                  errors);
  return true;
}

bool Mtg_deck_request_handler::match_name(Mtg_inet_app_init &mtg_inet_app_init,
                                          const Mtg_request &mtg_request,
                                          Mtg_request_response &mtg_request_response,
                                          Bdb_errors &errors) {
  if (mtg_request.request != "deck_match_name")
    return false;
  Mtg_request_handler::match_text<Account_DTO, Account_DAO, Account_DTO_list>
      (mtg_inet_app_init,
       mtg_request,
       "deck",
       "deck_name_triplets",
       mtg_request_response,
       errors);
  return true;
}

bool Mtg_deck_request_handler::select_decks_for_account_id(Mtg_inet_app_init &mtg_inet_app_init,
                                                           const Mtg_request &mtg_request,
                                                           Mtg_request_response &mtg_request_response,
                                                           Bdb_errors &errors) {
  if (mtg_request.request != "select_decks_for_account_id")
    return false;
  if (mtg_request.arguments.empty())
    errors.add("Mtg_deck_request_handler::select_decks_for_account_id",
               "1",
               "parameters: account_id");
  std::string account_id;
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor = std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database_config deck_primary_database_config;
    mtg_inet_app_init.bdb_databases_config.select("deck", deck_primary_database_config, errors);
    Secondary_database_config deck_account_id_secondary_database_config;
    if (!errors.has())
      deck_primary_database_config.select("deck_account_id",
                                          deck_account_id_secondary_database_config,
                                          errors);
    if (!errors.has()) {
      Primary_database deck_db(deck_primary_database_config,
                               mtg_bdb_key_extractor.get(),
                               mtg_inet_app_init.db_home,
                               errors);
      if (!errors.has()) {
        Secondary_database deck_account_id_sdb(deck_account_id_secondary_database_config,
                                               mtg_inet_app_init.db_home,
                                               errors);
        Deck_DTO_list deck_dto_list;
        if (!errors.has()) {
          account_id = mtg_request.arguments.at(0);
          Deck_DAO::select_decks_for_account_id(deck_account_id_sdb.bdb_db,
                                                deck_db.bdb_db,
                                                account_id,
                                                deck_dto_list,
                                                errors);
        }
        if (!errors.has()) {
          json_object *deck_dto_list_json = deck_dto_list.to_json(errors);
          if (!errors.has()) {
            mtg_request_response.add_response(deck_dto_list_json);
          }
        }
      }
    }
  }
  return true;
}

bool Mtg_deck_request_handler::select_all(Mtg_inet_app_init &mtg_inet_app_init,
                                          const Mtg_request &mtg_request,
                                          Mtg_request_response &mtg_request_response,
                                          Bdb_errors &errors) {
  if (mtg_request.request != "deck_select_all")
    return false;
  Mtg_request_handler::select_all<Deck_DTO_list, Deck_DAO>(mtg_inet_app_init,
                                                           "deck",
                                                           mtg_request_response,
                                                           errors);
  return true;

}

bool Mtg_deck_request_handler::select_all_cards(Mtg_inet_app_init &mtg_inet_app_init,
                                                const Mtg_request &mtg_request,
                                                Mtg_request_response &mtg_request_response,
                                                Bdb_errors &errors) {
  if (mtg_request.request != "select_all_cards")
    return false;
  if (mtg_request.arguments.empty())
    errors.add("Mtg_deck_request_handler::select_all_cards", "1", "missing deck_id");
  Primary_database_config deck_card_primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("deck_card", deck_card_primary_database_config, errors);
  Secondary_database_config deck_card_deck_id_secondary_database_config;
  deck_card_primary_database_config.select("deck_card_deck_id",
                                           deck_card_deck_id_secondary_database_config,
                                           errors);
  Primary_database_config card_primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("card", card_primary_database_config, errors);
  std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
      std::make_unique<Mtg_bdb_key_extractor>();
  Primary_database
      card_db(card_primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home, errors);
  Primary_database
      deck_card_db(deck_card_primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home,
                   errors);
  Secondary_database
      deck_card_deck_id_sdb(deck_card_deck_id_secondary_database_config, mtg_inet_app_init.db_home, errors);
  if (!errors.has()) {
    std::string deck_id = mtg_request.arguments.at(0);
    Card_DTO_list card_dto_list;
    Deck_card_DAO::select_by_deck(deck_card_deck_id_sdb.bdb_db,
                                  deck_card_db.bdb_db,
                                  card_db.bdb_db,
                                  deck_id,
                                  card_dto_list,
                                  errors);
    // TODO: convert deck_card list to deck list
    if (!errors.has()) {
      json_object *card_dto_list_json = card_dto_list.to_json(errors);
      if (!errors.has()) {
        mtg_request_response.add_response(card_dto_list_json);
      }
    }
  }
  return true;
}

// TODO
bool Mtg_deck_request_handler::select_other_cards(Mtg_inet_app_init &mtg_inet_app_init,
                                                  const Mtg_request &mtg_request,
                                                  Mtg_request_response &mtg_request_response,
                                                  Bdb_errors &errors) {
  if (mtg_request.request != "deck_select_other_cards")
    return false;
  return true;
}

bool Mtg_deck_request_handler::update(Mtg_inet_app_init &mtg_inet_app_init,
                                      const Mtg_request &mtg_request,
                                      Mtg_request_response &mtg_request_response,
                                      Bdb_errors &errors) {
  if (mtg_request.request != "deck_update")
    return false;
  if (mtg_request.arguments.size() < 3)
    errors.add("Mtg_deck_request_handler::process_update_deck_request", "1", "missing deck_id");
  Primary_database_config primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("deck", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
        std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database deck_db(primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home, errors);
    if (!errors.has()) {
      std::string deck_id = mtg_request.arguments.at(0);
      std::string name = mtg_request.arguments.at(1);
      std::string account_id = mtg_request.arguments.at(2);
      Deck_DTO deck_dto(deck_id, account_id, name);
      Deck_DAO::update(deck_db.bdb_db, deck_dto, errors);
      if (!errors.has()) {
        json_object *deck_json = deck_dto.to_json(errors);
        if (!errors.has()) {
          mtg_request_response.add_response(deck_json);
        }
      }
    }
  }
  return true;
}

// Mtg_deck_card_request_handler methods

bool Mtg_deck_card_request_handler::handle(Mtg_inet_app_init &mtg_inet_app_init,
                                           const Mtg_request &mtg_request,
                                           Mtg_request_response &mtg_request_response,
                                           Bdb_errors &errors) {
  return Mtg_deck_card_request_handler::load(mtg_inet_app_init, mtg_request, mtg_request_response, errors);
}

bool Mtg_deck_card_request_handler::load(Mtg_inet_app_init &mtg_inet_app_init,
                                         const Mtg_request &mtg_request,
                                         Mtg_request_response &mtg_request_response,
                                         Bdb_errors &errors) {
  if (mtg_request.request != "deck_card_load")
    return false;
  if (mtg_request.arguments.empty())
    errors.add("Mtg_deck_card_request_handler::load", "1", "missing mtg load deck_card request arguments");
  Primary_database_config deck_card_primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("deck_card", deck_card_primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
        std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database
        deck_card_db(deck_card_primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home, errors);
    if (!errors.has()) {
      std::string text_file = mtg_inet_app_init.tsv_home + "/" + mtg_request.arguments.at(0);
      int count = Deck_card_DAO::load(deck_card_db.bdb_db, text_file, errors, tab);
      if (!errors.has())
        mtg_request_response.add_response(Mtg_request_response::to_load_response(count, errors));
    }
  }
  return true;
}

// Mtg_workflow_request_handler methods

bool Mtg_workflow_request_handler::handle(Mtg_inet_app_init &mtg_inet_app_init,
                                          const Mtg_request &mtg_request,
                                          Mtg_request_response &mtg_request_response,
                                          Bdb_errors &errors) {
  if (!Mtg_workflow_request_handler::create_account(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_workflow_request_handler::remove_account(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_workflow_request_handler::create_card(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_workflow_request_handler::remove_card(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_workflow_request_handler::select_cards_for_type_id(mtg_inet_app_init,
                                                                 mtg_request,
                                                                 mtg_request_response,
                                                                 errors)
      && !Mtg_workflow_request_handler::add_card_to_deck(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_workflow_request_handler::create_deck(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_workflow_request_handler::remove_deck(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_workflow_request_handler::remove_card_from_deck(mtg_inet_app_init,
                                                              mtg_request,
                                                              mtg_request_response,
                                                              errors)
      && !Mtg_workflow_request_handler::select_decks_for_name(mtg_inet_app_init,
                                                              mtg_request,
                                                              mtg_request_response,
                                                              errors))
    return false;
  return true;
}

bool Mtg_workflow_request_handler::create_account(Mtg_inet_app_init &Mtg_inet_app_init,
                                                  const Mtg_request &mtg_request,
                                                  Mtg_request_response &mtg_request_response,
                                                  Bdb_errors &errors) {

  return false;
}

bool Mtg_workflow_request_handler::remove_account(Mtg_inet_app_init &Mtg_inet_app_init,
                                                  const Mtg_request &mtg_request,
                                                  Mtg_request_response &mtg_request_response,
                                                  Bdb_errors &errors) {

  return false;
}

bool Mtg_workflow_request_handler::create_card(Mtg_inet_app_init &mtg_inet_app_init,
                                               const Mtg_request &mtg_request,
                                               Mtg_request_response &mtg_request_response,
                                               Bdb_errors &errors) {
  return false;
}

bool Mtg_workflow_request_handler::remove_card(Mtg_inet_app_init &mtg_inet_app_init,
                                               const Mtg_request &mtg_request,
                                               Mtg_request_response &mtg_request_response,
                                               Bdb_errors &errors) {
  return true;
}

bool Mtg_workflow_request_handler::select_cards_for_type_id(Mtg_inet_app_init &mtg_inet_app_init,
                                                            const Mtg_request &mtg_request,
                                                            Mtg_request_response &mtg_request_response,
                                                            Bdb_errors &errors) {
  return false;
}

bool Mtg_workflow_request_handler::add_card_to_deck(Mtg_inet_app_init &mtg_inet_app_init,
                                                    const Mtg_request &mtg_request,
                                                    Mtg_request_response &mtg_request_response,
                                                    Bdb_errors &errors) {
  return false;
}

bool Mtg_workflow_request_handler::create_deck(Mtg_inet_app_init &mtg_inet_app_init,
                                               const Mtg_request &mtg_request,
                                               Mtg_request_response &mtg_request_response,
                                               Bdb_errors &errors) {
  return false;
}

bool Mtg_workflow_request_handler::remove_deck(Mtg_inet_app_init &mtg_inet_app_init,
                                               const Mtg_request &mtg_request,
                                               Mtg_request_response &mtg_request_response,
                                               Bdb_errors &errors) {
  return false;
}

bool Mtg_workflow_request_handler::remove_card_from_deck(Mtg_inet_app_init &mtg_inet_app_init,
                                                         const Mtg_request &mtg_request,
                                                         Mtg_request_response &mtg_request_response,
                                                         Bdb_errors &errors) {
  return false;
}

bool Mtg_workflow_request_handler::select_decks_for_name(Mtg_inet_app_init &mtg_inet_app_init,
                                                         const Mtg_request &mtg_request,
                                                         Mtg_request_response &mtg_request_response,
                                                         Bdb_errors &errors) {
  return false;
}

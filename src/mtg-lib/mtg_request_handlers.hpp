#pragma once
#include <list>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases_config.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "mtg_inet_app_init.hpp"
#include "timer.hpp"

class Mtg_request_handler {
 public:
  static void handle(Mtg_inet_app_init &mtg_inet_app_init,
                     const Mtg_request &mtg_request,
                     Mtg_request_response &mtg_request_response,
                     Bdb_errors &errors);
};

class Mtg_account_request_handler {
 public:
  static bool handle(Mtg_inet_app_init &mtg_inet_app_init,
                     const Mtg_request &mtg_request,
                     Mtg_request_response &mtg_request_response,
                     Bdb_errors &errors);

  static bool load(Mtg_inet_app_init &Mtg_inet_app_init,
                   const Mtg_request &mtg_request,
                   Mtg_request_response &mtg_request_response,
                   Bdb_errors &errors);

  static bool lookup(Mtg_inet_app_init &mtg_inet_app_init,
                     const Mtg_request &mtg_request,
                     Mtg_request_response &mtg_request_response,
                     Bdb_errors &errors);

  static bool match_username(Mtg_inet_app_init &mtg_inet_app_init,
                             const Mtg_request &mtg_request,
                             Mtg_request_response &mtg_request_response,
                             Bdb_errors &errors);

  static bool select_all_email(Mtg_inet_app_init &mtg_inet_app_init,
                               const Mtg_request &mtg_request,
                               Mtg_request_response &mtg_request_response,
                               Bdb_errors &errors);

  static bool select_all(Mtg_inet_app_init &mtg_inet_app_init,
                         const Mtg_request &mtg_request,
                         Mtg_request_response &mtg_request_response,
                         Bdb_errors &errors);

  static bool update(Mtg_inet_app_init &mtg_inet_app_init,
                     const Mtg_request &mtg_request,
                     Mtg_request_response &mtg_request_response,
                     Bdb_errors &errors);
};

class Mtg_card_request_handler {
 public:
  static bool handle(Mtg_inet_app_init &mtg_inet_app_init,
                     const Mtg_request &mtg_request,
                     Mtg_request_response &mtg_request_response,
                     Bdb_errors &errors);

  static bool load(Mtg_inet_app_init &Mtg_inet_app_init,
                   const Mtg_request &mtg_request,
                   Mtg_request_response &mtg_request_response,
                   Bdb_errors &errors);

  static bool lookup(Mtg_inet_app_init &mtg_inet_app_init,
                     const Mtg_request &mtg_request,
                     Mtg_request_response &mtg_request_response,
                     Bdb_errors &errors);

  static bool match_name(Mtg_inet_app_init &mtg_inet_app_init,
                         const Mtg_request &mtg_request,
                         Mtg_request_response &mtg_request_response,
                         Bdb_errors &errors);

  static bool select_all(Mtg_inet_app_init &mtg_inet_app_init,
                         const Mtg_request &mtg_request,
                         Mtg_request_response &mtg_request_response,
                         Bdb_errors &errors);

  static bool select_cards_for_deck(Mtg_inet_app_init &mtg_inet_app_init,
                                    const Mtg_request &mtg_request,
                                    Mtg_request_response &mtg_request_response,
                                    Bdb_errors &errors);

  static bool select_cards_for_name(Mtg_inet_app_init &mtg_inet_app_init,
                                    const Mtg_request &mtg_request,
                                    Mtg_request_response &mtg_request_response,
                                    Bdb_errors &errors);

  static bool update(Mtg_inet_app_init &mtg_inet_app_init,
                     const Mtg_request &mtg_request,
                     Mtg_request_response &mtg_request_response,
                     Bdb_errors &errors);
};

class Mtg_deck_request_handler {
 public:
  static bool handle(Mtg_inet_app_init &mtg_inet_app_init,
                     const Mtg_request &mtg_request,
                     Mtg_request_response &mtg_request_response,
                     Bdb_errors &errors);
  static bool load(Mtg_inet_app_init &Mtg_inet_app_init,
                   const Mtg_request &mtg_request,
                   Mtg_request_response &mtg_request_response,
                   Bdb_errors &errors);
  static bool lookup(Mtg_inet_app_init &mtg_inet_app_init,
                     const Mtg_request &mtg_request,
                     Mtg_request_response &mtg_request_response,
                     Bdb_errors &errors);
  static bool match_name(Mtg_inet_app_init &mtg_inet_app_init,
                         const Mtg_request &mtg_request,
                         Mtg_request_response &mtg_request_response,
                         Bdb_errors &errors);
  static bool select_all(Mtg_inet_app_init &mtg_inet_app_init,
                         const Mtg_request &mtg_request,
                         Mtg_request_response &mtg_request_response,
                         Bdb_errors &errors);
  static bool select_decks_for_account_id(Mtg_inet_app_init &mtg_inet_app_init,
                                          const Mtg_request &mtg_request,
                                          Mtg_request_response &mtg_request_response,
                                          Bdb_errors &errors);
  static bool select_all_cards(Mtg_inet_app_init &mtg_inet_app_init,
                               const Mtg_request &mtg_request,
                               Mtg_request_response &mtg_request_response,
                               Bdb_errors &errors);
  static bool select_other_cards(Mtg_inet_app_init &mtg_inet_app_init,
                                 const Mtg_request &mtg_request,
                                 Mtg_request_response &mtg_request_response,
                                 Bdb_errors &errors);
  static bool update(Mtg_inet_app_init &mtg_inet_app_init,
                     const Mtg_request &mtg_request,
                     Mtg_request_response &mtg_request_response,
                     Bdb_errors &errors);
};

class Mtg_deck_card_request_handler {
 public:
  static bool handle(Mtg_inet_app_init &mtg_inet_app_init,
                     const Mtg_request &mtg_request,
                     Mtg_request_response &mtg_request_response,
                     Bdb_errors &errors);
  static bool load(Mtg_inet_app_init &Mtg_inet_app_init,
                   const Mtg_request &mtg_request,
                   Mtg_request_response &mtg_request_response,
                   Bdb_errors &errors);
};

static bool create_acccount(Mtg_inet_app_init &Mtg_inet_app_init,
                            const Mtg_request &mtg_request,
                            Mtg_request_response &mtg_request_response,
                            Bdb_errors &errors);

static bool remove_acccount(Mtg_inet_app_init &Mtg_inet_app_init,
                            const Mtg_request &mtg_request,
                            Mtg_request_response &mtg_request_response,
                            Bdb_errors &errors);

class Mtg_workflow_request_handler {
 public:
  static bool create_card(Mtg_inet_app_init &Mtg_inet_app_init,
                          const Mtg_request &mtg_request,
                          Mtg_request_response &mtg_request_response,
                          Bdb_errors &errors);

  static bool remove_card(Mtg_inet_app_init &Mtg_inet_app_init,
                          const Mtg_request &mtg_request,
                          Mtg_request_response &mtg_request_response,
                          Bdb_errors &errors);

  static bool select_cards_for_type_id(Mtg_inet_app_init &mtg_inet_app_init,
                                       const Mtg_request &mtg_request,
                                       Mtg_request_response &mtg_request_response,
                                       Bdb_errors &errors);

  static bool add_card_to_deck(Mtg_inet_app_init &Mtg_inet_app_init,
                               const Mtg_request &mtg_request,
                               Mtg_request_response &mtg_request_response,
                               Bdb_errors &errors);

  static bool create_deck(Mtg_inet_app_init &Mtg_inet_app_init,
                          const Mtg_request &mtg_request,
                          Mtg_request_response &mtg_request_response,
                          Bdb_errors &errors);

  static bool remove_deck(Mtg_inet_app_init &Mtg_inet_app_init,
                          const Mtg_request &mtg_request,
                          Mtg_request_response &mtg_request_response,
                          Bdb_errors &errors);

  static bool remove_card_from_deck(Mtg_inet_app_init &Mtg_inet_app_init,
                                    const Mtg_request &mtg_request,
                                    Mtg_request_response &mtg_request_response,
                                    Bdb_errors &errors);

  static bool select_decks_for_name(Mtg_inet_app_init &mtg_inet_app_init,
                                    const Mtg_request &mtg_request,
                                    Mtg_request_response &mtg_request_response,
                                    Bdb_errors &errors);
};

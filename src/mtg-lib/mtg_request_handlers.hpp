#pragma once
#include <list>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_dao.hpp"
#include "bdb_databases.hpp"
#include "bdb_databases_config.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "misc_utils.hpp"
#include "mtg_bdb_keys.hpp"
#include "mtg_inet_app_init.hpp"

class Mtg_request_handler {
 public:
  static void handle(Mtg_inet_app_init &mtg_inet_app_init,
                     const Mtg_request &mtg_request,
                     Mtg_request_response &mtg_request_response,
                     Bdb_errors &errors);

  template<typename DA>
  static void load(Mtg_inet_app_init &mtg_inet_app_init,
                   const Mtg_request &mtg_request,
                   const std::string &dto_db_name,
                   const std::string &dto_triplet_db_name,
                   Mtg_request_response &mtg_request_response,
                   Bdb_errors &errors) {
    if (mtg_request.arguments.empty())
      errors.add("Mtg_request_handler::load", "1", "missing text file");
    Primary_database_config dto_primary_database_config;
    mtg_inet_app_init.bdb_databases_config.select(dto_db_name,
                                                  dto_primary_database_config,
                                                  errors);
    if (!errors.has()) {
      std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
          std::make_unique<Mtg_bdb_key_extractor>();
      Primary_database
          dto_db(dto_primary_database_config, mtg_bdb_key_extractor.get(),
                 mtg_inet_app_init.db_home, errors);
      if (!errors.has()) {
        Primary_database_config dto_triplets_primary_database_config;
        mtg_inet_app_init.bdb_databases_config.select(dto_triplet_db_name,
                                                      dto_triplets_primary_database_config,
                                                      errors);
        if (!errors.has()) {
          Primary_database dto_triplets_db(dto_triplets_primary_database_config,
                                           mtg_bdb_key_extractor.get(),
                                           mtg_inet_app_init.db_home,
                                           errors);
          if (!errors.has()) {
            std::string text_file = mtg_inet_app_init.tsv_home + "/"
                + mtg_request.arguments.at(0);
            int count = DA::load(dto_db.bdb_db, dto_triplets_db.bdb_db,
                                 text_file, errors, tab);
            if (!errors.has())
              mtg_request_response.add_response(
                  Mtg_request_response::to_load_response(count, errors));
          }
        }
      }
    }
  }

  template<typename D, typename DA>
  static void lookup(Mtg_inet_app_init &mtg_inet_app_init,
                     const Mtg_request &mtg_request,
                     const std::string &dto_db_name,
                     Mtg_request_response &mtg_request_response,
                     Bdb_errors &errors) {
    if (mtg_request.arguments.empty())
      errors.add("Mtg_request_handler::lookup", "1", "missing id");
    Primary_database_config primary_database_config;
    mtg_inet_app_init.bdb_databases_config.select(dto_db_name,
                                                  primary_database_config,
                                                  errors);
    if (!errors.has()) {
      std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
          std::make_unique<Mtg_bdb_key_extractor>();
      Primary_database
          dto_db(primary_database_config, mtg_bdb_key_extractor.get(),
                 mtg_inet_app_init.db_home, errors);
      if (!errors.has()) {
        std::string dto_id = mtg_request.arguments.at(0);
        D dto;
        DA::lookup(dto_db.bdb_db, dto_id, dto, errors);
        if (!errors.has()) {
          json_object *dto_json = dto.to_json(errors);
          if (!errors.has()) {
            mtg_request_response.add_response(dto_json);
          }
        }
      }
    }
  }

  template<typename D, typename DA, typename DL>
  static void match_text(Mtg_inet_app_init &mtg_inet_app_init,
                         const Mtg_request &mtg_request,
                         const std::string &dto_db_name,
                         const std::string &dto_triplet_db_name,
                         Mtg_request_response &mtg_request_response,
                         Bdb_errors &errors) {
    if (mtg_request.arguments.size() < 3)
      errors.add("Mtg_request_handler::match_text",
                 "1", "parameters: text, min_score, score_count");
    std::string text;
    int min_score;
    int score_count;
    if (!errors.has()) {
      text = mtg_request.arguments.at(0);
      min_score = Misc_utils::string_to_int(mtg_request.arguments.at(1), errors);
      score_count = Misc_utils::string_to_int(mtg_request.arguments.at(2), errors);
    }
    if (!errors.has()) {
      std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
          std::make_unique<Mtg_bdb_key_extractor>();
      Primary_database_config dto_primary_database_config;
      mtg_inet_app_init.bdb_databases_config.select(dto_db_name,
                                                    dto_primary_database_config,
                                                    errors);
      if (!errors.has()) {
        Primary_database dto_db(dto_primary_database_config,
                                mtg_bdb_key_extractor.get(),
                                mtg_inet_app_init.db_home,
                                errors);
        Primary_database_config dto_triplets_primary_database_config;
        mtg_inet_app_init.bdb_databases_config.select(dto_triplet_db_name,
                                                      dto_triplets_primary_database_config,
                                                      errors);
        if (!errors.has()) {
          Primary_database dto_triplets_db(dto_triplets_primary_database_config,
                                           mtg_bdb_key_extractor.get(),
                                           mtg_inet_app_init.db_home,
                                           errors);
          if (!errors.has()) {
            Bdb_text_triplets bdb_text_triplets(text);
            std::map<std::string, int> triplet_counts;
            std::map<std::string, int> triplet_match_counts;
            std::map<std::string, int> id_scores;
            for (const auto &text_triplet_occurrence: bdb_text_triplets.list) {
              Bdb_text_id_occurrence_list bdb_text_id_occurrence_list;
              Bdb_DAO::select_triplets_by_key_list(dto_triplets_db.bdb_db,
                                                   text_triplet_occurrence.triplet,
                                                   bdb_text_id_occurrence_list,
                                                   errors, true);
              if (!errors.has()) {
                for (const auto &text_id_occurence: bdb_text_id_occurrence_list.list) {
                  std::string id = text_id_occurence.id;
                  triplet_counts[id] = text_id_occurence.triplets_count;
                  triplet_match_counts[id] += std::min(text_id_occurence.occurrence_count,
                                                       text_triplet_occurrence.occurrence_count);
                }
              } else
                break;
            }
            // sort scores
            if (!errors.has()) {
              for (const auto &triplet_count: triplet_counts) {
                std::string id = triplet_count.first;
                // match in integer percent
                id_scores[id] = (100 * triplet_match_counts[id] * triplet_match_counts[id]) /
                    (bdb_text_triplets.triplets_count * triplet_counts[id]);
              }
              std::map<int, std::string, std::greater<> > id_score_sort;
              for (const auto &id_score_entry: id_scores)
                id_score_sort[id_score_entry.second] = id_score_entry.first;
              DL dto_list;
              int dto_count{};
              for (const auto &id_score_sort_entry: id_score_sort) {
                int score = id_score_sort_entry.first;
                if (score > min_score && dto_count < score_count) {
                  std::string dto_id = id_score_sort_entry.second;
                  D dto;
                  DA::lookup(dto_db.bdb_db, dto_id, dto, errors);
                  dto.score = std::to_string(score);
                  if (!errors.has()) {
                    dto_list.list.push_back(dto);
                  } else
                    break;
                }
                ++dto_count;
              }
              json_object *dto_list_json = dto_list.to_json(errors);
              if (!errors.has()) {
                mtg_request_response.add_response(dto_list_json);
              }
            }
          }
        }
      }
    }
  }

  template<typename DL, typename DA>
  static void select_all(Mtg_inet_app_init &mtg_inet_app_init,
                         const std::string &dto_db_name,
                         Mtg_request_response &mtg_request_response,
                         Bdb_errors &errors) {
    Primary_database_config primary_database_config;
    mtg_inet_app_init.bdb_databases_config.select(dto_db_name,
                                                  primary_database_config,
                                                  errors);
    if (!errors.has()) {
      std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
          std::make_unique<Mtg_bdb_key_extractor>();
      Primary_database
          deck_db(primary_database_config,
                  mtg_bdb_key_extractor.get(),
                  mtg_inet_app_init.db_home,
                  errors);
      if (!errors.has()) {
        DL deck_dto_list;
        DA::select_all(deck_db.bdb_db, deck_dto_list, errors);
        if (!errors.has()) {
          json_object *deck_list_json = deck_dto_list.to_json(errors);
          if (!errors.has()) {
            mtg_request_response.add_response(deck_list_json);
          }
        }
      }
    }
  }

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

  static bool select_all_for_email(Mtg_inet_app_init &mtg_inet_app_init,
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

  static bool select_cards_for_type_id(Mtg_inet_app_init &mtg_inet_app_init,
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
  static bool lookup(Mtg_inet_app_init &mtg_inet_app_init,
                     const Mtg_request &mtg_request,
                     Mtg_request_response &mtg_request_response,
                     Bdb_errors &errors);

};

class Mtg_workflow_request_handler {
 public:
  static bool handle(Mtg_inet_app_init &mtg_inet_app_init,
                     const Mtg_request &mtg_request,
                     Mtg_request_response &mtg_request_response,
                     Bdb_errors &errors);

  static bool create_account(Mtg_inet_app_init &Mtg_inet_app_init,
                             const Mtg_request &mtg_request,
                             Mtg_request_response &mtg_request_response,
                             Bdb_errors &errors);

  static bool remove_account(Mtg_inet_app_init &Mtg_inet_app_init,
                             const Mtg_request &mtg_request,
                             Mtg_request_response &mtg_request_response,
                             Bdb_errors &errors);

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

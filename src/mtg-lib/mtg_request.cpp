#include <json-c/json.h>
#include <sstream>
#include "bdb_databases.hpp"
#include "bdb_databases_config.hpp"
#include "bdb_common.hpp"
#include "bdb_dao.hpp"
#include "bdb_json_utils.hpp"
#include "account_dao.hpp"
#include "account_dto.hpp"
#include "card_dao.hpp"
#include "card_dto.hpp"
#include "deck_card_dao.hpp"
#include "deck_card_dto.hpp"
#include "deck_dao.hpp"
#include "deck_dto.hpp"
#include "mtg_bdb_keys.hpp"
#include "mtg_dto.hpp"
#include "mtg_global.hpp"
#include "mtg_inet_app_init.hpp"
#include "mtg_request.hpp"
#include "mtg_tokens.hpp"
#include "misc_utils.hpp"
#include "timer.hpp"
#include "mtg_request.hpp"

Mtg_request::Mtg_request(const std::string &request, Bdb_errors &errors) {
  request_json = json_tokener_parse(request.c_str());
  if (request_json == nullptr)
    errors.add("Mtg_request_handler::Mtg_request",
               "1",
               "invalid request json: " + request, errno);
  else {
    std::string request_class_name =
        Bdb_json_utils::get_json_string("Mtg_request_handler::Mtg_request", "2",
                                        request_json, "class_name",
                                        errors);
    if (request_class_name != Mtg_request::class_name())
      errors.add("Mtg_request_handler::Mtg_request", "3", "invalid request class: " + request_class_name);
  }
  if (!errors.has())
    from_json(request_json, errors);
}

/*
  {
    "class_name": "mtg_request",
    "request": "mtg_load",
    "arguments": [
      "../data/title.basics.tsv"
    ]
  }
 */
void Mtg_request::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_name": ... `
  std::string jobj_class_name =
      Bdb_json_utils::get_json_string("Mtg_request::from_json", "1", jobj, "class_name", errors);
  if (!errors.has() && jobj_class_name != class_name())
    errors.add("Mtg_request::from_json", "1", "not class mtg_request");
  // parse: request": ...
  request = Bdb_json_utils::get_json_string("Mtg_request::from_json", "3", jobj, "request", errors);
  if (!errors.has()) {
    // parse: ' { "arguments": ... `
    json_object *arguments_json =
        Bdb_json_utils::get_json_object("Mtg_request::from_json", "2", jobj, "arguments", json_type_array,
                                        errors);
    if (!errors.has()) {
      size_t n_args = json_object_array_length(arguments_json);
      for (size_t i = 0; i < n_args && !errors.has(); i++) {
        json_object *argument_json = json_object_array_get_idx(arguments_json, i);
        std::string
            argument = Bdb_json_utils::get_json_string_type("Mtg_request::from_json", "3", argument_json,
                                                            errors);
        if (!errors.has())
          arguments.push_back(argument);
      }
    }
  }
}

json_object *Mtg_request::process_request(
    Mtg_inet_app_init &mtg_inet_app_init,
    const std::string &request,
    Bdb_errors &errors) {
  Mtg_request_response mtg_request_response(errors);
  json_object *request_json = json_tokener_parse(request.c_str());
  if (request_json == nullptr)
    errors.add("Mtg_request::process_request",
               "1",
               "invalid request json: " + request, errno);
  std::string request_class_name;
  if (!errors.has()) {
    mtg_request_response.add_request(request_json);
    request_class_name =
        Bdb_json_utils::get_json_string("Mtg_request::process_request", "2",
                                        request_json, "class_name",
                                        errors);
  }
  if (!errors.has()) {
    if (request_class_name == Mtg_request::class_name()) {
      Mtg_request mtg_request(request_json, errors);
      if (!errors.has()) {
        json_object *jobj = mtg_request.handle(mtg_inet_app_init, errors);
        if (!errors.has())
          mtg_request_response.add_response(jobj);
      }
    } else
      errors.add("Mtg_request::process_request", "3", "invalid request class: " + request_class_name);
  }
  if (errors.has()) {
    mtg_request_response.cleanup();
    return errors.to_json();
  } else
    return mtg_request_response.jobj;
}

json_object *Mtg_request::process_search_name_request(
    Mtg_inet_app_init &mtg_inet_app_init, Bdb_errors &errors) {
  if (arguments.empty())
    errors.add("Mtg_request::process_search_name_request", "1", "missing name_id");
  Primary_database_config principals_primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("principals", principals_primary_database_config, errors);
  Secondary_database_config principals_title_id_secondary_database_config;
  principals_primary_database_config.select("principals_title_id",
                                            principals_title_id_secondary_database_config,
                                            errors);
  Primary_database_config name_primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("name", name_primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
        std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database name_db(name_primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home,
                             errors);
    Primary_database
        principals_db(principals_primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home,
                      errors);
    Secondary_database
        principals_title_id_sdb(principals_title_id_secondary_database_config, mtg_inet_app_init.db_home, errors);
    if (!errors.has()) {
      std::string title_id = arguments.at(0);
//      Name_DTO_list name_dto_list;
//      Principals_DAO::search_by_title(principals_title_id_sdb.bdb_db,
//                                      principals_db.bdb_db,
//                                      name_db.bdb_db,
//                                      title_id,
//                                      name_dto_list,
//                                      errors);
//      if (!errors.has()) {
//        json_object *name_dto_list_json = name_dto_list.to_json(errors);
//        if (!errors.has()) {
//          return name_dto_list_json;
//        }
      //     }
    }
  }
  return nullptr;
}

json_object *Mtg_request::process_search_name_tripthongs_request(
    Mtg_inet_app_init &mtg_inet_app_init, Bdb_errors &errors) {
  if (arguments.size() < 3)
    errors.add("Mtg_request::process_search_name_tripthongs_request",
               "1",
               "parameters: text, min-score, score-count");
  std::string text;
  int min_score;
  int score_count;
  if (!errors.has()) {
    text = arguments.at(0);
    min_score = Misc_utils::string_to_int(arguments.at(1), errors);
    score_count = Misc_utils::string_to_int(arguments.at(2), errors);
  }
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
        std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database_config name_primary_database_config;
    mtg_inet_app_init.bdb_databases_config.select("name", name_primary_database_config, errors);
    if (!errors.has()) {
      Primary_database name_db(name_primary_database_config,
                               mtg_bdb_key_extractor.get(),
                               mtg_inet_app_init.db_home,
                               errors);
      Primary_database_config name_tripthongs_primary_database_config;
      mtg_inet_app_init.bdb_databases_config.select("name_tripthongs",
                                                    name_tripthongs_primary_database_config,
                                                    errors);
      if (!errors.has()) {
        Primary_database name_tripthongs_db(name_tripthongs_primary_database_config,
                                            mtg_bdb_key_extractor.get(),
                                            mtg_inet_app_init.db_home,
                                            errors);
        if (!errors.has()) {
          Timer timer;
          Bdb_text_tripthongs bdb_text_tripthongs(text);
          std::map<std::string, int> tripthong_counts;
          std::map<std::string, int> tripthong_match_counts;
          std::map<std::string, int> id_scores;
          for (const auto &text_tripthong_occurrence: bdb_text_tripthongs.list) {
            Bdb_text_id_occurrence_list bdb_text_id_occurrence_list;
            Bdb_DAO::select_tripthongs_by_key_list(name_tripthongs_db.bdb_db,
                                                   text_tripthong_occurrence.tripthong,
                                                   bdb_text_id_occurrence_list,
                                                   errors);
            if (!errors.has()) {
              for (const auto &text_id_occurence: bdb_text_id_occurrence_list.list) {
                std::string id = text_id_occurence.id;
                tripthong_counts[id] = text_id_occurence.tripthongs_count;
                tripthong_match_counts[id] += std::min(text_id_occurence.occurrence_count,
                                                       text_tripthong_occurrence.occurrence_count);
              }
            } else
              break;
          }
          if (errors.has()) return nullptr;
          // sort scores
          for (const auto &tripthong_count: tripthong_counts) {
            std::string id = tripthong_count.first;
            // match in integer percent
            id_scores[id] = (100 * tripthong_match_counts[id] * tripthong_match_counts[id]) /
                (bdb_text_tripthongs.tripthongs_count * tripthong_counts[id]);
          }
          std::map<int, std::string, std::greater<> > id_score_sort;
          for (const auto &id_score_entry: id_scores)
            id_score_sort[id_score_entry.second] = id_score_entry.first;
          Name_DTO_list name_dto_list;
          int name_count{};
          for (const auto &id_score_sort_entry: id_score_sort) {
            int score = id_score_sort_entry.first;
            if (score > min_score && name_count < score_count) {
              std::string name_id = id_score_sort_entry.second;
              Name_DTO name_dto;
              Name_DAO::lookup(name_db.bdb_db, name_id, name_dto, errors);
              name_dto.score = std::to_string(score);
              if (!errors.has()) {
                name_dto_list.list.push_back(name_dto);
              } else
                break;
            }
            ++name_count;
          }
          timer.end();
          json_object *name_dto_list_json = name_dto_list.to_json(errors);
          if (!errors.has()) {
            return name_dto_list_json;
          }
        }
      }
    }
  }
//  }
  return nullptr;
}

json_object *Mtg_request::process_search_title_request(
    Mtg_inet_app_init &mtg_inet_app_init, Bdb_errors &errors) {
  if (arguments.empty())
    errors.add("Mtg_request::process_search_title_request", "1", "missing name_id");
  Primary_database_config principals_primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("principals", principals_primary_database_config, errors);
  Secondary_database_config principals_name_id_secondary_database_config;
  principals_primary_database_config.select("principals_name_id",
                                            principals_name_id_secondary_database_config,
                                            errors);
  Primary_database_config title_primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("title", title_primary_database_config, errors);
  Primary_database_config ratings_primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("ratings", ratings_primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
        std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database
        title_db(title_primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home, errors);
    Primary_database
        ratings_db(ratings_primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home, errors);
    Primary_database
        principals_db(principals_primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home,
                      errors);
    Secondary_database
        principals_name_id_sdb(principals_name_id_secondary_database_config, mtg_inet_app_init.db_home, errors);
    if (!errors.has()) {
      std::string name_id = arguments.at(0);
      Title_DTO_list title_dto_list;
      Principals_DAO::search_by_name(principals_name_id_sdb.bdb_db,
                                     principals_db.bdb_db,
                                     title_db.bdb_db,
                                     ratings_db.bdb_db,
                                     name_id,
                                     title_dto_list,
                                     errors);
      // TODO: convert principals list to name list
      if (!errors.has()) {
        json_object *title_dto_list_json = title_dto_list.to_json(errors);
        if (!errors.has()) {
          return title_dto_list_json;
        }
      }
    }
  }
  return nullptr;
}

json_object *Mtg_request::process_search_title_tripthongs_request(
    Mtg_inet_app_init &mtg_inet_app_init, Bdb_errors &errors) {
  if (arguments.size() < 3)
    errors.add("Mtg_request::process_search_title_tripthongs_request",
               "1",
               "parameters: text, min-score, score-count");
  std::string text;
  int min_score;
  int score_count;
  if (!errors.has()) {
    text = arguments.at(0);
    min_score = Misc_utils::string_to_int(arguments.at(1), errors);
    score_count = Misc_utils::string_to_int(arguments.at(2), errors);
  }
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
        std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database_config title_primary_database_config;
    mtg_inet_app_init.bdb_databases_config.select("title", title_primary_database_config, errors);
    if (!errors.has()) {
      Primary_database title_db(title_primary_database_config,
                                mtg_bdb_key_extractor.get(),
                                mtg_inet_app_init.db_home,
                                errors);
      Primary_database_config ratings_primary_database_config;
      mtg_inet_app_init.bdb_databases_config.select("ratings", ratings_primary_database_config, errors);
      if (!errors.has()) {
        Primary_database ratings_db(ratings_primary_database_config,
                                    mtg_bdb_key_extractor.get(),
                                    mtg_inet_app_init.db_home,
                                    errors);
        Primary_database_config title_tripthongs_primary_database_config;
        mtg_inet_app_init.bdb_databases_config.select("title_tripthongs",
                                                      title_tripthongs_primary_database_config,
                                                      errors);
        if (!errors.has()) {
          Primary_database title_tripthongs_db(title_tripthongs_primary_database_config,
                                               mtg_bdb_key_extractor.get(),
                                               mtg_inet_app_init.db_home,
                                               errors);
          if (!errors.has()) {
            Timer timer;
            Bdb_text_tripthongs bdb_text_tripthongs(text);
            std::map<std::string, int> tripthong_counts;
            std::map<std::string, int> tripthong_match_counts;
            std::map<std::string, int> id_scores;
            for (const auto &text_tripthong_occurrence: bdb_text_tripthongs.list) {
              Bdb_text_id_occurrence_list bdb_text_id_occurrence_list;
              Bdb_DAO::select_tripthongs_by_key_list(title_tripthongs_db.bdb_db,
                                                     text_tripthong_occurrence.tripthong,
                                                     bdb_text_id_occurrence_list,
                                                     errors);
              if (!errors.has()) {
                for (const auto &text_id_occurence: bdb_text_id_occurrence_list.list) {
                  std::string id = text_id_occurence.id;
                  tripthong_counts[id] = text_id_occurence.tripthongs_count;
                  tripthong_match_counts[id] += std::min(text_id_occurence.occurrence_count,
                                                         text_tripthong_occurrence.occurrence_count);
                }
              }
            }
            if (errors.has())
              return nullptr;
            // sort scores
            for (const auto &tripthong_count: tripthong_counts) {
              std::string id = tripthong_count.first;
              // match in integer percent
              id_scores[id] = (100 * tripthong_match_counts[id] * tripthong_match_counts[id]) /
                  (bdb_text_tripthongs.tripthongs_count * tripthong_counts[id]);
            }
            std::map<int, std::string, std::greater<> > id_score_sort;
            for (const auto &id_score_entry: id_scores)
              id_score_sort[id_score_entry.second] = id_score_entry.first;
            Title_DTO_list title_dto_list;
            int title_count{};
            for (const auto &id_score_sort_entry: id_score_sort) {
              int score = id_score_sort_entry.first;
              if (score > min_score && title_count < score_count) {
                std::string title_id = id_score_sort_entry.second;
                Title_DTO title_dto;
                Title_DAO::lookup(title_db.bdb_db, ratings_db.bdb_db, title_id, title_dto, errors);
                title_dto.score = std::to_string(score);
                if (!errors.has()) {
                  title_dto_list.list.push_back(title_dto);
                } else
                  break;
              }
              ++title_count;
            }
            timer.end();
            if (errors.has())
              return nullptr;
            json_object *title_dto_list_json = title_dto_list.to_json(errors);
            if (!errors.has()) {
              return title_dto_list_json;
            }
          }
        }
      }
    }
  }
  return nullptr;
}

json_object *Mtg_request::process_select_name_request(Mtg_inet_app_init &mtg_inet_app_init,
                                                      Bdb_errors &errors) {
  return process_search_name_tripthongs_request(mtg_inet_app_init, errors);
}

json_object *Mtg_request::process_select_ratings_request(Mtg_inet_app_init &mtg_inet_app_init,
                                                         Bdb_errors &errors) {
  Primary_database_config primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("ratings", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
        std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database ratings_db(primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home,
                                errors);
    if (!errors.has()) {
      if (!errors.has()) {
        json_object *ratings_dto_list_json = ratings_dto_list.to_json(errors);
        if (!errors.has()) {
          return ratings_dto_list_json;
        }
      }
    }
  }
  return nullptr;
}

json_object *Mtg_request::process_select_title_request(Mtg_inet_app_init &mtg_inet_app_init,
                                                       Bdb_errors &errors) {
  return process_search_title_tripthongs_request(mtg_inet_app_init, errors);
}

json_object *Mtg_request::process_update_name_request(Mtg_inet_app_init &mtg_inet_app_init,
                                                      Bdb_errors &errors) {
  if (arguments.size() < 3)
    errors.add("Mtg_request::process_update_name_request", "1", "missing name_id");
  Primary_database_config primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("name", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
        std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database name_db(primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home, errors);
    if (!errors.has()) {
      std::string name_id = arguments.at(0);
      std::string user_ratings = arguments.at(1);
      std::string priority = arguments.at(2);
      Name_DTO name_dto;
      Name_DAO::update(name_db.bdb_db, name_id, user_ratings, priority, name_dto, errors);
      if (!errors.has()) {
        json_object *name_json = name_dto.to_json(errors);
        if (!errors.has()) {
          return name_json;
        }
      }
    }
  }
  return nullptr;
}

json_object *Mtg_request::process_update_title_request(Mtg_inet_app_init &mtg_inet_app_init,
                                                       Bdb_errors &errors) {
  if (arguments.size() < 3)
    errors.add("Mtg_request::process_update_name_request", "1", "missing title_id");
  std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
      std::make_unique<Mtg_bdb_key_extractor>();
  Primary_database_config primary_title_database_config;
  mtg_inet_app_init.bdb_databases_config.select("title", primary_title_database_config, errors);
  if (!errors.has()) {
    Primary_database_config primary_ratings_database_config;
    mtg_inet_app_init.bdb_databases_config.select("ratings", primary_ratings_database_config, errors);
    if (!errors.has()) {
      Primary_database
          title_db(primary_title_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home, errors);
      Primary_database
          ratings_db(primary_ratings_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home,
                     errors);
      if (!errors.has()) {
        if (!errors.has()) {
          std::string title_id = arguments.at(0);
          std::string user_ratings = arguments.at(1);
          std::string priority = arguments.at(2);
          Title_DTO title_dto;
          Title_DAO::update(title_db.bdb_db,
                            ratings_db.bdb_db,
                            title_id,
                            user_ratings,
                            priority,
                            title_dto,
                            errors);
          if (!errors.has()) {
            json_object *title_json = title_dto.to_json(errors);
            if (!errors.has()) {
              return title_json;
            }
          }
        }
      }
    }
  }
  return nullptr;
}

json_object *Mtg_request::handle(Mtg_inet_app_init &mtg_inet_app_init,
                                 Bdb_errors &errors) {
  json_object *request_response = nullptr;
  if (request == "load_name") {
    request_response = process_load_name_request(mtg_inet_app_init, errors);
  } else if (request == "load_principals") {
    request_response = process_load_principals_request(mtg_inet_app_init, errors);
  } else if (request == "load_ratings") {
    request_response = process_load_ratings_request(mtg_inet_app_init, errors);
  } else if (request == "load_title") {
    request_response = process_load_title_request(mtg_inet_app_init, errors);
  } else if (request == "lookup_name") {
    request_response = process_lookup_name_request(mtg_inet_app_init, errors);
  } else if (request == "lookup_ratings") {
    request_response = process_lookup_ratings_request(mtg_inet_app_init, errors);
  } else if (request == "lookup_title") {
    request_response = process_lookup_title_request(mtg_inet_app_init, errors);
  } else if (request == "search_name") {
    request_response = process_search_name_request(mtg_inet_app_init, errors);
  } else if (request == "search_name_tripthongs") {
    request_response = process_search_name_tripthongs_request(mtg_inet_app_init, errors);
  } else if (request == "search_title") {
    request_response = process_search_title_request(mtg_inet_app_init, errors);
  } else if (request == "search_title_tripthongs") {
    request_response = process_search_title_tripthongs_request(mtg_inet_app_init, errors);
  } else if (request == "select_name") {
    request_response = process_select_name_request(mtg_inet_app_init, errors);
  } else if (request == "select_principals") {
    request_response = process_select_principals_request(mtg_inet_app_init, errors);
  } else if (request == "select_ratings") {
    request_response = process_select_ratings_request(mtg_inet_app_init, errors);
  } else if (request == "select_title") {
    request_response = process_select_title_request(mtg_inet_app_init, errors);
  } else if (request == "update_name") {
    request_response = process_update_name_request(mtg_inet_app_init, errors);
  } else if (request == "update_title") {
    request_response = process_update_title_request(mtg_inet_app_init, errors);
  } else
    errors.add("Mtg_request::run", "1", "unrecognized request " + request);
  if (!errors.has())
    return request_response;
  else
    return errors.to_json();
}

json_object *Mtg_request::to_json(Bdb_errors &errors) {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Mtg_request::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_name", json_object_new_string(class_name().c_str()));
  json_object_object_add(root, "request", json_object_new_string(request.c_str()));

  json_object *arguments_json = json_object_new_array();
  json_object_object_add(root, "arguments", arguments_json);
  for (const auto &argument: arguments) {
    json_object *argument_json = json_object_new_string(argument.c_str());
    json_object_array_add(arguments_json, argument_json);
  }
  return root;
}

std::string Mtg_request::to_string() const {
  std::ostringstream os;
  os << "mtg_request:" << std::endl;
  os << "request        " << request << std::endl;
  os << "arguments       " << std::endl;
  for (const auto &argument: arguments)
    os << "\t\t    " << argument << std::endl;
  return os.str();
}

// Mtg_request_response methods

Mtg_request_response::Mtg_request_response(Bdb_errors &errors) {
  jobj = json_object_new_object();
  if (!jobj) {
    errors.add("Mtg_request_response::to_json", "1", "json-c allocate error");
  } else {
    json_object_object_add(jobj, "class_name", json_object_new_string(class_name().c_str()));
  }
}

void Mtg_request_response::add_array() {
  jobj_array = json_object_new_array();
  json_object_object_add(jobj, "mtg_request_responses", jobj_array);
}

void Mtg_request_response::add_request(json_object *request_json) const {
  json_object_object_add(jobj, "mtg_request", request_json);
}

void Mtg_request_response::add_response(json_object *response_json) const {
  json_object_object_add(jobj, "mtg_request_response", response_json);
}

void Mtg_request_response::add_response_array(json_object *response_json) const {
  json_object_array_add(jobj_array, response_json);
}

void Mtg_request_response::cleanup() const {
  json_object_put(jobj);
}

json_object *Mtg_request_response::to_load_response(int count, Timer &timer, Bdb_errors &errors) {
  json_object *load_response_json = json_object_new_object();
  if (!load_response_json) {
    errors.add("Mtg_request::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(load_response_json, "class_name", json_object_new_string(class_name().c_str()));
  json_object_object_add(load_response_json, "record_count", json_object_new_string(std::to_string(count).c_str()));
  return load_response_json;
}

std::string Mtg_request_response::to_string() const {
  return json_object_to_json_string_ext(jobj,
                                        JSON_C_TO_STRING_PRETTY);
}

void Mtg_request_handler::handle(Mtg_inet_app_init &mtg_inet_app_init,
                                 const Mtg_request &mtg_request,
                                 Mtg_request_response &mtg_request_response,
                                 Bdb_errors &errors) {
  mtg_request_response.add_response(mtg_request.request_json);
  if (!Mtg_account_request_handler::handle(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_card_request_handler::handle(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_deck_request_handler::handle(mtg_inet_app_init, mtg_request, mtg_request_response, errors))
    errors.add("Mtg_request_handler::handle", "", "invalid request: " + mtg_request.request);
}

// Mtg_account_request_handler methods

bool Mtg_account_request_handler::handle(Mtg_inet_app_init &mtg_inet_app_init,
                                         const Mtg_request &mtg_request,
                                         Mtg_request_response &mtg_request_response,
                                         Bdb_errors &errors) {
  if (!Mtg_account_request_handler::lookup(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_account_request_handler::search_username(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_account_request_handler::search_email(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_account_request_handler::select_all(mtg_inet_app_init, mtg_request, mtg_request_response, errors))
    return false;
  return true;
}

bool Mtg_account_request_handler::lookup(Mtg_inet_app_init &mtg_inet_app_init,
                                         const Mtg_request &mtg_request,
                                         Mtg_request_response &mtg_request_response,
                                         Bdb_errors &errors) {
  if (mtg_request.request != "account_lookup")
    return false;
  if (mtg_request.arguments.empty())
    errors.add("Mtg_account_request_handler::lookup", "1", "missing account_id");
  Primary_database_config primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("account", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
        std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database
        account_db(primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home, errors);
    if (!errors.has()) {
      std::string account_id = mtg_request.arguments.at(0);
      Account_DTO account_dto;
      Account_DAO::lookup(account_db.bdb_db, account_id, account_dto, errors);
      if (!errors.has()) {
        json_object *account_json = account_dto.to_json(errors);
        if (!errors.has()) {
          return account_json;
        }
      }
    }
  }
  return true;
}

bool Mtg_account_request_handler::search_username(Mtg_inet_app_init &mtg_inet_app_init,
                                                  const Mtg_request &mtg_request,
                                                  Mtg_request_response &mtg_request_response,
                                                  Bdb_errors &errors) {
  if (mtg_request.request != "account_search_username")
    return false;
  if (mtg_request.arguments.size() < 3)
    errors.add("Mtg_request::process_search_account_tripthongs_request",
               "1",
               "parameters: text, min-score, score-count");
  std::string text;
  int min_score;
  int score_count;
  if (!errors.has()) {
    text = mtg_request.arguments.at(0);
    min_score = Misc_utils::string_to_int(mtg_request.arguments.at(1), errors);
    score_count = Misc_utils::string_to_int(mtg_request.arguments.at(2), errors);
  }
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor = std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database_config account_primary_database_config;
    mtg_inet_app_init.bdb_databases_config.select("account", account_primary_database_config, errors);
    if (!errors.has()) {
      Primary_database account_db(account_primary_database_config,
                                  mtg_bdb_key_extractor.get(),
                                  mtg_inet_app_init.db_home,
                                  errors);
      Primary_database_config account_username_tripthongs_primary_database_config;
      mtg_inet_app_init.bdb_databases_config.select("account_username_tripthongs",
                                                    account_username_tripthongs_primary_database_config,
                                                    errors);
      if (!errors.has()) {
        Primary_database account_username_tripthongs_db(account_username_tripthongs_primary_database_config,
                                                        mtg_bdb_key_extractor.get(),
                                                        mtg_inet_app_init.db_home,
                                                        errors);
        if (!errors.has()) {
          Timer timer;
          Bdb_text_tripthongs bdb_text_tripthongs(text);
          std::map<std::string, int> tripthong_counts;
          std::map<std::string, int> tripthong_match_counts;
          std::map<std::string, int> id_scores;
          for (const auto &text_tripthong_occurrence: bdb_text_tripthongs.list) {
            Bdb_text_id_occurrence_list bdb_text_id_occurrence_list;
            Bdb_DAO::select_tripthongs_by_key_list(account_username_tripthongs_db.bdb_db,
                                                   text_tripthong_occurrence.tripthong,
                                                   bdb_text_id_occurrence_list,
                                                   errors);
            if (!errors.has()) {
              for (const auto &text_id_occurence: bdb_text_id_occurrence_list.list) {
                std::string id = text_id_occurence.id;
                tripthong_counts[id] = text_id_occurence.tripthongs_count;
                tripthong_match_counts[id] += std::min(text_id_occurence.occurrence_count,
                                                       text_tripthong_occurrence.occurrence_count);
              }
            } else
              break;
          }
          if (errors.has()) return true;
          // sort scores
          for (const auto &tripthong_count: tripthong_counts) {
            std::string id = tripthong_count.first;
            // match in integer percent
            id_scores[id] = (100 * tripthong_match_counts[id] * tripthong_match_counts[id]) /
                (bdb_text_tripthongs.tripthongs_count * tripthong_counts[id]);
          }
          std::map<int, std::string, std::greater<> > id_score_sort;
          for (const auto &id_score_entry: id_scores)
            id_score_sort[id_score_entry.second] = id_score_entry.first;
          Account_DTO_list account_dto_list;
          int account_count{};
          for (const auto &id_score_sort_entry: id_score_sort) {
            int score = id_score_sort_entry.first;
            if (score > min_score && account_count < score_count) {
              std::string account_id = id_score_sort_entry.second;
              Account_DTO account_dto;
              Account_DAO::lookup(account_db.bdb_db, account_id, account_dto, errors);
              account_dto.score = std::to_string(score);
              if (!errors.has()) {
                account_dto_list.list.push_back(account_dto);
              } else
                break;
            }
            ++account_count;
          }
          timer.end();
          json_object *account_dto_list_json = account_dto_list.to_json(errors);
          if (!errors.has()) {
            return account_dto_list_json;
          }
        }
      }
    }
  }
  return true;
}

bool Mtg_account_request_handler::search_email(Mtg_inet_app_init &mtg_inet_app_init,
                                               const Mtg_request &mtg_request,
                                               Mtg_request_response &mtg_request_response,
                                               Bdb_errors &errors) {
  if (mtg_request.request != "account_search_email")
    return false;
  if (mtg_request.arguments.size() < 3)
    errors.add("Mtg_request::process_search_account_tripthongs_request",
               "1",
               "parameters: text, min-score, score-count");
  std::string text;
  int min_score;
  int score_count;
  if (!errors.has()) {
    text = mtg_request.arguments.at(0);
    min_score = Misc_utils::string_to_int(mtg_request.arguments.at(1), errors);
    score_count = Misc_utils::string_to_int(mtg_request.arguments.at(2), errors);
  }
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor = std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database_config account_primary_database_config;
    mtg_inet_app_init.bdb_databases_config.select("account", account_primary_database_config, errors);
    if (!errors.has()) {
      Primary_database account_db(account_primary_database_config,
                                  mtg_bdb_key_extractor.get(),
                                  mtg_inet_app_init.db_home,
                                  errors);
      Primary_database_config account_email_tripthongs_primary_database_config;
      mtg_inet_app_init.bdb_databases_config.select("account_email_tripthongs",
                                                    account_email_tripthongs_primary_database_config,
                                                    errors);
      if (!errors.has()) {
        Primary_database account_email_tripthongs_db(account_email_tripthongs_primary_database_config,
                                                        mtg_bdb_key_extractor.get(),
                                                        mtg_inet_app_init.db_home,
                                                        errors);
        if (!errors.has()) {
          Timer timer;
          Bdb_text_tripthongs bdb_text_tripthongs(text);
          std::map<std::string, int> tripthong_counts;
          std::map<std::string, int> tripthong_match_counts;
          std::map<std::string, int> id_scores;
          for (const auto &text_tripthong_occurrence: bdb_text_tripthongs.list) {
            Bdb_text_id_occurrence_list bdb_text_id_occurrence_list;
            Bdb_DAO::select_tripthongs_by_key_list(account_email_tripthongs_db.bdb_db,
                                                   text_tripthong_occurrence.tripthong,
                                                   bdb_text_id_occurrence_list,
                                                   errors);
            if (!errors.has()) {
              for (const auto &text_id_occurence: bdb_text_id_occurrence_list.list) {
                std::string id = text_id_occurence.id;
                tripthong_counts[id] = text_id_occurence.tripthongs_count;
                tripthong_match_counts[id] += std::min(text_id_occurence.occurrence_count,
                                                       text_tripthong_occurrence.occurrence_count);
              }
            } else
              break;
          }
          if (errors.has()) return true;
          // sort scores
          for (const auto &tripthong_count: tripthong_counts) {
            std::string id = tripthong_count.first;
            // match in integer percent
            id_scores[id] = (100 * tripthong_match_counts[id] * tripthong_match_counts[id]) /
                (bdb_text_tripthongs.tripthongs_count * tripthong_counts[id]);
          }
          std::map<int, std::string, std::greater<> > id_score_sort;
          for (const auto &id_score_entry: id_scores)
            id_score_sort[id_score_entry.second] = id_score_entry.first;
          Account_DTO_list account_dto_list;
          int account_count{};
          for (const auto &id_score_sort_entry: id_score_sort) {
            int score = id_score_sort_entry.first;
            if (score > min_score && account_count < score_count) {
              std::string account_id = id_score_sort_entry.second;
              Account_DTO account_dto;
              Account_DAO::lookup(account_db.bdb_db, account_id, account_dto, errors);
              account_dto.score = std::to_string(score);
              if (!errors.has()) {
                account_dto_list.list.push_back(account_dto);
              } else
                break;
            }
            ++account_count;
          }
          timer.end();
          json_object *account_dto_list_json = account_dto_list.to_json(errors);
          if (!errors.has()) {
            return account_dto_list_json;
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
  Primary_database_config primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("account", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
        std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database
        account_db(primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home, errors);
    if (!errors.has()) {
      Account_DTO_list account_dto_list;
      Account_DAO::select_all(account_db.bdb_db, account_dto_list, errors);
      if (!errors.has()) {
        json_object *account_list_json = account_dto_list.to_json(errors);
        if (!errors.has()) {
          return account_list_json;
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
  if (!Mtg_card_request_handler::lookup(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_card_request_handler::search_name(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_card_request_handler::select_all(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_card_request_handler::select_decks(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_card_request_handler::select_in_pool(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_card_request_handler::select_type_id(mtg_inet_app_init, mtg_request, mtg_request_response, errors))
    return false;
  return true;
}

bool Mtg_card_request_handler::lookup(Mtg_inet_app_init &mtg_inet_app_init,
                                      const Mtg_request &mtg_request,
                                      Mtg_request_response &mtg_request_response,
                                      Bdb_errors &errors) {
  if (mtg_request.request == "lookup_card") {
    if (mtg_request.arguments.empty())
      errors.add("Mtg_card_request_handler::lookup", "1", "missing card_id");
    Primary_database_config primary_database_config;
    mtg_inet_app_init.bdb_databases_config.select("card", primary_database_config, errors);
    if (!errors.has()) {
      std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
          std::make_unique<Mtg_bdb_key_extractor>();
      Primary_database
          card_db(primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home, errors);
      if (!errors.has()) {
        std::string card_id = mtg_request.arguments.at(0);
        Card_DTO card_dto;
        Card_DAO::lookup(card_db.bdb_db, card_id, card_dto, errors);
        if (!errors.has()) {
          json_object *card_json = card_dto.to_json(errors);
          if (!errors.has()) {
            return card_json;
          }
        }
      }
    }
    return true;
  } else
    return false;
}

bool Mtg_card_request_handler::search_name(Mtg_inet_app_init &mtg_inet_app_init,
                                           const Mtg_request &mtg_request,
                                           Mtg_request_response &mtg_request_response,
                                           Bdb_errors &errors) {
  if (mtg_request.request != "card_search_name")
    return false;
  if (mtg_request.arguments.size() < 3)
    errors.add("Mtg_request::process_search_card_tripthongs_request",
               "1",
               "parameters: text, min-score, score-count");
  std::string text;
  int min_score;
  int score_count;
  if (!errors.has()) {
    text = mtg_request.arguments.at(0);
    min_score = Misc_utils::string_to_int(mtg_request.arguments.at(1), errors);
    score_count = Misc_utils::string_to_int(mtg_request.arguments.at(2), errors);
  }
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor = std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database_config card_primary_database_config;
    mtg_inet_app_init.bdb_databases_config.select("card", card_primary_database_config, errors);
    if (!errors.has()) {
      Primary_database card_db(card_primary_database_config,
                                  mtg_bdb_key_extractor.get(),
                                  mtg_inet_app_init.db_home,
                                  errors);
      Primary_database_config card_name_tripthongs_primary_database_config;
      mtg_inet_app_init.bdb_databases_config.select("card_name_tripthongs",
                                                    card_name_tripthongs_primary_database_config,
                                                    errors);
      if (!errors.has()) {
        Primary_database card_name_tripthongs_db(card_name_tripthongs_primary_database_config,
                                                     mtg_bdb_key_extractor.get(),
                                                     mtg_inet_app_init.db_home,
                                                     errors);
        if (!errors.has()) {
          Timer timer;
          Bdb_text_tripthongs bdb_text_tripthongs(text);
          std::map<std::string, int> tripthong_counts;
          std::map<std::string, int> tripthong_match_counts;
          std::map<std::string, int> id_scores;
          for (const auto &text_tripthong_occurrence: bdb_text_tripthongs.list) {
            Bdb_text_id_occurrence_list bdb_text_id_occurrence_list;
            Bdb_DAO::select_tripthongs_by_key_list(card_name_tripthongs_db.bdb_db,
                                                   text_tripthong_occurrence.tripthong,
                                                   bdb_text_id_occurrence_list,
                                                   errors);
            if (!errors.has()) {
              for (const auto &text_id_occurence: bdb_text_id_occurrence_list.list) {
                std::string id = text_id_occurence.id;
                tripthong_counts[id] = text_id_occurence.tripthongs_count;
                tripthong_match_counts[id] += std::min(text_id_occurence.occurrence_count,
                                                       text_tripthong_occurrence.occurrence_count);
              }
            } else
              break;
          }
          if (errors.has()) return true;
          // sort scores
          for (const auto &tripthong_count: tripthong_counts) {
            std::string id = tripthong_count.first;
            // match in integer percent
            id_scores[id] = (100 * tripthong_match_counts[id] * tripthong_match_counts[id]) /
                (bdb_text_tripthongs.tripthongs_count * tripthong_counts[id]);
          }
          std::map<int, std::string, std::greater<> > id_score_sort;
          for (const auto &id_score_entry: id_scores)
            id_score_sort[id_score_entry.second] = id_score_entry.first;
          Card_DTO_list card_dto_list;
          int card_count{};
          for (const auto &id_score_sort_entry: id_score_sort) {
            int score = id_score_sort_entry.first;
            if (score > min_score && card_count < score_count) {
              std::string card_id = id_score_sort_entry.second;
              Card_DTO card_dto;
              Card_DAO::lookup(card_db.bdb_db, card_id, card_dto, errors);
              card_dto.score = std::to_string(score);
              if (!errors.has()) {
                card_dto_list.list.push_back(card_dto);
              } else
                break;
            }
            ++card_count;
          }
          timer.end();
          json_object *card_dto_list_json = card_dto_list.to_json(errors);
          if (!errors.has()) {
            return card_dto_list_json;
          }
        }
      }
    }
  }
  return true;
}

bool Mtg_card_request_handler::select_all(Mtg_inet_app_init &mtg_inet_app_init,
                                          const Mtg_request &mtg_request,
                                          Mtg_request_response &mtg_request_response,
                                          Bdb_errors &errors) {
  if (mtg_request.request != "card_select_all")
    return false;
  Primary_database_config primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("card", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
        std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database
        card_db(primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home, errors);
    if (!errors.has()) {
      Card_DTO_list card_dto_list;
      Card_DAO::select_all(card_db.bdb_db, card_dto_list, errors);
      if (!errors.has()) {
        json_object *card_list_json = card_dto_list.to_json(errors);
        if (!errors.has()) {
          return card_list_json;
        }
      }
    }
  }
  return true;
}

bool Mtg_card_request_handler::select_decks(Mtg_inet_app_init &mtg_inet_app_init,
                                            const Mtg_request &mtg_request,
                                            Mtg_request_response &mtg_request_response,
                                            Bdb_errors &errors) {
  if (mtg_request.request != "card_select_decks")
    return false;
  if (mtg_request.arguments.empty())
    errors.add("Mtg_card_request_handler::select_decks", "1", "missing card_id");
  Primary_database_config deck_card_primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("deck_card", deck_card_primary_database_config, errors);
  Secondary_database_config deck_card_title_id_secondary_database_config;
  deck_card_primary_database_config.select("deck_card_title_id",
                                            deck_card_title_id_secondary_database_config,
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
        deck_card_title_id_sdb(deck_card_title_id_secondary_database_config, mtg_inet_app_init.db_home, errors);
    if (!errors.has()) {
      std::string title_id = mtg_request.arguments.at(0);
      Card_DTO_list card_dto_list;
      Deck_card_DAO::search_by_card(deck_card_title_id_sdb.bdb_db,
                                      deck_card_db.bdb_db,
                                      card_db.bdb_db,
                                      title_id,
                                      card_dto_list,
                                      errors);
      if (!errors.has()) {
        json_object *card_dto_list_json = card_dto_list.to_json(errors);
        if (!errors.has()) {
          return card_dto_list_json;
        }
           }
    }
  }
  return nullptr;
}

bool Mtg_card_request_handler::select_in_pool(Mtg_inet_app_init &mtg_inet_app_init,
                                              const Mtg_request &mtg_request,
                                              Mtg_request_response &mtg_request_response,
                                              Bdb_errors &errors) {
}

bool Mtg_card_request_handler::select_type_id(Mtg_inet_app_init &mtg_inet_app_init,
                                              const Mtg_request &mtg_request,
                                              Mtg_request_response &mtg_request_response,
                                              Bdb_errors &errors) {
}


// Mtg_deck_request_handler methods

bool Mtg_deck_request_handler::handle(Mtg_inet_app_init &mtg_inet_app_init,
                                      const Mtg_request &mtg_request,
                                      Mtg_request_response &mtg_request_response,
                                      Bdb_errors &errors) {
  if (!Mtg_deck_request_handler::lookup(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_deck_request_handler::search_name(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_deck_request_handler::select_account_id(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_deck_request_handler::select_all(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_deck_request_handler::select_all_cards(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_deck_request_handler::select_cards_type_id(mtg_inet_app_init, mtg_request, mtg_request_response, errors)
      && !Mtg_deck_request_handler::select_other_cards(mtg_inet_app_init, mtg_request, mtg_request_response, errors))
    return false;
  return true;
}

bool Mtg_deck_request_handler::lookup(Mtg_inet_app_init &mtg_inet_app_init,
                                      const Mtg_request &mtg_request,
                                      Mtg_request_response &mtg_request_response,
                                      Bdb_errors &errors) {
  if (mtg_request.request == "lookup_deck") {
    if (mtg_request.arguments.empty())
      errors.add("Mtg_deck_request_handler::lookup", "1", "missing deck_id");
    Primary_database_config primary_database_config;
    mtg_inet_app_init.bdb_databases_config.select("deck", primary_database_config, errors);
    if (!errors.has()) {
      std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
          std::make_unique<Mtg_bdb_key_extractor>();
      Primary_database
          deck_db(primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home, errors);
      if (!errors.has()) {
        std::string deck_id = mtg_request.arguments.at(0);
        Deck_DTO deck_dto;
        Deck_DAO::lookup(deck_db.bdb_db, deck_id, deck_dto, errors);
        if (!errors.has()) {
          json_object *deck_json = deck_dto.to_json(errors);
          if (!errors.has()) {
            return deck_json;
          }
        }
      }
    }
    return true;
  } else
    return false;
}

bool Mtg_deck_request_handler::search_name(Mtg_inet_app_init &mtg_inet_app_init,
                                           const Mtg_request &mtg_request,
                                           Mtg_request_response &mtg_request_response,
                                           Bdb_errors &errors) {
  if (mtg_request.request != "deck_search_name")
    return false;
  if (mtg_request.arguments.size() < 3)
    errors.add("Mtg_request::process_search_deck_tripthongs_request",
               "1",
               "parameters: text, min-score, score-count");
  std::string text;
  int min_score;
  int score_count;
  if (!errors.has()) {
    text = mtg_request.arguments.at(0);
    min_score = Misc_utils::string_to_int(mtg_request.arguments.at(1), errors);
    score_count = Misc_utils::string_to_int(mtg_request.arguments.at(2), errors);
  }
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor = std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database_config deck_primary_database_config;
    mtg_inet_app_init.bdb_databases_config.select("deck", deck_primary_database_config, errors);
    if (!errors.has()) {
      Primary_database deck_db(deck_primary_database_config,
                               mtg_bdb_key_extractor.get(),
                               mtg_inet_app_init.db_home,
                               errors);
      Primary_database_config deck_name_tripthongs_primary_database_config;
      mtg_inet_app_init.bdb_databases_config.select("deck_name_tripthongs",
                                                    deck_name_tripthongs_primary_database_config,
                                                    errors);
      if (!errors.has()) {
        Primary_database deck_name_tripthongs_db(deck_name_tripthongs_primary_database_config,
                                                 mtg_bdb_key_extractor.get(),
                                                 mtg_inet_app_init.db_home,
                                                 errors);
        if (!errors.has()) {
          Timer timer;
          Bdb_text_tripthongs bdb_text_tripthongs(text);
          std::map<std::string, int> tripthong_counts;
          std::map<std::string, int> tripthong_match_counts;
          std::map<std::string, int> id_scores;
          for (const auto &text_tripthong_occurrence: bdb_text_tripthongs.list) {
            Bdb_text_id_occurrence_list bdb_text_id_occurrence_list;
            Bdb_DAO::select_tripthongs_by_key_list(deck_name_tripthongs_db.bdb_db,
                                                   text_tripthong_occurrence.tripthong,
                                                   bdb_text_id_occurrence_list,
                                                   errors);
            if (!errors.has()) {
              for (const auto &text_id_occurence: bdb_text_id_occurrence_list.list) {
                std::string id = text_id_occurence.id;
                tripthong_counts[id] = text_id_occurence.tripthongs_count;
                tripthong_match_counts[id] += std::min(text_id_occurence.occurrence_count,
                                                       text_tripthong_occurrence.occurrence_count);
              }
            } else
              break;
          }
          if (errors.has()) return true;
          // sort scores
          for (const auto &tripthong_count: tripthong_counts) {
            std::string id = tripthong_count.first;
            // match in integer percent
            id_scores[id] = (100 * tripthong_match_counts[id] * tripthong_match_counts[id]) /
                (bdb_text_tripthongs.tripthongs_count * tripthong_counts[id]);
          }
          std::map<int, std::string, std::greater<> > id_score_sort;
          for (const auto &id_score_entry: id_scores)
            id_score_sort[id_score_entry.second] = id_score_entry.first;
          Deck_DTO_list deck_dto_list;
          int deck_count{};
          for (const auto &id_score_sort_entry: id_score_sort) {
            int score = id_score_sort_entry.first;
            if (score > min_score && deck_count < score_count) {
              std::string deck_id = id_score_sort_entry.second;
              Deck_DTO deck_dto;
              Deck_DAO::lookup(deck_db.bdb_db, deck_id, deck_dto, errors);
              deck_dto.score = std::to_string(score);
              if (!errors.has()) {
                deck_dto_list.list.push_back(deck_dto);
              } else
                break;
            }
            ++deck_count;
          }
          timer.end();
          json_object *deck_dto_list_json = deck_dto_list.to_json(errors);
          if (!errors.has()) {
            return deck_dto_list_json;
          }
        }
      }
    }
  }
  return true;
}

bool Mtg_deck_request_handler::select_account_id(Mtg_inet_app_init &mtg_inet_app_init,
                                                 const Mtg_request &mtg_request,
                                                 Mtg_request_response &mtg_request_response,
                                                 Bdb_errors &errors) {
}

bool Mtg_deck_request_handler::select_all(Mtg_inet_app_init &mtg_inet_app_init,
                                          const Mtg_request &mtg_request,
                                          Mtg_request_response &mtg_request_response,
                                          Bdb_errors &errors) {
  if (mtg_request.request != "deck_select_all")
    return false;
  Primary_database_config primary_database_config;
  mtg_inet_app_init.bdb_databases_config.select("deck", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> mtg_bdb_key_extractor =
        std::make_unique<Mtg_bdb_key_extractor>();
    Primary_database
        deck_db(primary_database_config, mtg_bdb_key_extractor.get(), mtg_inet_app_init.db_home, errors);
    if (!errors.has()) {
      Deck_DTO_list deck_dto_list;
      Deck_DAO::select_all(deck_db.bdb_db, deck_dto_list, errors);
      if (!errors.has()) {
        json_object *deck_list_json = deck_dto_list.to_json(errors);
        if (!errors.has()) {
          return deck_list_json;
        }
      }
    }
  }
  return true;

}

bool Mtg_deck_request_handler::select_all_cards(Mtg_inet_app_init &mtg_inet_app_init,
                                                const Mtg_request &mtg_request,
                                                Mtg_request_response &mtg_request_response,
                                                Bdb_errors &errors) {
}

bool Mtg_deck_request_handler::select_cards_type_id(Mtg_inet_app_init &mtg_inet_app_init,
                                                    const Mtg_request &mtg_request,
                                                    Mtg_request_response &mtg_request_response,
                                                    Bdb_errors &errors) {
}

bool Mtg_deck_request_handler::select_other_cards(Mtg_inet_app_init &mtg_inet_app_init,
                                                  const Mtg_request &mtg_request,
                                                  Mtg_request_response &mtg_request_response,
                                                  Bdb_errors &errors) {
}


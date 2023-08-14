#include <json-c/json.h>
#include <sstream>
#include "bdb_databases.hpp"
#include "bdb_databases_config.hpp"
#include "bdb_common.hpp"
#include "bdb_dao.hpp"
#include "bdb_json_utils.hpp"
#include "imdb_bdb_keys.hpp"
#include "misc_utils.hpp"
#include "name_dao.hpp"
#include "name_dto.hpp"
#include "principals_dao.hpp"
#include "principals_dto.hpp"
#include "ratings_dao.hpp"
#include "ratings_dto.hpp"
#include "timer.hpp"
#include "title_dao.hpp"
#include "title_dto.hpp"
#include "imdb_unix_request.hpp"

Imdb_unix_request::Imdb_unix_request(json_object *jobj, Bdb_errors &errors) {
  from_json(jobj, errors);
}

/*
  {
    "class_name": "Imdb_unix_request",
    "request": "imdb_load",
    "arguments": [
      "../data/title.basics.tsv"
    ]
  }
 */
void Imdb_unix_request::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_name": ... `
  std::string jobj_class_name =
      Bdb_json_utils::get_json_string("Imdb_unix_request::from_json", "1", jobj, "class_name", errors);
  if (!errors.has() && jobj_class_name != class_name())
    errors.add("Imdb_unix_request::from_json", "1", "not class Imdb_unix_request");
  // parse: request": ...
  request = Bdb_json_utils::get_json_string("Imdb_unix_request::from_json", "3", jobj, "request", errors);
  if (!errors.has()) {
    // parse: ' { "arguments": ... `
    json_object *arguments_json =
        Bdb_json_utils::get_json_object("Imdb_unix_request::from_json", "2", jobj, "arguments", json_type_array,
                                        errors);
    if (!errors.has()) {
      size_t n_args = json_object_array_length(arguments_json);
      for (size_t i = 0; i < n_args && !errors.has(); i++) {
        json_object *argument_json = json_object_array_get_idx(arguments_json, i);
        std::string
            argument = Bdb_json_utils::get_json_string_type("Imdb_unix_request::from_json", "3", argument_json,
                                                            errors);
        if (!errors.has())
          arguments.push_back(argument);
      }
    }
  }
}

json_object *Imdb_unix_request::process_load_name_request(Imdb_unix_app_init &imdb_unix_app_init, Bdb_errors &errors) {
  if (arguments.empty())
    errors.add("Imdb_unix_request::process_load_name_request", "1", "missing imdb load name request arguments");
  Primary_database_config name_primary_database_config;
  imdb_unix_app_init.bdb_databases_config.select("name", name_primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
        std::make_unique<Imdb_bdb_key_extractor>();
    Primary_database
        name_db(name_primary_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home, errors);
    if (!errors.has()) {
      Primary_database_config name_tripthongs_primary_database_config;
      imdb_unix_app_init.bdb_databases_config.select("name_tripthongs",
                                                     name_tripthongs_primary_database_config,
                                                     errors);
      if (!errors.has()) {
        Primary_database name_tripthongs_db(name_tripthongs_primary_database_config,
                                            imdb_bdb_key_extractor.get(),
                                            imdb_unix_app_init.db_home,
                                            errors);
        if (!errors.has()) {
          std::string text_file = arguments.at(0);
          Timer timer;
          int count = Name_DAO::load(name_db.bdb_db, name_tripthongs_db.bdb_db, text_file, errors, tab);
          timer.end();
          if (!errors.has())
            return Imdb_request_response::to_load_response(count, timer, errors);
        }
      }
    }
  }
  return nullptr;
}

json_object *Imdb_unix_request::process_load_principals_request(Imdb_unix_app_init &imdb_unix_app_init,
                                                                Bdb_errors &errors) {
  if (arguments.empty())
    errors.add("Imdb_unix_request::process_load_principals_request", "1",
               "missing imdb load principals request arguments");
  Primary_database_config primary_database_config;
  imdb_unix_app_init.bdb_databases_config.select("principals", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
        std::make_unique<Imdb_bdb_key_extractor>();
    Primary_database
        principals_db(primary_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home, errors);
    if (!errors.has()) {
      std::string text_file = arguments.at(0);
      Timer timer;
      int count = Principals_DAO::load(principals_db.bdb_db, text_file, errors, tab);
      timer.end();
      if (!errors.has())
        return Imdb_request_response::to_load_response(count, timer, errors);
    }
  }
  return nullptr;
}

json_object *Imdb_unix_request::process_load_ratings_request(Imdb_unix_app_init &imdb_unix_app_init,
                                                             Bdb_errors &errors) {
  if (arguments.empty())
    errors.add("Imdb_unix_request::process_load_ratings_request", "1", "missing ratings_id");
  Primary_database_config primary_database_config;
  imdb_unix_app_init.bdb_databases_config.select("ratings", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
        std::make_unique<Imdb_bdb_key_extractor>();
    Primary_database ratings_db(primary_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home,
                                errors);
    if (!errors.has()) {
      std::string text_file = arguments.at(0);
      Timer timer;
      int count = Ratings_DAO::load(ratings_db.bdb_db, text_file, errors, tab);
      timer.end();
      if (!errors.has())
        return Imdb_request_response::to_load_response(count, timer, errors);
    }
  }
  return nullptr;
}

json_object *Imdb_unix_request::process_load_title_request(Imdb_unix_app_init &imdb_unix_app_init, Bdb_errors &errors) {
  if (arguments.empty())
    errors.add("Imdb_unix_request::process_load_title_request", "1", "missing title_id");
  Primary_database_config title_primary_database_config;
  imdb_unix_app_init.bdb_databases_config.select("title", title_primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
        std::make_unique<Imdb_bdb_key_extractor>();
    Primary_database
        title_db(title_primary_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home, errors);
    if (!errors.has()) {
      Primary_database_config title_tripthongs_primary_database_config;
      imdb_unix_app_init.bdb_databases_config.select("title_tripthongs",
                                                     title_tripthongs_primary_database_config,
                                                     errors);
      if (!errors.has()) {
        Primary_database title_tripthongs_db(title_tripthongs_primary_database_config,
                                             imdb_bdb_key_extractor.get(),
                                             imdb_unix_app_init.db_home,
                                             errors);
        if (!errors.has()) {
          std::string text_file = arguments.at(0);
          Timer timer;
          int count = Title_DAO::load(title_db.bdb_db, title_tripthongs_db.bdb_db, text_file, errors, tab);
          timer.end();
          if (!errors.has())
            return Imdb_request_response::to_load_response(count, timer, errors);
        }
      }
    }
  }
  return nullptr;
}

json_object *Imdb_unix_request::process_lookup_name_request(
    Imdb_unix_app_init &imdb_unix_app_init, Bdb_errors &errors) {
  if (arguments.empty())
    errors.add("Imdb_unix_request::process_lookup_name_request", "1", "missing name_id");
  Primary_database_config primary_database_config;
  imdb_unix_app_init.bdb_databases_config.select("name", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
        std::make_unique<Imdb_bdb_key_extractor>();
    Primary_database name_db(primary_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home, errors);
    if (!errors.has()) {
      std::string name_id = arguments.at(0);
      Name_DTO name_dto;
      Name_DAO::lookup(name_db.bdb_db, name_id, name_dto, errors);
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

json_object *Imdb_unix_request::process_lookup_ratings_request(
    Imdb_unix_app_init &imdb_unix_app_init, Bdb_errors &errors) {
  if (arguments.empty())
    errors.add("Imdb_unix_request::run", "2", "missing ratings_id");
  Primary_database_config primary_database_config;
  imdb_unix_app_init.bdb_databases_config.select("ratings", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
        std::make_unique<Imdb_bdb_key_extractor>();
    Primary_database ratings_db(primary_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home,
                                errors);
    if (!errors.has()) {
      std::string ratings_id = arguments.at(0);
      Ratings_DTO ratings_dto;
      Ratings_DAO::lookup(ratings_db.bdb_db, ratings_id, ratings_dto, errors);
      if (!errors.has()) {
        json_object *ratings_json = ratings_dto.to_json(errors);
        if (!errors.has()) {
          return ratings_json;
        }
      }
    }
  }
  return nullptr;
}

json_object *Imdb_unix_request::process_lookup_title_request(
    Imdb_unix_app_init &imdb_unix_app_init, Bdb_errors &errors) {
  if (arguments.empty())
    errors.add("Imdb_unix_request::process_lookup_title_request", "1", "missing title_id");
  std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
      std::make_unique<Imdb_bdb_key_extractor>();
  Primary_database_config primary_title_database_config;
  imdb_unix_app_init.bdb_databases_config.select("title", primary_title_database_config, errors);
  if (!errors.has()) {
    Primary_database_config primary_ratings_database_config;
    imdb_unix_app_init.bdb_databases_config.select("ratings", primary_ratings_database_config, errors);
    if (!errors.has()) {
      Primary_database
          title_db(primary_title_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home,
                   errors);
      Primary_database
          ratings_db(primary_ratings_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home,
                     errors);
      if (!errors.has()) {
        std::string title_id = arguments.at(0);
        Title_DTO title_dto;
        Title_DAO::lookup(title_db.bdb_db, ratings_db.bdb_db, title_id, title_dto, errors);
        if (!errors.has()) {
          json_object *title_json = title_dto.to_json(errors);
          if (!errors.has()) {
            return title_json;
          }
        }
      }
    }
  }
  return nullptr;
}

json_object *Imdb_unix_request::process_request(
    Imdb_unix_app_init &imdb_unix_app_init,
    json_object *request_json,
    Bdb_errors &errors) {
  Imdb_request_response imdb_request_response(errors);
  if (!errors.has() && request_json == nullptr) {
    if (imdb_unix_app_init.have_imdb_request_json_filename)
      request_json = json_object_from_file(imdb_unix_app_init.imdb_request_json_filename.c_str());
    else
      request_json = json_tokener_parse(imdb_unix_app_init.imdb_request_json_filename.c_str());
    if (request_json == nullptr)
      errors.add("Imdb_unix_request::process_request",
                 "1",
                 "invalid request json: " + imdb_unix_app_init.imdb_request_json_filename + "(error: " +
                     db_strerror(errno)
                     + ")");
  }
  std::string request_class_name;
  if (!errors.has()) {
    imdb_request_response.add_request(request_json);
    request_class_name =
        Bdb_json_utils::get_json_string("Imdb_unix_request::process_request", "2", request_json, "class_name",
                                        errors);
  }
  if (!errors.has()) {
    if (request_class_name == Imdb_unix_request::class_name()) {
      Imdb_unix_request imdb_unix_request(request_json, errors);
      if (!errors.has()) {
        json_object *request_response_json = imdb_unix_request.run(imdb_unix_app_init, errors);
        imdb_request_response.add_response(request_response_json);
      }
    } else if (request_class_name == Imdb_requests::class_name()) {
      imdb_request_response.add_array();
      Imdb_requests imdb_requests(request_json, errors);
      if (!errors.has())
        for (Imdb_unix_request &imdb_unix_request: imdb_requests.request_list) {
          json_object *request_response_json =
              imdb_unix_request.run(imdb_unix_app_init, errors);
          imdb_request_response.add_response_array(request_response_json);
        }
    } else
      errors.add("Imdb_unix_request::process_request", "3", "invalid request class: " + request_class_name);
  }
  if (errors.has()) {
    imdb_request_response.cleanup();
    return errors.to_json();
  } else
    return imdb_request_response.request_response_json;
}

json_object *Imdb_unix_request::process_search_name_request(
    Imdb_unix_app_init &imdb_unix_app_init, Bdb_errors &errors) {
  if (arguments.empty())
    errors.add("Imdb_unix_request::process_search_name_request", "1", "missing name_id");
  Primary_database_config principals_primary_database_config;
  imdb_unix_app_init.bdb_databases_config.select("principals", principals_primary_database_config, errors);
  Secondary_database_config principals_title_id_secondary_database_config;
  principals_primary_database_config.select("principals_title_id",
                                            principals_title_id_secondary_database_config,
                                            errors);
  Primary_database_config name_primary_database_config;
  imdb_unix_app_init.bdb_databases_config.select("name", name_primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
        std::make_unique<Imdb_bdb_key_extractor>();
    Primary_database name_db(name_primary_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home,
                             errors);
    Primary_database
        principals_db(principals_primary_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home,
                      errors);
    Secondary_database
        principals_title_id_sdb(principals_title_id_secondary_database_config, imdb_unix_app_init.db_home, errors);
    if (!errors.has()) {
      std::string title_id = arguments.at(0);
      Name_DTO_list name_dto_list;
      Principals_DAO::search_by_title(principals_title_id_sdb.bdb_db,
                                      principals_db.bdb_db,
                                      name_db.bdb_db,
                                      title_id,
                                      name_dto_list,
                                      errors);
      if (!errors.has()) {
        json_object *name_dto_list_json = name_dto_list.to_json(errors);
        if (!errors.has()) {
          return name_dto_list_json;
        }
      }
    }
  }
  return nullptr;
}

json_object *Imdb_unix_request::process_search_name_tripthongs_request(
    Imdb_unix_app_init &imdb_unix_app_init, Bdb_errors &errors) {
  if (arguments.size() < 3)
    errors.add("Imdb_unix_request::process_search_name_tripthongs_request",
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
    std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
        std::make_unique<Imdb_bdb_key_extractor>();
    Primary_database_config name_primary_database_config;
    imdb_unix_app_init.bdb_databases_config.select("name", name_primary_database_config, errors);
    if (!errors.has()) {
      Primary_database name_db(name_primary_database_config,
                               imdb_bdb_key_extractor.get(),
                               imdb_unix_app_init.db_home,
                               errors);
/*
      Primary_database_config ratings_primary_database_config;
      imdb_unix_app_init.bdb_databases_config.select("ratings", ratings_primary_database_config, errors);
      if (!errors.has()) {
        Primary_database ratings_db(ratings_primary_database_config,
                                    imdb_bdb_key_extractor.get(),
                                    imdb_unix_app_init.db_home,
                                    errors);
*/
      Primary_database_config name_tripthongs_primary_database_config;
      imdb_unix_app_init.bdb_databases_config.select("name_tripthongs",
                                                     name_tripthongs_primary_database_config,
                                                     errors);
      if (!errors.has()) {
        Primary_database name_tripthongs_db(name_tripthongs_primary_database_config,
                                            imdb_bdb_key_extractor.get(),
                                            imdb_unix_app_init.db_home,
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
            }
          }
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

json_object *Imdb_unix_request::process_search_title_request(
    Imdb_unix_app_init &imdb_unix_app_init, Bdb_errors &errors) {
  if (arguments.empty())
    errors.add("Imdb_unix_request::process_search_title_request", "1", "missing name_id");
  Primary_database_config principals_primary_database_config;
  imdb_unix_app_init.bdb_databases_config.select("principals", principals_primary_database_config, errors);
  Secondary_database_config principals_name_id_secondary_database_config;
  principals_primary_database_config.select("principals_name_id",
                                            principals_name_id_secondary_database_config,
                                            errors);
  Primary_database_config title_primary_database_config;
  imdb_unix_app_init.bdb_databases_config.select("title", title_primary_database_config, errors);
  Primary_database_config ratings_primary_database_config;
  imdb_unix_app_init.bdb_databases_config.select("ratings", ratings_primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
        std::make_unique<Imdb_bdb_key_extractor>();
    Primary_database
        title_db(title_primary_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home, errors);
    Primary_database
        ratings_db(ratings_primary_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home, errors);
    Primary_database
        principals_db(principals_primary_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home,
                      errors);
    Secondary_database
        principals_name_id_sdb(principals_name_id_secondary_database_config, imdb_unix_app_init.db_home, errors);
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

json_object *Imdb_unix_request::process_search_title_tripthongs_request(
    Imdb_unix_app_init &imdb_unix_app_init, Bdb_errors &errors) {
  if (arguments.size() < 3)
    errors.add("Imdb_unix_request::process_search_title_tripthongs_request",
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
    std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
        std::make_unique<Imdb_bdb_key_extractor>();
    Primary_database_config title_primary_database_config;
    imdb_unix_app_init.bdb_databases_config.select("title", title_primary_database_config, errors);
    if (!errors.has()) {
      Primary_database title_db(title_primary_database_config,
                                imdb_bdb_key_extractor.get(),
                                imdb_unix_app_init.db_home,
                                errors);
      Primary_database_config ratings_primary_database_config;
      imdb_unix_app_init.bdb_databases_config.select("ratings", ratings_primary_database_config, errors);
      if (!errors.has()) {
        Primary_database ratings_db(ratings_primary_database_config,
                                    imdb_bdb_key_extractor.get(),
                                    imdb_unix_app_init.db_home,
                                    errors);
        Primary_database_config title_tripthongs_primary_database_config;
        imdb_unix_app_init.bdb_databases_config.select("title_tripthongs",
                                                       title_tripthongs_primary_database_config,
                                                       errors);
        if (!errors.has()) {
          Primary_database title_tripthongs_db(title_tripthongs_primary_database_config,
                                               imdb_bdb_key_extractor.get(),
                                               imdb_unix_app_init.db_home,
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

json_object *Imdb_unix_request::process_select_name_request(Imdb_unix_app_init &imdb_unix_app_init,
                                                            Bdb_errors &errors) {
  return process_search_name_tripthongs_request(imdb_unix_app_init, errors);
/*

  Primary_database_config primary_database_config;
  imdb_unix_app_init.bdb_databases_config.select("name", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
        std::make_unique<Imdb_bdb_key_extractor>();
    Primary_database name_db(primary_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home, errors);
    if (!errors.has()) {
      Name_DTO_list name_dto_list;
      Name_DAO::select_all(name_db.bdb_db, name_dto_list, errors);
      if (!errors.has()) {
        json_object *name_dto_list_json = name_dto_list.to_json(errors);
        if (!errors.has()) {
          return name_dto_list_json;
        }
      }
    }
  }
  return nullptr;
*/
}

json_object *Imdb_unix_request::process_select_principals_request(Imdb_unix_app_init &imdb_unix_app_init,
                                                                  Bdb_errors &errors) {
  Primary_database_config primary_database_config;
  imdb_unix_app_init.bdb_databases_config.select("principals", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
        std::make_unique<Imdb_bdb_key_extractor>();
    Primary_database
        principals_db(primary_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home, errors);
    if (!errors.has()) {
      Principals_DTO_list principals_dto_list;
      Principals_DAO::select_all(principals_db.bdb_db, principals_dto_list, errors);
      if (!errors.has()) {
        json_object *principals_dto_list_json = principals_dto_list.to_json(errors);
        if (!errors.has()) {
          return principals_dto_list_json;
        }
      }
    }
  }
  return nullptr;
}

json_object *Imdb_unix_request::process_select_ratings_request(Imdb_unix_app_init &imdb_unix_app_init,
                                                               Bdb_errors &errors) {
  Primary_database_config primary_database_config;
  imdb_unix_app_init.bdb_databases_config.select("ratings", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
        std::make_unique<Imdb_bdb_key_extractor>();
    Primary_database ratings_db(primary_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home,
                                errors);
    if (!errors.has()) {
      Ratings_DTO_list ratings_dto_list;
      Ratings_DAO::select_all(ratings_db.bdb_db, ratings_dto_list, errors);
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

json_object *Imdb_unix_request::process_select_title_request(Imdb_unix_app_init &imdb_unix_app_init,
                                                             Bdb_errors &errors) {
  return process_search_title_tripthongs_request(imdb_unix_app_init, errors);
/*
  std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
      std::make_unique<Imdb_bdb_key_extractor>();
  Primary_database_config primary_title_database_config;
  imdb_unix_app_init.bdb_databases_config.select("title", primary_title_database_config, errors);
  if (!errors.has()) {
    Primary_database_config primary_ratings_database_config;
    imdb_unix_app_init.bdb_databases_config.select("ratings", primary_ratings_database_config, errors);
    if (!errors.has()) {
      Primary_database
          title_db(primary_title_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home, errors);
      Primary_database
          ratings_db(primary_ratings_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home,
                     errors);
      if (!errors.has()) {
        Title_DTO_list title_dto_list;
        Title_DAO::select_all(title_db.bdb_db, ratings_db.bdb_db, title_dto_list, errors);
        if (!errors.has()) {
          json_object *title_dto_list_json = title_dto_list.to_json(errors);
          if (!errors.has()) {
            return title_dto_list_json;
          }
        }
      }
    }
  }
  return nullptr;
*/
}

json_object *Imdb_unix_request::process_update_actor_request(Imdb_unix_app_init &imdb_unix_app_init,
                                                             Bdb_errors &errors) {
  if (arguments.size() < 3)
    errors.add("Imdb_unix_request::process_update_actor_request", "1", "missing name_id");
  Primary_database_config primary_database_config;
  imdb_unix_app_init.bdb_databases_config.select("name", primary_database_config, errors);
  if (!errors.has()) {
    std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
        std::make_unique<Imdb_bdb_key_extractor>();
    Primary_database name_db(primary_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home, errors);
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

json_object *Imdb_unix_request::process_update_title_request(Imdb_unix_app_init &imdb_unix_app_init,
                                                             Bdb_errors &errors) {
  if (arguments.size() < 3)
    errors.add("Imdb_unix_request::process_update_actor_request", "1", "missing title_id");
  std::unique_ptr<Bdb_key_extractor> imdb_bdb_key_extractor =
      std::make_unique<Imdb_bdb_key_extractor>();
  Primary_database_config primary_title_database_config;
  imdb_unix_app_init.bdb_databases_config.select("title", primary_title_database_config, errors);
  if (!errors.has()) {
    Primary_database_config primary_ratings_database_config;
    imdb_unix_app_init.bdb_databases_config.select("ratings", primary_ratings_database_config, errors);
    if (!errors.has()) {
      Primary_database
          title_db(primary_title_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home, errors);
      Primary_database
          ratings_db(primary_ratings_database_config, imdb_bdb_key_extractor.get(), imdb_unix_app_init.db_home,
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

json_object *Imdb_unix_request::run(Imdb_unix_app_init &imdb_unix_app_init,
                                    Bdb_errors &errors) {
  json_object *request_response = nullptr;
  if (request == "load_name") {
    request_response = process_load_name_request(imdb_unix_app_init, errors);
  } else if (request == "load_principals") {
    request_response = process_load_principals_request(imdb_unix_app_init, errors);
  } else if (request == "load_ratings") {
    request_response = process_load_ratings_request(imdb_unix_app_init, errors);
  } else if (request == "load_title") {
    request_response = process_load_title_request(imdb_unix_app_init, errors);
  } else if (request == "lookup_name") {
    request_response = process_lookup_name_request(imdb_unix_app_init, errors);
  } else if (request == "lookup_ratings") {
    request_response = process_lookup_ratings_request(imdb_unix_app_init, errors);
  } else if (request == "lookup_title") {
    request_response = process_lookup_title_request(imdb_unix_app_init, errors);
  } else if (request == "search_name") {
    request_response = process_search_name_request(imdb_unix_app_init, errors);
  } else if (request == "search_name_tripthongs") {
    request_response = process_search_name_tripthongs_request(imdb_unix_app_init, errors);
  } else if (request == "search_title") {
    request_response = process_search_title_request(imdb_unix_app_init, errors);
  } else if (request == "search_title_tripthongs") {
    request_response = process_search_title_tripthongs_request(imdb_unix_app_init, errors);
  } else if (request == "select_name") {
    request_response = process_select_name_request(imdb_unix_app_init, errors);
  } else if (request == "select_principals") {
    request_response = process_select_principals_request(imdb_unix_app_init, errors);
  } else if (request == "select_ratings") {
    request_response = process_select_ratings_request(imdb_unix_app_init, errors);
  } else if (request == "select_title") {
    request_response = process_select_title_request(imdb_unix_app_init, errors);
  } else if (request == "update_actor") {
    request_response = process_update_actor_request(imdb_unix_app_init, errors);
  } else if (request == "update_title") {
    request_response = process_update_title_request(imdb_unix_app_init, errors);
  } else
    errors.add("Imdb_unix_request::run", "1", "unrecognized request " + request);
  if (!errors.has())
    return request_response;
  else
    return errors.to_json();
}

json_object *Imdb_unix_request::to_json(Bdb_errors &errors) {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Imdb_unix_request::to_json", "1", "json-c allocate error");
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

std::string Imdb_unix_request::to_string() const {
  std::ostringstream os;
  os << "imdb_unix_request:" << std::endl;
  os << "request        " << request << std::endl;
  os << "arguments       " << std::endl;
  for (const auto &argument: arguments)
    os << "\t\t    " << argument << std::endl;
  return os.str();
}

// Imdb_requests methods


Imdb_requests::Imdb_requests(json_object *jobj, Bdb_errors &errors) {
  from_json(jobj, errors);
}

void Imdb_requests::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_name": ... `
  std::string jobj_class_name =
      Bdb_json_utils::get_json_string("Imdb_requests::from_json", "1", jobj, "class_name", errors);
  if (!errors.has()) {
    if (jobj_class_name != class_name())
      errors.add("Imdb_requests::from_json", "1", "not class Imdb_requests");
  }
  // parse: ' { "requests": ... `
  json_object *requests_json =
      Bdb_json_utils::get_json_object("Imdb_requests::from_json", "2", jobj, "requests", json_type_array, errors);
  if (!errors.has()) {
    size_t n_request = json_object_array_length(requests_json);
    for (size_t i = 0; i < n_request && !errors.has(); i++) {
      json_object *request_json = json_object_array_get_idx(requests_json, i);
//      Imdb_unix_request imdb_unix_request(request_json, errors);
      request_list.emplace_back(request_json, errors);
    }
  }
}

json_object *Imdb_requests::to_json(Bdb_errors &errors) {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Imdb_requests::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  if (!errors.has()) {
    json_object_object_add(root, "class_name",
                           json_object_new_string(Imdb_requests::class_name().c_str()));
    json_object *requests_json = json_object_new_array();
    json_object_object_add(root, "requests", requests_json);
    for (auto &imdb_unix_request: request_list) {
      json_object *request_json = imdb_unix_request.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(requests_json, request_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}

// Imdb_request_response methods


Imdb_request_response::Imdb_request_response(Bdb_errors &errors) {
  request_response_json = json_object_new_object();
  if (!request_response_json) {
    errors.add("Imdb_request_response::to_json", "1", "json-c allocate error");
  } else {
    json_object_object_add(request_response_json, "class_name", json_object_new_string(class_name().c_str()));
  }
}

void Imdb_request_response::add_array() {
  request_response_json_array = json_object_new_array();
  json_object_object_add(request_response_json, "imdb_request_responses", request_response_json_array);
}

void Imdb_request_response::add_request(json_object *request_json) const {
  json_object_object_add(request_response_json, "imdb_request", request_json);
}

void Imdb_request_response::add_response(json_object *response_json) const {
  json_object_object_add(request_response_json, "imdb_request_response", response_json);
}

void Imdb_request_response::add_response_array(json_object *response_json) const {
  json_object_array_add(request_response_json_array, response_json);
}

void Imdb_request_response::cleanup() const {
  json_object_put(request_response_json);
}

json_object *Imdb_request_response::to_load_response(int count, Timer &timer, Bdb_errors &errors) {
  json_object *load_response_json = json_object_new_object();
  if (!load_response_json) {
    errors.add("Imdb_unix_request::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(load_response_json, "class_name", json_object_new_string(class_name().c_str()));
  //json_object_object_add(load_response_json, "timer", timer.to_json(errors));
  json_object_object_add(load_response_json, "record_count", json_object_new_string(std::to_string(count).c_str()));
  return load_response_json;
}

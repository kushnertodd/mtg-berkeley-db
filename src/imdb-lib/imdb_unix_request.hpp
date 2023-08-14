#pragma once
#include <list>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases_config.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "imdb_unix_app_init.hpp"
#include "timer.hpp"

/*
 * class Imdb_request
 *
 *  {
 *    "class_name": "Imdb_request",
 *    "request": "title-lookup",
 *    "arguments": [
 *      "nm0000026"
 *    ]
 *  },
 *  {
 *    "class_name": "Imdb_request",
 *    "request": "name-lookup",
 *    "arguments": [
 *      "tt0060522"
 *    ]
 *  }
 */


class Imdb_unix_request {
 public:
  static std::string class_name() { return "Imdb_request"; }
  std::string request{};
  std::vector<std::string> arguments{};

  Imdb_unix_request() = default;
  Imdb_unix_request(json_object *jobj, Bdb_errors &errors);
  void from_json(json_object *jobj, Bdb_errors &errors);
  json_object *process_load_name_request(Imdb_unix_app_init &imdb_unix_app_init,
                                         Bdb_errors &errors);
  json_object *process_load_principals_request(Imdb_unix_app_init &imdb_unix_app_init,
                                               Bdb_errors &errors);
  json_object *process_load_ratings_request(Imdb_unix_app_init &imdb_unix_app_init,
                                            Bdb_errors &errors);
  json_object *process_load_title_request(Imdb_unix_app_init &imdb_unix_app_init,
                                          Bdb_errors &errors);
  json_object *process_lookup_name_request(Imdb_unix_app_init &imdb_unix_app_init,
                                           Bdb_errors &errors);
  json_object *process_lookup_ratings_request(Imdb_unix_app_init &imdb_unix_app_init,
                                              Bdb_errors &errors);
  json_object *process_lookup_title_request(Imdb_unix_app_init &imdb_unix_app_init,
                                            Bdb_errors &errors);
  static json_object *process_request(
      Imdb_unix_app_init &imdb_unix_app_init,
      json_object *request_json,
      Bdb_errors &errors);
  json_object *process_search_name_request(Imdb_unix_app_init &imdb_unix_app_init,
                                           Bdb_errors &errors);
  json_object *process_search_name_tripthongs_request(Imdb_unix_app_init &imdb_unix_app_init, Bdb_errors &errors);
  json_object *process_search_title_request(Imdb_unix_app_init &imdb_unix_app_init,
                                            Bdb_errors &errors);
  json_object *process_search_title_tripthongs_request(Imdb_unix_app_init &imdb_unix_app_init, Bdb_errors &errors);
  /* static */ json_object *process_select_name_request(Imdb_unix_app_init &imdb_unix_app_init,
                                                        Bdb_errors &errors);
  /* static */ json_object *process_select_principals_request(Imdb_unix_app_init &imdb_unix_app_init,
                                                              Bdb_errors &errors);
  /* static */ json_object *process_select_ratings_request(Imdb_unix_app_init &imdb_unix_app_init,
                                                           Bdb_errors &errors);
  /* static */ json_object *process_select_title_request(Imdb_unix_app_init &imdb_unix_app_init,
                                                         Bdb_errors &errors);
  json_object *run(Imdb_unix_app_init &imdb_unix_app_init, Bdb_errors &errors);
  json_object *to_json(Bdb_errors &errors);
  [[nodiscard]] std::string to_string() const;
  json_object *process_update_actor_request(Imdb_unix_app_init &imdb_unix_app_init,
                                            Bdb_errors &errors);
  json_object *process_update_title_request(Imdb_unix_app_init &imdb_unix_app_init,
                                            Bdb_errors &errors);
};

/*
 * class Imdb_requests
 *
 * {
 *   "class_name": "Imdb_requests",
 *   "requests": [
 *         <imdb_request>,
 *         ...
 *    ]
 * }
 */

class Imdb_requests {
 public:
  static std::string class_name() { return "Imdb_requests"; }
  std::list<Imdb_unix_request> request_list;
  Imdb_requests() = default;
  Imdb_requests(json_object *jobj, Bdb_errors &errors);
  void from_json(json_object *jobj, Bdb_errors &errors);
  json_object *to_json(Bdb_errors &errors);
};

class Imdb_request_response {
 public:
  static std::string class_name() { return "Imdb_request_response"; }
  json_object *request_response_json{};
  json_object *request_response_json_array{};

  Imdb_request_response() = default;
  explicit Imdb_request_response(Bdb_errors &errors);

  void add_array();
  void add_request(json_object *request_json) const;
  void add_response(json_object *response_json) const;
  void add_response_array(json_object *response_json) const;
  void cleanup() const;
  static json_object *to_load_response(int count, Timer &timer, Bdb_errors &errors);
};

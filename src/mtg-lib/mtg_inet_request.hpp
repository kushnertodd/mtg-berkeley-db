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

/*
 * class Mtg_request
 *
 *  {
 *    "class_name": "Mtg_request",
 *    "request": "title-lookup",
 *    "arguments": [
 *      "nm0000026"
 *    ]
 *  },
 *  {
 *    "class_name": "Mtg_request",
 *    "request": "name-lookup",
 *    "arguments": [
 *      "tt0060522"
 *    ]
 *  }
 */


class Mtg_inet_request {
 public:
  static std::string class_name() { return "Mtg_request"; }
  std::string request{};
  std::vector<std::string> arguments{};

  Mtg_inet_request() = default;
  Mtg_inet_request(json_object *jobj, Bdb_errors &errors);
  void from_json(json_object *jobj, Bdb_errors &errors);
  json_object *process_load_name_request(Mtg_inet_app_init &Mtg_inet_app_init,
                                         Bdb_errors &errors);
  json_object *process_load_principals_request(Mtg_inet_app_init &Mtg_inet_app_init,
                                               Bdb_errors &errors);
  json_object *process_load_ratings_request(Mtg_inet_app_init &Mtg_inet_app_init,
                                            Bdb_errors &errors);
  json_object *process_load_title_request(Mtg_inet_app_init &Mtg_inet_app_init,
                                          Bdb_errors &errors);
  json_object *process_lookup_name_request(Mtg_inet_app_init &Mtg_inet_app_init,
                                           Bdb_errors &errors);
  json_object *process_lookup_ratings_request(Mtg_inet_app_init &Mtg_inet_app_init,
                                              Bdb_errors &errors);
  json_object *process_lookup_title_request(Mtg_inet_app_init &Mtg_inet_app_init,
                                            Bdb_errors &errors);
  static json_object *process_request(
      Mtg_inet_app_init &Mtg_inet_app_init,
      const std::string &request,
      Bdb_errors &errors);
  json_object *process_search_name_request(Mtg_inet_app_init &Mtg_inet_app_init,
                                           Bdb_errors &errors);
  json_object *process_search_name_tripthongs_request(Mtg_inet_app_init &Mtg_inet_app_init, Bdb_errors &errors);
  json_object *process_search_title_request(Mtg_inet_app_init &Mtg_inet_app_init,
                                            Bdb_errors &errors);
  json_object *process_search_title_tripthongs_request(Mtg_inet_app_init &Mtg_inet_app_init, Bdb_errors &errors);
  /* static */ json_object *process_select_name_request(Mtg_inet_app_init &Mtg_inet_app_init,
                                                        Bdb_errors &errors);
  /* static */ json_object *process_select_principals_request(Mtg_inet_app_init &Mtg_inet_app_init,
                                                              Bdb_errors &errors);
  /* static */ json_object *process_select_ratings_request(Mtg_inet_app_init &Mtg_inet_app_init,
                                                           Bdb_errors &errors);
  /* static */ json_object *process_select_title_request(Mtg_inet_app_init &Mtg_inet_app_init,
                                                         Bdb_errors &errors);
  json_object *run(Mtg_inet_app_init &Mtg_inet_app_init, Bdb_errors &errors);
  json_object *to_json(Bdb_errors &errors);
  [[nodiscard]] std::string to_string() const;
  json_object *process_update_name_request(Mtg_inet_app_init &Mtg_inet_app_init,
                                           Bdb_errors &errors);
  json_object *process_update_title_request(Mtg_inet_app_init &Mtg_inet_app_init,
                                            Bdb_errors &errors);
};

/*
 * class Mtg_requests
 *
 * {
 *   "class_name": "Mtg_requests",
 *   "requests": [
 *         <mtg_request>,
 *         ...
 *    ]
 * }
 */

class Mtg_requests {
 public:
  static std::string class_name() { return "Mtg_requests"; }
  std::list<Mtg_inet_request> request_list;
  Mtg_requests() = default;
  Mtg_requests(json_object *jobj, Bdb_errors &errors);
  void from_json(json_object *jobj, Bdb_errors &errors);
  json_object *to_json(Bdb_errors &errors);
};

class Mtg_request_response {
 public:
  static std::string class_name() { return "Mtg_request_response"; }
  json_object *request_response_json{};
  json_object *request_response_json_array{};

  Mtg_request_response() = default;
  explicit Mtg_request_response(Bdb_errors &errors);

  void add_array();
  void add_request(json_object *request_json) const;
  void add_response(json_object *response_json) const;
  void add_response_array(json_object *response_json) const;
  void cleanup() const;
  static json_object *to_load_response(int count, Timer &timer, Bdb_errors &errors);
};

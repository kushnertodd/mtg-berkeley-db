#pragma once
#include <list>
#include <vector>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_databases_config.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "mtg_inet_app_init.hpp"

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


class Mtg_request {
 public:
  static std::string class_name() { return "Mtg_request"; }
  std::string request{};
  json_object *request_json{};
  std::vector<std::string> arguments{};

  Mtg_request() = default;
  Mtg_request(const std::string &request, Bdb_errors &errors);
  void from_json(json_object *jobj, Bdb_errors &errors);
  json_object *to_json(Bdb_errors &errors);
  [[nodiscard]] std::string to_string() const;
};

class Mtg_request_response {
 public:
  static std::string class_name() { return "Mtg_request_response"; }
  json_object *jobj{};
  json_object *jobj_array{};

  Mtg_request_response() = default;
  explicit Mtg_request_response(Bdb_errors &errors);

  void add_array();
  void add_request(json_object *request_json) const;
  void add_response(json_object *response_json) const;
  void add_response_array(json_object *response_json) const;
  void cleanup() const;
  static json_object *to_load_response(int count, Bdb_errors &errors);
  [[nodiscard]] std::string to_string() const;
};


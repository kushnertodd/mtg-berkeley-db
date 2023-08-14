#pragma once

#include <json-c/json.h>
#include <filesystem>
#include <map>
#include "bdb_errors.hpp"

/*!
 * @class Secondary_database_config json format secondary database configuration
 *
 *         {
 *           "class_name": "Secondary_database_config",
 *           "name": "name_id",
 *           "database-file": <database-file>,
 *           "key_extractor": "get_principals_name_id"
 *         }
 */
class Secondary_database_config {
 public:
  static std::string class_name() { return "Secondary_database_config"; }
  std::string name{};
  std::string filename{};
  std::string key_extractor{};

  Secondary_database_config() = default;
  Secondary_database_config(const Secondary_database_config &other) = default;
  Secondary_database_config(Secondary_database_config &&other) noexcept;
  Secondary_database_config(json_object *jobj, Bdb_errors &errors);

  Secondary_database_config &operator=(const Secondary_database_config &other);
  Secondary_database_config &operator=(Secondary_database_config &&other) noexcept;

  void from_json(json_object *jobj, Bdb_errors &errors);
  [[nodiscard]] std::string get_filename(const std::string &db_home) const;
  json_object *to_json(Bdb_errors &errors) const;
};

/*!
 * @class Primary_database_config json format primary database configuration
 *
 *     {
 *       "class_name": "Primary_database_config",
 *       "name": "principals-1",
 *       "database-file": <database-file>,
 *       "secondary_databases": [
 *         <secondary-database>,
 *         ...
 *       ]
 *     }
 *
 */

using Secondary_database_config_map = std::map<std::string, Secondary_database_config>;
using Secondary_database_config_map_iter = Secondary_database_config_map::iterator;
class Primary_database_config {
 public:
  static std::string class_name() { return "Primary_database_config"; }
  std::string name{};
  std::string filename{};
  Secondary_database_config_map secondary_database_config_map;
  bool has_duplicates{};

  Primary_database_config() = default;
  Primary_database_config(const Primary_database_config &other) = default;
  Primary_database_config(Primary_database_config &&other) noexcept;
  Primary_database_config(json_object *jobj, Bdb_errors &errors);

  Primary_database_config &operator=(const Primary_database_config &other) = default;
  Primary_database_config &operator=(Primary_database_config &&other) noexcept;

  void add(const Secondary_database_config &secondary_database_config);
  void from_json(json_object *jobj, Bdb_errors &errors);
  [[nodiscard]] std::string get_filename(const std::string &db_home) const;
  void select(const std::string &db_name, Secondary_database_config &secondary_database_config, Bdb_errors &errors);
  json_object *to_json(Bdb_errors &errors);
};

/*!
 * @class Bdb_databases_config json format database configuration
 *
 *   {
 *     "class_name": "Bdb_databases_config",
 *     "primary_databases": [
 *         <primary-database>,
 *         ...
 *       ]
 *    }
 *
 */

using Primary_database_config_map = std::map<std::string, Primary_database_config>;
using Primary_database_config_map_iter = Primary_database_config_map::iterator;
class Bdb_databases_config {
 public:
  static std::string class_name() { return "Bdb_databases_config"; }

  Primary_database_config_map primary_database_config_map;

  Bdb_databases_config() = default;
  Bdb_databases_config(json_object *jobj, Bdb_errors &errors);

  Bdb_databases_config(const Bdb_databases_config &other) = default;
  Bdb_databases_config &operator=(const Bdb_databases_config &other) = default;
  Bdb_databases_config(Bdb_databases_config &&other) = default;
  Bdb_databases_config &operator=(Bdb_databases_config &&other) = default;

  void from_json(json_object *jobj, Bdb_errors &errors);
  void select(const std::string &db_name, Primary_database_config &primary_database_config, Bdb_errors &errors);
  json_object *to_json(Bdb_errors &errors);
};


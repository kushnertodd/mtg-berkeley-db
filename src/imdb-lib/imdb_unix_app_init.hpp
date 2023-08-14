#pragma once

#include "bdb_databases_config.hpp"
#include "bdb_errors.hpp"

class Imdb_unix_app_init {
 public:
  static std::string app_name;

  bool load_request{};
  int cache_bytes{0};
  int cache_gbytes{4};
  std::string db_config_filename{};
  std::string db_home{"."};
  std::string imdb_request_json_filename{};

  bool have_db_config_filename{false};
  bool have_db_home{false};
  bool have_imdb_request_json_filename{false};

  Bdb_databases_config bdb_databases_config;

  Imdb_unix_app_init(int argc,
                     const char **argv,
                     bool load_request,
                     Bdb_errors &errors);

  void init(Bdb_errors &errors);
  static void usage(const std::string &message = "");
};


#include <json-c/json.h>
#include "bdb_errors.hpp"
#include "bdb_file_io.hpp"
#include "misc_utils.hpp"
#include "imdb_unix_app_init.hpp"

std::string Imdb_unix_app_init::app_name{"bdb_lookup"};

Imdb_unix_app_init::Imdb_unix_app_init(int argc,
                                       const char **argv,
                                       bool load_request,
                                       Bdb_errors &errors) {
  // https://man7.org/linux/man-pages/man3/getopt.3.html
  int ch;
  while ((ch = getopt(argc, (char **) argv, ":b:d:f:g:h:")) != EOF)
    switch (ch) {
      case 'b':
        cache_bytes = Misc_utils::string_to_int(optarg, errors);
        break;
      case 'd':
        if (optarg != nullptr) {
          db_config_filename = optarg;
          have_db_config_filename = true;
        }
        break;
      case 'f':
        if (optarg != nullptr) {
          imdb_request_json_filename = optarg;
          have_imdb_request_json_filename = true;
        }
        break;
      case 'g':
        cache_gbytes = Misc_utils::string_to_int(optarg, errors);
        break;
      case 'h':
        if (optarg != nullptr) {
          db_home = optarg;
          have_db_home = true;
        }
        break;
      case ':':
        errors.add("Imdb_unix_app_init::Imdb_unix_app_init",
                   "1",
                   "Option -" + Misc_utils::char_to_string((char) ch) + " requires an argument");
        break;
      case '?':
        errors.add("Imdb_unix_app_init::Imdb_unix_app_init",
                   "1",
                   "Invalid option -" + Misc_utils::char_to_string((char) ch));
        break;
      default:
        errors.add("Imdb_unix_app_init::init", "1", "Invalid option '-" + Misc_utils::char_to_string((char) ch));
    }
  if (!have_db_config_filename)
    errors.add("Imdb_unix_app_init::init", "2", "-d required");
  if (!have_db_home)
    errors.add("Imdb_unix_app_init::init", "4", "-h required");
  if (!errors.has())
    init(errors);
  else
    //usage(errors.to_string());
    usage("");
}

void Imdb_unix_app_init::init(Bdb_errors &errors) {
  std::string directory = Bdb_file_IO::get_current_directory();
  json_object *bdb_json = json_object_from_file(db_config_filename.c_str());
  if (bdb_json != nullptr)
    bdb_databases_config.from_json(bdb_json, errors);
  else
    errors.add("Imdb_unix_app_init::init",
               "1",
               "invalid json file " + db_config_filename + "(error: " + db_strerror(errno) + ")");
}

void Imdb_unix_app_init::usage(const std::string &message) {
  std::cout << (message.empty() ? app_name + " usage: " : message) << std::endl
            << " -d <request db config json file>" << std::endl
            << " -h <database home directory>" << std::endl
            << " -r <etf json request file>" << std::endl
            << std::endl;
}

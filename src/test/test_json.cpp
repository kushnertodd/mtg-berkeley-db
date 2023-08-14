#include <cstdlib>
#include <cstring>
#include <db_cxx.h>
#include <json-c/json.h>
#include "bdb_json_utils.hpp"
#ifdef _WIN32
extern "C" {
    extern int getopt(int, char* const*, const char*);
    extern char* optarg;
    extern int optopt;
}
#include <direct.h>
#define get_current_dir _getcwd
#else
#include <unistd.h>
#define get_current_dir getcwd
#endif
#define COMPQUIET(n, v) do {     \
    (n) = (v);                  \
    (n) = (n);                  \
} while (0)
#include <iostream>
#include "bdb_db.hpp"
#include "bdb_databases_config.hpp"
#include "bdb_databases.hpp"
#include "bdb_errors.hpp"
#include "bdb_file_io.hpp"
#include "imdb_request.hpp"
#include "name.hpp"
#include "principals.hpp"
#include "title.hpp"
#include "bdb_tokens.hpp"

json_object *print_json(std::string filename) {
  Bdb_errors errors;
  json_object *root = json_object_from_file(filename.c_str());
  if (root == nullptr)
    errors.add("main", "1", "invalid json: " + filename);
  else {
    std::string jobj_class_name = Json_utils::get_json_string("Names::from_json", "1", root, "class_name", errors);
    if (!errors.has()) {
      json_object *jobj = nullptr;

      if (jobj_class_name == Bdb_database_file_config::class_name()) {
        Bdb_database_file_config *json = Bdb_database_file_config::from_json(root, errors);
        jobj = json->to_json(errors);
      } else if (jobj_class_name == Bdb_databases_config::class_name()) {
        Bdb_databases_config *json = Bdb_databases_config::from_json(root, errors);
        jobj = json->to_json(errors);
      } else if (jobj_class_name == Bdb_tsv_file_config::class_name()) {
        Bdb_tsv_file_config *json = Bdb_tsv_file_config::from_json(root, errors);
        jobj = json->to_json(errors);
      } else if (jobj_class_name == Imdb_request::class_name()) {
        json_object *bdb = json_object_from_file("json/bdb_databases-sample.json");
        Bdb_databases_config *json = Bdb_databases_config::from_json(bdb, errors);
        Bdb_databases *bdb_databases = nullptr;
        Imdb_request *imdb_request = nullptr;
        if (!errors.has()) {
          bdb_databases = new Bdb_databases(json);
          imdb_request = Imdb_request::from_json(root, errors);
        }
        if (!errors.has()) {
          std::string json_string = imdb_request->run(bdb_databases, errors);
          std::cout << json_string << std::endl;
        }
      } else if (jobj_class_name == Imdb_requests::class_name()) {
        json_object *bdb = json_object_from_file("json/bdb_databases-sample.json");
        Bdb_databases_config *json = Bdb_databases_config::from_json(bdb, errors);
        Bdb_databases *bdb_databases = nullptr;
        Imdb_requests *imdb_requests = nullptr;
        if (!errors.has()) {
          bdb_databases = new Bdb_databases(json);
          imdb_requests = Imdb_requests::from_json(root, errors);
        }
        if (!errors.has()) {
          for (Imdb_request *imdb_request: imdb_requests->requests) {
            std::string json_string = imdb_request->run(bdb_databases, errors);
            std::cout << json_string << std::endl;
          }
        }
      } else if (jobj_class_name == Name::class_name()) {
        Name *json = Name::from_json(root, errors);
        jobj = json->to_json(errors);
      } else if (jobj_class_name == Names::class_name()) {
        Names *json = Names::from_json(root, errors);
        jobj = json->to_json(errors);
      } else if (jobj_class_name == Primary_database_config::class_name()) {
        Primary_database_config *json = Primary_database_config::from_json(root, errors);
        jobj = json->to_json(errors);
      } else if (jobj_class_name == Principals::class_name()) {
        Principals *json = Principals::from_json(root, errors);
        jobj = json->to_json(errors);
      } else if (jobj_class_name == Secondary_database_config::class_name()) {
        Secondary_database_config *json = Secondary_database_config::from_json(root, errors);
        jobj = json->to_json(errors);
      } else if (jobj_class_name == Titles::class_name()) {
        Titles *json = Titles::from_json(root, errors);
        jobj = json->to_json(errors);
      } else if (jobj_class_name == Title::class_name()) {
        Title *json = Title::from_json(root, errors);
        jobj = json->to_json(errors);
      }
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    std::cout << errors.to_string();
    json_object_put(root);
    return nullptr;
  }
}

int usage(std::string message) {
  std::cout << message << std::endl;
  std::cout << "bdb_database_load"
            << " [-h <database home directory> (default: ./)]"
            << " [-f <json file>]"
            << std::endl;
  return (-1);
}

// Loads the contents of vendors.txt and inventory.txt into
// Berkeley DB databases.
int main(int argc, char *argv[]) {

  //std::string current_directory = Bdb_common::get_current_directory();
  //std::cout << "current directory is '" << current_directory << "'" << std::endl;


  // Initialize the path to the database files
  std::string datafile_home("./");
  // Identify the full name for our input files, which should
  // also include some path information.

  // Parse the command line arguments
  bool have_json_filename = false;

  std::string json_filename;

  // https://man7.org/linux/man-pages/man3/getopt.3.html
  int ch;
  while ((ch = getopt(argc, argv, "f:h:")) != EOF)
    switch (ch) {
      case 'f': {
        have_json_filename = true;
        if (optarg != 0)
          json_filename = optarg;
        break;
      }
      case 'h': {
        datafile_home = optarg;
        char lastChar = datafile_home[datafile_home.size() - 1];
        if (lastChar != '/' && lastChar != '\\')
          return usage("-h option required trailing /");
        break;
      }
      case '?':
      default: {
        const char c = optopt;
        return usage("invalid option -" + std::string(1, c));
        break;
      }
    }

  if (!have_json_filename)
    usage("-f required");

  std::string json_file_path = datafile_home + json_filename;

  Bdb_errors errors;
  json_object *root = print_json(json_file_path);
  if (!errors.has()) {
    printf("%s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
    json_object_put(root);
  }
  return (0);
} // End main


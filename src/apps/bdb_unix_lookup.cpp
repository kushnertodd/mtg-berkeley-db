#include <json-c/json.h>
#include <iostream>
#include "bdb_common.hpp"
#include "bdb_errors.hpp"
#include "bdb_unix_socket_server.hpp"
#include "imdb_unix_app_init.hpp"
#include "imdb_unix_request.hpp"

int main(int argc, const char **argv) {
  Bdb_errors errors;
  Imdb_unix_app_init imdb_unix_app_init(argc, argv, true, errors);
  if (!errors.has()) {
    if (imdb_unix_app_init.have_imdb_request_json_filename) {
      json_object *request_response_json =
          Imdb_unix_request::process_request(imdb_unix_app_init, nullptr, errors);
      if (request_response_json != nullptr) {
        std::string json_string = json_object_to_json_string_ext(request_response_json,
                                                                 JSON_C_TO_STRING_PRETTY);
        std::cout << std::endl << json_string << std::endl;
      } else
        std::cout << std::endl << "no results" << std::endl;
    } else {
      Bdb_unix_socket_server bdb_socket_unix_server(errors);
      while (true) { // !errors.has()) {
        Bdb_unix_socket_server::Bdb_unix_socket_client bdb_unix_socket_client(bdb_socket_unix_server,
                                                                              errors);
        int buffer_len = 4096;
        char buffer[buffer_len];
        std::string json_string;
        if (!errors.has()) {
          if (bdb_unix_socket_client.read_socket(buffer, buffer_len, errors)) {
            std::cout << "test_server: received '" << buffer << "'" << std::endl;
            std::string request = buffer;
            if (request == Server_socket_stop_command)
              break;
            imdb_unix_app_init.imdb_request_json_filename = request;
            json_object *request_response_json =
                Imdb_unix_request::process_request(imdb_unix_app_init, nullptr, errors);
            if (request_response_json != nullptr) {
              json_string = json_object_to_json_string_ext(request_response_json,
                                                           JSON_C_TO_STRING_PRETTY);
              std::cout << std::endl << json_string << std::endl;
            } else
              std::cout << std::endl << "no results" << std::endl;
          }
          if (!errors.has()) {
            const std::string &server_message = json_string;//"test_server message";
            std::cout << "test_server: sending '" << server_message
                      << "'" << std::endl;
            bdb_unix_socket_client.write_socket(server_message.c_str(),
                                                server_message.length() + 1,
                                                errors);
          }
        }
      }
    }
  }
  if (errors.has())
    std::cout << errors.to_string() << std::endl;
  return 0;
} // End main


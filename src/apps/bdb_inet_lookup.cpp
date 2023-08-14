#include <json-c/json.h>
#include <iostream>
#include "bdb_common.hpp"
#include "bdb_errors.hpp"
#include "bdb_inet_socket_server.hpp"
#include "imdb_inet_app_init.hpp"
#include "imdb_inet_request.hpp"

int main(int argc, const char **argv) {
  Bdb_errors errors;
  Imdb_inet_app_init imdb_inet_app_init(argc, argv, errors);
  if (!errors.has()) {
    Bdb_inet_socket_server bdb_socket_server(imdb_inet_app_init.port, errors);
    while (true) { // !errors.has()) {
      errors.error_ct = 0;
      Bdb_inet_socket_server::Bdb_inet_socket_client bdb_socket_client(bdb_socket_server,
                                                                       errors);
      int buffer_len = 1 << 16;
      char buffer[buffer_len];
      std::memset(buffer, 0, buffer_len);
      std::string json_string;
      if (!errors.has()) {
        int bytes_read = bdb_socket_client.read_socket(buffer, buffer_len, errors);
        if (bytes_read) {
          std::cout << "test_server: received " << std::endl << buffer << std::endl;
          std::string request = Bdb_inet_socket_server::http_body(buffer, bytes_read);
          if (request == Server_socket_stop_command)
            break;
          json_object *request_response_json =
              Imdb_inet_request::process_request(imdb_inet_app_init, request, errors);
          if (request_response_json != nullptr) {
            json_string = json_object_to_json_string_ext(request_response_json,
                                                         JSON_C_TO_STRING_PRETTY);
            std::cout << std::endl << json_string << std::endl;
          } else
            std::cout << std::endl << "no results" << std::endl;
        }
        if (!errors.has()) {
          const std::string &server_message =
              Bdb_inet_socket_server::http_header(json_string);
          std::cout << "test_server: sending:" << std::endl << server_message << std::endl;
          bdb_socket_client.write_socket(server_message.c_str(),
                                         server_message.length() + 1,
                                         errors);
        }
      }
    }
  }
  if (errors.has())
    std::cout << errors.to_string() << std::endl;
  return 0;
} // End main


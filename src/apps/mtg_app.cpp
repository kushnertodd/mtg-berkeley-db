#include <iostream>
#include "bdb_common.hpp"
#include "bdb_errors.hpp"
#include "bdb_inet_socket_server.hpp"
#include "mtg_inet_app_init.hpp"
#include "mtg_request.hpp"
#include "mtg_request_handlers.hpp"

int main(int argc, const char **argv) {
  Bdb_errors errors;
  Mtg_inet_app_init mtg_inet_app_init(argc, argv, errors);
  if (!errors.has()) {
    Bdb_inet_socket_server bdb_socket_server(mtg_inet_app_init.port, errors);
    if (!errors.has())
      while (true) {
        errors.clear();
        Bdb_inet_socket_server::Bdb_inet_socket_client bdb_socket_client(bdb_socket_server,
                                                                         errors);
        if (!errors.has()) {
          int buffer_len = 1 << 16;
          char buffer[buffer_len];
          int bytes_read = bdb_socket_client.read_socket(buffer, buffer_len, errors);
          if (!errors.has() && bytes_read) {
            std::cout << "mtg_app: received " << std::endl << buffer << std::endl;
            std::string request = Bdb_inet_socket_server::http_body(buffer, bytes_read);
            if (request == Server_socket_stop_command)
              break;
            Mtg_request mtg_request(request, errors);
            Mtg_request_response mtg_request_response(errors);
            if (!errors.has())
              mtg_request_response.add_request(mtg_request.request_json);
            if (!errors.has())
              Mtg_request_handler::handle(mtg_inet_app_init, mtg_request, mtg_request_response, errors);
            std::string response_string;
            if (!errors.has()) {
              response_string = mtg_request_response.to_string();
              std::cout << std::endl << "results: " << response_string << std::endl;
            } else {
              response_string = errors.to_json_string();
              std::cout << std::endl << "no results: " << response_string << std::endl;
            }
            const std::string &server_message = Bdb_inet_socket_server::http_header(response_string);
            std::cout << "mtg_app: sending:" << std::endl << server_message << std::endl;
            bdb_socket_client.write_socket(server_message.c_str(),
                                           server_message.length() + 1,
                                           errors);
            mtg_request_response.cleanup();
          }
        }
        if (errors.has()) {
          std::cout << errors.to_string() << std::endl;
        }
      }
  }
  return 0;
} // End main


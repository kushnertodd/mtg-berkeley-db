#include <iostream>
#include "bdb_common.hpp"
#include "bdb_unix_socket_server.hpp"

int main(int argc, char **argv) {
  Bdb_errors errors;
  std::string stop_command = (argc > 1 ? argv[1] : Server_socket_stop_command);
  Bdb_unix_socket_server bdb_unix_socket_server(errors);
  while (!errors.has()) {
    Bdb_unix_socket_server::Bdb_unix_socket_client bdb_unix_socket_client(bdb_unix_socket_server,
                                                                          errors);
    int buffer_len = 4096;
    char buffer[buffer_len];
    if (bdb_unix_socket_client.read_socket(buffer, buffer_len, errors)) {
      std::cout << "test_server: received '" << buffer << "'" << std::endl;
      if (std::string(buffer) == stop_command)
        break;
    }
    if (!errors.has()) {
      std::string server_message = "test_server message";
      std::cout << "test_server: sending '" << server_message
                << "'" << std::endl;
      bdb_unix_socket_client.write_socket(server_message.c_str(),
                                          server_message.length() + 1,
                                          errors);
    }
    if (errors.has())
      std::cout << errors.to_string() << std::endl;
  }
}


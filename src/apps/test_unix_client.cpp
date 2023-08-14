#include <iostream>
#include "bdb_unix_socket_client.hpp"

int main(int argc, char**argv) {
    Bdb_errors errors;
    std::string server_message = (argc > 1 ? argv[1] : "test_client message");
    Bdb_unix_socket_client bdb_unix_socket_client(errors);
    if (!errors.has()) {
        std::cout << "test_client: sending '" << server_message << "'" << std::endl;
        bdb_unix_socket_client.write_socket(server_message.c_str(),
                                       server_message.length() + 1, errors);
        if (!errors.has()) {
            int buffer_len = 4096;
            char buffer[buffer_len];
            if (bdb_unix_socket_client.read_socket(buffer, buffer_len, errors))
                std::cout << "test_client: received '" << buffer << "'" << std::endl;
        }
    }
    if (errors.has())
        std::cout << errors.to_string() << std::endl;
}

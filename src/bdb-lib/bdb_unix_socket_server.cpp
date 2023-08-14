#include <cerrno>
#include <cstring>
#include <utility>
#include <iostream>
#include "bdb_errors.hpp"
#include "bdb_common.hpp"
#include "bdb_unix_socket_server.hpp"

/*!
 * @brief close server socket and delete unix domain socket file when destructed
 * @details invariant: opened = socket is open
 */
Bdb_unix_socket_server::~Bdb_unix_socket_server() {
  close_socket();
}

/*!
 * @brief construct and open server socket 
 * @param filename_ unix domain socket filename
 * @param errors if open errors
 * @details invariant: opened = socket is open
 */
Bdb_unix_socket_server::Bdb_unix_socket_server(Bdb_errors &errors) :
    Bdb_unix_socket_server(Server_socket_filename, errors) {
}

Bdb_unix_socket_server::Bdb_unix_socket_server(std::string filename_, Bdb_errors &errors) :
    filename(std::move(filename_)) {
  open_socket(errors);
}

/*!
 * @brief ensure server socket is closed and unix domain socket file is deleted
 * @details invariant: opened = socket is open
 */
void Bdb_unix_socket_server::close_socket() {
  // Close the server socket
  if (opened) {
    close(server_socket);
    opened = false;
    std::cout << "Bdb_socket_unix_server::close_socket: server socket " << filename
              << " closed (" << server_socket
              << ")" << std::endl;
  }
  // Remove the socket file
  std::remove(filename.c_str());
}

/*!
 * @brief open server socket
 * @param errors if open errors
 * @details invariant: opened = socket is open
 */
void Bdb_unix_socket_server::open_socket(Bdb_errors &errors) {
  // only called from constructor (once)
  opened = false;
  // Create a Unix domain socket
  server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (server_socket == -1)
    errors.add("Bdb_socket_unix_server::", "1",
               "Failed to create socket", errno);
  else {
    opened = true;
    std::cout << "Bdb_socket_unix_server::open_socket: server socket opened ("
              << server_socket << ")"
              << std::endl;
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    // Set up the server address
    server_sockaddr.sun_family = AF_UNIX;
    strncpy(server_sockaddr.sun_path, filename.c_str(), UNIX_PATH_MAX);
    // Bind the socket to the server address
    std::remove(filename.c_str());
    if (bind(server_socket, reinterpret_cast<sockaddr *>(&server_sockaddr),
             sizeof(server_sockaddr)) == -1)
      errors.add("Bdb_socket_unix_server::open_socket", "2",
                 "Failed to bind socket " + filename + " (" +
                     std::to_string(server_socket) + ")", errno);
  }
  if (!errors.has()) {
    // Listen for incoming connections
    if (listen(server_socket, backlog) == -1)
      errors.add("Bdb_socket_unix_server::open_socket", "3",
                 "Failed to listen for connections", errno);
  }
}

/*
 * @brief ensure server socket is closed
 * @details invariant: opened = socket is open
 */
Bdb_unix_socket_server::Bdb_unix_socket_client::~Bdb_unix_socket_client() {
  close_socket();
}

/*!
 * @brief construct and open client socket
 * @param bdb_socket_unix_server unix domain server socket
 * @param errors if open errors
 * @details invariant: opened = socket is open
 */
Bdb_unix_socket_server::Bdb_unix_socket_client::Bdb_unix_socket_client(Bdb_unix_socket_server &bdb_socket_unix_server,
                                                             Bdb_errors &errors) {
  open_socket(bdb_socket_unix_server, errors);
}

/*!
 * @brief ensure client socket is closed
 * @details invariant: opened = socket is open
 */
void Bdb_unix_socket_server::Bdb_unix_socket_client::close_socket() {
  // Close the server socket
  if (opened) {
    close(client_socket);
    opened = false;
    std::cout << "Bdb_socket_unix_server::Bdb_unix_socket_client::close_socket socket closed ("
              << client_socket << ")"
              << std::endl;
  }
}

/*!
 * @brief open socket client
 * @param errors if open errors
 * @details invariant: opened = socket is open
 */
void Bdb_unix_socket_server::Bdb_unix_socket_client::open_socket(Bdb_unix_socket_server &bdb_socket_unix_server,
                                                            Bdb_errors &errors) {
  // only called from constructor (once)
  opened = false;
  if (!bdb_socket_unix_server.is_open())
    errors.add("Bdb_socket_unix_server::Bdb_unix_socket_client::open_socket", "1",
               "Server socket not open");
  else {
    // Accept a client connection
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    socklen_t client_address_length = sizeof(client_sockaddr);
    client_socket = accept(bdb_socket_unix_server.get_server_socket(),
                           reinterpret_cast<sockaddr *>(&client_sockaddr),
                           &client_address_length);
    opened = true;
    std::cout << "Bdb_socket_unix_server::Bdb_unix_socket_client::open_socket client socket opened ("
              << client_socket << ")"
              << std::endl;
    if (client_socket == -1)
      errors.add("Bdb_socket_unix_server::Bdb_unix_socket_client::open_socket", "2",
                 "Failed to accept client connection",
                 errno);
    else if (getpeername(client_socket, (struct sockaddr *) &client_sockaddr,
                         &client_address_length) == -1)
      errors.add("Bdb_socket_unix_server::Bdb_unix_socket_client::open_socket", "3",
                 "Failed to get socket peername",
                 errno);
    else
      std::cout << "Bdb_socket_unix_server::Bdb_unix_socket_client::open_socket: client socket file is '"
                << client_sockaddr.sun_path << "'" << std::endl;
  }
}

bool Bdb_unix_socket_server::Bdb_unix_socket_client::read_socket(char *buffer, size_t buffer_len,
                                                            Bdb_errors &errors) const {
  if (!opened) {
    errors.add("Bdb_socket_unix_server::Bdb_unix_socket_client::read_socket", "1",
               "Client socket not open");
    return false;
  }
  // Read data from the client
  std::memset(buffer, 0, buffer_len);
  size_t bytes_read = recv(client_socket, buffer, buffer_len, 0);
  if (bytes_read == -1) {
    errors.add("Bdb_socket_unix_server::Bdb_unix_socket_client::read_socket", "2",
               "Failed to read from client socket (" +
                   std::to_string(client_socket) + ")", errno);
    return false;
  }
  if (bytes_read == 0) {
    // Client closed the connection
    return false;
  }
  std::cout << "Bdb_socket_unix_server::Bdb_unix_socket_client::read_socket: read '" << buffer
            << "'" << std::endl;
  return true;
}

void
Bdb_unix_socket_server::Bdb_unix_socket_client::write_socket(const char *buffer, size_t buffer_len,
                                                        Bdb_errors &errors) const {
  if (!opened)
    errors.add("Bdb_socket_unix_server::Bdb_unix_socket_client::write_socket", "1",
               "Client socket not open");
  if (!errors.has()) {
    std::cout << "Bdb_socket_unix_server::Bdb_unix_socket_client::write_socket: writing '"
              << buffer << "'" << std::endl;
    size_t bytes_sent = send(client_socket, buffer, buffer_len, 0);
    if (bytes_sent == -1)
      errors.add("Bdb_socket_unix_server::Bdb_unix_socket_client::write_socket", "2",
                 "Failed to send response to client",
                 errno);
    else if (bytes_sent != buffer_len)
      errors.add("Bdb_socket_unix_server::Bdb_unix_socket_client::write_socket", "3",
                 "bytes written (" + std::to_string(bytes_sent) +
                     " not equal to number requested ("
                     + std::to_string(buffer_len) + ")");
  }
}
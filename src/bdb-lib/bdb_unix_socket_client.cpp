#include <cerrno>
#include <utility>
#include <iostream>
#include "bdb_common.hpp"
#include "bdb_errors.hpp"
#include "bdb_unix_socket_client.hpp"

/*!
 * @brief close server socket and delete unix domain socket file when destructed
 * @details invariant: opened = socket is open
 */
Bdb_unix_socket_client::~Bdb_unix_socket_client() {
  close_socket();
}

Bdb_unix_socket_client::Bdb_unix_socket_client(Bdb_errors &errors) :
    Bdb_unix_socket_client(Server_socket_filename,
                           Client_socket_filename, errors) {
}

/*!
 * @brief construct and open server socket
 * @param server_filename_ unix domain server socket filename
 * @param client_filename_ unix domain client socket filename
 * @param errors if open errors
 * @details invariant: opened = socket is open
 */
Bdb_unix_socket_client::Bdb_unix_socket_client(std::string server_filename_,
                                               std::string client_filename_,
                                               Bdb_errors &errors) :
    server_filename(std::move(server_filename_)),
    client_filename(std::move(client_filename_)) {
  open_socket(errors);
}

/*!
 * @brief ensure server socket is closed and unix domain socket file is deleted
 * @details invariant: opened = socket is open
 */
void Bdb_unix_socket_client::close_socket() {
  // Close the server socket
  if (opened) {
    close(client_socket);
    opened = false;
    std::cout << "Bdb_unix_socket_client::close_socket socket closed ("
              << client_socket << ")" << std::endl;
  }
  // Remove the socket file
  std::remove(client_filename.c_str());
}

/*!
 * @brief open server socket
 * @param errors if open errors
 * @details invariant: opened = socket is open
 */
void Bdb_unix_socket_client::open_socket(Bdb_errors &errors) {
  // only called from constructor (once)
  opened = false;
  // Create a Unix domain socket
  client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  opened = true;
  if (client_socket == -1)
    errors.add("Bdb_unix_socket_client::open_socket", "1",
               "Failed to create socket", errno);
  if (!errors.has()) {
    std::cout << "Bdb_unix_socket_client::open_socket socket client opened ("
              << client_socket << ")" << std::endl;
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    // Set up the client address
    client_sockaddr.sun_family = AF_UNIX;
    strncpy(client_sockaddr.sun_path, client_filename.c_str(),
            UNIX_PATH_MAX);
    // Bind the socket to the client address
    std::remove(client_filename.c_str());
    if (bind(client_socket,
             reinterpret_cast<sockaddr *>(&client_sockaddr),
             sizeof(client_sockaddr)) == -1)
      errors.add("Bdb_unix_socket_client::open_socket", "2",
                 "Failed to bind socket " + client_filename + " (" +
                     std::to_string(client_socket) + ")",
                 errno);

    // Set up the server address
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    server_sockaddr.sun_family = AF_UNIX;
    strncpy(server_sockaddr.sun_path, server_filename.c_str(),
            UNIX_PATH_MAX);
    // Bind the socket to the server address
    if (connect(client_socket,
                reinterpret_cast<sockaddr *>(&server_sockaddr),
                sizeof(server_sockaddr)) == -1)
      errors.add("Bdb_unix_socket_client::open_socket", "3",
                 "Failed to connect socket" + server_filename
                     + " (" + std::to_string(client_socket) + ")",
                 errno);
  }
}

bool Bdb_unix_socket_client::read_socket(char *buffer, size_t buffer_len,
                                         Bdb_errors &errors) const {
  if (!opened) {
    errors.add("Bdb_unix_socket_client::read_socket", "1",
               "Client socket not open");
    return false;
  }
  // Read data from the client
  std::memset(buffer, 0, buffer_len);
  size_t bytes_read = recv(client_socket, buffer, buffer_len, 0);
  if (bytes_read == -1) {
    errors.add("Bdb_unix_socket_client::read_socket", "2",
               "Failed to read from client", errno);
    return false;
  }
  if (bytes_read == 0) {
    // Client closed the connection
    return false;
  }
  std::cout << "Bdb_unix_socket_client::read_socket: read '"
            << buffer << "'" << std::endl;
  return true;
}

void Bdb_unix_socket_client::write_socket(const char *buffer, size_t buffer_len,
                                          Bdb_errors &errors) const {
  if (!opened)
    errors.add("Bdb_unix_socket_client::write_socket", "1",
               "Client socket not open");
  if (!errors.has()) {
    std::cout << "Bdb_unix_socket_client::write_socket: writing '" << buffer
              << "'" << std::endl;
    size_t bytes_sent = send(client_socket, buffer,
                             buffer_len, 0);
    if (bytes_sent == -1)
      errors.add("Bdb_unix_socket_client::write_socket", "2",
                 "Failed to send response to client",
                 errno);
    else if (bytes_sent != buffer_len)
      errors.add("Bdb_unix_socket_client::write_socket", "3",
                 "bytes written ("
                     + std::to_string(bytes_sent)
                     + " not equal to number requested ("
                     + std::to_string(buffer_len) + ")");
  }
}
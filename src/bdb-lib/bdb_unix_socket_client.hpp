#pragma once

#include <cstring>
#include <list>
#include <string>
#include <sys/socket.h>
#include <linux/un.h>
#include <unistd.h>
#include "bdb_errors.hpp"

/*!
 * @class Bdb socket server class
 * https://gist.github.com/alexandruc/2350954
 * https://www.ibm.com/docs/en/ztpf/1.1.0.15?topic=considerations-unix-domain-sockets
 */
class Bdb_unix_socket_client {
 private:
  std::string server_filename{};
  std::string client_filename{};
  bool opened{};
  int client_socket{};

  void close_socket();

  void open_socket(Bdb_errors &errors);

  struct sockaddr_un server_sockaddr{};
  struct sockaddr_un client_sockaddr{};
 public:
  ~Bdb_unix_socket_client();

  Bdb_unix_socket_client(std::string server_filename_, std::string client_filename_,
                         Bdb_errors &errors);

  explicit Bdb_unix_socket_client(Bdb_errors &errors);

  //[[nodiscard]] int get_server_socket() const { return server_socket; }
  [[nodiscard]] bool is_open() const { return opened; }

  bool read_socket(char *buffer, size_t buffer_len, Bdb_errors &errors) const;

  void write_socket(const char *buffer, size_t buffer_len, Bdb_errors &errors) const;
};


#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // Add this line

const int PORT = 8080;
const int MAX_CLIENTS = 5;
const int BUFFER_SIZE = 1024;

std::string http_body(char *buffer, int bytesRead) {
  std::string requestData(buffer, bytesRead);
  size_t headerEnd = requestData.find("\r\n\r\n");
  if (headerEnd == std::string::npos) {
    std::cerr << "Invalid request. Header not found.\n";
    return "";
  }

  // Strip off the header and extract the request body
  return requestData.substr(headerEnd + 4);
}

int main() {
  int serverSocket, clientSocket;
  struct sockaddr_in serverAddr, clientAddr;
  socklen_t addrLen = sizeof(clientAddr);
  char buffer[BUFFER_SIZE] = {0};

  // Create a socket
  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    std::cerr << "Error creating socket.\n";
    return 1;
  }

  // Set up the server address
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(PORT);

  // Bind the socket to the server address
  if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
    std::cerr << "Error binding socket.\n";
    close(serverSocket);
    return 1;
  }

  // Listen for incoming connections
  if (listen(serverSocket, MAX_CLIENTS) == -1) {
    std::cerr << "Error listening for connections.\n";
    close(serverSocket);
    return 1;
  }

  std::cout << "Server started. Listening for connections on port " << PORT << ".\n";

  while (true) {
    // Accept incoming connections
    clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &addrLen);
    if (clientSocket == -1) {
      std::cerr << "Error accepting connection.\n";
      continue;
    }

    // Print client information
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
    std::cout << "Connection established with client: " << clientIP << ":" << ntohs(clientAddr.sin_port) << "\n";

    // Receive and send data for this client
    while (true) {
      // Receive data from the client
      int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
      if (bytesRead == -1) {
        std::cerr << "Error receiving data from client.\n";
        break;
      }

      if (bytesRead == 0) {
        std::cout << "Client disconnected.\n";
        break;
      }

      //std::cout << "Received from client: " << buffer << std::endl;
      std::cout << "Received from client: " << std::endl;
      std::cout << "=================================" << std::endl;
      std::cout << "'" << buffer << "'" << std::endl;
      std::cout << "=================================" << std::endl;

      std::cout << "Request data: " << std::endl;
      std::cout << "=================================" << std::endl;
      std::cout << "'" << http_body(buffer, bytesRead) << "'" << std::endl;
      std::cout << "=================================" << std::endl;

      // Send data back to the client
      if (send(clientSocket, buffer, bytesRead, 0) == -1) {
        std::cerr << "Error sending data to client.\n";
        break;
      }

      // Clear the buffer for the next iteration
      memset(buffer, 0, BUFFER_SIZE);
    }

    // Close the connection for this client
    close(clientSocket);
  }

  // Close the server socket
  close(serverSocket);

  return 0;
}

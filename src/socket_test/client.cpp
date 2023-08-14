#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
This code creates a TCP socket client that connects to the server at the specified IP address (SERVER_IP) and port (PORT). It then enters a loop where it prompts the user to enter a message to send to the server. The client sends the message to the server and waits for a response. The loop continues until the user enters "exit" as the message, at which point the client disconnects and exits.
*/

const int PORT = 8080;
const int BUFFER_SIZE = 1024;
const char *SERVER_IP = "127.0.0.1"; // Change this to the server IP address

int main() {
  int clientSocket;
  struct sockaddr_in serverAddr;
  char buffer[BUFFER_SIZE] = {0};

  // Create a socket
  clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket == -1) {
    std::cerr << "Error creating socket.\n";
    return 1;
  }

  // Set up the server address
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
    std::cerr << "Invalid address/ Address not supported.\n";
    close(clientSocket);
    return 1;
  }

  // Connect to the server
  if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
    std::cerr << "Connection failed.\n";
    close(clientSocket);
    return 1;
  }

  std::cout << "Connected to the server.\n";

  // Send and receive data
  while (true) {
    // Send data to the server
    std::cout << "Enter a message: ";
    std::string message;
    std::getline(std::cin, message);
    if (message == "exit")
      break;

    if (send(clientSocket, message.c_str(), message.length(), 0) == -1) {
      std::cerr << "Error sending data.\n";
      close(clientSocket);
      return 1;
    }

    // Receive data from the server
    int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesRead == -1) {
      std::cerr << "Error receiving data.\n";
      close(clientSocket);
      return 1;
    }

    buffer[bytesRead] = '\0';
    std::cout << "Server response: " << buffer << std::endl;

    // Clear the buffer for the next iteration
    memset(buffer, 0, BUFFER_SIZE);
  }

  // Close the socket
  close(clientSocket);

  return 0;
}


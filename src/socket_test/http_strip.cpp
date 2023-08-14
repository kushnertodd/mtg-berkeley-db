#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

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
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket.\n";
        close(serverSocket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) == -1) {
        std::cerr << "Error listening for connections.\n";
        close(serverSocket);
        return 1;
    }

    std::cout << "Server started. Listening for connections on port " << PORT << ".\n";

    while (true) {
        // Accept incoming connections
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection.\n";
            continue;
        }

        // Read data from the client
        int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesRead == -1) {
            std::cerr << "Error receiving data from client.\n";
            close(clientSocket);
            continue;
        }

        // Find the end of the header
        std::string requestData(buffer, bytesRead);
        size_t headerEnd = requestData.find("\r\n\r\n");
        if (headerEnd == std::string::npos) {
            std::cerr << "Invalid request. Header not found.\n";
            close(clientSocket);
            continue;
        }

        // Strip off the header and extract the request body
        std::string requestBody = requestData.substr(headerEnd + 4);

        // Do whatever processing you need with the request body
        std::cout << "Received request body: " << requestBody << std::endl;

        // Respond to the client (you may send an appropriate response here)

        // Close the connection for this client
        close(clientSocket);
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}

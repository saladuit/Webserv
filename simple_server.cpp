#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void handleClient(int clientSocket) {
    const char* message = "Hello from my first simple server!\n";
    char buffer[1024] = {0};

    // send is used to transmit a message to another socket
    send(clientSocket, message, strlen(message), 0);

    // recv is used for receiving data from a connected socket
    recv(clientSocket, buffer, sizeof(buffer), 0);

    std::cout << "Received from client: " << buffer << std::endl;
    close(clientSocket);
}

int main() {
    // -- SOCKET DESCRIPTORS --
    int	serverSocket;
    int	clientSocket;

    // -- SOCKET DATA STRUCTURES -- for representing network addresses, specifically IPv4 addresses,
    // in the internet domain
    // you would use these structs to bind the server socket to a specific IP address and port,
    // and when accepting client connections, you would use the clientAddr struct to retrieve
    // the client's IP address and port information
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;

    socklen_t clientAddrLen = sizeof(clientAddr);

    // -- SOCKET FUNCTION -- creates a socket, which enables data transfer between processes over a network
    // AF_INET indicates that we are using the IPv4 address family
    // SOCK_STREAM indicates that we want to create a stream socket, which provides
    // reliable, two-way, connection-based communication
    // The last parameter represents the specific protocol to be used with the socket,
    // for TCP the protocol is usually set to 0; this is a way to let the operating system choose the
    // appropriate protocol automatically based on the address family and socket type
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    // .sin_family: this member specifies the address family
    serverAddr.sin_family = AF_INET;

    // .sin_port: this member hold the port number in network byte order
    // htons is used to convert the number
    serverAddr.sin_port = htons(8080); // Choose any available port


    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding to port\n";
        close(serverSocket);
        return 1;
    }

    // Start listening for connections
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening\n";
        close(serverSocket);
        return 1;
    }

    std::cout << "Server listening on port 8080...\n";

    // Accept and handle client connections
    while (true) {
//		if (std::cin.eof())
//			break ;
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection\n";
            continue;
        }
        handleClient(clientSocket);
    }

    close(serverSocket);
    return 0;
}

#include "NetBolt.h"
#include <iostream>

namespace netblt
{
    #ifdef NETBLT_PLATFORM_MACOS

    // MacOS implementation of CreateSocket
    Socket MacOS_CreateSocket(Port _port, SocketType _type, IP _ip)
    {
        // Create a socket
        int sockfd = socket(AF_INET, to_bsd_socket_type(_type), IPPROTO_TCP);
        if (sockfd < 0) {
            std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
            return -1; // Error creating socket
        }

        return sockfd; // Return the created socket file descriptor
    }

    // MacOS implementation of CloseSocket
    void MacOS_CloseSocket(int _socket)
    {
        if (_socket >= 0) {
            close(_socket);
        }
    }

    void MacOS_AcceptClient(Socket& _serverSocket, ClientSocket& _client)
    {
        // Wait for a client to connect
        if ((_client.socket = accept(_serverSocket, (struct sockaddr*)&_client.ip, &_client.len)) < 0) {
            std::cerr << "Error accepting client connection: " << strerror(errno) << std::endl;
            if (_serverSocket < 0) {
                std::cerr << "- Make sure the server socket is valid and listening." << std::endl;
            }
            return; // Error accepting client connection
        }
    }


    bool MacOS_Connect(Socket& _clientSocket, IP _serverIp, Port _port)
    {
        SAddrIn server_addr;
        memset(&server_addr, 0, sizeof(server_addr));  
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(_port);

        if (inet_pton(AF_INET, _serverIp, &server_addr.sin_addr) <= 0) {
            std::cerr << "Invalid server IP address: " << _serverIp << std::endl;
            return false;
        }

        if (connect(_clientSocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Error connecting to server: " << strerror(errno) << std::endl;
            return false;
        }

        return true;
    }

    void MacOS_BindSocket(Socket& _socket, Port _port)
    {
        SAddrIn srv_addr;
        memset(&srv_addr, 0, sizeof(srv_addr));
        srv_addr.sin_family = AF_INET;
        srv_addr.sin_port = htons(_port); // Use the provided port for binding
        srv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to any available address

        if (bind(_socket, (struct sockaddr*)&srv_addr, sizeof(srv_addr)) < 0) {
            std::cerr << "Error binding socket: " << strerror(errno) << std::endl;
            std::cerr << "- Make sure the port is not already in use." << std::endl;
            close(_socket);
            _socket = -1; // Set to -1 to indicate an error
        }
    }

    void MacOS_Listen(Socket& _socket)
    {
        if (listen(_socket, SOMAXCONN) < 0) {
            std::cerr << "Error listening on socket: " << strerror(errno) << std::endl;
            close(_socket);
            _socket = -1; // Set to -1 to indicate an error
        }
    }

    void MacOS_SendData(Socket& _socket, std::string _data)
    {
        ssize_t bytes_sent = send(_socket, _data.c_str(), _data.size(), 0);
        if (bytes_sent < 0) {
            std::cerr << "Error sending data: " << strerror(errno) << std::endl;
        }
    }

    void MacOS_ReceiveData(Socket& _socket, std::string _data)
    {
        char buffer[1024];
        ssize_t bytes_received = recv(_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
        } else if (bytes_received == 0) {
            std::cerr << "Connection closed by peer." << std::endl;
        } else {
            buffer[bytes_received] = '\0'; // Null-terminate the received data
            _data.assign(buffer, bytes_received);
        }
    }

    #endif // NETBLT_PLATFORM_MACOS
} // namespace netblt
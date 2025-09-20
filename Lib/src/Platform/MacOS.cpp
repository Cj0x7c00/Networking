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
            NB_ERROR("Error creating socket: %s", strerror(errno));
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
            NB_ERROR("Error accepting client connection: %s", strerror(errno));
            if (_serverSocket < 0) {
                NB_INFO("\t- Make sure the server socket is valid and listening");
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
            NB_ERROR("Invalid server IP address: %s", _serverIp);
            return false;
        }

        if (connect(_clientSocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            NB_ERROR( "Error connecting to server: %s", strerror(errno));
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
            NB_ERROR("Error binding socket: %s", strerror(errno));
            NB_INFO("\t- Make sure the port is not already in use"); 
            close(_socket);
            _socket = -1; // Set to -1 to indicate an error
        }
    }

    void MacOS_Listen(Socket& _socket)
    {
        if (listen(_socket, SOMAXCONN) < 0) {
            NB_ERROR("Error listening on socket: %s", strerror(errno));
            close(_socket);
            _socket = -1; // Set to -1 to indicate an error
        }
    }

    void MacOS_SendData(Socket& _socket, std::string _data)
    {
        ssize_t bytes_sent = send(_socket, _data.c_str(), _data.size(), 0);
        if (bytes_sent < 0) {
            NB_ERROR( "Error sending data: %s", strerror(errno));
        }
    }

    void MacOS_ReceiveData(Socket& _socket, std::string _data)
    {
        char buffer[1024];
        ssize_t bytes_received = recv(_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            NB_ERROR("Error receiving data: %s", strerror(errno));
        } else if (bytes_received == 0) {
            NB_INFO("Connection closed by peer");
        } else {
            buffer[bytes_received] = '\0'; // Null-terminate the received data
            _data.assign(buffer, bytes_received);
        }
    }

    #endif // NETBLT_PLATFORM_MACOS
} // namespace netblt
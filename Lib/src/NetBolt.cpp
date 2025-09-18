#include "NetBolt.h"
#include <iostream>

namespace netblt
{
    ///////////////////////////////////////////////
    ////// NETBOLT FUNCTION IMPLEMENTATIONS ///////
    ///////////////////////////////////////////////

    Socket CreateSocket(Socket _port, SocketType _type, IP _ip)
    {
        #ifdef NETBLT_PLATFORM_MACOS
        return MacOS_CreateSocket(_port, _type, _ip);
        #else
        // For other platforms, you can implement similar functions or return an error
        return -1; // Not implemented for this platform
        #endif
    }

    void CloseSocket(Socket _socket)
    {
        #ifdef NETBLT_PLATFORM_MACOS
        MacOS_CloseSocket(_socket);
        #else
        // For other platforms, you can implement similar functions or return an error
        // No-op for now
        #endif
    }

    void AcceptConnection(Socket& _serverSocket, ClientSocket& _client)
    {
        #ifdef NETBLT_PLATFORM_MACOS
        MacOS_AcceptClient(_serverSocket, _client);
        #else
        // For other platforms, you can implement similar functions or return an error
        // No-op for now
        #endif
    }

    bool Connect(Socket& _clientSocket, IP _serverIp, Port _port)
    {
        #ifdef NETBLT_PLATFORM_MACOS
        return MacOS_Connect(_clientSocket, _serverIp, _port);
        #else
        // For other platforms, you can implement similar functions or return an error
        // No-op for now
        #endif
    }

    void BindSocket(Socket& _socket, Port _port)
    {
        #ifdef NETBLT_PLATFORM_MACOS
        MacOS_BindSocket(_socket, _port);
        #else
        // For other platforms, you can implement similar functions or return an error
        // No-op for now
        #endif
    }

    void Listen(Socket& _socket)
    {
        #ifdef NETBLT_PLATFORM_MACOS
        MacOS_Listen(_socket);
        #else
        // For other platforms, you can implement similar functions or return an error
        // No-op for now
        #endif
    }

    void SendData(Socket& _socket, std::string _data)
    {
        #ifdef NETBLT_PLATFORM_MACOS
        MacOS_SendData(_socket, _data);
        #endif
    }

    void ReceiveData(Socket& _socket, std::string _data)
    {
        #ifdef NETBLT_PLATFORM_MACOS
        MacOS_ReceiveData(_socket, _data);
        #endif
    }

    /////////////////////////////////////////
    /////// TCP SERVER IMPLEMENTATION ///////
    /////////////////////////////////////////

    TCPServer::TCPServer(Port port, IP ip)
        : m_Port(port), m_IP(ip), m_IsRunning(false)
    {
        if ((m_Socket = CreateSocket(port, SocketType::NETBLT_SOCK_STREAM, ip)) >= 0) {
            std::cout << "TCPServer created on port " << port << " with IP " << ip << std::endl;
        }
    }

    TCPServer::~TCPServer()
    {
        if (m_IsRunning) {
            Stop();
        }

        CloseSocket(m_Socket);
    }

    void TCPServer::Start()
    {
        if (m_IsRunning) {
            std::cerr << "Server is already running." << std::endl;
        } else { m_IsRunning = true; }

        ClientSocket client;
        client.len = sizeof(client.ip);

        BindSocket(m_Socket, m_Port);
        Listen(m_Socket);
        AcceptConnection(m_Socket, client);
        
        handleClientConnection(client);

    }

    void TCPServer::OnClientConnected(ClientHandler handler)
    {
        m_ClientHandler = handler;
    }

    void TCPServer::handleClientConnection(ClientSocket& client)
    {
        if (m_ClientHandler) {
            m_ClientHandler(client);
        } else {
            std::cerr << "No client handler set." << std::endl;
        }
    }

    void TCPServer::Stop()
    {
        if (!m_IsRunning) {
            std::cerr << "Server is not running." << std::endl;
            return;
        }

        m_IsRunning = false;

        // Close the server socket
        CloseSocket(m_Socket);
        std::cout << "TCPServer stopped." << std::endl;
    }

} // namespace netblt

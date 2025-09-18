#pragma once
#include "NetBoltUtil.h"
#include "NetBoltTypes.h"
#include "NetBoltPlatform.h"

namespace netblt
{
    // MacOS PROTOTYPES
    #ifdef NETBLT_PLATFORM_MACOS

        // Function to create a socket
        Socket MacOS_CreateSocket(Port _port, SocketType _type, IP _ip);

        // Function to close a socket
        void MacOS_CloseSocket(Socket _socket);

        // Accept a client connection
        void MacOS_AcceptClient(Socket& _serverSocket, ClientSocket& _client);

        // Function to connect a client socket to a server IP
        bool MacOS_Connect(Socket& _clientSocket, IP _serverIp, Port _port);

        void MacOS_BindSocket(Socket& _socket, Port _port);

        void MacOS_Listen(Socket& _socket);

        void MacOS_SendData(Socket& _socket, std::string _data);
        
        void MacOS_ReceiveData(Socket& _socket, std::string _data);

    #endif
    



    // NetBolt function declarations

    // SOCKETS
    Socket CreateSocket(Socket _port, SocketType _type, IP _ip);
    void CloseSocket(Socket _socket);

    // CONNECTION
    void AcceptConnection(Socket& _serverSocket, ClientSocket& _client);
    bool Connect(Socket& _clientSocket, IP _serverIp, Port _port);

    void BindSocket(Socket& _socket);
    void Listen(Socket& _socket);

    void SendData(Socket& _socket, std::string _data);
    void ReceiveData(Socket& _socket, std::string _data);

    // SERVER
    class TCPServer
    {
        using ClientHandler = std::function<void(ClientSocket&)>;

    public:
        TCPServer(Port port, IP ip);
        ~TCPServer();
        
        void Start();
        void OnClientConnected(ClientHandler handler);
        void Stop();

        bool IsRunning() const { return m_IsRunning; }
        bool IsValid() const { return (m_Socket >= 0 &&
             m_Port > 0 && m_ClientHandler && m_IP != "\0"); }

    private:
        void handleClientConnection(ClientSocket& client);
        
    private:
        // Member variables
        IP m_IP = "\0";
        Port m_Port = 0;
        Socket m_Socket = -1;
        bool m_IsRunning = false;
        ClientHandler m_ClientHandler = nullptr;
    };

} // namespace netblt

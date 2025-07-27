#include <iostream>
#include "NetBolt.h"

enum ARGS {
    CLIENT,
    SERVER,
    IP,
    PORT
};

int match(const char* str) {
    if (strcmp(str, "-client") == 0 || strcmp(str, "-c") == 0) {
        return CLIENT;
    } else if (strcmp(str, "-server") == 0 || strcmp(str, "-s") == 0) {
        return SERVER;
    } else if (strcmp(str, "-ip") == 0){
        return IP;
    } else if (strcmp(str, "-p") == 0) {
        return PORT;
    }
    return -1;
}


struct TestInfo
{
    netblt::IP ip;
    netblt::IP serverIp = "192.168.2.10";
    netblt::Port port;
    netblt::TCPServer* server = nullptr;
};

void MakeClient(TestInfo& testInfo) {

    netblt::Socket socket = netblt::CreateSocket(testInfo.port, netblt::SocketType::NETBLT_SOCK_STREAM, testInfo.ip);

    if (socket > 0) {
        std::cout << "Created socket on port " << testInfo.port << " with IP " << testInfo.ip << std::endl;
    }

    if (netblt::Connect(socket, testInfo.serverIp, 8080))
    {
        std::cout << "Client Connected!\n";
    }
    
    netblt::CloseSocket(socket);
}


void MakeServer(TestInfo& testInfo) {
    testInfo.server = new netblt::TCPServer(testInfo.port, testInfo.ip);

    testInfo.server->OnClientConnected([](netblt::ClientSocket& client) {
        std::cout << "Client connected!" << std::endl;
        std::cout << "Client IP: " << netblt::InetToString(client.ip) << std::endl;
    });

    testInfo.server->Start();
    std::cout << "Server started on port " << testInfo.port << " with IP " << testInfo.ip << std::endl;
}


int main(int argc, char* argv[]) {

    TestInfo testInfo;

    for (int i = 0; i < argc; ++i) {
        switch (match(argv[i]))  // Example of using a switch statement
        {
        case CLIENT:
            MakeClient(testInfo); 
            break;

        case SERVER:
            MakeServer(testInfo);
            break;

        case IP:
            if (i + 1 < argc) { // Check if there is an argument after "-ip"
                testInfo.ip = argv[i + 1]; // Assign the next argument as the IP
            } else {
                std::cerr << "Error: No IP address provided after -ip option." << std::endl;
                return 1; // Exit with an error code
            }
            break;

        case PORT:
            if (i + 1 < argc) { // Check if there is an argument after "-p"
                testInfo.port = static_cast<netblt::Port>(std::stoi(argv[i + 1])); // Convert the next argument to Port type
            } else {
                std::cerr << "Error: No port number provided after -p option." << std::endl;
                return 1; // Exit with an error code
            }
            break;

        default:
            break;
        }
    }

    return 0;
}
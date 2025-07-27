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
    netblt::IP serverIp = "0.0.0.0";
    netblt::Port port;
    netblt::TCPServer* server = nullptr;
};

void MakeClient(TestInfo& testInfo) {

    netblt::Socket socket = netblt::CreateSocket(testInfo.port, netblt::SocketType::NETBLT_SOCK_STREAM, testInfo.ip);

    if (socket > 0) {
        std::cout << "Created socket on port " << testInfo.port << " with IP " << testInfo.ip << std::endl;
    }

    if (netblt::Connect(socket, testInfo.serverIp, 8084))
    {
        std::cout << "Server Connected!\n";

        while (true) {
            std::string data;
            std::cout << "send>> ";
            std::getline(std::cin, data);
            if (data == "exit") {
                break; // Exit the loop if 'exit' is entered
            }
            netblt::SendData(socket, data);
            
            data = "";

            netblt::ReceiveData(socket, data);
            if (!data.empty()) {
                std::cout << "recv>> "<< data << std::endl;
            }  
        }
    }
    
    netblt::CloseSocket(socket);
}


void MakeServer(TestInfo& testInfo) {
    testInfo.server = new netblt::TCPServer(testInfo.port, testInfo.ip);

    testInfo.server->OnClientConnected([](netblt::ClientSocket& client) {
        std::cout << "Client connected!" << std::endl;
        std::cout << "Client IP: " << netblt::InetToString(client.ip) << std::endl;

        while (true) {
            std::string data;
            netblt::ReceiveData(client.socket, data);
            if (data.empty()) {
                std::cout << "Client disconnected." << std::endl;
                break; // Exit the loop if no data is received
            }
            std::cout << "Received data: " << data << std::endl;

            if (data == "Hello, Server!") {
                std::string response = "Hello, Client!";
                netblt::SendData(client.socket, response);
                std::cout << "Sent response: " << response << std::endl;
            } else {
                if (data == "exit") {
                    std::cout << "Client requested exit." << std::endl;
                    break; // Exit the loop if 'exit' is received
                } else {
                    netblt::SendData(client.socket, data);
                    std::cout << "..." << data << std::endl;
                }
            }
        }

    });

    testInfo.server->Start();
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
#include "NetBolt.h"
#include <cassert>
#include <iostream>
#include <string>
#include <thread>
#include <assert.h>

void test_socket_creation() {
    netblt::IP ip = "127.0.0.1";
    netblt::Port port = 8080;

    netblt::Socket sock = netblt::CreateSocket(port, netblt::SocketType::NETBLT_SOCK_STREAM, ip);
    assert(sock > 0 && "Socket creation failed");

    netblt::CloseSocket(sock);
}

void run_srv()
{
    netblt::IP ip = "192.168.0.1";     // listen on all interfaces
    netblt::Port port = 8080;
    netblt::TCPServer server(port, ip);

    // Requires OnClientConnected to take std::function<void(ClientSocket&)>
    server.OnClientConnected([&server](netblt::ClientSocket& sock){
        std::cout << "test client connected: "
                  << netblt::InetToString(sock.ip) << '\n';
        // Prefer to signal stop asynchronously if Stop() blocks the accept thread
        server.Stop();
    });

    server.Start();  // blocks until Stop()
}

void test_server_binding_and_listen()
{
    // Start server
    std::thread srv_thread(run_srv);

    // let the srv start
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    // Create client and connect to localhost 
    netblt::IP ip = "127.0.0.1";
    netblt::Port local_port = 0; // let OS pick an ephemeral port
    netblt::Socket client = netblt::CreateSocket(local_port, netblt::NETBLT_SOCK_STREAM, ip);

    // Connect to the server's listening endpoint
    netblt::Connect(client, "127.0.0.1", 8080);

    srv_thread.join();
}



int main() {
    test_socket_creation();
    std::cout << "[PASS] test_socket_creation" << std::endl;

    test_server_binding_and_listen();
    std::cout << "[PASS] test_server_binding_and_listen" << std::endl;

    return 0;
}

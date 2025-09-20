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
    if (!(sock > 0))
        NB_FATAL("==> FAIL");

    netblt::CloseSocket(sock);
}

void run_srv()
{
    netblt::IP ip = "192.168.0.1";     
    netblt::Port port = 8081;
    netblt::TCPServer server(port, ip);

    // Requires OnClientConnected to take std::function<void(ClientSocket&)>
    server.OnClientConnected([&server](netblt::ClientSocket& sock){
        NB_INFO("test client connected: %s", netblt::InetToString(sock.ip));
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
    if (!netblt::Connect(client, "127.0.0.1", 8081))
    {
        srv_thread.join();
        NB_FATAL("== TERMINATING ==");
    }
    
    srv_thread.join();
}



int main() {
    NB_DEBUG("== TESTING | SOCKET CREATION ==");
    test_socket_creation();
    NB_DEBUG("==> [PASS] \n");

    NB_DEBUG("== TESTING | TCP SRV ==");
    test_server_binding_and_listen();
    NB_DEBUG("==> [PASS] \n");

    return 0;
}

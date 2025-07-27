#pragma once

#include "NetBoltPlatform.h"

namespace netblt
{
    // NetBolt Type Definitions
    using Socket = int;     // Define Socket as an integer type for socket file descriptors
    using IP = const char*; // Define IP as a pointer to a constant character (string) for IP addresses 
    using Port = uint16_t;  // Define Port as a 16-bit unsigned integer for port numbers

    enum SocketType
    {
        NETBLT_SOCK_STREAM = 0, // Stream socket (TCP)
        NETBLT_SOCK_DGRAM = 1,   // Datagram socket (UDP)
    };

    #ifdef NETBLT_PLATFORM_MACOS

    using SAddrIn = struct sockaddr_in; // Define SAddrIn as sockaddr_in for MacOS compatibility

    struct ClientSocket
    {
        Socket socket; // The socket file descriptor
        SAddrIn ip;  // The IP address structure      
        unsigned int len; // Length of the sockaddr_in structure
    };

    inline int to_bsd_socket_type(SocketType type)
    {
        switch (type)
        {
            case NETBLT_SOCK_STREAM:
                return SOCK_STREAM;
            case NETBLT_SOCK_DGRAM:
                return SOCK_DGRAM;
            default:
                return -1; // Invalid socket type
        }
    }

    #endif // NETBLT_PLATFORM_MACOS
}

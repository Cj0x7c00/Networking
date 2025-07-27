#pragma once 
#include "NetBoltPlatform.h"
#include "NetBoltTypes.h"
#include <iostream>

namespace netblt
{
    #ifdef NETBLT_DBUG
    // Debugging function to print socket creation details
    #define NETBLT_PRINT_DEBUG(msg) \
        std::cout << "DEBUG: " << msg << std::endl;
    #else
    // No-op for release builds
    #define NETBLT_PRINT_DEBUG(msg) \
        do {} while (0)
    #endif

    #ifdef NETBLT_PLATFORM_MACOS
        inline const char* InetToString(SAddrIn& _addr) 
        {
            return inet_ntoa(_addr.sin_addr);
        }
    #endif
}
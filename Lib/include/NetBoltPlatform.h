#pragma once


#ifdef __APPLE__
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
        #define NETBLT_PLATFORM_IOS
    #elif TARGET_OS_MAC
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <netinet/in.h>     // For sockaddr_in, htons, etc.
        #include <arpa/inet.h>      // For inet_pton, inet_ntop
        #include <unistd.h>         // For close()
        #include <netdb.h>          // For getaddrinfo(), freeaddrinfo()
        #include <string.h>         // For memset()
        #include <errno.h>          // For errno
        #define NETBLT_PLATFORM_MACOS

    #endif
#endif
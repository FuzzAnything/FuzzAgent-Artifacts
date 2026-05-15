#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ares.h"

namespace harness_006 {


// Callback for ares_getnameinfo
static void nameinfo_callback(void *arg, int status, int timeouts,
                               const char *node, const char *service) {
    // Do nothing - just consume the callback
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)node;
    (void)service;
}

// Callback for ares_gethostbyaddr
static void hostbyaddr_callback(void *arg, int status, int timeouts,
                                 const struct hostent *hostent) {
    // Do nothing - just consume the callback
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
}


// Simple socket create callback for ares_set_socket_functions_ex
static ares_socket_t socket_create_callback(int domain, int type, int protocol, void *data) {
    (void)domain;
    (void)type;
    (void)protocol;
    (void)data;
    return ARES_SOCKET_BAD;
}

// Simple socket close callback for ares_set_socket_functions_ex
static int socket_close_callback(ares_socket_t socket_fd, void *data) {
    (void)socket_fd;
    (void)data;
    return 0;
}

// Simple socket setsockopt callback for ares_set_socket_functions_ex
static int socket_setsockopt_callback(ares_socket_t sock, ares_socket_opt_t opt, const void *val,
                                     ares_socklen_t val_size, void *data) {
    (void)sock;
    (void)opt;
    (void)val;
    (void)val_size;
    (void)data;
    return -1; // Indicate not supported
}

// Simple socket connect callback for ares_set_socket_functions_ex
static int socket_connect_callback(ares_socket_t sock, const struct sockaddr *address,
                                   ares_socklen_t address_len, unsigned int flags, void *data) {
    (void)sock;
    (void)address;
    (void)address_len;
    (void)flags;
    (void)data;
    return 0;
}

// Simple socket recvfrom callback for ares_set_socket_functions_ex
static ares_ssize_t socket_recvfrom_callback(ares_socket_t sock, void *buffer, size_t length,
                                             int flags, struct sockaddr *address,
                                             ares_socklen_t *address_len, void *data) {
    (void)sock;
    (void)buffer;
    (void)length;
    (void)flags;
    (void)address;
    (void)address_len;
    (void)data;
    return -1;
}

// Simple socket sendto callback for ares_set_socket_functions_ex
static ares_ssize_t socket_sendto_callback(ares_socket_t sock, const void *buffer, size_t length,
                                           int flags, const struct sockaddr *address,
                                           ares_socklen_t address_len, void *data) {
    (void)sock;
    (void)buffer;
    (void)length;
    (void)flags;
    (void)address;
    (void)address_len;
    (void)data;
    return -1;
}

// Simple socket getsockname callback for ares_set_socket_functions_ex
static int socket_getsockname_callback(ares_socket_t sock, struct sockaddr *address,
                                       ares_socklen_t *address_len, void *data) {
    (void)sock;
    (void)address;
    (void)address_len;
    (void)data;
    return -1;
}

// Simple socket bind callback for ares_set_socket_functions_ex
static int socket_bind_callback(ares_socket_t sock, unsigned int flags,
                                const struct sockaddr *address, socklen_t address_len, void *data) {
    (void)sock;
    (void)flags;
    (void)address;
    (void)address_len;
    (void)data;
    return -1;
}
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for initialization and basic parameters
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // Step 1: ares_init_options()
    ares_channel_t* channel = NULL;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    
    // Consume options from fuzzer input
    options.flags = fdp.ConsumeIntegral<int>();
    options.timeout = fdp.ConsumeIntegralInRange<int>(100, 10000);
    options.tries = fdp.ConsumeIntegralInRange<int>(1, 10);
    options.ndots = fdp.ConsumeIntegralInRange<int>(0, 15);
    options.udp_port = fdp.ConsumeIntegralInRange<unsigned short>(1, 65535);
    options.tcp_port = fdp.ConsumeIntegralInRange<unsigned short>(1, 65535);
    
    int optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES | 
                  ARES_OPT_NDOTS | ARES_OPT_UDP_PORT | ARES_OPT_TCP_PORT;
    
    int init_result = ares_init_options(&channel, &options, optmask);
    
    if (init_result != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Step 2: Optional ares_set_socket_functions_ex()
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 16) {
        ares_socket_functions_ex socket_funcs;
        memset(&socket_funcs, 0, sizeof(socket_funcs));
        socket_funcs.version = 1;
        socket_funcs.flags = ARES_SOCKFUNC_FLAG_NONBLOCKING;
        socket_funcs.asocket = socket_create_callback;
        socket_funcs.aclose = socket_close_callback;
        socket_funcs.asetsockopt = socket_setsockopt_callback;
        socket_funcs.aconnect = socket_connect_callback;
        socket_funcs.arecvfrom = socket_recvfrom_callback;
        socket_funcs.asendto = socket_sendto_callback;
        socket_funcs.agetsockname = socket_getsockname_callback;
        socket_funcs.abind = socket_bind_callback;
        socket_funcs.aif_nametoindex = NULL; // Optional
        socket_funcs.aif_indextoname = NULL; // Optional
        
        ares_set_socket_functions_ex(channel, &socket_funcs, NULL);
    }
    
    // Step 3: ares_getnameinfo() with IPv4/IPv6 addresses and flag combinations
    if (fdp.remaining_bytes() > 32) {
        // Determine address type from fuzzer input
        bool use_ipv6 = fdp.ConsumeBool();
        int flags = fdp.ConsumeIntegral<int>();
        
        struct sockaddr_storage ss;
        struct sockaddr *sa = (struct sockaddr *)&ss;
        ares_socklen_t salen;
        
        if (use_ipv6 && fdp.remaining_bytes() >= 16) {
            // IPv6 address
            struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *)&ss;
            sa6->sin6_family = AF_INET6;
            sa6->sin6_port = htons(fdp.ConsumeIntegralInRange<uint16_t>(1, 65535));
            
            // Consume 16 bytes for IPv6 address
            std::vector<uint8_t> ipv6_bytes = fdp.ConsumeBytes<uint8_t>(16);
            if (ipv6_bytes.size() == 16) {
                memcpy(&sa6->sin6_addr, ipv6_bytes.data(), 16);
            } else {
                // Fallback to some IPv6 address
                memset(&sa6->sin6_addr, 0, sizeof(sa6->sin6_addr));
                sa6->sin6_addr.s6_addr[0] = 0x20;
                sa6->sin6_addr.s6_addr[1] = 0x01;
                sa6->sin6_addr.s6_addr[15] = 0x01;
            }
            
            salen = sizeof(struct sockaddr_in6);
        } else {
            // IPv4 address
            struct sockaddr_in *sa4 = (struct sockaddr_in *)&ss;
            sa4->sin_family = AF_INET;
            sa4->sin_port = htons(fdp.ConsumeIntegralInRange<uint16_t>(1, 65535));
            
            // Consume 4 bytes for IPv4 address
            std::vector<uint8_t> ipv4_bytes = fdp.ConsumeBytes<uint8_t>(4);
            if (ipv4_bytes.size() == 4) {
                memcpy(&sa4->sin_addr, ipv4_bytes.data(), 4);
            } else {
                // Fallback to some IPv4 address
                sa4->sin_addr.s_addr = htonl(0x7f000001); // 127.0.0.1
            }
            
            salen = sizeof(struct sockaddr_in);
        }
        
        // Call ares_getnameinfo with consumed parameters
        ares_getnameinfo(channel, sa, salen, flags, nameinfo_callback, NULL);
    }
    
    // Step 4: ares_gethostbyaddr() for reverse lookups
    if (fdp.remaining_bytes() > 20) {
        // Determine address family from fuzzer input
        int family = fdp.ConsumeBool() ? AF_INET : AF_INET6;
        int addrlen = (family == AF_INET) ? 4 : 16;
        
        // Consume address bytes
        std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(addrlen);
        if (addr_bytes.size() >= addrlen) {
            ares_gethostbyaddr(channel, addr_bytes.data(), addrlen, family, 
                               hostbyaddr_callback, NULL);
        }
    }
    
    // Step 5: ares_process_pending_write() for write operations
    if (fdp.ConsumeBool()) {
        ares_process_pending_write(channel);
    }
    
    // Step 6: ares_getsock()/ares_fds() for socket descriptor management
    if (fdp.remaining_bytes() > 4) {
        // Test ares_getsock
        ares_socket_t socks[ARES_GETSOCK_MAXNUM];
        int numsocks = fdp.ConsumeIntegralInRange<int>(1, ARES_GETSOCK_MAXNUM);
        int getsock_result = ares_getsock(channel, socks, numsocks);
        (void)getsock_result;
        
        // Test ares_fds (deprecated but still in the API)
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        int fds_result = ares_fds(channel, &read_fds, &write_fds);
        (void)fds_result;
    }
    
    // Step 7: ares_process_fd()/ares_process_fds() for I/O processing
    if (fdp.remaining_bytes() > 8) {
        // Test ares_process_fd with random file descriptors
        ares_socket_t read_fd = fdp.ConsumeIntegral<ares_socket_t>();
        ares_socket_t write_fd = fdp.ConsumeIntegral<ares_socket_t>();
        ares_process_fd(channel, read_fd, write_fd);
        
        // Test ares_process_fds with empty events array (just timeout processing)
        if (fdp.ConsumeBool()) {
            ares_process_fds(channel, NULL, 0, ARES_PROCESS_FLAG_NONE);
        }
        
        // Test ares_process_fds with some events if we have enough data
        if (fdp.remaining_bytes() > 16) {
            ares_fd_events_t events[2];
            size_t nevents = fdp.ConsumeIntegralInRange<size_t>(0, 2);
            
            for (size_t i = 0; i < nevents && i < 2; i++) {
                events[i].fd = fdp.ConsumeIntegral<ares_socket_t>();
                events[i].events = fdp.ConsumeIntegral<unsigned int>() & 
                                  (ARES_FD_EVENT_READ | ARES_FD_EVENT_WRITE);
            }
            
            unsigned int process_flags = fdp.ConsumeIntegral<unsigned int>() & 
                                        ARES_PROCESS_FLAG_SKIP_NON_FD;
            
            ares_process_fds(channel, events, nevents, process_flags);
        }
    }
    
    // Process any pending operations with timeouts
    if (fdp.ConsumeBool()) {
        // Get timeout
        struct timeval maxtv;
        struct timeval tv;
        memset(&maxtv, 0, sizeof(maxtv));
        memset(&tv, 0, sizeof(tv));
        
        struct timeval *timeout = ares_timeout(channel, &maxtv, &tv);
        (void)timeout;
        
        // Also test the deprecated ares_process
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        ares_process(channel, &read_fds, &write_fds);
    }
    
    // Step 8: Cleanup
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_006

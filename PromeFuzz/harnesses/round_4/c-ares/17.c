// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_gethostbyaddr at ares_gethostbyaddr.c:108:6 in ares.h
// ares_getnameinfo at ares_getnameinfo.c:188:6 in ares.h
// ares_getnameinfo at ares_getnameinfo.c:188:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_process_fd at ares_process.c:267:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_getaddrinfo at ares_getaddrinfo.c:695:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

static void dummy_addrinfo_callback(void *arg, int status, int timeouts, struct ares_addrinfo *result) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)result;
}

static void dummy_host_callback(void *arg, int status, int timeouts, const struct hostent *hostent) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
}

static void dummy_nameinfo_callback(void *arg, int status, int timeouts, const char *node, const char *service) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)node;
    (void)service;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize channel
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    options.timeout = 1000;
    optmask |= ARES_OPT_TIMEOUTMS;
    options.tries = 1;
    optmask |= ARES_OPT_TRIES;
    
    int status = ares_init_options(&channel, &options, optmask);
    if (status != ARES_SUCCESS) {
        return 0;
    }

    // Prepare data for API calls
    char node[256] = {0};
    char service[256] = {0};
    unsigned char addr[16] = {0};
    struct sockaddr_in sa = {0};
    struct sockaddr_in6 sa6 = {0};
    struct ares_addrinfo_hints hints = {0};
    
    // Use fuzz data to populate parameters
    size_t node_len = Size > 255 ? 255 : Size;
    memcpy(node, Data, node_len);
    node[node_len] = '\0';
    
    if (Size > 256) {
        size_t service_len = (Size - 256) > 255 ? 255 : (Size - 256);
        memcpy(service, Data + 256, service_len);
        service[service_len] = '\0';
    }
    
    // Prepare address data
    size_t addr_len = Size > 16 ? 16 : Size;
    memcpy(addr, Data, addr_len);
    
    // Set up sockaddr structures
    sa.sin_family = AF_INET;
    sa.sin_port = htons(80);
    if (Size >= 4) {
        memcpy(&sa.sin_addr, Data, 4);
    }
    
    sa6.sin6_family = AF_INET6;
    sa6.sin6_port = htons(80);
    if (Size >= 16) {
        memcpy(&sa6.sin6_addr, Data, 16);
    }
    
    // Set hints
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    // Call target functions in specified order
    
    // 1. ares_getaddrinfo
    ares_getaddrinfo(channel, node, service, &hints, dummy_addrinfo_callback, NULL);
    
    // 2. ares_gethostbyaddr
    int family = (Size % 2) ? AF_INET : AF_INET6;
    int len = (family == AF_INET) ? 4 : 16;
    ares_gethostbyaddr(channel, addr, len, family, dummy_host_callback, NULL);
    
    // 3. ares_getnameinfo
    int flags = 0;
    if (Size % 2 == 0) {
        flags |= ARES_NI_NOFQDN;
    }
    if (Size % 3 == 0) {
        flags |= ARES_NI_NUMERICHOST;
    }
    
    if (Size % 4 == 0) {
        ares_getnameinfo(channel, (struct sockaddr*)&sa, sizeof(sa), flags, 
                        dummy_nameinfo_callback, NULL);
    } else {
        ares_getnameinfo(channel, (struct sockaddr*)&sa6, sizeof(sa6), flags, 
                        dummy_nameinfo_callback, NULL);
    }
    
    // 4. ares_reinit
    ares_reinit(channel);
    
    // Process pending queries
    ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    
    // Cleanup
    ares_destroy(channel);
    
    return 0;
}
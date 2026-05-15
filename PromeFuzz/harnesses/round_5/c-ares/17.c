// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_getaddrinfo at ares_getaddrinfo.c:695:6 in ares.h
// ares_getaddrinfo at ares_getaddrinfo.c:695:6 in ares.h
// ares_gethostbyaddr at ares_gethostbyaddr.c:108:6 in ares.h
// ares_gethostbyaddr at ares_gethostbyaddr.c:108:6 in ares.h
// ares_getnameinfo at ares_getnameinfo.c:188:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_process_fd at ares_process.c:267:6 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static void dummy_host_callback(void *arg, int status, int timeouts, const struct hostent *hostent) {
    // Dummy callback - do NOT free hostent as library handles it
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
}

static void dummy_addrinfo_callback(void *arg, int status, int timeouts, struct ares_addrinfo *res) {
    // Dummy callback - do NOT free addrinfo as library handles it
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)res;
}

static void dummy_nameinfo_callback(void *arg, int status, int timeouts, const char *node, const char *service) {
    // Dummy callback - do NOT free strings as library handles them
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)node;
    (void)service;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize channel
    ares_channel_t *channel = NULL;
    int options = ARES_OPT_FLAGS;
    struct ares_options ares_opts;
    
    ares_opts.flags = ARES_FLAG_NOCHECKRESP;
    ares_opts.timeout = 1000;
    ares_opts.tries = 1;
    
    int status = ares_init_options(&channel, &ares_opts, options);
    if (status != ARES_SUCCESS) {
        return 0;
    }

    // Prepare data for function calls
    char *node = NULL;
    char *service = NULL;
    struct ares_addrinfo_hints hints;
    struct sockaddr_in sa_in;
    void *addr = NULL;
    int addrlen = 0;
    int family = AF_INET;
    int flags = 0;

    // Use fuzzer data to set parameters
    if (Size >= 1) {
        // For ares_getaddrinfo
        if (Size >= 2) {
            size_t node_len = Size < 100 ? Size : 100;
            node = malloc(node_len + 1);
            if (node) {
                memcpy(node, Data, node_len);
                node[node_len] = '\0';
            }
        }

        if (Size >= 3) {
            size_t service_len = Size - 1 < 50 ? Size - 1 : 50;
            service = malloc(service_len + 1);
            if (service) {
                memcpy(service, Data + 1, service_len);
                service[service_len] = '\0';
            }
        }

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        // For ares_gethostbyaddr
        if (Size >= 7) {  // Enough for IPv4 address
            addr = malloc(4);
            if (addr) {
                memcpy(addr, Data + 2, 4);
                addrlen = 4;
                family = AF_INET;
            }
        } else if (Size >= 5) {
            addrlen = Size - 2 < 4 ? Size - 2 : 4;
            addr = malloc(addrlen);
            if (addr) {
                memcpy(addr, Data + 2, addrlen);
                family = AF_INET;
            }
        }

        // For ares_getnameinfo
        memset(&sa_in, 0, sizeof(sa_in));
        sa_in.sin_family = AF_INET;
        sa_in.sin_port = htons(80);
        if (Size >= sizeof(struct sockaddr_in) + 4) {
            memcpy(&sa_in.sin_addr, Data + 10, 4);
        } else {
            sa_in.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        }

        flags = Data[0] & 0xFF;
    }

    // Call functions in specified order
    if (node) {
        ares_getaddrinfo(channel, node, service, &hints, dummy_addrinfo_callback, NULL);
    } else {
        ares_getaddrinfo(channel, "localhost", "http", &hints, dummy_addrinfo_callback, NULL);
    }

    if (addr && addrlen > 0) {
        ares_gethostbyaddr(channel, addr, addrlen, family, dummy_host_callback, NULL);
    } else {
        // Use default address
        struct in_addr default_addr;
        default_addr.s_addr = htonl(INADDR_LOOPBACK);
        ares_gethostbyaddr(channel, &default_addr, sizeof(default_addr), AF_INET, dummy_host_callback, NULL);
    }

    ares_getnameinfo(channel, (struct sockaddr *)&sa_in, sizeof(sa_in), flags, dummy_nameinfo_callback, NULL);

    ares_reinit(channel);

    // Process pending callbacks to avoid leaks
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    // Process any pending timeouts
    ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    
    // Cleanup
    if (node) free(node);
    if (service) free(service);
    if (addr) free(addr);
    
    // Cancel any pending queries before destroying
    ares_cancel(channel);
    
    ares_destroy(channel);
    
    return 0;
}
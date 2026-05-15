// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init at ares_init.c:67:5 in ares.h
// ares_getaddrinfo at ares_getaddrinfo.c:695:6 in ares.h
// ares_gethostbyaddr at ares_gethostbyaddr.c:108:6 in ares.h
// ares_getnameinfo at ares_getnameinfo.c:188:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

static void dummy_addrinfo_callback(void* arg, int status, int timeouts, struct ares_addrinfo* result) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)result;
}

static void dummy_host_callback(void* arg, int status, int timeouts, const struct hostent* hostent) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
}

static void dummy_nameinfo_callback(void* arg, int status, int timeouts, const char* node, const char* service) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)node;
    (void)service;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(struct sockaddr_in)) {
        return 0;
    }

    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;

    if (ares_init(&channel) != ARES_SUCCESS) {
        return 0;
    }

    char node_buf[256];
    char service_buf[256];
    
    size_t node_len = Size > 0 ? (Size < sizeof(node_buf) ? Size : sizeof(node_buf) - 1) : 0;
    if (node_len > 0) {
        memcpy(node_buf, Data, node_len);
        node_buf[node_len] = '\0';
    } else {
        node_buf[0] = '\0';
    }
    
    const char *node = node_buf;
    const char *service = "";
    
    if (Size > node_len) {
        size_t service_len = (Size - node_len) < sizeof(service_buf) ? (Size - node_len) : sizeof(service_buf) - 1;
        if (service_len > 0) {
            memcpy(service_buf, Data + node_len, service_len);
            service_buf[service_len] = '\0';
            service = service_buf;
        }
    }
    
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = 0;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    ares_getaddrinfo(channel, node, service, &hints, dummy_addrinfo_callback, NULL);

    struct sockaddr_in addr_in;
    memset(&addr_in, 0, sizeof(addr_in));
    if (Size >= sizeof(struct sockaddr_in)) {
        memcpy(&addr_in, Data, sizeof(struct sockaddr_in));
    }
    ares_gethostbyaddr(channel, &addr_in, sizeof(struct sockaddr_in), AF_INET, dummy_host_callback, NULL);

    if (Size >= sizeof(struct sockaddr)) {
        struct sockaddr sa;
        memset(&sa, 0, sizeof(sa));
        size_t copy_size = Size < sizeof(sa) ? Size : sizeof(sa);
        memcpy(&sa, Data, copy_size);
        ares_socklen_t salen = copy_size;
        ares_getnameinfo(channel, &sa, salen, 0, dummy_nameinfo_callback, NULL);
    }

    ares_reinit(channel);

    ares_destroy(channel);
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_set_socket_configure_callback at ares_socket.c:406:6 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_getnameinfo at ares_getnameinfo.c:188:6 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "ares.h"

static int dummy_socket_create_callback(ares_socket_t sockfd, int type, void *user_data) {
    (void)sockfd;
    (void)type;
    (void)user_data;
    return 0;
}

static int dummy_socket_configure_callback(ares_socket_t fd, int type, void *data) {
    (void)fd;
    (void)type;
    (void)data;
    return 0;
}

static void dummy_nameinfo_callback(void *arg, int status, int timeouts, const char *node, const char *service) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)node;
    (void)service;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Initialize channel
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    // Use first byte for configuration
    uint8_t config = Data[0];
    Data++; Size--;
    
    // Set some options based on fuzzer input
    memset(&options, 0, sizeof(options));
    if (config & 0x01) {
        options.timeout = 1000;
        optmask |= ARES_OPT_TIMEOUTMS;
    }
    if (config & 0x02) {
        options.tries = 2;
        optmask |= ARES_OPT_TRIES;
    }
    if (config & 0x04) {
        options.ndots = 1;
        optmask |= ARES_OPT_NDOTS;
    }
    if (config & 0x08) {
        options.flags = ARES_FLAG_USEVC | ARES_FLAG_PRIMARY;
        optmask |= ARES_OPT_FLAGS;
    }
    
    // Create initial channel
    int status = ares_init_options(&channel, &options, optmask);
    if (status != ARES_SUCCESS) {
        return 0;
    }
    
    // Use next byte to determine which functions to call
    uint8_t function_selector = Data[0];
    Data++; Size--;
    
    // Try to create a dummy sockaddr for getnameinfo
    struct sockaddr_storage sa_storage;
    struct sockaddr *sa = (struct sockaddr*)&sa_storage;
    ares_socklen_t salen = 0;
    
    if (Size >= 16) {  // Enough for IPv4 or IPv6
        if (Data[0] % 2 == 0) {  // IPv4
            struct sockaddr_in *sa4 = (struct sockaddr_in*)sa;
            sa4->sin_family = AF_INET;
            sa4->sin_port = htons(80);
            // Copy some bytes for address
            size_t copy_size = (Size - 1) < sizeof(sa4->sin_addr) ? (Size - 1) : sizeof(sa4->sin_addr);
            memcpy(&sa4->sin_addr, Data + 1, copy_size);
            salen = sizeof(struct sockaddr_in);
        } else {  // IPv6
            struct sockaddr_in6 *sa6 = (struct sockaddr_in6*)sa;
            sa6->sin6_family = AF_INET6;
            sa6->sin6_port = htons(80);
            // Copy some bytes for address
            size_t copy_size = (Size - 1) < sizeof(sa6->sin6_addr) ? (Size - 1) : sizeof(sa6->sin6_addr);
            memcpy(&sa6->sin6_addr, Data + 1, copy_size);
            salen = sizeof(struct sockaddr_in6);
        }
    }
    
    // Test different functions based on selector
    if (function_selector & 0x01) {
        // Test ares_set_socket_callback
        ares_set_socket_callback(channel, dummy_socket_create_callback, NULL);
    }
    
    if (function_selector & 0x02) {
        // Test ares_set_socket_configure_callback
        ares_set_socket_configure_callback(channel, dummy_socket_configure_callback, NULL);
    }
    
    if (function_selector & 0x04) {
        // Test ares_cancel
        ares_cancel(channel);
    }
    
    if (function_selector & 0x08 && salen > 0) {
        // Test ares_getnameinfo if we have a valid sockaddr
        int flags = 0;
        if (config & 0x10) flags |= ARES_NI_NOFQDN;
        if (config & 0x20) flags |= ARES_NI_NUMERICHOST;
        if (config & 0x40) flags |= ARES_NI_NAMEREQD;
        
        ares_getnameinfo(channel, sa, salen, flags, dummy_nameinfo_callback, NULL);
    }
    
    if (function_selector & 0x10) {
        // Test ares_dup
        ares_channel_t *dup_channel = NULL;
        int dup_status = ares_dup(&dup_channel, channel);
        if (dup_status == ARES_SUCCESS && dup_channel) {
            // Use the duplicated channel for some operations
            if (function_selector & 0x20) {
                ares_cancel(dup_channel);
            }
            ares_destroy(dup_channel);
        }
    }
    
    // Always destroy the original channel
    ares_destroy(channel);
    
    return 0;
}
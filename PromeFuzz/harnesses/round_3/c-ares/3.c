// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_getaddrinfo at ares_getaddrinfo.c:695:6 in ares.h
// ares_fds at ares_fds.c:30:5 in ares.h
// ares_process at ares_process.c:334:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
// ares_library_init at ares_library_init.c:108:5 in ares.h
// ares_strerror at ares_strerror.c:30:13 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_strerror at ares_strerror.c:30:13 in ares.h
// ares_get_servers_csv at ares_update_servers.c:1314:7 in ares.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_callback(void* arg, int status, int timeouts, struct ares_addrinfo* result) {
    (void)arg;
    (void)status;
    (void)timeouts;
    if (result) {
        ares_freeaddrinfo(result);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Initialize library
    int flags = 0;
    if (Size > 0) {
        flags = Data[0] & 0xFF;
    }
    ares_library_init(flags);
    
    // First ares_strerror call with random error code
    int error_code1 = 0;
    if (Size > 1) {
        error_code1 = Data[1] & 0xFF;
    }
    const char* error_str1 = ares_strerror(error_code1);
    (void)error_str1;
    
    // Initialize channel with options
    ares_channel_t* channel = NULL;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    
    int optmask = 0;
    if (Size > 2) {
        optmask = Data[2] & 0xFF;
    }
    
    // Set some safe options based on fuzzer input
    if (Size > 3) {
        options.timeout = Data[3] & 0x0F;
        optmask |= ARES_OPT_TIMEOUTMS;
    }
    if (Size > 4) {
        options.tries = (Data[4] & 0x07) + 1;
        optmask |= ARES_OPT_TRIES;
    }
    
    int init_result = ares_init_options(&channel, &options, optmask);
    
    // Second ares_strerror call with initialization result
    const char* error_str2 = ares_strerror(init_result);
    (void)error_str2;
    
    if (channel != NULL) {
        // Get servers CSV
        char* servers_csv = ares_get_servers_csv(channel);
        if (servers_csv != NULL) {
            ares_free_string(servers_csv);
        }
        
        // Prepare node and service strings from fuzzer input
        char node[256] = {0};
        char service[256] = {0};
        
        // Use fuzzer data for node name (up to 255 bytes)
        size_t node_len = Size > 10 ? (Size - 10) : 0;
        if (node_len > 255) node_len = 255;
        if (node_len > 0) {
            memcpy(node, Data + 10, node_len);
            node[node_len] = '\0';
        } else {
            strcpy(node, "localhost");
        }
        
        // Use fuzzer data for service (up to 255 bytes)
        size_t service_offset = 10 + node_len;
        size_t service_len = Size > service_offset ? (Size - service_offset) : 0;
        if (service_len > 255) service_len = 255;
        if (service_len > 0) {
            memcpy(service, Data + service_offset, service_len);
            service[service_len] = '\0';
        } else {
            strcpy(service, "80");
        }
        
        // Prepare hints
        struct ares_addrinfo_hints hints;
        memset(&hints, 0, sizeof(hints));
        
        if (Size > 5) {
            hints.ai_flags = Data[5] & 0xFF;
        }
        if (Size > 6) {
            hints.ai_family = Data[6] & 0x03;
        }
        if (Size > 7) {
            hints.ai_socktype = Data[7] & 0x03;
        }
        if (Size > 8) {
            hints.ai_protocol = Data[8] & 0xFF;
        }
        
        // Call getaddrinfo with dummy callback
        ares_getaddrinfo(channel, node, service, &hints, dummy_callback, NULL);
        
        // Process any pending events to allow callback to complete
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        
        fd_set read_fds, write_fds;
        int nfds;
        
        // Process once to allow immediate completions
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        nfds = ares_fds(channel, &read_fds, &write_fds);
        if (nfds > 0) {
            ares_process(channel, &read_fds, &write_fds);
        }
        
        // Destroy channel
        ares_destroy(channel);
    }
    
    // Cleanup library
    ares_library_cleanup();
    
    return 0;
}
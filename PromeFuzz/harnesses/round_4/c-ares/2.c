// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_getaddrinfo at ares_getaddrinfo.c:695:6 in ares.h
// ares_process_fd at ares_process.c:267:6 in ares.h
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
#include <stdio.h>

static void dummy_addrinfo_callback(void *arg, int status, int timeouts,
                                    struct ares_addrinfo *result) {
    // Free the result if it exists
    if (result != NULL) {
        ares_freeaddrinfo(result);
    }
    (void)arg;
    (void)status;
    (void)timeouts;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Initialize library
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // First ares_strerror call with arbitrary error code
    int error_code = 0;
    if (Size > 0) {
        error_code = Data[0];
    }
    const char *error_str1 = ares_strerror(error_code);
    (void)error_str1;  // Mark as used
    
    // Initialize channel with options
    ares_channel_t *channel = NULL;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    
    int optmask = 0;
    
    // Use fuzzer data to set some options if we have enough data
    if (Size >= sizeof(int)) {
        options.timeout = *(const int*)Data % 5000;  // Reasonable timeout
        optmask |= ARES_OPT_TIMEOUTMS;
    }
    
    if (Size >= 2 * sizeof(int)) {
        options.tries = *(const int*)(Data + sizeof(int)) % 5;
        optmask |= ARES_OPT_TRIES;
    }
    
    int init_result = ares_init_options(&channel, &options, optmask);
    
    // Second ares_strerror call with init result
    const char *error_str2 = ares_strerror(init_result);
    (void)error_str2;  // Mark as used
    
    if (channel != NULL) {
        // Get servers CSV
        char *servers_csv = ares_get_servers_csv(channel);
        if (servers_csv != NULL) {
            ares_free_string(servers_csv);
        }
        
        // Prepare node and service strings for getaddrinfo
        const char *node = NULL;
        const char *service = NULL;
        
        // Create dummy strings from fuzzer data if available
        static char node_buf[256];
        static char service_buf[256];
        
        if (Size > 0) {
            size_t node_len = Size > 256 ? 256 : Size;
            memcpy(node_buf, Data, node_len);
            node_buf[node_len - 1] = '\0';
            node = node_buf;
            
            // Use remaining data for service if available
            if (Size > 256) {
                size_t service_len = Size - 256 > 256 ? 256 : Size - 256;
                memcpy(service_buf, Data + 256, service_len);
                service_buf[service_len - 1] = '\0';
                service = service_buf;
            } else {
                service = "http";  // Default service
            }
        } else {
            node = "localhost";
            service = "http";
        }
        
        // Call getaddrinfo with dummy hints
        struct ares_addrinfo_hints hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        
        ares_getaddrinfo(channel, node, service, &hints, 
                        dummy_addrinfo_callback, NULL);
        
        // Process any pending callbacks by doing a simple timeout check
        // This helps ensure callbacks are called and memory is freed
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 1000;  // 1ms timeout
        
        // Process any immediate responses
        ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
        
        // Destroy channel
        ares_destroy(channel);
    }
    
    // Cleanup library
    ares_library_cleanup();
    
    return 0;
}
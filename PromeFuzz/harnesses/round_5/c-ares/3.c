// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
// ares_library_init at ares_library_init.c:108:5 in ares.h
// ares_strerror at ares_strerror.c:30:13 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_strerror at ares_strerror.c:30:13 in ares.h
// ares_get_servers_csv at ares_update_servers.c:1314:7 in ares.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_getaddrinfo at ares_getaddrinfo.c:695:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_addrinfo_callback(void* arg, int status, int timeouts,
                                    struct ares_addrinfo* result) {
    if (result) {
        ares_freeaddrinfo(result);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t* Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Step 1: Initialize library
    ares_library_init(0);

    // Step 2: First call to ares_strerror
    int error_code1 = (int)Data[0];
    const char* error_str1 = ares_strerror(error_code1);

    // Step 3: Initialize channel with options
    ares_channel_t* channel = NULL;
    struct ares_options options;
    int optmask = 0;

    // Initialize options with some values
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    options.timeout = 1000;  // 1 second
    options.tries = 2;
    optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;

    int init_result = ares_init_options(&channel, &options, optmask);
    
    // Step 4: Second call to ares_strerror
    int error_code2 = (int)(Data[0] + 1);
    const char* error_str2 = ares_strerror(error_code2);

    if (init_result == ARES_SUCCESS && channel != NULL) {
        // Step 5: Get servers CSV
        char* servers_csv = ares_get_servers_csv(channel);
        
        // Step 6: Free the string
        if (servers_csv) {
            ares_free_string(servers_csv);
        }

        // Step 7: Call ares_getaddrinfo
        // Create node and service strings from fuzzer input
        char node[256];
        char service[256];
        size_t node_len = Size > 255 ? 255 : Size;
        size_t service_len = Size > 511 ? 255 : (Size > 256 ? Size - 256 : 0);
        
        if (node_len > 0) {
            memcpy(node, Data, node_len);
            node[node_len] = '\0';
        } else {
            strcpy(node, "example.com");
        }
        
        if (service_len > 0) {
            memcpy(service, Data + 256, service_len);
            service[service_len] = '\0';
        } else {
            strcpy(service, "80");
        }

        struct ares_addrinfo_hints hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = ARES_AI_CANONNAME;

        ares_getaddrinfo(channel, node, service, &hints,
                         dummy_addrinfo_callback, NULL);

        // Step 8: Destroy channel
        ares_destroy(channel);
    }

    // Step 9: Cleanup library
    ares_library_cleanup();

    return 0;
}
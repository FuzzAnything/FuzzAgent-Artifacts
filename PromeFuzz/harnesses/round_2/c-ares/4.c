// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_getaddrinfo at ares_getaddrinfo.c:695:6 in ares.h
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
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include "ares.h"

static void dummy_addrinfo_callback(void* arg, int status, int timeouts,
                                    struct ares_addrinfo* result) {
    (void)arg;
    (void)status;
    (void)timeouts;
    if (result) {
        ares_freeaddrinfo(result);
    }
}

int LLVMFuzzerTestOneInput_4(const uint8_t* Data, size_t Size) {
    // Initialize library first
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // First call to ares_strerror with random error code
    int error_code1 = 0;
    if (Size > 0) {
        error_code1 = Data[0] % 256;  // Use first byte for error code
    }
    const char* error_str1 = ares_strerror(error_code1);
    (void)error_str1;  // Prevent unused variable warning
    
    // Initialize channel with options
    ares_channel_t* channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize options structure to zero
    memset(&options, 0, sizeof(options));
    
    // Set some basic options from fuzz data if we have enough
    if (Size >= sizeof(int)) {
        options.flags = ARES_FLAG_STAYOPEN;
        optmask |= ARES_OPT_FLAGS;
        
        // Use some bytes for timeout if available
        if (Size >= 2 * sizeof(int)) {
            int timeout_val;
            memcpy(&timeout_val, Data, sizeof(int));
            options.timeout = timeout_val;
            optmask |= ARES_OPT_TIMEOUTMS;
            
            options.tries = 2;
            optmask |= ARES_OPT_TRIES;
        }
    }
    
    int init_result = ares_init_options(&channel, &options, optmask);
    
    // Second call to ares_strerror with different error code
    int error_code2 = 0;
    if (Size > 1) {
        error_code2 = Data[1] % 256;  // Use second byte for error code
    }
    const char* error_str2 = ares_strerror(error_code2);
    (void)error_str2;  // Prevent unused variable warning
    
    if (init_result == ARES_SUCCESS && channel) {
        // Get servers CSV
        char* servers_csv = ares_get_servers_csv(channel);
        if (servers_csv) {
            ares_free_string(servers_csv);
        }
        
        // Prepare data for getaddrinfo
        const char* node = NULL;
        const char* service = NULL;
        struct ares_addrinfo_hints hints;
        memset(&hints, 0, sizeof(hints));
        
        // Create temporary strings from fuzz data if we have enough
        if (Size > 2) {
            // Create a temporary file with the data for node
            int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd >= 0) {
                write(fd, Data, Size > 1024 ? 1024 : Size);
                close(fd);
                
                // Use part of data for node string
                size_t node_len = Size > 100 ? 100 : Size - 2;
                if (node_len > 0) {
                    char* node_buf = malloc(node_len + 1);
                    if (node_buf) {
                        memcpy(node_buf, Data + 2, node_len);
                        node_buf[node_len] = '\0';
                        node = node_buf;
                        
                        // Set some basic hints
                        hints.ai_family = AF_UNSPEC;
                        hints.ai_socktype = SOCK_STREAM;
                        
                        // Call getaddrinfo with dummy callback
                        ares_getaddrinfo(channel, node, service, &hints,
                                         dummy_addrinfo_callback, NULL);
                        
                        free((void*)node_buf);
                    }
                }
                
                unlink("./dummy_file");
            }
        }
        
        // Destroy the channel
        ares_destroy(channel);
    }
    
    // Clean up library
    ares_library_cleanup();
    
    return 0;
}
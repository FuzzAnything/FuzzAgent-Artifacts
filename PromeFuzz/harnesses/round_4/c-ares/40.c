// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_set_sortlist at ares_init.c:581:5 in ares.h
// ares_set_servers_csv at ares_update_servers.c:1304:5 in ares.h
// ares_set_servers_ports_csv at ares_update_servers.c:1309:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_set_sortlist at ares_init.c:581:5 in ares.h
// ares_set_servers_csv at ares_update_servers.c:1304:5 in ares.h
// ares_set_servers_ports_csv at ares_update_servers.c:1309:5 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_set_sortlist at ares_init.c:581:5 in ares.h
// ares_set_servers_csv at ares_update_servers.c:1304:5 in ares.h
// ares_set_servers_ports_csv at ares_update_servers.c:1309:5 in ares.h
// ares_set_servers_ports at ares_update_servers.c:1245:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <unistd.h>
#include <fcntl.h>

static void dummy_socket_cb(void *socket_data, ares_socket_t socket_fd,
                            int readable, int writable) {
    // Empty callback for fuzzing
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    // Create dummy file if needed
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            size_t write_size = Size > 1024 ? 1024 : Size;
            write(fd, Data, write_size);
        }
        close(fd);
    }

    // Initialize channel
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    // Try to initialize with various options based on fuzzer input
    memset(&options, 0, sizeof(options));
    
    // Use first few bytes to set some options
    if (Size >= sizeof(int)) {
        options.flags = *(const int*)Data;
        optmask |= ARES_OPT_FLAGS;
    }
    if (Size >= 2 * sizeof(int)) {
        options.timeout = *(const int*)(Data + sizeof(int));
        optmask |= ARES_OPT_TIMEOUT;
    }
    if (Size >= 3 * sizeof(int)) {
        options.tries = *(const int*)(Data + 2 * sizeof(int));
        optmask |= ARES_OPT_TRIES;
    }
    
    // Set socket state callback
    options.sock_state_cb = dummy_socket_cb;
    optmask |= ARES_OPT_SOCK_STATE_CB;
    
    // Initialize channel
    int init_result = ares_init_options(&channel, &options, optmask);
    
    if (init_result == ARES_SUCCESS && channel != NULL) {
        // Test ares_set_sortlist
        if (Size > 0) {
            // Create null-terminated string from fuzzer data
            char *sort_str = (char*)malloc(Size + 1);
            if (sort_str) {
                memcpy(sort_str, Data, Size);
                sort_str[Size] = '\0';
                ares_set_sortlist(channel, sort_str);
                free(sort_str);
            }
        }
        
        // Test ares_set_servers_csv and ares_set_servers_ports_csv
        if (Size > 0) {
            // Create null-terminated string for CSV functions
            char *csv_str = (char*)malloc(Size + 1);
            if (csv_str) {
                memcpy(csv_str, Data, Size);
                csv_str[Size] = '\0';
                
                // Test both CSV functions
                ares_set_servers_csv(channel, csv_str);
                ares_set_servers_ports_csv(channel, csv_str);
                
                free(csv_str);
            }
        }
        
        // Test ares_set_servers_ports (deprecated but still testable)
        if (Size >= sizeof(struct ares_addr_port_node)) {
            struct ares_addr_port_node server_node;
            memset(&server_node, 0, sizeof(server_node));
            
            // Copy fuzzer data into server node structure
            size_t copy_size = Size > sizeof(server_node) ? sizeof(server_node) : Size;
            memcpy(&server_node, Data, copy_size);
            
            // Ensure reasonable values
            server_node.family = AF_INET;
            server_node.next = NULL;
            server_node.udp_port = 53;
            server_node.tcp_port = 53;
            
            ares_set_servers_ports(channel, &server_node);
        }
        
        // Clean up
        ares_destroy(channel);
    }
    
    // Try another initialization with different parameters
    if (Size > 0) {
        ares_channel_t *channel2 = NULL;
        struct ares_options options2;
        memset(&options2, 0, sizeof(options2));
        
        // Use different parts of fuzzer data for options
        int optmask2 = 0;
        if (Size >= sizeof(int)) {
            options2.ndots = *(const int*)Data % 10;  // Reasonable ndots value
            optmask2 |= ARES_OPT_NDOTS;
        }
        if (Size >= 2 * sizeof(int)) {
            options2.udp_port = (*(const int*)(Data + sizeof(int)) % 65535) + 1;
            options2.tcp_port = (*(const int*)(Data + sizeof(int)) % 65535) + 1;
            optmask2 |= ARES_OPT_UDP_PORT | ARES_OPT_TCP_PORT;
        }
        
        int init_result2 = ares_init_options(&channel2, &options2, optmask2);
        if (init_result2 == ARES_SUCCESS && channel2 != NULL) {
            // Test with empty strings
            ares_set_sortlist(channel2, "");
            ares_set_servers_csv(channel2, "");
            ares_set_servers_ports_csv(channel2, "");
            
            ares_destroy(channel2);
        }
    }
    
    // Test with NULL channel (should handle gracefully)
    if (Size > 0) {
        char *test_str = (char*)malloc(Size + 1);
        if (test_str) {
            memcpy(test_str, Data, Size);
            test_str[Size] = '\0';
            
            // These should handle NULL channel (though behavior is undefined)
            ares_set_sortlist(NULL, test_str);
            ares_set_servers_csv(NULL, test_str);
            ares_set_servers_ports_csv(NULL, test_str);
            
            free(test_str);
        }
    }
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}
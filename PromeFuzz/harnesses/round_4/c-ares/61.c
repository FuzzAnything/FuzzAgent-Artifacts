// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init at ares_init.c:67:5 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_save_options at ares_options.c:83:5 in ares.h
// ares_set_servers_csv at ares_update_servers.c:1304:5 in ares.h
// ares_get_servers_ports at ares_update_servers.c:1164:5 in ares.h
// ares_set_servers_ports at ares_update_servers.c:1245:5 in ares.h
// ares_free_data at ares_data.c:46:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_save_options at ares_options.c:83:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

static void dummy_sock_state_cb(void *data, ares_socket_t socket_fd, int readable, int writable) {
    (void)data;
    (void)socket_fd;
    (void)readable;
    (void)writable;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    // Initialize a channel first
    ares_channel_t *channel = NULL;
    int status;
    
    // Try to initialize with default settings using ares_init (deprecated)
    status = ares_init(&channel);
    if (status != ARES_SUCCESS) {
        // If ares_init fails, try ares_init_options with empty options
        struct ares_options options;
        int optmask = 0;
        status = ares_init_options(&channel, &options, optmask);
        if (status != ARES_SUCCESS) {
            return 0; // Can't proceed without a channel
        }
    }
    
    // Test ares_save_options
    struct ares_options saved_options;
    int saved_optmask;
    ares_save_options(channel, &saved_options, &saved_optmask);
    
    // Test ares_set_servers_csv if we have enough data
    if (Size > 0) {
        // Create a null-terminated string from fuzzer input
        char *servers_csv = (char*)malloc(Size + 1);
        if (servers_csv) {
            memcpy(servers_csv, Data, Size);
            servers_csv[Size] = '\0';
            ares_set_servers_csv(channel, servers_csv);
            free(servers_csv);
        }
    }
    
    // Test ares_get_servers_ports and ares_set_servers_ports
    struct ares_addr_port_node *servers = NULL;
    ares_get_servers_ports(channel, &servers);
    
    if (servers) {
        // Try to set the servers back
        ares_set_servers_ports(channel, servers);
        
        // Free the server list using ares_free_data as documented
        ares_free_data(servers);
    }
    
    // Create another channel with options from saved_options
    if (saved_optmask != 0) {
        ares_channel_t *channel2 = NULL;
        
        // Set a callback to avoid crashes
        if (saved_optmask & ARES_OPT_SOCK_STATE_CB) {
            saved_options.sock_state_cb = dummy_sock_state_cb;
            saved_options.sock_state_cb_data = NULL;
        }
        
        // Initialize with saved options
        ares_init_options(&channel2, &saved_options, saved_optmask);
        
        if (channel2) {
            // Test ares_save_options on the new channel
            struct ares_options saved_options2;
            int saved_optmask2;
            ares_save_options(channel2, &saved_options2, &saved_optmask2);
            
            // Clean up the second channel
            ares_destroy(channel2);
        }
    }
    
    // Clean up
    ares_destroy(channel);
    
    return 0;
}
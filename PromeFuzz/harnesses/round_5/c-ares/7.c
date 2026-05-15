// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_save_options at ares_options.c:83:5 in ares.h
// ares_destroy_options at ares_options.c:37:6 in ares.h
// ares_save_options at ares_options.c:83:5 in ares.h
// ares_destroy_options at ares_options.c:37:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_set_servers_csv at ares_update_servers.c:1304:5 in ares.h
// ares_set_sortlist at ares_init.c:581:5 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
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

static void dummy_sock_state_cb(void *data, ares_socket_t socket_fd, int readable, int writable) {
    (void)data;
    (void)socket_fd;
    (void)readable;
    (void)writable;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Use the input data to generate strings for CSV servers and sortlist
    // We'll use the entire input for both, but ensure null termination
    char *servers_csv = NULL;
    char *sortlist_str = NULL;
    
    if (Size > 0) {
        // Allocate buffers with null termination
        servers_csv = (char*)malloc(Size + 1);
        sortlist_str = (char*)malloc(Size + 1);
        if (!servers_csv || !sortlist_str) {
            free(servers_csv);
            free(sortlist_str);
            return 0;
        }
        
        // Copy data and null terminate
        memcpy(servers_csv, Data, Size);
        servers_csv[Size] = '\0';
        
        memcpy(sortlist_str, Data, Size);
        sortlist_str[Size] = '\0';
    } else {
        // Use empty strings if no data
        servers_csv = strdup("");
        sortlist_str = strdup("");
        if (!servers_csv || !sortlist_str) {
            free(servers_csv);
            free(sortlist_str);
            return 0;
        }
    }
    
    // First ares_init_options
    ares_channel_t *channel1 = NULL;
    struct ares_options options1;
    int optmask1 = 0;
    
    // Set some options to test different code paths
    options1.flags = ARES_FLAG_USEVC | ARES_FLAG_PRIMARY;
    optmask1 |= ARES_OPT_FLAGS;
    
    options1.timeout = 2000;
    optmask1 |= ARES_OPT_TIMEOUTMS;
    
    options1.tries = 3;
    optmask1 |= ARES_OPT_TRIES;
    
    options1.sock_state_cb = dummy_sock_state_cb;
    options1.sock_state_cb_data = NULL;
    optmask1 |= ARES_OPT_SOCK_STATE_CB;
    
    int init_result1 = ares_init_options(&channel1, &options1, optmask1);
    
    // First ares_destroy (even if init failed, destroy is safe on NULL)
    ares_destroy(channel1);
    
    // Second ares_init_options with different options
    ares_channel_t *channel2 = NULL;
    struct ares_options options2;
    int optmask2 = 0;
    
    // Try with minimal options
    options2.flags = 0;
    optmask2 |= ARES_OPT_FLAGS;
    
    options2.timeout = 1000;
    optmask2 |= ARES_OPT_TIMEOUTMS;
    
    int init_result2 = ares_init_options(&channel2, &options2, optmask2);
    
    if (channel2) {
        // ares_set_servers_csv
        ares_set_servers_csv(channel2, servers_csv);
        
        // ares_set_sortlist
        ares_set_sortlist(channel2, sortlist_str);
        
        // ares_dup
        ares_channel_t *channel_dup = NULL;
        ares_dup(&channel_dup, channel2);
        
        // ares_save_options
        struct ares_options saved_options;
        int saved_optmask = 0;
        if (channel_dup) {
            ares_save_options(channel_dup, &saved_options, &saved_optmask);
            
            // First ares_destroy_options
            ares_destroy_options(&saved_options);
            
            // Second ares_destroy_options (on already freed, but function should handle)
            // We'll save options again to have valid data
            struct ares_options saved_options2;
            int saved_optmask2 = 0;
            ares_save_options(channel_dup, &saved_options2, &saved_optmask2);
            ares_destroy_options(&saved_options2);
            
            // Destroy duplicated channel
            ares_destroy(channel_dup);
        }
        
        // Destroy original channel
        ares_destroy(channel2);
    }
    
    // Clean up allocated strings
    free(servers_csv);
    free(sortlist_str);
    
    return 0;
}
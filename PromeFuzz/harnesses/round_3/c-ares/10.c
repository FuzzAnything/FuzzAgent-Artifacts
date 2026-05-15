// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_set_sortlist at ares_init.c:581:5 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_save_options at ares_options.c:83:5 in ares.h
// ares_destroy_options at ares_options.c:37:6 in ares.h
// ares_destroy_options at ares_options.c:37:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy_options at ares_options.c:37:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_set_servers_csv at ares_update_servers.c:1304:5 in ares.h
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

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Use first byte to determine optmask
    int optmask = Data[0] & 0xFF;
    
    // Use second byte to determine flags
    int flags = Data[1] & 0xFF;
    
    // Use third byte for various numeric options
    int numeric_val = Data[2] & 0xFF;
    
    // Use fourth byte for another numeric option
    int numeric_val2 = Data[3] & 0xFF;
    
    // Prepare options struct
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    
    // Set some options based on fuzzer input
    options.flags = flags;
    options.timeout = numeric_val;
    options.tries = numeric_val2;
    options.ndots = numeric_val % 4;
    options.udp_port = 53;
    options.tcp_port = 53;
    
    // Set server failover options
    options.server_failover_opts.retry_chance = numeric_val % 10;
    options.server_failover_opts.retry_delay = numeric_val2 * 100;
    
    // Set sock_state_cb
    options.sock_state_cb = dummy_sock_state_cb;
    options.sock_state_cb_data = NULL;
    
    // First ares_init_options
    ares_channel_t *channel1 = NULL;
    int result1 = ares_init_options(&channel1, &options, optmask);
    
    // First ares_destroy
    if (channel1) {
        ares_destroy(channel1);
        channel1 = NULL;
    }
    
    // Second ares_init_options with different options
    struct ares_options options2;
    memset(&options2, 0, sizeof(options2));
    options2.flags = flags ^ 0xFF;  // Different flags
    options2.timeout = numeric_val2;
    options2.tries = numeric_val;
    options2.ednspsz = numeric_val % 1500;
    
    ares_channel_t *channel2 = NULL;
    int result2 = ares_init_options(&channel2, &options2, optmask ^ 0xFF);
    
    if (channel2) {
        // Prepare CSV servers string from fuzzer data
        char *servers_csv = NULL;
        if (Size > 4) {
            size_t csv_len = Size - 4;
            if (csv_len > 0) {
                servers_csv = (char*)malloc(csv_len + 1);
                if (servers_csv) {
                    memcpy(servers_csv, Data + 4, csv_len);
                    servers_csv[csv_len] = '\0';
                    
                    // Replace non-printable characters with commas for CSV format
                    for (size_t i = 0; i < csv_len; i++) {
                        if (servers_csv[i] < 32 || servers_csv[i] > 126) {
                            servers_csv[i] = ',';
                        }
                    }
                    
                    // ares_set_servers_csv
                    ares_set_servers_csv(channel2, servers_csv);
                    
                    free(servers_csv);
                }
            }
        }
        
        // Prepare sortlist string from fuzzer data
        char *sortlist_str = NULL;
        if (Size > 8) {
            size_t sort_len = (Size - 8) % 256;  // Limit length
            if (sort_len > 0) {
                sortlist_str = (char*)malloc(sort_len + 1);
                if (sortlist_str) {
                    memcpy(sortlist_str, Data + 8, sort_len);
                    sortlist_str[sort_len] = '\0';
                    
                    // Replace non-printable characters with dots/numbers
                    for (size_t i = 0; i < sort_len; i++) {
                        if (sortlist_str[i] < 32 || sortlist_str[i] > 126) {
                            sortlist_str[i] = '.';
                        }
                    }
                    
                    // ares_set_sortlist
                    ares_set_sortlist(channel2, sortlist_str);
                    
                    free(sortlist_str);
                }
            }
        }
        
        // ares_dup
        ares_channel_t *channel3 = NULL;
        int dup_result = ares_dup(&channel3, channel2);
        
        if (channel3) {
            // ares_save_options
            struct ares_options saved_options;
            int saved_optmask = 0;
            int save_result = ares_save_options(channel3, &saved_options, &saved_optmask);
            
            // First ares_destroy_options (on saved options)
            ares_destroy_options(&saved_options);
            
            // Second ares_destroy_options (on options2)
            ares_destroy_options(&options2);
            
            // Destroy duplicated channel
            ares_destroy(channel3);
        }
        
        // Destroy original channel
        ares_destroy(channel2);
    }
    
    // Destroy first options struct
    ares_destroy_options(&options);
    
    return 0;
}
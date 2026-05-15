// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_set_servers_csv at ares_update_servers.c:1304:5 in ares.h
// ares_set_servers_csv at ares_update_servers.c:1304:5 in ares.h
// ares_set_sortlist at ares_init.c:581:5 in ares.h
// ares_set_sortlist at ares_init.c:581:5 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy_options at ares_options.c:37:6 in ares.h
// ares_save_options at ares_options.c:83:5 in ares.h
// ares_destroy_options at ares_options.c:37:6 in ares.h
// ares_destroy_options at ares_options.c:37:6 in ares.h
// ares_destroy_options at ares_options.c:37:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_destroy_options at ares_options.c:37:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_destroy_options at ares_options.c:37:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

#ifdef __cplusplus
extern "C" {
#endif

static void dummy_sock_state_cb(void *data, ares_socket_t socket_fd,
                                int readable, int writable) {}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    ares_channel_t *channel1 = NULL;
    ares_channel_t *channel2 = NULL;
    struct ares_options options1;
    struct ares_options options2;
    int optmask1;
    int optmask2;
    int result;
    
    // First ares_init_options
    memset(&options1, 0, sizeof(options1));
    optmask1 = 0;
    
    // Try to use fuzzer data to set some options if we have enough data
    if (Size >= sizeof(int)) {
        options1.timeout = *(int*)Data;
        optmask1 |= ARES_OPT_TIMEOUT;
    }
    if (Size >= 2 * sizeof(int)) {
        options1.tries = *(int*)(Data + sizeof(int));
        optmask1 |= ARES_OPT_TRIES;
    }
    
    result = ares_init_options(&channel1, &options1, optmask1);
    if (result != ARES_SUCCESS) {
        // Clean up options if needed
        if (optmask1 != 0) {
            ares_destroy_options(&options1);
        }
        return 0;
    }
    
    // ares_destroy
    ares_destroy(channel1);
    
    // Second ares_init_options
    memset(&options2, 0, sizeof(options2));
    optmask2 = 0;
    
    // Use different parts of fuzzer data for options
    size_t offset = 0;
    if (Size >= offset + sizeof(int)) {
        options2.flags = *(int*)(Data + offset);
        optmask2 |= ARES_OPT_FLAGS;
        offset += sizeof(int);
    }
    if (Size >= offset + sizeof(int)) {
        options2.ndots = *(int*)(Data + offset);
        optmask2 |= ARES_OPT_NDOTS;
        offset += sizeof(int);
    }
    if (Size >= offset + sizeof(int)) {
        options2.socket_send_buffer_size = *(int*)(Data + offset);
        optmask2 |= ARES_OPT_SOCK_SNDBUF;
        offset += sizeof(int);
    }
    if (Size >= offset + sizeof(int)) {
        options2.socket_receive_buffer_size = *(int*)(Data + offset);
        optmask2 |= ARES_OPT_SOCK_RCVBUF;
        offset += sizeof(int);
    }
    
    // Set socket state callback if we have room
    if (Size >= offset + sizeof(void*)) {
        options2.sock_state_cb = dummy_sock_state_cb;
        optmask2 |= ARES_OPT_SOCK_STATE_CB;
        offset += sizeof(void*);
    }
    
    result = ares_init_options(&channel1, &options2, optmask2);
    if (result != ARES_SUCCESS) {
        // Clean up options if needed
        if (optmask2 != 0) {
            ares_destroy_options(&options2);
        }
        return 0;
    }
    
    // ares_set_servers_csv - use fuzzer data as CSV string
    if (Size > 0) {
        // Create null-terminated string from fuzzer data
        char *servers_csv = malloc(Size + 1);
        if (servers_csv) {
            memcpy(servers_csv, Data, Size);
            servers_csv[Size] = '\0';
            ares_set_servers_csv(channel1, servers_csv);
            free(servers_csv);
        }
    } else {
        // Use empty string
        ares_set_servers_csv(channel1, "");
    }
    
    // ares_set_sortlist - use fuzzer data as sortlist string
    if (Size > 0) {
        // Create null-terminated string from fuzzer data
        char *sortstr = malloc(Size + 1);
        if (sortstr) {
            memcpy(sortstr, Data, Size);
            sortstr[Size] = '\0';
            ares_set_sortlist(channel1, sortstr);
            free(sortstr);
        }
    } else {
        // Use empty string
        ares_set_sortlist(channel1, "");
    }
    
    // ares_dup
    result = ares_dup(&channel2, channel1);
    if (result != ARES_SUCCESS) {
        // Clean up and return
        ares_destroy(channel1);
        if (optmask2 != 0) {
            ares_destroy_options(&options2);
        }
        return 0;
    }
    
    // ares_save_options
    struct ares_options saved_options;
    int saved_optmask;
    result = ares_save_options(channel2, &saved_options, &saved_optmask);
    
    // ares_destroy_options (first call)
    if (result == ARES_SUCCESS) {
        ares_destroy_options(&saved_options);
    }
    
    // ares_destroy_options (second call - on zeroed options)
    struct ares_options zero_options;
    memset(&zero_options, 0, sizeof(zero_options));
    ares_destroy_options(&zero_options);
    
    // Clean up original options
    if (optmask2 != 0) {
        ares_destroy_options(&options2);
    }
    
    // Final ares_destroy
    ares_destroy(channel1);
    ares_destroy(channel2);
    
    return 0;
}

#ifdef __cplusplus
}
#endif
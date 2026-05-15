// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_destroy_options at ares_options.c:37:6 in ares.h
// ares_destroy_options at ares_options.c:37:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_set_servers_csv at ares_update_servers.c:1304:5 in ares.h
// ares_set_sortlist at ares_init.c:581:5 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_save_options at ares_options.c:83:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

static void dummy_sock_state_cb(void *data, ares_socket_t socket_fd, int readable, int writable) {
    // Empty callback
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create null-terminated strings from fuzzer input
    char *input_str = malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    ares_channel_t *channel1 = NULL;
    ares_channel_t *channel2 = NULL;
    ares_channel_t *channel_dup = NULL;
    struct ares_options options;
    int optmask = 0;
    int result;

    // 1. First ares_init_options
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_USEVC;
    options.timeout = 2000;
    options.tries = 3;
    optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    result = ares_init_options(&channel1, &options, optmask);
    if (result != ARES_SUCCESS) {
        free(input_str);
        return 0;
    }

    // 2. First ares_destroy
    ares_destroy(channel1);
    channel1 = NULL;

    // 3. Second ares_init_options (with different options)
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_PRIMARY;
    options.timeout = 1000;
    options.tries = 2;
    options.sock_state_cb = dummy_sock_state_cb;
    options.sock_state_cb_data = NULL;
    optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES | ARES_OPT_SOCK_STATE_CB;
    
    result = ares_init_options(&channel2, &options, optmask);
    if (result != ARES_SUCCESS) {
        free(input_str);
        return 0;
    }

    // 4. ares_set_servers_csv - use first half of input as server string
    size_t half_size = Size / 2;
    if (half_size > 0) {
        char *server_str = malloc(half_size + 1);
        if (server_str) {
            memcpy(server_str, Data, half_size);
            server_str[half_size] = '\0';
            ares_set_servers_csv(channel2, server_str);
            free(server_str);
        }
    }

    // 5. ares_set_sortlist - use second half of input as sortlist string
    if (Size - half_size > 0) {
        char *sort_str = malloc(Size - half_size + 1);
        if (sort_str) {
            memcpy(sort_str, Data + half_size, Size - half_size);
            sort_str[Size - half_size] = '\0';
            ares_set_sortlist(channel2, sort_str);
            free(sort_str);
        }
    }

    // 6. ares_dup
    result = ares_dup(&channel_dup, channel2);
    if (result != ARES_SUCCESS) {
        ares_destroy(channel2);
        free(input_str);
        return 0;
    }

    // 7. ares_save_options
    struct ares_options saved_options;
    int saved_optmask = 0;
    memset(&saved_options, 0, sizeof(saved_options));
    result = ares_save_options(channel_dup, &saved_options, &saved_optmask);
    
    if (result == ARES_SUCCESS) {
        // 8. First ares_destroy_options
        ares_destroy_options(&saved_options);
        
        // 9. Second ares_destroy_options (with nulled structure)
        memset(&saved_options, 0, sizeof(saved_options));
        ares_destroy_options(&saved_options);
    }

    // 10. ares_destroy (destroy both channels)
    ares_destroy(channel_dup);
    ares_destroy(channel2);

    free(input_str);
    return 0;
}
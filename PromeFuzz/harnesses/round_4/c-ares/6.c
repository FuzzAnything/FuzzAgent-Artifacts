// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_set_servers_csv at ares_update_servers.c:1304:5 in ares.h
// ares_set_sortlist at ares_init.c:581:5 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_save_options at ares_options.c:83:5 in ares.h
// ares_destroy_options at ares_options.c:37:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string from fuzzer input for CSV and sortlist functions
    char *input_str = NULL;
    if (Size > 0) {
        input_str = (char *)malloc(Size + 1);
        if (!input_str) return 0;
        memcpy(input_str, Data, Size);
        input_str[Size] = '\0';
    } else {
        // If input is empty, use a minimal valid string
        input_str = strdup("8.8.8.8");
        if (!input_str) return 0;
    }

    ares_channel_t *channel = NULL;
    ares_channel_t *dup_channel = NULL;
    struct ares_options options;
    int optmask = 0;
    int result;

    // First ares_init_options
    result = ares_init_options(&channel, NULL, 0);
    if (result != ARES_SUCCESS) {
        free(input_str);
        return 0;
    }

    // First ares_destroy
    ares_destroy(channel);
    channel = NULL;

    // Second ares_init_options
    result = ares_init_options(&channel, NULL, 0);
    if (result != ARES_SUCCESS) {
        free(input_str);
        return 0;
    }

    // ares_set_servers_csv
    ares_set_servers_csv(channel, input_str);

    // ares_set_sortlist
    ares_set_sortlist(channel, input_str);

    // ares_dup
    result = ares_dup(&dup_channel, channel);
    if (result != ARES_SUCCESS) {
        dup_channel = NULL;
    }

    // ares_save_options
    if (dup_channel) {
        // Clear the options struct before saving
        memset(&options, 0, sizeof(options));
        optmask = 0;
        ares_save_options(dup_channel, &options, &optmask);
    }

    // First ares_destroy_options - only call if we have valid options
    if (dup_channel && optmask != 0) {
        ares_destroy_options(&options);
        // Clear the options struct after destroying to prevent double-free
        memset(&options, 0, sizeof(options));
        optmask = 0;
    }

    // Second ares_destroy_options - only call if we have a valid options struct
    // But we already cleared it, so we should not call it again
    // ares_destroy_options(&options); // Removed to avoid double-free

    // Cleanup
    if (dup_channel) {
        ares_destroy(dup_channel);
    }
    if (channel) {
        ares_destroy(channel);
    }
    free(input_str);

    return 0;
}
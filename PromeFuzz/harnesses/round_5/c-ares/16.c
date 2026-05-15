// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init at ares_init.c:67:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_expand_name at ares_expand_name.c:84:5 in ares.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_expand_string at ares_expand_string.c:93:5 in ares.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_fds at ares_fds.c:30:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "ares.h"
#include <sys/select.h>

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Prepare dummy file if needed
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // 1. Prepare for ares_destroy - create a channel first
    ares_channel_t *channel = NULL;
    ares_init(&channel);
    
    // Call ares_destroy (even if channel is NULL)
    ares_destroy(channel);
    
    // 2. Prepare for ares_expand_name
    const unsigned char *encoded = Data;
    const unsigned char *abuf = Data;
    int alen = (Size > INT_MAX) ? INT_MAX : (int)Size;
    char *expanded_name = NULL;
    long name_enclen = 0;
    
    // Call ares_expand_name
    int name_result = ares_expand_name(encoded, abuf, alen, &expanded_name, &name_enclen);
    if (name_result == ARES_SUCCESS && expanded_name != NULL) {
        // Free the expanded string if allocation succeeded
        ares_free_string(expanded_name);
    }
    
    // 3. Prepare for ares_expand_string
    unsigned char *expanded_string = NULL;
    long string_enclen = 0;
    
    // Call ares_expand_string
    int string_result = ares_expand_string(encoded, abuf, alen, &expanded_string, &string_enclen);
    if (string_result == ARES_SUCCESS && expanded_string != NULL) {
        // Free the expanded string if allocation succeeded
        ares_free_string(expanded_string);
    }
    
    // 4. Prepare for ares_fds - create another channel since we destroyed the first one
    ares_channel_t *channel2 = NULL;
    ares_init(&channel2);
    
    if (channel2 != NULL) {
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        // Call ares_fds
        int fds_result = ares_fds(channel2, &read_fds, &write_fds);
        (void)fds_result; // Use result to avoid unused variable warning
        
        // Clean up the channel
        ares_destroy(channel2);
    }
    
    // Clean up dummy file
    remove(dummy_file);
    
    return 0;
}
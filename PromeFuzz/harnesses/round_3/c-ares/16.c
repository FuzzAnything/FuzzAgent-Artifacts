// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init at ares_init.c:67:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_expand_name at ares_expand_name.c:84:5 in ares.h
// ares_expand_string at ares_expand_string.c:93:5 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_fds at ares_fds.c:30:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <sys/select.h>

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Create a dummy channel for ares_destroy and ares_fds
    ares_channel_t *channel = NULL;
    int status = ares_init(&channel);
    if (status != ARES_SUCCESS) {
        // If channel initialization fails, we can't proceed with ares_destroy
        // but we can still test other functions
        channel = NULL;
    }
    
    // 1. Test ares_destroy
    if (channel != NULL) {
        ares_destroy(channel);
        // Note: channel is now invalid after destroy
    }
    
    // Prepare buffers for expansion functions
    unsigned char *encoded = NULL;
    char *expanded_name = NULL;
    unsigned char *expanded_string = NULL;
    long enclen = 0;
    
    // Allocate buffer from fuzzer input for encoded data
    if (Size > 0) {
        encoded = (unsigned char *)malloc(Size);
        if (encoded != NULL) {
            memcpy(encoded, Data, Size);
            
            // 2. Test ares_expand_name
            // Use the same buffer for both encoded and abuf
            ares_expand_name(encoded, encoded, (int)Size, &expanded_name, &enclen);
            
            // Free expanded name if allocated
            if (expanded_name != NULL) {
                free(expanded_name);
                expanded_name = NULL;
            }
            
            // Reset enclen for next function
            enclen = 0;
            
            // 3. Test ares_expand_string
            ares_expand_string(encoded, encoded, (int)Size, &expanded_string, &enclen);
            
            // Free expanded string if allocated
            if (expanded_string != NULL) {
                free(expanded_string);
                expanded_string = NULL;
            }
            
            free(encoded);
        }
    }
    
    // Create another channel for ares_fds since the first one was destroyed
    ares_channel_t *channel2 = NULL;
    status = ares_init(&channel2);
    if (status == ARES_SUCCESS && channel2 != NULL) {
        // 4. Test ares_fds
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        ares_fds(channel2, &read_fds, &write_fds);
        
        // Clean up the channel
        ares_destroy(channel2);
    }
    
    return 0;
}
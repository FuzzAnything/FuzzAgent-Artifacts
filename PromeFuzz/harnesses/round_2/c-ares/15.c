// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init at ares_init.c:67:5 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_expand_name at ares_expand_name.c:84:5 in ares.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_expand_name at ares_expand_name.c:84:5 in ares.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_expand_string at ares_expand_string.c:93:5 in ares.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_expand_string at ares_expand_string.c:93:5 in ares.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_fds at ares_fds.c:30:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include <limits.h>
#include "ares.h"

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // For ares_destroy and ares_fds, we need a valid channel
    ares_channel_t *channel = NULL;
    
    // Try to initialize a channel for testing
    int status = ares_init(&channel);
    if (status != ARES_SUCCESS) {
        // If init fails, we can't test channel functions
        return 0;
    }
    
    // 1. Call ares_destroy (but we need to keep channel for later tests)
    // We'll create a copy for destruction
    ares_channel_t *channel_copy = NULL;
    if (ares_dup(&channel_copy, channel) == ARES_SUCCESS) {
        ares_destroy(channel_copy);
    }
    
    // 2. Call ares_expand_name
    // Prepare parameters for ares_expand_name
    const unsigned char *encoded = (const unsigned char *)Data;
    const unsigned char *abuf = (const unsigned char *)Data;
    int alen = Size > INT_MAX ? INT_MAX : (int)Size;
    char *s = NULL;
    long enclen = 0;
    
    // Call with varying parameters based on fuzzer input
    if (Size > 0) {
        // Try with original parameters
        ares_expand_name(encoded, abuf, alen, &s, &enclen);
        
        // If s was allocated, free it
        if (s) {
            ares_free_string(s);
            s = NULL;
        }
        
        // Try with different pointer offsets if we have enough data
        if (Size > 1) {
            size_t offset = Data[0] % (Size - 1);
            encoded = (const unsigned char *)Data + offset;
            ares_expand_name(encoded, abuf, alen, &s, &enclen);
            if (s) {
                ares_free_string(s);
                s = NULL;
            }
        }
    }
    
    // 3. Call ares_expand_string
    unsigned char *s2 = NULL;
    long enclen2 = 0;
    
    if (Size > 0) {
        // Try with original parameters
        ares_expand_string(encoded, abuf, alen, &s2, &enclen2);
        
        // If s2 was allocated, free it
        if (s2) {
            ares_free_string(s2);
            s2 = NULL;
        }
        
        // Try with different pointer offsets if we have enough data
        if (Size > 1) {
            size_t offset = Data[0] % (Size - 1);
            encoded = (const unsigned char *)Data + offset;
            ares_expand_string(encoded, abuf, alen, &s2, &enclen2);
            if (s2) {
                ares_free_string(s2);
                s2 = NULL;
            }
        }
    }
    
    // 4. Call ares_fds
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    // Call ares_fds with the channel
    int fds_count = ares_fds(channel, &read_fds, &write_fds);
    
    // Clean up the main channel
    ares_destroy(channel);
    
    return 0;
}
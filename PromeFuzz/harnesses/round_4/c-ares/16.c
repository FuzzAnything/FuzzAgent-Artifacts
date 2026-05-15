// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_expand_name at ares_expand_name.c:84:5 in ares.h
// ares_expand_string at ares_expand_string.c:93:5 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_fds at ares_fds.c:30:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Create a dummy file if needed
    const char *dummy_file = "./dummy_file";
    int fd = open(dummy_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size);
        }
        close(fd);
    }
    
    // 1. Call ares_destroy with a NULL channel (safe according to docs)
    ares_destroy(NULL);
    
    // Try to create a real channel for more thorough testing
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize with default options
    if (ares_init_options(&channel, &options, optmask) == ARES_SUCCESS) {
        // Destroy the initialized channel
        ares_destroy(channel);
    }
    
    // 2. Call ares_expand_name
    if (Size > 0) {
        const unsigned char *encoded = Data;
        const unsigned char *abuf = Data;
        int alen = (Size > INT_MAX) ? INT_MAX : (int)Size;
        char *s = NULL;
        long enclen = 0;
        
        // Try with different offsets and lengths
        for (int offset = 0; offset < 4 && offset < alen; offset++) {
            int try_len = alen - offset;
            if (try_len > 0) {
                ares_expand_name(encoded + offset, abuf, try_len, &s, &enclen);
                if (s) {
                    free(s);
                    s = NULL;
                }
            }
        }
    }
    
    // 3. Call ares_expand_string
    if (Size > 0) {
        const unsigned char *encoded = Data;
        const unsigned char *abuf = Data;
        int alen = (Size > INT_MAX) ? INT_MAX : (int)Size;
        unsigned char *s = NULL;
        long enclen = 0;
        
        // Try with different offsets and lengths
        for (int offset = 0; offset < 4 && offset < alen; offset++) {
            int try_len = alen - offset;
            if (try_len > 0) {
                ares_expand_string(encoded + offset, abuf, try_len, &s, &enclen);
                if (s) {
                    free(s);
                    s = NULL;
                }
            }
        }
    }
    
    // 4. Call ares_fds
    // Create a new channel for ares_fds
    ares_channel_t *channel2 = NULL;
    if (ares_init_options(&channel2, &options, optmask) == ARES_SUCCESS) {
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        ares_fds(channel2, &read_fds, &write_fds);
        
        // Clean up
        ares_destroy(channel2);
    }
    
    // Clean up dummy file
    unlink(dummy_file);
    
    return 0;
}
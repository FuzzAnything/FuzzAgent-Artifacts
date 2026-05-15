// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init at ares_init.c:67:5 in ares.h
// ares_threadsafety at ares_threads.c:632:13 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_queue_wait_empty at ares_threads.c:768:15 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_queue_wait_empty at ares_threads.c:768:15 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

static int dummy_socket_callback(int sock, int type, void *data) {
    return ARES_SOCKET_BAD;
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    ares_channel_t *channel = NULL;
    ares_channel_t *duped_channel = NULL;
    int status;
    
    // Initialize a channel first for functions that require it
    if (ares_init(&channel) != ARES_SUCCESS) {
        return 0;
    }
    
    // Use fuzzing data to determine which operations to perform
    // We'll use the first byte as a selector
    uint8_t selector = 0;
    if (Size > 0) {
        selector = Data[0];
    }
    
    // Test ares_threadsafety - always safe to call
    ares_bool_t thread_safe = ares_threadsafety();
    
    // Test ares_dup based on selector bit 0
    if (selector & 0x01) {
        status = ares_dup(&duped_channel, channel);
        // If duplication succeeded, we have a second channel to test
        if (status == ARES_SUCCESS && duped_channel != NULL) {
            // Test ares_set_socket_callback on duplicated channel
            ares_set_socket_callback(duped_channel, dummy_socket_callback, NULL);
            
            // Test ares_reinit on duplicated channel
            ares_reinit(duped_channel);
            
            // Test ares_queue_wait_empty with different timeouts based on fuzzing data
            int timeout_ms = 0;
            if (Size > 1) {
                // Use second byte to determine timeout (-1, 0, or positive)
                timeout_ms = (int)Data[1];
                if (timeout_ms == 0) {
                    timeout_ms = -1;  // Test infinite wait
                }
            }
            ares_queue_wait_empty(duped_channel, timeout_ms);
            
            // Test ares_cancel on duplicated channel
            ares_cancel(duped_channel);
        }
    }
    
    // Test ares_set_socket_callback on original channel
    if (selector & 0x02) {
        ares_set_socket_callback(channel, dummy_socket_callback, NULL);
    }
    
    // Test ares_reinit on original channel
    if (selector & 0x04) {
        ares_reinit(channel);
    }
    
    // Test ares_queue_wait_empty on original channel
    if (selector & 0x08) {
        int timeout_ms = 0;
        if (Size > 2) {
            // Use third byte to determine timeout
            timeout_ms = (int)Data[2];
            if (timeout_ms == 0) {
                timeout_ms = -1;
            }
        }
        ares_queue_wait_empty(channel, timeout_ms);
    }
    
    // Test ares_cancel on original channel
    if (selector & 0x10) {
        ares_cancel(channel);
    }
    
    // Cleanup
    if (duped_channel != NULL) {
        ares_destroy(duped_channel);
    }
    if (channel != NULL) {
        ares_destroy(channel);
    }
    
    return 0;
}
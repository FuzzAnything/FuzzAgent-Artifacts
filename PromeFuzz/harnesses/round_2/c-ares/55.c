// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init at ares_init.c:67:5 in ares.h
// ares_threadsafety at ares_threads.c:632:13 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_queue_wait_empty at ares_threads.c:768:15 in ares.h
// ares_queue_wait_empty at ares_threads.c:768:15 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

static ares_socket_t dummy_socket_callback(int socket_fd, int type, void *data) {
    /* Return the same socket file descriptor */
    return socket_fd;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    ares_channel_t *channel = NULL;
    ares_channel_t *dup_channel = NULL;
    int result;
    
    /* Initialize a channel first to have something to work with */
    if (ares_init(&channel) != ARES_SUCCESS) {
        return 0;
    }
    
    /* Call ares_threadsafety - doesn't need any parameters */
    ares_bool_t thread_safe = ares_threadsafety();
    (void)thread_safe; /* Mark as used to avoid warnings */
    
    /* Call ares_dup if we have some data */
    if (Size > 0) {
        result = ares_dup(&dup_channel, channel);
        if (result == ARES_SUCCESS && dup_channel != NULL) {
            /* We successfully duplicated the channel, now test functions on both channels */
            
            /* Test ares_set_socket_callback on both channels */
            if (Size > 1) {
                ares_set_socket_callback(channel, dummy_socket_callback, (void*)Data);
                ares_set_socket_callback(dup_channel, dummy_socket_callback, (void*)(Data + 1));
            }
            
            /* Test ares_reinit on both channels */
            ares_reinit(channel);
            ares_reinit(dup_channel);
            
            /* Test ares_queue_wait_empty on both channels */
            if (Size > 2) {
                int timeout_ms = (int)(Data[0] % 256);
                if (timeout_ms == 0) timeout_ms = 1;
                ares_queue_wait_empty(channel, timeout_ms);
                ares_queue_wait_empty(dup_channel, -timeout_ms);
            }
            
            /* Test ares_cancel on both channels */
            ares_cancel(channel);
            ares_cancel(dup_channel);
            
            /* Clean up the duplicated channel */
            ares_destroy(dup_channel);
        }
    } else {
        /* Even with minimal input, test some functions on the original channel */
        ares_reinit(channel);
        ares_cancel(channel);
    }
    
    /* Clean up the original channel */
    ares_destroy(channel);
    
    return 0;
}
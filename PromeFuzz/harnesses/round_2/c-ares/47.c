// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init at ares_init.c:67:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_queue_wait_empty at ares_threads.c:768:15 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

static ares_socket_t dummy_socket_callback(int af, int type, void *data) {
    (void)af;
    (void)type;
    (void)data;
    return ARES_SOCKET_BAD;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    ares_channel_t *channel = NULL;
    ares_channel_t *dup_channel = NULL;
    int timeout_ms = -1;
    
    if (Size < 1) {
        return 0;
    }
    
    unsigned char operation = Data[0] % 6;
    
    int init_result = ares_init(&channel);
    if (init_result != ARES_SUCCESS) {
        return 0;
    }
    
    switch (operation) {
        case 0:
            ares_destroy(channel);
            channel = NULL;
            break;
            
        case 1:
            if (ares_dup(&dup_channel, channel) == ARES_SUCCESS) {
                if (dup_channel) {
                    ares_destroy(dup_channel);
                    dup_channel = NULL;
                }
            }
            break;
            
        case 2:
            ares_set_socket_callback(channel, dummy_socket_callback, NULL);
            break;
            
        case 3:
            ares_reinit(channel);
            break;
            
        case 4:
            if (Size >= 5) {
                memcpy(&timeout_ms, Data + 1, sizeof(int));
            }
            ares_queue_wait_empty(channel, timeout_ms);
            break;
            
        case 5:
            ares_cancel(channel);
            break;
    }
    
    if (channel) {
        ares_destroy(channel);
    }
    
    return 0;
}
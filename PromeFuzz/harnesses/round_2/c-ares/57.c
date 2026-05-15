// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_process_pending_write at ares_process.c:408:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_process_pending_write at ares_process.c:408:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_set_socket_configure_callback at ares_socket.c:406:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy callback functions for fuzzing */
static int dummy_socket_create_cb(int sock, int type, void *user_data)
{
    (void)sock;
    (void)type;
    (void)user_data;
    return 0;
}

static int dummy_socket_config_cb(int sock, int type, void *user_data)
{
    (void)sock;
    (void)type;
    (void)user_data;
    return 0;
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size)
{
    ares_channel_t *channel = NULL;
    ares_channel_t *dup_channel = NULL;
    int init_result;
    
    /* Initialize a channel to work with */
    init_result = ares_init(&channel);
    if (init_result != ARES_SUCCESS) {
        return 0;
    }
    
    if (channel == NULL) {
        return 0;
    }
    
    /* Use fuzzer data to decide which functions to call */
    if (Size > 0) {
        uint8_t action = Data[0] % 6;
        
        switch (action) {
            case 0:
                /* Test ares_dup */
                if (Size > 1) {
                    /* Use second byte to decide whether to destroy the dup channel immediately */
                    ares_dup(&dup_channel, channel);
                    if (dup_channel != NULL) {
                        if (Data[1] % 2 == 0) {
                            ares_destroy(dup_channel);
                            dup_channel = NULL;
                        }
                    }
                }
                break;
                
            case 1:
                /* Test ares_set_socket_callback */
                if (Size > 8) {
                    /* Use some fuzzer data as user_data pointer (cast carefully) */
                    void *user_data = (void *)(uintptr_t)((Data[1] << 24) | (Data[2] << 16) | 
                                                          (Data[3] << 8) | Data[4]);
                    ares_set_socket_callback(channel, dummy_socket_create_cb, user_data);
                }
                break;
                
            case 2:
                /* Test ares_process_pending_write */
                ares_process_pending_write(channel);
                break;
                
            case 3:
                /* Test ares_reinit */
                ares_reinit(channel);
                break;
                
            case 4:
                /* Test ares_cancel */
                ares_cancel(channel);
                break;
                
            case 5:
                /* Test ares_set_socket_configure_callback */
                if (Size > 8) {
                    /* Use some fuzzer data as user_data pointer (cast carefully) */
                    void *user_data = (void *)(uintptr_t)((Data[5] << 24) | (Data[6] << 16) | 
                                                          (Data[7] << 8) | Data[8]);
                    ares_set_socket_configure_callback(channel, dummy_socket_config_cb, user_data);
                }
                break;
        }
    }
    
    /* Try to exercise more states by calling multiple functions */
    if (Size > 1 && channel != NULL) {
        /* Call reinit with some probability */
        if ((Data[0] + Data[Size > 1 ? 1 : 0]) % 3 == 0) {
            ares_reinit(channel);
        }
        
        /* Call cancel with some probability */
        if ((Data[0] + Data[Size > 2 ? 2 : 0]) % 4 == 0) {
            ares_cancel(channel);
        }
        
        /* Call process_pending_write with some probability */
        if ((Data[0] + Data[Size > 3 ? 3 : 0]) % 5 == 0) {
            ares_process_pending_write(channel);
        }
    }
    
    /* Clean up dup channel if it exists */
    if (dup_channel != NULL) {
        ares_destroy(dup_channel);
    }
    
    /* Clean up main channel */
    if (channel != NULL) {
        ares_destroy(channel);
    }
    
    return 0;
}
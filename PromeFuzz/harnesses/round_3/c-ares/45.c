// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init at ares_init.c:67:5 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_process_pending_write at ares_process.c:408:6 in ares.h
// ares_set_socket_configure_callback at ares_socket.c:406:6 in ares.h
// ares_set_socket_configure_callback at ares_socket.c:406:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

static int dummy_socket_config_callback(ares_socket_t socket_fd, int type, void *user_data) {
    (void)socket_fd;
    (void)type;
    (void)user_data;
    return 0;
}

static int dummy_socket_create_callback(int af, int type, void *user_data) {
    (void)af;
    (void)type;
    (void)user_data;
    return ARES_SOCKET_BAD;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    ares_channel_t *channel = NULL;
    ares_channel_t *dup_channel = NULL;
    int status;
    
    status = ares_init(&channel);
    if (status != ARES_SUCCESS) {
        return 0;
    }
    
    if (Size > 0) {
        uint8_t operation = Data[0] % 6;
        size_t offset = 1;
        
        switch (operation) {
            case 0:
                ares_cancel(channel);
                break;
                
            case 1:
                ares_process_pending_write(channel);
                break;
                
            case 2:
                if (Size > offset) {
                    void *user_data = (void *)(uintptr_t)Data[offset];
                    ares_set_socket_configure_callback(channel, dummy_socket_config_callback, user_data);
                } else {
                    ares_set_socket_configure_callback(channel, dummy_socket_config_callback, NULL);
                }
                break;
                
            case 3:
                ares_reinit(channel);
                break;
                
            case 4:
                if (Size > offset) {
                    void *user_data = (void *)(uintptr_t)Data[offset];
                    ares_set_socket_callback(channel, dummy_socket_create_callback, user_data);
                } else {
                    ares_set_socket_callback(channel, dummy_socket_create_callback, NULL);
                }
                break;
                
            case 5:
                if (Size > offset) {
                    int dup_result = ares_dup(&dup_channel, channel);
                    (void)dup_result;
                    
                    if (dup_channel != NULL) {
                        ares_destroy(dup_channel);
                        dup_channel = NULL;
                    }
                }
                break;
        }
    }
    
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size > 1024 ? 1024 : Size);
        }
        close(fd);
        unlink("./dummy_file");
    }
    
    if (channel != NULL) {
        ares_destroy(channel);
    }
    
    return 0;
}
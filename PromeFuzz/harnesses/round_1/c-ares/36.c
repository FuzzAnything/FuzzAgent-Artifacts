// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_set_socket_configure_callback at ares_socket.c:406:6 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_queue_wait_empty at ares_threads.c:768:15 in ares.h
// ares_set_socket_configure_callback at ares_socket.c:406:6 in ares.h
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_queue_wait_empty at ares_threads.c:768:15 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int dummy_sock_config_callback(ares_socket_t socket_fd, int type, void *user_data) {
    (void)socket_fd;
    (void)type;
    (void)user_data;
    return 0;
}

static int dummy_sock_create_callback(int af, int type, void *user_data) {
    (void)af;
    (void)type;
    (void)user_data;
    return ARES_SOCKET_BAD;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    unsigned char selector = Data[0];
    int use_timeout = (Data[0] % 3) - 1;
    
    ares_channel_t *channel = NULL;
    int init_options = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS;
    struct ares_options options;
    options.flags = ARES_FLAG_NOCHECKRESP;
    options.timeout = 1000;
    
    if (ares_init_options(&channel, &options, init_options) != ARES_SUCCESS) {
        return 0;
    }
    
    switch (selector % 6) {
        case 0:
            ares_set_socket_configure_callback(channel, dummy_sock_config_callback, NULL);
            break;
        case 1: {
            ares_channel_t *dest = NULL;
            ares_dup(&dest, channel);
            if (dest) {
                ares_cancel(dest);
                ares_reinit(dest);
                ares_destroy(dest);
            }
            break;
        }
        case 2:
            ares_cancel(channel);
            break;
        case 3:
            ares_reinit(channel);
            break;
        case 4:
            ares_set_socket_callback(channel, dummy_sock_create_callback, NULL);
            break;
        case 5:
            ares_queue_wait_empty(channel, use_timeout);
            break;
    }
    
    if (selector & 0x80) {
        ares_set_socket_configure_callback(channel, NULL, NULL);
        ares_set_socket_callback(channel, NULL, NULL);
    }
    
    if (selector & 0x40) {
        ares_channel_t *dup_channel = NULL;
        int dup_result = ares_dup(&dup_channel, channel);
        if (dup_result == ARES_SUCCESS && dup_channel) {
            ares_queue_wait_empty(dup_channel, 0);
            ares_destroy(dup_channel);
        }
    }
    
    if (selector & 0x20) {
        for (int i = 0; i < 2 && i < (int)Size; i++) {
            ares_reinit(channel);
        }
    }
    
    if (selector & 0x10) {
        int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd >= 0) {
            if (Size > 0) {
                write(fd, Data, Size > 1024 ? 1024 : Size);
            }
            close(fd);
            unlink("./dummy_file");
        }
    }
    
    ares_destroy(channel);
    
    return 0;
}
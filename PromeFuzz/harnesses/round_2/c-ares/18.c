// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_fds at ares_fds.c:30:5 in ares.h
// ares_timeout at ares_timeout.c:135:17 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_timeout at ares_timeout.c:135:17 in ares.h
// ares_process at ares_process.c:334:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_library_init at ares_library_init.c:108:5 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
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
#include <sys/select.h>
#include <sys/time.h>

static void dummy_socket_state_cb(void *data, ares_socket_t socket_fd, int readable, int writable) {
    (void)data;
    (void)socket_fd;
    (void)readable;
    (void)writable;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    if (Size < 1) {
        return 0;
    }
    
    int init_flags = (int)Data[0] % 16;
    if (ares_library_init(init_flags) != ARES_SUCCESS) {
        return 0;
    }
    
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    memset(&options, 0, sizeof(options));
    
    if (Size > 1) {
        options.flags = (unsigned int)Data[1] % 256;
        optmask |= ARES_OPT_FLAGS;
    }
    
    if (Size > 2) {
        options.timeout = (size_t)(Data[2] % 1000);
        optmask |= ARES_OPT_TIMEOUTMS;
    }
    
    if (Size > 3) {
        options.tries = (size_t)(Data[3] % 10) + 1;
        optmask |= ARES_OPT_TRIES;
    }
    
    if (Size > 4) {
        options.ndots = (size_t)(Data[4] % 10);
        optmask |= ARES_OPT_NDOTS;
    }
    
    if (Size > 5) {
        options.socket_send_buffer_size = (int)Data[5] * 1024;
        optmask |= ARES_OPT_SOCK_SNDBUF;
    }
    
    if (Size > 6) {
        options.socket_receive_buffer_size = (int)Data[6] * 1024;
        optmask |= ARES_OPT_SOCK_RCVBUF;
    }
    
    if (Size > 7) {
        options.lookups = strdup("b");
        optmask |= ARES_OPT_LOOKUPS;
    }
    
    if (Size > 8) {
        options.ednspsz = (size_t)Data[8] % 4096;
        optmask |= ARES_OPT_EDNSPSZ;
    }
    
    if (Size > 9) {
        options.nsort = 0;
        optmask |= ARES_OPT_SORTLIST;
    }
    
    if (Size > 10 && Size < 100) {
        options.domains = malloc(sizeof(char*));
        if (options.domains) {
            options.domains[0] = strdup("example.com");
            options.ndomains = 1;
            optmask |= ARES_OPT_DOMAINS;
        }
    }
    
    options.sock_state_cb = dummy_socket_state_cb;
    optmask |= ARES_OPT_SOCK_STATE_CB;
    
    ares_destroy(channel);
    channel = NULL;
    
    ares_init_options(&channel, &options, optmask);
    
    if (options.lookups) {
        free(options.lookups);
    }
    
    if (options.domains) {
        if (options.domains[0]) {
            free(options.domains[0]);
        }
        free(options.domains);
    }
    
    if (!channel) {
        ares_library_cleanup();
        return 0;
    }
    
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    struct timeval maxtv, tv;
    struct timeval *timeout_result;
    
    int fds_count = ares_fds(channel, &read_fds, &write_fds);
    (void)fds_count;
    
    maxtv.tv_sec = (long)(Data[0] % 5);
    maxtv.tv_usec = 0;
    if (Size > 1) {
        maxtv.tv_usec = (long)(Data[1] % 1000000);
    }
    
    timeout_result = ares_timeout(channel, &maxtv, &tv);
    (void)timeout_result;
    
    ares_cancel(channel);
    
    maxtv.tv_sec = 0;
    maxtv.tv_usec = 0;
    if (Size > 2) {
        maxtv.tv_sec = (long)(Data[2] % 10);
    }
    if (Size > 3) {
        maxtv.tv_usec = (long)(Data[3] % 1000000);
    }
    
    timeout_result = ares_timeout(channel, &maxtv, &tv);
    (void)timeout_result;
    
    ares_process(channel, &read_fds, &write_fds);
    
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
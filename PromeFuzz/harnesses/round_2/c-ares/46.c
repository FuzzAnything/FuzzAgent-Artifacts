// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_queue_active_queries at ares_send.c:285:8 in ares.h
// ares_fds at ares_fds.c:30:5 in ares.h
// ares_timeout at ares_timeout.c:135:17 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_sock_state_cb(void *data, ares_socket_t socket_fd,
                                int readable, int writable) {
    (void)data;
    (void)socket_fd;
    (void)readable;
    (void)writable;
}

static int init_channel(ares_channel_t **channel) {
    int status;
    struct ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    
    options.sock_state_cb = dummy_sock_state_cb;
    optmask |= ARES_OPT_SOCK_STATE_CB;
    
    options.timeout = 1000;
    optmask |= ARES_OPT_TIMEOUTMS;
    
    options.tries = 4;
    optmask |= ARES_OPT_TRIES;
    
    status = ares_init_options(channel, &options, optmask);
    
    return status;
}

static void create_resolvconf_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (size > 0 && data) {
            fwrite(data, 1, size, fp);
        } else {
            const char *default_content = "nameserver 8.8.8.8\n";
            fwrite(default_content, 1, strlen(default_content), fp);
        }
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *data, size_t size) {
    ares_channel_t *channel = NULL;
    struct timeval maxtv, tv;
    fd_set read_fds, write_fds;
    size_t active_queries;
    ares_status_t reinit_status;
    
    if (size < sizeof(struct timeval)) {
        return 0;
    }
    
    memcpy(&maxtv, data, sizeof(struct timeval));
    
    if (init_channel(&channel) != ARES_SUCCESS) {
        return 0;
    }
    
    active_queries = ares_queue_active_queries(channel);
    (void)active_queries;
    
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    (void)ares_fds(channel, &read_fds, &write_fds);
    
    struct timeval *timeout_result = ares_timeout(channel, &maxtv, &tv);
    (void)timeout_result;
    
    reinit_status = ares_reinit(channel);
    (void)reinit_status;
    
    ares_cancel(channel);
    
    create_resolvconf_file(data, size);
    
    ares_destroy(channel);
    
    unlink("./dummy_file");
    
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_gethostbyname at ares_gethostbyname.c:99:6 in ares.h
// ares_gethostbyname at ares_gethostbyname.c:99:6 in ares.h
// ares_timeout at ares_timeout.c:135:17 in ares.h
// ares_timeout at ares_timeout.c:135:17 in ares.h
// ares_queue_active_queries at ares_send.c:285:8 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_fds at ares_fds.c:30:5 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netdb.h>

#include "ares.h"

static void dummy_callback(void *arg, int status, int timeouts, const struct hostent *hostent) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
}

static void dummy_sock_state_callback(void *data, ares_socket_t socket_fd, int readable, int writable) {
    (void)data;
    (void)socket_fd;
    (void)readable;
    (void)writable;
}

static ares_socket_t dummy_asocket(int domain, int type, int protocol, void *user_data) {
    (void)domain;
    (void)type;
    (void)protocol;
    (void)user_data;
    return ARES_SOCKET_BAD;
}

static int dummy_aclose(ares_socket_t sock, void *user_data) {
    (void)sock;
    (void)user_data;
    return -1;
}

static int dummy_aconnect(ares_socket_t sock, const struct sockaddr *addr, ares_socklen_t addrlen, void *user_data) {
    (void)sock;
    (void)addr;
    (void)addrlen;
    (void)user_data;
    return -1;
}

static ares_ssize_t dummy_arecvfrom(ares_socket_t sock, void *buf, size_t len, int flags, struct sockaddr *from, ares_socklen_t *fromlen, void *user_data) {
    (void)sock;
    (void)buf;
    (void)len;
    (void)flags;
    (void)from;
    (void)fromlen;
    (void)user_data;
    return -1;
}

static ares_ssize_t dummy_asendv(ares_socket_t sock, const struct iovec *vec, int len, void *user_data) {
    (void)sock;
    (void)vec;
    (void)len;
    (void)user_data;
    return -1;
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    ares_channel_t *channel = NULL;
    struct ares_options options;
    memset(&options, 0, sizeof(options));

    unsigned char *data_copy = (unsigned char *)malloc(Size);
    if (!data_copy) return 0;
    memcpy(data_copy, Data, Size);

    int init_flags = data_copy[0] & 0x0F;
    int optmask = 0;

    if (Size > 1) {
        options.timeout = (data_copy[1] % 5000) + 1;
        optmask |= ARES_OPT_TIMEOUTMS;
    }
    if (Size > 2) {
        options.tries = (data_copy[2] % 4) + 1;
        optmask |= ARES_OPT_TRIES;
    }
    if (Size > 3) {
        options.ndots = (data_copy[3] % 15) + 1;
        optmask |= ARES_OPT_NDOTS;
    }
    if (Size > 4) {
        options.udp_port = htons((data_copy[4] % 100) + 1024);
        optmask |= ARES_OPT_UDP_PORT;
    }
    if (Size > 5) {
        options.tcp_port = htons((data_copy[5] % 100) + 1024);
        optmask |= ARES_OPT_TCP_PORT;
    }
    if (Size > 6 && (data_copy[6] & 0x01)) {
        optmask |= ARES_OPT_FLAGS;
        options.flags = ARES_FLAG_STAYOPEN | ARES_FLAG_IGNTC;
    }

    if (Size > 7) {
        optmask |= ARES_OPT_SOCK_STATE_CB;
        options.sock_state_cb = dummy_sock_state_callback;
        options.sock_state_cb_data = NULL;
    }

    int status = ares_init_options(&channel, &options, optmask | init_flags);
    
    if (status == ARES_SUCCESS && channel) {
        size_t query_count = 0;
        if (Size > 9 && (data_copy[8] & 0x01)) {
            ares_gethostbyname(channel, "example.com", AF_INET, dummy_callback, NULL);
            ares_gethostbyname(channel, "example.org", AF_INET6, dummy_callback, NULL);
        }

        if (Size > 10) {
            struct timeval maxtv;
            maxtv.tv_sec = data_copy[9] % 5;
            maxtv.tv_usec = data_copy[10] % 1000000;
            
            struct timeval tv;
            struct timeval *result = ares_timeout(channel, &maxtv, &tv);
            (void)result;
        } else {
            struct timeval tv;
            struct timeval *result = ares_timeout(channel, NULL, &tv);
            (void)result;
        }

        query_count = ares_queue_active_queries(channel);
        (void)query_count;

        if (Size > 11 && (data_copy[11] & 0x01)) {
            ares_cancel(channel);
        }

        if (Size > 12) {
            fd_set read_fds, write_fds;
            FD_ZERO(&read_fds);
            FD_ZERO(&write_fds);
            int nfds = ares_fds(channel, &read_fds, &write_fds);
            (void)nfds;
        }

        if (Size > 13) {
            ares_status_t reinit_status = ares_reinit(channel);
            (void)reinit_status;
        }

        ares_destroy(channel);
    }

    free(data_copy);
    return 0;
}
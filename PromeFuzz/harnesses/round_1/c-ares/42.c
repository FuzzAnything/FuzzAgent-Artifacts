// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_process_pending_write at ares_process.c:408:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_set_socket_functions_ex at ares_set_socket_functions.c:89:3 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_set_socket_configure_callback at ares_socket.c:406:6 in ares.h
// ares_process_fds at ares_process.c:251:15 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

static int dummy_socket_create_callback(int domain, int type, void *user_data) {
    return ARES_SOCKET_BAD;
}

static int dummy_socket_configure_callback(ares_socket_t socket_fd, int type, void *user_data) {
    return 0;
}

static ares_socket_t dummy_asocket(int domain, int type, int protocol, void *user_data) {
    return ARES_SOCKET_BAD;
}

static int dummy_aclose(ares_socket_t sock, void *user_data) {
    return 0;
}

static int dummy_asetsockopt(ares_socket_t sock, ares_socket_opt_t opt, const void *val,
                             ares_socklen_t val_size, void *user_data) {
    return 0;
}

static int dummy_aconnect(ares_socket_t sock, const struct sockaddr *address,
                          ares_socklen_t address_len, unsigned int flags,
                          void *user_data) {
    return 0;
}

static ares_ssize_t dummy_arecvfrom(ares_socket_t sock, void *buffer, size_t length,
                                    int flags, struct sockaddr *address,
                                    ares_socklen_t *address_len, void *user_data) {
    return -1;
}

static ares_ssize_t dummy_asendto(ares_socket_t sock, const void *buffer, size_t length,
                                  int flags, const struct sockaddr *address,
                                  ares_socklen_t address_len, void *user_data) {
    return -1;
}

static void setup_dummy_socket_functions(struct ares_socket_functions_ex *funcs) {
    memset(funcs, 0, sizeof(*funcs));
    funcs->version = 1;
    funcs->asocket = dummy_asocket;
    funcs->aclose = dummy_aclose;
    funcs->asetsockopt = dummy_asetsockopt;
    funcs->aconnect = dummy_aconnect;
    funcs->arecvfrom = dummy_arecvfrom;
    funcs->asendto = dummy_asendto;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;

    if (Size < 1) return 0;

    memset(&options, 0, sizeof(options));
    options.timeout = 1000;
    options.tries = 1;
    optmask = ARES_OPT_TIMEOUT | ARES_OPT_TRIES;

    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        return 0;
    }

    uint8_t opcode = Data[0] % 6;
    size_t data_offset = 1;

    switch (opcode) {
        case 0: {
            ares_set_socket_configure_callback(channel, dummy_socket_configure_callback, NULL);
            break;
        }
        case 1: {
            if (data_offset + sizeof(size_t) <= Size) {
                size_t nevents = 0;
                memcpy(&nevents, Data + data_offset, sizeof(size_t));
                nevents = nevents % 4;
                data_offset += sizeof(size_t);

                ares_fd_events_t *events = NULL;
                if (nevents > 0 && data_offset + sizeof(ares_fd_events_t) * nevents <= Size) {
                    events = (ares_fd_events_t *)(Data + data_offset);
                }

                unsigned int flags = 0;
                if (data_offset + sizeof(unsigned int) <= Size) {
                    memcpy(&flags, Data + data_offset, sizeof(unsigned int));
                }

                ares_process_fds(channel, events, nevents, flags);
            }
            break;
        }
        case 2: {
            ares_cancel(channel);
            break;
        }
        case 3: {
            ares_process_pending_write(channel);
            break;
        }
        case 4: {
            ares_reinit(channel);
            break;
        }
        case 5: {
            ares_set_socket_callback(channel, dummy_socket_create_callback, NULL);
            struct ares_socket_functions_ex funcs;
            setup_dummy_socket_functions(&funcs);
            ares_set_socket_functions_ex(channel, &funcs, NULL);
            break;
        }
    }

    ares_destroy(channel);
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_set_socket_configure_callback at ares_socket.c:406:6 in ares.h
// ares_set_socket_functions at ares_set_socket_functions.c:579:6 in ares.h
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_set_socket_configure_callback at ares_socket.c:406:6 in ares.h
// ares_set_socket_functions at ares_set_socket_functions.c:579:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_library_init at ares_library_init.c:108:5 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy callbacks for socket functions */
static ares_socket_t dummy_asocket(int domain, int type, int protocol, void *user_data) {
    return ARES_SOCKET_BAD;
}

static int dummy_aclose(ares_socket_t sockfd, void *user_data) {
    return 0;
}

static int dummy_aconnect(ares_socket_t sockfd, const struct sockaddr *addr, 
                         ares_socklen_t len, void *user_data) {
    return -1;
}

static ares_ssize_t dummy_arecvfrom(ares_socket_t sockfd, void *buf, size_t len,
                                   int flags, struct sockaddr *addr,
                                   ares_socklen_t *addrlen, void *user_data) {
    return -1;
}

static ares_ssize_t dummy_asendv(ares_socket_t sockfd, const struct iovec *vec,
                                int len, void *user_data) {
    return -1;
}

/* Dummy socket creation callback - fixed to return int */
static int dummy_sock_create_callback(ares_socket_t sockfd, int type, void *user_data) {
    return 0;
}

/* Dummy socket configuration callback */
static int dummy_sock_config_callback(ares_socket_t sockfd, int type, void *user_data) {
    return 0;
}

/* Global socket functions structure */
static const struct ares_socket_functions dummy_socket_funcs = {
    dummy_asocket,
    dummy_aclose,
    dummy_aconnect,
    dummy_arecvfrom,
    dummy_asendv
};

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    /* Initialize ares library */
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    /* Initialize channel with some basic options */
    memset(&options, 0, sizeof(options));
    options.timeout = 5000; /* 5 seconds */
    options.tries = 3;
    optmask = ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    /* Create channel - use fuzz data to influence some parameters if possible */
    if (Size > 0) {
        /* Use first byte to influence timeout (0-255ms) */
        options.timeout = Data[0] * 10;
    }
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        /* Try without options */
        if (ares_init(&channel) != ARES_SUCCESS) {
            ares_library_cleanup();
            return 0;
        }
    }
    
    /* Use fuzz data to create user_data pointer */
    void *user_data = NULL;
    if (Size > 0) {
        /* Create a copy of the data to use as user_data */
        user_data = malloc(Size);
        if (user_data) {
            memcpy(user_data, Data, Size);
        }
    }
    
    /* Call target functions in specified order */
    
    /* 1. ares_cancel */
    ares_cancel(channel);
    
    /* 2. ares_set_socket_callback */
    ares_set_socket_callback(channel, dummy_sock_create_callback, user_data);
    
    /* 3. ares_set_socket_configure_callback */
    ares_set_socket_configure_callback(channel, dummy_sock_config_callback, user_data);
    
    /* 4. ares_set_socket_functions */
    ares_set_socket_functions(channel, &dummy_socket_funcs, user_data);
    
    /* Try to trigger different states by calling functions multiple times with different parameters */
    if (Size > 1) {
        /* Use second byte to decide whether to call again with NULL callbacks */
        if (Data[1] & 0x01) {
            ares_set_socket_callback(channel, NULL, NULL);
        }
        if (Data[1] & 0x02) {
            ares_set_socket_configure_callback(channel, NULL, NULL);
        }
        if (Data[1] & 0x04) {
            ares_set_socket_functions(channel, NULL, NULL);
        }
    }
    
    /* Clean up */
    if (user_data) {
        free(user_data);
    }
    
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
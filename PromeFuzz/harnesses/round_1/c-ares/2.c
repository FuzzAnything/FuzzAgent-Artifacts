// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_getaddrinfo at ares_getaddrinfo.c:695:6 in ares.h
// ares_process_fd at ares_process.c:267:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
// ares_library_init at ares_library_init.c:108:5 in ares.h
// ares_strerror at ares_strerror.c:30:13 in ares.h
// ares_strerror at ares_strerror.c:30:13 in ares.h
// ares_strerror at ares_strerror.c:30:13 in ares.h
// ares_strerror at ares_strerror.c:30:13 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_get_servers_csv at ares_update_servers.c:1314:7 in ares.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_strerror at ares_strerror.c:30:13 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_addrinfo_callback(void *arg, int status,
                                    int timeouts,
                                    struct ares_addrinfo *result) {
    /* Free the result if provided */
    if (result != NULL) {
        ares_freeaddrinfo(result);
    }
    (void)arg;
    (void)status;
    (void)timeouts;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    int flags = 0;
    if (Size > 0) {
        flags = Data[0] & 0xFF;
    }
    
    /* Initialize library */
    int init_result = ares_library_init(flags);
    (void)init_result;
    
    /* Call ares_strerror with some error codes */
    const char *err_str1 = ares_strerror(ARES_SUCCESS);
    (void)err_str1;
    const char *err_str2 = ares_strerror(ARES_ENOTFOUND);
    (void)err_str2;
    const char *err_str3 = ares_strerror(ARES_ECONNREFUSED);
    (void)err_str3;
    const char *err_str4 = ares_strerror(ARES_ETIMEOUT);
    (void)err_str4;
    
    /* Initialize channel */
    ares_channel_t *channel = NULL;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    
    int optmask = 0;
    
    if (Size > 1) {
        options.flags = Data[1] & 0xFF;
        optmask |= ARES_OPT_FLAGS;
    }
    
    if (Size > 2) {
        options.timeout = Data[2] & 0xFF;
        optmask |= ARES_OPT_TIMEOUTMS;
    }
    
    if (Size > 3) {
        options.tries = Data[3] & 0xFF;
        optmask |= ARES_OPT_TRIES;
    }
    
    if (Size > 4) {
        options.ndots = Data[4] & 0xFF;
        optmask |= ARES_OPT_NDOTS;
    }
    
    int init_opt_result = ares_init_options(&channel, &options, optmask);
    
    if (init_opt_result == ARES_SUCCESS && channel != NULL) {
        /* Get servers CSV */
        char *servers_csv = ares_get_servers_csv(channel);
        if (servers_csv != NULL) {
            ares_free_string(servers_csv);
        }
        
        /* Call ares_strerror again with init_opt_result */
        const char *err_str5 = ares_strerror(init_opt_result);
        (void)err_str5;
        
        /* Prepare getaddrinfo parameters */
        static char dummy_node[256] = "localhost";
        static char dummy_service[256] = "80";
        
        /* Ensure node is never NULL or empty by always using a valid string */
        const char *node = dummy_node;
        const char *service = dummy_service;
        
        /* Prepare hints */
        struct ares_addrinfo_hints hints;
        memset(&hints, 0, sizeof(hints));
        
        /* Set safe default values */
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        
        /* Only set hints from fuzz data if we have enough data */
        if (Size >= 8) {
            /* Use modulo to get valid flag values */
            hints.ai_flags = Data[5] & 0xFF;
            
            /* Limit family to valid values */
            int family = Data[6] & 0xFF;
            if (family == AF_INET || family == AF_INET6 || family == AF_UNSPEC) {
                hints.ai_family = family;
            }
            
            /* Limit socktype to valid values */
            int socktype = Data[7] & 0xFF;
            if (socktype == SOCK_STREAM || socktype == SOCK_DGRAM) {
                hints.ai_socktype = socktype;
            }
        }
        
        /* Call getaddrinfo */
        ares_getaddrinfo(channel, node, service, &hints,
                        dummy_addrinfo_callback, NULL);
        
        /* Process any pending callbacks */
        ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
        
        /* Destroy channel */
        ares_destroy(channel);
    }
    
    /* Cleanup library */
    ares_library_cleanup();
    
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_create_query at ares_create_query.c:66:5 in ares.h
// ares_mkquery at ares_create_query.c:74:5 in ares.h
// ares_send at ares_send.c:247:6 in ares.h
// ares_query at ares_query.c:133:6 in ares.h
// ares_search at ares_search.c:431:6 in ares.h
// ares_query_dnsrec at ares_query.c:115:15 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_mkquery at ares_create_query.c:74:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "ares.h"

#ifdef __cplusplus
}
#endif

static void dummy_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    // Do nothing - just a dummy callback for deprecated functions
}

static void dummy_callback_dnsrec(void *arg, ares_status_t status, unsigned long timeouts, const ares_dns_record_t *dnsrec) {
    // Do nothing - just a dummy callback for ares_query_dnsrec
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    // Create a dummy file if needed (not used in this case)
    const char *dummy_file = "./dummy_file";
    int fd = open(dummy_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size);
        }
        close(fd);
    }

    // Initialize a channel for testing
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    // Set minimal options
    options.timeout = 1000; // 1 second
    optmask |= ARES_OPT_TIMEOUTMS;
    options.tries = 1;
    optmask |= ARES_OPT_TRIES;
    
    // Try to initialize the channel
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        // If initialization fails, we can still test some functions
        channel = NULL;
    }

    // Extract parameters from fuzzer input
    // Use first few bytes for integer parameters
    unsigned int params[6] = {0};
    size_t param_bytes = sizeof(params);
    if (Size >= param_bytes) {
        memcpy(params, Data, param_bytes);
    } else if (Size > 0) {
        // Copy whatever we have
        memcpy(params, Data, Size);
    }
    
    // Extract name string from remaining data
    const char *name = "example.com"; // Default
    char *dynamic_name = NULL;
    if (Size > param_bytes) {
        size_t name_len = Size - param_bytes;
        dynamic_name = (char *)malloc(name_len + 1);
        if (dynamic_name) {
            memcpy(dynamic_name, Data + param_bytes, name_len);
            dynamic_name[name_len] = '\0';
            name = dynamic_name;
        }
    }

    // Test ares_mkquery
    if (Size >= 2) {
        unsigned char *buf = NULL;
        int buflen = 0;
        unsigned short id = (unsigned short)params[3];
        int rd = params[4] & 1;
        
        ares_mkquery(name, (int)params[0] & 0xFFFF, (int)params[1] & 0xFFFF, 
                     id, rd, &buf, &buflen);
        
        if (buf) {
            free(buf);
        }
    }

    // Test ares_create_query
    if (Size >= 3) {
        unsigned char *buf = NULL;
        int buflen = 0;
        unsigned short id = (unsigned short)params[3];
        int rd = params[4] & 1;
        int max_udp_size = (int)params[5] & 0xFFFF;
        
        ares_create_query(name, (int)params[0] & 0xFFFF, (int)params[1] & 0xFFFF,
                         id, rd, &buf, &buflen, max_udp_size);
        
        if (buf) {
            free(buf);
        }
    }

    // Test channel-dependent functions if channel was initialized
    if (channel) {
        // Test ares_send with generated query buffer
        if (Size >= 2) {
            unsigned char *qbuf = NULL;
            int qlen = 0;
            unsigned short id = (unsigned short)params[3];
            int rd = params[4] & 1;
            
            if (ares_mkquery(name, (int)params[0] & 0xFFFF, (int)params[1] & 0xFFFF,
                           id, rd, &qbuf, &qlen) == 0 && qbuf) {
                ares_send(channel, qbuf, qlen, dummy_callback, NULL);
                free(qbuf);
            }
        }

        // Test ares_query
        ares_query(channel, name, (int)params[0] & 0xFFFF, 
                   (int)params[1] & 0xFFFF, dummy_callback, NULL);

        // Test ares_search
        ares_search(channel, name, (int)params[0] & 0xFFFF,
                    (int)params[1] & 0xFFFF, dummy_callback, NULL);

        // Test ares_query_dnsrec
        if (Size >= 2) {
            unsigned short qid = 0;
            ares_dns_class_t dnsclass = (ares_dns_class_t)(params[0] & 0xFFFF);
            ares_dns_rec_type_t type = (ares_dns_rec_type_t)(params[1] & 0xFFFF);
            
            ares_query_dnsrec(channel, name, dnsclass, type, 
                             dummy_callback_dnsrec, NULL, &qid);
        }

        // Clean up channel
        ares_destroy(channel);
    }

    // Clean up
    if (dynamic_name) {
        free(dynamic_name);
    }
    
    // Remove dummy file
    unlink(dummy_file);

    return 0;
}
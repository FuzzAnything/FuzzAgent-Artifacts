// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_mkquery at ares_create_query.c:74:5 in ares.h
// ares_send at ares_send.c:247:6 in ares.h
// ares_query at ares_query.c:133:6 in ares.h
// ares_create_query at ares_create_query.c:66:5 in ares.h
// ares_process_fd at ares_process.c:267:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_query_dnsrec at ares_query.c:115:15 in ares.h
// ares_mkquery at ares_create_query.c:74:5 in ares.h
// ares_search at ares_search.c:431:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <unistd.h>

static void dummy_callback(void *arg, int status, int timeouts,
                           const unsigned char *abuf, int alen) {
    // Do nothing
}

static void dummy_callback_dnsrec(void *arg, ares_status_t status,
                                  unsigned long dnsrec_handle, 
                                  const ares_dns_record_t *dnsrec) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize channel
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_NOCHECKRESP;
    options.timeout = 1000;
    options.tries = 1;
    options.ndots = 1;
    options.udp_port = 0;
    options.tcp_port = 0;
    optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES |
              ARES_OPT_NDOTS | ARES_OPT_UDP_PORT | ARES_OPT_TCP_PORT;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        return 0;
    }
    
    if (!channel) {
        return 0;
    }

    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    // Create name string from data
    size_t name_len = (Size - offset > 100) ? 100 : Size - offset;
    char *name = NULL;
    if (name_len > 0) {
        name = (char *)malloc(name_len + 1);
        if (name) {
            memcpy(name, Data + offset, name_len);
            name[name_len] = '\0';
            offset += name_len;
        }
    }
    
    if (!name) {
        name = strdup("example.com");
    }

    // Calculate remaining data for other parameters
    size_t remaining = Size - offset;
    const uint8_t *remaining_data = Data + offset;
    
    // Call target functions based on selector
    if (selector & 0x01) {
        // ares_query_dnsrec
        unsigned short qid = 0;
        ares_status_t status = ares_query_dnsrec(
            channel, name, ARES_CLASS_IN, ARES_REC_TYPE_A,
            dummy_callback_dnsrec, NULL, &qid);
        (void)status;
    }
    
    if (selector & 0x02) {
        // ares_mkquery
        unsigned short id = 1234;
        int rd = 1;
        unsigned char *buf = NULL;
        int buflen = 0;
        ares_mkquery(name, ARES_CLASS_IN, ARES_REC_TYPE_A,
                     id, rd, &buf, &buflen);
        if (buf) {
            free(buf);
        }
    }
    
    if (selector & 0x04) {
        // ares_search
        ares_search(channel, name, ARES_CLASS_IN, ARES_REC_TYPE_A,
                    dummy_callback, NULL);
    }
    
    if (selector & 0x08) {
        // ares_send
        unsigned short id = 5678;
        int rd = 1;
        unsigned char *qbuf = NULL;
        int qlen = 0;
        
        // Create query first
        ares_mkquery(name, ARES_CLASS_IN, ARES_REC_TYPE_A,
                     id, rd, &qbuf, &qlen);
        
        if (qbuf && qlen > 0) {
            ares_send(channel, qbuf, qlen, dummy_callback, NULL);
        }
        
        if (qbuf) {
            free(qbuf);
        }
    }
    
    if (selector & 0x10) {
        // ares_query
        ares_query(channel, name, ARES_CLASS_IN, ARES_REC_TYPE_A,
                   dummy_callback, NULL);
    }
    
    if (selector & 0x20) {
        // ares_create_query
        unsigned short id = 9012;
        int rd = 1;
        unsigned char *buf = NULL;
        int buflen = 0;
        int max_udp_size = 512;
        
        ares_create_query(name, ARES_CLASS_IN, ARES_REC_TYPE_A,
                          id, rd, &buf, &buflen, max_udp_size);
        
        if (buf) {
            free(buf);
        }
    }

    // Clean up
    if (name) {
        free(name);
    }
    
    // Process any pending events
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    ares_destroy(channel);
    
    return 0;
}
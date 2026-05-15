// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_mkquery at ares_create_query.c:74:5 in ares.h
// ares_search at ares_search.c:431:6 in ares.h
// ares_query_dnsrec at ares_query.c:115:15 in ares.h
// ares_create_query at ares_create_query.c:66:5 in ares.h
// ares_send at ares_send.c:247:6 in ares.h
// ares_process_fd at ares_process.c:267:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_query at ares_query.c:133:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

static void dummy_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    // Do nothing
}

static void dummy_callback_dnsrec(void *arg, ares_status_t status, unsigned long timeouts, const struct ares_dns_record *dnsrec) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize channel
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    optmask |= ARES_OPT_FLAGS;
    
    options.timeout = 1000;
    optmask |= ARES_OPT_TIMEOUTMS;
    
    options.tries = 1;
    optmask |= ARES_OPT_TRIES;
    
    // Create a dummy server address
    struct in_addr server_addr;
    server_addr.s_addr = inet_addr("127.0.0.1");
    options.servers = &server_addr;
    options.nservers = 1;
    optmask |= ARES_OPT_SERVERS;
    
    int status = ares_init_options(&channel, &options, optmask);
    if (status != ARES_SUCCESS) {
        return 0;
    }
    
    // Use first byte to select function
    uint8_t func_selector = Data[0];
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    // Extract name from data (null-terminated)
    char *name = NULL;
    if (remaining_size > 0) {
        name = (char *)malloc(remaining_size + 1);
        if (name) {
            memcpy(name, remaining_data, remaining_size);
            name[remaining_size] = '\0';
        }
    }
    
    if (!name) {
        ares_destroy(channel);
        return 0;
    }
    
    switch (func_selector % 6) {
        case 0: { // ares_query
            int dnsclass = 1; // Default to IN class
            int type = 1; // Default to A record
            
            if (remaining_size >= 2) {
                dnsclass = remaining_data[0] % 256;
            }
            if (remaining_size >= 4) {
                type = (remaining_data[1] % 256) | ((remaining_data[2] % 256) << 8);
            }
            ares_query(channel, name, dnsclass, type, dummy_callback, NULL);
            break;
        }
        
        case 1: { // ares_mkquery
            int dnsclass = 1;
            int type = 1;
            unsigned short id = 1234;
            int rd = 1;
            
            if (remaining_size >= 1) {
                dnsclass = remaining_data[0] % 256;
            }
            if (remaining_size >= 2) {
                type = remaining_data[1] % 256;
            }
            if (remaining_size >= 4) {
                id = (remaining_data[2] % 256) | ((remaining_data[3] % 256) << 8);
            }
            if (remaining_size >= 5) {
                rd = remaining_data[4] % 2;
            }
            
            unsigned char *buf = NULL;
            int buflen = 0;
            
            ares_mkquery(name, dnsclass, type, id, rd, &buf, &buflen);
            if (buf) {
                free(buf);
            }
            break;
        }
        
        case 2: { // ares_search
            int dnsclass = 1;
            int type = 1;
            
            if (remaining_size >= 1) {
                dnsclass = remaining_data[0] % 256;
            }
            if (remaining_size >= 2) {
                type = remaining_data[1] % 256;
            }
            ares_search(channel, name, dnsclass, type, dummy_callback, NULL);
            break;
        }
        
        case 3: { // ares_query_dnsrec
            ares_dns_class_t dnsclass = ARES_CLASS_IN;
            ares_dns_rec_type_t type = ARES_REC_TYPE_A;
            unsigned short qid = 0;
            
            if (remaining_size >= 1) {
                // Use modulo to ensure valid enum value
                type = (ares_dns_rec_type_t)(remaining_data[0] % 256);
            }
            
            ares_query_dnsrec(channel, name, dnsclass, type, dummy_callback_dnsrec, NULL, &qid);
            break;
        }
        
        case 4: { // ares_create_query
            int dnsclass = 1;
            int type = 1;
            unsigned short id = 1234;
            int rd = 1;
            int max_udp_size = 512;
            
            if (remaining_size >= 1) {
                dnsclass = remaining_data[0] % 256;
            }
            if (remaining_size >= 2) {
                type = remaining_data[1] % 256;
            }
            if (remaining_size >= 4) {
                id = (remaining_data[2] % 256) | ((remaining_data[3] % 256) << 8);
            }
            if (remaining_size >= 5) {
                rd = remaining_data[4] % 2;
            }
            
            unsigned char *buf = NULL;
            int buflen = 0;
            
            ares_create_query(name, dnsclass, type, id, rd, &buf, &buflen, max_udp_size);
            if (buf) {
                free(buf);
            }
            break;
        }
        
        case 5: { // ares_send
            unsigned char *qbuf = NULL;
            int qlen = 0;
            
            if (remaining_size > 0) {
                qbuf = (unsigned char *)malloc(remaining_size);
                if (qbuf) {
                    memcpy(qbuf, remaining_data, remaining_size);
                    qlen = (int)remaining_size;
                    ares_send(channel, qbuf, qlen, dummy_callback, NULL);
                    free(qbuf);
                }
            }
            break;
        }
    }
    
    // Clean up
    free(name);
    
    // Process any pending events
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    
    ares_destroy(channel);
    
    return 0;
}
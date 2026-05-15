// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_getnameinfo at ares_getnameinfo.c:188:6 in ares.h
// ares_query_dnsrec at ares_query.c:115:15 in ares.h
// ares_query at ares_query.c:133:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_queue_active_queries at ares_send.c:285:8 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/* Dummy callbacks for async operations */
static void dummy_nameinfo_callback(void *arg, int status, int timeouts,
                                   const char *node, const char *service) {
    /* Do nothing */
}

static void dummy_callback(void *arg, int status, int timeouts,
                          const unsigned char *abuf, int alen) {
    /* Do nothing */
}

static void dummy_dnsrec_callback(void *arg, ares_status_t status,
                                 size_t timeouts,
                                 const ares_dns_record_t *dnsrec) {
    /* Do nothing */
}

/* Helper to create dummy sockaddr from fuzzer data */
static void setup_dummy_sockaddr(const uint8_t *data, size_t size,
                                struct sockaddr *sa, ares_socklen_t *salen) {
    if (size >= sizeof(struct sockaddr_in)) {
        struct sockaddr_in *sin = (struct sockaddr_in *)sa;
        sin->sin_family = AF_INET;
        sin->sin_port = htons(53);
        if (size >= sizeof(struct sockaddr_in) + 4) {
            memcpy(&sin->sin_addr, data, 4);
        } else {
            sin->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        }
        *salen = sizeof(struct sockaddr_in);
    } else {
        struct sockaddr_in *sin = (struct sockaddr_in *)sa;
        sin->sin_family = AF_INET;
        sin->sin_port = htons(53);
        sin->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        *salen = sizeof(struct sockaddr_in);
    }
}

/* Helper to extract string from fuzzer data */
static char *extract_string(const uint8_t *data, size_t size, size_t *consumed) {
    if (size == 0) {
        *consumed = 0;
        return strdup("example.com");
    }
    
    /* Find null terminator or use all data */
    size_t len = 0;
    while (len < size && data[len] != 0) {
        len++;
    }
    
    char *str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data, len);
    str[len] = '\0';
    
    *consumed = (len < size) ? len + 1 : len;
    return str;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    /* Initialize channel */
    ares_channel_t *channel = NULL;
    ares_init_options(&channel, NULL, 0);
    if (!channel) return 0;
    
    /* Use first byte to select which function to test */
    uint8_t selector = Data[0];
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    switch (selector % 6) {
        case 0: { /* ares_getnameinfo */
            if (remaining_size > 0) {
                struct sockaddr sa;
                ares_socklen_t salen;
                setup_dummy_sockaddr(remaining_data, remaining_size, &sa, &salen);
                
                int flags = 0;
                if (remaining_size > 0) {
                    flags = remaining_data[0] % 256;
                }
                
                ares_getnameinfo(channel, &sa, salen, flags,
                               dummy_nameinfo_callback, NULL);
            }
            break;
        }
        
        case 1: { /* ares_query_dnsrec */
            if (remaining_size > 0) {
                size_t consumed = 0;
                char *name = extract_string(remaining_data, remaining_size, &consumed);
                if (name) {
                    ares_dns_class_t dnsclass = ARES_CLASS_IN;
                    ares_dns_rec_type_t type = ARES_REC_TYPE_A;
                    unsigned short qid = 0;
                    
                    if (remaining_size > consumed + 2) {
                        dnsclass = (remaining_data[consumed] % 3) + 1; /* Valid classes */
                        type = (remaining_data[consumed + 1] % 16) + 1; /* Common types */
                    }
                    
                    ares_query_dnsrec(channel, name, dnsclass, type,
                                     dummy_dnsrec_callback, NULL, &qid);
                    free(name);
                }
            }
            break;
        }
        
        case 2: { /* ares_query */
            if (remaining_size > 0) {
                size_t consumed = 0;
                char *name = extract_string(remaining_data, remaining_size, &consumed);
                if (name) {
                    int dnsclass = 1; /* IN */
                    int type = 1; /* A */
                    
                    if (remaining_size > consumed + 2) {
                        dnsclass = remaining_data[consumed] % 4;
                        type = remaining_data[consumed + 1] % 16;
                    }
                    
                    ares_query(channel, name, dnsclass, type,
                              dummy_callback, NULL);
                    free(name);
                }
            }
            break;
        }
        
        case 3: { /* ares_destroy */
            /* Test destroying and reinitializing */
            ares_destroy(channel);
            channel = NULL;
            ares_init_options(&channel, NULL, 0);
            break;
        }
        
        case 4: { /* ares_queue_active_queries */
            size_t active = ares_queue_active_queries(channel);
            (void)active; /* Use variable to avoid unused warning */
            break;
        }
        
        case 5: { /* ares_cancel */
            ares_cancel(channel);
            break;
        }
    }
    
    /* Clean up */
    if (channel) {
        ares_destroy(channel);
    }
    
    return 0;
}
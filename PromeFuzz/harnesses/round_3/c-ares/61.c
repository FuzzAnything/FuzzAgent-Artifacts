// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_query_dnsrec at ares_query.c:115:15 in ares.h
// ares_process_fds at ares_process.c:251:15 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_process_pending_write at ares_process.c:408:6 in ares.h
// ares_getnameinfo at ares_getnameinfo.c:188:6 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_process_fds at ares_process.c:251:15 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
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
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Callback functions */
static void query_callback(void *arg, ares_status_t status,
                          size_t timeouts, const struct ares_dns_record *dnsrec) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
    /* Empty callback for fuzzing */
}

static void nameinfo_callback(void *arg, int status,
                             int flags, const char *node, const char *service) {
    (void)arg;
    (void)status;
    (void)flags;
    (void)node;
    (void)service;
    /* Empty callback for fuzzing */
}

/* Helper function to create a channel with fuzzer data */
static ares_channel_t *create_channel_from_data(const uint8_t *data, size_t size) {
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    if (ares_init(&channel) != ARES_SUCCESS) {
        return NULL;
    }
    
    /* Use fuzzer data to set some options if we have enough data */
    if (size >= sizeof(options)) {
        memcpy(&options, data, sizeof(options));
        optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
        
        /* Limit values to reasonable ranges */
        options.flags = options.flags & 0xFF;
        options.timeout = (options.timeout % 5000) + 100; /* 100-5100ms */
        options.tries = (options.tries % 5) + 1; /* 1-5 tries */
        
        ares_init_options(&channel, &options, optmask);
    }
    
    return channel;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    ares_channel_t *channel = NULL;
    ares_channel_t *dup_channel = NULL;
    unsigned short qid = 0;
    ares_status_t status;
    
    /* Create initial channel */
    channel = create_channel_from_data(Data, Size);
    if (!channel) {
        return 0;
    }
    
    /* Use fuzzer data for query parameters */
    const char *name = "example.com";
    ares_dns_class_t dnsclass = ARES_CLASS_IN;
    ares_dns_rec_type_t type = ARES_REC_TYPE_A;
    
    /* Extract parameters from fuzzer data if available */
    if (Size >= 3) {
        /* Use first byte for name selection */
        switch (Data[0] % 8) {
            case 0: name = "example.com"; break;
            case 1: name = "test.local"; break;
            case 2: name = "a.b.c"; break;
            case 3: name = ""; break;  /* Empty string */
            case 4: name = "very.long.domain.name.that.goes.on.and.on.test"; break;
            case 5: name = "123.456.789"; break;
            case 6: name = "-invalid-"; break;
            case 7: name = "*.wildcard"; break;
        }
        
        /* Second byte for DNS class */
        dnsclass = (Data[1] % 4);
        
        /* Third byte for record type */
        type = (Data[2] % 256);
    }
    
    /* 1. Test ares_query_dnsrec */
    status = ares_query_dnsrec(channel, name, dnsclass, type,
                              query_callback, NULL, &qid);
    (void)status; /* Result intentionally ignored for fuzzing */
    
    /* 2. Test ares_process_fds */
    ares_fd_events_t events[2];
    size_t nevents = 0;
    unsigned int flags = 0;
    
    /* Create some dummy events if we have data */
    if (Size >= 8) {
        /* Use data to determine number of events (0-2) */
        nevents = Data[3] % 3;
        flags = Data[4] & 0xFF;
        
        for (size_t i = 0; i < nevents && i < 2; i++) {
            events[i].fd = (Data[5 + i] % 100) + 3; /* Avoid stdin/stdout/stderr */
            events[i].events = (Data[7] >> (i * 2)) & 0x03;
        }
    }
    
    ares_process_fds(channel, events, nevents, flags);
    
    /* 3. Test ares_cancel */
    ares_cancel(channel);
    
    /* 4. Test ares_process_pending_write */
    ares_process_pending_write(channel);
    
    /* 5. Test ares_getnameinfo - Fixed to avoid stack buffer overflow */
    struct sockaddr_storage ss;
    struct sockaddr *sa = (struct sockaddr *)&ss;
    ares_socklen_t salen = 0;
    int nameinfo_flags = 0;
    
    /* Initialize with safe defaults */
    memset(&ss, 0, sizeof(ss));
    
    if (Size >= 1) {
        nameinfo_flags = Data[0] & 0xFF;
    }
    
    /* Create either IPv4 or IPv6 address based on fuzzer data */
    if (Size >= 2 && (Data[1] & 0x01)) {
        /* IPv6 */
        struct sockaddr_in6 *sa6 = (struct sockaddr_in6 *)&ss;
        sa6->sin6_family = AF_INET6;
        sa6->sin6_port = htons(53);
        
        /* Copy address safely with bounds checking */
        size_t addr_offset = 2;
        size_t addr_size = sizeof(sa6->sin6_addr.s6_addr);
        if (Size >= addr_offset + addr_size) {
            memcpy(&sa6->sin6_addr.s6_addr, Data + addr_offset, addr_size);
        } else {
            /* Use IPv6 loopback if not enough data */
            sa6->sin6_addr = in6addr_loopback;
        }
        
        salen = sizeof(struct sockaddr_in6);
    } else {
        /* IPv4 */
        struct sockaddr_in *sa4 = (struct sockaddr_in *)&ss;
        sa4->sin_family = AF_INET;
        sa4->sin_port = htons(53);
        
        /* Copy address safely with bounds checking */
        size_t addr_offset = 2;
        size_t addr_size = sizeof(sa4->sin_addr.s_addr);
        if (Size >= addr_offset + addr_size) {
            memcpy(&sa4->sin_addr.s_addr, Data + addr_offset, addr_size);
        } else {
            /* Use IPv4 loopback if not enough data */
            sa4->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        }
        
        salen = sizeof(struct sockaddr_in);
    }
    
    ares_getnameinfo(channel, sa, salen, nameinfo_flags,
                    nameinfo_callback, NULL);
    
    /* 6. Test ares_dup */
    int dup_result = ares_dup(&dup_channel, channel);
    if (dup_result == ARES_SUCCESS && dup_channel) {
        /* Process some events on the duplicated channel */
        ares_process_fds(dup_channel, NULL, 0, 0);
        ares_cancel(dup_channel);
        ares_destroy(dup_channel);
    }
    
    /* Cleanup */
    ares_destroy(channel);
    
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_mkquery at ares_create_query.c:74:5 in ares.h
// ares_create_query at ares_create_query.c:66:5 in ares.h
// ares_create_query at ares_create_query.c:66:5 in ares.h
// ares_send at ares_send.c:247:6 in ares.h
// ares_search at ares_search.c:431:6 in ares.h
// ares_query at ares_query.c:133:6 in ares.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_send_dnsrec at ares_send.c:227:15 in ares.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

static void dummy_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    // Do nothing
}

static void dummy_callback_dnsrec(void *arg, ares_status_t status, unsigned long timeout_ms, const ares_dns_record_t *dnsrec) {
    // Do nothing - dnsrec is const, we don't destroy it here
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a dummy channel
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    int status = ares_init_options(&channel, &options, optmask);
    if (status != ARES_SUCCESS) {
        return 0;
    }

    // Extract parameters from fuzzer input
    // Use first byte for various flags/values
    uint8_t flags = Data[0];
    int dnsclass = (flags & 0x03) + 1;  // Ensure non-zero
    int type = ((flags >> 2) & 0x0F) + 1;
    int rd = (flags >> 6) & 0x01;
    unsigned short id = (flags << 8) | ((flags >> 8) & 0xFF);
    int max_udp_size = 512 + (flags * 10);

    // Prepare name (ensure null-terminated)
    size_t name_len = Size > 1 ? Size - 1 : 0;
    char *name = NULL;
    if (name_len > 0) {
        name = (char *)malloc(name_len + 1);
        if (!name) {
            ares_destroy(channel);
            return 0;
        }
        memcpy(name, Data + 1, name_len);
        name[name_len] = '\0';
    } else {
        name = strdup("example.com");
        if (!name) {
            ares_destroy(channel);
            return 0;
        }
    }

    // Test ares_mkquery
    unsigned char *buf1 = NULL;
    int buflen1 = 0;
    ares_mkquery(name, dnsclass, type, id, rd, &buf1, &buflen1);
    if (buf1) {
        free(buf1);
    }

    // Test ares_create_query
    unsigned char *buf2 = NULL;
    int buflen2 = 0;
    ares_create_query(name, dnsclass, type, id, rd, &buf2, &buflen2, max_udp_size);
    if (buf2) {
        free(buf2);
    }

    // Test ares_send with buffer from ares_create_query if available
    unsigned char *qbuf = NULL;
    int qlen = 0;
    ares_create_query(name, dnsclass, type, id, rd, &qbuf, &qlen, max_udp_size);
    if (qbuf && qlen > 0) {
        ares_send(channel, qbuf, qlen, dummy_callback, NULL);
        free(qbuf);
    }

    // Test ares_search
    ares_search(channel, name, dnsclass, type, dummy_callback, NULL);

    // Test ares_query
    ares_query(channel, name, dnsclass, type, dummy_callback, NULL);

    // Test ares_send_dnsrec
    ares_dns_record_t *dnsrec = NULL;
    status = ares_dns_record_create(&dnsrec, id, ARES_FLAG_RD, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status == ARES_SUCCESS && dnsrec) {
        unsigned short qid = 0;
        ares_send_dnsrec(channel, dnsrec, dummy_callback_dnsrec, NULL, &qid);
        ares_dns_record_destroy(dnsrec);
    }

    // Cleanup
    free(name);
    ares_destroy(channel);

    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_write at ares_dns_write.c:1193:15 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_record_duplicate at ares_dns_record.c:1664:20 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_query_cnt at ares_dns_record.c:244:8 in ares_dns_record.h
// ares_dns_record_query_set_type at ares_dns_record.c:306:15 in ares_dns_record.h
// ares_search_dnsrec at ares_search.c:473:15 in ares.h
// ares_send_dnsrec at ares_send.c:227:15 in ares.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef CARES_EXTERN
#define CARES_EXTERN extern
#endif

#include "ares.h"
#include "ares_dns_record.h"

static void dummy_callback(void *arg, ares_status_t status,
                           unsigned long timeouts, const ares_dns_record_t *dnsrec) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    ares_dns_record_t *dnsrec = NULL;
    ares_channel_t *channel = NULL;
    ares_status_t status;
    unsigned char *buf = NULL;
    size_t buf_len = 0;
    unsigned short qid = 0;

    /* Initialize channel */
    struct ares_options options;
    int optmask = 0;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        channel = NULL;
        goto cleanup;
    }

    /* Create DNS record */
    status = ares_dns_record_create(&dnsrec, 0, 0, ARES_OPCODE_QUERY, 0);
    if (status != ARES_SUCCESS) {
        goto cleanup;
    }

    /* Prepare name from fuzzer data */
    char name[256];
    size_t name_len = Size < 255 ? Size : 255;
    memcpy(name, Data, name_len);
    name[name_len] = '\0';

    /* 1. Test ares_dns_record_query_add */
    ares_dns_rec_type_t qtype = (Data[0] % 64) + 1;
    ares_dns_class_t qclass = (Data[0] % 4);
    status = ares_dns_record_query_add(dnsrec, name, qtype, qclass);
    (void)status;

    /* 2. Test ares_dns_write */
    status = ares_dns_write(dnsrec, &buf, &buf_len);
    if (status == ARES_SUCCESS && buf != NULL) {
        ares_free_string(buf);
        buf = NULL;
    }

    /* 3. Test ares_dns_record_duplicate */
    ares_dns_record_t *dup_rec = ares_dns_record_duplicate(dnsrec);
    if (dup_rec != NULL) {
        ares_dns_record_destroy(dup_rec);
    }

    /* 4. Test ares_dns_record_query_set_type */
    size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
    if (query_cnt > 0) {
        size_t idx = 0;
        ares_dns_rec_type_t new_qtype = ((Data[0] + 1) % 64) + 1;
        status = ares_dns_record_query_set_type(dnsrec, idx, new_qtype);
        (void)status;
    }

    /* 5. Test ares_search_dnsrec */
    status = ares_search_dnsrec(channel, dnsrec, dummy_callback, NULL);
    (void)status;

    /* 6. Test ares_send_dnsrec */
    status = ares_send_dnsrec(channel, dnsrec, dummy_callback, NULL, &qid);
    (void)status;

cleanup:
    if (dnsrec != NULL) {
        ares_dns_record_destroy(dnsrec);
    }
    if (channel != NULL) {
        ares_destroy(channel);
    }
    if (buf != NULL) {
        ares_free_string(buf);
    }
    return 0;
}
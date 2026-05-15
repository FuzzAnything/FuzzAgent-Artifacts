// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_library_init at ares_library_init.c:108:5 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_dns_record_query_set_type at ares_dns_record.c:306:15 in ares_dns_record.h
// ares_dns_write at ares_dns_write.c:1193:15 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_record_duplicate at ares_dns_record.c:1664:20 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_send_dnsrec at ares_send.c:227:15 in ares.h
// ares_search_dnsrec at ares_search.c:473:15 in ares.h
// ares_process_fd at ares_process.c:267:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_callback(void *arg, ares_status_t status, unsigned long timeout_ms, const ares_dns_record_t *dnsrec) {
    (void)arg;
    (void)status;
    (void)timeout_ms;
    (void)dnsrec;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize library
    ares_library_init(ARES_LIB_INIT_ALL);

    // Create channel
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    options.flags = ARES_FLAG_STAYOPEN;
    optmask |= ARES_OPT_FLAGS;
    options.timeout = 1000;
    optmask |= ARES_OPT_TIMEOUTMS;
    options.tries = 1;
    optmask |= ARES_OPT_TRIES;

    ares_status_t status = ares_init_options(&channel, &options, optmask);
    if (status != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Create DNS record
    ares_dns_record_t *dnsrec = NULL;
    status = ares_dns_record_create(&dnsrec, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }

    // Use fuzz data to generate name
    char name[256];
    size_t name_len = Size < 255 ? Size : 255;
    memcpy(name, Data, name_len);
    name[name_len] = '\0';

    // Add query with fuzzed parameters
    ares_dns_rec_type_t qtype = (ares_dns_rec_type_t)(Data[0] % 256);
    ares_dns_class_t qclass = (ares_dns_class_t)(Data[0] % 256);
    
    status = ares_dns_record_query_add(dnsrec, name, qtype, qclass);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }

    // Test ares_dns_record_query_set_type
    size_t idx = 0;
    ares_dns_rec_type_t new_qtype = (ares_dns_rec_type_t)((Data[0] + 1) % 256);
    ares_dns_record_query_set_type(dnsrec, idx, new_qtype);

    // Test ares_dns_write
    unsigned char *buf = NULL;
    size_t buf_len = 0;
    ares_dns_write(dnsrec, &buf, &buf_len);
    if (buf) {
        ares_free_string(buf);
    }

    // Test ares_dns_record_duplicate
    ares_dns_record_t *duplicate = ares_dns_record_duplicate(dnsrec);
    if (duplicate) {
        ares_dns_record_destroy(duplicate);
    }

    // Test ares_send_dnsrec
    unsigned short qid = 0;
    ares_send_dnsrec(channel, dnsrec, dummy_callback, NULL, &qid);

    // Test ares_search_dnsrec
    ares_search_dnsrec(channel, dnsrec, dummy_callback, NULL);

    // Process any pending events
    ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);

    // Cleanup
    ares_dns_record_destroy(dnsrec);
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}
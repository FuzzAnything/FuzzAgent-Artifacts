// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_query_set_type at ares_dns_record.c:306:15 in ares_dns_record.h
// ares_search_dnsrec at ares_search.c:473:15 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_dns_record_duplicate at ares_dns_record.c:1664:20 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_send_dnsrec at ares_send.c:227:15 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_dns_write at ares_dns_write.c:1193:15 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

/* Dummy callback for functions requiring a callback */
static void dummy_callback(void *arg, ares_status_t status,
                           unsigned long timeouts,
                           const ares_dns_record_t *dnsrec) {
    /* Do nothing */
}

/* Helper to create a minimal DNS record from fuzzer data */
static ares_dns_record_t* create_dns_record_from_fuzz(const uint8_t *data, size_t size) {
    if (size < 2) return NULL;
    
    /* Use first byte for ID, second for flags */
    unsigned short id = (data[0] << 8) | data[1];
    unsigned short flags = size > 2 ? data[2] : 0;
    
    ares_dns_record_t *dnsrec = NULL;
    ares_dns_record_create(&dnsrec, id, flags, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    
    if (!dnsrec) return NULL;
    
    /* Try to add a query if we have enough data */
    if (size > 10) {
        /* Use some data for the name */
        size_t name_len = size > 50 ? 20 : (size - 10);
        char *name = malloc(name_len + 1);
        if (name) {
            memcpy(name, data + 3, name_len);
            name[name_len] = '\0';
            
            /* Use data bytes for qtype and qclass */
            ares_dns_rec_type_t qtype = (data[size-2] % 256) + 1;
            ares_dns_class_t qclass = (data[size-1] % 256) + 1;
            
            ares_dns_record_query_add(dnsrec, name, qtype, qclass);
            free(name);
        }
    }
    
    return dnsrec;
}

/* Helper to create a minimal channel */
static ares_channel_t* create_minimal_channel(void) {
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    /* Initialize with minimal options */
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    optmask = ARES_OPT_FLAGS;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        return NULL;
    }
    
    return channel;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Test 1: ares_dns_record_duplicate */
    ares_dns_record_t *dnsrec = create_dns_record_from_fuzz(Data, Size);
    if (dnsrec) {
        ares_dns_record_t *dup = ares_dns_record_duplicate(dnsrec);
        if (dup) {
            ares_dns_record_destroy(dup);
        }
    }
    
    /* Test 2: ares_send_dnsrec */
    if (dnsrec && Size > 5) {
        ares_channel_t *channel = create_minimal_channel();
        if (channel) {
            unsigned short qid = 0;
            ares_send_dnsrec(channel, dnsrec, dummy_callback, NULL, &qid);
            
            /* Clean up channel */
            ares_destroy(channel);
        }
    }
    
    /* Test 3: ares_dns_write */
    if (dnsrec) {
        unsigned char *buf = NULL;
        size_t buf_len = 0;
        ares_dns_write(dnsrec, &buf, &buf_len);
        if (buf) {
            ares_free_string(buf);
        }
    }
    
    /* Test 4: ares_dns_record_query_add (already tested in helper) */
    
    /* Test 5: ares_dns_record_query_set_type */
    if (dnsrec) {
        /* Try to set type if we have at least one query */
        size_t idx = 0;
        ares_dns_rec_type_t qtype = (Size > 3 ? Data[3] % 256 : 1) + 1;
        ares_dns_record_query_set_type(dnsrec, idx, qtype);
    }
    
    /* Test 6: ares_search_dnsrec */
    if (dnsrec && Size > 5) {
        ares_channel_t *channel = create_minimal_channel();
        if (channel) {
            ares_search_dnsrec(channel, dnsrec, dummy_callback, NULL);
            
            /* Clean up channel */
            ares_destroy(channel);
        }
    }
    
    /* Clean up */
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    return 0;
}
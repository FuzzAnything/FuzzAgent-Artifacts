// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_search_dnsrec at ares_search.c:473:15 in ares.h
// ares_send_dnsrec at ares_send.c:227:15 in ares.h
// ares_process at ares_process.c:334:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_dns_record_duplicate at ares_dns_record.c:1664:20 in ares_dns_record.h
// ares_dns_write at ares_dns_write.c:1193:15 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_query_set_type at ares_dns_record.c:306:15 in ares_dns_record.h
// ares_dns_write at ares_dns_write.c:1193:15 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

/* Dummy callback for ares_search_dnsrec and ares_send_dnsrec */
static void dummy_callback(void *arg, ares_status_t status,
                           unsigned long timeout_ms,
                           const ares_dns_record_t *dnsrec) {
    /* Do nothing */
}

/* Helper to create a DNS record with fuzzed data */
static ares_dns_record_t* create_dns_record_from_fuzz(const uint8_t *data, size_t size) {
    ares_dns_record_t *record = NULL;
    
    /* Extract values from fuzz data for DNS record creation */
    unsigned short id = 0;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    if (size >= 1) id = data[0];
    if (size >= 2) flags = data[1];
    if (size >= 3) opcode = (ares_dns_opcode_t)data[2];
    if (size >= 4) rcode = (ares_dns_rcode_t)data[3];
    
    /* Create a basic DNS record */
    if (ares_dns_record_create(&record, id, flags, opcode, rcode) != ARES_SUCCESS) {
        return NULL;
    }
    
    /* Add a query if we have enough data */
    if (size > 8) {
        /* Use bytes for qtype and qclass */
        ares_dns_rec_type_t qtype = (ares_dns_rec_type_t)data[4];
        ares_dns_class_t qclass = (ares_dns_class_t)data[5];
        
        /* Use remaining bytes for name (null-terminated) */
        char name[256];
        size_t name_len = size - 6 > 255 ? 255 : size - 6;
        memcpy(name, data + 6, name_len);
        name[name_len] = '\0';
        
        ares_dns_record_query_add(record, name, qtype, qclass);
    }
    
    return record;
}

/* Helper to create a minimal channel */
static ares_channel_t* create_minimal_channel(void) {
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    /* Initialize with minimal options */
    options.timeout = 1; /* 1ms */
    optmask |= ARES_OPT_TIMEOUTMS;
    options.tries = 1;
    optmask |= ARES_OPT_TRIES;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        return NULL;
    }
    
    return channel;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *data, size_t size) {
    /* Create a DNS record from fuzz data */
    ares_dns_record_t *dnsrec = create_dns_record_from_fuzz(data, size);
    if (!dnsrec) return 0;
    
    /* 1. Test ares_dns_record_duplicate */
    ares_dns_record_t *duplicate = ares_dns_record_duplicate(dnsrec);
    if (duplicate) {
        /* Test the duplicate with other operations */
        unsigned char *buf = NULL;
        size_t buf_len = 0;
        ares_dns_write(duplicate, &buf, &buf_len);
        if (buf) ares_free_string(buf);
        ares_dns_record_destroy(duplicate);
    }
    
    /* 2. Test ares_dns_record_query_set_type if we have a query */
    if (size > 4) {
        ares_dns_record_query_set_type(dnsrec, 0, (ares_dns_rec_type_t)data[4]);
    }
    
    /* 3. Test ares_dns_write */
    unsigned char *buf = NULL;
    size_t buf_len = 0;
    ares_status_t write_status = ares_dns_write(dnsrec, &buf, &buf_len);
    if (write_status == ARES_SUCCESS && buf) {
        ares_free_string(buf);
    }
    
    /* 4. Test ares_search_dnsrec and ares_send_dnsrec with a minimal channel */
    ares_channel_t *channel = create_minimal_channel();
    if (channel) {
        unsigned short qid = 0;
        
        /* Test ares_search_dnsrec */
        ares_search_dnsrec(channel, dnsrec, dummy_callback, NULL);
        
        /* Test ares_send_dnsrec */
        ares_send_dnsrec(channel, dnsrec, dummy_callback, NULL, &qid);
        
        /* Process any pending events (non-blocking) */
        struct timeval tv = {0, 0};
        ares_process(channel, NULL, NULL);
        
        /* Clean up channel */
        ares_destroy(channel);
    }
    
    /* 5. Test ares_dns_record_query_add with more variations */
    if (size > 12) {
        char name2[256];
        size_t name_len = size - 12 > 255 ? 255 : size - 12;
        memcpy(name2, data + 12, name_len);
        name2[name_len] = '\0';
        
        ares_dns_record_query_add(dnsrec, name2, 
                                 (ares_dns_rec_type_t)data[10], 
                                 (ares_dns_class_t)data[11]);
    }
    
    /* Clean up */
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
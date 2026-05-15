// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_get_rcode at ares_dns_record.c:125:18 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_query_cnt at ares_dns_record.c:244:8 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define missing types that are referenced in ares_dns_record.h */
#ifndef CARES_EXTERN
#define CARES_EXTERN extern
#endif

typedef int ares_bool_t;
typedef int ares_status_t;

/* Include the required header */
#include "ares_dns_record.h"

/* Forward declare the structure since we need to create a dummy instance */
struct ares_dns_record {
 unsigned short id;
 unsigned short flags;
 ares_dns_opcode_t opcode;
 ares_dns_rcode_t rcode;
 unsigned short raw_rcode;
 unsigned int ttl_decrement;
 void *qd;
 void *an;
 void *ns;
 void *ar;
};

static ares_dns_record_t* create_test_record(const uint8_t *data, size_t size) {
    /* Create a minimal DNS record structure for testing */
    static struct ares_dns_record record;
    
    /* Initialize with some default values */
    record.id = size > 0 ? (data[0] | (size > 1 ? data[1] << 8 : 0)) : 0;
    record.flags = size > 2 ? (data[2] | (size > 3 ? data[3] << 8 : 0)) : 0;
    
    /* Initialize rcode from data if available, otherwise use 0 */
    if (size > 4) {
        record.rcode = (ares_dns_rcode_t)(data[4] & 0xF);
        record.raw_rcode = data[4];
    } else {
        record.rcode = 0;
        record.raw_rcode = 0;
    }
    
    record.ttl_decrement = 0;
    record.opcode = 0;
    
    /* Initialize arrays as NULL */
    record.qd = NULL;
    record.an = NULL;
    record.ns = NULL;
    record.ar = NULL;
    
    return &record;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    /* Create a test DNS record using the fuzzer input */
    ares_dns_record_t *dnsrec = create_test_record(Data, Size);
    
    /* Call the target functions in the specified order */
    ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
    unsigned short id = ares_dns_record_get_id(dnsrec);
    unsigned short flags = ares_dns_record_get_flags(dnsrec);
    size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
    
    /* Use the results to avoid compiler optimizations */
    volatile ares_dns_rcode_t v_rcode = rcode;
    volatile unsigned short v_id = id;
    volatile unsigned short v_flags = flags;
    volatile size_t v_query_cnt = query_cnt;
    
    (void)v_rcode;
    (void)v_id;
    (void)v_flags;
    (void)v_query_cnt;
    
    return 0;
}
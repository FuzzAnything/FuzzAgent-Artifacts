// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_query_set_name at ares_dns_record.c:283:15 in ares_dns_record.h
// ares_dns_record_query_set_type at ares_dns_record.c:306:15 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include necessary headers in correct order */
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_record_t* create_test_record(const uint8_t *Data, size_t Size) {
    ares_dns_record_t* dnsrec = NULL;
    ares_status_t status;
    
    /* Extract parameters from fuzzer input for record creation */
    unsigned short id = 0x1234;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    if (Size >= 6) {
        id = (Data[0] << 8) | Data[1];
        flags = (Data[2] << 8) | Data[3];
        opcode = (ares_dns_opcode_t)(Data[4] % 16); /* Opcode values are 0-15 */
        rcode = (ares_dns_rcode_t)(Data[5] % 16);   /* Rcode values are 0-15 */
    }
    
    /* Create a DNS record with parameters from fuzzer input */
    status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS) {
        return NULL;
    }
    
    /* Set up a basic query - we need at least one question for our tests */
    status = ares_dns_record_query_add(dnsrec, "example.com", ARES_REC_TYPE_A, ARES_CLASS_IN);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return NULL;
    }
    
    return dnsrec;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    /* Create a test DNS record */
    ares_dns_record_t* dnsrec = create_test_record(Data, Size);
    if (!dnsrec) {
        return 0;
    }

    /* Use fuzzer input to generate parameters */
    size_t idx = 0; /* Typically 0 for first question */
    if (Size > 6) {
        idx = Data[6] % 4; /* Try indices 0-3 */
    }

    /* Prepare name from fuzzer data (ensure null-terminated) */
    char name[256];
    size_t name_len = 0;
    
    /* Start from byte 7 for name data if available */
    if (Size > 7) {
        name_len = Size - 7;
        if (name_len > 255) {
            name_len = 255;
        }
        memcpy(name, Data + 7, name_len);
    }
    name[name_len] = '\0';

    /* Call functions in specified order */
    /* 1. ares_dns_record_query_set_name */
    ares_dns_record_query_set_name(dnsrec, idx, name);

    /* 2. ares_dns_record_query_set_type */
    ares_dns_rec_type_t qtype = ARES_REC_TYPE_A;
    if (Size > 8) {
        qtype = (ares_dns_rec_type_t)(Data[8] % 256); /* Try various types */
    }
    ares_dns_record_query_set_type(dnsrec, idx, qtype);

    /* 3. ares_dns_record_query_get */
    const char* ret_name = NULL;
    ares_dns_rec_type_t ret_qtype;
    ares_dns_class_t ret_qclass;
    ares_dns_record_query_get(dnsrec, idx, &ret_name, &ret_qtype, &ret_qclass);

    /* 4. ares_dns_record_rr_cnt */
    ares_dns_section_t sections[] = {
        ARES_SECTION_ANSWER,
        ARES_SECTION_AUTHORITY,
        ARES_SECTION_ADDITIONAL
    };
    for (size_t i = 0; i < sizeof(sections)/sizeof(sections[0]); i++) {
        ares_dns_record_rr_cnt(dnsrec, sections[i]);
    }

    /* Cleanup */
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
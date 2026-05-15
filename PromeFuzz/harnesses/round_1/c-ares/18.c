// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_query_set_name at ares_dns_record.c:283:15 in ares_dns_record.h
// ares_dns_record_query_set_type at ares_dns_record.c:306:15 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include required headers in correct order */
#include "ares.h"
#include "ares_dns_record.h"

static const char dummy_name[] = "example.com";

static ares_dns_record_t* create_test_record(void) {
    ares_dns_record_t* dnsrec = NULL;
    
    /* Create a simple DNS record with one query */
    if (ares_dns_record_create(&dnsrec, 0x1234, 0, ARES_OPCODE_QUERY, 0) != ARES_SUCCESS) {
        return NULL;
    }
    
    /* Add a query to the record */
    if (ares_dns_record_query_add(dnsrec, dummy_name, ARES_REC_TYPE_A, ARES_CLASS_IN) != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return NULL;
    }
    
    return dnsrec;
}

static char* create_fuzz_string(const uint8_t* data, size_t size) {
    if (size == 0) {
        char* str = malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    char* str = malloc(size + 1);
    if (!str) return NULL;
    
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    /* Create a test DNS record */
    ares_dns_record_t* dnsrec = create_test_record();
    if (!dnsrec) return 0;
    
    /* Prepare fuzzed name string */
    char* fuzz_name = create_fuzz_string(Data, Size);
    if (!fuzz_name) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    /* Calculate index and type from fuzz data */
    size_t idx = 0;  /* Typically 0 for first query */
    ares_dns_rec_type_t qtype = (ares_dns_rec_type_t)(Data[0] % 256);
    
    /* Call target functions in specified order */
    ares_dns_record_query_set_name(dnsrec, idx, fuzz_name);
    ares_dns_record_query_set_type(dnsrec, idx, qtype);
    
    /* Get query details */
    const char* name_out = NULL;
    ares_dns_rec_type_t type_out;
    ares_dns_class_t class_out;
    ares_dns_record_query_get(dnsrec, idx, &name_out, &type_out, &class_out);
    
    /* Get RR counts for different sections */
    ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
    ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_AUTHORITY);
    ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ADDITIONAL);
    
    /* Try with invalid section for more coverage */
    ares_dns_record_rr_cnt(dnsrec, (ares_dns_section_t)999);
    
    /* Test with different index values */
    if (Size > 1) {
        size_t alt_idx = Data[1] % 4;  /* 0-3 */
        ares_dns_record_query_get(dnsrec, alt_idx, NULL, NULL, NULL);
    }
    
    /* Cleanup */
    free(fuzz_name);
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
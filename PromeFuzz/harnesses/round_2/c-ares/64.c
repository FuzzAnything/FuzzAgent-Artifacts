// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_get_opt at ares_dns_record.c:1016:16 in ares_dns_record.h
// ares_dns_opt_get_name at ares_dns_mapping.c:896:13 in ares_dns_record.h
// ares_dns_rr_get_str at ares_dns_record.c:981:13 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_opt_get_name at ares_dns_mapping.c:896:13 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include ares.h first to get CARES_EXTERN defined */
#include "ares.h"
/* Then include the DNS record header */
#include "ares_dns_record.h"

static ares_dns_rr_t* create_test_rr(ares_dns_record_t **dns_record) {
    /* Create a DNS record with some default parameters */
    ares_status_t status = ares_dns_record_create(dns_record, 12345, 
                                                  ARES_FLAG_QR, 
                                                  ARES_OPCODE_QUERY, 
                                                  ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || !*dns_record) {
        return NULL;
    }
    
    /* Add a resource record to the additional section (index 3) */
    ares_dns_rr_t *rr = NULL;
    status = ares_dns_record_rr_add(&rr, *dns_record, 3,
                                    "test.example.com", 
                                    ARES_REC_TYPE_OPT, 
                                    ARES_CLASS_IN, 0);
    if (status != ARES_SUCCESS || !rr) {
        ares_dns_record_destroy(*dns_record);
        *dns_record = NULL;
        return NULL;
    }
    return rr;
}

static void cleanup_test_rr(ares_dns_record_t *dns_record) {
    if (dns_record) {
        ares_dns_record_destroy(dns_record);
    }
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    ares_dns_record_t *dns_record = NULL;
    ares_dns_rr_t *rr = create_test_rr(&dns_record);
    if (!rr || !dns_record) {
        cleanup_test_rr(dns_record);
        return 0;
    }
    
    /* Use first 2 bytes for key selection - modulo to get valid enum range */
    ares_dns_rr_key_t key = (ares_dns_rr_key_t)(*(uint16_t*)Data % 100);
    
    /* Use next 2 bytes for opt selection */
    unsigned short opt = *(uint16_t*)(Data + 2);
    
    /* Use remaining data as value */
    const unsigned char *val = NULL;
    size_t val_len = 0;
    
    if (Size > 4) {
        val = Data + 4;
        val_len = Size - 4;
    }
    
    /* Test ares_dns_rr_set_opt */
    ares_status_t status = ares_dns_rr_set_opt(rr, key, opt, val, val_len);
    
    /* Test ares_dns_rr_key_datatype */
    ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
    
    /* Test ares_dns_rr_get_opt */
    const unsigned char *retrieved_val = NULL;
    size_t retrieved_len = 0;
    unsigned short retrieved_opt = ares_dns_rr_get_opt(rr, key, 0, 
                                                      &retrieved_val, 
                                                      &retrieved_len);
    
    /* Test ares_dns_opt_get_name */
    const char *opt_name = ares_dns_opt_get_name(key, opt);
    
    /* Test ares_dns_rr_get_str */
    const char *str_val = ares_dns_rr_get_str(rr, key);
    
    /* Test ares_dns_rr_key_tostr */
    const char *key_str = ares_dns_rr_key_tostr(key);
    
    cleanup_test_rr(dns_record);
    
    /* Also test with different key values */
    for (int i = 0; i < 10; i++) {
        ares_dns_rr_key_t test_key = (ares_dns_rr_key_t)((key + i) % 100);
        ares_dns_rr_key_datatype(test_key);
        ares_dns_opt_get_name(test_key, opt + i);
        ares_dns_rr_key_tostr(test_key);
    }
    
    return 0;
}
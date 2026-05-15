// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_opt_get_name at ares_dns_mapping.c:896:13 in ares_dns_record.h
// ares_dns_rr_get_opt at ares_dns_record.c:1016:16 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_addr at ares_dns_record.c:1103:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_get_str at ares_dns_record.c:981:13 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include required headers in the correct order */
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"

/* Helper function to create a minimal DNS record for testing */
static ares_dns_record_t* create_test_dns_record(void) {
    ares_dns_record_t* dnsrec = NULL;
    
    /* Use the library function to create a DNS record with correct arguments */
    ares_status_t status = ares_dns_record_create(&dnsrec, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || !dnsrec) {
        return NULL;
    }
    
    return dnsrec;
}

/* Helper function to create a minimal resource record for testing */
static ares_dns_rr_t* create_test_resource_record(ares_dns_record_t** dnsrec_out) {
    /* Create a basic DNS record first */
    ares_dns_record_t* dnsrec = create_test_dns_record();
    if (!dnsrec) {
        return NULL;
    }
    
    /* Add an A record to the answer section */
    ares_dns_rr_t* rr = NULL;
    ares_status_t status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                                  "test.example.com", 
                                                  ARES_REC_TYPE_A, 
                                                  ARES_CLASS_IN, 
                                                  300);
    if (status != ARES_SUCCESS || !rr) {
        ares_dns_record_destroy(dnsrec);
        return NULL;
    }
    
    /* Set the A record data */
    struct in_addr addr;
    addr.s_addr = 0x0100007F;  /* 127.0.0.1 */
    status = ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return NULL;
    }
    
    /* Return the DNS record through output parameter */
    if (dnsrec_out) {
        *dnsrec_out = dnsrec;
    }
    
    return rr;
}

/* Cleanup function */
static void cleanup_test_records(ares_dns_record_t* dnsrec) {
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Create test records */
    ares_dns_record_t* dnsrec = NULL;
    ares_dns_rr_t* rr = create_test_resource_record(&dnsrec);
    
    if (!dnsrec || !rr) {
        cleanup_test_records(dnsrec);
        return 0;
    }
    
    /* Use fuzzer input to generate test parameters */
    
    /* First byte for key selection - use modulo to keep within reasonable range */
    ares_dns_rr_key_t key = (ares_dns_rr_key_t)(Data[0] % 100);
    
    /* Test ares_dns_rr_key_tostr */
    const char* key_str = ares_dns_rr_key_tostr(key);
    (void)key_str; /* Prevent unused variable warning */
    
    /* Test ares_dns_rr_key_datatype */
    ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
    (void)datatype; /* Prevent unused variable warning */
    
    /* Test ares_dns_rr_get_str - only valid for string/name datatypes */
    const char* rr_str = ares_dns_rr_get_str(rr, key);
    (void)rr_str; /* Prevent unused variable warning */
    
    /* Test ares_dns_rr_set_opt if we have enough data */
    if (Size >= 3) {
        unsigned short opt = (Data[1] << 8) | Data[2];
        const unsigned char* val = (Size > 3) ? Data + 3 : NULL;
        size_t val_len = (Size > 3) ? Size - 3 : 0;
        
        /* Test ares_dns_rr_set_opt */
        ares_status_t set_status = ares_dns_rr_set_opt(rr, key, opt, val, val_len);
        (void)set_status; /* Prevent unused variable warning */
        
        /* Test ares_dns_opt_get_name with the same opt */
        const char* opt_name = ares_dns_opt_get_name(key, opt);
        (void)opt_name; /* Prevent unused variable warning */
        
        /* Test ares_dns_rr_get_opt with index 0 */
        const unsigned char* opt_val = NULL;
        size_t opt_val_len = 0;
        unsigned short get_opt_result = ares_dns_rr_get_opt(rr, key, 0, &opt_val, &opt_val_len);
        (void)get_opt_result; /* Prevent unused variable warning */
        (void)opt_val; /* Prevent unused variable warning */
        (void)opt_val_len; /* Prevent unused variable warning */
    }
    
    /* Cleanup */
    cleanup_test_records(dnsrec);
    
    return 0;
}
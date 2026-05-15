// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_get_u16 at ares_dns_record.c:808:16 in ares_dns_record.h
// ares_dns_rr_get_opt_byid at ares_dns_record.c:1054:13 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_opt_get_datatype at ares_dns_mapping.c:819:25 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Include required headers with proper paths */
#include "ares.h"
#include "ares_dns_record.h"

static void dummy_cleanup(void) {
    // Dummy cleanup function
}

static ares_dns_rr_t* create_test_rr(ares_dns_rec_type_t type) {
    // Create a DNS record first, then add a resource record to it
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, 0, 0, 
                                                  ARES_OPCODE_QUERY, 
                                                  ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || !dnsrec) {
        return NULL;
    }
    
    // Add a resource record to the answer section
    ares_dns_rr_t *rr = NULL;
    status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                    "test.example.com", 
                                    type, ARES_CLASS_IN, 3600);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return NULL;
    }
    
    // We need to keep the parent record alive, so we store it in a static variable
    // This is a hack for the fuzzer - in real code you'd manage this properly
    static ares_dns_record_t *last_dnsrec = NULL;
    if (last_dnsrec) {
        ares_dns_record_destroy(last_dnsrec);
    }
    last_dnsrec = dnsrec;
    
    return rr;
}

static void free_test_rr(ares_dns_rr_t* rr) {
    // The parent DNS record is cleaned up in create_test_rr
    (void)rr;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine test variations
    uint8_t test_selector = Data[0];
    
    // Test ares_dns_rr_get_abin_cnt
    {
        ares_dns_rr_t* rr = create_test_rr(ARES_REC_TYPE_OPT);
        if (rr) {
            // Try different keys with the function
            for (int i = 0; i < 10; i++) {
                ares_dns_rr_key_t key = (ares_dns_rr_key_t)(test_selector + i);
                size_t cnt = ares_dns_rr_get_abin_cnt(rr, key);
                (void)cnt; // Use result to avoid unused variable warning
            }
            free_test_rr(rr);
        }
    }
    
    // Test ares_dns_opt_get_datatype
    {
        // Try various combinations of keys and options
        for (int i = 0; i < 5; i++) {
            ares_dns_rr_key_t key = (ares_dns_rr_key_t)(test_selector * i);
            unsigned short opt = (unsigned short)(Size + i);
            ares_dns_opt_datatype_t dt = ares_dns_opt_get_datatype(key, opt);
            (void)dt; // Use result
        }
    }
    
    // Test ares_dns_rr_set_bin
    {
        ares_dns_rr_t* rr = create_test_rr(ARES_REC_TYPE_TXT);
        if (rr && Size > 1) {
            // Use remaining data for binary input
            const unsigned char* bin_data = Data + 1;
            size_t bin_size = Size - 1;
            if (bin_size > 0) {
                // Try different keys
                for (int i = 0; i < 3; i++) {
                    ares_dns_rr_key_t key = (ares_dns_rr_key_t)(test_selector + i * 10);
                    ares_status_t status = ares_dns_rr_set_bin(rr, key, bin_data, 
                                            bin_size > 100 ? 100 : bin_size);
                    (void)status; // Use result
                }
            }
            free_test_rr(rr);
        }
    }
    
    // Test ares_dns_rr_get_keys
    {
        size_t cnt = 0;
        // Try different record types
        for (int i = 0; i < 5; i++) {
            ares_dns_rec_type_t type = (ares_dns_rec_type_t)(test_selector + i);
            const ares_dns_rr_key_t* keys = ares_dns_rr_get_keys(type, &cnt);
            (void)keys; // Use result
            (void)cnt; // Use result
        }
    }
    
    // Test ares_dns_rr_get_u16
    {
        ares_dns_rr_t* rr = create_test_rr(ARES_REC_TYPE_MX);
        if (rr) {
            // Try different keys
            for (int i = 0; i < 5; i++) {
                ares_dns_rr_key_t key = (ares_dns_rr_key_t)(test_selector + i * 20);
                unsigned short val = ares_dns_rr_get_u16(rr, key);
                (void)val; // Use result
            }
            free_test_rr(rr);
        }
    }
    
    // Test ares_dns_rr_get_opt_byid
    {
        ares_dns_rr_t* rr = create_test_rr(ARES_REC_TYPE_OPT);
        if (rr) {
            const unsigned char* val = NULL;
            size_t val_len = 0;
            
            // Try different keys and option IDs
            for (int i = 0; i < 3; i++) {
                ares_dns_rr_key_t key = (ares_dns_rr_key_t)(test_selector + i * 30);
                unsigned short opt = (unsigned short)(Size + i * 10);
                ares_bool_t result = ares_dns_rr_get_opt_byid(rr, key, opt, &val, &val_len);
                (void)result; // Use result
                (void)val; // Use result
                (void)val_len; // Use result
            }
            free_test_rr(rr);
        }
    }
    
    dummy_cleanup();
    return 0;
}
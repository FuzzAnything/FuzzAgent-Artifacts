// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_get_opt at ares_dns_record.c:1016:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_del_opt_byid at ares_dns_record.c:1428:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_get_opt at ares_dns_record.c:1016:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_del_opt_byid at ares_dns_record.c:1428:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_rr_t* create_test_rr(ares_dns_rec_type_t type) {
    ares_dns_record_t* dns_record = NULL;
    ares_status_t status = ares_dns_record_create(&dns_record, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || !dns_record) {
        return NULL;
    }
    
    ares_dns_rr_t* rr = NULL;
    status = ares_dns_record_rr_add(&rr, dns_record, ARES_SECTION_ADDITIONAL, "example.com", type, ARES_CLASS_IN, 0);
    if (status != ARES_SUCCESS || !rr) {
        ares_dns_record_destroy(dns_record);
        return NULL;
    }
    
    // Store the parent record in the RR for cleanup
    // Note: We cannot access the parent field directly since struct is opaque
    // We'll store it in a way that allows us to clean it up later
    // For simplicity, we'll just keep track of it separately
    return rr;
}

static void cleanup_rr(ares_dns_rr_t* rr, ares_dns_record_t* dns_record) {
    if (dns_record) {
        ares_dns_record_destroy(dns_record);
    }
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use input data to generate various parameter values
    uint8_t key_selector = Data[0] % 255;
    uint8_t opt_selector = Data[1 % Size] % 255;
    uint16_t u16_val = (Data[2 % Size] << 8) | Data[3 % Size];
    size_t idx = Data[4 % Size] % 16;
    
    // Test ares_dns_rr_key_datatype
    ares_dns_rr_key_t key = (ares_dns_rr_key_t)key_selector;
    ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
    (void)datatype;
    
    // Test ares_dns_rr_key_tostr
    const char* key_str = ares_dns_rr_key_tostr(key);
    (void)key_str;
    
    // Test ares_dns_rr_set_u16
    ares_dns_record_t* dns_record1 = NULL;
    ares_status_t status = ares_dns_record_create(&dns_record1, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status == ARES_SUCCESS && dns_record1) {
        ares_dns_rr_t* rr = NULL;
        status = ares_dns_record_rr_add(&rr, dns_record1, ARES_SECTION_ADDITIONAL, "example.com", ARES_REC_TYPE_A, ARES_CLASS_IN, 0);
        if (status == ARES_SUCCESS && rr) {
            status = ares_dns_rr_set_u16(rr, key, u16_val);
            (void)status;
            
            // Test ares_dns_rr_del_opt_byid
            status = ares_dns_rr_del_opt_byid(rr, key, opt_selector);
            (void)status;
            
            // Test ares_dns_rr_set_opt
            const unsigned char* opt_val = (Size > 5) ? Data + 5 : NULL;
            size_t opt_val_len = (Size > 5) ? Size - 5 : 0;
            if (opt_val_len > 256) opt_val_len = 256;
            
            status = ares_dns_rr_set_opt(rr, key, opt_selector, opt_val, opt_val_len);
            (void)status;
            
            // Test ares_dns_rr_get_opt
            const unsigned char* retrieved_val = NULL;
            size_t retrieved_len = 0;
            unsigned short opt_key = ares_dns_rr_get_opt(rr, key, idx, &retrieved_val, &retrieved_len);
            (void)opt_key;
        }
        ares_dns_record_destroy(dns_record1);
    }
    
    // Create OPT record for more comprehensive testing
    ares_dns_record_t* dns_record2 = NULL;
    status = ares_dns_record_create(&dns_record2, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status == ARES_SUCCESS && dns_record2) {
        ares_dns_rr_t* opt_rr = NULL;
        status = ares_dns_record_rr_add(&opt_rr, dns_record2, ARES_SECTION_ADDITIONAL, "example.com", ARES_REC_TYPE_OPT, ARES_CLASS_IN, 0);
        if (status == ARES_SUCCESS && opt_rr) {
            ares_dns_rr_key_t opt_key = ARES_RR_OPT_OPTIONS;
            status = ares_dns_rr_set_opt(opt_rr, opt_key, opt_selector, Data, (Size < 256) ? Size : 256);
            (void)status;
            
            status = ares_dns_rr_del_opt_byid(opt_rr, opt_key, opt_selector);
            (void)status;
            
            const unsigned char* retrieved_val = NULL;
            size_t retrieved_len = 0;
            unsigned short retrieved_opt = ares_dns_rr_get_opt(opt_rr, opt_key, 0, &retrieved_val, &retrieved_len);
            (void)retrieved_opt;
        }
        ares_dns_record_destroy(dns_record2);
    }
    
    // Test with different record types
    static const ares_dns_rec_type_t record_types[] = {
        ARES_REC_TYPE_A,
        ARES_REC_TYPE_NS,
        ARES_REC_TYPE_CNAME,
        ARES_REC_TYPE_SOA,
        ARES_REC_TYPE_PTR,
        ARES_REC_TYPE_MX,
        ARES_REC_TYPE_TXT,
        ARES_REC_TYPE_AAAA,
        ARES_REC_TYPE_SRV,
        ARES_REC_TYPE_OPT,
        ARES_REC_TYPE_URI
    };
    
    for (size_t i = 0; i < sizeof(record_types)/sizeof(record_types[0]) && i < 3; i++) {
        ares_dns_record_t* dns_record = NULL;
        status = ares_dns_record_create(&dns_record, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
        if (status == ARES_SUCCESS && dns_record) {
            ares_dns_rr_t* test_rr = NULL;
            status = ares_dns_record_rr_add(&test_rr, dns_record, ARES_SECTION_ADDITIONAL, "example.com", record_types[i], ARES_CLASS_IN, 0);
            if (status == ARES_SUCCESS && test_rr) {
                ares_dns_rr_key_t test_key = (ares_dns_rr_key_t)((key_selector + i) % 255);
                ares_dns_datatype_t test_datatype = ares_dns_rr_key_datatype(test_key);
                (void)test_datatype;
                
                const char* test_key_str = ares_dns_rr_key_tostr(test_key);
                (void)test_key_str;
                
                if (test_datatype == ARES_DATATYPE_U16) {
                    status = ares_dns_rr_set_u16(test_rr, test_key, u16_val);
                    (void)status;
                }
            }
            ares_dns_record_destroy(dns_record);
        }
    }
    
    return 0;
}
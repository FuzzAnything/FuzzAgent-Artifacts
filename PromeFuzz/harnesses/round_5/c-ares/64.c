// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_get_str at ares_dns_record.c:981:13 in ares_dns_record.h
// ares_dns_opt_get_name at ares_dns_mapping.c:896:13 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_get_opt at ares_dns_record.c:1016:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    // We need at least some data to work with
    if (Size < 2) return 0;
    
    // Create a DNS record structure
    ares_dns_record_t* dns = NULL;
    ares_status_t status = ares_dns_record_create(&dns, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || dns == NULL) {
        return 0;
    }
    
    // Add an OPT resource record to the additional section
    ares_dns_rr_t* rr = NULL;
    status = ares_dns_record_rr_add(&rr, dns, ARES_SECTION_ADDITIONAL, ".", ARES_REC_TYPE_OPT, 0, 0);
    if (status != ARES_SUCCESS || !rr) {
        ares_dns_record_destroy(dns);
        return 0;
    }
    
    // Use fuzzer data to generate key values
    // We'll use the first byte for key selection and second for option selection
    ares_dns_rr_key_t key = (ares_dns_rr_key_t)(Data[0] % 256);
    unsigned short opt = (unsigned short)(Data[1] % 256);
    
    // Test ares_dns_rr_get_str
    const char* str_result = ares_dns_rr_get_str(rr, key);
    (void)str_result;
    
    // Test ares_dns_opt_get_name
    const char* opt_name = ares_dns_opt_get_name(key, opt);
    (void)opt_name;
    
    // Test ares_dns_rr_key_datatype
    ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
    (void)datatype;
    
    // Test ares_dns_rr_key_tostr
    const char* key_str = ares_dns_rr_key_tostr(key);
    (void)key_str;
    
    // Test ares_dns_rr_set_opt
    if (Size > 4) {
        size_t val_len = Size - 2;
        if (val_len > 0) {
            const unsigned char* val = Data + 2;
            ares_status_t set_status = ares_dns_rr_set_opt(rr, key, opt, val, val_len);
            (void)set_status;
            
            // Test ares_dns_rr_get_opt if we successfully set an option
            if (set_status == ARES_SUCCESS) {
                const unsigned char* get_val = NULL;
                size_t get_val_len = 0;
                unsigned short get_opt = ares_dns_rr_get_opt(rr, key, 0, &get_val, &get_val_len);
                (void)get_opt;
                (void)get_val;
                (void)get_val_len;
            }
        }
    }
    
    // Clean up
    ares_dns_record_destroy(dns);
    
    return 0;
}
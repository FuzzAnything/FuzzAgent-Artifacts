// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_opt_get_name at ares_dns_mapping.c:896:13 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_library_init at ares_library_init.c:108:5 in ares.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_str at ares_dns_record.c:981:13 in ares_dns_record.h
// ares_dns_rr_get_opt at ares_dns_record.c:1016:16 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if it's not defined yet */
#ifndef CARES_EXTERN
#define CARES_EXTERN extern
#endif

/* Include required headers */
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"

static void dummy_cleanup(void) {
    // No dynamic resources to clean up in this fuzzer
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for various operations
    if (Size < 4) return 0;

    // Test ares_dns_rr_key_tostr
    // Use first byte to generate key enum value (modulo to reasonable range)
    ares_dns_rr_key_t key1 = (ares_dns_rr_key_t)(Data[0] % 1000);
    const char *str1 = ares_dns_rr_key_tostr(key1);
    (void)str1; // Use result to avoid unused variable warning

    // Test ares_dns_opt_get_name
    // Use second and third bytes for key and opt
    ares_dns_rr_key_t key2 = (ares_dns_rr_key_t)(Data[1] % 1000);
    unsigned short opt = (unsigned short)(Data[2] << 8) | Data[3];
    const char *str2 = ares_dns_opt_get_name(key2, opt);
    (void)str2;

    // Test ares_dns_rr_key_datatype
    // Use fourth byte for key
    ares_dns_rr_key_t key3 = (ares_dns_rr_key_t)(Data[0] % 1000);
    ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key3);
    (void)datatype;

    // Initialize ares library for creating DNS records
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a DNS record with minimal parameters
    ares_dns_record_t *dns_record = NULL;
    ares_status_t create_status = ares_dns_record_create(&dns_record, 
                                                       0,  // id
                                                       0,  // flags
                                                       ARES_OPCODE_QUERY,  // opcode
                                                       ARES_RCODE_NOERROR); // rcode
    if (create_status != ARES_SUCCESS || !dns_record) {
        ares_library_cleanup();
        return 0;
    }

    // Test ares_dns_rr_get_str - need to create a resource record first
    // We'll parse a DNS packet from the input data to get resource records
    ares_dns_record_t *parsed_record = NULL;
    ares_status_t parse_status = ares_dns_parse((const unsigned char *)Data, 
                                               Size, 
                                               0,  // flags
                                               &parsed_record);
    
    if (parse_status == ARES_SUCCESS && parsed_record) {
        // Try to get resource records from the parsed packet
        // Check answer section
        size_t an_count = ares_dns_record_rr_cnt(parsed_record, ARES_SECTION_ANSWER);
        
        // Test ares_dns_rr_get_str if we have answer records
        if (an_count > 0) {
            // Get first answer record
            const ares_dns_rr_t *rr = ares_dns_record_rr_get(parsed_record, 
                                                           ARES_SECTION_ANSWER, 0);
            if (rr) {
                // Test with a few keys
                for (int i = 0; i < 3 && i < (int)Size; i++) {
                    ares_dns_rr_key_t test_key = (ares_dns_rr_key_t)(Data[i] % 1000);
                    const char *result = ares_dns_rr_get_str(rr, test_key);
                    (void)result;
                }
                
                // Test ares_dns_rr_get_opt if we have enough data
                if (Size >= 6) {
                    ares_dns_rr_key_t get_key = (ares_dns_rr_key_t)(Data[4] % 1000);
                    size_t idx = Data[5] % 10;
                    
                    const unsigned char *ret_val = NULL;
                    size_t ret_val_len = 0;
                    unsigned short ret_opt = ares_dns_rr_get_opt(rr, get_key, 
                                                               idx, &ret_val, &ret_val_len);
                    (void)ret_opt;
                    (void)ret_val;
                    (void)ret_val_len;
                }
            }
        }
        
        // Test ares_dns_rr_set_opt - we need a mutable DNS record
        // Create a new record and add an OPT record to test set_opt
        if (Size >= 8) {
            // Create a new DNS record
            ares_dns_record_t *test_record = NULL;
            if (ares_dns_record_create(&test_record, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) == ARES_SUCCESS) {
                // Add an OPT record
                ares_dns_rr_t *opt_rr = NULL;
                // Note: We use ares_dns_rr_new to create a resource record
                // We need to check if this function exists in the headers
                // Since it's not in the provided API, we'll skip this part
                // and only test functions that don't require creating new RRs
                // For now, we'll just use the parsed record's RR if available
                
                // Clean up test record
                ares_dns_record_destroy(test_record);
            }
        }
        
        ares_dns_record_destroy(parsed_record);
    }

    // Clean up
    ares_dns_record_destroy(dns_record);
    ares_library_cleanup();
    dummy_cleanup();
    
    return 0;
}
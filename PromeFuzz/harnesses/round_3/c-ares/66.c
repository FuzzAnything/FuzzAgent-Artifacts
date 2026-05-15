// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_rr_del_abin at ares_dns_record.c:917:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_rr_get_opt_cnt at ares_dns_record.c:999:8 in ares_dns_record.h
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_rr_del_abin at ares_dns_record.c:917:15 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_rr_get_opt_cnt at ares_dns_record.c:999:8 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ares.h"
#include "ares_dns_record.h"

static void fuzz_target_functions(const uint8_t *data, size_t size) {
    // Create a DNS record to work with
    ares_dns_record_t *dns_record = NULL;
    ares_status_t status;
    
    // Create a new record with correct number of arguments
    status = ares_dns_record_create(&dns_record, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS) {
        return;
    }
    
    // Create and add an OPT resource record for testing
    ares_dns_rr_t *rr = NULL;
    status = ares_dns_record_rr_add(&rr, dns_record, ARES_SECTION_ADDITIONAL, 
                                   "example.com", ARES_REC_TYPE_OPT, ARES_CLASS_IN, 0);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dns_record);
        return;
    }
    
    // Test ares_dns_rr_get_keys with various record types
    size_t key_cnt = 0;
    for (int type = ARES_REC_TYPE_A; type <= ARES_REC_TYPE_URI; type++) {
        const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys((ares_dns_rec_type_t)type, &key_cnt);
        (void)keys;
    }
    
    // Get keys for OPT record type specifically
    const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(ARES_REC_TYPE_OPT, &key_cnt);
    
    if (keys && key_cnt > 0) {
        // Test each function with the available keys
        for (size_t i = 0; i < key_cnt; i++) {
            ares_dns_rr_key_t key = keys[i];
            
            // Test ares_dns_rr_get_abin_cnt
            size_t abin_cnt = ares_dns_rr_get_abin_cnt(rr, key);
            
            // Test ares_dns_rr_get_opt_cnt
            size_t opt_cnt = ares_dns_rr_get_opt_cnt(rr, key);
            
            // Test ares_dns_rr_add_abin if we have data
            if (size > 0) {
                size_t data_len = size > 1024 ? 1024 : size;
                ares_dns_rr_add_abin(rr, key, data, data_len);
                
                // Update count after addition
                abin_cnt = ares_dns_rr_get_abin_cnt(rr, key);
                
                // Test ares_dns_rr_del_abin if we have elements
                if (abin_cnt > 0) {
                    size_t idx_to_delete = 0;
                    if (size > 0) {
                        idx_to_delete = data[0] % abin_cnt;
                    }
                    ares_dns_rr_del_abin(rr, key, idx_to_delete);
                }
            }
            
            // Test ares_dns_rr_set_bin if we have data
            if (size > 0) {
                size_t data_len = size > 512 ? 512 : size;
                ares_dns_rr_set_bin(rr, key, data, data_len);
            }
        }
    }
    
    // Create and add another RR of different type for more coverage
    ares_dns_rr_t *rr2 = NULL;
    status = ares_dns_record_rr_add(&rr2, dns_record, ARES_SECTION_ANSWER, 
                                   "test.com", ARES_REC_TYPE_A, ARES_CLASS_IN, 3600);
    if (status == ARES_SUCCESS) {
        // Test functions on this RR too
        const ares_dns_rr_key_t *a_keys = ares_dns_rr_get_keys(ARES_REC_TYPE_A, &key_cnt);
        if (a_keys && key_cnt > 0 && size > 0) {
            for (size_t i = 0; i < key_cnt && i < 3; i++) {
                ares_dns_rr_key_t key = a_keys[i];
                
                // Test get functions
                size_t abin_cnt = ares_dns_rr_get_abin_cnt(rr2, key);
                size_t opt_cnt = ares_dns_rr_get_opt_cnt(rr2, key);
                
                // Test set_bin if we have data
                size_t data_len = size > 256 ? 256 : size;
                ares_dns_rr_set_bin(rr2, key, data, data_len);
                
                // Test add_abin if we have data
                ares_dns_rr_add_abin(rr2, key, data, data_len);
                
                // Test del_abin if we added something
                abin_cnt = ares_dns_rr_get_abin_cnt(rr2, key);
                if (abin_cnt > 0) {
                    size_t idx_to_delete = 0;
                    if (size > 1) {
                        idx_to_delete = data[1] % abin_cnt;
                    }
                    ares_dns_rr_del_abin(rr2, key, idx_to_delete);
                }
            }
        }
    }
    
    // Cleanup
    ares_dns_record_destroy(dns_record);
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    fuzz_target_functions(Data, Size);
    
    return 0;
}
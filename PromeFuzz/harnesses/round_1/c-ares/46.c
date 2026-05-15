// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_rr_get_abin at ares_dns_record.c:899:22 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_rr_get_bin at ares_dns_record.c:842:22 in ares_dns_record.h
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_rr_get_abin at ares_dns_record.c:899:22 in ares_dns_record.h
// ares_dns_rr_del_abin at ares_dns_record.c:917:15 in ares_dns_record.h
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_rr_get_abin at ares_dns_record.c:899:22 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_rr_del_abin at ares_dns_record.c:917:15 in ares_dns_record.h
#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static void fuzz_apis(const uint8_t *Data, size_t Size) {
    // Create a dummy DNS record to work with
    ares_dns_record_t *dns_record = NULL;
    ares_status_t status = ares_dns_record_create(&dns_record, 12345, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || !dns_record) return;
    
    // Create a resource record - use a type that likely has BIN/BINP/ABINP fields
    // For TXT records, the RDATA field is ARES_RR_TXT_DATA which is typically BINP type
    ares_dns_rr_t *rr = NULL;
    status = ares_dns_record_rr_add(&rr, dns_record, ARES_SECTION_ANSWER, 
                                    "example.com", ARES_REC_TYPE_TXT, 
                                    ARES_CLASS_IN, 300);
    if (status != ARES_SUCCESS || !rr) {
        ares_dns_record_destroy(dns_record);
        return;
    }
    
    // For fuzzing, we'll use some common keys that might be BIN/BINP/ABINP types
    // TXT RDATA field is typically BINP type
    ares_dns_rr_key_t binp_key = ARES_RR_TXT_DATA;
    
    // Test 1: ares_dns_rr_set_bin
    if (Size > 0) {
        ares_dns_rr_set_bin(rr, binp_key, Data, Size);
    }
    
    // Test 2: ares_dns_rr_get_bin
    size_t len = 0;
    const unsigned char *bin_data = ares_dns_rr_get_bin(rr, binp_key, &len);
    // Use the result to avoid optimization
    if (bin_data && len > 0) {
        volatile char dummy = bin_data[0];
        (void)dummy;
    }
    
    // Test 3: ares_dns_rr_add_abin (if ABINP type)
    // Add some binary data to array
    if (Size > 0) {
        unsigned char small_data[1] = {Data[0] % 256};
        ares_dns_rr_add_abin(rr, binp_key, small_data, 1);
    }
    
    // Test 4: ares_dns_rr_get_abin_cnt
    size_t cnt = ares_dns_rr_get_abin_cnt(rr, binp_key);
    // Use cnt to avoid optimization
    volatile size_t dummy_cnt = cnt;
    (void)dummy_cnt;
    
    // Test 5: ares_dns_rr_get_abin
    if (cnt > 0) {
        size_t abin_len = 0;
        const unsigned char *abin_data = ares_dns_rr_get_abin(rr, binp_key, 0, &abin_len);
        if (abin_data && abin_len > 0) {
            volatile char dummy_abin = abin_data[0];
            (void)dummy_abin;
        }
    }
    
    // Test 6: ares_dns_rr_del_abin
    if (cnt > 0) {
        ares_dns_rr_del_abin(rr, binp_key, 0);
    }
    
    // Add more data for additional testing
    if (Size > 1) {
        unsigned char more_data[1] = {Data[1] % 256};
        ares_dns_rr_add_abin(rr, binp_key, more_data, 1);
    }
    
    // Test get_abin_cnt again after modifications
    cnt = ares_dns_rr_get_abin_cnt(rr, binp_key);
    
    // Test get_abin with various indices
    for (size_t i = 0; i < cnt && i < 3; i++) {
        size_t tmp_len = 0;
        const unsigned char *tmp_data = ares_dns_rr_get_abin(rr, binp_key, i, &tmp_len);
        if (tmp_data && tmp_len > 0) {
            volatile char dummy_tmp = tmp_data[0];
            (void)dummy_tmp;
        }
    }
    
    // Test deletion of multiple elements
    while (ares_dns_rr_get_abin_cnt(rr, binp_key) > 0) {
        ares_dns_rr_del_abin(rr, binp_key, 0);
    }
    
    // Test with empty state
    cnt = ares_dns_rr_get_abin_cnt(rr, binp_key);
    size_t tmp_len = 0;
    const unsigned char *tmp_data = ares_dns_rr_get_abin(rr, binp_key, 0, &tmp_len);
    
    // Clean up
    ares_dns_record_destroy(dns_record);
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data
    if (Size == 0) {
        // Still test with empty input
        uint8_t dummy = 0;
        fuzz_apis(&dummy, 0);
        return 0;
    }
    
    fuzz_apis(Data, Size);
    
    return 0;
}
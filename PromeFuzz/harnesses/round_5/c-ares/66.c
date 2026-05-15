// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_set_u8 at ares_dns_record.c:1140:15 in ares_dns_record.h
// ares_dns_rr_get_u8 at ares_dns_record.c:791:15 in ares_dns_record.h
// ares_dns_opt_get_datatype at ares_dns_mapping.c:819:25 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_u8 at ares_dns_record.c:1140:15 in ares_dns_record.h
// ares_dns_rr_get_u8 at ares_dns_record.c:791:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_get_u16 at ares_dns_record.c:808:16 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_opt_get_datatype at ares_dns_mapping.c:819:25 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_rr_t* create_test_rr(ares_dns_rec_type_t type) {
    ares_dns_record_t* dns = NULL;
    ares_status_t status = ares_dns_record_create(&dns, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS) {
        return NULL;
    }
    
    ares_dns_rr_t* rr = NULL;
    const char* name = (type == ARES_REC_TYPE_OPT) ? "" : "example.com";
    unsigned int ttl = (type == ARES_REC_TYPE_OPT) ? 0 : 3600;
    
    status = ares_dns_record_rr_add(&rr, dns, ARES_SECTION_ANSWER, name, 
                                    type, ARES_CLASS_IN, ttl);
    if (status != ARES_SUCCESS || !rr) {
        ares_dns_record_destroy(dns);
        return NULL;
    }
    
    return rr;
}

static void cleanup_rr(ares_dns_rr_t* rr) {
    if (rr) {
        ares_dns_record_t* dns = NULL;
        ares_dns_section_t sections[] = {ARES_SECTION_ANSWER, ARES_SECTION_AUTHORITY, 
                                         ARES_SECTION_ADDITIONAL};
        for (int i = 0; i < 3; i++) {
            for (size_t j = 0; j < ares_dns_record_rr_cnt(dns, sections[i]); j++) {
                const ares_dns_rr_t* current_rr = ares_dns_record_rr_get(dns, sections[i], j);
                if (current_rr == rr) {
                    ares_dns_record_destroy(dns);
                    return;
                }
            }
        }
    }
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    // Create test resource records
    ares_dns_rr_t* rr_a = create_test_rr(ARES_REC_TYPE_A);
    ares_dns_rr_t* rr_opt = create_test_rr(ARES_REC_TYPE_OPT);
    
    if (!rr_a || !rr_opt) {
        if (rr_a) cleanup_rr(rr_a);
        if (rr_opt) cleanup_rr(rr_opt);
        return 0;
    }
    
    // Extract values from fuzzer input
    unsigned char u8_val = Data[0];
    unsigned short u16_val = (Data[1] << 8) | Data[2];
    ares_dns_rr_key_t key = (ares_dns_rr_key_t)(Data[3] % 1000);
    unsigned short opt_key = (Data[0] << 8) | Data[1];
    
    // Test ares_dns_rr_set_u8 and ares_dns_rr_get_u8
    ares_status_t status = ares_dns_rr_set_u8(rr_a, key, u8_val);
    if (status == ARES_SUCCESS) {
        unsigned char retrieved = ares_dns_rr_get_u8(rr_a, key);
        (void)retrieved;
    }
    
    // Test ares_dns_rr_set_u16 and ares_dns_rr_get_u16
    status = ares_dns_rr_set_u16(rr_a, key, u16_val);
    if (status == ARES_SUCCESS) {
        unsigned short retrieved = ares_dns_rr_get_u16(rr_a, key);
        (void)retrieved;
    }
    
    // Test ares_dns_rr_set_bin
    if (Size > 4) {
        const unsigned char* bin_data = Data + 4;
        size_t bin_len = Size - 4;
        if (bin_len > 0) {
            status = ares_dns_rr_set_bin(rr_a, key, bin_data, bin_len);
            (void)status;
        }
    }
    
    // Test ares_dns_opt_get_datatype
    ares_dns_opt_datatype_t dt = ares_dns_opt_get_datatype(key, opt_key);
    (void)dt;
    
    // Test with different keys to explore more states
    for (int i = 0; i < 5 && i * 2 + 1 < (int)Size; i++) {
        ares_dns_rr_key_t alt_key = (ares_dns_rr_key_t)(Data[i * 2] % 1000);
        unsigned char alt_u8 = Data[i * 2 + 1];
        
        ares_dns_rr_set_u8(rr_opt, alt_key, alt_u8);
        unsigned char retrieved = ares_dns_rr_get_u8(rr_opt, alt_key);
        (void)retrieved;
        
        unsigned short alt_opt = (Data[i * 2] << 8) | Data[i * 2 + 1];
        dt = ares_dns_opt_get_datatype(alt_key, alt_opt);
        (void)dt;
    }
    
    // Cleanup
    cleanup_rr(rr_a);
    cleanup_rr(rr_opt);
    
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_set_ttl at ares_dns_record.c:537:15 in ares_dns_record.h
// ares_dns_rr_get_ttl at ares_dns_record.c:529:14 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_rr_del at ares_dns_record.c:450:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_record_t *create_test_dns_record(const uint8_t *Data, size_t Size) {
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status;
    
    // Use fuzzer data to create parameters
    unsigned short id = Size > 0 ? Data[0] : 1234;
    unsigned short flags = Size > 1 ? Data[1] : 0;
    ares_dns_opcode_t opcode = Size > 2 ? (ares_dns_opcode_t)(Data[2] % 16) : ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = Size > 3 ? (ares_dns_rcode_t)(Data[3] % 16) : ARES_RCODE_NOERROR;
    
    status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || !dnsrec) {
        return NULL;
    }
    
    // Add some dummy resource records to work with
    const char *name = "example.com";
    ares_dns_rr_t *rr_out = NULL;
    
    // Add a few records to different sections
    if (Size > 4) {
        ares_dns_record_rr_add(&rr_out, dnsrec, ARES_SECTION_ANSWER, name, 
                              ARES_REC_TYPE_A, ARES_CLASS_IN, 300);
    }
    if (Size > 5) {
        ares_dns_record_rr_add(&rr_out, dnsrec, ARES_SECTION_ANSWER, name, 
                              ARES_REC_TYPE_AAAA, ARES_CLASS_IN, 300);
    }
    if (Size > 6) {
        ares_dns_record_rr_add(&rr_out, dnsrec, ARES_SECTION_AUTHORITY, name, 
                              ARES_REC_TYPE_NS, ARES_CLASS_IN, 300);
    }
    if (Size > 7) {
        ares_dns_record_rr_add(&rr_out, dnsrec, ARES_SECTION_ADDITIONAL, name, 
                              ARES_REC_TYPE_OPT, ARES_CLASS_IN, 300);
    }
    
    return dnsrec;
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    // Initialize a DNS record to work with
    ares_dns_record_t *dnsrec = create_test_dns_record(Data, Size);
    if (!dnsrec) {
        return 0;
    }
    
    // Use fuzzer input to determine parameters
    uint8_t section_idx = Data[0] % 3;  // 0-2 for AN, NS, AR (skip QD section)
    ares_dns_section_t section;
    switch (section_idx) {
        case 0: section = ARES_SECTION_ANSWER; break;
        case 1: section = ARES_SECTION_AUTHORITY; break;
        case 2: section = ARES_SECTION_ADDITIONAL; break;
        default: section = ARES_SECTION_ANSWER; break;
    }
    
    // Get record count in the selected section
    size_t rr_count = ares_dns_record_rr_cnt(dnsrec, section);
    if (rr_count == 0) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    // Use fuzzer input to determine index (wrap around if needed)
    size_t idx = (size_t)Data[1] % rr_count;
    
    // Test ares_dns_record_rr_del
    if (Size > 2 && (Data[2] % 2) == 0) {
        ares_dns_record_rr_del(dnsrec, section, idx);
        // Update count after deletion
        rr_count = ares_dns_record_rr_cnt(dnsrec, section);
        if (rr_count > 0) {
            idx = idx % rr_count;
        }
    }
    
    // Test ares_dns_record_rr_get
    ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, section, idx);
    if (rr) {
        // Test ares_dns_rr_set_ttl and ares_dns_rr_get_ttl
        unsigned int ttl = 0;
        if (Size > 6) {
            ttl = (unsigned int)Data[3 % Size] << 24 | 
                  (unsigned int)Data[4 % Size] << 16 |
                  (unsigned int)Data[5 % Size] << 8 |
                  (unsigned int)Data[6 % Size];
        } else {
            ttl = (unsigned int)Data[0] * 1000;
        }
        ares_dns_rr_set_ttl(rr, ttl);
        unsigned int retrieved_ttl = ares_dns_rr_get_ttl(rr);
        
        // Test ares_dns_rr_set_u16 with various keys
        if (Size > 7) {
            unsigned short u16_val = (unsigned short)(Data[7 % Size] << 8 | Data[8 % Size]);
            // Try different U16 keys based on record type
            ares_dns_rr_key_t u16_key;
            
            // Use fuzzer data to select from known U16 keys
            uint8_t key_selector = Data[9 % Size] % 3;
            switch (key_selector) {
                case 0:
                    u16_key = ARES_RR_MX_PREFERENCE;
                    break;
                case 1:
                    u16_key = ARES_RR_SRV_PRIORITY;
                    break;
                case 2:
                    u16_key = ARES_RR_OPT_UDP_SIZE;
                    break;
                default:
                    u16_key = ARES_RR_MX_PREFERENCE;
                    break;
            }
            
            ares_dns_rr_set_u16(rr, u16_key, u16_val);
        }
        
        // Test ares_dns_rr_add_abin
        if (Size > 10) {
            // Use TXT_DATA as a known ABINP key
            ares_dns_rr_key_t abin_key = ARES_RR_TXT_DATA;
            
            // Use remaining data for binary input
            size_t abin_len = Size > 20 ? Size - 20 : 1;
            const unsigned char *abin_data = Data + (20 % Size);
            
            ares_dns_rr_add_abin(rr, abin_key, abin_data, abin_len);
        }
    }
    
    // Clean up
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
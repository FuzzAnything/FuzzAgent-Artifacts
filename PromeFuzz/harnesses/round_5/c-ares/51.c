// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_get_const at ares_dns_record.c:499:3 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_del at ares_dns_record.c:450:15 in ares_dns_record.h
// ares_dns_record_rr_get_const at ares_dns_record.c:499:3 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_del at ares_dns_record.c:450:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Include required headers in the correct order
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_record_t *create_test_record(void) {
    ares_dns_record_t *record = NULL;
    // Create record with default values
    ares_status_t status = ares_dns_record_create(&record, 
                                                  1234,      // id
                                                  0x0100,    // flags (standard query, recursion desired)
                                                  ARES_OPCODE_QUERY,  // opcode
                                                  ARES_RCODE_NOERROR); // rcode
    if (status != ARES_SUCCESS) {
        return NULL;
    }
    
    return record;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a test DNS record
    ares_dns_record_t *record = create_test_record();
    if (!record) return 0;
    
    // Use fuzzer input to determine parameters
    // Use first byte to determine section (0-3 for valid sections)
    ares_dns_section_t sect = (ares_dns_section_t)(Data[0] % 4);
    
    // Use second byte (if available) for index
    size_t idx = 0;
    if (Size > 1) {
        idx = Data[1];
    }
    
    // Test ares_dns_record_rr_get_const
    const ares_dns_rr_t *const_rr = ares_dns_record_rr_get_const(record, sect, idx);
    (void)const_rr;
    
    // Test ares_dns_record_rr_get
    ares_dns_rr_t *rr = ares_dns_record_rr_get(record, sect, idx);
    (void)rr;
    
    // Test ares_dns_record_get_flags
    unsigned short flags = ares_dns_record_get_flags(record);
    (void)flags;
    
    // Test ares_dns_record_rr_cnt for different sections
    // Use integer values 0-3 for sections
    for (int i = 0; i < 4; i++) {
        size_t cnt = ares_dns_record_rr_cnt(record, (ares_dns_section_t)i);
        (void)cnt;
    }
    
    // Test ares_dns_record_rr_del if we have an index
    if (Size > 2) {
        // Use third byte for deletion index
        size_t del_idx = Data[2];
        ares_status_t status = ares_dns_record_rr_del(record, sect, del_idx);
        (void)status;
    }
    
    // Try different sections and indices based on fuzzer input
    for (size_t i = 0; i < Size && i < 8; i++) {
        ares_dns_section_t test_sect = (ares_dns_section_t)Data[i];
        size_t test_idx = (i < Size - 1) ? Data[i + 1] : 0;
        
        // Test get_const with various inputs
        const ares_dns_rr_t *test_const_rr = ares_dns_record_rr_get_const(record, test_sect, test_idx);
        (void)test_const_rr;
        
        // Test get with various inputs
        ares_dns_rr_t *test_rr = ares_dns_record_rr_get(record, test_sect, test_idx);
        (void)test_rr;
        
        // Test rr_cnt with various sections
        size_t test_cnt = ares_dns_record_rr_cnt(record, test_sect);
        (void)test_cnt;
        
        // Test rr_del with various inputs (skip if i would overflow)
        if (i + 2 < Size) {
            size_t del_test_idx = Data[i + 2];
            ares_status_t del_status = ares_dns_record_rr_del(record, test_sect, del_test_idx);
            (void)del_status;
        }
    }
    
    // Clean up
    ares_dns_record_destroy(record);
    
    return 0;
}
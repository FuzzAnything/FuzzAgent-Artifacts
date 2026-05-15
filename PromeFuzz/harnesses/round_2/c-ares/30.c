// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_record_set_id at ares_dns_record.c:100:13 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_record_set_id at ares_dns_record.c:100:13 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_record_set_id at ares_dns_record.c:100:13 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include ares.h first to get CARES_EXTERN definition */
#include "ares.h"
/* Then include the required header */
#include "ares_dns_record.h"

static void test_parsing(const uint8_t *data, size_t size) {
    ares_dns_record_t *record = NULL;
    ares_status_t status = ares_dns_parse(data, size, 0, &record);
    
    if (status == ARES_SUCCESS && record != NULL) {
        // Test getter functions on parsed record
        (void)ares_dns_record_get_flags(record);
        (void)ares_dns_record_get_id(record);
        (void)ares_dns_record_get_opcode(record);
        
        // Test setting a new ID
        if (size > 0) {
            unsigned short new_id = (data[0] << 8) | (size > 1 ? data[1] : 0);
            (void)ares_dns_record_set_id(record, new_id);
            (void)ares_dns_record_get_id(record);
        }
        
        ares_dns_record_destroy(record);
    }
}

static void test_creation(const uint8_t *data, size_t size) {
    if (size < 6) {
        return;
    }
    
    // Extract parameters from fuzzer input
    unsigned short id = (data[0] << 8) | data[1];
    unsigned short flags = (data[2] << 8) | data[3];
    ares_dns_opcode_t opcode = (ares_dns_opcode_t)(data[4] % 16);  // Valid opcodes are 0-15
    ares_dns_rcode_t rcode = (ares_dns_rcode_t)(data[5] % 16);    // Valid rcodes are 0-15
    
    ares_dns_record_t *record = NULL;
    ares_status_t status = ares_dns_record_create(&record, id, flags, opcode, rcode);
    
    if (status == ARES_SUCCESS && record != NULL) {
        // Test getter functions
        (void)ares_dns_record_get_flags(record);
        unsigned short retrieved_id = ares_dns_record_get_id(record);
        (void)retrieved_id;
        (void)ares_dns_record_get_opcode(record);
        
        // Test setting a different ID
        if (size > 7) {
            unsigned short new_id = (data[6] << 8) | data[7];
            (void)ares_dns_record_set_id(record, new_id);
            
            // Verify the new ID was set
            unsigned short verify_id = ares_dns_record_get_id(record);
            (void)verify_id;
        }
        
        ares_dns_record_destroy(record);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Test parsing the input as a DNS message
    test_parsing(Data, Size);
    
    // Test creating a new DNS record with parameters from the input
    test_creation(Data, Size);
    
    // Additional test: Create with edge cases using remaining data
    if (Size > 12) {
        // Test with various combinations of flags and opcodes
        for (int i = 0; i < 3 && i + 8 < Size; i++) {
            unsigned short test_id = (Data[8 + i] << 8) | (i + 9 < Size ? Data[8 + i + 1] : 0);
            unsigned short test_flags = (Data[8 + i] << 8) | (i + 10 < Size ? Data[8 + i + 2] : 0);
            ares_dns_opcode_t test_opcode = (ares_dns_opcode_t)(Data[8 + i] % 16);
            ares_dns_rcode_t test_rcode = (ares_dns_rcode_t)((Data[8 + i] + i) % 16);
            
            ares_dns_record_t *test_record = NULL;
            ares_status_t test_status = ares_dns_record_create(&test_record, test_id, test_flags, test_opcode, test_rcode);
            
            if (test_status == ARES_SUCCESS && test_record != NULL) {
                (void)ares_dns_record_get_flags(test_record);
                (void)ares_dns_record_get_id(test_record);
                (void)ares_dns_record_get_opcode(test_record);
                
                // Try setting ID to 0 (valid for new queries)
                (void)ares_dns_record_set_id(test_record, 0);
                
                ares_dns_record_destroy(test_record);
            }
        }
    }
    
    return 0;
}
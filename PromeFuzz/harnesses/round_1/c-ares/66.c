// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_set_id at ares_dns_record.c:100:13 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_set_id at ares_dns_record.c:100:13 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef CARES_EXTERN
#define CARES_EXTERN extern
#endif

/* Include required headers in correct order */
#include "ares.h"
#include "ares_dns_record.h"

static void test_parse_and_inspect(const uint8_t *data, size_t size) {
    ares_dns_record_t *parsed_record = NULL;
    ares_status_t parse_status = ares_dns_parse(data, size, 0, &parsed_record);
    
    if (parse_status == ARES_SUCCESS && parsed_record != NULL) {
        // Test getter functions on successfully parsed record
        unsigned short flags = ares_dns_record_get_flags(parsed_record);
        ares_dns_opcode_t opcode = ares_dns_record_get_opcode(parsed_record);
        unsigned short id = ares_dns_record_get_id(parsed_record);
        
        // Use values to avoid optimization
        (void)flags;
        (void)opcode;
        (void)id;
        
        // Try setting a new ID derived from input data
        unsigned short new_id = 0;
        if (size >= 2) {
            new_id = (data[0] << 8) | data[1];
        }
        ares_bool_t set_result = ares_dns_record_set_id(parsed_record, new_id);
        (void)set_result;
        
        // Clean up
        ares_dns_record_destroy(parsed_record);
    }
}

static void test_create_and_manipulate(const uint8_t *data, size_t size) {
    ares_dns_record_t *created_record = NULL;
    
    // Extract parameters from fuzzer input with bounds checking
    unsigned short id = 0;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    if (size >= 2) id = (data[0] << 8) | data[1];
    if (size >= 4) flags = (data[2] << 8) | data[3];
    if (size >= 5) opcode = data[4] % 16; // Opcode is 4-bit field
    if (size >= 6) rcode = data[5] % 16;  // Rcode is 4-bit field
    
    ares_status_t create_status = ares_dns_record_create(&created_record, id, flags, opcode, rcode);
    
    if (create_status == ARES_SUCCESS && created_record != NULL) {
        // Test getter functions on newly created record
        unsigned short retrieved_flags = ares_dns_record_get_flags(created_record);
        ares_dns_opcode_t retrieved_opcode = ares_dns_record_get_opcode(created_record);
        unsigned short retrieved_id = ares_dns_record_get_id(created_record);
        
        // Use values to avoid optimization
        (void)retrieved_flags;
        (void)retrieved_opcode;
        (void)retrieved_id;
        
        // Try setting a new ID derived from input data (using different bytes)
        unsigned short new_id = 0;
        if (size >= 8) {
            new_id = (data[6] << 8) | data[7];
        } else if (size >= 2) {
            new_id = (data[size-2] << 8) | data[size-1];
        }
        ares_bool_t set_result = ares_dns_record_set_id(created_record, new_id);
        (void)set_result;
        
        // Test getter again after modification
        unsigned short modified_id = ares_dns_record_get_id(created_record);
        (void)modified_id;
        
        // Clean up
        ares_dns_record_destroy(created_record);
    }
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Test parsing raw DNS message
    test_parse_and_inspect(Data, Size);
    
    // Test creating and manipulating DNS record
    test_create_and_manipulate(Data, Size);
    
    return 0;
}
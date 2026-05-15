// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_record_set_id at ares_dns_record.c:100:13 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_record_set_id at ares_dns_record.c:100:13 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static unsigned short get_ushort_from_data(const uint8_t *data, size_t size, size_t offset) {
    if (offset + 1 < size) {
        return (data[offset] << 8) | data[offset + 1];
    }
    return 0;
}

static unsigned char get_uchar_from_data(const uint8_t *data, size_t size, size_t offset) {
    if (offset < size) {
        return data[offset];
    }
    return 0;
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    // Test ares_dns_parse with the entire input buffer
    ares_dns_record_t *parsed_record = NULL;
    ares_status_t parse_status = ares_dns_parse(Data, Size, 0, &parsed_record);
    
    if (parse_status == ARES_SUCCESS && parsed_record != NULL) {
        // Test getter functions on parsed record
        unsigned short parsed_id = ares_dns_record_get_id(parsed_record);
        unsigned short parsed_flags = ares_dns_record_get_flags(parsed_record);
        ares_dns_opcode_t parsed_opcode = ares_dns_record_get_opcode(parsed_record);
        
        // Test setter function on parsed record
        unsigned short new_id = get_ushort_from_data(Data, Size, 0);
        ares_dns_record_set_id(parsed_record, new_id);
        
        // Verify the change
        unsigned short updated_id = ares_dns_record_get_id(parsed_record);
        
        // Clean up parsed record
        ares_dns_record_destroy(parsed_record);
    }
    
    // Test ares_dns_record_create with parameters derived from input
    ares_dns_record_t *created_record = NULL;
    
    // Extract parameters from input data with bounds checking
    unsigned short create_id = get_ushort_from_data(Data, Size, 0);
    unsigned short create_flags = get_ushort_from_data(Data, Size, 2);
    ares_dns_opcode_t create_opcode = (ares_dns_opcode_t)get_uchar_from_data(Data, Size, 4);
    ares_dns_rcode_t create_rcode = (ares_dns_rcode_t)get_uchar_from_data(Data, Size, 5);
    
    ares_status_t create_status = ares_dns_record_create(&created_record, 
                                                         create_id, 
                                                         create_flags, 
                                                         create_opcode, 
                                                         create_rcode);
    
    if (create_status == ARES_SUCCESS && created_record != NULL) {
        // Test getter functions on created record
        unsigned short created_id = ares_dns_record_get_id(created_record);
        unsigned short created_flags = ares_dns_record_get_flags(created_record);
        ares_dns_opcode_t created_opcode = ares_dns_record_get_opcode(created_record);
        
        // Test setter function on created record
        unsigned short set_id = get_ushort_from_data(Data, Size, 6);
        ares_dns_record_set_id(created_record, set_id);
        
        // Verify the change
        unsigned short final_id = ares_dns_record_get_id(created_record);
        
        // Clean up created record
        ares_dns_record_destroy(created_record);
    }
    
    return 0;
}
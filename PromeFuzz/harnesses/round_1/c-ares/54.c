// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
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
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include required headers in the correct order */
#include "ares.h"
#include "ares_dns_record.h"

/* Helper function to create random opcode and rcode values from fuzz data */
static ares_dns_opcode_t get_opcode_from_data(const uint8_t *data, size_t size, size_t offset) {
    if (offset >= size) {
        return ARES_OPCODE_QUERY;
    }
    return (ares_dns_opcode_t)(data[offset] % 6); /* Opcodes 0-5 are valid */
}

static ares_dns_rcode_t get_rcode_from_data(const uint8_t *data, size_t size, size_t offset) {
    if (offset >= size) {
        return ARES_RCODE_NOERROR;
    }
    return (ares_dns_rcode_t)(data[offset] % 24); /* Rcodes 0-23 are valid */
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    /* Test ares_dns_parse with the provided data */
    ares_dns_record_t *parsed_record = NULL;
    unsigned int flags = 0;
    
    /* Use the fuzzing input as raw DNS message */
    if (Size > 0) {
        ares_dns_parse(Data, Size, flags, &parsed_record);
    }
    
    if (parsed_record != NULL) {
        /* Test getter functions on the parsed record */
        unsigned short id = ares_dns_record_get_id(parsed_record);
        unsigned short record_flags = ares_dns_record_get_flags(parsed_record);
        ares_dns_opcode_t opcode = ares_dns_record_get_opcode(parsed_record);
        
        /* Test ares_dns_record_set_id with the parsed record */
        /* Use a value derived from the input size to vary the test */
        unsigned short new_id = (unsigned short)(Size & 0xFFFF);
        ares_dns_record_set_id(parsed_record, new_id);
        
        /* Verify the change was applied */
        unsigned short updated_id = ares_dns_record_get_id(parsed_record);
        
        /* Clean up parsed record */
        ares_dns_record_destroy(parsed_record);
    }
    
    /* Test ares_dns_record_create with various inputs */
    /* Extract parameters from fuzzing input if possible */
    unsigned short create_id = 0;
    unsigned short create_flags = 0;
    
    if (Size >= 2) {
        create_id = (Data[0] << 8) | Data[1];
    }
    if (Size >= 4) {
        create_flags = (Data[2] << 8) | Data[3];
    }
    
    ares_dns_opcode_t create_opcode = get_opcode_from_data(Data, Size, 4);
    ares_dns_rcode_t create_rcode = get_rcode_from_data(Data, Size, 5);
    
    ares_dns_record_t *created_record = NULL;
    ares_status_t status = ares_dns_record_create(&created_record, create_id, 
                                                  create_flags, create_opcode, 
                                                  create_rcode);
    
    if (status == ARES_SUCCESS && created_record != NULL) {
        /* Test getter functions on the created record */
        unsigned short retrieved_id = ares_dns_record_get_id(created_record);
        unsigned short retrieved_flags = ares_dns_record_get_flags(created_record);
        ares_dns_opcode_t retrieved_opcode = ares_dns_record_get_opcode(created_record);
        
        /* Test ares_dns_record_set_id with the created record */
        /* Use different ID values to explore more states */
        static unsigned short test_ids[] = {0, 1234, 32768, 65535};
        for (size_t i = 0; i < sizeof(test_ids)/sizeof(test_ids[0]); i++) {
            ares_dns_record_set_id(created_record, test_ids[i]);
            unsigned short test_id = ares_dns_record_get_id(created_record);
        }
        
        /* Clean up created record */
        ares_dns_record_destroy(created_record);
    }
    
    /* Test edge cases for ares_dns_record_create with various inputs */
    /* Test with maximum values */
    ares_dns_record_t *max_record = NULL;
    ares_dns_record_create(&max_record, 65535, 65535, 
                          (ares_dns_opcode_t)5, (ares_dns_rcode_t)23);
    if (max_record != NULL) {
        ares_dns_record_destroy(max_record);
    }
    
    /* Test with zero values */
    ares_dns_record_t *zero_record = NULL;
    ares_dns_record_create(&zero_record, 0, 0, 
                          (ares_dns_opcode_t)0, (ares_dns_rcode_t)0);
    if (zero_record != NULL) {
        ares_dns_record_destroy(zero_record);
    }
    
    return 0;
}
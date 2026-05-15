// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_set_id at ares_dns_record.c:100:13 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_set_id at ares_dns_record.c:100:13 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ares.h"
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    // Test ares_dns_parse with the fuzzer input
    ares_dns_record_t *parsed_record = NULL;
    ares_status_t parse_status = ares_dns_parse(Data, Size, 0, &parsed_record);
    
    if (parse_status == ARES_SUCCESS && parsed_record != NULL) {
        // Test getter functions on parsed record
        ares_dns_opcode_t parsed_opcode = ares_dns_record_get_opcode(parsed_record);
        unsigned short parsed_id = ares_dns_record_get_id(parsed_record);
        unsigned short parsed_flags = ares_dns_record_get_flags(parsed_record);
        
        // Test set_id on parsed record
        unsigned short new_id = 12345;  // Arbitrary test value
        ares_dns_record_set_id(parsed_record, new_id);
        
        // Verify the change
        unsigned short updated_id = ares_dns_record_get_id(parsed_record);
        
        // Clean up parsed record
        ares_dns_record_destroy(parsed_record);
    }
    
    // Test ares_dns_record_create with various inputs
    // Use fuzzer data to generate test parameters
    unsigned short id = 0;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    // Extract values from fuzzer input if available
    if (Size >= 2) {
        id = (Data[0] << 8) | Data[1];
    }
    if (Size >= 4) {
        flags = (Data[2] << 8) | Data[3];
    }
    if (Size >= 5) {
        opcode = (ares_dns_opcode_t)(Data[4] % 6);  // 0-5 are valid opcodes
    }
    if (Size >= 6) {
        rcode = (ares_dns_rcode_t)(Data[5] % 16);  // 0-15 are valid rcodes
    }
    
    ares_dns_record_t *created_record = NULL;
    ares_status_t create_status = ares_dns_record_create(&created_record, id, flags, opcode, rcode);
    
    if (create_status == ARES_SUCCESS && created_record != NULL) {
        // Test getter functions on created record
        ares_dns_opcode_t created_opcode = ares_dns_record_get_opcode(created_record);
        unsigned short created_id = ares_dns_record_get_id(created_record);
        unsigned short created_flags = ares_dns_record_get_flags(created_record);
        
        // Test set_id on created record with different values
        // Use various test values including edge cases
        static const unsigned short test_ids[] = {0, 1, 65535, 12345};
        
        for (size_t i = 0; i < sizeof(test_ids)/sizeof(test_ids[0]); i++) {
            ares_dns_record_set_id(created_record, test_ids[i]);
            unsigned short test_id_result = ares_dns_record_get_id(created_record);
        }
        
        // Clean up created record
        ares_dns_record_destroy(created_record);
    }
    
    // Test with zero ID (as recommended for new queries)
    ares_dns_record_t *zero_id_record = NULL;
    ares_status_t zero_id_status = ares_dns_record_create(&zero_id_record, 0, flags, opcode, rcode);
    
    if (zero_id_status == ARES_SUCCESS && zero_id_record != NULL) {
        unsigned short zero_id = ares_dns_record_get_id(zero_id_record);
        ares_dns_record_destroy(zero_id_record);
    }
    
    // Test with various flag combinations
    static const unsigned short test_flags[] = {0, 1, 256, 512, 1024, 2048, 4095, 65535};
    
    for (size_t i = 0; i < sizeof(test_flags)/sizeof(test_flags[0]); i++) {
        ares_dns_record_t *flag_record = NULL;
        ares_status_t flag_status = ares_dns_record_create(&flag_record, id, test_flags[i], opcode, rcode);
        
        if (flag_status == ARES_SUCCESS && flag_record != NULL) {
            unsigned short retrieved_flags = ares_dns_record_get_flags(flag_record);
            ares_dns_record_destroy(flag_record);
        }
    }
    
    // Test with various opcodes
    static const ares_dns_opcode_t test_opcodes[] = {
        ARES_OPCODE_QUERY,
        ARES_OPCODE_IQUERY,
        ARES_OPCODE_STATUS,
        ARES_OPCODE_NOTIFY,
        ARES_OPCODE_UPDATE
    };
    
    for (size_t i = 0; i < sizeof(test_opcodes)/sizeof(test_opcodes[0]); i++) {
        ares_dns_record_t *opcode_record = NULL;
        ares_status_t opcode_status = ares_dns_record_create(&opcode_record, id, flags, test_opcodes[i], rcode);
        
        if (opcode_status == ARES_SUCCESS && opcode_record != NULL) {
            ares_dns_opcode_t retrieved_opcode = ares_dns_record_get_opcode(opcode_record);
            ares_dns_record_destroy(opcode_record);
        }
    }
    
    // Test with various rcodes
    static const ares_dns_rcode_t test_rcodes[] = {
        ARES_RCODE_NOERROR,
        ARES_RCODE_FORMERR,
        ARES_RCODE_SERVFAIL,
        ARES_RCODE_NXDOMAIN,
        ARES_RCODE_NOTIMP,
        ARES_RCODE_REFUSED,
        ARES_RCODE_YXDOMAIN,
        ARES_RCODE_YXRRSET,
        ARES_RCODE_NXRRSET,
        ARES_RCODE_NOTAUTH,
        ARES_RCODE_NOTZONE
    };
    
    for (size_t i = 0; i < sizeof(test_rcodes)/sizeof(test_rcodes[0]); i++) {
        ares_dns_record_t *rcode_record = NULL;
        ares_status_t rcode_status = ares_dns_record_create(&rcode_record, id, flags, opcode, test_rcodes[i]);
        
        if (rcode_status == ARES_SUCCESS && rcode_record != NULL) {
            ares_dns_record_destroy(rcode_record);
        }
    }
    
    // Test ares_dns_parse with different flags
    if (Size > 0) {
        ares_dns_record_t *parsed_record_flags = NULL;
        unsigned int parse_flags = 0;
        
        // Test with various flag combinations
        if (Size >= 1) {
            parse_flags = Data[0];
        }
        
        ares_status_t parse_status_flags = ares_dns_parse(Data, Size, parse_flags, &parsed_record_flags);
        
        if (parse_status_flags == ARES_SUCCESS && parsed_record_flags != NULL) {
            ares_dns_record_destroy(parsed_record_flags);
        }
    }
    
    return 0;
}
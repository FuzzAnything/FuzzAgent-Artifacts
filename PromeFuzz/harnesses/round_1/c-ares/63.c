// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_record_set_id at ares_dns_record.c:100:13 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_record_set_id at ares_dns_record.c:100:13 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

/* Define minimal types needed for the fuzz driver */
typedef int ares_bool_t;
typedef int ares_status_t;

enum {
    ARES_FALSE = 0,
    ARES_TRUE = 1,
    ARES_SUCCESS = 0
};

/* Include the actual header */
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    // Test ares_dns_parse with fuzzer input
    ares_dns_record_t *parsed_rec = NULL;
    unsigned int flags = 0;
    
    // Try different flag combinations
    for (unsigned int i = 0; i < 4; i++) {
        flags = i;
        ares_status_t parse_status = ares_dns_parse(Data, Size, flags, &parsed_rec);
        
        if (parse_status == ARES_SUCCESS && parsed_rec != NULL) {
            // Test getter functions on parsed record
            unsigned short record_id = ares_dns_record_get_id(parsed_rec);
            unsigned short record_flags = ares_dns_record_get_flags(parsed_rec);
            ares_dns_opcode_t opcode = ares_dns_record_get_opcode(parsed_rec);
            
            // Try setting ID with various values
            for (unsigned short test_id = 0; test_id < 65535; test_id += 16383) {
                ares_dns_record_set_id(parsed_rec, test_id);
            }
            
            // Clean up parsed record
            ares_dns_record_destroy(parsed_rec);
            parsed_rec = NULL;
        }
    }
    
    // Test ares_dns_record_create with various parameters
    for (unsigned short id = 0; id < 65535; id += 32767) {
        for (unsigned short flags_val = 0; flags_val < 65535; flags_val += 32767) {
            for (int opcode_val = 0; opcode_val <= 5; opcode_val++) {
                if (opcode_val == 3) continue; // Skip value 3 which doesn't exist in enum
                for (int rcode_val = 0; rcode_val <= 23; rcode_val++) {
                    if (rcode_val >= 11 && rcode_val <= 15) continue; // Skip undefined values
                    if (rcode_val == 16) continue; // Skip 16 which has duplicate definitions
                    
                    ares_dns_record_t *created_rec = NULL;
                    ares_status_t create_status = ares_dns_record_create(&created_rec, id, flags_val, opcode_val, rcode_val);
                    
                    if (create_status == ARES_SUCCESS && created_rec != NULL) {
                        // Test getter functions on created record
                        unsigned short get_id = ares_dns_record_get_id(created_rec);
                        unsigned short get_flags = ares_dns_record_get_flags(created_rec);
                        ares_dns_opcode_t get_opcode = ares_dns_record_get_opcode(created_rec);
                        
                        // Try setting ID with various values
                        for (unsigned short set_id = 0; set_id < 65535; set_id += 21845) {
                            ares_dns_record_set_id(created_rec, set_id);
                        }
                        
                        // Clean up created record
                        ares_dns_record_destroy(created_rec);
                    }
                }
            }
        }
    }
    
    return 0;
}
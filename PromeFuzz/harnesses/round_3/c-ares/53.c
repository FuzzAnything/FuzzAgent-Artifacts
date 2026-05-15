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

static void test_parsing(const uint8_t *data, size_t size) {
    ares_dns_record_t *dnsrec = NULL;
    unsigned int flags = 0;
    
    // Try parsing with different flag combinations
    for (unsigned int i = 0; i < 4; i++) {
        ares_status_t status = ares_dns_parse(data, size, i, &dnsrec);
        if (status == ARES_SUCCESS && dnsrec != NULL) {
            // Test getter functions on parsed record
            unsigned short id = ares_dns_record_get_id(dnsrec);
            unsigned short flags_val = ares_dns_record_get_flags(dnsrec);
            ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
            
            // Try setting a new ID (use data-derived value if possible)
            unsigned short new_id = 0;
            if (size >= 2) {
                new_id = (data[0] << 8) | data[1];
            }
            ares_dns_record_set_id(dnsrec, new_id);
            
            // Verify the new ID was set
            unsigned short updated_id = ares_dns_record_get_id(dnsrec);
            
            ares_dns_record_destroy(dnsrec);
            dnsrec = NULL;
        }
    }
}

static void test_record_creation(const uint8_t *data, size_t size) {
    ares_dns_record_t *dnsrec = NULL;
    
    // Extract parameters from fuzz data
    unsigned short id = 0;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    if (size >= 2) {
        id = (data[0] << 8) | data[1];
    }
    if (size >= 4) {
        flags = (data[2] << 8) | data[3];
    }
    if (size >= 5) {
        opcode = (ares_dns_opcode_t)(data[4] % 16); // Opcode is 4 bits
    }
    if (size >= 6) {
        rcode = (ares_dns_rcode_t)(data[5] % 16); // Rcode is 4 bits
    }
    
    // Create record with extracted parameters
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status == ARES_SUCCESS && dnsrec != NULL) {
        // Test getter functions
        unsigned short retrieved_id = ares_dns_record_get_id(dnsrec);
        unsigned short retrieved_flags = ares_dns_record_get_flags(dnsrec);
        ares_dns_opcode_t retrieved_opcode = ares_dns_record_get_opcode(dnsrec);
        
        // Test setter function with different IDs
        for (int i = 0; i < 3; i++) {
            unsigned short test_id = 0;
            if (size >= (i * 2 + 2)) {
                test_id = (data[i * 2] << 8) | data[i * 2 + 1];
            }
            ares_dns_record_set_id(dnsrec, test_id);
            
            // Verify the ID was set
            unsigned short current_id = ares_dns_record_get_id(dnsrec);
        }
        
        // Clean up
        ares_dns_record_destroy(dnsrec);
        dnsrec = NULL;
    }
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Test DNS parsing functionality
    test_parsing(Data, Size);
    
    // Test record creation and manipulation
    test_record_creation(Data, Size);
    
    return 0;
}
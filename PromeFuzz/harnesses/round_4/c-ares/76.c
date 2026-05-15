// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_record_set_id at ares_dns_record.c:100:13 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_get_flags at ares_dns_record.c:109:16 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_record_set_id at ares_dns_record.c:100:13 in ares_dns_record.h
// ares_dns_record_get_id at ares_dns_record.c:92:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_opcode_t get_opcode_from_byte(uint8_t byte) {
    switch (byte % 6) {
        case 0: return ARES_OPCODE_QUERY;
        case 1: return ARES_OPCODE_IQUERY;
        case 2: return ARES_OPCODE_STATUS;
        case 3: return ARES_OPCODE_NOTIFY;
        case 4: return ARES_OPCODE_UPDATE;
        default: return ARES_OPCODE_QUERY;
    }
}

static ares_dns_rcode_t get_rcode_from_byte(uint8_t byte) {
    switch (byte % 16) {
        case 0: return ARES_RCODE_NOERROR;
        case 1: return ARES_RCODE_FORMERR;
        case 2: return ARES_RCODE_SERVFAIL;
        case 3: return ARES_RCODE_NXDOMAIN;
        case 4: return ARES_RCODE_NOTIMP;
        case 5: return ARES_RCODE_REFUSED;
        case 6: return ARES_RCODE_YXDOMAIN;
        case 7: return ARES_RCODE_YXRRSET;
        case 8: return ARES_RCODE_NXRRSET;
        case 9: return ARES_RCODE_NOTAUTH;
        case 10: return ARES_RCODE_NOTZONE;
        case 11: return ARES_RCODE_DSOTYPEI;
        case 12: return ARES_RCODE_BADSIG;
        case 13: return ARES_RCODE_BADKEY;
        case 14: return ARES_RCODE_BADTIME;
        case 15: return ARES_RCODE_BADMODE;
        default: return ARES_RCODE_NOERROR;
    }
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test ares_dns_parse with the input data
    ares_dns_record_t *parsed_record = NULL;
    unsigned int parse_flags = 0;
    if (Size > 1) {
        parse_flags = Data[0] % 256;
    }
    
    ares_status_t parse_status = ares_dns_parse(Data, Size, parse_flags, &parsed_record);
    
    if (parse_status == ARES_SUCCESS && parsed_record != NULL) {
        // Test getter functions on parsed record
        unsigned short parsed_id = ares_dns_record_get_id(parsed_record);
        unsigned short parsed_flags = ares_dns_record_get_flags(parsed_record);
        ares_dns_opcode_t parsed_opcode = ares_dns_record_get_opcode(parsed_record);
        
        // Test set_id on parsed record
        if (Size > 2) {
            unsigned short new_id = (Data[1] << 8) | Data[2];
            ares_bool_t set_result = ares_dns_record_set_id(parsed_record, new_id);
            if (set_result == ARES_TRUE) {
                unsigned short updated_id = ares_dns_record_get_id(parsed_record);
            }
        }
        
        // Clean up parsed record
        ares_dns_record_destroy(parsed_record);
    }

    // Test ares_dns_record_create with various parameters
    ares_dns_record_t *created_record = NULL;
    
    // Extract parameters from input data
    unsigned short id = 0;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    if (Size >= 2) {
        id = (Data[0] << 8) | Data[1];
    }
    if (Size >= 4) {
        flags = (Data[2] << 8) | Data[3];
    }
    if (Size >= 5) {
        opcode = get_opcode_from_byte(Data[4]);
    }
    if (Size >= 6) {
        rcode = get_rcode_from_byte(Data[5]);
    }
    
    ares_status_t create_status = ares_dns_record_create(&created_record, id, flags, opcode, rcode);
    
    if (create_status == ARES_SUCCESS && created_record != NULL) {
        // Test getter functions on created record
        unsigned short created_id = ares_dns_record_get_id(created_record);
        unsigned short created_flags = ares_dns_record_get_flags(created_record);
        ares_dns_opcode_t created_opcode = ares_dns_record_get_opcode(created_record);
        
        // Test set_id on created record
        if (Size >= 8) {
            unsigned short new_id2 = (Data[6] << 8) | Data[7];
            ares_bool_t set_result2 = ares_dns_record_set_id(created_record, new_id2);
            if (set_result2 == ARES_TRUE) {
                unsigned short updated_id2 = ares_dns_record_get_id(created_record);
            }
        }
        
        // Clean up created record
        ares_dns_record_destroy(created_record);
    }

    return 0;
}
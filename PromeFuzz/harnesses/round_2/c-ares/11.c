// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_opcode_tostr at ares_dns_mapping.c:253:13 in ares_dns_record.h
// ares_dns_rcode_tostr at ares_dns_mapping.c:910:13 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_record_t* create_dummy_record(const uint8_t* data, size_t size) {
    ares_dns_record_t* record = NULL;
    ares_status_t status;
    
    // Extract parameters from fuzz data with fallback defaults
    unsigned short id = 1234;
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
        opcode = (ares_dns_opcode_t)(data[4] % 16);
    }
    
    if (size >= 6) {
        rcode = (ares_dns_rcode_t)(data[5] % 16);
    }
    
    // Create DNS record with the correct number of arguments
    status = ares_dns_record_create(&record, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || !record) {
        return NULL;
    }
    
    return record;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Create a DNS record for fuzzing
    ares_dns_record_t* dnsrec = create_dummy_record(Data, Size);
    if (!dnsrec) {
        return 0;
    }
    
    // Call ares_dns_record_rr_cnt three times with different sections
    // Using modulo operations to get valid section values from fuzz data
    for (int i = 0; i < 3; i++) {
        size_t section_idx = Size > i ? Data[i] % 4 : 0;  // 4 possible sections
        ares_dns_section_t sect = (ares_dns_section_t)section_idx;
        size_t count = ares_dns_record_rr_cnt(dnsrec, sect);
        (void)count;  // Use count to avoid unused variable warning
    }
    
    // Get opcode and convert to string
    ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
    const char* opcode_str = ares_dns_opcode_tostr(opcode);
    (void)opcode_str;  // Use opcode_str to avoid unused variable warning
    
    // Convert rcode to string (use data to generate rcode value)
    ares_dns_rcode_t rcode = Size > 0 ? (ares_dns_rcode_t)(Data[Size-1] % 16) : 0;
    const char* rcode_str = ares_dns_rcode_tostr(rcode);
    (void)rcode_str;  // Use rcode_str to avoid unused variable warning
    
    // Cleanup
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
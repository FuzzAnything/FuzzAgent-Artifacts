// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_rr_del at ares_dns_record.c:450:15 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_name at ares_dns_record.c:505:13 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include ares.h first to define CARES_EXTERN */
#include "ares.h"
/* Now include the DNS record header */
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // Create dummy file for any potential file operations
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Initialize a DNS record with parameters from fuzzer input
    ares_dns_record_t* dnsrec = NULL;
    
    // Use fuzzer data to determine parameters for record creation
    unsigned short id = 12345;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    if (Size > 0) {
        id = (unsigned short)Data[0];
    }
    if (Size > 1) {
        flags = (unsigned short)Data[1];
    }
    if (Size > 2) {
        opcode = (ares_dns_opcode_t)(Data[2] % 16); // Opcode values 0-15
    }
    if (Size > 3) {
        rcode = (ares_dns_rcode_t)(Data[3] % 16); // Rcode values 0-15
    }
    
    ares_status_t create_status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (create_status != ARES_SUCCESS || !dnsrec) {
        return 0;
    }

    // Extract parameters from fuzzer input for RR operations
    ares_dns_section_t sect = ARES_SECTION_ANSWER;
    const char* name = "test.example.com";
    ares_dns_rec_type_t type = ARES_REC_TYPE_A;
    ares_dns_class_t rclass = ARES_CLASS_IN;
    unsigned int ttl = 300;

    // Use different bytes for section to explore different paths
    if (Size > 4) {
        // Section can be 1-4 based on enum: ANSWER, AUTHORITY, ADDITIONAL
        sect = (ares_dns_section_t)((Data[4] % 3) + 1);
    }
    if (Size > 5) {
        type = (ares_dns_rec_type_t)(Data[5] % 256);
    }
    if (Size > 6) {
        rclass = (ares_dns_class_t)(Data[6] % 256);
    }
    if (Size > 10) {
        ttl = *(unsigned int*)(Data + 7);
    }

    // Test ares_dns_record_rr_add
    ares_dns_rr_t* rr_out = NULL;
    ares_status_t add_status = ares_dns_record_rr_add(&rr_out, dnsrec, sect, name, type, rclass, ttl);
    
    // Test ares_dns_record_rr_del (try to delete first record if added)
    ares_status_t del_status = ares_dns_record_rr_del(dnsrec, sect, 0);
    
    // Test ares_dns_record_rr_get (try to get first record)
    ares_dns_rr_t* rr_get = ares_dns_record_rr_get(dnsrec, sect, 0);
    
    // Test ares_dns_rr_get_name if we got a valid record
    if (rr_get) {
        const char* rr_name = ares_dns_rr_get_name(rr_get);
        // Use rr_name to avoid compiler optimization
        if (rr_name) {
            volatile const char* dummy = rr_name;
        }
    }

    // Clean up
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
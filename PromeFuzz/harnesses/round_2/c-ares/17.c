// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_query_set_name at ares_dns_record.c:283:15 in ares_dns_record.h
// ares_dns_record_query_set_type at ares_dns_record.c:306:15 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/* Include necessary headers first */
#include "ares.h"
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for basic operations
    if (Size < 2) {
        return 0;
    }

    // Create a DNS record to work with
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status;
    
    // Create a DNS query record with one question
    unsigned short id = 0x1234;
    unsigned short flags = 0x0100;  // Standard query flag
    
    // Create a record with proper arguments
    status = ares_dns_record_create(&dnsrec, id, flags, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    
    // If we couldn't create a record, return early
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        return 0;
    }
    
    // Add a question to the record
    const char *initial_name = "example.com";
    status = ares_dns_record_query_add(dnsrec, initial_name, 
                                       ARES_REC_TYPE_A, ARES_CLASS_IN);
    
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    // Use fuzzer data for parameters
    // Use first byte as index (mod 4 to keep it small)
    size_t idx = Data[0] % 4;
    
    // Use second byte for query type (convert to valid enum values)
    ares_dns_rec_type_t qtype = (ares_dns_rec_type_t)((Data[1] % 16) + 1);
    
    // Use remaining data for name string
    const char *name = NULL;
    char *name_buf = NULL;
    
    if (Size > 2) {
        // Allocate buffer for name (null-terminated)
        // Limit size to avoid huge allocations
        size_t name_len = Size - 2;
        if (name_len > 1024) {
            name_len = 1024;
        }
        
        name_buf = (char*)malloc(name_len + 1);
        if (name_buf) {
            memcpy(name_buf, Data + 2, name_len);
            name_buf[name_len] = '\0'; // Ensure null termination
            
            // Ensure the name is at least one character
            if (name_buf[0] != '\0') {
                name = name_buf;
            }
        }
    }
    
    // If we couldn't create a name buffer, use a default
    if (!name) {
        name = "fuzz.test";
    }
    
    // Test 1: ares_dns_record_query_set_name
    status = ares_dns_record_query_set_name(dnsrec, idx, name);
    
    // Test 2: ares_dns_record_query_set_type
    status = ares_dns_record_query_set_type(dnsrec, idx, qtype);
    
    // Test 3: ares_dns_record_query_get
    const char *out_name = NULL;
    ares_dns_rec_type_t out_qtype;
    ares_dns_class_t out_qclass;
    status = ares_dns_record_query_get(dnsrec, idx, &out_name, &out_qtype, &out_qclass);
    
    // Test 4: ares_dns_record_rr_cnt for different sections
    // We'll test with different enum values
    for (int sect_val = ARES_SECTION_ANSWER; sect_val <= ARES_SECTION_ADDITIONAL; sect_val++) {
        ares_dns_section_t section = (ares_dns_section_t)sect_val;
        size_t cnt = ares_dns_record_rr_cnt(dnsrec, section);
        (void)cnt; // Use variable to avoid unused warning
    }
    
    // Clean up
    if (name_buf) {
        free(name_buf);
    }
    
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    return 0;
}
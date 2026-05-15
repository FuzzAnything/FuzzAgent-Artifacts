// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_get_str at ares_dns_record.c:981:13 in ares_dns_record.h
// ares_dns_rr_get_opt at ares_dns_record.c:1016:16 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_opt_get_name at ares_dns_mapping.c:896:13 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_rr_t* create_test_rr(void) {
    // Create a minimal DNS record structure to work with
    ares_dns_record_t* dns_record = NULL;
    ares_status_t status = ares_dns_record_create(&dns_record, 12345, 0, 
                                                  ARES_OPCODE_QUERY, 
                                                  ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || !dns_record) {
        return NULL;
    }
    
    // Try to parse a minimal DNS packet to get a valid rr
    unsigned char dns_data[] = {
        0x00, 0x00,  // ID
        0x81, 0x80,  // Flags
        0x00, 0x01,  // QDCOUNT
        0x00, 0x01,  // ANCOUNT
        0x00, 0x00,  // NSCOUNT
        0x00, 0x01,  // ARCOUNT
        
        // Query: example.com
        0x07, 'e', 'x', 'a', 'm', 'p', 'l', 'e',
        0x03, 'c', 'o', 'm',
        0x00,
        0x00, 0x01,  // TYPE A
        0x00, 0x01,  // CLASS IN
        
        // Answer: example.com A 192.0.2.1
        0xc0, 0x0c,  // Name pointer
        0x00, 0x01,  // TYPE A
        0x00, 0x01,  // CLASS IN
        0x00, 0x00, 0x0e, 0x10,  // TTL
        0x00, 0x04,  // RDLENGTH
        0xc0, 0x00, 0x02, 0x01,  // 192.0.2.1
        
        // Additional: OPT record
        0x00,  // Empty name
        0x00, 0x29,  // TYPE OPT
        0x05, 0x00,  // UDP payload size
        0x00,  // Extended RCODE
        0x00,  // EDNS0 version
        0x00, 0x00,  // Flags
        0x00, 0x00,  // RDLENGTH
    };
    
    ares_dns_record_t* parsed_record = NULL;
    status = ares_dns_parse(dns_data, sizeof(dns_data), 0, &parsed_record);
    if (status == ARES_SUCCESS && parsed_record) {
        // Get the first resource record from the additional section
        size_t count = ares_dns_record_rr_cnt(parsed_record, ARES_SECTION_ADDITIONAL);
        if (count > 0) {
            ares_dns_rr_t* rr = ares_dns_record_rr_get(parsed_record, ARES_SECTION_ADDITIONAL, 0);
            // We need to keep the parsed record alive, so we'll just return the rr
            // The parsed record will be leaked, but that's acceptable for fuzzing
            return rr;
        }
        ares_dns_record_destroy(parsed_record);
    }
    
    // Clean up the first record we created
    if (dns_record) {
        ares_dns_record_destroy(dns_record);
    }
    
    return NULL;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    // Test ares_dns_rr_key_tostr with various keys
    for (int i = 0; i < 256 && i < (int)Size; i++) {
        ares_dns_rr_key_t key = (ares_dns_rr_key_t)Data[i];
        const char* str = ares_dns_rr_key_tostr(key);
        (void)str; // Avoid unused variable warning
    }
    
    // Test ares_dns_rr_key_datatype
    for (int i = 0; i < 256 && i < (int)Size; i++) {
        ares_dns_rr_key_t key = (ares_dns_rr_key_t)Data[i];
        ares_dns_datatype_t dt = ares_dns_rr_key_datatype(key);
        (void)dt; // Avoid unused variable warning
    }
    
    // Test ares_dns_opt_get_name
    for (int i = 0; i < 256 && i + 1 < (int)Size; i++) {
        ares_dns_rr_key_t key = (ares_dns_rr_key_t)Data[i];
        unsigned short opt = (unsigned short)Data[i + 1];
        const char* name = ares_dns_opt_get_name(key, opt);
        (void)name; // Avoid unused variable warning
    }
    
    // Create a test resource record for functions that need it
    ares_dns_rr_t* rr = create_test_rr();
    
    if (rr) {
        // Test ares_dns_rr_get_str
        for (int i = 0; i < 256 && i < (int)Size; i++) {
            ares_dns_rr_key_t key = (ares_dns_rr_key_t)Data[i];
            const char* str = ares_dns_rr_get_str(rr, key);
            (void)str; // Avoid unused variable warning
        }
        
        // Test ares_dns_rr_get_opt
        for (int i = 0; i < 256 && i + 2 < (int)Size; i++) {
            ares_dns_rr_key_t key = (ares_dns_rr_key_t)Data[i];
            size_t idx = (size_t)Data[i + 1] % 16; // Limit index to reasonable range
            const unsigned char* val = NULL;
            size_t val_len = 0;
            unsigned short result = ares_dns_rr_get_opt(rr, key, idx, &val, &val_len);
            (void)result; // Avoid unused variable warning
        }
        
        // Test ares_dns_rr_set_opt
        for (int i = 0; i < 256 && i + 3 < (int)Size; i++) {
            ares_dns_rr_key_t key = (ares_dns_rr_key_t)Data[i];
            unsigned short opt = (unsigned short)Data[i + 1];
            
            // Use remaining data as value
            size_t val_len = Size - i - 2;
            if (val_len > 256) val_len = 256; // Limit size
            
            const unsigned char* val = Data + i + 2;
            ares_status_t status = ares_dns_rr_set_opt(rr, key, opt, val, val_len);
            (void)status; // Avoid unused variable warning
        }
    }
    
    return 0;
}
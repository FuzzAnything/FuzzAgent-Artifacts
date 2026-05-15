// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_get_str at ares_dns_record.c:981:13 in ares_dns_record.h
// ares_dns_opt_get_name at ares_dns_mapping.c:896:13 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_get_opt at ares_dns_record.c:1016:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_opt_get_name at ares_dns_mapping.c:896:13 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static void dummy_cleanup(void) {
    // Placeholder for any cleanup needed
}

static ares_dns_rr_t* create_dummy_rr(ares_dns_record_t** parent_out) {
    // Create a minimal DNS record structure
    ares_dns_record_t* dns_rec = NULL;
    ares_status_t status = ares_dns_record_create(&dns_rec, 12345, 0, 
                                                 ARES_OPCODE_QUERY, 
                                                 ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || !dns_rec) {
        return NULL;
    }
    
    // Create a resource record with OPT type (supports options)
    // Note: We need to use the proper function to create RRs
    // For simplicity, we'll try to parse a minimal DNS packet to get an RR
    // But first, let's try to create a simple RR using available functions
    
    // We'll create a dummy RR by parsing a minimal DNS response
    unsigned char dns_data[] = {
        0x12, 0x34,  // ID
        0x81, 0x80,  // Flags
        0x00, 0x01,  // QDCOUNT
        0x00, 0x01,  // ANCOUNT
        0x00, 0x00,  // NSCOUNT
        0x00, 0x00,  // ARCOUNT
        // Question
        0x07, 'e', 'x', 'a', 'm', 'p', 'l', 'e',  // "example"
        0x03, 'c', 'o', 'm',  // "com"
        0x00,  // null terminator
        0x00, 0x01,  // TYPE A
        0x00, 0x01,  // CLASS IN
        // Answer
        0xc0, 0x0c,  // Pointer to name
        0x00, 0x01,  // TYPE A
        0x00, 0x01,  // CLASS IN
        0x00, 0x00, 0x0e, 0x10,  // TTL 3600
        0x00, 0x04,  // RDLENGTH 4
        0x7f, 0x00, 0x00, 0x01  // 127.0.0.1
    };
    
    ares_dns_record_t* parsed_rec = NULL;
    status = ares_dns_parse(dns_data, sizeof(dns_data), 0, &parsed_rec);
    if (status != ARES_SUCCESS || !parsed_rec) {
        ares_dns_record_destroy(dns_rec);
        return NULL;
    }
    
    // Get the first RR from answer section
    ares_dns_rr_t* rr = ares_dns_record_rr_get(parsed_rec, ARES_SECTION_ANSWER, 0);
    if (rr == NULL) {
        ares_dns_record_destroy(parsed_rec);
        ares_dns_record_destroy(dns_rec);
        return NULL;
    }
    
    // Destroy the parsed record but keep the RR (it should be owned by dns_rec)
    // Actually, we need to transfer ownership or use the parsed record
    ares_dns_record_destroy(dns_rec);
    
    if (parent_out) {
        *parent_out = parsed_rec;
    } else {
        ares_dns_record_destroy(parsed_rec);
        return NULL;
    }
    
    return rr;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a dummy resource record for testing
    ares_dns_record_t* dns_rec = NULL;
    ares_dns_rr_t* dns_rr = create_dummy_rr(&dns_rec);
    if (!dns_rr || !dns_rec) {
        // If we can't create a dummy RR, still test functions that don't need RR
        // Extract parameters from fuzzed data
        ares_dns_rr_key_t key = (ares_dns_rr_key_t)(Data[0] % 256);
        unsigned short opt = 0;
        if (Size >= 3) {
            opt = (Data[1] << 8) | Data[2];
        } else if (Size >= 2) {
            opt = Data[1];
        }
        
        // Test functions that don't require a resource record
        const char* opt_name = ares_dns_opt_get_name(key, opt);
        (void)opt_name;
        
        ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
        (void)datatype;
        
        const char* key_str = ares_dns_rr_key_tostr(key);
        (void)key_str;
        
        return 0;
    }
    
    // Extract parameters from fuzzed data
    // Use first byte for key (modulo to keep in reasonable range)
    ares_dns_rr_key_t key = (ares_dns_rr_key_t)(Data[0] % 256);
    
    // Use second and third bytes for option if available
    unsigned short opt = 0;
    if (Size >= 3) {
        opt = (Data[1] << 8) | Data[2];
    } else if (Size >= 2) {
        opt = Data[1];
    }
    
    // Test ares_dns_rr_get_str
    const char* str_result = ares_dns_rr_get_str(dns_rr, key);
    (void)str_result;
    
    // Test ares_dns_opt_get_name
    const char* opt_name = ares_dns_opt_get_name(key, opt);
    (void)opt_name;
    
    // Test ares_dns_rr_key_datatype
    ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
    (void)datatype;
    
    // Test ares_dns_rr_key_tostr
    const char* key_str = ares_dns_rr_key_tostr(key);
    (void)key_str;
    
    // Test ares_dns_rr_set_opt
    if (Size > 3) {
        const unsigned char* val = Data + 3;
        size_t val_len = Size - 3;
        if (val_len > 0) {
            ares_status_t status = ares_dns_rr_set_opt(dns_rr, key, opt, val, val_len);
            (void)status;
            
            // Test ares_dns_rr_get_opt if set_opt succeeded
            if (status == ARES_SUCCESS) {
                const unsigned char* retrieved_val = NULL;
                size_t retrieved_len = 0;
                unsigned short retrieved_opt = ares_dns_rr_get_opt(dns_rr, key, 0, 
                                                                  &retrieved_val, 
                                                                  &retrieved_len);
                (void)retrieved_opt;
                (void)retrieved_val;
                (void)retrieved_len;
            }
        }
    }
    
    // Clean up
    ares_dns_record_destroy(dns_rec);
    
    dummy_cleanup();
    return 0;
}
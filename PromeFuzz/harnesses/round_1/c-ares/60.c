// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_opt_get_name at ares_dns_mapping.c:896:13 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_str at ares_dns_record.c:981:13 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include "ares.h"
#include "ares_dns_record.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    // Test ares_dns_rr_key_tostr with fuzzer data
    ares_dns_rr_key_t key1 = (ares_dns_rr_key_t)(Data[0] % 256);
    const char* str1 = ares_dns_rr_key_tostr(key1);
    
    // Test ares_dns_opt_get_name with fuzzer data
    ares_dns_rr_key_t key2 = (ares_dns_rr_key_t)(Data[1] % 256);
    unsigned short opt = (unsigned short)(Data[2] | (Data[3] << 8));
    const char* str2 = ares_dns_opt_get_name(key2, opt);
    
    // Test ares_dns_rr_key_datatype with fuzzer data
    ares_dns_rr_key_t key3 = (ares_dns_rr_key_t)(Data[0] % 256);
    ares_dns_datatype_t dt = ares_dns_rr_key_datatype(key3);
    
    // Create a dummy DNS record for testing functions that require ares_dns_rr_t*
    ares_dns_record_t* dns_record = NULL;
    ares_status_t create_status = ares_dns_record_create(&dns_record, 0x1234, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    
    if (create_status == ARES_SUCCESS && dns_record != NULL) {
        // Add a question to the record
        ares_dns_record_query_add(dns_record, "example.com", ARES_REC_TYPE_A, ARES_CLASS_IN);
        
        // Get the first resource record (if any exists)
        ares_dns_rr_t* dns_rr = ares_dns_record_rr_get(dns_record, ARES_SECTION_ANSWER, 0);
        
        if (dns_rr != NULL) {
            // Test ares_dns_rr_get_str with fuzzer data
            ares_dns_rr_key_t key4 = (ares_dns_rr_key_t)(Data[1] % 256);
            const char* str3 = ares_dns_rr_get_str(dns_rr, key4);
            
            // Test ares_dns_rr_set_opt with fuzzer data
            ares_dns_rr_key_t key5 = (ares_dns_rr_key_t)(Data[2] % 256);
            unsigned short opt2 = (unsigned short)(Data[3] | (Data[0] << 8));
            
            // Use remaining data for value if available
            const unsigned char* val_data = NULL;
            size_t val_len = 0;
            if (Size > 4) {
                val_data = Data + 4;
                val_len = Size - 4;
                if (val_len > 1024) {
                    val_len = 1024;  // Limit size
                }
            }
            
            ares_status_t status1 = ares_dns_rr_set_opt(dns_rr, key5, opt2, val_data, val_len);
            
            // Test ares_dns_rr_set_bin with fuzzer data
            ares_dns_rr_key_t key6 = (ares_dns_rr_key_t)(Data[3] % 256);
            ares_status_t status2 = ares_dns_rr_set_bin(dns_rr, key6, val_data, val_len);
        }
        
        // Clean up
        ares_dns_record_destroy(dns_record);
    }
    
    return 0;
}
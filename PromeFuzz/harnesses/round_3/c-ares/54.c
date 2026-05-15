// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_get_class at ares_dns_record.c:521:18 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_class_fromstr at ares_dns_mapping.c:663:13 in ares_dns_record.h
// ares_dns_class_fromstr at ares_dns_mapping.c:663:13 in ares_dns_record.h
// ares_dns_class_fromstr at ares_dns_mapping.c:663:13 in ares_dns_record.h
// ares_dns_class_tostr at ares_dns_mapping.c:236:13 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if not already defined */
#ifndef CARES_EXTERN
#ifdef CARES_STATICLIB
#define CARES_EXTERN
#else
#ifdef _WIN32
#define CARES_EXTERN __declspec(dllimport)
#else
#define CARES_EXTERN extern
#endif
#endif
#endif

#include "ares.h"
#include "ares_dns_record.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    // Test ares_dns_class_fromstr
    if (Size > 0) {
        // Create null-terminated string from fuzzer input
        char *str = malloc(Size + 1);
        if (str) {
            memcpy(str, Data, Size);
            str[Size] = '\0';
            
            ares_dns_class_t qclass;
            ares_dns_class_fromstr(&qclass, str);
            
            free(str);
        }
    }
    
    // Test ares_dns_class_tostr with various enum values
    // Use first byte to generate different enum values
    if (Size > 0) {
        ares_dns_class_t class_val = (ares_dns_class_t)(Data[0] % 256);
        const char *result = ares_dns_class_tostr(class_val);
        (void)result; // Avoid unused variable warning
    }
    
    // Test ares_dns_parse with the input buffer
    if (Size > 0) {
        ares_dns_record_t *dnsrec = NULL;
        unsigned int flags = 0;
        
        // Try parsing with different flags based on input
        if (Size > 1) {
            flags = Data[1] % 256;
        }
        
        ares_status_t status = ares_dns_parse(Data, Size, flags, &dnsrec);
        
        if (status == ARES_SUCCESS && dnsrec != NULL) {
            // Test ares_dns_record_query_get if we have queries
            size_t idx = 0;
            const char *name = NULL;
            ares_dns_rec_type_t qtype;
            ares_dns_class_t qclass;
            
            // Try to get first query
            ares_dns_record_query_get(dnsrec, idx, &name, &qtype, &qclass);
            
            // Test with NULL parameters
            ares_dns_record_query_get(dnsrec, idx, NULL, NULL, NULL);
            
            // Test ares_dns_rr_get_class on resource records
            // Try to get RR from answer section if available
            // We'll try to access first RR in each section
            // Since we don't have direct access to the arrays, we'll try to add RRs instead
            
            // Clean up
            ares_dns_record_destroy(dnsrec);
        }
    }
    
    // Test ares_dns_record_rr_add by creating a new DNS record
    // First create a DNS record using parsing with minimal data
    if (Size > 2) {
        // Create minimal DNS packet for testing
        unsigned char minimal_dns[12] = {
            0x00, 0x00,  // ID
            0x00, 0x00,  // Flags
            0x00, 0x01,  // QDCOUNT = 1
            0x00, 0x00,  // ANCOUNT = 0
            0x00, 0x00,  // NSCOUNT = 0
            0x00, 0x00   // ARCOUNT = 0
        };
        
        // Copy query name from input if available
        size_t query_offset = 12;
        size_t total_size = query_offset + 5; // Minimum for query: label + null + type + class
        
        if (Size > total_size) {
            unsigned char *dns_buf = malloc(total_size);
            if (dns_buf) {
                memcpy(dns_buf, minimal_dns, query_offset);
                
                // Add query name (single label)
                dns_buf[query_offset] = 3; // Length of label
                memcpy(dns_buf + query_offset + 1, Data, 3); // Use first 3 bytes as label
                dns_buf[query_offset + 4] = 0; // Null terminator
                dns_buf[query_offset + 5] = 0x00; // Type = A
                dns_buf[query_offset + 6] = 0x01;
                dns_buf[query_offset + 7] = 0x00; // Class = IN
                dns_buf[query_offset + 8] = 0x01;
                
                ares_dns_record_t *dnsrec = NULL;
                ares_status_t status = ares_dns_parse(dns_buf, total_size, 0, &dnsrec);
                
                if (status == ARES_SUCCESS && dnsrec != NULL) {
                    // Now test ares_dns_record_rr_add
                    ares_dns_rr_t *rr_out = NULL;
                    const char *name = "test.example.com";
                    ares_dns_rec_type_t type = ARES_REC_TYPE_A;
                    ares_dns_class_t rclass = ARES_CLASS_IN;
                    unsigned int ttl = 3600;
                    
                    // Try adding to different sections
                    ares_dns_section_t sections[] = {
                        ARES_SECTION_ANSWER,
                        ARES_SECTION_AUTHORITY,
                        ARES_SECTION_ADDITIONAL
                    };
                    
                    for (int i = 0; i < 3; i++) {
                        ares_status_t add_status = ares_dns_record_rr_add(
                            &rr_out, dnsrec, sections[i], name, type, rclass, ttl);
                        
                        if (add_status == ARES_SUCCESS && rr_out != NULL) {
                            // Test ares_dns_rr_get_class on the newly added RR
                            ares_dns_class_t rr_class = ares_dns_rr_get_class(rr_out);
                            (void)rr_class; // Avoid unused variable warning
                        }
                    }
                    
                    // Clean up
                    ares_dns_record_destroy(dnsrec);
                }
                
                free(dns_buf);
            }
        }
    }
    
    // Test ares_dns_class_fromstr with various string inputs
    // Use different positions in the input for string testing
    if (Size > 10) {
        // Test with common DNS class strings
        const char *class_strings[] = {"IN", "CS", "CH", "HS", "NONE", "ANY"};
        
        for (int i = 0; i < 6; i++) {
            ares_dns_class_t qclass;
            ares_dns_class_fromstr(&qclass, class_strings[i]);
        }
        
        // Test with random string from input
        if (Size > 20) {
            char *random_str = malloc(11);
            if (random_str) {
                memcpy(random_str, Data + 10, 10);
                random_str[10] = '\0';
                
                ares_dns_class_t qclass;
                ares_dns_class_fromstr(&qclass, random_str);
                
                free(random_str);
            }
        }
    }
    
    return 0;
}

#ifdef __cplusplus
}
#endif
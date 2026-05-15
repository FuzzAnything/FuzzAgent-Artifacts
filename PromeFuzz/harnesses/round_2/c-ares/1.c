// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_query_cnt at ares_dns_record.c:244:8 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_query_cnt at ares_dns_record.c:244:8 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef CARES_EXTERN
#define CARES_EXTERN extern
#endif

#include "ares.h"
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Step 1: Parse DNS message using ares_dns_parse
    ares_dns_record_t *dnsrec = NULL;
    unsigned int flags = 0;
    ares_status_t parse_status = ares_dns_parse(Data, Size, flags, &dnsrec);
    
    if (parse_status != ARES_SUCCESS) {
        // If parsing fails, we can still test some functions with NULL input
        // to explore error handling paths
        ares_dns_record_query_cnt(NULL);
        ares_dns_record_rr_cnt(NULL, ARES_SECTION_ANSWER);
        return 0;
    }
    
    // Step 2: Get query count and iterate through queries
    size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
    for (size_t i = 0; i < query_cnt; i++) {
        const char *name = NULL;
        ares_dns_rec_type_t qtype;
        ares_dns_class_t qclass;
        
        ares_status_t query_status = ares_dns_record_query_get(
            dnsrec, i, &name, &qtype, &qclass);
        
        // Test with NULL parameters to explore different paths
        if (i % 2 == 0) {
            ares_dns_record_query_get(dnsrec, i, NULL, &qtype, &qclass);
            ares_dns_record_query_get(dnsrec, i, &name, NULL, &qclass);
            ares_dns_record_query_get(dnsrec, i, &name, &qtype, NULL);
            ares_dns_record_query_get(dnsrec, i, NULL, NULL, NULL);
        }
    }
    
    // Step 3: Test resource record counts for different sections
    static const ares_dns_section_t sections[] = {
        ARES_SECTION_ANSWER,
        ARES_SECTION_AUTHORITY,
        ARES_SECTION_ADDITIONAL
    };
    
    for (int sect_idx = 0; sect_idx < 3; sect_idx++) {
        ares_dns_section_t sect = sections[sect_idx];
        size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, sect);
        
        // Step 4: Get and examine resource records
        for (size_t i = 0; i < rr_cnt; i++) {
            ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, sect, i);
            if (rr) {
                // Step 5: Get the RR type
                ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                
                // Step 6: Get keys for this RR type
                size_t key_cnt = 0;
                const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(rr_type, &key_cnt);
                
                // Test with NULL parameters
                if (i % 2 == 0) {
                    ares_dns_rr_get_keys(rr_type, NULL);
                }
            }
            
            // Test out of bounds access
            if (i == 0 && rr_cnt > 0) {
                ares_dns_record_rr_get(dnsrec, sect, rr_cnt);  // Should return NULL
            }
        }
        
        // Test with invalid section (if any)
        if (sect_idx == 0) {
            // Use a potentially invalid section value
            ares_dns_record_rr_cnt(dnsrec, (ares_dns_section_t)999);
            ares_dns_record_rr_get(dnsrec, (ares_dns_section_t)999, 0);
        }
    }
    
    // Cleanup
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    return 0;
}
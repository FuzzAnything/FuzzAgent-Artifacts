// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_query_cnt at ares_dns_record.c:244:8 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_record_query_cnt at ares_dns_record.c:244:8 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include necessary c-ares headers to get type definitions */
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Parse DNS record from input data
    ares_dns_record_t *dnsrec = NULL;
    unsigned int flags = 0;
    
    ares_status_t parse_status = ares_dns_parse(Data, Size, flags, &dnsrec);
    
    if (parse_status != ARES_SUCCESS || dnsrec == NULL) {
        // If parsing failed, still exercise some functions with NULL input
        ares_dns_record_query_cnt(NULL);
        ares_dns_record_rr_cnt(NULL, ARES_SECTION_ANSWER);
        
        // Test ares_dns_rr_get_keys with some example types
        size_t cnt = 0;
        ares_dns_rr_get_keys(ARES_REC_TYPE_A, &cnt);
        ares_dns_rr_get_keys(ARES_REC_TYPE_AAAA, &cnt);
        ares_dns_rr_get_keys(ARES_REC_TYPE_MX, &cnt);
        ares_dns_rr_get_keys(ARES_REC_TYPE_TXT, &cnt);
        
        return 0;
    }
    
    // Get query count
    size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
    
    // For each query, get its details
    for (size_t i = 0; i < query_cnt; i++) {
        const char *name = NULL;
        ares_dns_rec_type_t qtype;
        ares_dns_class_t qclass;
        
        ares_status_t query_get_status = ares_dns_record_query_get(
            dnsrec, i, &name, &qtype, &qclass);
        
        // Try with NULL outputs to test different code paths
        if (i % 2 == 0) {
            ares_dns_record_query_get(dnsrec, i, NULL, NULL, NULL);
        }
    }
    
    // Define sections to iterate through
    ares_dns_section_t sections[] = {
        ARES_SECTION_ANSWER,
        ARES_SECTION_AUTHORITY,
        ARES_SECTION_ADDITIONAL
    };
    size_t num_sections = sizeof(sections) / sizeof(sections[0]);
    
    // For each section, get RR count and iterate through RRs
    for (size_t sect_idx = 0; sect_idx < num_sections; sect_idx++) {
        size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, sections[sect_idx]);
        
        for (size_t rr_idx = 0; rr_idx < rr_cnt; rr_idx++) {
            // Get the RR object
            ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, sections[sect_idx], rr_idx);
            
            if (rr != NULL) {
                // Get the RR type
                ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                
                // Get keys for this RR type
                size_t key_cnt = 0;
                const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(rr_type, &key_cnt);
                
                // Also test with some other common types
                if (rr_idx % 3 == 0) {
                    size_t dummy_cnt;
                    ares_dns_rr_get_keys(ARES_REC_TYPE_NS, &dummy_cnt);
                    ares_dns_rr_get_keys(ARES_REC_TYPE_CNAME, &dummy_cnt);
                    ares_dns_rr_get_keys(ARES_REC_TYPE_SOA, &dummy_cnt);
                }
            }
            
            // Try to get out-of-bounds RR (should return NULL)
            if (rr_idx == 0) {
                ares_dns_record_rr_get(dnsrec, sections[sect_idx], rr_cnt + 1);
            }
        }
    }
    
    // Test with invalid section
    ares_dns_record_rr_cnt(dnsrec, (ares_dns_section_t)999);
    
    // Test ares_dns_record_rr_get with invalid parameters
    ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, query_cnt + 100);
    
    // Test ares_dns_rr_get_type with NULL
    ares_dns_rr_get_type(NULL);
    
    // Clean up - ares_dns_record_destroy is defined in ares_dns_record.h
    if (dnsrec != NULL) {
        ares_dns_record_destroy(dnsrec);
    }
    
    return 0;
}
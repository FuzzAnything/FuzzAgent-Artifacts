// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define missing types that are referenced in ares_dns_record.h */
typedef int ares_bool_t;
typedef int ares_status_t;

/* Define CARES_EXTERN if not already defined */
#ifndef CARES_EXTERN
# if defined(_WIN32) && !defined(CARES_STATICLIB)
#  if defined(CARES_BUILDING_LIBRARY)
#   define CARES_EXTERN __declspec(dllexport)
#  else
#   define CARES_EXTERN __declspec(dllimport)
#  endif
# else
#  define CARES_EXTERN extern
# endif
#endif

#include "ares_dns_record.h"

static void dummy_cleanup(ares_dns_record_t *dnsrec) {
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    ares_dns_record_t *dnsrec = NULL;
    
    // Step 1: Parse the DNS message
    ares_status_t status = ares_dns_parse(Data, Size, 0, &dnsrec);
    if (status != 0) {  // ARES_SUCCESS is typically 0
        // If parsing fails, we can't proceed with the other functions
        dummy_cleanup(dnsrec);
        return 0;
    }
    
    // Step 2: Get query count
    size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
    
    // Step 3: Try to get each query
    for (size_t i = 0; i < query_cnt; i++) {
        const char *name = NULL;
        ares_dns_rec_type_t qtype;
        ares_dns_class_t qclass;
        
        // Call with all parameters
        ares_dns_record_query_get(dnsrec, i, &name, &qtype, &qclass);
        
        // Also try calling with NULL for optional parameters
        ares_dns_record_query_get(dnsrec, i, NULL, NULL, NULL);
        ares_dns_record_query_get(dnsrec, i, &name, NULL, NULL);
        ares_dns_record_query_get(dnsrec, i, NULL, &qtype, NULL);
        ares_dns_record_query_get(dnsrec, i, NULL, NULL, &qclass);
    }
    
    // Define sections to iterate through
    ares_dns_section_t sections[] = {
        ARES_SECTION_ANSWER,
        ARES_SECTION_AUTHORITY,
        ARES_SECTION_ADDITIONAL
    };
    
    // Step 4 & 5: Process each section
    for (size_t sect_idx = 0; sect_idx < sizeof(sections)/sizeof(sections[0]); sect_idx++) {
        size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, sections[sect_idx]);
        
        // Step 6: Get each RR in the section
        for (size_t rr_idx = 0; rr_idx < rr_cnt; rr_idx++) {
            ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, sections[sect_idx], rr_idx);
            if (!rr) {
                continue;
            }
            
            // Step 7: Get RR type
            ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
            
            // Step 8: Get keys for this RR type
            size_t key_cnt = 0;
            const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(rr_type, &key_cnt);
            
            // Also try with NULL for cnt parameter
            ares_dns_rr_get_keys(rr_type, NULL);
        }
    }
    
    // Cleanup
    dummy_cleanup(dnsrec);
    return 0;
}
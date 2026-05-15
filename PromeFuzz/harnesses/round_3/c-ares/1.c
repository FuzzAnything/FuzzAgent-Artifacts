// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_query_cnt at ares_dns_record.c:244:8 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include necessary headers in correct order */
#include "ares.h"
#include "ares_dns_record.h"

static void dummy_cleanup(ares_dns_record_t *dnsrec) {
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status;
    
    // 1. Parse the DNS message
    status = ares_dns_parse(Data, Size, 0, &dnsrec);
    if (status != ARES_SUCCESS) {
        // If parsing fails, we can't proceed with the other functions
        dummy_cleanup(dnsrec);
        return 0;
    }
    
    // 2. Get query count
    size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
    
    // 3. Try to get each query if there are any
    for (size_t i = 0; i < query_cnt; i++) {
        const char *name = NULL;
        ares_dns_rec_type_t qtype;
        ares_dns_class_t qclass;
        
        ares_dns_record_query_get(dnsrec, i, &name, &qtype, &qclass);
    }
    
    // 4. Get RR counts for each section
    ares_dns_section_t sections[] = {
        ARES_SECTION_ANSWER,
        ARES_SECTION_AUTHORITY,
        ARES_SECTION_ADDITIONAL
    };
    
    for (size_t sect_idx = 0; sect_idx < 3; sect_idx++) {
        size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, sections[sect_idx]);
        
        // 5. Try to get each RR in the section
        for (size_t rr_idx = 0; rr_idx < rr_cnt; rr_idx++) {
            ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, sections[sect_idx], rr_idx);
            if (rr) {
                // 6. Get RR type
                ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                
                // 7. Get keys for this RR type
                size_t key_cnt = 0;
                const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(rr_type, &key_cnt);
                // keys can be NULL if key_cnt is 0 or type is invalid
            }
        }
    }
    
    // Clean up
    dummy_cleanup(dnsrec);
    return 0;
}
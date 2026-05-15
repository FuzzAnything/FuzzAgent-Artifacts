// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get_const at ares_dns_record.c:499:3 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
// ares_dns_record_rr_get_const at ares_dns_record.c:499:3 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Include ares.h first to define CARES_EXTERN before including ares_dns_record.h
#include "ares.h"
#include "ares_dns_record.h"

static void test_functions(const ares_dns_record_t *dnsrec) {
    if (!dnsrec) return;
    
    // Test all possible sections
    ares_dns_section_t sections[] = {
        ARES_SECTION_ANSWER,
        ARES_SECTION_AUTHORITY,
        ARES_SECTION_ADDITIONAL
    };
    
    for (size_t s = 0; s < sizeof(sections)/sizeof(sections[0]); s++) {
        ares_dns_section_t sect = sections[s];
        
        // First function: get count of RRs in section
        size_t cnt = ares_dns_record_rr_cnt(dnsrec, sect);
        
        // Second and third functions: iterate through RRs
        for (size_t idx = 0; idx < cnt; idx++) {
            const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, sect, idx);
            if (rr) {
                // Third function: get RR type
                ares_dns_rec_type_t type = ares_dns_rr_get_type(rr);
                (void)type; // Use result to avoid unused variable warning
            }
        }
        
        // Also test out-of-bounds indices
        const ares_dns_rr_t *out_of_bounds = ares_dns_record_rr_get_const(dnsrec, sect, cnt);
        (void)out_of_bounds;
    }
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    // Create a DNS record from the fuzzer input
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status = ares_dns_parse(Data, Size, 0, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Test the target functions
        test_functions(dnsrec);
        
        // Clean up
        ares_dns_record_destroy(dnsrec);
    }
    
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_section_tostr at ares_dns_mapping.c:737:13 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef CARES_EXTERN
# if defined(WIN32) && defined(CARES_STATICLIB)
#  define CARES_EXTERN
# elif defined(WIN32)
#  define CARES_EXTERN __declspec(dllimport)
# else
#  define CARES_EXTERN extern
# endif
#endif

#include "ares.h"
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    // Generate random enum values from the fuzzer input
    ares_dns_rec_type_t rec_type = 0;
    ares_dns_section_t section = 0;
    
    if (Size > 0) {
        // Use first byte for record type
        rec_type = (ares_dns_rec_type_t)(Data[0]);
    }
    
    if (Size > 1) {
        // Use second byte for section
        section = (ares_dns_section_t)(Data[1]);
    }
    
    // Call the API functions in the specified order
    
    // 1. ares_dns_rec_type_tostr
    const char *type_str = ares_dns_rec_type_tostr(rec_type);
    (void)type_str;
    
    // 2. ares_dns_section_tostr
    const char *section_str = ares_dns_section_tostr(section);
    (void)section_str;
    
    // For ares_dns_record_rr_cnt and ares_dns_record_rr_get, we need a DNS record
    // Since we don't have a create function in the API, we'll try to use NULL
    ares_dns_record_t *dnsrec = NULL;
    
    // 3. ares_dns_record_rr_cnt
    size_t rr_count = ares_dns_record_rr_cnt(dnsrec, section);
    (void)rr_count;
    
    // 4. ares_dns_record_rr_get - try to get RRs with NULL dnsrec
    ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, section, 0);
    (void)rr;
    
    // Also test with some random indices if we have enough data
    if (Size > 2) {
        for (size_t i = 0; i < 4 && i < Size - 2; i++) {
            size_t idx = (size_t)(Data[2 + i]) % 16;
            ares_dns_rr_t *rr2 = ares_dns_record_rr_get(dnsrec, section, idx);
            (void)rr2;
        }
    }
    
    return 0;
}
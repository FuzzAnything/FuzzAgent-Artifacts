// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_section_tostr at ares_dns_mapping.c:737:13 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_section_tostr at ares_dns_mapping.c:737:13 in ares_dns_record.h
// ares_dns_section_tostr at ares_dns_mapping.c:737:13 in ares_dns_record.h
#include "ares.h"
#include "ares_dns_record.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // 1. Call ares_dns_rec_type_tostr with various enum values derived from fuzzer input
    if (Size > 0) {
        ares_dns_rec_type_t type = (ares_dns_rec_type_t)(Data[0] % 256);
        const char *type_str = ares_dns_rec_type_tostr(type);
        (void)type_str;
    }
    
    // 2. Call ares_dns_section_tostr with various enum values derived from fuzzer input
    if (Size > 1) {
        ares_dns_section_t section = (ares_dns_section_t)(Data[1] % 256);
        const char *section_str = ares_dns_section_tostr(section);
        (void)section_str;
    }
    
    // 3. Call ares_dns_record_rr_cnt with NULL and various sections
    if (Size > 2) {
        for (int i = 0; i < 4 && i + 2 < (int)Size; i++) {
            ares_dns_section_t test_section = (ares_dns_section_t)(Data[2 + i] % 4);
            size_t count = ares_dns_record_rr_cnt(NULL, test_section);
            (void)count;
            
            // 4. Call ares_dns_record_rr_get with NULL and the same section and an index
            if (Size > 3 + i) {
                size_t idx = Data[3 + i];
                ares_dns_rr_t *rr = ares_dns_record_rr_get(NULL, test_section, idx);
                (void)rr;
            }
        }
    }
    
    // Additional edge case tests
    const char *min_type_str = ares_dns_rec_type_tostr((ares_dns_rec_type_t)0);
    const char *max_type_str = ares_dns_rec_type_tostr((ares_dns_rec_type_t)255);
    (void)min_type_str;
    (void)max_type_str;
    
    const char *min_section_str = ares_dns_section_tostr((ares_dns_section_t)0);
    const char *max_section_str = ares_dns_section_tostr((ares_dns_section_t)255);
    (void)min_section_str;
    (void)max_section_str;
    
    return 0;
}
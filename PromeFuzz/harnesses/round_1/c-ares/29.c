// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_get_name at ares_dns_record.c:505:13 in ares_dns_record.h
// ares_dns_class_tostr at ares_dns_mapping.c:236:13 in ares_dns_record.h
// ares_dns_rr_get_class at ares_dns_record.c:521:18 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_class_tostr at ares_dns_mapping.c:236:13 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    // Test ares_dns_rr_get_name with NULL
    ares_dns_rr_get_name(NULL);
    
    // Test ares_dns_class_tostr with various values from fuzz data
    if (Size > 0) {
        for (size_t i = 0; i < Size && i < 8; i++) {
            ares_dns_class_t test_class = (ares_dns_class_t)Data[i];
            const char *class_str = ares_dns_class_tostr(test_class);
            (void)class_str;
        }
    }
    
    // Test ares_dns_rr_get_class with NULL
    ares_dns_class_t class_result = ares_dns_rr_get_class(NULL);
    (void)class_result;
    
    // Test ares_dns_rec_type_tostr with various values from fuzz data
    if (Size > 0) {
        for (size_t i = 0; i < Size && i < 8; i++) {
            ares_dns_rec_type_t test_type = (ares_dns_rec_type_t)Data[i];
            const char *type_str = ares_dns_rec_type_tostr(test_type);
            (void)type_str;
        }
    }
    
    // Also test some out-of-range values
    if (Size > 0) {
        for (size_t i = 0; i < Size && i < 4; i++) {
            ares_dns_class_t test_class = (ares_dns_class_t)(Data[i] + 256);
            ares_dns_rec_type_t test_type = (ares_dns_rec_type_t)(Data[i] + 256);
            
            ares_dns_class_tostr(test_class);
            ares_dns_rec_type_tostr(test_type);
        }
    }
    
    return 0;
}
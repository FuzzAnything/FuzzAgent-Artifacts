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

int LLVMFuzzerTestOneInput_13(const uint8_t *data, size_t size) {
    /* 1. Call ares_dns_rr_get_name with NULL to test that path */
    const char *name = ares_dns_rr_get_name(NULL);
    (void)name;  /* Use result to avoid compiler warnings */
    
    /* 2. Call ares_dns_class_tostr with various enum values derived from fuzz data */
    ares_dns_class_t class_val;
    if (size > 0) {
        /* Use data[0] to generate different class values */
        class_val = (ares_dns_class_t)(data[0] % 256);  /* Cast to enum type */
    } else {
        class_val = ARES_CLASS_IN;  /* Default value */
    }
    const char *class_str = ares_dns_class_tostr(class_val);
    (void)class_str;
    
    /* 3. Call ares_dns_rr_get_class with NULL to test that path */
    ares_dns_class_t rr_class = ares_dns_rr_get_class(NULL);
    (void)rr_class;
    
    /* 4. Call ares_dns_rec_type_tostr with various enum values derived from fuzz data */
    ares_dns_rec_type_t type_val;
    if (size > 1) {
        /* Use data[1] to generate different record type values */
        type_val = (ares_dns_rec_type_t)(data[1] % 256);  /* Cast to enum type */
    } else {
        type_val = ARES_REC_TYPE_A;  /* Default value */
    }
    const char *type_str = ares_dns_rec_type_tostr(type_val);
    (void)type_str;
    
    /* Also test with some known valid enum values if we have enough data */
    if (size >= 4) {
        /* Test with a range of values from the fuzz data */
        for (size_t i = 0; i < size && i < 16; i += 2) {
            ares_dns_class_t test_class = (ares_dns_class_t)data[i];
            const char *test_class_str = ares_dns_class_tostr(test_class);
            (void)test_class_str;
            
            if (i + 1 < size) {
                ares_dns_rec_type_t test_type = (ares_dns_rec_type_t)data[i + 1];
                const char *test_type_str = ares_dns_rec_type_tostr(test_type);
                (void)test_type_str;
            }
        }
    }
    
    return 0;
}
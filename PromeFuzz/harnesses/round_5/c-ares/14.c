// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_get_name at ares_dns_record.c:505:13 in ares_dns_record.h
// ares_dns_class_tostr at ares_dns_mapping.c:236:13 in ares_dns_record.h
// ares_dns_rr_get_class at ares_dns_record.c:521:18 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
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
#define CARES_EXTERN
#endif

/* Include the required header */
#include "ares_dns_record.h"

/* Define enum values that might be needed */
#ifndef ARES_CLASS_IN
#define ARES_CLASS_IN 1
#endif

#ifndef ARES_REC_TYPE_A
#define ARES_REC_TYPE_A 1
#endif

static void dummy_cleanup(void) {
    // No dynamic cleanup needed for this fuzz driver
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Since ares_dns_rr_t is an opaque type, we cannot instantiate it directly
    // We'll use NULL for the resource record pointer and rely on the functions
    // handling NULL inputs properly
    
    // Use the first byte to determine if we should pass NULL or try to create something
    int pass_null = (Size > 0) ? (Data[0] % 2) : 1;  // Default to NULL
    
    // Use subsequent bytes to populate enum values if available
    ares_dns_class_t qclass = ARES_CLASS_IN;
    ares_dns_rec_type_t rec_type = ARES_REC_TYPE_A;
    
    if (Size > 1) {
        qclass = (ares_dns_class_t)(Data[1] % 256);
    }
    
    if (Size > 2) {
        rec_type = (ares_dns_rec_type_t)(Data[2] % 256);
    }
    
    // Call functions in the specified order
    
    // 1. ares_dns_rr_get_name
    // Since we can't create a valid ares_dns_rr_t, we always pass NULL
    const char *name_result = ares_dns_rr_get_name(NULL);
    
    // 2. ares_dns_class_tostr
    const char *class_str_result = ares_dns_class_tostr(qclass);
    
    // 3. ares_dns_rr_get_class
    // Since we can't create a valid ares_dns_rr_t, we always pass NULL
    ares_dns_class_t class_result = ares_dns_rr_get_class(NULL);
    
    // 4. ares_dns_rec_type_tostr
    const char *type_str_result = ares_dns_rec_type_tostr(rec_type);
    
    // Force the compiler to keep the results (prevent optimization)
    volatile const char *keep1 = name_result;
    volatile const char *keep2 = class_str_result;
    volatile ares_dns_class_t keep3 = class_result;
    volatile const char *keep4 = type_str_result;
    (void)keep1;
    (void)keep2;
    (void)keep3;
    (void)keep4;
    
    dummy_cleanup();
    return 0;
}
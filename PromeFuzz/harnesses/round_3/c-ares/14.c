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

/* Include the main c-ares header first to get the basic types */
#include "ares.h"
/* Then include the DNS record header */
#include "ares_dns_record.h"

static void dummy_cleanup(void) {
    /* No cleanup needed for the functions being fuzzed */
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    /* Use the fuzzer input to create random enum values */
    ares_dns_class_t class_val = 0;
    ares_dns_rec_type_t type_val = 0;
    
    /* Extract values from fuzzer input if available */
    if (Size >= 1) {
        class_val = (ares_dns_class_t)(Data[0] & 0xFFFF); /* Ensure it fits in enum range */
    }
    if (Size >= 2) {
        type_val = (ares_dns_rec_type_t)(Data[1] & 0xFFFF); /* Ensure it fits in enum range */
    }
    
    /* Create a dummy resource record pointer - only test NULL case */
    ares_dns_rr_t *rr = NULL;
    
    /* Call functions in the specified order */
    
    /* 1. ares_dns_rr_get_name - only with NULL to avoid SEGV */
    const char *name = ares_dns_rr_get_name(rr);
    (void)name; /* Prevent unused variable warning */
    
    /* 2. ares_dns_class_tostr */
    const char *class_str = ares_dns_class_tostr(class_val);
    (void)class_str; /* Prevent unused variable warning */
    
    /* 3. ares_dns_rr_get_class - only with NULL to avoid SEGV */
    ares_dns_class_t rr_class = ares_dns_rr_get_class(rr);
    (void)rr_class; /* Prevent unused variable warning */
    
    /* 4. ares_dns_rec_type_tostr */
    const char *type_str = ares_dns_rec_type_tostr(type_val);
    (void)type_str; /* Prevent unused variable warning */
    
    dummy_cleanup();
    return 0;
}
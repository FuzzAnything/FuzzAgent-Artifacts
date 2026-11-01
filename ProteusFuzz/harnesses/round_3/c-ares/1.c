// This fuzz driver is generated for library c-ares, aiming to fuzz the following functions:
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_key_to_rec_type at ares_dns_mapping.c:176:21 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_key_to_rec_type at ares_dns_mapping.c:176:21 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rec_type_fromstr at ares_dns_mapping.c:692:13 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if not already defined to fix compilation error */
#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

/* Define missing types that are expected by ares_dns_record.h but might not be included transitively */
typedef enum {
    ARES_FALSE = 0,
    ARES_TRUE = 1
} ares_bool_t;

typedef enum {
    ARES_SUCCESS = 0,
    ARES_EFORMERR = 1,
    ARES_ESERVFAIL = 2,
    ARES_ENOTFOUND = 3,
    ARES_ENODATA = 4,
    ARES_EBADQUERY = 5,
    ARES_EBADNAME = 6,
    ARES_EBADFAMILY = 7,
    ARES_EBADRESP = 8,
    ARES_ECONNREFUSED = 9,
    ARES_ETIMEOUT = 10,
    ARES_EOF = 11,
    ARES_EFILE = 12,
    ARES_ENOMEM = 13,
    ARES_EDESTRUCTION = 14,
    ARES_EBADSTR = 15,
    ARES_EBADFLAGS = 16,
    ARES_ENOTIMP = 17,
    ARES_EQUERYFAIL = 18,
    ARES_ENODNS = 19,
    ARES_EBADHINTS = 20,
    ARES_ELOADIPHLPAPI = 21,
    ARES_EADDRGETNETWORKPARAMS = 22,
    ARES_ECANCELLED = 23
} ares_status_t;

#include <ares_dns_record.h>

/* 
 * The fuzzer entry point must be defined exactly as LLVMFuzzerTestOneInput.
 * The linker error "undefined reference to `LLVMFuzzerTestOneInput'" usually happens
 * if the function is not visible or if there is a mismatch in the build configuration.
 * We ensure it is a global function here.
 */
int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* 1. Prepare environment */
    ares_dns_rec_type_t qtype;
    const char *str;
    const char *str_result;
    const ares_dns_rr_key_t *keys;
    size_t cnt = 0;
    ares_dns_datatype_t datatype;
    ares_dns_rec_type_t rec_type;
    ares_bool_t fromstr_result;
    const char *key_str;

    /* Use the input data to generate a string for parsing */
    /* Ensure null termination for the string */
    size_t str_len = Size;
    if (str_len > 100) {
        str_len = 100;
    }
    char input_buf[101];
    memcpy(input_buf, Data, str_len);
    input_buf[str_len] = '\0';
    str = input_buf;

    /* Invoke: ares_dns_rec_type_fromstr */
    /* This function expects a string representation of a record type */
    fromstr_result = ares_dns_rec_type_fromstr(&qtype, str);

    /* Invoke: ares_dns_rec_type_tostr */
    /* Convert the result back to string (or test with derived types) */
    if (fromstr_result) {
        str_result = ares_dns_rec_type_tostr(qtype);
        (void)str_result; /* Use result to avoid unused warning */
    }

    /* Test with a few known types to ensure coverage if fromstr failed */
    ares_dns_rec_type_t test_types[] = {
        ARES_REC_TYPE_A,
        ARES_REC_TYPE_AAAA,
        ARES_REC_TYPE_CNAME,
        ARES_REC_TYPE_NS,
        ARES_REC_TYPE_MX,
        ARES_REC_TYPE_TXT,
        ARES_REC_TYPE_ANY,
        ARES_REC_TYPE_SOA,
        ARES_REC_TYPE_PTR
    };
    size_t num_test_types = sizeof(test_types) / sizeof(test_types[0]);

    for (size_t i = 0; i < num_test_types; i++) {
        ares_dns_rec_type_t current_type = test_types[i];
        
        /* Invoke: ares_dns_rec_type_tostr */
        str_result = ares_dns_rec_type_tostr(current_type);
        (void)str_result;

        /* Invoke: ares_dns_rr_get_keys */
        keys = ares_dns_rr_get_keys(current_type, &cnt);
        if (keys != NULL) {
            /* Iterate over keys to invoke other functions */
            for (size_t j = 0; j < cnt; j++) {
                ares_dns_rr_key_t key = keys[j];

                /* Invoke: ares_dns_rr_key_tostr */
                key_str = ares_dns_rr_key_tostr(key);
                (void)key_str;

                /* Invoke: ares_dns_rr_key_to_rec_type */
                rec_type = ares_dns_rr_key_to_rec_type(key);
                (void)rec_type;

                /* Invoke: ares_dns_rr_key_datatype */
                datatype = ares_dns_rr_key_datatype(key);
                (void)datatype;
            }
        }
    }

    /* Also test with a value derived from input to explore more states */
    if (Size >= sizeof(ares_dns_rr_key_t)) {
        ares_dns_rr_key_t fuzz_key;
        memcpy(&fuzz_key, Data, sizeof(fuzz_key));
        
        /* Invoke: ares_dns_rr_key_tostr */
        key_str = ares_dns_rr_key_tostr(fuzz_key);
        (void)key_str;

        /* Invoke: ares_dns_rr_key_to_rec_type */
        rec_type = ares_dns_rr_key_to_rec_type(fuzz_key);
        (void)rec_type;

        /* Invoke: ares_dns_rr_key_datatype */
        datatype = ares_dns_rr_key_datatype(fuzz_key);
        (void)datatype;
    }

    /* 2. Cleanup */
    /* No dynamic memory allocated by the API functions that needs freeing here. */
    /* The returned strings are static. */
    
    return 0;
}
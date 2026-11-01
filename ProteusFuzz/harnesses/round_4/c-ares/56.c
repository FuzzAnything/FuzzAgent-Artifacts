// This fuzz driver is generated for library c-ares, aiming to fuzz the following functions:
// ares_dns_rec_type_fromstr at ares_dns_mapping.c:692:13 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_rr_key_to_rec_type at ares_dns_mapping.c:176:21 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if not already defined to fix compilation errors */
#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

/* Define missing types that are usually in ares.h or ares_private.h */
typedef enum { ARES_FALSE = 0, ARES_TRUE = 1 } ares_bool_t;

typedef enum {
  ARES_SUCCESS = 0,
  ARES_EFORMERR = 1,
  ARES_ESERVFAIL = 2,
  ARES_ENOTFOUND = 3,
  ARES_ENODATA = 4,
  ARES_ENXDOMAIN = 5,
  ARES_EBADQUERY = 6,
  ARES_EBADNAME = 7,
  ARES_EBADFAMILY = 8,
  ARES_EBADRESP = 9,
  ARES_ECONNREFUSED = 10,
  ARES_ETIMEOUT = 11,
  ARES_EOF = 12,
  ARES_EFILE = 13,
  ARES_ENOMEM = 14,
  ARES_EDESTRUCTION = 15,
  ARES_EBADSTR = 16,
  ARES_EBADFLAGS = 17,
  ARES_ENODNS = 18,
  ARES_EBADHINTS = 19,
  ARES_ELOADIPHLPAPI = 20,
  ARES_EADDRGETNETWORKPARAMS = 21,
  ARES_ECANCELLED = 22
} ares_status_t;

#include <ares_dns_record.h>

static void test_rec_type_conversion(const uint8_t *data, size_t size) {
    ares_dns_rec_type_t qtype;
    char str_buf[256];
    const char *str = NULL;

    if (size > 0) {
        size_t len = (size < sizeof(str_buf) - 1) ? size : sizeof(str_buf) - 1;
        memcpy(str_buf, data, len);
        str_buf[len] = '\0';
        str = str_buf;
    }

    if (str) {
        ares_bool_t res = ares_dns_rec_type_fromstr(&qtype, str);
        (void)res;
        
        const char *to_str = ares_dns_rec_type_tostr(qtype);
        if (to_str) {
            (void)strlen(to_str);
        }
    }
}

static void test_rr_keys(const uint8_t *data, size_t size) {
    ares_dns_rec_type_t type;
    size_t cnt = 0;
    
    if (size > 0) {
        type = (ares_dns_rec_type_t)(data[0] & 0xFF);
    } else {
        type = ARES_REC_TYPE_A;
    }

    const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(type, &cnt);
    if (keys) {
        for (size_t i = 0; i < cnt; i++) {
            ares_dns_rec_type_t derived_type = ares_dns_rr_key_to_rec_type(keys[i]);
            const char *key_str = ares_dns_rr_key_tostr(keys[i]);
            if (key_str) {
                (void)strlen(key_str);
            }
            (void)derived_type;
        }
    }
}

static void test_rr_type_from_data(const uint8_t *data, size_t size) {
    /* ares_dns_rr_t is an opaque struct, we cannot instantiate it directly.
       We can only pass NULL or a pointer to a valid object created by the library.
       Since we cannot create a valid object without internal library functions,
       we test the NULL case and rely on other tests for valid objects if they existed.
       However, to exercise the function logic, we can only pass NULL here safely.
    */
    ares_dns_rec_type_t null_type = ares_dns_rr_get_type(NULL);
    (void)null_type;
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    test_rec_type_conversion(Data, Size);
    test_rr_keys(Data, Size);
    test_rr_type_from_data(Data, Size);

    return 0;
}
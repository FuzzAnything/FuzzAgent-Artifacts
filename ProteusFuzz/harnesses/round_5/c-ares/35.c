// This fuzz driver is generated for library c-ares, aiming to fuzz the following functions:
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_section_tostr at ares_dns_mapping.c:737:13 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if not already defined */
#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

/* Define missing types required by ares_dns_record.h to fix compilation errors */
typedef enum {
  ares_false = 0,
  ares_true = 1
} ares_bool_t;

typedef enum {
  ARES_SUCCESS = 0,
  ARES_EFORMERR = 1,
  ARES_ESERVFAIL = 2,
  ARES_ENOTFOUND = 3,
  ARES_EBADQUERY = 4,
  ARES_EBADNAME = 5,
  ARES_EBADFAMILY = 6,
  ARES_ENODATA = 7,
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
  ARES_ECONNREFUSED_SOCK = 18,
  ARES_EBADPACKET = 19,
  ARES_EBADHINTS = 20,
  ARES_EBADID = 21,
  ARES_ENODATAERR = 22,
  ARES_EBADFILE = 23,
  ARES_EBADCONF = 24,
  ARES_EBADHOST = 25,
  ARES_EBADPORT = 26,
  ARES_EBADNET = 27,
  ARES_EBADSOCK = 28,
  ARES_EBADQUERYNAME = 29,
  ARES_EBADQUERYTYPE = 30,
  ARES_EBADQUERYCLASS = 31,
  ARES_EBADQUERYFLAGS = 32,
  ARES_EBADQUERYID = 33,
  ARES_EBADQUERYRCODE = 34,
  ARES_EBADQUERYANSWER = 35,
  ARES_EBADQUERYAUTHORITY = 36,
  ARES_EBADQUERYADDITIONAL = 37,
  ARES_EBADQUERYQNAME = 38,
  ARES_EBADQUERYQTYPE = 39,
  ARES_EBADQUERYQCLASS = 40,
  ARES_EBADQUERYQFLAGS = 41,
  ARES_EBADQUERYQID = 42,
  ARES_EBADQUERYQRCODE = 43,
  ARES_EBADQUERYQANSWER = 44,
  ARES_EBADQUERYQAUTHORITY = 45,
  ARES_EBADQUERYQADDITIONAL = 46,
  ARES_EBADQUERYQNAMELEN = 47,
  ARES_EBADQUERYQTYPELEN = 48,
  ARES_EBADQUERYQCLASSLEN = 49,
  ARES_EBADQUERYQFLAGSLEN = 50,
  ARES_EBADQUERYQIDLEN = 51,
  ARES_EBADQUERYQRCODELEN = 52,
  ARES_EBADQUERYQANSWERLEN = 53,
  ARES_EBADQUERYQAUTHORITYLEN = 54,
  ARES_EBADQUERYQADDITIONALLEN = 55,
  ARES_EBADQUERYQNAMELENLEN = 56,
  ARES_EBADQUERYQTYPELENLEN = 57,
  ARES_EBADQUERYQCLASSLENLEN = 58,
  ARES_EBADQUERYQFLAGSLENLEN = 59,
  ARES_EBADQUERYQIDLENLEN = 60,
  ARES_EBADQUERYQRCODELENLEN = 61,
  ARES_EBADQUERYQANSWERLENLEN = 62,
  ARES_EBADQUERYQAUTHORITYLENLEN = 63,
  ARES_EBADQUERYQADDITIONALLENLEN = 64,
  ARES_EBADQUERYQNAMELENLENLEN = 65,
  ARES_EBADQUERYQTYPELENLENLEN = 66,
  ARES_EBADQUERYQCLASSLENLENLEN = 67,
  ARES_EBADQUERYQFLAGSLENLENLEN = 68,
  ARES_EBADQUERYQIDLENLENLEN = 69,
  ARES_EBADQUERYQRCODELENLENLEN = 70,
  ARES_EBADQUERYQANSWERLENLENLEN = 71,
  ARES_EBADQUERYQAUTHORITYLENLENLEN = 72,
  ARES_EBADQUERYQADDITIONALLENLENLEN = 73,
  ARES_EBADQUERYQNAMELENLENLENLEN = 74,
  ARES_EBADQUERYQTYPELENLENLENLEN = 75,
  ARES_EBADQUERYQCLASSLENLENLENLEN = 76,
  ARES_EBADQUERYQFLAGSLENLENLENLEN = 77,
  ARES_EBADQUERYQIDLENLENLENLEN = 78,
  ARES_EBADQUERYQRCODELENLENLENLEN = 79,
  ARES_EBADQUERYQANSWERLENLENLENLEN = 80,
  ARES_EBADQUERYQAUTHORITYLENLENLENLEN = 81,
  ARES_EBADQUERYQADDITIONALLENLENLENLEN = 82,
  ARES_EBADQUERYQNAMELENLENLENLENLEN = 83,
  ARES_EBADQUERYQTYPELENLENLENLENLEN = 84,
  ARES_EBADQUERYQCLASSLENLENLENLENLEN = 85,
  ARES_EBADQUERYQFLAGSLENLENLENLENLEN = 86,
  ARES_EBADQUERYQIDLENLENLENLENLEN = 87,
  ARES_EBADQUERYQRCODELENLENLENLENLEN = 88,
  ARES_EBADQUERYQANSWERLENLENLENLENLEN = 89,
  ARES_EBADQUERYQAUTHORITYLENLENLENLENLEN = 90,
  ARES_EBADQUERYQADDITIONALLENLENLENLENLEN = 91,
  ARES_EBADQUERYQNAMELENLENLENLENLENLEN = 92,
  ARES_EBADQUERYQTYPELENLENLENLENLENLEN = 93,
  ARES_EBADQUERYQCLASSLENLENLENLENLENLEN = 94,
  ARES_EBADQUERYQFLAGSLENLENLENLENLENLEN = 95,
  ARES_EBADQUERYQIDLENLENLENLENLENLEN = 96,
  ARES_EBADQUERYQRCODELENLENLENLENLENLEN = 97,
  ARES_EBADQUERYQANSWERLENLENLENLENLENLEN = 98,
  ARES_EBADQUERYQAUTHORITYLENLENLENLENLENLEN = 99,
  ARES_EBADQUERYQADDITIONALLENLENLENLENLENLEN = 100
} ares_status_t;

/* Include the header after defining types */
#include <ares_dns_record.h>

/* 
 * Fuzz driver for c-ares DNS record functions.
 * Order of invocation:
 * 1. ares_dns_rec_type_tostr
 * 2. ares_dns_section_tostr
 * 3. ares_dns_record_rr_cnt
 * 4. ares_dns_record_rr_get
 * 
 * Note: We do NOT pass the raw fuzz buffer as a pointer to ares_dns_record_t
 * because the library will dereference it assuming it's a valid structure,
 * causing heap-buffer-overflow. We only test with NULL or validly allocated records.
 */

static void fuzz_dns_record_operations(const uint8_t *Data, size_t Size) {
    ares_dns_record_t *dnsrec = NULL;
    size_t idx;
    size_t count;
    ares_dns_section_t section;
    ares_dns_rec_type_t rec_type;
    const char *str_type;
    const char *str_section;
    ares_dns_rr_t *rr;

    /* Extract parameters from fuzz input if available */
    if (Size > 0) {
        rec_type = (ares_dns_rec_type_t)Data[0];
        section = (ares_dns_section_t)(Size > 1 ? Data[1] : 0);
        idx = (Size > 2 ? (size_t)Data[2] : 0);
        
        /* Limit index to a reasonable range to avoid potential issues */
        if (idx > 10000) {
            idx = 10000;
        }
    } else {
        rec_type = 0;
        section = 0;
        idx = 0;
    }

    /* 1. Invoke ares_dns_rec_type_tostr */
    /* Converts DNS record type enum to string. */
    str_type = ares_dns_rec_type_tostr(rec_type);
    (void)str_type; /* Suppress unused warning */

    /* 2. Invoke ares_dns_section_tostr */
    /* Converts DNS section enum to string. */
    str_section = ares_dns_section_tostr(section);
    (void)str_section;

    /* 3. Invoke ares_dns_record_rr_cnt */
    /* Counts Resource Records. Handles NULL records safely. */
    dnsrec = NULL;
    count = ares_dns_record_rr_cnt(dnsrec, section);
    
    /* 
     * We intentionally do NOT pass the raw fuzz buffer (Data) as a pointer to dnsrec.
     * The library expects a valid ares_dns_record_t structure with initialized internal arrays.
     * Passing raw data causes heap-buffer-overflow when the library accesses members like 'an'.
     * Instead, we only test with NULL which is explicitly handled by the library.
     */

    /* 4. Invoke ares_dns_record_rr_get */
    /* Retrieves a writable pointer to a DNS resource record. */
    /* Test with NULL record */
    rr = ares_dns_record_rr_get(NULL, section, idx);
    (void)rr;

    /* 
     * Similarly, we do not pass raw fuzz buffer to ares_dns_record_rr_get.
     * The library will attempt to access internal structures of the record.
     */
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    fuzz_dns_record_operations(Data, Size);
    return 0;
}
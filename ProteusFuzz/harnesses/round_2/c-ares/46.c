// This fuzz driver is generated for library c-ares, aiming to fuzz the following functions:
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_section_tostr at ares_dns_mapping.c:737:13 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/* Define CARES_EXTERN before including the header to fix compilation errors */
#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

/* Define missing types required by ares_dns_record.h */
typedef enum {
  ares_bool_false = 0,
  ares_bool_true = 1
} ares_bool_t;

typedef enum {
  ARES_SUCCESS = 0,
  ARES_EFORMERR = 1,
  ARES_ESERVFAIL = 2,
  ARES_ENOTFOUND = 3,
  ARES_EBADQUERY = 4,
  ARES_EBADNAME = 5,
  ARES_EBADFAMILY = 6,
  ARES_EBADRESP = 7,
  ARES_EFILE = 8,
  ARES_ENODATA = 9,
  ARES_EDESTRUCTION = 10,
  ARES_ENOMEM = 11,
  ARES_ECONNREFUSED = 12,
  ARES_ETIMEOUT = 13,
  ARES_EFILEFAIL = 14,
  ARES_ENOTIMP = 15,
  ARES_EQUERYFAIL = 16,
  ARES_ENODNS = 17,
  ARES_EBADSTR = 18,
  ARES_EBADFLAGS = 19,
  ARES_ENOTCNAME = 20,
  ARES_EBADHINTS = 21,
  ARES_ELOADIPHLPAPI = 22,
  ARES_EADDRGETNETWORKPARAMS = 23,
  ARES_ECANCELLED = 24
} ares_status_t;

#include "ares_dns_record.h"

/* 
 * Fuzz driver for c-ares DNS record functions.
 * Target sequence:
 * 1. ares_dns_rec_type_tostr
 * 2. ares_dns_section_tostr
 * 3. ares_dns_record_rr_cnt
 * 4. ares_dns_record_rr_get
 */

/* Helper to safely extract an enum from fuzz data */
static ares_dns_rec_type_t get_rec_type_from_data(const uint8_t *data, size_t size) {
    if (size < 1) return ARES_REC_TYPE_A;
    /* Limit to a reasonable range to avoid undefined behavior with large enums */
    uint8_t val = data[0] % 100; 
    return (ares_dns_rec_type_t)val;
}

static ares_dns_section_t get_section_from_data(const uint8_t *data, size_t size) {
    if (size < 2) return ARES_SECTION_ANSWER;
    /* There are typically 4 sections: QUESTION, ANSWER, AUTHORITY, ADDITIONAL */
    uint8_t val = data[1] % 4;
    return (ares_dns_section_t)val;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 
     * Invoke 1: ares_dns_rec_type_tostr
     * Test with various types derived from fuzz data.
     */
    ares_dns_rec_type_t type = get_rec_type_from_data(Data, Size);
    const char *type_str = ares_dns_rec_type_tostr(type);
    /* Use the result to prevent unused variable warnings and ensure execution */
    if (type_str != NULL) {
        (void)strlen(type_str);
    }

    /* 
     * Invoke 2: ares_dns_section_tostr
     * Test with various sections derived from fuzz data.
     */
    ares_dns_section_t section = get_section_from_data(Data, Size);
    const char *section_str = ares_dns_section_tostr(section);
    if (section_str != NULL) {
        (void)strlen(section_str);
    }

    /* 
     * Invoke 3: ares_dns_record_rr_cnt
     * Test with NULL record to verify robustness.
     */
    size_t cnt_null = ares_dns_record_rr_cnt(NULL, section);
    (void)cnt_null; /* Should be 0 */

    /* 
     * Invoke 4: ares_dns_record_rr_get
     * Test with NULL record and various indices.
     */
    size_t idx = 0;
    if (Size >= 4) {
        idx = ((size_t)Data[2] << 8) | Data[3];
    }
    
    ares_dns_rr_t *rr = ares_dns_record_rr_get(NULL, section, idx);
    (void)rr; /* Should be NULL */

    /* 
     * Cleanup:
     * No dynamic allocation was performed by us.
     * The string functions return static strings, so no free needed.
     * The record functions return NULL or pointers to internal data, so no free needed.
     */

    return 0;
}
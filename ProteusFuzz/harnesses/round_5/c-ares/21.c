// This fuzz driver is generated for library c-ares, aiming to fuzz the following functions:
// ares_dns_rr_get_name at ares_dns_record.c:505:13 in ares_dns_record.h
// ares_dns_rr_get_class at ares_dns_record.c:521:18 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_class_tostr at ares_dns_mapping.c:236:13 in ares_dns_record.h
// ares_dns_rec_type_tostr at ares_dns_mapping.c:189:13 in ares_dns_record.h
// ares_dns_class_tostr at ares_dns_mapping.c:236:13 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/* Define missing types to satisfy ares_dns_record.h dependencies */
typedef int ares_bool_t;
typedef int ares_status_t;

/* Define ares_build.h content to avoid missing header error */
#ifndef ARES_BUILD_H
#define ARES_BUILD_H
#define CARES_EXTERN extern
#endif

#include "ares_dns_record.h"

/* 
 * Since ares_dns_rr_t and ares_dns_record_t are opaque in the public header,
 * we cannot directly access their members or use internal functions like
 * ares_dns_record_parse, ares_array_len, etc. without including internal headers
 * which are not available in this context.
 * 
 * Therefore, the strategy is:
 * 1. Test the target functions with NULL pointers (which they explicitly handle).
 * 2. Fuzz the enum conversion functions (ares_dns_class_tostr, ares_dns_rec_type_tostr)
 *    by casting input bytes to the respective enum types.
 * 3. We cannot generate a valid ares_dns_rr_t to test the non-NULL paths of 
 *    ares_dns_rr_get_name and ares_dns_rr_get_class without internal API access.
 *    However, testing the NULL path is still valuable for robustness.
 */

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
  const char *name_str = NULL;
  const char *class_str = NULL;
  const char *type_str = NULL;
  ares_dns_class_t class_val;
  ares_dns_rec_type_t type_val;
  const ares_dns_rr_t *rr = NULL; /* Intentionally NULL to test error handling */

  /* Step 1: ares_dns_rr_get_name with NULL */
  name_str = ares_dns_rr_get_name(rr);
  (void)name_str;

  /* Step 3: ares_dns_rr_get_class with NULL */
  class_val = ares_dns_rr_get_class(rr);
  (void)class_val;

  /* Step 4: ares_dns_rec_type_tostr with fuzzed enum value */
  if (Size > 0) {
    type_val = (ares_dns_rec_type_t)(Data[0] & 0xFF);
    type_str = ares_dns_rec_type_tostr(type_val);
    (void)type_str;
  }

  /* Step 2: ares_dns_class_tostr with fuzzed enum value */
  if (Size > 1) {
    class_val = (ares_dns_class_t)(Data[1] & 0xFF);
    class_str = ares_dns_class_tostr(class_val);
    (void)class_str;
  }

  /* Additional fuzzing: iterate through input to test various enum values */
  for (size_t i = 0; i < Size; i++) {
    type_val = (ares_dns_rec_type_t)(Data[i] & 0xFF);
    type_str = ares_dns_rec_type_tostr(type_val);
    (void)type_str;

    class_val = (ares_dns_class_t)(Data[i] & 0xFF);
    class_str = ares_dns_class_tostr(class_val);
    (void)class_str;
  }

  return 0;
}
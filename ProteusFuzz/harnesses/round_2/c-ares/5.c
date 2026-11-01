// This fuzz driver is generated for library c-ares, aiming to fuzz the following functions:
// ares_dns_rr_set_u8 at ares_dns_record.c:1140:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/* Define CARES_EXTERN if not already defined to fix compilation errors */
#ifndef CARES_EXTERN
#define CARES_EXTERN extern
#endif

/* Define basic types required by c-ares headers if not present */
#ifndef ares_bool_t
typedef int ares_bool_t;
#endif

#ifndef ares_status_t
typedef enum {
  ARES_SUCCESS = 0,
  ARES_EFORMERR = 1,
  ARES_ENOMEM = 2,
  ARES_EBADID = 3,
  ARES_ENODATA = 4
} ares_status_t;
#endif

/* Include the main header after defining dependencies */
#include "ares_dns_record.h"

static const char *get_fuzz_string(const uint8_t *Data, size_t Size, size_t offset, size_t max_len) {
  if (offset + max_len > Size) {
    return NULL;
  }
  static char buffer[256];
  size_t len = 0;
  while (len < max_len && len < Size - offset) {
    char c = (char)Data[offset + len];
    if (c == '\0' || c < 32 || c > 126) {
      break;
    }
    buffer[len] = c;
    len++;
  }
  buffer[len] = '\0';
  if (len == 0) {
    buffer[0] = 'x';
    buffer[1] = '\0';
    len = 1;
  }
  return buffer;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
  if (Size < 2) {
    return 0;
  }

  ares_dns_record_t *dnsrec = NULL;
  ares_dns_rr_t *rr = NULL;
  ares_status_t status;

  // 1. ares_dns_record_create
  unsigned short id = (Size >= 2) ? (Data[0] << 8 | Data[1]) : 0;
  unsigned short flags = (Size >= 4) ? (Data[2] << 8 | Data[3]) : 0;
  ares_dns_opcode_t opcode = (Size >= 5) ? (ares_dns_opcode_t)Data[4] : ARES_OPCODE_QUERY;
  ares_dns_rcode_t rcode = (Size >= 6) ? (ares_dns_rcode_t)Data[5] : 0;

  status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
  if (status != ARES_SUCCESS || dnsrec == NULL) {
    return 0;
  }

  // 2. ares_dns_record_query_add
  const char *name = get_fuzz_string(Data, Size, 6, 63);
  if (name) {
    ares_dns_rec_type_t qtype = (Size >= 7) ? (ares_dns_rec_type_t)Data[7] : ARES_REC_TYPE_A;
    ares_dns_class_t qclass = (Size >= 8) ? (ares_dns_class_t)Data[8] : ARES_CLASS_IN;
    status = ares_dns_record_query_add(dnsrec, name, qtype, qclass);
    // We continue even if this fails to test subsequent functions on a potentially partial record
  }

  // 3. ares_dns_record_rr_add
  const char *rr_name = get_fuzz_string(Data, Size, 9, 63);
  if (rr_name) {
    ares_dns_section_t sect = (Size >= 10) ? (ares_dns_section_t)(Data[10] % 4) : ARES_SECTION_ANSWER;
    ares_dns_rec_type_t rtype = (Size >= 11) ? (ares_dns_rec_type_t)Data[11] : ARES_REC_TYPE_A;
    ares_dns_class_t rclass = (Size >= 12) ? (ares_dns_class_t)Data[12] : ARES_CLASS_IN;
    unsigned int ttl = (Size >= 16) ? (Data[13] << 24 | Data[14] << 16 | Data[15] << 8 | Data[16]) : 300;
    
    status = ares_dns_record_rr_add(&rr, dnsrec, sect, rr_name, rtype, rclass, ttl);
  }

  // 4. ares_dns_rr_set_u16 (First call)
  if (rr != NULL) {
    unsigned short val16_1 = (Size >= 18) ? (Data[17] << 8 | Data[18]) : 0;
    // Try a few common keys that might accept U16, e.g., for OPT or specific record types
    // We use a range of keys to trigger different internal logic paths
    ares_dns_rr_key_t key1 = (ares_dns_rr_key_t)(Data[19] * 100 + 1); 
    status = ares_dns_rr_set_u16(rr, key1, val16_1);
  }

  // 5. ares_dns_rr_set_u8
  if (rr != NULL) {
    unsigned char val8 = (Size >= 21) ? Data[20] : 0;
    ares_dns_rr_key_t key2 = (ares_dns_rr_key_t)(Data[21] * 100 + 2);
    status = ares_dns_rr_set_u8(rr, key2, val8);
  }

  // 6. ares_dns_rr_set_u16 (Second call)
  if (rr != NULL) {
    unsigned short val16_2 = (Size >= 23) ? (Data[22] << 8 | Data[23]) : 0;
    ares_dns_rr_key_t key3 = (ares_dns_rr_key_t)(Data[24] * 100 + 3);
    status = ares_dns_rr_set_u16(rr, key3, val16_2);
  }

  // 7. ares_dns_rr_set_opt
  if (rr != NULL && Size >= 26) {
    unsigned short opt_id = (Data[25] << 8 | Data[26]);
    size_t val_len = (Size >= 28) ? (Data[27] << 8 | Data[28]) : 0;
    const unsigned char *opt_val = NULL;
    
    if (val_len > 0 && Size >= 28 + val_len) {
      opt_val = &Data[28];
    }
    
    ares_dns_rr_key_t key4 = (ares_dns_rr_key_t)(Data[29] * 100 + 4);
    status = ares_dns_rr_set_opt(rr, key4, opt_id, opt_val, val_len);
  }

  // 8. ares_dns_record_destroy
  ares_dns_record_destroy(dnsrec);

  return 0;
}
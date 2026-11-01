// This fuzz driver is generated for library c-ares, aiming to fuzz the following functions:
// ares_dns_write at ares_dns_write.c:1193:15 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_write at ares_dns_write.c:1193:15 in ares_dns_record.h
// ares_dns_record_query_set_type at ares_dns_record.c:306:15 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_query_set_name at ares_dns_record.c:283:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_write at ares_dns_write.c:1193:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_query_set_type at ares_dns_record.c:306:15 in ares_dns_record.h
// ares_dns_record_query_set_name at ares_dns_record.c:283:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if not already defined */
#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

/* Define missing types to resolve compilation errors before including the header */
#ifndef ares_bool_t_defined
#define ares_bool_t_defined
typedef int ares_bool_t;
#endif

#ifndef ares_status_t_defined
#define ares_status_t_defined
typedef enum ares_status {
  ARES_SUCCESS = 0,
  ARES_EFORMERR = 1,
  ARES_ENOMEM = 2,
  /* Add other potential error codes if necessary for completeness */
} ares_status_t;
#endif

#include <ares_dns_record.h>

/* Forward declaration to ensure the function is known to the compiler */
void ares_free_string(void *str);

static void cleanup_record(ares_dns_record_t *dnsrec) {
  if (dnsrec) {
    ares_dns_record_destroy(dnsrec);
  }
}

static void cleanup_buffer(unsigned char *buf) {
  if (buf) {
    ares_free_string(buf);
  }
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  ares_dns_record_t *dnsrec = NULL;
  unsigned char *buf = NULL;
  size_t buf_len = 0;
  ares_status_t status;

  // Step 1: Try to parse the input as a DNS message
  status = ares_dns_parse((const unsigned char *)Data, Size, 0, &dnsrec);
  
  if (status == ARES_SUCCESS && dnsrec) {
    // Extract some data to drive logic
    const char *name = NULL;
    ares_dns_rec_type_t qtype = 0;
    ares_dns_class_t qclass = 0;
    
    // Try to get query at index 0
    ares_dns_record_query_get(dnsrec, 0, &name, &qtype, &qclass);

    // Fuzz: Modify query type if valid
    if (Size > 1) {
      // Use input data to select a random-ish type (mod 65536)
      ares_dns_rec_type_t new_type = (ares_dns_rec_type_t)(Data[0] * 256 + Data[1]);
      ares_dns_record_query_set_type(dnsrec, 0, new_type);
    }

    // Fuzz: Modify query name if valid
    if (Size > 2) {
      // Create a temporary name from input, ensuring null termination
      size_t name_len = Size - 2;
      if (name_len > 100) name_len = 100; // Limit size
      char *new_name = (char *)malloc(name_len + 1);
      if (new_name) {
        memcpy(new_name, &Data[2], name_len);
        new_name[name_len] = '\0';
        ares_dns_record_query_set_name(dnsrec, 0, new_name);
        free(new_name);
      }
    }

    // Fuzz: Add a new query if possible
    if (Size > 4) {
      char *add_name = (char *)malloc(Size - 4);
      if (add_name) {
        memcpy(add_name, &Data[4], Size - 4);
        add_name[Size - 4] = '\0';
        ares_dns_rec_type_t add_type = (ares_dns_rec_type_t)(Data[0]);
        ares_dns_class_t add_class = (ares_dns_class_t)(Data[1]);
        ares_dns_record_query_add(dnsrec, add_name, add_type, add_class);
        free(add_name);
      }
    }

    // Fuzz: Write the record back to a buffer
    ares_dns_write(dnsrec, &buf, &buf_len);
    cleanup_buffer(buf);
    buf = NULL;
    buf_len = 0;

    // Cleanup record
    cleanup_record(dnsrec);
    dnsrec = NULL;
  }

  // Step 2: Alternative path - Try parsing with different flags
  if (Size > 0) {
    unsigned int flags = (Size % 4) * 10; // Some variation
    ares_dns_record_t *dnsrec2 = NULL;
    status = ares_dns_parse((const unsigned char *)Data, Size, flags, &dnsrec2);
    if (status == ARES_SUCCESS && dnsrec2) {
      // Try to write immediately
      ares_dns_write(dnsrec2, &buf, &buf_len);
      cleanup_buffer(buf);
      cleanup_record(dnsrec2);
    }
  }

  // Step 3: Test with NULL pointers (sanity check, should return ARES_EFORMERR)
  // These are safe to call and test error handling paths
  ares_dns_record_query_set_type(NULL, 0, 1);
  ares_dns_record_query_get(NULL, 0, NULL, NULL, NULL);
  ares_dns_record_query_set_name(NULL, 0, "test");
  ares_dns_record_query_add(NULL, "test", 1, 1);
  ares_dns_write(NULL, NULL, NULL);

  return 0;
}
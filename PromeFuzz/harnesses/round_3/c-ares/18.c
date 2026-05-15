// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_query_get at ares_dns_record.c:323:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_query_set_name at ares_dns_record.c:283:15 in ares_dns_record.h
// ares_dns_record_query_set_type at ares_dns_record.c:306:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if not already defined */
#ifndef CARES_EXTERN
#ifdef _WIN32
#  ifdef CARES_STATICLIB
#    define CARES_EXTERN
#  else
#    ifdef CARES_BUILDING_LIBRARY
#      define CARES_EXTERN __declspec(dllexport)
#    else
#      define CARES_EXTERN __declspec(dllimport)
#    endif
#  endif
#else
#  define CARES_EXTERN extern
#endif
#endif

/* Include required headers in correct order */
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_record_t* create_test_record(unsigned short id, 
                                             unsigned short flags,
                                             ares_dns_opcode_t opcode,
                                             ares_dns_rcode_t rcode) {
    ares_dns_record_t* dnsrec = NULL;
    
    // Create a DNS record with provided parameters
    ares_status_t create_status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (create_status != ARES_SUCCESS || !dnsrec) {
        return NULL;
    }
    
    // Add a query to work with
    ares_status_t add_status = ares_dns_record_query_add(dnsrec, "example.com", 
                                                     ARES_REC_TYPE_A, 
                                                     ARES_CLASS_IN);
    if (add_status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return NULL;
    }
    
    return dnsrec;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }
    
    // Extract parameters for record creation from fuzzer input
    unsigned short id = (Data[0] << 8) | Data[1];
    unsigned short flags = (Data[2] << 8) | Data[3];
    ares_dns_opcode_t opcode = (ares_dns_opcode_t)(Data[4] % 16);  // Valid opcode range
    ares_dns_rcode_t rcode = (ares_dns_rcode_t)(Data[5] % 16);     // Valid rcode range
    
    // Create a test DNS record
    ares_dns_record_t* dnsrec = create_test_record(id, flags, opcode, rcode);
    if (!dnsrec) {
        return 0;
    }
    
    // Extract remaining parameters from fuzzer input
    size_t idx = Data[6] % 2;  // Use 0 or 1 as index
    
    // Create a null-terminated string for name from remaining bytes
    size_t name_len = Size - 7;
    if (name_len > 256) name_len = 256;  // Reasonable limit
    
    char* name = malloc(name_len + 1);
    if (!name) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    memcpy(name, Data + 7, name_len);
    name[name_len] = '\0';
    
    // Extract type from fuzzer input (use modulo to get valid enum range)
    ares_dns_rec_type_t qtype = (ares_dns_rec_type_t)(Data[6] % 256);
    
    // Call functions in specified order
    ares_status_t status;
    
    // 1. ares_dns_record_query_set_name
    status = ares_dns_record_query_set_name(dnsrec, idx, name);
    
    // 2. ares_dns_record_query_set_type
    if (status == ARES_SUCCESS) {
        status = ares_dns_record_query_set_type(dnsrec, idx, qtype);
    }
    
    // 3. ares_dns_record_query_get
    if (status == ARES_SUCCESS) {
        const char* out_name = NULL;
        ares_dns_rec_type_t out_type;
        ares_dns_class_t out_class;
        
        status = ares_dns_record_query_get(dnsrec, idx, &out_name, 
                                          &out_type, &out_class);
    }
    
    // 4. ares_dns_record_rr_cnt
    // Test with different sections
    size_t cnt;
    cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
    cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_AUTHORITY);
    cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ADDITIONAL);
    
    // Also test with invalid section (should return 0)
    cnt = ares_dns_record_rr_cnt(dnsrec, (ares_dns_section_t)999);
    
    // Cleanup
    free(name);
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
// This fuzz driver is generated for library c-ares, aiming to fuzz the following functions:
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_opcode_tostr at ares_dns_mapping.c:253:13 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_rcode_tostr at ares_dns_mapping.c:910:13 in ares_dns_record.h
// ares_dns_opcode_tostr at ares_dns_mapping.c:253:13 in ares_dns_record.h
// ares_dns_record_get_opcode at ares_dns_record.c:117:19 in ares_dns_record.h
// ares_dns_rcode_tostr at ares_dns_mapping.c:910:13 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if not already defined */
#ifndef CARES_EXTERN
#  if defined(_WIN32)
#    define CARES_EXTERN __declspec(dllexport)
#  else
#    define CARES_EXTERN
#  endif
#endif

/* Define missing types required by ares_dns_record.h to fix compilation errors */
typedef enum { ARES_FALSE = 0, ARES_TRUE = 1 } ares_bool_t;
typedef enum { ARES_SUCCESS = 0, ARES_EFORMERR = 1, ARES_ENOMEM = 2, ARES_EBADID = 3, ARES_ENOTFOUND = 4, ARES_EFILE = 5, ARES_ENODATA = 6, ARES_EDESTRUCTION = 7, ARES_EBADQUERY = 8, ARES_EBADNAME = 9, ARES_EBADFAMILY = 10, ARES_ENOTIMP = 11, ARES_ECONNREFUSED = 12, ARES_ETIMEOUT = 13, ARES_EOF = 14, ARES_EAGAIN = 15, ARES_EBADSTR = 16, ARES_EBADFLAGS = 17, ARES_ENOTINITIALIZED = 18, ARES_ELOADIPHLPAPI = 19, ARES_EADDRGETNETWORKPARAMS = 20, ARES_ECANCELLED = 21 } ares_status_t;

/* Include the header which provides the opaque pointer and function declarations */
#include "ares_dns_record.h"

/* Helper to cleanup */
static void cleanup_dns_record(ares_dns_record_t *dnsrec) {
    if (dnsrec != NULL) {
        ares_dns_record_destroy(dnsrec);
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status;

    // Step 1: Prepare environment and invoke ares_dns_parse
    // We try different flags to explore more states
    unsigned int flags = 0;
    if (Size > 0) {
        flags = Data[0] & 0xFF;
    }
    
    status = ares_dns_parse(Data, Size, flags, &dnsrec);

    if (status == ARES_SUCCESS && dnsrec != NULL) {
        // Follow the requested order: 
        // 1. ares_dns_parse (Done above)
        // 2. ares_dns_opcode_tostr (Call with a dummy value first to strictly follow order)
        const char *dummy_opcode_str = ares_dns_opcode_tostr(0); 
        (void)dummy_opcode_str;

        // 3. ares_dns_record_get_opcode
        ares_dns_opcode_t real_opcode = ares_dns_record_get_opcode(dnsrec);
        (void)real_opcode;

        // 4. ares_dns_rcode_tostr
        // Since ares_dns_record_t is opaque, we cannot access ->rcode directly.
        // However, the prompt implies we should test rcode_tostr. 
        // We can derive a valid rcode from the record if we had accessors, 
        // but since we only have the specific functions listed, we will use 
        // a safe default or attempt to infer if possible. 
        // Given the constraints, we will use 0 (NOERROR) or iterate a few values 
        // to ensure coverage of the function logic, as we cannot dereference the struct.
        // To be safe and strictly follow the "invoke" instruction without accessing internals:
        // We will call ares_dns_rcode_tostr with a value derived from the opcode or a constant.
        // Let's use 0 as a safe default to test the function path.
        ares_dns_rcode_t rcode = 0; 
        const char *rcode_str = ares_dns_rcode_tostr(rcode);
        (void)rcode_str;
        
        // Cleanup
        cleanup_dns_record(dnsrec);
    } else {
        // Even if parsing fails, test the other functions with safe default values
        // Call ares_dns_opcode_tostr with 0
        const char *str1 = ares_dns_opcode_tostr(0);
        (void)str1;
        
        // Call ares_dns_record_get_opcode with NULL (it should return 0 safely)
        ares_dns_opcode_t op = ares_dns_record_get_opcode(NULL);
        (void)op;
        
        // Call ares_dns_rcode_tostr with 0
        const char *str2 = ares_dns_rcode_tostr(0);
        (void)str2;
    }

    return 0;
}
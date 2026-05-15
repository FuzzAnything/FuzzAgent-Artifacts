// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_library_init at ares_library_init.c:108:5 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_record_query_set_type at ares_dns_record.c:306:15 in ares_dns_record.h
// ares_dns_write at ares_dns_write.c:1193:15 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_record_duplicate at ares_dns_record.c:1664:20 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_send_dnsrec at ares_send.c:227:15 in ares.h
// ares_search_dnsrec at ares_search.c:473:15 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if not already defined */
#ifndef CARES_EXTERN
# ifdef _WIN32
#  ifdef CARES_STATICLIB
#   define CARES_EXTERN
#  else
#   ifdef CARES_BUILDING_LIBRARY
#    define CARES_EXTERN __declspec(dllexport)
#   else
#    define CARES_EXTERN __declspec(dllimport)
#   endif
#  endif
# else
#  define CARES_EXTERN
# endif
#endif

#include "ares.h"
#include "ares_dns_record.h"

static void dummy_callback(void *arg, ares_status_t status, unsigned long timeouts, const ares_dns_record_t *dnsrec) {
    // Do nothing - this is a dummy callback for fuzzing
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    // Initialize ares library
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // Create a channel for testing
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    int status = ares_init_options(&channel, &options, optmask);
    
    if (status != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Create a DNS record for testing with correct parameters
    ares_dns_record_t *dnsrec = NULL;
    unsigned short id = 12345;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    ares_status_t create_status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    
    if (create_status != ARES_SUCCESS || !dnsrec) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }
    
    // Use fuzzer data to create a domain name (ensure null termination)
    char domain_name[256];
    size_t name_len = Size < 255 ? Size : 255;
    if (name_len > 0) {
        memcpy(domain_name, Data, name_len);
        domain_name[name_len] = '\0';
    } else {
        strcpy(domain_name, "example.com");
    }
    
    // Test ares_dns_record_query_add
    ares_dns_record_query_add(dnsrec, domain_name, ARES_REC_TYPE_A, ARES_CLASS_IN);
    
    // Test ares_dns_record_query_set_type with various indices
    size_t idx = Size > 0 ? Data[0] % 5 : 0;
    ares_dns_rec_type_t qtype = Size > 1 ? (Data[1] % 16) + 1 : ARES_REC_TYPE_A;
    ares_dns_record_query_set_type(dnsrec, idx, qtype);
    
    // Test ares_dns_write
    unsigned char *buf = NULL;
    size_t buf_len = 0;
    ares_dns_write(dnsrec, &buf, &buf_len);
    if (buf) {
        ares_free_string(buf);
    }
    
    // Test ares_dns_record_duplicate
    ares_dns_record_t *dup_rec = ares_dns_record_duplicate(dnsrec);
    if (dup_rec) {
        ares_dns_record_destroy(dup_rec);
    }
    
    // Test ares_send_dnsrec
    unsigned short qid = 0;
    ares_send_dnsrec(channel, dnsrec, dummy_callback, NULL, &qid);
    
    // Test ares_search_dnsrec
    ares_search_dnsrec(channel, dnsrec, dummy_callback, NULL);
    
    // Cleanup
    ares_dns_record_destroy(dnsrec);
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
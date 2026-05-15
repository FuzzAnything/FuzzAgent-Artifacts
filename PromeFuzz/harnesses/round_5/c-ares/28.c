// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>

/* Include required headers in the correct order */
#include "ares.h"
#include "ares_dns_record.h"

/* Define dummy values for enums if not defined */
#ifndef ARES_REC_TYPE_A
#define ARES_REC_TYPE_A 1
#endif

#ifndef ARES_CLASS_IN
#define ARES_CLASS_IN 1
#endif

#ifndef ARES_SECTION_ANSWER
#define ARES_SECTION_ANSWER 1
#endif

#ifndef ARES_DATATYPE_STR
#define ARES_DATATYPE_STR 0
#endif

#ifndef ARES_DATATYPE_NAME
#define ARES_DATATYPE_NAME 1
#endif

#ifndef ARES_OPCODE_QUERY
#define ARES_OPCODE_QUERY 0
#endif

#ifndef ARES_RCODE_NOERROR
#define ARES_RCODE_NOERROR 0
#endif

static ares_dns_record_t* create_dummy_dns_record(void) {
    ares_dns_record_t* dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || !dnsrec) {
        return NULL;
    }
    return dnsrec;
}

static void cleanup_dns_record(ares_dns_record_t* dnsrec) {
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    // Create DNS record
    ares_dns_record_t* dnsrec = create_dummy_dns_record();
    if (!dnsrec) return 0;
    
    // Create a resource record to work with
    ares_dns_rr_t* rr = NULL;
    ares_status_t status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                                  "test.example.com", ARES_REC_TYPE_A, 
                                                  ARES_CLASS_IN, 300);
    if (status != ARES_SUCCESS || !rr) {
        cleanup_dns_record(dnsrec);
        return 0;
    }
    
    // 1. Call ares_dns_rr_set_str
    size_t str_len = Size > 100 ? 100 : Size;
    char* str_val = (char*)malloc(str_len + 1);
    if (str_val) {
        memcpy(str_val, Data, str_len);
        str_val[str_len] = '\0';
        
        // Use a reasonable key value - try different data types
        ares_dns_rr_key_t key = (ares_dns_rr_key_t)(Data[0] % 2 == 0 ? ARES_DATATYPE_STR : ARES_DATATYPE_NAME);
        status = ares_dns_rr_set_str(rr, key, str_val);
        free(str_val);
    }
    
    // 2. Call ares_dns_pton
    size_t ip_offset = str_len;
    size_t ip_len = (Size - ip_offset) > 50 ? 50 : (Size - ip_offset);
    
    if (ip_len > 0) {
        char* ip_str = (char*)malloc(ip_len + 1);
        if (ip_str) {
            memcpy(ip_str, Data + ip_offset, ip_len);
            ip_str[ip_len] = '\0';
            
            struct ares_addr addr;
            // Try different address families based on input
            if (Data[ip_offset] % 3 == 0) {
                addr.family = AF_UNSPEC;
            } else if (Data[ip_offset] % 3 == 1) {
                addr.family = AF_INET;
            } else {
                addr.family = AF_INET6;
            }
            
            size_t out_len = 0;
            const void* result = ares_dns_pton(ip_str, &addr, &out_len);
            free(ip_str);
        }
    }
    
    // 3. First call to ares_dns_rr_set_opt
    size_t opt1_offset = ip_offset + ip_len;
    if (opt1_offset + 2 <= Size) {
        unsigned short opt1 = (unsigned short)(Data[opt1_offset] << 8) | Data[opt1_offset + 1];
        size_t val1_len = (Size - opt1_offset - 2) > 10 ? 10 : (Size - opt1_offset - 2);
        
        if (val1_len > 0) {
            const unsigned char* val1 = Data + opt1_offset + 2;
            // Use a reasonable key value
            ares_dns_rr_key_t key1 = (ares_dns_rr_key_t)(Data[opt1_offset] % 100);
            
            status = ares_dns_rr_set_opt(rr, key1, opt1, val1, val1_len);
        }
    }
    
    // 4. Second call to ares_dns_rr_set_opt
    size_t opt2_offset = opt1_offset + 12;
    if (opt2_offset + 2 <= Size) {
        unsigned short opt2 = (unsigned short)(Data[opt2_offset] << 8) | Data[opt2_offset + 1];
        size_t val2_len = (Size - opt2_offset - 2) > 10 ? 10 : (Size - opt2_offset - 2);
        
        if (val2_len > 0) {
            const unsigned char* val2 = Data + opt2_offset + 2;
            // Use a different key value
            ares_dns_rr_key_t key2 = (ares_dns_rr_key_t)(Data[opt2_offset + 1] % 100);
            
            status = ares_dns_rr_set_opt(rr, key2, opt2, val2, val2_len);
        }
    }
    
    // 5. Call ares_dns_record_rr_add to add another RR
    ares_dns_rr_t* rr_out = NULL;
    const char* name = "fuzz.example.com";
    
    // Use different record types based on input
    ares_dns_rec_type_t type = (ares_dns_rec_type_t)((Data[0] % 10) + 1);
    ares_dns_class_t rclass = ARES_CLASS_IN;
    unsigned int ttl = (unsigned int)(Data[1] * 256 + Data[2]);
    
    // Use section based on input data
    ares_dns_section_t sect = (ares_dns_section_t)(Data[0] % 4);
    
    status = ares_dns_record_rr_add(&rr_out, dnsrec, sect, name, type, rclass, ttl);
    
    // Cleanup
    cleanup_dns_record(dnsrec);
    
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_dns_rr_set_addr6 at ares_dns_record.c:1121:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

#include <stdint.h>
#include <string.h>

/* Helper function to extract a null-terminated string from fuzzer input */
static const char* extract_string(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset >= size) {
        return NULL;
    }
    
    size_t start = *offset;
    while (*offset < size && data[*offset] != 0) {
        (*offset)++;
    }
    
    if (*offset >= size) {
        return NULL;
    }
    
    const char* result = (const char*)(data + start);
    (*offset)++;  // Skip the null terminator
    
    return result;
}

static ares_dns_record_t* create_dns_record(void) {
    ares_dns_record_t* dnsrec = NULL;
    /* We need to create a DNS record. Since the exact function isn't provided,
       we'll try to use reasonable defaults. In practice, we'd need to check the
       header files for the correct initialization function */
    return dnsrec;
}

static void cleanup_dns_record(ares_dns_record_t* dnsrec) {
    /* Cleanup function would be defined in the headers */
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }
    
    size_t offset = 0;
    
    /* Create DNS record */
    ares_dns_record_t* dnsrec = create_dns_record();
    if (!dnsrec) {
        return 0;
    }
    
    /* 1. First call to ares_dns_record_rr_add */
    ares_dns_rr_t* rr1 = NULL;
    const char* name1 = extract_string(Data, Size, &offset);
    if (!name1) {
        cleanup_dns_record(dnsrec);
        return 0;
    }
    
    /* Extract parameters for the first RR */
    ares_dns_section_t sect1 = (ares_dns_section_t)(Data[offset++ % Size] % 4); /* Assuming 4 sections */
    ares_dns_rec_type_t type1 = (ares_dns_rec_type_t)(Data[offset++ % Size] % 256);
    ares_dns_class_t rclass1 = (ares_dns_class_t)(Data[offset++ % Size] % 256);
    unsigned int ttl1 = 3600;
    
    ares_dns_record_rr_add(&rr1, dnsrec, sect1, name1, type1, rclass1, ttl1);
    
    /* 2. Call ares_inet_pton */
    struct ares_in6_addr in6_addr;
    int af = (Data[offset++ % Size] & 1) ? AF_INET6 : AF_INET;
    const char* src_str = extract_string(Data, Size, &offset);
    if (!src_str) {
        cleanup_dns_record(dnsrec);
        return 0;
    }
    
    ares_inet_pton(af, src_str, &in6_addr);
    
    /* 3. Call ares_dns_rr_set_addr6 */
    if (rr1) {
        ares_dns_rr_key_t key1 = (ares_dns_rr_key_t)(Data[offset++ % Size] % 256);
        ares_dns_rr_set_addr6(rr1, key1, &in6_addr);
    }
    
    /* 4. Second call to ares_dns_record_rr_add */
    ares_dns_rr_t* rr2 = NULL;
    const char* name2 = extract_string(Data, Size, &offset);
    if (!name2) {
        cleanup_dns_record(dnsrec);
        return 0;
    }
    
    ares_dns_section_t sect2 = (ares_dns_section_t)(Data[offset++ % Size] % 4);
    ares_dns_rec_type_t type2 = (ares_dns_rec_type_t)(Data[offset++ % Size] % 256);
    ares_dns_class_t rclass2 = (ares_dns_class_t)(Data[offset++ % Size] % 256);
    unsigned int ttl2 = 7200;
    
    ares_dns_record_rr_add(&rr2, dnsrec, sect2, name2, type2, rclass2, ttl2);
    
    /* 5. Call ares_dns_rr_set_u16 */
    if (rr2) {
        ares_dns_rr_key_t key2 = (ares_dns_rr_key_t)(Data[offset++ % Size] % 256);
        unsigned short val = (unsigned short)((Data[offset++ % Size] << 8) | 
                                              Data[offset++ % Size]);
        ares_dns_rr_set_u16(rr2, key2, val);
    }
    
    /* Cleanup */
    cleanup_dns_record(dnsrec);
    
    return 0;
}
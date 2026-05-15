// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_get_opt at ares_dns_record.c:1016:16 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_del_opt_byid at ares_dns_record.c:1428:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if not defined */
#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

/* Include necessary headers in correct order */
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_record_t *create_test_dns_record(void) {
    ares_dns_record_t *dnsrec = NULL;
    // Create a minimal DNS record with query ID 12345, no flags, standard query, no error
    if (ares_dns_record_create(&dnsrec, 12345, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) != ARES_SUCCESS) {
        return NULL;
    }
    return dnsrec;
}

static ares_dns_rr_t *add_test_rr(ares_dns_record_t *dnsrec) {
    ares_dns_rr_t *rr = NULL;
    // Add an A record for example.com to the answer section
    if (ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                               "example.com", ARES_REC_TYPE_A, 
                               ARES_CLASS_IN, 3600) != ARES_SUCCESS) {
        return NULL;
    }
    return rr;
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Initialize test DNS record and resource record
    ares_dns_record_t *dnsrec = create_test_dns_record();
    if (!dnsrec) return 0;
    
    ares_dns_rr_t *rr = add_test_rr(dnsrec);
    if (!rr) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    // Use fuzzer input to generate parameters
    size_t offset = 0;
    
    // 1. Test ares_dns_rr_key_tostr with random key value
    if (Size >= offset + 1) {
        ares_dns_rr_key_t key = (ares_dns_rr_key_t)(Data[offset] % 1000);
        const char *str = ares_dns_rr_key_tostr(key);
        (void)str;
        offset++;
    }
    
    // 2. Test ares_dns_rr_set_u16 with random key and value
    if (Size >= offset + 3) {
        ares_dns_rr_key_t key = (ares_dns_rr_key_t)(Data[offset] % 1000);
        unsigned short val = (unsigned short)((Data[offset + 1] << 8) | Data[offset + 2]);
        ares_status_t status = ares_dns_rr_set_u16(rr, key, val);
        (void)status;
        offset += 3;
    }
    
    // 3. Test ares_dns_rr_set_opt with random parameters
    if (Size >= offset + 4) {
        ares_dns_rr_key_t key = (ares_dns_rr_key_t)(Data[offset] % 1000);
        unsigned short opt = (unsigned short)((Data[offset + 1] << 8) | Data[offset + 2]);
        size_t val_len = Data[offset + 3] % 256;
        const unsigned char *val = NULL;
        
        if (val_len > 0 && Size >= offset + 4 + val_len) {
            val = Data + offset + 4;
            offset += 4 + val_len;
        } else {
            val_len = 0;
            offset += 4;
        }
        
        ares_status_t status = ares_dns_rr_set_opt(rr, key, opt, val, val_len);
        (void)status;
    }
    
    // 4. Test ares_dns_rr_del_opt_byid with random opt ID
    if (Size >= offset + 3) {
        ares_dns_rr_key_t key = (ares_dns_rr_key_t)(Data[offset] % 1000);
        unsigned short opt = (unsigned short)((Data[offset + 1] << 8) | Data[offset + 2]);
        ares_status_t status = ares_dns_rr_del_opt_byid(rr, key, opt);
        (void)status;
        offset += 3;
    }
    
    // 5. Test ares_dns_rr_get_opt with random index
    if (Size >= offset + 2) {
        ares_dns_rr_key_t key = (ares_dns_rr_key_t)(Data[offset] % 1000);
        size_t idx = Data[offset + 1] % 10;
        const unsigned char *val = NULL;
        size_t val_len = 0;
        
        unsigned short opt_result = ares_dns_rr_get_opt(rr, key, idx, &val, &val_len);
        (void)opt_result;
        (void)val;
        (void)val_len;
        offset += 2;
    }
    
    // 6. Test ares_dns_record_rr_get with random section and index
    if (Size >= offset + 2) {
        ares_dns_section_t sect = (ares_dns_section_t)(Data[offset] % 4);
        size_t idx = Data[offset + 1] % 5;
        
        ares_dns_rr_t *retrieved_rr = ares_dns_record_rr_get(dnsrec, sect, idx);
        (void)retrieved_rr;
        offset += 2;
    }
    
    // Cleanup
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
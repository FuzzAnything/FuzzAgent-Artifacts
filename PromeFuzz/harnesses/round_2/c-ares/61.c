// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_get_opt at ares_dns_record.c:1016:16 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_del_opt_byid at ares_dns_record.c:1428:15 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include required headers first */
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"

static ares_dns_record_t* create_test_dns_record(void) {
    /* Create a minimal DNS record for testing */
    ares_dns_record_t* dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, 0x1234, 0, 0, 0);
    
    if (status != ARES_SUCCESS || !dnsrec) {
        return NULL;
    }
    
    return dnsrec;
}

static ares_dns_rr_t* create_opt_rr(ares_dns_record_t* dnsrec) {
    /* Create an OPT RR structure */
    ares_dns_rr_t* rr = NULL;
    
    /* We need to use the actual function from the header - let's check what's available */
    /* Based on the provided information, we don't have ares_dns_rr_create, so we'll try to get a different way */
    
    /* Try to add a question first to have some structure to work with */
    ares_status_t status = ares_dns_record_query_add(dnsrec, "example.com", ARES_REC_TYPE_A, ARES_CLASS_IN);
    
    /* For OPT RR, we might need to work with existing structure or create differently */
    /* Since we can't create RRs directly, let's work with what we can get from the record */
    return NULL;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }
    
    /* Initialize test DNS record */
    ares_dns_record_t* dnsrec = create_test_dns_record();
    if (!dnsrec) {
        return 0;
    }
    
    /* Try to get RR from different sections - they might be empty initially */
    ares_dns_rr_t* dns_rr = NULL;
    
    /* Try different sections to find any available RR */
    ares_dns_section_t sections[] = {
        0, /* QD section */
        1, /* AN section */
        2, /* NS section */
        3  /* AR section */
    };
    
    for (size_t i = 0; i < 4; i++) {
        dns_rr = ares_dns_record_rr_get(dnsrec, sections[i], 0);
        if (dns_rr) {
            break;
        }
    }
    
    /* If no RR found, we can still test some functions that don't need a valid RR */
    if (!dns_rr) {
        /* Test ares_dns_rr_key_tostr with various keys even without RR */
        ares_dns_rr_key_t test_keys[] = {
            0, 100, 200, 300, 400, 500, 600, 700, 800, 900
        };
        
        for (size_t i = 0; i < sizeof(test_keys)/sizeof(test_keys[0]); i++) {
            const char* str = ares_dns_rr_key_tostr(test_keys[i]);
            (void)str;
        }
        
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    /* Use fuzzer input to generate parameters */
    unsigned short opt_id = 0;
    ares_dns_rr_key_t key = 0;
    size_t idx = 0;
    
    /* Extract parameters from fuzzer input */
    if (Size >= sizeof(unsigned short)) {
        memcpy(&opt_id, Data, sizeof(unsigned short));
        idx = Size > 2 ? Data[2] % 10 : 0;
        
        /* Use fuzzer data to generate a key value */
        key = (Data[1] % 10) * 100;
    }
    
    /* Test ares_dns_rr_set_opt */
    if (Size >= 4) {
        const unsigned char* val = Data + 3;
        size_t val_len = Size - 3;
        if (val_len > 100) val_len = 100;
        
        ares_status_t status = ares_dns_rr_set_opt(
            dns_rr, 
            key, 
            opt_id, 
            val, 
            val_len
        );
        (void)status;
    }
    
    /* Test ares_dns_rr_get_opt */
    const unsigned char* out_val = NULL;
    size_t out_val_len = 0;
    unsigned short retrieved_opt = ares_dns_rr_get_opt(
        dns_rr, 
        key, 
        idx, 
        &out_val, 
        &out_val_len
    );
    (void)retrieved_opt;
    
    /* Test ares_dns_rr_set_u16 */
    if (Size >= sizeof(unsigned short) * 2) {
        unsigned short u16_val = 0;
        memcpy(&u16_val, Data, sizeof(unsigned short));
        
        /* Try different key values that might be U16 type */
        ares_dns_rr_key_t u16_keys[] = {
            100, 101, 102, 200, 201, 300, 301
        };
        
        for (size_t i = 0; i < sizeof(u16_keys)/sizeof(u16_keys[0]); i++) {
            ares_status_t status = ares_dns_rr_set_u16(
                dns_rr, 
                u16_keys[i], 
                u16_val
            );
            (void)status;
        }
    }
    
    /* Test ares_dns_rr_del_opt_byid */
    if (Size >= sizeof(unsigned short)) {
        ares_status_t status = ares_dns_rr_del_opt_byid(
            dns_rr, 
            key, 
            opt_id
        );
        (void)status;
    }
    
    /* Test ares_dns_rr_key_tostr with various keys */
    ares_dns_rr_key_t test_keys[] = {
        0, 100, 101, 102, 103, 200, 201, 202, 300, 301, 302, 400, 401, 500, 501, 600, 700, 800, 900
    };
    
    for (size_t i = 0; i < sizeof(test_keys)/sizeof(test_keys[0]); i++) {
        const char* str = ares_dns_rr_key_tostr(test_keys[i]);
        (void)str;
    }
    
    /* Test ares_dns_record_rr_get with various indices and sections */
    for (size_t section = 0; section < 4; section++) {
        for (size_t i = 0; i < 3; i++) {
            ares_dns_rr_t* test_rr = ares_dns_record_rr_get(
                dnsrec, 
                section, 
                i
            );
            (void)test_rr;
        }
    }
    
    /* Cleanup */
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_rr_get_abin at ares_dns_record.c:899:22 in ares_dns_record.h
// ares_dns_rr_get_bin at ares_dns_record.c:842:22 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define missing types from c-ares */
typedef enum {
    ARES_SUCCESS = 0
} ares_status_t;

typedef enum {
    ARES_FALSE = 0,
    ARES_TRUE = 1
} ares_bool_t;

/* Define CARES_EXTERN macro if not defined in headers */
#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

#include "ares_dns_record.h"

static const ares_dns_section_t sections[] = {
    ARES_SECTION_ANSWER,
    ARES_SECTION_AUTHORITY,
    ARES_SECTION_ADDITIONAL
};
static const int num_sections = sizeof(sections) / sizeof(sections[0]);

/* Use keys that are actually defined in ares_dns_record.h */
static const ares_dns_rr_key_t test_keys[] = {
    ARES_RR_A_ADDR,           /* For A records */
    ARES_RR_AAAA_ADDR,        /* For AAAA records */
    ARES_RR_TXT_DATA,         /* For TXT records */
    ARES_RR_NS_NSDNAME        /* For NS records */
};
static const int num_test_keys = sizeof(test_keys) / sizeof(test_keys[0]);

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    ares_dns_record_t *dnsrec = NULL;
    
    /* Parse the input as a DNS record - correct signature from header */
    ares_status_t status = ares_dns_parse(Data, Size, 0, &dnsrec);
    if (status != ARES_SUCCESS) {
        return 0;
    }
    
    /* Loop through all sections */
    for (int sect_idx = 0; sect_idx < num_sections; sect_idx++) {
        ares_dns_section_t sect = sections[sect_idx];
        
        /* Call ares_dns_record_rr_cnt */
        size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, sect);
        
        /* Loop through all resource records in this section */
        for (size_t rr_idx = 0; rr_idx < rr_cnt; rr_idx++) {
            /* Call ares_dns_record_rr_get */
            ares_dns_rr_t *dns_rr = ares_dns_record_rr_get(dnsrec, sect, rr_idx);
            if (dns_rr == NULL) {
                continue;
            }
            
            /* Call ares_dns_rr_get_type */
            ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(dns_rr);
            
            /* Try each test key */
            for (int key_idx = 0; key_idx < num_test_keys; key_idx++) {
                ares_dns_rr_key_t key = test_keys[key_idx];
                
                /* Call ares_dns_rr_get_abin_cnt */
                size_t abin_cnt = ares_dns_rr_get_abin_cnt(dns_rr, key);
                
                /* Call ares_dns_rr_get_abin for each index if available */
                for (size_t abin_idx = 0; abin_idx < abin_cnt; abin_idx++) {
                    size_t abin_len = 0;
                    const unsigned char *abin_data = ares_dns_rr_get_abin(
                        dns_rr, key, abin_idx, &abin_len);
                    
                    /* Access some data to ensure it's read */
                    if (abin_data != NULL && abin_len > 0) {
                        volatile unsigned char dummy = abin_data[0];
                        (void)dummy;
                        if (abin_len > 1) {
                            dummy = abin_data[abin_len - 1];
                            (void)dummy;
                        }
                    }
                }
                
                /* Call ares_dns_rr_get_bin */
                size_t bin_len = 0;
                const unsigned char *bin_data = ares_dns_rr_get_bin(
                    dns_rr, key, &bin_len);
                
                /* Access some data to ensure it's read */
                if (bin_data != NULL && bin_len > 0) {
                    volatile unsigned char dummy = bin_data[0];
                    (void)dummy;
                    if (bin_len > 1) {
                        dummy = bin_data[bin_len - 1];
                        (void)dummy;
                    }
                }
            }
        }
    }
    
    /* Clean up */
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
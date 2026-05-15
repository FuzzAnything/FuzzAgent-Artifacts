// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_rr_get_const at ares_dns_record.c:499:3 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
// ares_dns_record_rr_get_const at ares_dns_record.c:499:3 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get_const at ares_dns_record.c:499:3 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
// ares_dns_record_rr_get_const at ares_dns_record.c:499:3 in ares_dns_record.h
// ares_dns_rr_get_type at ares_dns_record.c:513:21 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if not already defined */
#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

/* Include necessary headers */
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_record_t* create_test_record(const uint8_t *data, size_t size) {
    ares_dns_record_t *record = NULL;
    ares_status_t status;
    
    /* Extract parameters from fuzz data for ares_dns_record_create */
    unsigned short id = 12345;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    /* Use fuzz data to set parameters if we have enough data */
    if (size >= 2) {
        id = (data[0] << 8) | data[1];
    }
    if (size >= 4) {
        flags = (data[2] << 8) | data[3];
    }
    if (size >= 5) {
        opcode = (ares_dns_opcode_t)(data[4] % 16); /* Opcode is 4 bits */
    }
    if (size >= 6) {
        rcode = (ares_dns_rcode_t)(data[5] % 16); /* RCODE is 4 bits */
    }
    
    /* Create DNS record with extracted parameters */
    status = ares_dns_record_create(&record, id, flags, opcode, rcode);
    
    if (status != ARES_SUCCESS) {
        return NULL;
    }
    
    return record;
}

static void free_test_record(ares_dns_record_t *record) {
    if (record) {
        ares_dns_record_destroy(record);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    /* Create a test DNS record using fuzz data */
    ares_dns_record_t *dnsrec = create_test_record(Data, Size);
    if (!dnsrec) {
        return 0;
    }
    
    /* Test different sections */
    ares_dns_section_t sections[] = {
        ARES_SECTION_ANSWER,
        ARES_SECTION_AUTHORITY,
        ARES_SECTION_ADDITIONAL
    };
    
    for (size_t sect_idx = 0; sect_idx < sizeof(sections)/sizeof(sections[0]); sect_idx++) {
        /* First function: ares_dns_record_rr_cnt */
        size_t rr_count = ares_dns_record_rr_cnt(dnsrec, sections[sect_idx]);
        
        /* Second function: ares_dns_record_rr_get_const */
        if (rr_count > 0) {
            /* Try to get some resource records */
            for (size_t i = 0; i < rr_count && i < 10; i++) { /* Limit to 10 to avoid excessive loops */
                const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, sections[sect_idx], i);
                
                /* Third function: ares_dns_rr_get_type */
                if (rr) {
                    ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                    /* Type is returned, we don't need to do anything with it */
                    (void)rr_type;
                }
            }
        }
        
        /* Also test with potentially invalid indices */
        if (rr_count > 0) {
            /* Test index at count (should be invalid) */
            const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, sections[sect_idx], rr_count);
            if (rr) {
                ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                (void)rr_type;
            }
        }
        
        /* Test with very large index */
        const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, sections[sect_idx], SIZE_MAX);
        if (rr) {
            ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
            (void)rr_type;
        }
    }
    
    /* Test with NULL input for ares_dns_rr_get_type */
    ares_dns_rec_type_t null_type = ares_dns_rr_get_type(NULL);
    (void)null_type;
    
    /* Test with NULL input for ares_dns_record_rr_get_const */
    const ares_dns_rr_t *null_rr = ares_dns_record_rr_get_const(NULL, ARES_SECTION_ANSWER, 0);
    if (null_rr) {
        ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(null_rr);
        (void)rr_type;
    }
    
    /* Test with NULL input for ares_dns_record_rr_cnt */
    size_t null_count = ares_dns_record_rr_cnt(NULL, ARES_SECTION_ANSWER);
    (void)null_count;
    
    /* Clean up */
    free_test_record(dnsrec);
    
    return 0;
}
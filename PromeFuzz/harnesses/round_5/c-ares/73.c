// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_get_u16 at ares_dns_record.c:808:16 in ares_dns_record.h
// ares_dns_rr_set_u8 at ares_dns_record.c:1140:15 in ares_dns_record.h
// ares_dns_rr_get_u8 at ares_dns_record.c:791:15 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_rr_get_u16 at ares_dns_record.c:808:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_get_u8 at ares_dns_record.c:791:15 in ares_dns_record.h
// ares_dns_opt_get_datatype at ares_dns_mapping.c:819:25 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
#include "ares.h"
#include "ares_dns_record.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static ares_dns_record_t *create_dummy_record(void) {
    ares_dns_record_t *dns_rec = NULL;
    ares_status_t status;
    
    /* Create a minimal DNS record with correct number of arguments */
    status = ares_dns_record_create(&dns_rec, 12345, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || dns_rec == NULL) {
        return NULL;
    }
    
    return dns_rec;
}

static ares_dns_rr_t *create_dummy_rr(ares_dns_record_t *dns_rec) {
    ares_dns_rr_t *rr = NULL;
    ares_status_t status;
    
    if (dns_rec == NULL) {
        return NULL;
    }
    
    /* Add a resource record to the answer section with correct argument order */
    status = ares_dns_record_rr_add(&rr, dns_rec, ARES_SECTION_ANSWER, 
                                    "example.com", ARES_REC_TYPE_A, 
                                    ARES_CLASS_IN, 300);
    if (status != ARES_SUCCESS || rr == NULL) {
        return NULL;
    }
    
    return rr;
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Create dummy DNS record and RR for testing */
    ares_dns_record_t *dns_rec = create_dummy_record();
    if (dns_rec == NULL) {
        return 0;
    }
    
    ares_dns_rr_t *rr = create_dummy_rr(dns_rec);
    if (rr == NULL) {
        ares_dns_record_destroy(dns_rec);
        return 0;
    }

    /* Use the fuzzer data to generate various inputs */
    const uint8_t *data_ptr = Data;
    size_t remaining = Size;

    /* Test ares_dns_rr_get_u8 with various keys */
    for (int key = 0; key < 1000 && remaining > 0; key++) {
        unsigned char result = ares_dns_rr_get_u8(rr, (ares_dns_rr_key_t)key);
        (void)result;
        remaining--;
    }

    /* Test ares_dns_opt_get_datatype with various keys and options */
    for (int key = 0; key < 100 && remaining >= 2; key++) {
        unsigned short opt = 0;
        if (remaining >= 2) {
            opt = (data_ptr[0] << 8) | data_ptr[1];
            data_ptr += 2;
            remaining -= 2;
        }
        ares_dns_opt_datatype_t datatype = ares_dns_opt_get_datatype(
            (ares_dns_rr_key_t)key, opt);
        (void)datatype;
    }

    /* Test ares_dns_rr_set_bin with binary data from fuzzer input */
    if (remaining > 0) {
        size_t bin_len = remaining > 100 ? 100 : remaining;
        ares_status_t status = ares_dns_rr_set_bin(
            rr, 
            ARES_RR_A_ADDR,
            data_ptr, 
            bin_len);
        (void)status;
        
        /* Also test with different keys */
        for (int key = 0; key < 50 && remaining > bin_len; key++) {
            size_t small_len = remaining > 10 ? 10 : remaining;
            status = ares_dns_rr_set_bin(
                rr, 
                (ares_dns_rr_key_t)key, 
                data_ptr, 
                small_len);
            (void)status;
            data_ptr += small_len;
            remaining -= small_len;
        }
    }

    /* Test ares_dns_rr_set_u16 with various values */
    for (int i = 0; i < 10 && remaining >= 2; i++) {
        unsigned short val = 0;
        if (remaining >= 2) {
            val = (data_ptr[0] << 8) | data_ptr[1];
            data_ptr += 2;
            remaining -= 2;
        }
        
        /* Test with different keys */
        for (int key = 0; key < 10; key++) {
            ares_status_t status = ares_dns_rr_set_u16(
                rr, 
                (ares_dns_rr_key_t)key, 
                val);
            (void)status;
            
            /* Also test the getter */
            unsigned short get_result = ares_dns_rr_get_u16(
                rr, 
                (ares_dns_rr_key_t)key);
            (void)get_result;
        }
    }

    /* Test ares_dns_rr_set_u8 with various values */
    for (int i = 0; i < 20 && remaining > 0; i++) {
        unsigned char val = *data_ptr;
        data_ptr++;
        remaining--;
        
        /* Test with different keys */
        for (int key = 0; key < 10; key++) {
            ares_status_t status = ares_dns_rr_set_u8(
                rr, 
                (ares_dns_rr_key_t)key, 
                val);
            (void)status;
            
            /* Also test the getter */
            unsigned char get_result = ares_dns_rr_get_u8(
                rr, 
                (ares_dns_rr_key_t)key);
            (void)get_result;
        }
    }

    /* Test ares_dns_rr_get_u16 with various keys */
    for (int key = 0; key < 1000; key++) {
        unsigned short result = ares_dns_rr_get_u16(rr, (ares_dns_rr_key_t)key);
        (void)result;
    }

    /* Cleanup */
    ares_dns_record_destroy(dns_rec);

    return 0;
}
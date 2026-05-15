// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_u8 at ares_dns_record.c:1140:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_get_u16 at ares_dns_record.c:808:16 in ares_dns_record.h
// ares_dns_opt_get_datatype at ares_dns_mapping.c:819:25 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_rr_get_bin at ares_dns_record.c:842:22 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_record_t* create_test_record(void) {
    /* Create a minimal DNS record to work with */
    ares_dns_record_t* dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, 0x1234, 0, 0, 0);
    if (status != ARES_SUCCESS || !dnsrec) return NULL;
    
    /* Add a resource record to the answer section */
    ares_dns_rr_t* rr = NULL;
    status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                                  "example.com", ARES_REC_TYPE_A, 
                                                  ARES_CLASS_IN, 300);
    if (status != ARES_SUCCESS || !rr) {
        ares_dns_record_destroy(dnsrec);
        return NULL;
    }
    
    return dnsrec;
}

static ares_dns_rr_t* get_test_rr(ares_dns_record_t* dnsrec) {
    /* Get the first resource record from answer section */
    if (!dnsrec) return NULL;
    return ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, 0);
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    /* Create test DNS record */
    ares_dns_record_t* dnsrec = create_test_record();
    if (!dnsrec) return 0;
    
    ares_dns_rr_t* rr = get_test_rr(dnsrec);
    if (!rr) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    /* Use fuzz data to generate various inputs */
    unsigned char u8_val = (Size > 0) ? Data[0] : 0;
    unsigned short u16_val = (Size > 1) ? (Data[0] << 8) | Data[1] : 0;
    ares_dns_rr_key_t key = (ares_dns_rr_key_t)((Size > 0) ? (Data[0] % 100) : 0);
    unsigned short opt = (Size > 1) ? (Data[1] << 8) | ((Size > 2) ? Data[2] : 0) : 0;
    
    /* Test ares_dns_rr_set_u8 */
    ares_status_t status1 = ares_dns_rr_set_u8(rr, key, u8_val);
    
    /* Test ares_dns_rr_set_u16 */
    ares_status_t status2 = ares_dns_rr_set_u16(rr, key, u16_val);
    
    /* Test ares_dns_rr_get_u16 */
    unsigned short retrieved_u16 = ares_dns_rr_get_u16(rr, key);
    (void)retrieved_u16;
    
    /* Test ares_dns_opt_get_datatype */
    ares_dns_opt_datatype_t datatype = ares_dns_opt_get_datatype(key, opt);
    (void)datatype;
    
    /* Test ares_dns_rr_set_bin */
    const unsigned char* bin_data = (Size > 0) ? Data : NULL;
    size_t bin_len = (Size > 0) ? Size : 0;
    ares_status_t status3 = ares_dns_rr_set_bin(rr, key, bin_data, bin_len);
    
    /* Test ares_dns_rr_get_bin */
    size_t out_len = 0;
    const unsigned char* retrieved_bin = ares_dns_rr_get_bin(rr, key, &out_len);
    (void)retrieved_bin;
    (void)out_len;
    
    /* Clean up */
    ares_dns_record_destroy(dnsrec);
    
    /* Use status variables to avoid unused variable warnings */
    (void)status1;
    (void)status2;
    (void)status3;
    
    return 0;
}
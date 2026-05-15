// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_del_opt_byid at ares_dns_record.c:1428:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_get_opt at ares_dns_record.c:1016:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include necessary headers in correct order */
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_record_t *create_dummy_dns_record(void) {
    /* Create a minimal DNS record for testing */
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, 0x1234, 0, 
                                                  ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || !dnsrec) return NULL;
    
    return dnsrec;
}

static ares_dns_rr_t *create_dummy_rr(ares_dns_record_t *dnsrec) {
    /* Create an OPT resource record (commonly used for EDNS) */
    ares_dns_rr_t *rr = NULL;
    
    /* First, we need to add the RR to the record */
    ares_status_t status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                                  NULL, ARES_REC_TYPE_OPT, 
                                                  ARES_CLASS_IN, 0);
    if (status != ARES_SUCCESS || !rr) return NULL;
    
    return rr;
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    /* Initialize with fuzzer data */
    if (Size < 2) return 0;
    
    /* Create dummy DNS record and resource record */
    ares_dns_record_t *dnsrec = create_dummy_dns_record();
    if (!dnsrec) return 0;
    
    ares_dns_rr_t *rr = create_dummy_rr(dnsrec);
    if (!rr) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    /* Extract key from fuzzer input (modulo to get valid enum range) */
    ares_dns_rr_key_t key = (ares_dns_rr_key_t)(Data[0] % 256);
    
    /* Test ares_dns_rr_key_datatype */
    ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
    (void)datatype; /* Use result to avoid unused variable warning */
    
    /* Test ares_dns_rr_key_tostr */
    const char *key_str = ares_dns_rr_key_tostr(key);
    (void)key_str;
    
    /* Test ares_dns_rr_set_u16 - only if we have enough data */
    if (Size >= 4) {
        unsigned short val = (Data[1] << 8) | Data[2];
        ares_status_t status = ares_dns_rr_set_u16(rr, key, val);
        (void)status;
    }
    
    /* Test ares_dns_rr_del_opt_byid - only for OPT datatype keys */
    if (Size >= 6) {
        unsigned short opt_id = (Data[3] << 8) | Data[4];
        ares_status_t status = ares_dns_rr_del_opt_byid(rr, key, opt_id);
        (void)status;
    }
    
    /* Test ares_dns_rr_set_opt */
    if (Size >= 8) {
        unsigned short opt_id = (Data[5] << 8) | Data[6];
        size_t val_len = Size - 7;
        const unsigned char *val = Data + 7;
        
        /* Ensure val_len doesn't cause overflow */
        if (val_len > Size - 7) val_len = Size - 7;
        
        ares_status_t status = ares_dns_rr_set_opt(rr, key, opt_id, val, val_len);
        (void)status;
    }
    
    /* Test ares_dns_rr_get_opt */
    if (Size >= 10) {
        size_t idx = Data[7] % 16; /* Limit index to small range */
        const unsigned char *val = NULL;
        size_t val_len = 0;
        
        unsigned short opt_ret = ares_dns_rr_get_opt(rr, key, idx, &val, &val_len);
        (void)opt_ret;
    }
    
    /* Cleanup */
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
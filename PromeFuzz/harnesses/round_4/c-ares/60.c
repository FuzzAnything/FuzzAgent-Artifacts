// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_get_abin at ares_dns_record.c:899:22 in ares_dns_record.h
// ares_dns_rr_del_abin at ares_dns_record.c:917:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_rr_get_bin at ares_dns_record.c:842:22 in ares_dns_record.h
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static const ares_dns_rr_key_t abinp_keys[] = {
    ARES_RR_OPT_OPTIONS,
    /* Add other ABINP keys as needed */
};

static const ares_dns_rr_key_t bin_keys[] = {
    ARES_RR_TXT_DATA,
    ARES_RR_OPT_OPTIONS,
    /* Add other BIN/BINP keys as needed */
};

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    /* Create a DNS record with safe bounds checking */
    ares_dns_record_t *dnsrec = NULL;
    unsigned short id = 0;
    unsigned short flags = 0;
    
    /* Safe access to Data[0] and Data[1] since we checked Size >= 2 */
    id = (unsigned short)(Data[0] % 65536);
    if (Size > 1) {
        flags = (unsigned short)(Data[1] % 65536);
    }
    
    ares_status_t status = ares_dns_record_create(&dnsrec, 
                                                  id,
                                                  flags,
                                                  ARES_OPCODE_QUERY,
                                                  ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || !dnsrec) return 0;

    /* Create and add a TXT resource record */
    ares_dns_rr_t *rr = NULL;
    status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                    "test.example.com", ARES_REC_TYPE_TXT, 
                                    ARES_CLASS_IN, 300);
    if (status != ARES_SUCCESS || !rr) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }

    /* Use fuzzer input for binary data with bounds checking */
    const unsigned char *val = Data;
    size_t len = Size;

    /* Test ares_dns_rr_set_bin with a BINP key */
    if (Size > 0) {
        size_t key_index = 0;
        if (Size > 2) {
            key_index = Data[2] % (sizeof(bin_keys)/sizeof(bin_keys[0]));
        }
        ares_dns_rr_key_t key = bin_keys[key_index];
        ares_dns_rr_set_bin(rr, key, val, len);
    }

    /* Test ares_dns_rr_get_bin */
    size_t get_len = 0;
    if (Size > 1) {
        size_t key_index = 0;
        if (Size > 3) {
            key_index = Data[3] % (sizeof(bin_keys)/sizeof(bin_keys[0]));
        }
        ares_dns_rr_key_t key = bin_keys[key_index];
        const unsigned char *bin_data = ares_dns_rr_get_bin(rr, key, &get_len);
        (void)bin_data; /* Use result to avoid unused variable warning */
    }

    /* Test ares_dns_rr_add_abin with an ABINP key */
    if (Size > 2) {
        size_t key_index = 0;
        if (Size > 4) {
            key_index = Data[4] % (sizeof(abinp_keys)/sizeof(abinp_keys[0]));
        }
        ares_dns_rr_key_t key = abinp_keys[key_index];
        ares_dns_rr_add_abin(rr, key, val, len);
    }

    /* Test ares_dns_rr_get_abin_cnt */
    if (Size > 3) {
        size_t key_index = 0;
        if (Size > 5) {
            key_index = Data[5] % (sizeof(abinp_keys)/sizeof(abinp_keys[0]));
        }
        ares_dns_rr_key_t key = abinp_keys[key_index];
        size_t cnt = ares_dns_rr_get_abin_cnt(rr, key);
        (void)cnt;
    }

    /* Test ares_dns_rr_get_abin */
    if (Size > 4) {
        size_t key_index = 0;
        if (Size > 6) {
            key_index = Data[6] % (sizeof(abinp_keys)/sizeof(abinp_keys[0]));
        }
        ares_dns_rr_key_t key = abinp_keys[key_index];
        size_t idx = 0;
        if (Size > 7) {
            idx = Data[7] % 8; /* Limit index to reasonable range */
        }
        size_t abin_len = 0;
        const unsigned char *abin_data = ares_dns_rr_get_abin(rr, key, idx, &abin_len);
        (void)abin_data;
    }

    /* Test ares_dns_rr_del_abin */
    if (Size > 6) {
        size_t key_index = 0;
        if (Size > 8) {
            key_index = Data[8] % (sizeof(abinp_keys)/sizeof(abinp_keys[0]));
        }
        ares_dns_rr_key_t key = abinp_keys[key_index];
        size_t idx = 0;
        if (Size > 9) {
            idx = Data[9] % 8;
        }
        ares_dns_rr_del_abin(rr, key, idx);
    }

    /* Cleanup */
    ares_dns_record_destroy(dnsrec);
    return 0;
}
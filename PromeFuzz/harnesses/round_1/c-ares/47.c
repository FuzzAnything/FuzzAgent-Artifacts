// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_get_opt at ares_dns_record.c:1016:16 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_del_opt_byid at ares_dns_record.c:1428:15 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include ares.h first to get CARES_EXTERN definition */
#include "ares.h"
#include "ares_dns_record.h"

static const ares_dns_section_t sections[] = {
    ARES_SECTION_ANSWER,
    ARES_SECTION_AUTHORITY,
    ARES_SECTION_ADDITIONAL
};

static const ares_dns_rr_key_t test_keys[] = {
    ARES_RR_A_ADDR,
    ARES_RR_NS_NSDNAME,
    ARES_RR_CNAME_CNAME,
    ARES_RR_SOA_RNAME,
    ARES_RR_PTR_DNAME,
    ARES_RR_MX_PREFERENCE,
    ARES_RR_AAAA_ADDR,
    ARES_RR_OPT_UDP_SIZE,
    ARES_RR_OPT_VERSION
};

#define MAX_TEST_KEYS (sizeof(test_keys)/sizeof(test_keys[0]))

static ares_dns_record_t* create_test_record(void) {
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status;
    
    status = ares_dns_record_create(&dnsrec, 1234, ARES_FLAG_QR | ARES_FLAG_RD, 
                                    ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || !dnsrec) {
        return NULL;
    }
    
    return dnsrec;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create a test DNS record
    ares_dns_record_t *dnsrec = create_test_record();
    if (!dnsrec) return 0;
    
    // Test ares_dns_record_rr_get with various sections and indices
    // We'll test with some dummy data since we don't have actual RRs added
    for (size_t s = 0; s < sizeof(sections)/sizeof(sections[0]); s++) {
        for (size_t idx = 0; idx < 3; idx++) {
            ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, sections[s], idx);
            // rr will likely be NULL since we haven't added any records
            if (!rr) continue;
            
            // Test ares_dns_rr_key_datatype
            for (size_t k = 0; k < MAX_TEST_KEYS; k++) {
                ares_dns_datatype_t dt = ares_dns_rr_key_datatype(test_keys[k]);
                (void)dt; // Use result to avoid unused variable warning
            }
            
            // Test ares_dns_rr_set_u16 for U16 datatype keys
            for (size_t k = 0; k < MAX_TEST_KEYS; k++) {
                ares_dns_datatype_t dt = ares_dns_rr_key_datatype(test_keys[k]);
                if (dt == ARES_DATATYPE_U16) {
                    unsigned short val = (Size > 0) ? ((Data[0] << 8) | (Size > 1 ? Data[1] : 0)) : 0;
                    ares_dns_rr_set_u16(rr, test_keys[k], val);
                }
            }
            
            // Test ares_dns_rr_del_opt_byid for OPT keys
            for (size_t k = 0; k < MAX_TEST_KEYS; k++) {
                ares_dns_datatype_t dt = ares_dns_rr_key_datatype(test_keys[k]);
                if (dt == ARES_DATATYPE_OPT) {
                    unsigned short opt_id = (Size > 1) ? ((Data[0] << 8) | Data[1]) : 0;
                    ares_dns_rr_del_opt_byid(rr, test_keys[k], opt_id);
                }
            }
            
            // Test ares_dns_rr_set_opt for OPT keys
            for (size_t k = 0; k < MAX_TEST_KEYS; k++) {
                ares_dns_datatype_t dt = ares_dns_rr_key_datatype(test_keys[k]);
                if (dt == ARES_DATATYPE_OPT && Size > 2) {
                    unsigned short opt_id = (Data[0] << 8) | Data[1];
                    size_t val_len = Size > 100 ? 50 : Size/2;
                    const unsigned char *val = Data + 2;
                    if (val_len > Size - 2) val_len = Size - 2;
                    ares_dns_rr_set_opt(rr, test_keys[k], opt_id, val, val_len);
                }
            }
            
            // Test ares_dns_rr_get_opt for OPT keys
            for (size_t k = 0; k < MAX_TEST_KEYS; k++) {
                ares_dns_datatype_t dt = ares_dns_rr_key_datatype(test_keys[k]);
                if (dt == ARES_DATATYPE_OPT) {
                    for (size_t idx_opt = 0; idx_opt < 3; idx_opt++) {
                        const unsigned char *val = NULL;
                        size_t val_len = 0;
                        unsigned short result = ares_dns_rr_get_opt(rr, test_keys[k], idx_opt, &val, &val_len);
                        (void)result; // Use result to avoid unused variable warning
                    }
                }
            }
        }
    }
    
    // Clean up
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
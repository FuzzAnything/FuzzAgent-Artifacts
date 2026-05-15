// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_get_addr at ares_dns_record.c:757:23 in ares_dns_record.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_dns_rr_get_addr6 at ares_dns_record.c:774:29 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_write at ares_dns_write.c:1193:15 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static ares_dns_record_t *create_test_dns_record(void) {
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status;
    
    status = ares_dns_record_create(&dnsrec, 0x1234, ARES_FLAG_QR | ARES_FLAG_RD,
                                    ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS) {
        return NULL;
    }
    
    return dnsrec;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    ares_dns_rr_key_t key = (ares_dns_rr_key_t)(Data[0] % 100);
    
    ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
    
    char dst[INET6_ADDRSTRLEN];
    struct in_addr addr4;
    struct ares_in6_addr addr6;
    
    if (Size >= 4) {
        memcpy(&addr4, Data, 4);
        ares_inet_ntop(AF_INET, &addr4, dst, sizeof(dst));
    }
    
    ares_dns_record_t *dnsrec = create_test_dns_record();
    if (dnsrec) {
        ares_dns_rr_t *rr = NULL;
        ares_status_t status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                                                      "example.com", ARES_REC_TYPE_A,
                                                      ARES_CLASS_IN, 3600);
        if (status == ARES_SUCCESS && rr) {
            const struct in_addr *addr_ptr = ares_dns_rr_get_addr(rr, ARES_RR_A_ADDR);
            
            if (Size >= 4) {
                ares_inet_ntop(AF_INET, &addr4, dst, sizeof(dst));
            }
            
            const struct ares_in6_addr *addr6_ptr = ares_dns_rr_get_addr6(rr, ARES_RR_AAAA_ADDR);
            
            size_t abin_cnt = ares_dns_rr_get_abin_cnt(rr, ARES_RR_OPT_OPTIONS);
        }
        
        unsigned char *buf = NULL;
        size_t buf_len = 0;
        ares_status_t write_status = ares_dns_write(dnsrec, &buf, &buf_len);
        if (write_status == ARES_SUCCESS && buf) {
            ares_free_string(buf);
        }
        
        ares_dns_record_destroy(dnsrec);
    }
    
    if (Size >= 16) {
        memcpy(&addr6, Data, 16);
        ares_inet_ntop(AF_INET6, &addr6, dst, sizeof(dst));
    }
    
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_ttl at ares_dns_record.c:537:15 in ares_dns_record.h
// ares_dns_rr_get_ttl at ares_dns_record.c:529:14 in ares_dns_record.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_dns_rr_set_addr at ares_dns_record.c:1103:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_rr_t* create_test_rr(ares_dns_record_t **parent_out) {
    /* Create a minimal DNS record */
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t create_status = ares_dns_record_create(&dnsrec, 1234, 0x8000, 
                                                        ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (create_status != ARES_SUCCESS || !dnsrec) return NULL;
    
    /* Add an A record to the answer section */
    ares_dns_rr_t *rr = NULL;
    ares_status_t add_status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                                     "example.com", ARES_REC_TYPE_A, 
                                                     ARES_CLASS_IN, 0);
    if (add_status != ARES_SUCCESS || !rr) {
        ares_dns_record_destroy(dnsrec);
        return NULL;
    }
    
    /* Return parent through output parameter */
    if (parent_out) {
        *parent_out = dnsrec;
    }
    
    return rr;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Create a test DNS resource record */
    ares_dns_record_t *parent = NULL;
    ares_dns_rr_t *dns_rr = create_test_rr(&parent);
    if (!dns_rr || !parent) return 0;
    
    /* 1. Test ares_dns_rr_set_ttl */
    unsigned int ttl = 0;
    if (Size >= 4) {
        ttl = *((unsigned int*)Data);
    }
    ares_status_t status1 = ares_dns_rr_set_ttl(dns_rr, ttl);
    (void)status1;
    
    /* 2. Test ares_dns_rr_get_ttl */
    unsigned int retrieved_ttl = ares_dns_rr_get_ttl(dns_rr);
    (void)retrieved_ttl;
    
    /* 3. Test ares_inet_pton */
    int af = AF_INET;
    if (Size >= 5 && (Data[4] & 1)) {
        af = AF_INET6;
    }
    
    /* Create a null-terminated string from fuzzer data for the address */
    char src[256];
    size_t str_len = Size - 5;
    if (str_len > 255) str_len = 255;
    if (str_len > 0 && Size > 5) {
        memcpy(src, Data + 5, str_len);
        src[str_len] = '\0';
    } else {
        strcpy(src, "127.0.0.1");
    }
    
    struct in_addr dst4;
    struct in6_addr dst6;
    void *dst = (af == AF_INET) ? (void*)&dst4 : (void*)&dst6;
    
    int pton_result = ares_inet_pton(af, src, dst);
    (void)pton_result;
    
    /* 4. Test ares_dns_rr_set_addr */
    struct in_addr addr;
    if (Size >= 9) {
        memcpy(&addr, Data + (Size >= 10 ? Size - 9 : 0), 4);
    } else {
        addr.s_addr = htonl(0x7f000001); /* 127.0.0.1 */
    }
    
    ares_status_t status2 = ares_dns_rr_set_addr(dns_rr, ARES_RR_A_ADDR, &addr);
    (void)status2;
    
    /* Clean up - destroy the parent DNS record which owns the RR */
    ares_dns_record_destroy(parent);
    
    return 0;
}
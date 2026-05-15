// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_ttl at ares_dns_record.c:537:15 in ares_dns_record.h
// ares_dns_rr_get_ttl at ares_dns_record.c:529:14 in ares_dns_record.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_dns_rr_set_addr at ares_dns_record.c:1103:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "ares.h"
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Create a DNS record to get a valid resource record */
    ares_dns_record_t *dns_record = NULL;
    ares_status_t status = ares_dns_record_create(&dns_record, 0, 0, ARES_OPCODE_QUERY, 0);
    if (status != ARES_SUCCESS || dns_record == NULL) {
        return 0;
    }
    
    /* Add an A record to the answer section */
    ares_dns_rr_t *dns_rr = NULL;
    status = ares_dns_record_rr_add(&dns_rr, dns_record, ARES_SECTION_ANSWER, 
                                    "example.com", ARES_REC_TYPE_A, ARES_CLASS_IN, 0);
    if (status != ARES_SUCCESS || dns_rr == NULL) {
        ares_dns_record_destroy(dns_record);
        return 0;
    }
    
    /* 1. Test ares_dns_rr_set_ttl */
    unsigned int ttl = 0;
    if (Size >= sizeof(unsigned int)) {
        memcpy(&ttl, Data, sizeof(unsigned int));
    } else {
        ttl = (unsigned int)Data[0];
    }
    
    status = ares_dns_rr_set_ttl(dns_rr, ttl);
    (void)status; /* Ignore return value for fuzzing */
    
    /* 2. Test ares_dns_rr_get_ttl */
    unsigned int retrieved_ttl = ares_dns_rr_get_ttl(dns_rr);
    (void)retrieved_ttl; /* Ignore return value for fuzzing */
    
    /* 3. Test ares_inet_pton */
    /* Create null-terminated string from fuzzer input for IP address */
    size_t ip_str_len = Size > 64 ? 64 : Size;
    char ip_str[65];
    memcpy(ip_str, Data, ip_str_len);
    ip_str[ip_str_len] = '\0';
    
    /* Test both IPv4 and IPv6 */
    struct in_addr addr4;
    struct in6_addr addr6;
    
    int result4 = ares_inet_pton(AF_INET, ip_str, &addr4);
    int result6 = ares_inet_pton(AF_INET6, ip_str, &addr6);
    (void)result4;
    (void)result6;
    
    /* 4. Test ares_dns_rr_set_addr */
    /* Use the IPv4 address we just converted (or default if conversion failed) */
    struct in_addr test_addr;
    if (result4 == 1) {
        test_addr = addr4;
    } else {
        test_addr.s_addr = htonl(0x7F000001); /* 127.0.0.1 */
    }
    
    /* Only use ARES_RR_A_ADDR since we have an A record */
    status = ares_dns_rr_set_addr(dns_rr, ARES_RR_A_ADDR, &test_addr);
    (void)status;
    
    /* Clean up */
    ares_dns_record_destroy(dns_record);
    
    return 0;
}
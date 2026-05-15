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
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_record_t* create_test_dns_record(void) {
    ares_dns_record_t* dns = NULL;
    ares_dns_record_create(&dns, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    return dns;
}

static ares_dns_rr_t* create_test_resource_record(ares_dns_record_t* dns) {
    if (!dns) return NULL;
    
    ares_dns_rr_t* rr = NULL;
    // Add an A record to the answer section
    ares_dns_record_rr_add(&rr, dns, ARES_SECTION_ANSWER, "example.com", ARES_REC_TYPE_A, ARES_CLASS_IN, 0);
    return rr;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(unsigned int) + 1) {
        return 0;
    }

    // Extract TTL from fuzzer data
    unsigned int ttl = 0;
    size_t offset = 0;
    if (Size >= offset + sizeof(unsigned int)) {
        memcpy(&ttl, Data + offset, sizeof(unsigned int));
        offset += sizeof(unsigned int);
    }

    // Create DNS record and resource record
    ares_dns_record_t* dns_record = create_test_dns_record();
    if (!dns_record) {
        return 0;
    }

    ares_dns_rr_t* rr = create_test_resource_record(dns_record);
    if (!rr) {
        ares_dns_record_destroy(dns_record);
        return 0;
    }

    // 1. Test ares_dns_rr_set_ttl
    ares_dns_rr_set_ttl(rr, ttl);
    
    // 2. Test ares_dns_rr_get_ttl
    unsigned int retrieved_ttl = ares_dns_rr_get_ttl(rr);
    
    // 3. Test ares_inet_pton
    // Prepare IP address string from fuzzer data
    char ip_str[256] = {0};
    size_t ip_str_len = Size - offset;
    if (ip_str_len > 255) ip_str_len = 255;
    if (ip_str_len > 0) {
        memcpy(ip_str, Data + offset, ip_str_len);
        ip_str[ip_str_len] = '\0';
    } else {
        // Default test IP
        strcpy(ip_str, "192.168.1.1");
    }
    
    // Test IPv4
    struct in_addr addr4;
    int result4 = ares_inet_pton(AF_INET, ip_str, &addr4);
    
    // Test IPv6 if enough data
    struct in6_addr addr6;
    int result6 = -1;
    if (Size > offset + 16) {
        char ip6_str[256] = {0};
        size_t ip6_len = (Size - offset > 255) ? 255 : Size - offset;
        memcpy(ip6_str, Data + offset, ip6_len);
        ip6_str[ip6_len] = '\0';
        result6 = ares_inet_pton(AF_INET6, ip6_str, &addr6);
    }
    
    // 4. Test ares_dns_rr_set_addr
    // Use the address from ares_inet_pton if successful
    struct in_addr test_addr;
    if (result4 == 1) {
        test_addr = addr4;
    } else {
        // Use a default address (127.0.0.1)
        test_addr.s_addr = htonl(0x7f000001);  // 127.0.0.1 in network byte order
    }
    
    // Try setting address with A record address key
    ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &test_addr);
    
    // Cleanup
    ares_dns_record_destroy(dns_record);
    
    return 0;
}
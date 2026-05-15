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
#include "ares.h"
#include "ares_dns_record.h"
#include <arpa/inet.h>

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Create DNS record with proper parameters */
    ares_dns_record_t* dns_rec = NULL;
    ares_dns_rr_t* rr = NULL;
    
    /* Create a DNS record with all required parameters */
    ares_status_t create_status = ares_dns_record_create(&dns_rec, 
                                                         12345,  /* id */
                                                         0,      /* flags */
                                                         ARES_OPCODE_QUERY,  /* opcode */
                                                         ARES_RCODE_NOERROR); /* rcode */
    
    if (create_status != ARES_SUCCESS || dns_rec == NULL) {
        return 0;
    }
    
    /* Add a resource record to the answer section */
    ares_status_t add_status = ares_dns_record_rr_add(&rr, dns_rec, 
                                                      ARES_SECTION_ANSWER,
                                                      "example.com",
                                                      ARES_REC_TYPE_A,
                                                      ARES_CLASS_IN,
                                                      300);
    
    if (add_status != ARES_SUCCESS || rr == NULL) {
        ares_dns_record_destroy(dns_rec);
        return 0;
    }
    
    /* Extract TTL from fuzzer input */
    unsigned int ttl = 0;
    if (Size >= sizeof(unsigned int)) {
        memcpy(&ttl, Data, sizeof(unsigned int));
        Data += sizeof(unsigned int);
        Size -= sizeof(unsigned int);
    } else {
        ttl = (unsigned int)Data[0];
    }
    
    /* 1. Test ares_dns_rr_set_ttl */
    ares_status_t status1 = ares_dns_rr_set_ttl(rr, ttl);
    
    /* 2. Test ares_dns_rr_get_ttl */
    unsigned int retrieved_ttl = ares_dns_rr_get_ttl(rr);
    
    /* 3. Test ares_inet_pton with various address families */
    if (Size > 0) {
        /* Create null-terminated string from fuzzer data */
        char* ip_str = malloc(Size + 1);
        if (ip_str) {
            memcpy(ip_str, Data, Size);
            ip_str[Size] = '\0';
            
            struct in_addr addr4;
            struct in6_addr addr6;
            
            /* Test IPv4 conversion */
            int result4 = ares_inet_pton(AF_INET, ip_str, &addr4);
            
            /* Test IPv6 conversion */
            int result6 = ares_inet_pton(AF_INET6, ip_str, &addr6);
            
            /* 4. Test ares_dns_rr_set_addr if IPv4 conversion succeeded */
            if (result4 == 1) {
                ares_status_t status2 = ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr4);
            }
            
            free(ip_str);
        }
    }
    
    /* Clean up - destroy the DNS record which will also clean up the resource record */
    ares_dns_record_destroy(dns_rec);
    
    return 0;
}
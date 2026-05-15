// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_set_addr at ares_dns_record.c:1103:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_library_init at ares_library_init.c:108:5 in ares.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_dns_rr_set_ttl at ares_dns_record.c:537:15 in ares_dns_record.h
// ares_dns_rr_get_ttl at ares_dns_record.c:529:14 in ares_dns_record.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "ares.h"
#include "ares_dns_record.h"

static void dummy_cleanup(void) {
    /* Placeholder for any cleanup if needed */
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    /* Initialize variables */
    ares_dns_rr_t *dns_rr = NULL;
    unsigned int ttl = 0;
    ares_status_t status;
    struct in_addr addr;
    int inet_result;
    char ip_str[16] = {0}; /* Buffer for IP address string */
    
    /* Since ares_dns_rr_t is opaque, we need to create it using library functions */
    /* First, create a DNS record */
    ares_dns_record_t *dns_record = NULL;
    
    /* Initialize the library if needed */
    ares_library_init(ARES_LIB_INIT_ALL);
    
    /* Create a DNS record - use random ID from fuzzer input */
    unsigned short id = 0;
    if (Size >= sizeof(unsigned short)) {
        memcpy(&id, Data, sizeof(unsigned short));
    }
    
    /* Create a DNS record with the given ID */
    status = ares_dns_record_create(&dns_record, id, ARES_FLAG_QR, 
                                    ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || dns_record == NULL) {
        ares_library_cleanup();
        return 0;
    }
    
    /* Add a resource record to the answer section */
    /* We'll use A record type which is common for IPv4 addresses */
    status = ares_dns_record_rr_add(&dns_rr, dns_record, ARES_SECTION_ANSWER, "example.com",
                                    ARES_REC_TYPE_A, ARES_CLASS_IN, 0);
    if (status != ARES_SUCCESS || dns_rr == NULL) {
        ares_dns_record_destroy(dns_record);
        ares_library_cleanup();
        return 0;
    }
    
    /* 1. Test ares_dns_rr_set_ttl */
    if (Size >= sizeof(unsigned int)) {
        /* Use first 4 bytes for TTL value */
        memcpy(&ttl, Data, sizeof(unsigned int));
    } else {
        /* Use partial data if available */
        ttl = 0;
        if (Size > 0) {
            memcpy(&ttl, Data, Size);
        }
    }
    
    status = ares_dns_rr_set_ttl(dns_rr, ttl);
    /* Check return status but continue regardless */
    
    /* 2. Test ares_dns_rr_get_ttl */
    unsigned int retrieved_ttl = ares_dns_rr_get_ttl(dns_rr);
    /* retrieved_ttl is ignored for fuzzing purposes */
    (void)retrieved_ttl; /* Avoid unused variable warning */
    
    /* 3. Test ares_inet_pton */
    /* Prepare IP address string from fuzzer input */
    if (Size > sizeof(unsigned int)) {
        size_t ip_str_len = Size - sizeof(unsigned int);
        if (ip_str_len > 15) ip_str_len = 15; /* IPv4 max length */
        
        /* Copy remaining data as IP string */
        memcpy(ip_str, Data + sizeof(unsigned int), ip_str_len);
        ip_str[ip_str_len] = '\0';
    } else {
        /* Use default IP if no data */
        strcpy(ip_str, "127.0.0.1");
    }
    
    /* Test with IPv4 */
    inet_result = ares_inet_pton(AF_INET, ip_str, &addr);
    /* Test with IPv6 if we have enough data */
    if (Size > sizeof(unsigned int) + 16) {
        struct in6_addr addr6;
        char ip6_str[40] = {0};
        size_t ip6_len = Size - (sizeof(unsigned int) + 16);
        if (ip6_len > 39) ip6_len = 39;
        memcpy(ip6_str, Data + sizeof(unsigned int) + 16, ip6_len);
        ip6_str[ip6_len] = '\0';
        ares_inet_pton(AF_INET6, ip6_str, &addr6);
    }
    
    /* 4. Test ares_dns_rr_set_addr */
    /* Only proceed if inet_pton was successful for IPv4 */
    if (inet_result == 1) {
        /* Use the correct key for A record address */
        status = ares_dns_rr_set_addr(dns_rr, ARES_RR_A_ADDR, &addr);
    }
    
    /* Cleanup */
    ares_dns_record_destroy(dns_record);
    ares_library_cleanup();
    dummy_cleanup();
    
    return 0;
}
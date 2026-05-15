// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_version at ares_version.c:29:13 in ares.h
// ares_version at ares_version.c:29:13 in ares.h
// ares_dns_rcode_tostr at ares_dns_mapping.c:910:13 in ares_dns_record.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_addr_to_ptr at ares_dns_record.c:1465:7 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_addr_to_ptr at ares_dns_record.c:1465:7 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    // 1. ares_version
    int version_num;
    const char *version_str = ares_version(&version_num);
    version_str = ares_version(NULL);
    
    // 2. ares_dns_rcode_tostr - generate rcode from input data
    if (Size > 0) {
        ares_dns_rcode_t rcode = (ares_dns_rcode_t)(Data[0] % 16); // Limit to reasonable range
        const char *rcode_str = ares_dns_rcode_tostr(rcode);
    }
    
    // 3. ares_dns_pton - prepare IP string from input data
    if (Size > 0) {
        // Create null-terminated IP string from input data
        char *ip_str = malloc(Size + 1);
        if (ip_str) {
            memcpy(ip_str, Data, Size);
            ip_str[Size] = '\0';
            
            struct ares_addr addr;
            size_t out_len;
            
            // Test with AF_UNSPEC
            addr.family = AF_UNSPEC;
            const void *result1 = ares_dns_pton(ip_str, &addr, &out_len);
            
            // Test with AF_INET
            addr.family = AF_INET;
            const void *result2 = ares_dns_pton(ip_str, &addr, &out_len);
            
            // Test with AF_INET6
            addr.family = AF_INET6;
            const void *result3 = ares_dns_pton(ip_str, &addr, &out_len);
            
            free(ip_str);
        }
    }
    
    // 4. ares_dns_addr_to_ptr - test with both IPv4 and IPv6 addresses
    struct ares_addr addr_ptr;
    
    // Test IPv4 address
    addr_ptr.family = AF_INET;
    addr_ptr.addr.addr4.s_addr = htonl(0x7F000001); // 127.0.0.1
    char *ptr_str4 = ares_dns_addr_to_ptr(&addr_ptr);
    if (ptr_str4) {
        ares_free_string(ptr_str4);
    }
    
    // Test IPv6 address
    addr_ptr.family = AF_INET6;
    uint8_t ipv6_addr[16] = {0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x00, 0x00,
                             0x00, 0x00, 0x8a, 0x2e, 0x03, 0x70, 0x73, 0x34};
    memcpy(&addr_ptr.addr.addr6, ipv6_addr, 16);
    char *ptr_str6 = ares_dns_addr_to_ptr(&addr_ptr);
    if (ptr_str6) {
        ares_free_string(ptr_str6);
    }
    
    // 5. ares_inet_pton - test with input data as IP string
    if (Size > 0) {
        char *ip_str2 = malloc(Size + 1);
        if (ip_str2) {
            memcpy(ip_str2, Data, Size);
            ip_str2[Size] = '\0';
            
            struct in_addr addr4;
            struct ares_in6_addr addr6;
            
            // Test IPv4
            int res1 = ares_inet_pton(AF_INET, ip_str2, &addr4);
            
            // Test IPv6
            int res2 = ares_inet_pton(AF_INET6, ip_str2, &addr6);
            
            free(ip_str2);
        }
    }
    
    // 6. ares_inet_ntop - test with binary addresses derived from input
    if (Size >= 4) {
        char dst[INET6_ADDRSTRLEN];
        
        // Test IPv4
        struct in_addr bin4;
        memcpy(&bin4, Data, 4);
        const char *ntop4 = ares_inet_ntop(AF_INET, &bin4, dst, sizeof(dst));
    }
    
    if (Size >= 16) {
        char dst[INET6_ADDRSTRLEN];
        
        // Test IPv6
        struct ares_in6_addr bin6;
        memcpy(&bin6, Data, 16);
        const char *ntop6 = ares_inet_ntop(AF_INET6, &bin6, dst, sizeof(dst));
    }
    
    return 0;
}
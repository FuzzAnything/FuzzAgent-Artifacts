// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_dns_rcode_tostr at ares_dns_mapping.c:910:13 in ares_dns_record.h
// ares_version at ares_version.c:29:13 in ares.h
// ares_version at ares_version.c:29:13 in ares.h
// ares_dns_addr_to_ptr at ares_dns_record.c:1465:7 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_addr_to_ptr at ares_dns_record.c:1465:7 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
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

static void generate_random_ipv4(uint8_t *data, size_t size, struct ares_addr *addr) {
    if (size >= 4) {
        memcpy(&addr->addr.addr4, data, 4);
        addr->family = AF_INET;
    } else {
        static const uint8_t default_ipv4[4] = {192, 168, 1, 1};
        memcpy(&addr->addr.addr4, default_ipv4, 4);
        addr->family = AF_INET;
    }
}

static void generate_random_ipv6(uint8_t *data, size_t size, struct ares_addr *addr) {
    if (size >= 16) {
        memcpy(&addr->addr.addr6, data, 16);
        addr->family = AF_INET6;
    } else {
        static const uint8_t default_ipv6[16] = {0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x00, 0x00, 
                                                  0x00, 0x00, 0x8a, 0x2e, 0x03, 0x70, 0x73, 0x34};
        memcpy(&addr->addr.addr6, default_ipv6, 16);
        addr->family = AF_INET6;
    }
}

static char* create_null_terminated_string(const uint8_t *data, size_t size, size_t max_len) {
    size_t len = (size < max_len) ? size : max_len;
    char *str = (char*)malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test ares_dns_rcode_tostr with various rcode values
    for (int rcode = 0; rcode < 16; rcode++) {
        const char *str = ares_dns_rcode_tostr((ares_dns_rcode_t)rcode);
        (void)str; // Consume result to avoid unused variable warning
    }
    
    // Test ares_version with both NULL and valid pointer
    const char *version_str = ares_version(NULL);
    (void)version_str;
    
    int version_num;
    version_str = ares_version(&version_num);
    (void)version_str;
    (void)version_num;
    
    // Test ares_dns_addr_to_ptr with IPv4 and IPv6 addresses
    struct ares_addr addr;
    
    // Test IPv4
    generate_random_ipv4(Data, Size, &addr);
    char *ptr_str_ipv4 = ares_dns_addr_to_ptr(&addr);
    if (ptr_str_ipv4) {
        ares_free_string(ptr_str_ipv4);
    }
    
    // Test IPv6 if we have enough data
    if (Size >= 16) {
        generate_random_ipv6(Data, Size, &addr);
        char *ptr_str_ipv6 = ares_dns_addr_to_ptr(&addr);
        if (ptr_str_ipv6) {
            ares_free_string(ptr_str_ipv6);
        }
    }
    
    // Test ares_dns_pton with different family settings
    char *ip_str = create_null_terminated_string(Data, Size, 64);
    if (ip_str) {
        size_t out_len;
        struct ares_addr pton_addr;
        
        // Test with AF_UNSPEC
        pton_addr.family = AF_UNSPEC;
        const void *result = ares_dns_pton(ip_str, &pton_addr, &out_len);
        (void)result;
        (void)out_len;
        
        // Test with AF_INET
        pton_addr.family = AF_INET;
        result = ares_dns_pton(ip_str, &pton_addr, &out_len);
        (void)result;
        (void)out_len;
        
        // Test with AF_INET6
        pton_addr.family = AF_INET6;
        result = ares_dns_pton(ip_str, &pton_addr, &out_len);
        (void)result;
        (void)out_len;
        
        free(ip_str);
    }
    
    // Test ares_inet_pton with different address families
    ip_str = create_null_terminated_string(Data, Size, 64);
    if (ip_str) {
        // Test IPv4
        struct in_addr dst4;
        int result4 = ares_inet_pton(AF_INET, ip_str, &dst4);
        (void)result4;
        
        // Test IPv6 if we have enough data for a valid address
        if (Size >= 16) {
            struct ares_in6_addr dst6;
            int result6 = ares_inet_pton(AF_INET6, ip_str, &dst6);
            (void)result6;
        }
        
        free(ip_str);
    }
    
    // Test ares_inet_ntop with both IPv4 and IPv6 addresses
    char dst_buffer[INET6_ADDRSTRLEN];
    
    // Test IPv4 ntop
    if (Size >= 4) {
        struct in_addr src4;
        memcpy(&src4, Data, (Size < 4) ? Size : 4);
        const char *ntop_result4 = ares_inet_ntop(AF_INET, &src4, dst_buffer, sizeof(dst_buffer));
        (void)ntop_result4;
    }
    
    // Test IPv6 ntop if we have enough data
    if (Size >= 16) {
        struct ares_in6_addr src6;
        memcpy(&src6, Data, 16);
        const char *ntop_result6 = ares_inet_ntop(AF_INET6, &src6, dst_buffer, sizeof(dst_buffer));
        (void)ntop_result6;
    }
    
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_dns_section_tostr at ares_dns_mapping.c:737:13 in ares_dns_record.h
// ares_version at ares_version.c:29:13 in ares.h
// ares_version at ares_version.c:29:13 in ares.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_addr_to_ptr at ares_dns_record.c:1465:7 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
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
#include <arpa/inet.h>

static void fuzz_ares_dns_addr_to_ptr(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    struct ares_addr addr;
    // Use first byte to determine family
    uint8_t family_selector = data[0] % 3; // 0=AF_INET, 1=AF_INET6, 2=invalid
    
    if (family_selector == 0) {
        addr.family = AF_INET;
        // Fill IPv4 address with available data
        size_t bytes_to_copy = (size - 1) < sizeof(addr.addr.addr4) ? (size - 1) : sizeof(addr.addr.addr4);
        if (bytes_to_copy > 0) {
            memcpy(&addr.addr.addr4, data + 1, bytes_to_copy);
        } else {
            memset(&addr.addr.addr4, 0, sizeof(addr.addr.addr4));
        }
    } else if (family_selector == 1) {
        addr.family = AF_INET6;
        // Fill IPv6 address with available data
        size_t bytes_to_copy = (size - 1) < sizeof(addr.addr.addr6) ? (size - 1) : sizeof(addr.addr.addr6);
        if (bytes_to_copy > 0) {
            memcpy(&addr.addr.addr6, data + 1, bytes_to_copy);
        } else {
            memset(&addr.addr.addr6, 0, sizeof(addr.addr.addr6));
        }
    } else {
        // Invalid family to test error handling
        addr.family = AF_UNSPEC;
    }
    
    char *result = ares_dns_addr_to_ptr(&addr);
    if (result) {
        ares_free_string(result);
    }
}

static void fuzz_ares_inet_pton(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Use first byte to determine address family
    int af;
    switch (data[0] % 3) {
        case 0: af = AF_INET; break;
        case 1: af = AF_INET6; break;
        default: af = AF_UNSPEC; break;
    }
    
    // Create null-terminated string from remaining data
    size_t str_len = size - 1;
    if (str_len > 255) str_len = 255; // Limit string length
    char *src = (char *)malloc(str_len + 1);
    if (!src) return;
    
    memcpy(src, data + 1, str_len);
    src[str_len] = '\0';
    
    void *dst;
    size_t dst_size;
    if (af == AF_INET) {
        dst_size = sizeof(struct in_addr);
    } else if (af == AF_INET6) {
        dst_size = sizeof(struct in6_addr);
    } else {
        dst_size = 0;
    }
    
    if (dst_size > 0) {
        dst = malloc(dst_size);
        if (dst) {
            ares_inet_pton(af, src, dst);
            free(dst);
        }
    }
    
    free(src);
}

static void fuzz_ares_inet_ntop(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Use first byte to determine address family
    int af;
    switch (data[0] % 2) {
        case 0: af = AF_INET; break;
        case 1: af = AF_INET6; break;
        default: af = AF_INET; break;
    }
    
    size_t src_size;
    if (af == AF_INET) {
        src_size = sizeof(struct in_addr);
    } else {
        src_size = sizeof(struct in6_addr);
    }
    
    if (size < 1 + src_size) return;
    
    void *src = malloc(src_size);
    if (!src) return;
    
    memcpy(src, data + 1, src_size);
    
    // Allocate destination buffer with reasonable size
    char dst[INET6_ADDRSTRLEN];
    ares_socklen_t dst_size = sizeof(dst);
    
    ares_inet_ntop(af, src, dst, dst_size);
    
    free(src);
}

static void fuzz_ares_dns_section_tostr(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Cast to enum type - let the function handle invalid values
    ares_dns_section_t section = (ares_dns_section_t)data[0];
    
    // Try a few more values by adding offsets
    for (int i = 0; i < 4 && i < (int)size; i++) {
        section = (ares_dns_section_t)(data[i] + i);
        const char *result = ares_dns_section_tostr(section);
        (void)result; // Use result to avoid unused variable warning
    }
}

static void fuzz_ares_version(const uint8_t *data, size_t size) {
    (void)data; // Unused parameter
    (void)size; // Unused parameter
    
    int version;
    const char *version_str = ares_version(&version);
    (void)version_str; // Use result
    
    // Also test without version pointer
    version_str = ares_version(NULL);
    (void)version_str;
}

static void fuzz_ares_dns_pton(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Create null-terminated string from data
    size_t str_len = size;
    if (str_len > 255) str_len = 255; // Limit string length
    char *ipaddr = (char *)malloc(str_len + 1);
    if (!ipaddr) return;
    
    memcpy(ipaddr, data, str_len);
    ipaddr[str_len] = '\0';
    
    struct ares_addr addr;
    size_t out_len;
    
    // Test with different family settings
    int families[] = {AF_UNSPEC, AF_INET, AF_INET6};
    for (int i = 0; i < 3; i++) {
        addr.family = families[i];
        const void *result = ares_dns_pton(ipaddr, &addr, &out_len);
        (void)result; // Use result
    }
    
    free(ipaddr);
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Seed random number generator with input data for diversity
    unsigned int seed = 0;
    for (size_t i = 0; i < Size && i < sizeof(seed); i++) {
        seed = (seed << 8) | Data[i];
    }
    srand(seed);
    
    // Call each API function with the fuzzing input
    fuzz_ares_dns_addr_to_ptr(Data, Size);
    fuzz_ares_inet_pton(Data, Size);
    fuzz_ares_inet_ntop(Data, Size);
    fuzz_ares_dns_section_tostr(Data, Size);
    fuzz_ares_version(Data, Size);
    fuzz_ares_dns_pton(Data, Size);
    
    return 0;
}
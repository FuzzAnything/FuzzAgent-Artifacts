// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_version at ares_version.c:29:13 in ares.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_addr_to_ptr at ares_dns_record.c:1465:7 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_addr_to_ptr at ares_dns_record.c:1465:7 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_dns_section_tostr at ares_dns_mapping.c:737:13 in ares_dns_record.h
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

static void fuzz_ares_dns_pton(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *ipaddr = (char *)malloc(size + 1);
    if (!ipaddr) return;
    
    memcpy(ipaddr, data, size);
    ipaddr[size] = '\0';
    
    struct ares_addr addr;
    size_t out_len;
    
    // Test with AF_UNSPEC
    addr.family = AF_UNSPEC;
    const void *result1 = ares_dns_pton(ipaddr, &addr, &out_len);
    // Result can be NULL on error
    
    // Test with AF_INET
    addr.family = AF_INET;
    const void *result2 = ares_dns_pton(ipaddr, &addr, &out_len);
    
    // Test with AF_INET6
    addr.family = AF_INET6;
    const void *result3 = ares_dns_pton(ipaddr, &addr, &out_len);
    
    free(ipaddr);
}

static void fuzz_ares_dns_addr_to_ptr(const uint8_t *data, size_t size) {
    struct ares_addr addr;
    
    if (size >= sizeof(struct in_addr)) {
        // Try IPv4
        addr.family = AF_INET;
        memcpy(&addr.addr.addr4, data, sizeof(struct in_addr));
        char *ptr = ares_dns_addr_to_ptr(&addr);
        if (ptr) {
            ares_free_string(ptr);
        }
    }
    
    if (size >= sizeof(struct ares_in6_addr)) {
        // Try IPv6
        addr.family = AF_INET6;
        memcpy(&addr.addr.addr6, data, sizeof(struct ares_in6_addr));
        char *ptr = ares_dns_addr_to_ptr(&addr);
        if (ptr) {
            ares_free_string(ptr);
        }
    }
}

static void fuzz_ares_inet_pton(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *src = (char *)malloc(size + 1);
    if (!src) return;
    
    memcpy(src, data, size);
    src[size] = '\0';
    
    char dst4[4];
    char dst6[16];
    
    // Test IPv4
    ares_inet_pton(AF_INET, src, dst4);
    
    // Test IPv6
    ares_inet_pton(AF_INET6, src, dst6);
    
    free(src);
}

static void fuzz_ares_dns_section_tostr(const uint8_t *data) {
    // Use first byte as section value
    ares_dns_section_t section = (ares_dns_section_t)*data;
    const char *str = ares_dns_section_tostr(section);
    (void)str; // Result can be used for debugging
}

static void fuzz_ares_inet_ntop(const uint8_t *data, size_t size) {
    char dst[INET6_ADDRSTRLEN];
    
    if (size >= 4) {
        // Test IPv4
        ares_inet_ntop(AF_INET, data, dst, sizeof(dst));
    }
    
    if (size >= 16) {
        // Test IPv6
        ares_inet_ntop(AF_INET6, data, dst, sizeof(dst));
    }
}

static void fuzz_ares_version(void) {
    int version;
    const char *ver_str = ares_version(&version);
    (void)ver_str; // Result can be used for debugging
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz ares_dns_pton
    fuzz_ares_dns_pton(Data, Size);
    
    // Fuzz ares_dns_addr_to_ptr
    fuzz_ares_dns_addr_to_ptr(Data, Size);
    
    // Fuzz ares_inet_pton
    fuzz_ares_inet_pton(Data, Size);
    
    // Fuzz ares_dns_section_tostr
    fuzz_ares_dns_section_tostr(Data);
    
    // Fuzz ares_inet_ntop
    fuzz_ares_inet_ntop(Data, Size);
    
    // Fuzz ares_version
    fuzz_ares_version();
    
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_version at ares_version.c:29:13 in ares.h
// ares_strerror at ares_strerror.c:30:13 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_dns_addr_to_ptr at ares_dns_record.c:1465:7 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
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

static void test_ares_dns_pton(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    struct ares_addr addr;
    size_t out_len;
    
    // Use first byte to determine address family
    int family_choice = data[0] % 3;
    if (family_choice == 0) {
        addr.family = AF_UNSPEC;
    } else if (family_choice == 1) {
        addr.family = AF_INET;
    } else {
        addr.family = AF_INET6;
    }
    
    // Create null-terminated string from remaining data
    char *ipaddr = (char *)malloc(size);
    if (!ipaddr) return;
    
    memcpy(ipaddr, data, size);
    ipaddr[size-1] = '\0';
    
    const void *result = ares_dns_pton(ipaddr, &addr, &out_len);
    (void)result; // Result may be NULL, that's okay for fuzzing
    
    free(ipaddr);
}

static void test_ares_version(void) {
    int version;
    const char *ver_str = ares_version(&version);
    (void)ver_str;
    (void)version;
}

static void test_ares_strerror(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Use data to generate error codes
    int code = (int)(data[0] % 256) - 128; // Range from -128 to 127
    const char *error_str = ares_strerror(code);
    (void)error_str;
}

static void test_ares_inet_ntop(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    int af = (data[0] % 2 == 0) ? AF_INET : AF_INET6;
    ares_socklen_t dst_size;
    char dst[INET6_ADDRSTRLEN];
    
    if (af == AF_INET) {
        if (size < 6) return; // Need at least 4 bytes for address + 2 for safety
        struct in_addr src;
        memcpy(&src, data + 1, sizeof(struct in_addr));
        dst_size = INET_ADDRSTRLEN;
        const char *result = ares_inet_ntop(af, &src, dst, dst_size);
        (void)result;
    } else {
        if (size < 18) return; // Need at least 16 bytes for address + 2 for safety
        struct ares_in6_addr src;
        memcpy(&src, data + 1, sizeof(struct ares_in6_addr));
        dst_size = INET6_ADDRSTRLEN;
        const char *result = ares_inet_ntop(af, &src, dst, dst_size);
        (void)result;
    }
}

static void test_ares_dns_addr_to_ptr(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    struct ares_addr addr;
    addr.family = (data[0] % 2 == 0) ? AF_INET : AF_INET6;
    
    if (addr.family == AF_INET) {
        if (size < 5) return; // Need 4 bytes for IPv4 address
        memcpy(&addr.addr.addr4, data + 1, sizeof(struct in_addr));
    } else {
        if (size < 17) return; // Need 16 bytes for IPv6 address
        memcpy(&addr.addr.addr6, data + 1, sizeof(struct ares_in6_addr));
    }
    
    char *ptr_str = ares_dns_addr_to_ptr(&addr);
    if (ptr_str) {
        ares_free_string(ptr_str);
    }
}

static void test_ares_inet_pton(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    int af = (data[0] % 2 == 0) ? AF_INET : AF_INET6;
    
    // Create null-terminated string from remaining data
    char *src = (char *)malloc(size);
    if (!src) return;
    
    memcpy(src, data, size);
    src[size-1] = '\0';
    
    if (af == AF_INET) {
        struct in_addr dst;
        int result = ares_inet_pton(af, src, &dst);
        (void)result;
    } else {
        struct ares_in6_addr dst;
        int result = ares_inet_pton(af, src, &dst);
        (void)result;
    }
    
    free(src);
}

int LLVMFuzzerTestOneInput_68(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Use first byte to determine which functions to test
    uint8_t selector = data[0];
    
    // Test all functions with different probabilities
    if ((selector & 0x01) != 0) {
        test_ares_dns_pton(data, size);
    }
    
    if ((selector & 0x02) != 0) {
        test_ares_version();
    }
    
    if ((selector & 0x04) != 0) {
        test_ares_strerror(data, size);
    }
    
    if ((selector & 0x08) != 0) {
        test_ares_inet_ntop(data, size);
    }
    
    if ((selector & 0x10) != 0) {
        test_ares_dns_addr_to_ptr(data, size);
    }
    
    if ((selector & 0x20) != 0) {
        test_ares_inet_pton(data, size);
    }
    
    return 0;
}
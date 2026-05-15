// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_strerror at ares_strerror.c:30:13 in ares.h
// ares_dns_addr_to_ptr at ares_dns_record.c:1465:7 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_addr_to_ptr at ares_dns_record.c:1465:7 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_version at ares_version.c:29:13 in ares.h
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

static void test_ares_dns_addr_to_ptr(const uint8_t *data, size_t size) {
    struct ares_addr addr;
    
    if (size < 1) return;
    
    // Use first byte to determine address family
    int family = (data[0] % 3);
    if (family == 0) {
        addr.family = AF_INET;
        if (size >= 1 + sizeof(struct in_addr)) {
            memcpy(&addr.addr.addr4, data + 1, sizeof(struct in_addr));
            char *ptr = ares_dns_addr_to_ptr(&addr);
            if (ptr) {
                ares_free_string(ptr);
            }
        }
    } else if (family == 1) {
        addr.family = AF_INET6;
        if (size >= 1 + sizeof(struct ares_in6_addr)) {
            memcpy(&addr.addr.addr6, data + 1, sizeof(struct ares_in6_addr));
            char *ptr = ares_dns_addr_to_ptr(&addr);
            if (ptr) {
                ares_free_string(ptr);
            }
        }
    }
}

static void test_ares_inet_pton(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    int af = (data[0] % 3);
    if (af == 0) af = AF_INET;
    else if (af == 1) af = AF_INET6;
    else af = AF_UNSPEC;
    
    // Create null-terminated string from remaining data
    char *src = (char*)malloc(size);
    if (!src) return;
    
    memcpy(src, data + 1, size - 1);
    src[size - 1] = '\0';
    
    void *dst;
    if (af == AF_INET) {
        dst = malloc(4);
        if (dst) {
            ares_inet_pton(af, src, dst);
            free(dst);
        }
    } else if (af == AF_INET6) {
        dst = malloc(16);
        if (dst) {
            ares_inet_pton(af, src, dst);
            free(dst);
        }
    }
    
    free(src);
}

static void test_ares_inet_ntop(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    int af = (data[0] % 2);
    af = (af == 0) ? AF_INET : AF_INET6;
    
    size_t addr_size = (af == AF_INET) ? 4 : 16;
    if (size < 1 + addr_size) return;
    
    void *src = malloc(addr_size);
    if (!src) return;
    
    memcpy(src, data + 1, addr_size);
    
    char dst[INET6_ADDRSTRLEN];
    ares_inet_ntop(af, src, dst, sizeof(dst));
    
    free(src);
}

static void test_ares_version(void) {
    int version;
    const char *ver_str = ares_version(&version);
    (void)ver_str;
    (void)version;
}

static void test_ares_dns_pton(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    struct ares_addr addr;
    addr.family = (data[0] % 3);
    if (addr.family == 0) addr.family = AF_UNSPEC;
    else if (addr.family == 1) addr.family = AF_INET;
    else addr.family = AF_INET6;
    
    // Create null-terminated string from remaining data
    char *ipaddr = (char*)malloc(size);
    if (!ipaddr) return;
    
    memcpy(ipaddr, data + 1, size - 1);
    ipaddr[size - 1] = '\0';
    
    size_t out_len;
    const void *result = ares_dns_pton(ipaddr, &addr, &out_len);
    (void)result;
    (void)out_len;
    
    free(ipaddr);
}

static void test_ares_strerror(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Generate error code from data
    int code = (int)data[0];
    // Try a range of possible error codes
    for (int i = -10; i <= 10; i++) {
        const char *err_str = ares_strerror(code + i);
        (void)err_str;
    }
}

int LLVMFuzzerTestOneInput_39(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Use first byte to determine which function to test
    uint8_t selector = data[0];
    
    switch (selector % 6) {
        case 0:
            test_ares_dns_addr_to_ptr(data, size);
            break;
        case 1:
            test_ares_inet_pton(data, size);
            break;
        case 2:
            test_ares_inet_ntop(data, size);
            break;
        case 3:
            test_ares_version();
            break;
        case 4:
            test_ares_dns_pton(data, size);
            break;
        case 5:
            test_ares_strerror(data, size);
            break;
    }
    
    return 0;
}
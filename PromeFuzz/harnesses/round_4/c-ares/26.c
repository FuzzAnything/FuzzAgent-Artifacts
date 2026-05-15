// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_strerror at ares_strerror.c:30:13 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static struct ares_addrinfo *create_dummy_addrinfo(const uint8_t *data, size_t size) {
    struct ares_addrinfo *ai = malloc(sizeof(struct ares_addrinfo));
    if (!ai) return NULL;
    
    ai->cnames = NULL;
    ai->nodes = NULL;
    
    if (size > 0) {
        size_t name_len = size > 256 ? 256 : size;
        ai->name = malloc(name_len + 1);
        if (ai->name) {
            memcpy(ai->name, data, name_len);
            ai->name[name_len] = '\0';
        }
    } else {
        ai->name = NULL;
    }
    
    return ai;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // 1. ares_strerror
    int error_code = Data[0] % 256;  // Use first byte for error code
    const char *error_str = ares_strerror(error_code);
    (void)error_str;  // Avoid unused variable warning
    
    // 2. ares_inet_ntop
    char dst[INET6_ADDRSTRLEN];
    const void *src;
    int af;
    ares_socklen_t addr_size;
    
    if (Size >= 20) {  // Enough for IPv6 address (16 bytes) + 4 bytes for parameters
        // Use second byte to determine address family
        if (Data[1] % 2 == 0) {
            af = AF_INET;
            src = Data + 2;
            addr_size = 4;
        } else {
            af = AF_INET6;
            src = Data + 2;
            addr_size = 16;
        }
        
        // Ensure we have enough data for the address
        if (Size >= 2 + addr_size) {
            const char *result = ares_inet_ntop(af, src, dst, sizeof(dst));
            (void)result;  // Avoid unused variable warning
        }
    }
    
    // 3. ares_freeaddrinfo
    struct ares_addrinfo *ai = create_dummy_addrinfo(Data, Size);
    if (ai) {
        ares_freeaddrinfo(ai);
    }
    
    return 0;
}
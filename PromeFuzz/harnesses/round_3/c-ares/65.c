// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_addr_to_ptr at ares_dns_record.c:1465:7 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_version at ares_version.c:29:13 in ares.h
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

static void dummy_cleanup(void) {
    // Placeholder for any cleanup if needed
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from fuzzer input for functions that need strings
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Use first byte to select which function to test
    uint8_t selector = Data[0] % 6;

    switch (selector) {
        case 0: {  // Test ares_dns_pton
            struct ares_addr addr;
            size_t out_len;
            
            // Try different address families
            int families[] = {AF_UNSPEC, AF_INET, AF_INET6};
            for (int i = 0; i < 3; i++) {
                addr.family = families[i];
                const void *result = ares_dns_pton(input_str, &addr, &out_len);
                // Result can be NULL on error, which is fine
                (void)result;
            }
            break;
        }
        
        case 1: {  // Test ares_version
            int version;
            const char *ver_str = ares_version(&version);
            (void)ver_str;
            // Also test without version pointer
            const char *ver_str2 = ares_version(NULL);
            (void)ver_str2;
            break;
        }
        
        case 2: {  // Test ares_strerror
            // Use input data to generate error codes
            int error_code = (int)(Data[0] % 256) - 128;  // Range -128 to 127
            const char *error_str = ares_strerror(error_code);
            (void)error_str;
            break;
        }
        
        case 3: {  // Test ares_inet_ntop
            char dst[46];  // Maximum size for IPv6 string
            ares_socklen_t dst_size = sizeof(dst);
            
            // Try with IPv4 binary data
            if (Size >= 4) {
                struct in_addr addr4;
                memcpy(&addr4, Data, 4);
                const char *result4 = ares_inet_ntop(AF_INET, &addr4, dst, dst_size);
                (void)result4;
            }
            
            // Try with IPv6 binary data
            if (Size >= 16) {
                struct ares_in6_addr addr6;
                memcpy(&addr6, Data, 16);
                const char *result6 = ares_inet_ntop(AF_INET6, &addr6, dst, dst_size);
                (void)result6;
            }
            break;
        }
        
        case 4: {  // Test ares_dns_addr_to_ptr
            struct ares_addr addr;
            
            // Try IPv4
            if (Size >= 4) {
                addr.family = AF_INET;
                memcpy(&addr.addr.addr4, Data, 4);
                char *ptr_str = ares_dns_addr_to_ptr(&addr);
                if (ptr_str) {
                    ares_free_string(ptr_str);
                }
            }
            
            // Try IPv6
            if (Size >= 16) {
                addr.family = AF_INET6;
                memcpy(&addr.addr.addr6, Data, 16);
                char *ptr_str = ares_dns_addr_to_ptr(&addr);
                if (ptr_str) {
                    ares_free_string(ptr_str);
                }
            }
            break;
        }
        
        case 5: {  // Test ares_inet_pton
            // Try IPv4
            struct in_addr dst4;
            int result4 = ares_inet_pton(AF_INET, input_str, &dst4);
            (void)result4;
            
            // Try IPv6
            struct ares_in6_addr dst6;
            int result6 = ares_inet_pton(AF_INET6, input_str, &dst6);
            (void)result6;
            
            // Try invalid family
            int result_invalid = ares_inet_pton(0, input_str, &dst4);
            (void)result_invalid;
            break;
        }
    }

    free(input_str);
    dummy_cleanup();
    return 0;
}
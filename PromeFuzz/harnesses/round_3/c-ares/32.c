// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_addr at ares_dns_record.c:757:23 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_addr_to_ptr at ares_dns_record.c:1465:7 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_addr_to_ptr at ares_dns_record.c:1465:7 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_addr at ares_dns_record.c:1103:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_addr6 at ares_dns_record.c:1121:15 in ares_dns_record.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get at ares_dns_record.c:474:16 in ares_dns_record.h
// ares_dns_rr_get_addr6 at ares_dns_record.c:774:29 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static void dummy_cleanup(void) {
    // No specific cleanup needed for this fuzzer
}

static struct ares_addr create_test_addr(const uint8_t *data, size_t size, int family) {
    struct ares_addr addr;
    addr.family = family;
    
    if (family == AF_INET && size >= 4) {
        memcpy(&addr.addr.addr4, data, 4);
    } else if (family == AF_INET6 && size >= 16) {
        memcpy(&addr.addr.addr6, data, 16);
    }
    
    return addr;
}

static ares_dns_record_t* create_test_dns_record(const uint8_t *data, size_t size, ares_dns_rec_type_t type) {
    ares_dns_record_t *dns_rec = NULL;
    
    // Create a minimal DNS record with QR flag set to 1 (response)
    if (ares_dns_record_create(&dns_rec, 0, 0x8000, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) != ARES_SUCCESS) {
        return NULL;
    }
    
    ares_dns_rr_t *rr = NULL;
    
    // Create a resource record based on type
    if (type == ARES_REC_TYPE_A) {
        if (ares_dns_record_rr_add(&rr, dns_rec, ARES_SECTION_ANSWER, 
                                   "example.com", ARES_REC_TYPE_A, ARES_CLASS_IN, 3600) == ARES_SUCCESS) {
            if (size >= 4) {
                struct in_addr addr4;
                memcpy(&addr4, data, 4);
                ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr4);
            }
        }
    } else if (type == ARES_REC_TYPE_AAAA) {
        if (ares_dns_record_rr_add(&rr, dns_rec, ARES_SECTION_ANSWER, 
                                   "example.com", ARES_REC_TYPE_AAAA, ARES_CLASS_IN, 3600) == ARES_SUCCESS) {
            if (size >= 16) {
                struct ares_in6_addr addr6;
                memcpy(&addr6, data, 16);
                ares_dns_rr_set_addr6(rr, ARES_RR_AAAA_ADDR, &addr6);
            }
        }
    }
    
    return dns_rec;
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Test ares_dns_pton
    {
        struct ares_addr addr;
        size_t out_len = 0;
        
        // Null-terminate the input for string functions
        char *ip_str = malloc(Size + 1);
        if (ip_str) {
            memcpy(ip_str, Data, Size);
            ip_str[Size] = '\0';
            
            // Test with different families
            addr.family = AF_UNSPEC;
            const void *result = ares_dns_pton(ip_str, &addr, &out_len);
            
            addr.family = AF_INET;
            ares_dns_pton(ip_str, &addr, &out_len);
            
            addr.family = AF_INET6;
            ares_dns_pton(ip_str, &addr, &out_len);
            
            free(ip_str);
        }
    }
    
    // Test ares_dns_rr_get_addr6
    {
        ares_dns_record_t *dns_rec = create_test_dns_record(Data, Size, ARES_REC_TYPE_AAAA);
        if (dns_rec) {
            // Get the first RR from the answer section
            size_t count = ares_dns_record_rr_cnt(dns_rec, ARES_SECTION_ANSWER);
            if (count > 0) {
                const ares_dns_rr_t *rr = ares_dns_record_rr_get(dns_rec, ARES_SECTION_ANSWER, 0);
                if (rr) {
                    const struct ares_in6_addr *addr6 = ares_dns_rr_get_addr6(rr, ARES_RR_AAAA_ADDR);
                }
            }
            ares_dns_record_destroy(dns_rec);
        }
    }
    
    // Test ares_inet_ntop
    {
        char dst[INET6_ADDRSTRLEN];
        
        if (Size >= 4) {
            // Test IPv4
            ares_inet_ntop(AF_INET, Data, dst, sizeof(dst));
        }
        
        if (Size >= 16) {
            // Test IPv6
            ares_inet_ntop(AF_INET6, Data, dst, sizeof(dst));
        }
    }
    
    // Test ares_dns_rr_get_addr
    {
        ares_dns_record_t *dns_rec = create_test_dns_record(Data, Size, ARES_REC_TYPE_A);
        if (dns_rec) {
            // Get the first RR from the answer section
            size_t count = ares_dns_record_rr_cnt(dns_rec, ARES_SECTION_ANSWER);
            if (count > 0) {
                const ares_dns_rr_t *rr = ares_dns_record_rr_get(dns_rec, ARES_SECTION_ANSWER, 0);
                if (rr) {
                    const struct in_addr *addr = ares_dns_rr_get_addr(rr, ARES_RR_A_ADDR);
                }
            }
            ares_dns_record_destroy(dns_rec);
        }
    }
    
    // Test ares_dns_addr_to_ptr
    {
        // Test with IPv4
        if (Size >= 4) {
            struct ares_addr addr4 = create_test_addr(Data, Size, AF_INET);
            char *ptr_str = ares_dns_addr_to_ptr(&addr4);
            if (ptr_str) {
                ares_free_string(ptr_str);
            }
        }
        
        // Test with IPv6
        if (Size >= 16) {
            struct ares_addr addr6 = create_test_addr(Data, Size, AF_INET6);
            char *ptr_str = ares_dns_addr_to_ptr(&addr6);
            if (ptr_str) {
                ares_free_string(ptr_str);
            }
        }
    }
    
    // Test ares_inet_pton
    {
        // Null-terminate the input for string functions
        char *ip_str = malloc(Size + 1);
        if (ip_str) {
            memcpy(ip_str, Data, Size);
            ip_str[Size] = '\0';
            
            if (Size >= 4) {
                struct in_addr dst4;
                ares_inet_pton(AF_INET, ip_str, &dst4);
            }
            
            if (Size >= 16) {
                struct ares_in6_addr dst6;
                ares_inet_pton(AF_INET6, ip_str, &dst6);
            }
            
            free(ip_str);
        }
    }
    
    dummy_cleanup();
    return 0;
}
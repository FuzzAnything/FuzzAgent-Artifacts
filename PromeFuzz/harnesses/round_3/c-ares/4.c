// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_get_addr at ares_dns_record.c:757:23 in ares_dns_record.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_dns_record_rr_get_const at ares_dns_record.c:499:3 in ares_dns_record.h
// ares_dns_rr_get_addr6 at ares_dns_record.c:774:29 in ares_dns_record.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_write at ares_dns_write.c:1193:15 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_addr at ares_dns_record.c:1103:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_addr6 at ares_dns_record.c:1121:15 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get_const at ares_dns_record.c:499:3 in ares_dns_record.h
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
    // Clean up any global state if needed
}

static ares_dns_record_t* create_test_dns_record(const uint8_t *data, size_t size) {
    // Create a minimal DNS record for testing
    ares_dns_record_t *dnsrec = NULL;
    
    // We'll create a simple query record
    if (ares_dns_record_create(&dnsrec, 0x1234, 0,  // Flags = 0 for query
                               ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) != ARES_SUCCESS) {
        return NULL;
    }
    
    // Add a question
    const char *name = "example.com";
    if (ares_dns_record_query_add(dnsrec, name, ARES_REC_TYPE_A, 
                                  ARES_CLASS_IN) != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return NULL;
    }
    
    // Add an answer with some data from the fuzzer input
    if (size >= 4) {
        struct in_addr addr;
        memcpy(&addr, data, 4);
        
        // Create an empty A record first
        ares_dns_rr_t *rr_a = NULL;
        if (ares_dns_record_rr_add(&rr_a, dnsrec, ARES_SECTION_ANSWER, "example.com",
                                   ARES_REC_TYPE_A, ARES_CLASS_IN, 300) == ARES_SUCCESS) {
            // Set the address for the A record
            ares_dns_rr_set_addr(rr_a, ARES_RR_A_ADDR, &addr);
        }
    }
    
    // Add an AAAA answer if we have enough data
    if (size >= 16) {
        struct ares_in6_addr addr6;
        memcpy(&addr6, data, 16);
        
        // Create an empty AAAA record first
        ares_dns_rr_t *rr_aaaa = NULL;
        if (ares_dns_record_rr_add(&rr_aaaa, dnsrec, ARES_SECTION_ANSWER, "example.com",
                                   ARES_REC_TYPE_AAAA, ARES_CLASS_IN, 300) == ARES_SUCCESS) {
            // Set the address for the AAAA record
            ares_dns_rr_set_addr6(rr_aaaa, ARES_RR_AAAA_ADDR, &addr6);
        }
    }
    
    return dnsrec;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    // 1. Test ares_dns_rr_key_datatype
    ares_dns_datatype_t dt = ares_dns_rr_key_datatype(ARES_RR_A_ADDR);
    dt = ares_dns_rr_key_datatype(ARES_RR_AAAA_ADDR);
    dt = ares_dns_rr_key_datatype(ARES_RR_NS_NSDNAME);
    
    // 2. Test ares_inet_ntop with IPv4
    char dst1[INET_ADDRSTRLEN];
    struct in_addr addr4;
    if (Size >= 4) {
        memcpy(&addr4, Data, 4);
        const char *result1 = ares_inet_ntop(AF_INET, &addr4, dst1, sizeof(dst1));
        (void)result1;
    }
    
    // Create a test DNS record
    ares_dns_record_t *dnsrec = create_test_dns_record(Data, Size);
    if (!dnsrec) {
        return 0;
    }
    
    // Get the first resource record from answers
    const ares_dns_rr_t *rr = NULL;
    size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
    if (rr_count > 0) {
        rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, 0);
    }
    
    if (rr) {
        // 3. Test ares_dns_rr_get_addr
        const struct in_addr *addr_ptr = ares_dns_rr_get_addr(rr, ARES_RR_A_ADDR);
        if (addr_ptr) {
            // 4. Test ares_inet_ntop again with the retrieved address
            char dst2[INET_ADDRSTRLEN];
            const char *result2 = ares_inet_ntop(AF_INET, addr_ptr, dst2, sizeof(dst2));
            (void)result2;
        }
        
        // Try to get AAAA record if available
        if (rr_count > 1) {
            const ares_dns_rr_t *rr_aaaa = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, 1);
            if (rr_aaaa) {
                // 5. Test ares_dns_rr_get_addr6
                const struct ares_in6_addr *addr6_ptr = ares_dns_rr_get_addr6(rr_aaaa, ARES_RR_AAAA_ADDR);
                if (addr6_ptr) {
                    char dst3[INET6_ADDRSTRLEN];
                    const char *result3 = ares_inet_ntop(AF_INET6, addr6_ptr, dst3, sizeof(dst3));
                    (void)result3;
                }
            }
        }
        
        // 6. Test ares_dns_rr_get_abin_cnt
        size_t abin_cnt = ares_dns_rr_get_abin_cnt(rr, ARES_RR_A_ADDR);
        (void)abin_cnt;
        
        // Try with different keys
        abin_cnt = ares_dns_rr_get_abin_cnt(rr, ARES_RR_AAAA_ADDR);
        abin_cnt = ares_dns_rr_get_abin_cnt(rr, ARES_RR_NS_NSDNAME);
    }
    
    // 7. Test ares_dns_write
    unsigned char *buf = NULL;
    size_t buf_len = 0;
    ares_status_t status = ares_dns_write(dnsrec, &buf, &buf_len);
    if (status == ARES_SUCCESS && buf) {
        // Free the buffer as required
        ares_free_string(buf);
    }
    
    // 8. Test ares_dns_record_destroy
    ares_dns_record_destroy(dnsrec);
    
    dummy_cleanup();
    return 0;
}
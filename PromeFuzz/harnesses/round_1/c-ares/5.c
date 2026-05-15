// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_query_add at ares_dns_record.c:252:15 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_dns_rr_get_addr at ares_dns_record.c:757:23 in ares_dns_record.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_dns_rr_get_addr6 at ares_dns_record.c:774:29 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_write at ares_dns_write.c:1193:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"
#include <arpa/inet.h>

static char *create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    fwrite(data, 1, size, fp);
    fclose(fp);
    return "./dummy_file";
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file for any potential file operations
    char *dummy_file = create_dummy_file(Data, Size);
    if (!dummy_file) return 0;
    
    // Initialize variables for API calls
    ares_dns_record_t *dnsrec = NULL;
    unsigned char *write_buf = NULL;
    size_t write_len = 0;
    char dst[INET6_ADDRSTRLEN];
    struct in_addr dummy_addr;
    struct ares_in6_addr dummy_addr6;
    
    // Initialize dummy addresses
    memset(&dummy_addr, 0, sizeof(dummy_addr));
    memset(&dummy_addr6, 0, sizeof(dummy_addr6));
    
    // Create a DNS record using the correct function signature
    ares_status_t status = ares_dns_record_create(&dnsrec, 0, 0, ARES_OPCODE_QUERY, 0);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Create a question using the correct function
        status = ares_dns_record_query_add(dnsrec, "example.com", ARES_REC_TYPE_A, ARES_CLASS_IN);
        
        // Try to get a resource record from the DNS record
        // We'll create a dummy RR structure for testing
        ares_dns_rr_t *dummy_rr = NULL;
        
        // 1. ares_dns_rr_key_datatype
        ares_dns_rr_key_t test_key = ARES_RR_A_ADDR;
        if (Size > 0) {
            test_key = (ares_dns_rr_key_t)(Data[0] % 100);
        }
        ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(test_key);
        
        // 2. First ares_inet_ntop (IPv4)
        const char *ntop_result1 = ares_inet_ntop(AF_INET, &dummy_addr, dst, sizeof(dst));
        
        // 3. ares_dns_rr_get_addr
        const struct in_addr *addr_result = NULL;
        if (dummy_rr) {
            addr_result = ares_dns_rr_get_addr(dummy_rr, test_key);
        }
        
        // 4. Second ares_inet_ntop (IPv6)
        const char *ntop_result2 = ares_inet_ntop(AF_INET6, &dummy_addr6, dst, sizeof(dst));
        
        // 5. ares_dns_rr_get_addr6
        const struct ares_in6_addr *addr6_result = NULL;
        if (dummy_rr) {
            addr6_result = ares_dns_rr_get_addr6(dummy_rr, test_key);
        }
        
        // 6. ares_dns_rr_get_abin_cnt
        size_t abin_cnt = 0;
        if (dummy_rr) {
            abin_cnt = ares_dns_rr_get_abin_cnt(dummy_rr, test_key);
        }
        
        // 7. ares_dns_write
        ares_status_t write_status = ares_dns_write(dnsrec, &write_buf, &write_len);
    }
    
    // 8. ares_dns_record_destroy
    ares_dns_record_destroy(dnsrec);
    
    // Cleanup allocated resources
    if (write_buf) {
        ares_free_string(write_buf);
    }
    
    cleanup_dummy_file();
    
    return 0;
}
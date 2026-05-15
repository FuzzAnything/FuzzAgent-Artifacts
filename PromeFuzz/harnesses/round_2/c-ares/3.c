// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_write at ares_dns_write.c:1193:15 in ares_dns_record.h
// ares_free_string at ares_free_string.c:30:6 in ares.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get_const at ares_dns_record.c:499:3 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_get_addr at ares_dns_record.c:757:23 in ares_dns_record.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_get_addr6 at ares_dns_record.c:774:29 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Create a DNS record using ares_dns_record_create
    ares_dns_record_t *dnsrec = NULL;
    
    // Use the first byte to determine record type
    ares_dns_rec_type_t rec_type = (Data[0] % 16) + 1; // Avoid 0
    
    // Create a DNS record with correct number of arguments
    ares_status_t status = ares_dns_record_create(&dnsrec, 
        (Data[1] & 0xFF),  // id
        0,                 // flags
        0,                 // opcode
        0                  // rcode
    );
    
    if (status != ARES_SUCCESS || !dnsrec) {
        return 0;
    }

    // Get the first resource record from the answer section (if any exist)
    const ares_dns_rr_t *rr = NULL;
    size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
    if (rr_count > 0) {
        rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, 0);
    }

    // Test ares_dns_rr_key_datatype
    // Use data to generate various key values
    ares_dns_rr_key_t test_key = (ares_dns_rr_key_t)(Data[0]);
    ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(test_key);
    (void)datatype;

    // Test ares_inet_ntop with IPv4
    char dst4[INET_ADDRSTRLEN];
    struct in_addr addr4;
    if (Size >= sizeof(struct in_addr)) {
        memcpy(&addr4, Data, sizeof(addr4));
    } else {
        memset(&addr4, 0, sizeof(addr4));
    }
    const char *result4 = ares_inet_ntop(AF_INET, &addr4, dst4, sizeof(dst4));
    (void)result4;

    // Test ares_dns_rr_get_addr if we have a valid RR
    const struct in_addr *addr_ptr = NULL;
    if (rr) {
        // Try to get address from A record key
        ares_dns_rr_key_t addr_key = ARES_RR_A_ADDR;
        if (ares_dns_rr_key_datatype(addr_key) == ARES_DATATYPE_INADDR) {
            addr_ptr = ares_dns_rr_get_addr(rr, addr_key);
        }
    }

    // Test ares_inet_ntop again with IPv6
    char dst6[INET6_ADDRSTRLEN];
    struct ares_in6_addr addr6;
    if (Size >= sizeof(struct ares_in6_addr)) {
        memcpy(&addr6, Data, sizeof(addr6));
    } else {
        memset(&addr6, 0, sizeof(addr6));
    }
    const char *result6 = ares_inet_ntop(AF_INET6, &addr6, dst6, sizeof(dst6));
    (void)result6;

    // Test ares_dns_rr_get_addr6 if we have a valid RR
    const struct ares_in6_addr *addr6_ptr = NULL;
    if (rr) {
        // Try to get IPv6 address from AAAA record key
        ares_dns_rr_key_t addr6_key = ARES_RR_AAAA_ADDR;
        if (ares_dns_rr_key_datatype(addr6_key) == ARES_DATATYPE_INADDR6) {
            addr6_ptr = ares_dns_rr_get_addr6(rr, addr6_key);
        }
    }

    // Test ares_dns_rr_get_abin_cnt with various keys
    size_t abin_cnt = 0;
    if (rr) {
        // Try keys that might be ABINP type
        ares_dns_rr_key_t keys[] = {ARES_RR_OPT_OPTIONS, ARES_RR_TXT_DATA};
        for (size_t i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
            if (ares_dns_rr_key_datatype(keys[i]) == ARES_DATATYPE_ABINP) {
                abin_cnt = ares_dns_rr_get_abin_cnt(rr, keys[i]);
                break;
            }
        }
    }

    // Test ares_dns_write
    unsigned char *buf = NULL;
    size_t buf_len = 0;
    ares_status_t write_status = ares_dns_write(dnsrec, &buf, &buf_len);
    if (write_status == ARES_SUCCESS && buf) {
        ares_free_string(buf);
    }

    // Clean up
    ares_dns_record_destroy(dnsrec);

    return 0;
}
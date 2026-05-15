// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_inet_pton at inet_net_pton.c:434:5 in ares.h
// ares_dns_rr_set_addr6 at ares_dns_record.c:1121:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static void dummy_cleanup(void) {
    // Placeholder for any cleanup if needed
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    // Initialize variables
    ares_dns_record_t *dnsrec = NULL;
    ares_dns_rr_t *rr1 = NULL;
    ares_dns_rr_t *rr2 = NULL;
    ares_status_t status;
    int inet_result;
    struct ares_in6_addr addr6;
    char ip_string[40] = {0};
    unsigned short u16_val;
    
    // Use fuzzer input to generate parameters
    if (Size < 10) {
        return 0;
    }
    
    // Extract values for ares_dns_record_create from fuzzer data
    unsigned short id = 0;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    if (Size >= 2) id = *(unsigned short*)Data;
    if (Size >= 4) flags = *(unsigned short*)(Data + 2);
    if (Size >= 5) opcode = (ares_dns_opcode_t)(Data[4] % 16); // Modulo to get valid opcode range
    if (Size >= 6) rcode = (ares_dns_rcode_t)(Data[5] % 16); // Modulo to get valid rcode range
    
    // Create a DNS record using library function with correct number of arguments
    status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || dnsrec == NULL) {
        return 0;
    }
    
    // Prepare name string from fuzzer data
    char name1[50] = {0};
    size_t name_len = Size > 50 ? 50 : Size;
    size_t data_offset = 6; // We used first 6 bytes for record creation params
    if (data_offset + name_len <= Size) {
        memcpy(name1, Data + data_offset, name_len);
        name1[name_len-1] = '\0';
    } else {
        strcpy(name1, "example.com");
    }
    
    // First call: ares_dns_record_rr_add
    status = ares_dns_record_rr_add(&rr1, dnsrec, ARES_SECTION_ANSWER,
                                    name1, ARES_REC_TYPE_AAAA, 
                                    ARES_CLASS_IN, 300);
    
    // Prepare IPv6 address string for ares_inet_pton
    size_t ip_str_len = Size > 40 ? 40 : Size/2;
    size_t ip_offset = data_offset + name_len;
    if (ip_offset + ip_str_len <= Size) {
        memcpy(ip_string, Data + ip_offset, ip_str_len);
        ip_string[ip_str_len-1] = '\0';
    } else {
        strcpy(ip_string, "::1");
    }
    
    // Second call: ares_inet_pton
    inet_result = ares_inet_pton(AF_INET6, ip_string, &addr6);
    
    // Third call: ares_dns_rr_set_addr6 (only if rr1 was created successfully)
    if (rr1) {
        status = ares_dns_rr_set_addr6(rr1, ARES_RR_AAAA_ADDR, &addr6);
    }
    
    // Prepare second name string
    char name2[50] = {0};
    size_t name2_offset = ip_offset + ip_str_len;
    if (name2_offset + 50 < Size) {
        memcpy(name2, Data + name2_offset, 50);
        name2[49] = '\0';
    } else {
        strcpy(name2, "example.com");
    }
    
    // Fourth call: ares_dns_record_rr_add (second time)
    status = ares_dns_record_rr_add(&rr2, dnsrec, ARES_SECTION_AUTHORITY,
                                    name2, ARES_REC_TYPE_SRV,
                                    ARES_CLASS_IN, 600);
    
    // Prepare u16 value from fuzzer data
    if (Size >= name2_offset + 52) {
        u16_val = *(unsigned short*)(Data + name2_offset + 50);
    } else {
        u16_val = 53;
    }
    
    // Fifth call: ares_dns_rr_set_u16 (only if rr2 was created successfully)
    if (rr2) {
        status = ares_dns_rr_set_u16(rr2, ARES_RR_SRV_PORT, u16_val);
    }
    
    // Cleanup
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    dummy_cleanup();
    return 0;
}
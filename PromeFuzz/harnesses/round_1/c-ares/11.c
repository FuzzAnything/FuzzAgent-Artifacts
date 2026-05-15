// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_u32 at ares_dns_record.c:1176:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include required headers in correct order */
#include "ares.h"
#include "ares_dns_record.h"

static void dummy_cleanup(ares_dns_record_t *dnsrec) {
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Create a DNS record first - needed for all operations
    ares_dns_record_t *dnsrec = NULL;
    unsigned short id = 0x1234;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || !dnsrec) {
        return 0;
    }

    // Prepare some default values for RR creation
    const char *name = "example.com";
    ares_dns_rec_type_t type = ARES_REC_TYPE_A;
    ares_dns_class_t rclass = ARES_CLASS_IN;
    unsigned int ttl = 300;
    
    // We'll create multiple RRs and manipulate them
    ares_dns_rr_t *rr1 = NULL;
    ares_dns_rr_t *rr2 = NULL;
    ares_dns_rr_t *rr3 = NULL;
    
    // Extract data for binary field
    size_t bin_len = Size > 100 ? 100 : Size;
    unsigned char *bin_data = NULL;
    if (bin_len > 0) {
        bin_data = (unsigned char *)malloc(bin_len);
        if (bin_data) {
            memcpy(bin_data, Data, bin_len);
        }
    }
    
    // First create a RR to work with
    status = ares_dns_record_rr_add(&rr1, dnsrec, ARES_SECTION_ANSWER, 
                                    name, ARES_REC_TYPE_TXT, rclass, ttl);
    if (status != ARES_SUCCESS) {
        dummy_cleanup(dnsrec);
        free(bin_data);
        return 0;
    }
    
    // 1. ares_dns_rr_set_bin
    if (bin_data) {
        ares_dns_rr_set_bin(rr1, ARES_RR_TXT_DATA, bin_data, bin_len);
    }
    
    // 2. ares_dns_record_rr_add
    status = ares_dns_record_rr_add(&rr2, dnsrec, ARES_SECTION_ANSWER, 
                                   name, ARES_REC_TYPE_CNAME, rclass, ttl);
    if (status != ARES_SUCCESS) {
        dummy_cleanup(dnsrec);
        free(bin_data);
        return 0;
    }
    
    // 3. ares_dns_rr_set_str on rr2
    ares_dns_rr_set_str(rr2, ARES_RR_CNAME_CNAME, "www.example.com");
    
    // 4. ares_dns_record_rr_add
    status = ares_dns_record_rr_add(&rr3, dnsrec, ARES_SECTION_ANSWER, 
                                   name, ARES_REC_TYPE_MX, rclass, ttl);
    if (status != ARES_SUCCESS) {
        dummy_cleanup(dnsrec);
        free(bin_data);
        return 0;
    }
    
    // 5. ares_dns_rr_set_str on rr3
    ares_dns_rr_set_str(rr3, ARES_RR_MX_EXCHANGE, "mail.example.com");
    
    // 6. ares_dns_record_rr_add
    ares_dns_rr_t *rr4 = NULL;
    status = ares_dns_record_rr_add(&rr4, dnsrec, ARES_SECTION_AUTHORITY, 
                                   "ns.example.com", ARES_REC_TYPE_NS, rclass, ttl);
    if (status != ARES_SUCCESS) {
        dummy_cleanup(dnsrec);
        free(bin_data);
        return 0;
    }
    
    // 7. ares_dns_rr_set_str on rr4
    ares_dns_rr_set_str(rr4, ARES_RR_NS_NSDNAME, "ns1.example.com");
    
    // 8. ares_dns_rr_set_str on rr4 again
    ares_dns_rr_set_str(rr4, ARES_RR_NS_NSDNAME, "ns2.example.com");
    
    // 9. ares_dns_rr_set_u32
    ares_dns_rr_t *rr5 = NULL;
    status = ares_dns_record_rr_add(&rr5, dnsrec, ARES_SECTION_AUTHORITY, 
                                   "example.com", ARES_REC_TYPE_SOA, rclass, ttl);
    if (status == ARES_SUCCESS) {
        unsigned int u32_val = 3600;
        if (Size >= sizeof(unsigned int)) {
            memcpy(&u32_val, Data, sizeof(unsigned int));
        }
        ares_dns_rr_set_u32(rr5, ARES_RR_SOA_REFRESH, u32_val);
    }
    
    // Clean up
    free(bin_data);
    dummy_cleanup(dnsrec);
    
    return 0;
}
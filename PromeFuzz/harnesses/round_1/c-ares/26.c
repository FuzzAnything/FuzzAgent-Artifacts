// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Include the necessary headers in the correct order
#include "ares.h"
#include "ares_dns_record.h"

static void initialize_dns_record(ares_dns_record_t **dnsrec) {
    // Create a DNS record with required parameters
    ares_status_t status = ares_dns_record_create(dnsrec, 1234, 0, 
                                                  ARES_OPCODE_QUERY, 
                                                  ARES_RCODE_NOERROR);
    // No need to set individual fields since they're set in create
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // Skip if input is too small
    if (Size < 10) return 0;
    
    ares_dns_record_t *dnsrec = NULL;
    ares_dns_rr_t *rr = NULL;
    ares_status_t status;
    
    // Step 1: Create and initialize DNS record
    initialize_dns_record(&dnsrec);
    if (!dnsrec) return 0;
    
    // Step 2: Create a resource record first (needed for other calls)
    // Using fuzzer data to create name string (null-terminated)
    size_t name_len = Size > 100 ? 100 : Size;
    char *name = malloc(name_len + 1);
    if (!name) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    memcpy(name, Data, name_len);
    name[name_len] = '\0';
    
    // Add resource record to get handle for set_str and set_opt
    status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                                   name, ARES_REC_TYPE_NS, ARES_CLASS_IN, 300);
    if (status != ARES_SUCCESS) {
        free(name);
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    // Step 3: Call ares_dns_rr_set_str
    // Use first part of data as string value
    size_t str_len = Size > 50 ? 50 : Size;
    char *str_val = malloc(str_len + 1);
    if (str_val) {
        memcpy(str_val, Data, str_len);
        str_val[str_len] = '\0';
        ares_dns_rr_set_str(rr, ARES_RR_NS_NSDNAME, str_val);
        free(str_val);
    }
    
    // Step 4: Call ares_dns_pton
    // Use middle part of data as IP string
    size_t ip_offset = Size / 3;
    size_t ip_len = Size > ip_offset + 20 ? 20 : Size - ip_offset;
    char *ip_str = malloc(ip_len + 1);
    if (ip_str) {
        memcpy(ip_str, Data + ip_offset, ip_len);
        ip_str[ip_len] = '\0';
        
        struct ares_addr addr;
        size_t out_len;
        addr.family = AF_UNSPEC;
        ares_dns_pton(ip_str, &addr, &out_len);
        free(ip_str);
    }
    
    // Step 5: First call to ares_dns_rr_set_opt
    // Use second part of data as option value
    size_t opt1_offset = Size * 2 / 3;
    size_t opt1_len = Size > opt1_offset + 10 ? 10 : Size - opt1_offset;
    unsigned char *opt1_val = malloc(opt1_len);
    if (opt1_val) {
        memcpy(opt1_val, Data + opt1_offset, opt1_len);
        ares_dns_rr_set_opt(rr, ARES_RR_OPT_OPTIONS, 1, opt1_val, opt1_len);
        free(opt1_val);
    }
    
    // Step 6: Second call to ares_dns_rr_set_opt (different key/opt)
    // Use last part of data as option value
    size_t opt2_offset = Size > 20 ? Size - 20 : 0;
    size_t opt2_len = Size - opt2_offset;
    unsigned char *opt2_val = malloc(opt2_len);
    if (opt2_val) {
        memcpy(opt2_val, Data + opt2_offset, opt2_len);
        ares_dns_rr_set_opt(rr, ARES_RR_SVCB_PARAMS, 2, opt2_val, opt2_len);
        free(opt2_val);
    }
    
    // Step 7: Call ares_dns_record_rr_add again with different parameters
    // Use remaining data for name
    size_t name2_len = Size > 30 ? 30 : Size;
    char *name2 = malloc(name2_len + 1);
    if (name2) {
        memcpy(name2, Data, name2_len);
        name2[name2_len] = '\0';
        
        // Try different record types and sections
        ares_dns_rr_t *rr2 = NULL;
        ares_dns_record_rr_add(&rr2, dnsrec, ARES_SECTION_AUTHORITY,
                              name2, ARES_REC_TYPE_MX, ARES_CLASS_IN, 600);
        free(name2);
    }
    
    // Cleanup
    free(name);
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
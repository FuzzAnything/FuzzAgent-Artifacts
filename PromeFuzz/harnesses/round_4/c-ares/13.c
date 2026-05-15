// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
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

/* Include required headers in the correct order */
#include "ares.h"
#include "ares_dns_record.h"

static char* create_null_terminated_string(const uint8_t* data, size_t size) {
    if (size == 0) {
        char* str = malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    char* str = malloc(size + 1);
    if (!str) return NULL;
    
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Create a DNS record first with required parameters
    ares_dns_record_t *dnsrec = NULL;
    unsigned short id = 0x1234;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    // Use fuzzer data to modify parameters if we have enough data
    if (Size >= 2) {
        id = (Data[0] << 8) | Data[1];
    }
    if (Size >= 4) {
        flags = (Data[2] << 8) | Data[3];
    }
    if (Size >= 5) {
        opcode = Data[4] % 16;  // Opcode is 4 bits
    }
    if (Size >= 6) {
        rcode = Data[5] % 16;  // Rcode is 4 bits
    }
    
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || !dnsrec) return 0;
    
    // Create resource record handles
    ares_dns_rr_t *rr1 = NULL;
    ares_dns_rr_t *rr2 = NULL;
    ares_dns_rr_t *rr3 = NULL;
    
    // Create null-terminated strings from fuzzer input
    char* str1 = create_null_terminated_string(Data, Size);
    char* str2 = create_null_terminated_string(Data, Size > 10 ? Size - 10 : Size);
    char* str3 = create_null_terminated_string(Data, Size > 20 ? Size - 20 : Size);
    char* str4 = create_null_terminated_string(Data, Size > 30 ? Size - 30 : Size);
    
    // 1. ares_dns_rr_set_bin
    // First we need a resource record to set bin on, so we'll create one
    status = ares_dns_record_rr_add(&rr1, dnsrec, ARES_SECTION_ANSWER, 
                                                  "example.com", ARES_REC_TYPE_A, 
                                                  ARES_CLASS_IN, 300);
    if (status == ARES_SUCCESS && rr1) {
        // Use first part of data for binary
        size_t bin_len = Size > 100 ? 100 : Size;
        // For A record, the data field is at offset 100 (1 * 100)
        ares_dns_rr_set_bin(rr1, 100, Data, bin_len);
    }
    
    // 2. ares_dns_record_rr_add
    status = ares_dns_record_rr_add(&rr2, dnsrec, ARES_SECTION_AUTHORITY,
                                    "ns.example.com", ARES_REC_TYPE_NS,
                                    ARES_CLASS_IN, 600);
    
    // 3. ares_dns_rr_set_str
    if (status == ARES_SUCCESS && rr2 && str1) {
        // For NS record, the nsdname field is at offset 200 (2 * 100)
        ares_dns_rr_set_str(rr2, 200, str1);
    }
    
    // 4. ares_dns_record_rr_add
    status = ares_dns_record_rr_add(&rr3, dnsrec, ARES_SECTION_ADDITIONAL,
                                    "mail.example.com", ARES_REC_TYPE_MX,
                                    ARES_CLASS_IN, 900);
    
    // 5. ares_dns_rr_set_str
    if (status == ARES_SUCCESS && rr3 && str2) {
        // For MX record, the exchange field is at offset 1500 (15 * 100)
        ares_dns_rr_set_str(rr3, 1500, str2);
    }
    
    // 6. ares_dns_record_rr_add (adding another RR)
    ares_dns_rr_t *rr4 = NULL;
    status = ares_dns_record_rr_add(&rr4, dnsrec, ARES_SECTION_ANSWER,
                                    "www.example.com", ARES_REC_TYPE_CNAME,
                                    ARES_CLASS_IN, 300);
    
    // 7. ares_dns_rr_set_str
    if (status == ARES_SUCCESS && rr4 && str3) {
        // For CNAME record, the cname field is at offset 500 (5 * 100)
        ares_dns_rr_set_str(rr4, 500, str3);
    }
    
    // 8. ares_dns_rr_set_str (on same RR with different key if available)
    if (rr4 && str4) {
        // Try with a different string key if available for CNAME
        // CNAME only has one string field, so we'll use the same key
        // or create another RR of different type
        ares_dns_rr_t *rr5 = NULL;
        status = ares_dns_record_rr_add(&rr5, dnsrec, ARES_SECTION_ANSWER,
                                        "ftp.example.com", ARES_REC_TYPE_PTR,
                                        ARES_CLASS_IN, 300);
        if (status == ARES_SUCCESS && rr5 && str4) {
            // For PTR record, the dname field is at offset 1200 (12 * 100)
            ares_dns_rr_set_str(rr5, 1200, str4);
            
            // 9. ares_dns_rr_set_u32
            // Find a key with U32 datatype - using SOA record which has serial
            ares_dns_rr_t *rr6 = NULL;
            status = ares_dns_record_rr_add(&rr6, dnsrec, ARES_SECTION_AUTHORITY,
                                            "example.com", ARES_REC_TYPE_SOA,
                                            ARES_CLASS_IN, 3600);
            if (status == ARES_SUCCESS && rr6) {
                // Use first 4 bytes as u32 value
                unsigned int u32_val = 0;
                if (Size >= 4) {
                    u32_val = *(unsigned int*)Data;
                } else if (Size > 0) {
                    // Use available bytes
                    memcpy(&u32_val, Data, Size);
                }
                // For SOA record, the serial field is at offset 600 (6 * 100)
                ares_dns_rr_set_u32(rr6, 600, u32_val);
            }
        }
    }
    
    // Cleanup
    if (str1) free(str1);
    if (str2) free(str2);
    if (str3) free(str3);
    if (str4) free(str4);
    
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    return 0;
}
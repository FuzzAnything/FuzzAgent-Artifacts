// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static char *dummy_name = "example.com";

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Create a DNS record using ares_dns_record_create with correct number of arguments
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS || !dnsrec) {
        return 0;
    }
    
    // First ares_dns_rr_set_str - we need a resource record first
    ares_dns_rr_t *rr1 = NULL;
    status = ares_dns_record_rr_add(&rr1, dnsrec, ARES_SECTION_ANSWER, 
                                     dummy_name, ARES_REC_TYPE_A, 
                                     ARES_CLASS_IN, 3600);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    // Now we can call the functions in the specified order
    // 1. ares_dns_rr_set_str
    // For A record, valid string key could be ARES_RR_A_ADDR (which is actually binary)
    // Let's try with a dummy string
    char str_val[256] = {0};
    if (Size > 0) {
        size_t str_len = Size > 255 ? 255 : Size;
        memcpy(str_val, Data, str_len);
        str_val[str_len] = '\0';
    } else {
        strcpy(str_val, "dummy");
    }
    ares_dns_rr_set_str(rr1, ARES_RR_A_ADDR, str_val);
    
    // 2. ares_dns_record_rr_add
    ares_dns_rr_t *rr2 = NULL;
    ares_dns_record_rr_add(&rr2, dnsrec, ARES_SECTION_AUTHORITY, 
                           dummy_name, ARES_REC_TYPE_NS, 
                           ARES_CLASS_IN, 7200);
    
    // 3. ares_dns_rr_set_str (on rr2)
    if (rr2) {
        ares_dns_rr_set_str(rr2, ARES_RR_NS_NSDNAME, "ns.example.com");
    }
    
    // 4. ares_dns_record_rr_add
    ares_dns_rr_t *rr3 = NULL;
    ares_dns_record_rr_add(&rr3, dnsrec, ARES_SECTION_ADDITIONAL, 
                           dummy_name, ARES_REC_TYPE_TXT, 
                           ARES_CLASS_IN, 0);
    
    // 5. ares_dns_rr_add_abin (on rr3 if TXT record)
    if (rr3 && Size > 0) {
        // TXT records have ARES_RR_TXT_DATA which is ABINP type
        size_t bin_len = Size > 100 ? 100 : Size;
        ares_dns_rr_add_abin(rr3, ARES_RR_TXT_DATA, Data, bin_len);
    }
    
    // 6. ares_dns_rr_add_abin (second time on rr3)
    if (rr3 && Size > 10) {
        size_t offset = 10;
        // Ensure we don't read past the buffer
        size_t remaining = Size - offset;
        size_t bin_len = remaining > 50 ? 50 : remaining;
        if (bin_len > 0) {
            ares_dns_rr_add_abin(rr3, ARES_RR_TXT_DATA, Data + offset, bin_len);
        }
    }
    
    // 7. ares_dns_record_rr_add
    ares_dns_rr_t *rr4 = NULL;
    ares_dns_record_rr_add(&rr4, dnsrec, ARES_SECTION_ANSWER, 
                           dummy_name, ARES_REC_TYPE_MX, 
                           ARES_CLASS_IN, 3600);
    
    // 8. ares_dns_rr_set_u16 (on rr4 if MX record)
    if (rr4) {
        unsigned short u16_val = Size > 0 ? (Data[0] << 8) | (Size > 1 ? Data[1] : 0) : 10;
        ares_dns_rr_set_u16(rr4, ARES_RR_MX_PREFERENCE, u16_val);
    }
    
    // Cleanup
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
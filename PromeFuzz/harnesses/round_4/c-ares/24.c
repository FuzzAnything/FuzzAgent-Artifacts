// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include required headers */
#include "ares.h"
#include "ares_dns_record.h"

static char* create_null_terminated_string(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset >= Size) {
        char* str = malloc(12);
        if (str) strcpy(str, "example.com");
        return str;
    }
    
    size_t max_len = Size - *offset;
    size_t len = max_len > 100 ? 100 : max_len;
    
    char* str = malloc(len + 1);
    if (!str) {
        *offset += len;
        char* default_str = malloc(12);
        if (default_str) strcpy(default_str, "example.com");
        return default_str;
    }
    
    memcpy(str, Data + *offset, len);
    str[len] = '\0';
    *offset += len;
    
    return str;
}

static unsigned short get_fuzz_u16(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + 1 >= Size) {
        return 12345;
    }
    
    unsigned short val = (Data[*offset] << 8) | Data[*offset + 1];
    *offset += 2;
    return val;
}

static const unsigned char* get_fuzz_bin(const uint8_t *Data, size_t Size, size_t *offset, size_t *len) {
    if (*offset >= Size) {
        static const unsigned char dummy[] = {0x01, 0x02, 0x03};
        *len = 3;
        return dummy;
    }
    
    size_t max_len = Size - *offset;
    *len = max_len > 50 ? 50 : max_len;
    const unsigned char* bin = Data + *offset;
    *offset += *len;
    return bin;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    size_t offset = 0;
    
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status;
    
    status = ares_dns_record_create(&dnsrec, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS) {
        return 0;
    }
    
    ares_dns_rr_t *rr1 = NULL;
    ares_dns_rr_t *rr2 = NULL;
    ares_dns_rr_t *rr3 = NULL;
    
    char* str1 = create_null_terminated_string(Data, Size, &offset);
    
    status = ares_dns_record_rr_add(&rr1, dnsrec, ARES_SECTION_ANSWER, 
                                   "test1.example.com", ARES_REC_TYPE_A, 
                                   ARES_CLASS_IN, 300);
    
    if (status == ARES_SUCCESS && rr1 != NULL && str1 != NULL) {
        ares_dns_rr_set_str(rr1, ARES_RR_A_ADDR, str1);
    }
    free(str1);
    
    char* str2 = create_null_terminated_string(Data, Size, &offset);
    
    status = ares_dns_record_rr_add(&rr2, dnsrec, ARES_SECTION_AUTHORITY,
                                   "test2.example.com", ARES_REC_TYPE_TXT,
                                   ARES_CLASS_IN, 600);
    
    if (status == ARES_SUCCESS && rr2 != NULL && str2 != NULL) {
        ares_dns_rr_set_str(rr2, ARES_RR_TXT_DATA, str2);
    }
    free(str2);
    
    size_t bin_len1;
    const unsigned char* bin1 = get_fuzz_bin(Data, Size, &offset, &bin_len1);
    if (rr2 != NULL) {
        ares_dns_rr_add_abin(rr2, ARES_RR_TXT_DATA, bin1, bin_len1);
    }
    
    size_t bin_len2;
    const unsigned char* bin2 = get_fuzz_bin(Data, Size, &offset, &bin_len2);
    if (rr2 != NULL) {
        ares_dns_rr_add_abin(rr2, ARES_RR_TXT_DATA, bin2, bin_len2);
    }
    
    status = ares_dns_record_rr_add(&rr3, dnsrec, ARES_SECTION_ADDITIONAL,
                                   "test3.example.com", ARES_REC_TYPE_MX,
                                   ARES_CLASS_IN, 1200);
    
    unsigned short u16_val = get_fuzz_u16(Data, Size, &offset);
    if (status == ARES_SUCCESS && rr3 != NULL) {
        ares_dns_rr_set_u16(rr3, ARES_RR_MX_PREFERENCE, u16_val);
    }
    
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_rr_set_u32 at ares_dns_record.c:1176:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static ares_dns_record_t *create_dns_record(void) {
    ares_dns_record_t *dnsrec = NULL;
    /* Create a DNS record with minimal valid parameters */
    if (ares_dns_record_create(&dnsrec, 12345, ARES_FLAG_QR, 
                               ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) != ARES_SUCCESS) {
        return NULL;
    }
    return dnsrec;
}

static char *extract_string(const uint8_t **data, size_t *size) {
    if (*size == 0) {
        char *empty = malloc(1);
        if (empty) {
            empty[0] = '\0';
        }
        return empty;
    }
    
    /* Find null terminator within remaining data */
    size_t len = 0;
    while (len < *size && (*data)[len] != 0) {
        len++;
    }
    
    /* Allocate and copy string */
    char *str = malloc(len + 1);
    if (!str) {
        return NULL;
    }
    
    memcpy(str, *data, len);
    str[len] = '\0';
    
    /* Advance data pointer */
    *data += (len < *size) ? len + 1 : len;
    *size -= (len < *size) ? len + 1 : len;
    
    return str;
}

static unsigned int extract_u32(const uint8_t **data, size_t *size) {
    if (*size >= 4) {
        unsigned int val = 0;
        memcpy(&val, *data, 4);
        *data += 4;
        *size -= 4;
        return val;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    /* Create DNS record */
    ares_dns_record_t *dnsrec = create_dns_record();
    if (!dnsrec) {
        return 0;
    }
    
    const uint8_t *ptr = Data;
    size_t remaining = Size;
    
    /* First RR: Set binary data */
    ares_dns_rr_t *rr1 = NULL;
    ares_status_t status = ares_dns_record_rr_add(&rr1, dnsrec, ARES_SECTION_ANSWER,
                                                  "example.com", ARES_REC_TYPE_A,
                                                  ARES_CLASS_IN, 3600);
    if (status == ARES_SUCCESS && rr1) {
        /* Extract binary data from fuzzer input */
        size_t bin_len = (remaining > 0) ? (ptr[0] % 256) : 0;
        if (bin_len > remaining - 1) {
            bin_len = (remaining > 1) ? remaining - 1 : 0;
        }
        
        if (bin_len > 0 && remaining > 1) {
            ares_dns_rr_set_bin(rr1, ARES_RR_A_ADDR, ptr + 1, bin_len);
            ptr += 1 + bin_len;
            remaining -= 1 + bin_len;
        }
    }
    
    /* Second RR: Set string data */
    ares_dns_rr_t *rr2 = NULL;
    status = ares_dns_record_rr_add(&rr2, dnsrec, ARES_SECTION_ANSWER,
                                    "test.com", ARES_REC_TYPE_NS,
                                    ARES_CLASS_IN, 7200);
    if (status == ARES_SUCCESS && rr2) {
        char *str1 = extract_string(&ptr, &remaining);
        if (str1) {
            ares_dns_rr_set_str(rr2, ARES_RR_NS_NSDNAME, str1);
            free(str1);
        }
    }
    
    /* Third RR: Set multiple string fields and a u32 */
    ares_dns_rr_t *rr3 = NULL;
    status = ares_dns_record_rr_add(&rr3, dnsrec, ARES_SECTION_ADDITIONAL,
                                    "mx.example.com", ARES_REC_TYPE_MX,
                                    ARES_CLASS_IN, 1800);
    if (status == ARES_SUCCESS && rr3) {
        /* First string */
        char *str2 = extract_string(&ptr, &remaining);
        if (str2) {
            ares_dns_rr_set_str(rr3, ARES_RR_MX_EXCHANGE, str2);
            free(str2);
        }
        
        /* Second string */
        char *str3 = extract_string(&ptr, &remaining);
        if (str3) {
            ares_dns_rr_set_str(rr3, ARES_RR_MX_EXCHANGE, str3);
            free(str3);
        }
        
        /* Third string */
        char *str4 = extract_string(&ptr, &remaining);
        if (str4) {
            ares_dns_rr_set_str(rr3, ARES_RR_MX_EXCHANGE, str4);
            free(str4);
        }
        
        /* Set u32 */
        unsigned int u32_val = extract_u32(&ptr, &remaining);
        ares_dns_rr_set_u32(rr3, ARES_RR_MX_PREFERENCE, u32_val);
    }
    
    /* Cleanup */
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_u16 at ares_dns_record.c:1158:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CARES_EXTERN if not defined */
#ifndef CARES_EXTERN
#define CARES_EXTERN extern
#endif

/* Include necessary headers for missing types */
#include "ares.h"
#include "ares_dns_record.h"

#define MAX_STR_LEN 100
#define MAX_BIN_LEN 100

static void read_string(const uint8_t **data, size_t *size, char *buf, size_t buf_size) {
    size_t i = 0;
    while (i < buf_size - 1 && *size > 0) {
        buf[i] = (char)**data;
        (*data)++;
        (*size)--;
        if (buf[i] == '\0') break;
        i++;
    }
    buf[i] = '\0';
}

static unsigned short read_u16(const uint8_t **data, size_t *size) {
    unsigned short val = 0;
    if (*size >= 2) {
        val = (unsigned short)((*data)[0] << 8) | (*data)[1];
        *data += 2;
        *size -= 2;
    }
    return val;
}

static unsigned int read_u32(const uint8_t **data, size_t *size) {
    unsigned int val = 0;
    if (*size >= 4) {
        val = (unsigned int)((*data)[0] << 24) | ((*data)[1] << 16) | ((*data)[2] << 8) | (*data)[3];
        *data += 4;
        *size -= 4;
    }
    return val;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    const uint8_t *ptr = Data;
    size_t remaining = Size;
    
    // Read parameters for DNS record creation
    unsigned short id = read_u16(&ptr, &remaining);
    unsigned short flags = read_u16(&ptr, &remaining);
    ares_dns_opcode_t opcode = (ares_dns_opcode_t)(read_u16(&ptr, &remaining) % 16);
    ares_dns_rcode_t rcode = (ares_dns_rcode_t)(read_u16(&ptr, &remaining) % 16);
    
    // Create DNS record
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || !dnsrec) return 0;
    
    // Prepare RR pointers
    ares_dns_rr_t *rr1 = NULL;
    ares_dns_rr_t *rr2 = NULL;
    ares_dns_rr_t *rr3 = NULL;
    ares_dns_rr_t *rr4 = NULL;
    
    // Read parameters from fuzzer input
    char name1[MAX_STR_LEN], name2[MAX_STR_LEN], name3[MAX_STR_LEN], name4[MAX_STR_LEN];
    char str1[MAX_STR_LEN], str2[MAX_STR_LEN];
    
    // Parameters for first RR
    ares_dns_section_t sect1 = (ares_dns_section_t)(read_u16(&ptr, &remaining) % 4);
    read_string(&ptr, &remaining, name1, MAX_STR_LEN);
    ares_dns_rec_type_t type1 = (ares_dns_rec_type_t)(read_u16(&ptr, &remaining) % 256);
    ares_dns_class_t rclass1 = (ares_dns_class_t)(read_u16(&ptr, &remaining) % 4);
    unsigned int ttl1 = read_u32(&ptr, &remaining);
    
    // Parameters for second RR
    ares_dns_section_t sect2 = (ares_dns_section_t)(read_u16(&ptr, &remaining) % 4);
    read_string(&ptr, &remaining, name2, MAX_STR_LEN);
    ares_dns_rec_type_t type2 = (ares_dns_rec_type_t)(read_u16(&ptr, &remaining) % 256);
    ares_dns_class_t rclass2 = (ares_dns_class_t)(read_u16(&ptr, &remaining) % 4);
    unsigned int ttl2 = read_u32(&ptr, &remaining);
    
    // Parameters for third RR
    ares_dns_section_t sect3 = (ares_dns_section_t)(read_u16(&ptr, &remaining) % 4);
    read_string(&ptr, &remaining, name3, MAX_STR_LEN);
    ares_dns_rec_type_t type3 = (ares_dns_rec_type_t)(read_u16(&ptr, &remaining) % 256);
    ares_dns_class_t rclass3 = (ares_dns_class_t)(read_u16(&ptr, &remaining) % 4);
    unsigned int ttl3 = read_u32(&ptr, &remaining);
    
    // Parameters for fourth RR
    ares_dns_section_t sect4 = (ares_dns_section_t)(read_u16(&ptr, &remaining) % 4);
    read_string(&ptr, &remaining, name4, MAX_STR_LEN);
    ares_dns_rec_type_t type4 = (ares_dns_rec_type_t)(read_u16(&ptr, &remaining) % 256);
    ares_dns_class_t rclass4 = (ares_dns_class_t)(read_u16(&ptr, &remaining) % 4);
    unsigned int ttl4 = read_u32(&ptr, &remaining);
    
    // String values
    read_string(&ptr, &remaining, str1, MAX_STR_LEN);
    read_string(&ptr, &remaining, str2, MAX_STR_LEN);
    
    // Binary data
    size_t bin1_len = remaining > MAX_BIN_LEN ? MAX_BIN_LEN : remaining;
    const unsigned char *bin1_data = ptr;
    ptr += bin1_len;
    remaining -= bin1_len;
    
    size_t bin2_len = remaining > MAX_BIN_LEN ? MAX_BIN_LEN : remaining;
    const unsigned char *bin2_data = ptr;
    ptr += bin2_len;
    remaining -= bin2_len;
    
    // Keys (use modulo to get valid-ish values)
    ares_dns_rr_key_t key1 = (ares_dns_rr_key_t)(read_u16(&ptr, &remaining) % 1000);
    ares_dns_rr_key_t key2 = (ares_dns_rr_key_t)(read_u16(&ptr, &remaining) % 1000);
    ares_dns_rr_key_t key3 = (ares_dns_rr_key_t)(read_u16(&ptr, &remaining) % 1000);
    ares_dns_rr_key_t key4 = (ares_dns_rr_key_t)(read_u16(&ptr, &remaining) % 1000);
    ares_dns_rr_key_t key5 = (ares_dns_rr_key_t)(read_u16(&ptr, &remaining) % 1000);
    
    // u16 value
    unsigned short u16_val = read_u16(&ptr, &remaining);
    
    // First: add RR1
    ares_dns_record_rr_add(&rr1, dnsrec, sect1, name1, type1, rclass1, ttl1);
    
    // Second: set string on RR1
    if (rr1) {
        ares_dns_rr_set_str(rr1, key1, str1);
    }
    
    // Third: add RR2
    ares_dns_record_rr_add(&rr2, dnsrec, sect2, name2, type2, rclass2, ttl2);
    
    // Fourth: set string on RR2
    if (rr2) {
        ares_dns_rr_set_str(rr2, key2, str2);
    }
    
    // Fifth: add RR3
    ares_dns_record_rr_add(&rr3, dnsrec, sect3, name3, type3, rclass3, ttl3);
    
    // Sixth: add binary to RR3 (first)
    if (rr3) {
        ares_dns_rr_add_abin(rr3, key3, bin1_data, bin1_len);
    }
    
    // Seventh: add binary to RR3 (second)
    if (rr3) {
        ares_dns_rr_add_abin(rr3, key4, bin2_data, bin2_len);
    }
    
    // Eighth: add RR4
    ares_dns_record_rr_add(&rr4, dnsrec, sect4, name4, type4, rclass4, ttl4);
    
    // Ninth: set u16 on RR4
    if (rr4) {
        ares_dns_rr_set_u16(rr4, key5, u16_val);
    }
    
    // Cleanup
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
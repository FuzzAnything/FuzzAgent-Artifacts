// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
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

static const char *get_string_from_data(const uint8_t **data, size_t *size) {
    if (*size == 0) return "";
    
    size_t len = 0;
    while (len < *size && (*data)[len] != 0) {
        len++;
    }
    
    char *str = (char*)malloc(len + 1);
    if (!str) return "";
    
    for (size_t i = 0; i < len; i++) {
        str[i] = (*data)[i];
    }
    str[len] = '\0';
    
    if (len < *size) {
        *data += len + 1;
        *size -= len + 1;
    } else {
        *data += len;
        *size -= len;
    }
    
    return str;
}

static const unsigned char *get_binary_data(const uint8_t **data, size_t *size, size_t *out_len) {
    if (*size == 0) {
        *out_len = 0;
        return NULL;
    }
    
    size_t max_len = (*size > 256) ? 256 : *size;
    size_t len = (*data)[0] % (max_len + 1);
    if (len > *size) len = *size;
    
    const unsigned char *bin = *data;
    *data += len;
    *size -= len;
    *out_len = len;
    
    return bin;
}

static unsigned short get_u16_from_data(const uint8_t **data, size_t *size) {
    if (*size >= 2) {
        unsigned short val = ((*data)[0] << 8) | (*data)[1];
        *data += 2;
        *size -= 2;
        return val;
    } else if (*size == 1) {
        unsigned short val = (*data)[0];
        *data += 1;
        *size -= 1;
        return val;
    }
    return 0;
}

static unsigned int get_u32_from_data(const uint8_t **data, size_t *size) {
    if (*size >= 4) {
        unsigned int val = ((*data)[0] << 24) | ((*data)[1] << 16) | ((*data)[2] << 8) | (*data)[3];
        *data += 4;
        *size -= 4;
        return val;
    } else if (*size >= 2) {
        unsigned int val = ((*data)[0] << 8) | (*data)[1];
        *data += 2;
        *size -= 2;
        return val;
    } else if (*size == 1) {
        unsigned int val = (*data)[0];
        *data += 1;
        *size -= 1;
        return val;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    const uint8_t *data_ptr = Data;
    size_t data_size = Size;
    
    // Extract parameters for ares_dns_record_create
    unsigned short id = get_u16_from_data(&data_ptr, &data_size);
    unsigned short flags = get_u16_from_data(&data_ptr, &data_size);
    ares_dns_opcode_t opcode = (data_size > 0) ? (ares_dns_opcode_t)(data_ptr[0] % 16) : ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = (data_size > 0) ? (ares_dns_rcode_t)(data_ptr[0] % 16) : ARES_RCODE_NOERROR;
    
    // Initialize DNS record with extracted parameters
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || !dnsrec) return 0;
    
    // Create some resource records to work with
    ares_dns_rr_t *rr1 = NULL;
    ares_dns_rr_t *rr2 = NULL;
    ares_dns_rr_t *rr3 = NULL;
    
    // First resource record - store string pointers to free later
    const char *name1 = get_string_from_data(&data_ptr, &data_size);
    const char *str_val1 = get_string_from_data(&data_ptr, &data_size);
    ares_dns_rec_type_t type1 = (data_size > 0) ? (ares_dns_rec_type_t)(data_ptr[0] % 256) : ARES_REC_TYPE_A;
    ares_dns_class_t class1 = (data_size > 0) ? (ares_dns_class_t)(data_ptr[0] % 4) : ARES_CLASS_IN;
    unsigned int ttl1 = get_u32_from_data(&data_ptr, &data_size);
    ares_dns_section_t sect1 = (data_size > 0) ? (ares_dns_section_t)(data_ptr[0] % 4) : ARES_SECTION_ANSWER;
    
    // First ares_dns_record_rr_add
    if (name1 && *name1) {
        ares_dns_record_rr_add(&rr1, dnsrec, sect1, name1, type1, class1, ttl1);
    }
    
    // Free name1 immediately after use
    if (name1 && name1 != "") {
        free((void*)name1);
        name1 = NULL;
    }
    
    // First ares_dns_rr_set_str
    if (rr1 && str_val1) {
        ares_dns_rr_set_str(rr1, ARES_RR_A_ADDR, str_val1);
    }
    
    // Free str_val1 immediately after use
    if (str_val1 && str_val1 != "") {
        free((void*)str_val1);
        str_val1 = NULL;
    }
    
    // Second resource record
    const char *name2 = get_string_from_data(&data_ptr, &data_size);
    const char *str_val2 = get_string_from_data(&data_ptr, &data_size);
    ares_dns_rec_type_t type2 = (data_size > 0) ? (ares_dns_rec_type_t)(data_ptr[0] % 256) : ARES_REC_TYPE_NS;
    ares_dns_class_t class2 = (data_size > 0) ? (ares_dns_class_t)(data_ptr[0] % 4) : ARES_CLASS_IN;
    unsigned int ttl2 = get_u32_from_data(&data_ptr, &data_size);
    ares_dns_section_t sect2 = (data_size > 0) ? (ares_dns_section_t)(data_ptr[0] % 4) : ARES_SECTION_AUTHORITY;
    
    // Second ares_dns_record_rr_add
    if (name2 && *name2) {
        ares_dns_record_rr_add(&rr2, dnsrec, sect2, name2, type2, class2, ttl2);
    }
    
    // Free name2 immediately after use
    if (name2 && name2 != "") {
        free((void*)name2);
        name2 = NULL;
    }
    
    // Second ares_dns_rr_set_str
    if (rr2 && str_val2) {
        ares_dns_rr_set_str(rr2, ARES_RR_NS_NSDNAME, str_val2);
    }
    
    // Free str_val2 immediately after use
    if (str_val2 && str_val2 != "") {
        free((void*)str_val2);
        str_val2 = NULL;
    }
    
    // Add binary data to first resource record
    size_t bin_len1 = 0, bin_len2 = 0;
    const unsigned char *bin_data1 = get_binary_data(&data_ptr, &data_size, &bin_len1);
    const unsigned char *bin_data2 = get_binary_data(&data_ptr, &data_size, &bin_len2);
    
    if (rr1 && bin_data1 && bin_len1 > 0) {
        ares_dns_rr_add_abin(rr1, ARES_RR_TXT_DATA, bin_data1, bin_len1);
    }
    if (rr1 && bin_data2 && bin_len2 > 0) {
        ares_dns_rr_add_abin(rr1, ARES_RR_TXT_DATA, bin_data2, bin_len2);
    }
    
    // Third resource record
    const char *name3 = get_string_from_data(&data_ptr, &data_size);
    ares_dns_rec_type_t type3 = (data_size > 0) ? (ares_dns_rec_type_t)(data_ptr[0] % 256) : ARES_REC_TYPE_MX;
    ares_dns_class_t class3 = (data_size > 0) ? (ares_dns_class_t)(data_ptr[0] % 4) : ARES_CLASS_IN;
    unsigned int ttl3 = get_u32_from_data(&data_ptr, &data_size);
    ares_dns_section_t sect3 = (data_size > 0) ? (ares_dns_section_t)(data_ptr[0] % 4) : ARES_SECTION_ADDITIONAL;
    
    // Third ares_dns_record_rr_add
    if (name3 && *name3) {
        ares_dns_record_rr_add(&rr3, dnsrec, sect3, name3, type3, class3, ttl3);
    }
    
    // Free name3 immediately after use
    if (name3 && name3 != "") {
        free((void*)name3);
        name3 = NULL;
    }
    
    // Set u16 value on third resource record
    unsigned short u16_val = get_u16_from_data(&data_ptr, &data_size);
    if (rr3) {
        ares_dns_rr_set_u16(rr3, ARES_RR_MX_PREFERENCE, u16_val);
    }
    
    // Cleanup
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
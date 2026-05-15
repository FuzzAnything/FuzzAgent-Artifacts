// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_opt_get_name at ares_dns_mapping.c:896:13 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_get_opt at ares_dns_record.c:1016:16 in ares_dns_record.h
// ares_dns_rr_get_str at ares_dns_record.c:981:13 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ares.h"
#include "ares_dns_record.h"

static uint16_t read_uint16(const uint8_t *data, size_t size, size_t offset) {
    if (offset + 2 > size) return 0;
    uint16_t val;
    memcpy(&val, data + offset, 2);
    return val;
}

static uint32_t read_uint32(const uint8_t *data, size_t size, size_t offset) {
    if (offset + 4 > size) return 0;
    uint32_t val;
    memcpy(&val, data + offset, 4);
    return val;
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract parameters from fuzzer input
    size_t offset = 0;
    uint16_t key_val = read_uint16(Data, Size, offset);
    offset += 2;
    uint16_t opt_val = read_uint16(Data, Size, offset);
    offset += 2;
    uint32_t idx_val = read_uint32(Data, Size, offset);
    offset += 4;
    
    // Convert to appropriate types
    ares_dns_rr_key_t key = (ares_dns_rr_key_t)key_val;
    unsigned short opt = (unsigned short)opt_val;
    size_t idx = (size_t)idx_val;
    
    // Test ares_dns_rr_key_tostr
    const char *str_result = ares_dns_rr_key_tostr(key);
    (void)str_result; // Use result to avoid unused variable warning
    
    // Test ares_dns_opt_get_name
    const char *opt_name_result = ares_dns_opt_get_name(key, opt);
    (void)opt_name_result;
    
    // Test ares_dns_rr_key_datatype
    ares_dns_datatype_t datatype_result = ares_dns_rr_key_datatype(key);
    (void)datatype_result;
    
    // For functions requiring ares_dns_rr_t pointer, we'll use NULL
    // to test error handling paths
    const unsigned char *val = NULL;
    size_t val_len = 0;
    
    // Test ares_dns_rr_get_opt with NULL pointer
    unsigned short opt_result = ares_dns_rr_get_opt(NULL, key, idx, &val, &val_len);
    (void)opt_result;
    
    // Test ares_dns_rr_get_str with NULL pointer
    const char *rr_str_result = ares_dns_rr_get_str(NULL, key);
    (void)rr_str_result;
    
    // Test ares_dns_rr_set_opt with NULL pointer
    // Use remaining fuzzer input as value data
    const unsigned char *set_val = NULL;
    size_t set_val_len = 0;
    if (offset < Size) {
        set_val = Data + offset;
        set_val_len = Size - offset;
    }
    ares_status_t set_result = ares_dns_rr_set_opt(NULL, key, opt, set_val, set_val_len);
    (void)set_result;
    
    return 0;
}
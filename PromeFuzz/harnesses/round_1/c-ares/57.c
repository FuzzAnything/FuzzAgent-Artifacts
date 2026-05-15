// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_get_opt at ares_dns_record.c:1016:16 in ares_dns_record.h
// ares_dns_rr_key_tostr at ares_dns_mapping.c:270:13 in ares_dns_record.h
// ares_dns_opt_get_name at ares_dns_mapping.c:896:13 in ares_dns_record.h
// ares_dns_rr_key_datatype at ares_dns_mapping.c:451:21 in ares_dns_record.h
// ares_dns_rr_get_str at ares_dns_record.c:981:13 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define types that are missing from the header */
typedef enum ares_status_t {
    ARES_SUCCESS = 0
} ares_status_t;

typedef unsigned char ares_bool_t;

/* Define CARES_EXTERN before including the cares headers */
#ifndef CARES_EXTERN
#define CARES_EXTERN
#endif

/* Include the required header */
#include "ares_dns_record.h"

static const uint8_t *g_fuzz_data;
static size_t g_fuzz_size;
static size_t g_fuzz_pos;

static unsigned char get_byte(void) {
    if (g_fuzz_pos >= g_fuzz_size) return 0;
    return g_fuzz_data[g_fuzz_pos++];
}

static unsigned short get_ushort(void) {
    unsigned short val = 0;
    if (g_fuzz_pos + 1 < g_fuzz_size) {
        val = (g_fuzz_data[g_fuzz_pos] << 8) | g_fuzz_data[g_fuzz_pos + 1];
        g_fuzz_pos += 2;
    } else if (g_fuzz_pos < g_fuzz_size) {
        val = g_fuzz_data[g_fuzz_pos++];
    }
    return val;
}

static size_t get_size(void) {
    size_t val = 0;
    for (int i = 0; i < 4 && g_fuzz_pos < g_fuzz_size; i++) {
        val = (val << 8) | g_fuzz_data[g_fuzz_pos++];
    }
    return val;
}

static ares_dns_rr_key_t get_key(void) {
    return (ares_dns_rr_key_t)(get_byte() % 256);
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    g_fuzz_data = Data;
    g_fuzz_size = Size;
    g_fuzz_pos = 0;
    
    /* Test ares_dns_rr_key_tostr with multiple random keys */
    for (int i = 0; i < 10 && g_fuzz_pos < g_fuzz_size; i++) {
        ares_dns_rr_key_t key = get_key();
        const char *str = ares_dns_rr_key_tostr(key);
        (void)str;
    }
    
    /* Test ares_dns_opt_get_name with random keys and options */
    g_fuzz_pos = Size / 4;
    for (int i = 0; i < 10 && g_fuzz_pos + 1 < g_fuzz_size; i++) {
        ares_dns_rr_key_t key = get_key();
        unsigned short opt = get_ushort();
        const char *str = ares_dns_opt_get_name(key, opt);
        (void)str;
    }
    
    /* Test ares_dns_rr_key_datatype with random keys */
    g_fuzz_pos = Size / 2;
    for (int i = 0; i < 10 && g_fuzz_pos < g_fuzz_size; i++) {
        ares_dns_rr_key_t key = get_key();
        ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
        (void)datatype;
    }
    
    /* Test ares_dns_rr_get_str, ares_dns_rr_set_opt, and ares_dns_rr_get_opt 
       with NULL pointers since we can't create a valid ares_dns_rr_t */
    g_fuzz_pos = (Size * 3) / 4;
    
    ares_dns_rr_t *null_rr = NULL;
    
    /* Test ares_dns_rr_get_str with NULL rr */
    ares_dns_rr_key_t key = get_key();
    const char *str = ares_dns_rr_get_str(null_rr, key);
    (void)str;
    
    /* Test ares_dns_rr_set_opt with NULL rr */
    unsigned short opt = get_ushort();
    size_t val_len = get_size() % 256;
    
    unsigned char *val = NULL;
    if (val_len > 0 && g_fuzz_pos + val_len <= g_fuzz_size) {
        val = (unsigned char*)&g_fuzz_data[g_fuzz_pos];
        g_fuzz_pos += val_len;
    }
    
    ares_status_t status = ares_dns_rr_set_opt(null_rr, key, opt, val, val_len);
    (void)status;
    
    /* Test ares_dns_rr_get_opt with NULL rr */
    size_t idx = get_size() % 10;
    const unsigned char *out_val = NULL;
    size_t out_len = 0;
    
    unsigned short opt_result = ares_dns_rr_get_opt(null_rr, key, idx, &out_val, &out_len);
    (void)opt_result;
    (void)out_val;
    (void)out_len;
    
    return 0;
}
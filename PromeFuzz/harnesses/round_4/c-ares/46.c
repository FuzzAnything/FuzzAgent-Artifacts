// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_get_keys at ares_dns_mapping.c:588:32 in ares_dns_record.h
// ares_dns_opt_get_datatype at ares_dns_mapping.c:819:25 in ares_dns_record.h
// ares_dns_rr_get_opt_byid at ares_dns_record.c:1054:13 in ares_dns_record.h
// ares_dns_rr_get_u16 at ares_dns_record.c:808:16 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
#ifndef CARES_EXTERN
# ifdef __cplusplus
#  define CARES_EXTERN extern "C"
# else
#  define CARES_EXTERN extern
# endif
#endif

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static void test_ares_dns_rr_get_keys(const uint8_t *data, size_t size) {
    if (size < sizeof(ares_dns_rec_type_t)) return;
    
    ares_dns_rec_type_t type;
    memcpy(&type, data, sizeof(ares_dns_rec_type_t));
    
    size_t cnt = 0;
    const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(type, &cnt);
    
    (void)keys;
    (void)cnt;
}

static void test_ares_dns_opt_get_datatype(const uint8_t *data, size_t size) {
    if (size < sizeof(ares_dns_rr_key_t) + sizeof(unsigned short)) return;
    
    ares_dns_rr_key_t key;
    unsigned short opt;
    
    memcpy(&key, data, sizeof(ares_dns_rr_key_t));
    memcpy(&opt, data + sizeof(ares_dns_rr_key_t), sizeof(unsigned short));
    
    ares_dns_opt_datatype_t datatype = ares_dns_opt_get_datatype(key, opt);
    (void)datatype;
}

static void test_ares_dns_rr_get_opt_byid(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    ares_dns_rr_t *dns_rr = NULL;
    ares_dns_rr_key_t key;
    unsigned short opt;
    const unsigned char *val = NULL;
    size_t val_len = 0;
    
    key = (ares_dns_rr_key_t)(data[0] % 100);
    opt = (unsigned short)(size > 1 ? data[1] : 0);
    
    ares_bool_t result = ares_dns_rr_get_opt_byid(dns_rr, key, opt, &val, &val_len);
    (void)result;
}

static void test_ares_dns_rr_get_u16(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    ares_dns_rr_t *dns_rr = NULL;
    ares_dns_rr_key_t key;
    
    key = (ares_dns_rr_key_t)(data[0] % 100);
    
    unsigned short result = ares_dns_rr_get_u16(dns_rr, key);
    (void)result;
}

static void test_ares_dns_rr_set_bin(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    ares_dns_rr_t *dns_rr = NULL;
    ares_dns_rr_key_t key;
    
    key = (ares_dns_rr_key_t)(data[0] % 100);
    
    const unsigned char *val = size > 1 ? data + 1 : NULL;
    size_t len = size > 1 ? size - 1 : 0;
    
    ares_status_t result = ares_dns_rr_set_bin(dns_rr, key, val, len);
    (void)result;
}

static void test_ares_dns_rr_get_abin_cnt(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    ares_dns_rr_t *dns_rr = NULL;
    ares_dns_rr_key_t key;
    
    key = (ares_dns_rr_key_t)(data[0] % 100);
    
    size_t result = ares_dns_rr_get_abin_cnt(dns_rr, key);
    (void)result;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    test_ares_dns_rr_get_keys(Data, Size);
    test_ares_dns_opt_get_datatype(Data, Size);
    test_ares_dns_rr_get_opt_byid(Data, Size);
    test_ares_dns_rr_get_u16(Data, Size);
    test_ares_dns_rr_set_bin(Data, Size);
    test_ares_dns_rr_get_abin_cnt(Data, Size);
    
    return 0;
}
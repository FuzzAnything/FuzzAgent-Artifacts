// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_get_u8 at ares_dns_record.c:791:15 in ares_dns_record.h
// ares_dns_rr_get_u16 at ares_dns_record.c:808:16 in ares_dns_record.h
// ares_dns_rr_get_u32 at ares_dns_record.c:825:14 in ares_dns_record.h
// ares_dns_rr_get_str at ares_dns_record.c:981:13 in ares_dns_record.h
// ares_dns_rr_get_u8 at ares_dns_record.c:791:15 in ares_dns_record.h
// ares_dns_rr_get_u16 at ares_dns_record.c:808:16 in ares_dns_record.h
// ares_dns_rr_get_u32 at ares_dns_record.c:825:14 in ares_dns_record.h
// ares_dns_rr_get_str at ares_dns_record.c:981:13 in ares_dns_record.h
// ares_dns_parse at ares_dns_parse.c:1310:15 in ares_dns_record.h
// ares_dns_record_rr_cnt at ares_dns_record.c:349:8 in ares_dns_record.h
// ares_dns_record_rr_get_const at ares_dns_record.c:499:3 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/* Define CARES_EXTERN if not defined */
#ifndef CARES_EXTERN
#ifdef _WIN32
#  ifdef CARES_STATICLIB
#    define CARES_EXTERN
#  else
#    ifdef CARES_BUILDING_LIBRARY
#      define CARES_EXTERN __declspec(dllexport)
#    else
#      define CARES_EXTERN __declspec(dllimport)
#    endif
#  endif
#else
#  define CARES_EXTERN
#endif
#endif

/* Include required header files in correct order */
#include "ares.h"
#include "ares_dns_record.h"

static void create_dummy_file(const uint8_t *data, size_t size) {
    const char *filename = "./dummy_file";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd != -1) {
        write(fd, data, size);
        close(fd);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

/* Helper function to get a valid resource record */
static const ares_dns_rr_t* get_test_rr(const uint8_t *data, size_t size) {
    /* Create a minimal DNS query and parse it to get a resource record */
    unsigned char dns_query[] = {
        0x12, 0x34, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x03, 0x77, 0x77, 0x77,
        0x06, 0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x03,
        0x63, 0x6f, 0x6d, 0x00, 0x00, 0x01, 0x00, 0x01
    };
    
    /* Override some bytes with fuzz data if available */
    for (size_t i = 0; i < size && i < sizeof(dns_query); i++) {
        dns_query[i] = data[i];
    }
    
    ares_dns_record_t *dns_record = NULL;
    ares_status_t status = ares_dns_parse(dns_query, sizeof(dns_query), 0, &dns_record);
    if (status != ARES_SUCCESS || !dns_record) {
        return NULL;
    }
    
    /* Try to get a resource record from answer section */
    const ares_dns_rr_t *rr = NULL;
    size_t an_count = ares_dns_record_rr_cnt(dns_record, ARES_SECTION_ANSWER);
    if (an_count > 0) {
        rr = ares_dns_record_rr_get_const(dns_record, ARES_SECTION_ANSWER, 0);
    }
    
    /* Store the record for cleanup later */
    static ares_dns_record_t *saved_record = NULL;
    if (saved_record) {
        ares_dns_record_destroy(saved_record);
    }
    saved_record = dns_record;
    
    return rr;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *data, size_t size) {
    if (size < 1) {
        return 0;
    }
    
    create_dummy_file(data, size);
    
    /* Get a resource record to test with */
    const ares_dns_rr_t *rr = get_test_rr(data, size);
    if (!rr) {
        remove_dummy_file();
        return 0;
    }
    
    /* Use fuzz data to select keys */
    size_t data_index = 0;
    
    /* Call all four functions in the required order with various keys */
    for (int i = 0; i < 10 && data_index < size; i++) {
        ares_dns_rr_key_t key = (ares_dns_rr_key_t)(data[data_index] % 256);
        data_index = (data_index + 1) % size;
        
        /* Call in required order: u8 -> u16 -> u32 -> str */
        unsigned char u8_val = ares_dns_rr_get_u8(rr, key);
        (void)u8_val;
        
        unsigned short u16_val = ares_dns_rr_get_u16(rr, key);
        (void)u16_val;
        
        unsigned int u32_val = ares_dns_rr_get_u32(rr, key);
        (void)u32_val;
        
        const char *str_val = ares_dns_rr_get_str(rr, key);
        (void)str_val;
    }
    
    /* Try some specific keys that might be valid */
    ares_dns_rr_key_t test_keys[] = {
        0, 1, 2, 3, 4, 5, 100, 101, 200, 201
    };
    
    for (size_t i = 0; i < sizeof(test_keys)/sizeof(test_keys[0]); i++) {
        ares_dns_rr_get_u8(rr, test_keys[i]);
        ares_dns_rr_get_u16(rr, test_keys[i]);
        ares_dns_rr_get_u32(rr, test_keys[i]);
        ares_dns_rr_get_str(rr, test_keys[i]);
    }
    
    remove_dummy_file();
    return 0;
}
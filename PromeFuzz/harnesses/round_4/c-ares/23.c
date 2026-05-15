// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Define CARES_EXTERN if not already defined */
#ifndef CARES_EXTERN
#define CARES_EXTERN extern
#endif

/* Include required headers in correct order */
#include "ares.h"
#include "ares_dns_record.h"

static char *read_string(const uint8_t **data, size_t *size) {
    if (*size == 0) return NULL;
    
    size_t len = 0;
    while (len < *size && (*data)[len] != 0) {
        len++;
    }
    
    char *str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, *data, len);
    str[len] = '\0';
    
    *data += len;
    *size -= len;
    
    // Skip null terminator if present
    if (len < *size && (*data)[0] == 0) {
        *data += 1;
        *size -= 1;
    }
    
    return str;
}

static unsigned short read_short(const uint8_t **data, size_t *size) {
    if (*size < 2) return 0;
    unsigned short val = (*data)[0] | ((*data)[1] << 8);
    *data += 2;
    *size -= 2;
    return val;
}

static unsigned int read_int(const uint8_t **data, size_t *size) {
    if (*size < 4) return 0;
    unsigned int val = (*data)[0] | ((*data)[1] << 8) | 
                      ((*data)[2] << 16) | ((*data)[3] << 24);
    *data += 4;
    *size -= 4;
    return val;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;
    
    const uint8_t *data = Data;
    size_t remaining = Size;
    
    /* Create DNS record using ares_dns_record_create */
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t create_status = ares_dns_record_create(&dnsrec, 0x1234, 
                                                        ARES_FLAG_QR | ARES_FLAG_RD,
                                                        ARES_OPCODE_QUERY,
                                                        ARES_RCODE_NOERROR);
    if (create_status != ARES_SUCCESS || !dnsrec) {
        return 0;
    }
    
    /* Create first RR for initial operations */
    ares_dns_rr_t *rr1 = NULL;
    ares_status_t status = ares_dns_record_rr_add(&rr1, dnsrec, 
                                                 ARES_SECTION_ANSWER,
                                                 "example.com",
                                                 ARES_REC_TYPE_A,
                                                 ARES_CLASS_IN,
                                                 300);
    if (status != ARES_SUCCESS || !rr1) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }
    
    /* 1. ares_dns_rr_set_str */
    char *str_val = read_string(&data, &remaining);
    if (str_val) {
        ares_dns_rr_set_str(rr1, ARES_RR_A_ADDR, str_val);
        free(str_val);
    }
    
    /* 2. ares_dns_pton */
    char *ip_str = read_string(&data, &remaining);
    if (ip_str) {
        struct ares_addr addr;
        size_t out_len = 0;
        addr.family = AF_UNSPEC;
        const void *bin_addr = ares_dns_pton(ip_str, &addr, &out_len);
        free(ip_str);
    }
    
    /* 3. First ares_dns_rr_set_opt */
    if (remaining >= 4) {
        unsigned short opt1 = read_short(&data, &remaining);
        unsigned short val_len1 = read_short(&data, &remaining);
        if (val_len1 > remaining) val_len1 = remaining;
        
        const unsigned char *val1 = data;
        ares_dns_rr_set_opt(rr1, ARES_RR_OPT_OPTIONS, opt1, val1, val_len1);
        data += val_len1;
        remaining -= val_len1;
    }
    
    /* 4. Second ares_dns_rr_set_opt */
    if (remaining >= 4) {
        unsigned short opt2 = read_short(&data, &remaining);
        unsigned short val_len2 = read_short(&data, &remaining);
        if (val_len2 > remaining) val_len2 = remaining;
        
        const unsigned char *val2 = data;
        ares_dns_rr_set_opt(rr1, ARES_RR_OPT_OPTIONS, opt2, val2, val_len2);
        data += val_len2;
        remaining -= val_len2;
    }
    
    /* 5. ares_dns_record_rr_add */
    if (remaining >= 10) {
        ares_dns_rr_t *rr2 = NULL;
        char *name = read_string(&data, &remaining);
        if (!name) name = "test.com";
        
        unsigned short type = read_short(&data, &remaining);
        unsigned short rclass = read_short(&data, &remaining);
        unsigned int ttl = read_int(&data, &remaining);
        
        ares_dns_record_rr_add(&rr2, dnsrec, ARES_SECTION_ADDITIONAL,
                              name, type, rclass, ttl);
        
        if (name && name != "test.com") free(name);
    }
    
    ares_dns_record_destroy(dnsrec);
    return 0;
}
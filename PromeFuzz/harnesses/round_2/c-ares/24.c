// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_rr_set_opt at ares_dns_record.c:1404:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_str at ares_dns_record.c:1294:15 in ares_dns_record.h
// ares_dns_pton at ares_hosts_file.c:103:13 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef CARES_EXTERN
#define CARES_EXTERN extern
#endif

/* Include required headers from c-ares */
#include "ares.h"
#include "ares_dns_record.h"

static void dummy_cleanup(void) {
    /* Cleanup if needed */
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Initialize variables for the API calls */
    ares_dns_record_t *dnsrec = NULL;
    ares_dns_rr_t *rr = NULL;
    ares_status_t status;
    struct ares_addr addr;
    size_t out_len;
    const void *result;
    ares_dns_rr_t *rr_out = NULL;
    
    /* Use first byte to determine family for ares_dns_pton */
    int family_choice = Data[0] % 3;
    switch (family_choice) {
        case 0: addr.family = AF_UNSPEC; break;
        case 1: addr.family = AF_INET; break;
        case 2: addr.family = AF_INET6; break;
        default: addr.family = AF_UNSPEC; break;
    }
    
    /* Create a null-terminated string from fuzz data for ares_dns_pton */
    char *ip_str = NULL;
    if (Size > 1) {
        size_t ip_str_len = (Size - 1) < 128 ? (Size - 1) : 128;
        ip_str = (char*)malloc(ip_str_len + 1);
        if (ip_str) {
            memcpy(ip_str, Data + 1, ip_str_len);
            ip_str[ip_str_len] = '\0';
        }
    } else {
        ip_str = strdup("127.0.0.1");
    }
    
    if (!ip_str) {
        return 0;
    }
    
    /* Create a dummy DNS record structure with required parameters */
    unsigned short id = 12345; /* Use a constant value instead of reading from Data */
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    status = ares_dns_record_create(&dnsrec, id, flags, opcode, rcode);
    if (status != ARES_SUCCESS || !dnsrec) {
        free(ip_str);
        return 0;
    }
    
    /* Create a dummy resource record */
    const char *name = "test.example.com";
    ares_dns_rec_type_t type = ARES_REC_TYPE_A;
    ares_dns_class_t rclass = ARES_CLASS_IN;
    unsigned int ttl = 300;
    
    status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                   name, type, rclass, ttl);
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        free(ip_str);
        return 0;
    }
    
    /* 1. Call ares_dns_rr_set_str */
    /* Use a valid key for A record - try ARES_RR_A_NAME if it exists, otherwise use a different key */
    const char *dummy_str = "test.example.com";
    ares_dns_rr_key_t key = (ares_dns_rr_key_t)((ARES_REC_TYPE_A * 100) + 0); /* Try to construct key for A record name */
    status = ares_dns_rr_set_str(rr, key, dummy_str);
    (void)status;
    
    /* 2. Call ares_dns_pton */
    result = ares_dns_pton(ip_str, &addr, &out_len);
    (void)result;
    
    /* 3. Call ares_dns_rr_set_opt */
    unsigned short opt1 = 0; /* Use a constant value instead of reading from Data[1] */
    const unsigned char *val1 = NULL;
    size_t val_len1 = 0;
    if (Size > 10) {
        val1 = Data + 2;
        val_len1 = (Size - 2 < 100) ? (Size - 2) : 100;
    }
    /* Use ARES_RR_OPT_FLAGS instead of ARES_RR_OPT_KEY */
    status = ares_dns_rr_set_opt(rr, ARES_RR_OPT_FLAGS, opt1, val1, val_len1);
    (void)status;
    
    /* 4. Call ares_dns_rr_set_opt again */
    unsigned short opt2 = 0; /* Use a constant value instead of reading from Data[2] */
    const unsigned char *val2 = NULL;
    size_t val_len2 = 0;
    if (Size > 20) {
        val2 = Data + 10;
        val_len2 = (Size - 10 < 100) ? (Size - 10) : 100;
    }
    status = ares_dns_rr_set_opt(rr, ARES_RR_OPT_FLAGS, opt2, val2, val_len2);
    (void)status;
    
    /* 5. Call ares_dns_record_rr_add */
    /* Use fuzzed data for parameters - but check bounds first */
    ares_dns_rec_type_t fuzz_type = ARES_REC_TYPE_A;
    ares_dns_class_t fuzz_class = ARES_CLASS_IN;
    
    if (Size > 0) {
        fuzz_type = (ares_dns_rec_type_t)(Data[0] % 256);
    }
    
    ares_dns_section_t fuzz_section = ARES_SECTION_ANSWER;
    unsigned int fuzz_ttl = 300;
    
    /* Ensure name is null-terminated */
    char *safe_name = NULL;
    if (Size > 30) {
        size_t name_len = (Size - 20 < 256) ? (Size - 20) : 256;
        safe_name = (char*)malloc(name_len + 1);
        if (safe_name) {
            memcpy(safe_name, Data + 20, name_len);
            safe_name[name_len] = '\0';
            name = safe_name;
        }
    }
    
    status = ares_dns_record_rr_add(&rr_out, dnsrec, fuzz_section,
                                    name, fuzz_type, fuzz_class, fuzz_ttl);
    (void)status;
    (void)rr_out;
    
    /* Cleanup */
    free(safe_name);
    free(ip_str);
    ares_dns_record_destroy(dnsrec);
    dummy_cleanup();
    
    return 0;
}
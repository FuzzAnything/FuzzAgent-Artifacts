// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_dns_rr_get_bin at ares_dns_record.c:842:22 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
// ares_dns_rr_get_abin at ares_dns_record.c:899:22 in ares_dns_record.h
// ares_dns_rr_del_abin at ares_dns_record.c:917:15 in ares_dns_record.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include "ares_dns_record.h"

static void fuzz_dns_rr_functions(ares_dns_rr_t *rr, const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Use first byte to determine which key to use (limited to valid range)
    uint8_t key_selector = data[0];
    ares_dns_rr_key_t keys[] = {
        ARES_RR_TXT_DATA,        // ABINP type
        ARES_RR_RAW_RR_DATA,     // BINP type
        ARES_RR_HTTPS_TARGET,    // ABINP type
        ARES_RR_SVCB_TARGET      // ABINP type
    };
    ares_dns_rr_key_t key = keys[key_selector % (sizeof(keys)/sizeof(keys[0]))];
    
    // Use second byte to determine operation
    if (size < 2) return;
    uint8_t operation = data[1];
    
    // Prepare binary data from fuzzer input (skip first 2 bytes)
    const unsigned char *bin_data = data + 2;
    size_t bin_size = (size > 2) ? size - 2 : 0;
    
    // Limit size to prevent excessive allocations
    if (bin_size > 1024) bin_size = 1024;
    
    switch (operation % 6) {
        case 0: {
            // Test ares_dns_rr_get_bin
            size_t len = 0;
            const unsigned char *result = ares_dns_rr_get_bin(rr, key, &len);
            (void)result; // Use result to avoid unused variable warning
            break;
        }
        case 1: {
            // Test ares_dns_rr_get_abin_cnt
            size_t cnt = ares_dns_rr_get_abin_cnt(rr, key);
            (void)cnt;
            break;
        }
        case 2: {
            // Test ares_dns_rr_set_bin
            ares_status_t status = ares_dns_rr_set_bin(rr, key, bin_data, bin_size);
            (void)status;
            break;
        }
        case 3: {
            // Test ares_dns_rr_add_abin
            ares_status_t status = ares_dns_rr_add_abin(rr, key, bin_data, bin_size);
            (void)status;
            break;
        }
        case 4: {
            // Test ares_dns_rr_get_abin
            size_t len = 0;
            size_t idx = (size > 3) ? data[2] % 8 : 0; // Use third byte as index
            const unsigned char *result = ares_dns_rr_get_abin(rr, key, idx, &len);
            (void)result;
            break;
        }
        case 5: {
            // Test ares_dns_rr_del_abin
            size_t idx = (size > 3) ? data[2] % 8 : 0; // Use third byte as index
            ares_status_t status = ares_dns_rr_del_abin(rr, key, idx);
            (void)status;
            break;
        }
    }
}

int LLVMFuzzerTestOneInput_52(const uint8_t *data, size_t size) {
    // Initialize c-ares library
    ares_channel channel;
    if (ares_init(&channel) != ARES_SUCCESS) {
        return 0;
    }
    
    // Create a DNS record
    ares_dns_record_t *dns = NULL;
    if (ares_dns_record_create(&dns, 0x1234, ARES_FLAG_QR | ARES_FLAG_RD, 
                               ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) != ARES_SUCCESS) {
        ares_destroy(channel);
        return 0;
    }
    
    // Create a TXT resource record (which has ABINP fields)
    ares_dns_rr_t *rr_txt = NULL;
    if (ares_dns_record_rr_add(&rr_txt, dns, ARES_SECTION_ANSWER, "example.com", 
                               ARES_REC_TYPE_TXT, ARES_CLASS_IN, 300) == ARES_SUCCESS && rr_txt) {
        // Fuzz the target functions
        fuzz_dns_rr_functions(rr_txt, data, size);
    }
    
    // Create a RAW_RR resource record (which has BINP fields)
    ares_dns_rr_t *rr_raw = NULL;
    if (ares_dns_record_rr_add(&rr_raw, dns, ARES_SECTION_ANSWER, "raw.example.com",
                               ARES_REC_TYPE_RAW_RR, ARES_CLASS_IN, 300) == ARES_SUCCESS && rr_raw) {
        // Fuzz the target functions with different data (shift by 1 byte)
        if (size > 1) {
            fuzz_dns_rr_functions(rr_raw, data + 1, size - 1);
        }
    }
    
    // Create an OPT resource record (which has BIN fields)
    ares_dns_rr_t *rr_opt = NULL;
    if (ares_dns_record_rr_add(&rr_opt, dns, ARES_SECTION_ADDITIONAL, "",
                               ARES_REC_TYPE_OPT, 4096, 0) == ARES_SUCCESS && rr_opt) {
        // Fuzz the target functions with different data (shift by 2 bytes)
        if (size > 2) {
            fuzz_dns_rr_functions(rr_opt, data + 2, size - 2);
        }
    }
    
    // Clean up
    ares_dns_record_destroy(dns);
    ares_destroy(channel);
    
    return 0;
}
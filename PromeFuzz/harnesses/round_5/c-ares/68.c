// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_rr_get_bin at ares_dns_record.c:842:22 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_rr_get_bin at ares_dns_record.c:842:22 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_record_create at ares_dns_record.c:52:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_rr_add at ares_dns_record.c:396:15 in ares_dns_record.h
// ares_dns_record_destroy at ares_dns_record.c:223:6 in ares_dns_record.h
// ares_dns_rr_set_bin at ares_dns_record.c:1240:15 in ares_dns_record.h
// ares_dns_rr_get_bin at ares_dns_record.c:842:22 in ares_dns_record.h
// ares_dns_rr_add_abin at ares_dns_record.c:934:15 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
// ares_dns_rr_get_abin at ares_dns_record.c:899:22 in ares_dns_record.h
// ares_dns_rr_del_abin at ares_dns_record.c:917:15 in ares_dns_record.h
// ares_dns_rr_get_abin_cnt at ares_dns_record.c:882:8 in ares_dns_record.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include necessary c-ares headers in the correct order */
#include "ares.h"
#include "ares_dns_record.h"

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    // Initialize with at least 1 byte for key selection
    if (Size < 1) return 0;
    
    // Use first byte to determine which key to test
    uint8_t key_selector = Data[0];
    
    // Create DNS record
    ares_dns_record_t *dns_record = NULL;
    ares_dns_record_create(&dns_record, 0x1234, 0x8000, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (!dns_record) return 0;
    
    // Create resource records using the correct function
    ares_dns_rr_t *txt_rr = NULL;
    ares_dns_rr_t *opt_rr = NULL;
    ares_dns_rr_t *uri_rr = NULL;
    
    // Create TXT record
    ares_dns_record_rr_add(&txt_rr, dns_record, ARES_SECTION_ANSWER, 
                          "example.com", ARES_REC_TYPE_TXT, ARES_CLASS_IN, 3600);
    
    // Create OPT record (special handling for OPT)
    ares_dns_record_rr_add(&opt_rr, dns_record, ARES_SECTION_ADDITIONAL, 
                          "", ARES_REC_TYPE_OPT, 0, 0);
    
    // Create URI record
    ares_dns_record_rr_add(&uri_rr, dns_record, ARES_SECTION_ANSWER, 
                          "example.com", ARES_REC_TYPE_URI, ARES_CLASS_IN, 3600);
    
    if (!txt_rr || !opt_rr || !uri_rr) {
        ares_dns_record_destroy(dns_record);
        return 0;
    }
    
    // Test with different keys based on selector
    ares_dns_rr_key_t test_key;
    ares_dns_rr_t *test_rr = txt_rr; // Default to TXT
    
    // Select key based on first byte - use only valid keys from the enum
    switch (key_selector % 3) {
        case 0:
            test_key = ARES_RR_TXT_DATA; // BINP type
            test_rr = txt_rr;
            break;
        case 1:
            // For OPT record, use ARES_RR_OPT_UDP_SIZE which should be valid
            test_key = ARES_RR_OPT_UDP_SIZE; // Different key for OPT
            test_rr = opt_rr;
            break;
        case 2:
            // For URI record, use ARES_RR_URI_TARGET if available, otherwise fall back to TXT
            test_key = ARES_RR_TXT_DATA; // Use TXT key for testing
            test_rr = uri_rr;
            break;
    }
    
    // Prepare binary data from fuzzer input (skip first byte used for key selection)
    const unsigned char *bin_data = Data + 1;
    size_t bin_len = Size > 1 ? Size - 1 : 0;
    
    // Test ares_dns_rr_set_bin
    ares_status_t status = ares_dns_rr_set_bin(test_rr, test_key, bin_data, bin_len);
    
    // Test ares_dns_rr_get_bin
    size_t get_len = 0;
    const unsigned char *retrieved_bin = ares_dns_rr_get_bin(test_rr, test_key, &get_len);
    
    // Test ABINP functions on TXT records (which support array of binary data)
    if (key_selector % 3 == 0) {
        // Add multiple binary entries
        for (int i = 0; i < 3 && i * 10 < (int)bin_len; i++) {
            size_t chunk_len = (i + 1) * 10;
            if (chunk_len > bin_len) chunk_len = bin_len;
            ares_dns_rr_add_abin(txt_rr, ARES_RR_TXT_DATA, bin_data, chunk_len);
        }
        
        // Get count of ABINP entries
        size_t abin_cnt = ares_dns_rr_get_abin_cnt(txt_rr, ARES_RR_TXT_DATA);
        
        // Retrieve each ABINP entry
        for (size_t i = 0; i < abin_cnt; i++) {
            size_t elem_len = 0;
            const unsigned char *abin_elem = ares_dns_rr_get_abin(txt_rr, ARES_RR_TXT_DATA, i, &elem_len);
            
            // Delete some entries
            if (i % 2 == 0 && abin_cnt > 0) {
                ares_dns_rr_del_abin(txt_rr, ARES_RR_TXT_DATA, 0);
                // Re-get count after deletion
                abin_cnt = ares_dns_rr_get_abin_cnt(txt_rr, ARES_RR_TXT_DATA);
            }
        }
    }
    
    // Test with empty data
    if (Size > 50) {
        ares_dns_rr_set_bin(test_rr, test_key, NULL, 0);
        ares_dns_rr_get_bin(test_rr, test_key, &get_len);
    }
    
    // Test with maximum size (limited to avoid excessive memory)
    if (Size > 100 && bin_len > 100) {
        size_t max_test_size = bin_len > 1000 ? 1000 : bin_len;
        ares_dns_rr_set_bin(test_rr, test_key, bin_data, max_test_size);
        ares_dns_rr_get_bin(test_rr, test_key, &get_len);
    }

    // Cleanup
    ares_dns_record_destroy(dns_record);
    
    return 0;
}
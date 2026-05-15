#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"

namespace harness_006 {
/*
 * Fuzzing harness for c-ares DNS library - DNS Mapping Utilities and Legacy Parsing
 * Targets: 7 critical uncovered DNS mapping utilities with 126+ undiscovered branches:
 *   - ares_dns_rr_get_keys, ares_dns_rcode_tostr, ares_dns_opt_get_name, ares_dns_opt_get_datatype
 *   - ares_dns_class_fromstr, ares_dns_rec_type_fromstr, ares_dns_section_tostr
 * Additional legacy parsing targets:
 *   - ares_parse_naptr_reply (30 branches, 143 undiscovered accumulated branches)
 *   - ares_parse_txt_reply_ext (2 branches, 116 undiscovered accumulated branches)
 * 
 * Semantic Focus: Enum-to-string conversions, string-to-enum mappings, DNS record key enumeration,
 * DNS option metadata extraction, and legacy NAPTR/TXT reply parsing.
 * Follows exact invocation sequence from coverage guidance for maximum branch coverage.
 */


/* Note: ares_dns_mapping.h doesn't exist in the built headers, 
 * mapping functions are in ares_dns_record.h */
/* ares_data.h might not exist, but ares_free_data is in ares.h */
/* We'll use ares.h for ares_free_data */

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for initialization and various tests
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // PHASE 1: Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // PHASE 2: Create test DNS records for mapping tests
    ares_dns_record_t *dns_records[5] = {NULL};
    size_t num_records = 0;
    
    // Create a few DNS records with different types based on fuzzer input
    // Use various DNS record types to exercise different key enumeration paths
    ares_dns_rec_type_t record_types[] = {
        ARES_REC_TYPE_A,
        ARES_REC_TYPE_AAAA,
        ARES_REC_TYPE_TXT,
        ARES_REC_TYPE_MX,
        ARES_REC_TYPE_NAPTR,
        ARES_REC_TYPE_SRV,
        ARES_REC_TYPE_OPT,
        ARES_REC_TYPE_CAA
    };
    
    size_t max_records_to_create = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    for (size_t i = 0; i < max_records_to_create && fdp.remaining_bytes() > 20; i++) {
        ares_dns_record_t *rec = NULL;
        uint16_t query_id = fdp.ConsumeIntegral<uint16_t>();
        
        // Choose record type from array or random
        ares_dns_rec_type_t rec_type;
        if (fdp.ConsumeBool() && i < sizeof(record_types)/sizeof(record_types[0])) {
            rec_type = record_types[i];
        } else {
            rec_type = static_cast<ares_dns_rec_type_t>(fdp.ConsumeIntegralInRange<int>(1, 100));
        }
        
        ares_status_t status = ares_dns_record_create(&rec, query_id, 
                                                     ARES_FLAG_RD | ARES_FLAG_AA,
                                                     ARES_OPCODE_QUERY,
                                                     ARES_RCODE_NOERROR);
        
        if (status == ARES_SUCCESS && rec != NULL) {
            // Add a question to the record
            std::string domain_name = fdp.ConsumeRandomLengthString(64);
            if (!domain_name.empty()) {
                ares_dns_record_query_add(rec, domain_name.c_str(), 
                                         rec_type, ARES_CLASS_IN);
                
                // Add some RRs if possible
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
                    ares_dns_rr_key_t rr_key = ARES_RR_A_ADDR;
                    std::vector<uint8_t> rr_data;
                    
                    switch (rec_type) {
                        case ARES_REC_TYPE_A:
                            rr_key = ARES_RR_A_ADDR;
                            rr_data = fdp.ConsumeBytes<uint8_t>(4); // IPv4 address
                            if (rr_data.size() == 4) {
                                ares_dns_rr_t *rr_out = NULL;
                                ares_dns_record_rr_add(&rr_out, rec, ARES_SECTION_ANSWER,
                                                      domain_name.c_str(), rec_type,
                                                      ARES_CLASS_IN, 3600);
                                // Note: rr_out is owned by rec, no need to free separately
                                ares_dns_rr_set_u32(rr_out, rr_key, *reinterpret_cast<uint32_t*>(rr_data.data()));
                            }
                            break;
                        case ARES_REC_TYPE_TXT:
                            rr_key = ARES_RR_TXT_DATA;
                            rr_data = fdp.ConsumeBytes<uint8_t>(fdp.ConsumeIntegralInRange<size_t>(1, 100));
                            if (!rr_data.empty()) {
                                ares_dns_rr_t *rr_out = NULL;
                                ares_dns_record_rr_add(&rr_out, rec, ARES_SECTION_ANSWER,
                                                      domain_name.c_str(), rec_type,
                                                      ARES_CLASS_IN, 3600);
                                ares_dns_rr_set_bin(rr_out, rr_key, rr_data.data(), rr_data.size());
                            }
                            break;
                        default:
                            // For other types, just add empty RR
                            ares_dns_rr_t *rr_out = NULL;
                            ares_dns_record_rr_add(&rr_out, rec, ARES_SECTION_ANSWER,
                                                  domain_name.c_str(), rec_type,
                                                  ARES_CLASS_IN, 3600);
                            break;
                    }
                }
                
                dns_records[num_records++] = rec;
            } else {
                ares_dns_record_destroy(rec);
            }
        }
    }
    
    // PHASE 3: Test DNS mapping utilities (primary targets)
    
    // 1. Test ares_dns_rr_get_keys for various record types
    for (int rec_type_val = 1; rec_type_val <= 100 && fdp.remaining_bytes() > 0; rec_type_val++) {
        ares_dns_rec_type_t rec_type = static_cast<ares_dns_rec_type_t>(rec_type_val);
        size_t key_count = 0;
        const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(rec_type, &key_count);
        // Use the result even if NULL (that's also valid behavior)
        (void)keys;
        (void)key_count;
        
        // Break early if we've consumed enough fuzzer cycles
        if (rec_type_val % 10 == 0 && fdp.remaining_bytes() < 50) {
            break;
        }
    }
    // 2. Test ares_dns_rcode_tostr with various RCODE values
    for (int rcode_val = 0; rcode_val <= 30 && fdp.remaining_bytes() > 0; rcode_val++) {
        ares_dns_rcode_t rcode = static_cast<ares_dns_rcode_t>(rcode_val);
        const char *rcode_str = ares_dns_rcode_tostr(rcode);
        (void)rcode_str; // Use result to avoid compiler warnings
    }
    
    // 3. Test ares_dns_opt_get_name and ares_dns_opt_get_datatype
    for (int opt_val = 0; opt_val < 50 && fdp.remaining_bytes() > 0; opt_val++) {
        unsigned short opt = static_cast<unsigned short>(opt_val);
        
        // Test with various RR keys that have options
        ares_dns_rr_key_t test_keys[] = {
            ARES_RR_OPT_OPTIONS,
            ARES_RR_SVCB_PARAMS,
            ARES_RR_HTTPS_PARAMS
        };
        
        for (size_t key_idx = 0; key_idx < sizeof(test_keys)/sizeof(test_keys[0]); key_idx++) {
            const char *opt_name = ares_dns_opt_get_name(test_keys[key_idx], opt);
            ares_dns_opt_datatype_t data_type = ares_dns_opt_get_datatype(test_keys[key_idx], opt);
            
            (void)opt_name;
            (void)data_type;
        }
    }
    // 4. Test ares_dns_class_fromstr and related bidirectional conversions
    if (fdp.remaining_bytes() > 10) {
        std::string class_str_test = fdp.ConsumeRandomLengthString(20);
        ares_dns_class_t out_class;
        ares_bool_t conv_result = ares_dns_class_fromstr(&out_class, class_str_test.c_str());
        (void)conv_result;
        (void)out_class;
        
        // Also test the reverse if conversion was successful
        if (conv_result == ARES_TRUE) {
            // Note: ares_dns_class_tostr might not exist, but we can check
            // For now, just test with known valid class strings
            const char *known_classes[] = {"IN", "CH", "HS", "NONE", "ANY"};
            for (size_t i = 0; i < sizeof(known_classes)/sizeof(known_classes[0]) && fdp.remaining_bytes() > 0; i++) {
                ares_dns_class_t temp_class;
                if (ares_dns_class_fromstr(&temp_class, known_classes[i]) == ARES_TRUE) {
                    // Conversion successful
                    (void)temp_class;
                }
            }
        }
    }
    
    // 5. Test ares_dns_rec_type_fromstr
    if (fdp.remaining_bytes() > 10) {
        std::string rec_type_str = fdp.ConsumeRandomLengthString(20);
        ares_dns_rec_type_t out_rec_type;
        ares_bool_t rec_conv_result = ares_dns_rec_type_fromstr(&out_rec_type, rec_type_str.c_str());
        (void)rec_conv_result;
        (void)out_rec_type;
        
        // Test with known record type strings
        const char *known_rec_types[] = {"A", "AAAA", "TXT", "MX", "NAPTR", "SRV", "OPT", "CAA"};
        for (size_t i = 0; i < sizeof(known_rec_types)/sizeof(known_rec_types[0]) && fdp.remaining_bytes() > 0; i++) {
            ares_dns_rec_type_t temp_rec_type;
            if (ares_dns_rec_type_fromstr(&temp_rec_type, known_rec_types[i]) == ARES_TRUE) {
                (void)temp_rec_type;
            }
        }
    }
    
    // 6. Test ares_dns_section_tostr
    for (int section_val = 1; section_val <= 3 && fdp.remaining_bytes() > 0; section_val++) {
        ares_dns_section_t section = static_cast<ares_dns_section_t>(section_val);
        const char *section_str = ares_dns_section_tostr(section);
        (void)section_str;
    }
    
    // PHASE 4: Test legacy parsing functions
    
    // 1. Test ares_parse_naptr_reply
    if (fdp.remaining_bytes() > 50) {
        size_t naptr_data_size = fdp.ConsumeIntegralInRange<size_t>(12, 1024);
        if (naptr_data_size <= fdp.remaining_bytes()) {
            std::vector<uint8_t> naptr_data = fdp.ConsumeBytes<uint8_t>(naptr_data_size);
            struct ares_naptr_reply *naptr_reply = NULL;
            
            // Call the legacy parsing function
            int parse_result = ares_parse_naptr_reply(naptr_data.data(), 
                                                     static_cast<int>(naptr_data.size()),
                                                     &naptr_reply);
            
            (void)parse_result;
            
            // Clean up if parsing succeeded
            if (naptr_reply != NULL) {
                ares_free_data(naptr_reply);
            }
        }
    }
    
    // 2. Test ares_parse_txt_reply_ext
    if (fdp.remaining_bytes() > 50) {
        size_t txt_data_size = fdp.ConsumeIntegralInRange<size_t>(12, 1024);
        if (txt_data_size <= fdp.remaining_bytes()) {
            std::vector<uint8_t> txt_data = fdp.ConsumeBytes<uint8_t>(txt_data_size);
            struct ares_txt_ext *txt_reply = NULL;
            
            // Call the extended TXT parsing function
            int parse_result = ares_parse_txt_reply_ext(txt_data.data(),
                                                       static_cast<int>(txt_data.size()),
                                                       &txt_reply);
            
            (void)parse_result;
            
            // Clean up if parsing succeeded
            if (txt_reply != NULL) {
                ares_free_data(txt_reply);
            }
        }
    }
    
    // PHASE 5: Cleanup - destroy all created DNS records
    for (size_t i = 0; i < num_records; i++) {
        if (dns_records[i] != NULL) {
            ares_dns_record_destroy(dns_records[i]);
        }
    }
    
    // Final library cleanup
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_006

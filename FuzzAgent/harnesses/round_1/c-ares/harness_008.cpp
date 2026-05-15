#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include "ares.h"
#include "ares_dns.h"
#include "ares_nameser.h"

namespace harness_008 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - we need enough data for basic operations
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Initialize c-ares library
    int init_result = ares_library_init(ARES_LIB_INIT_ALL);
    if (init_result != ARES_SUCCESS) {
        return 0;  // Library initialization failed
    }
    
    // 2. Create a DNS record for testing
    ares_dns_record_t* dns_record = NULL;
    ares_status_t status = ares_dns_record_create(&dns_record, 
                                                  fdp.ConsumeIntegral<uint16_t>(),  // id
                                                  fdp.ConsumeIntegral<uint16_t>(),  // flags
                                                  ARES_OPCODE_QUERY,                // opcode
                                                  ARES_RCODE_NOERROR);              // rcode
    
    if (status != ARES_SUCCESS || dns_record == NULL) {
        ares_library_cleanup();
        return 0;  // Record creation failed
    }
    
    // 3. Test ares_dns_rr_get_keys - enumerate keys for different record types
    // Test all valid record types
    ares_dns_rec_type_t rec_types[] = {
        ARES_REC_TYPE_A,
        ARES_REC_TYPE_NS,
        ARES_REC_TYPE_CNAME,
        ARES_REC_TYPE_SOA,
        ARES_REC_TYPE_PTR,
        ARES_REC_TYPE_HINFO,
        ARES_REC_TYPE_MX,
        ARES_REC_TYPE_TXT,
        ARES_REC_TYPE_SIG,
        ARES_REC_TYPE_AAAA,
        ARES_REC_TYPE_SRV,
        ARES_REC_TYPE_NAPTR,
        ARES_REC_TYPE_OPT,
        ARES_REC_TYPE_TLSA,
        ARES_REC_TYPE_SVCB,
        ARES_REC_TYPE_HTTPS,
        ARES_REC_TYPE_URI,
        ARES_REC_TYPE_CAA,
        ARES_REC_TYPE_RAW_RR
    };
    
    // Also test with fuzzed record types
    size_t rec_type_idx = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(rec_types)/sizeof(rec_types[0]) - 1);
    ares_dns_rec_type_t fuzzed_rec_type = rec_types[rec_type_idx];
    
    size_t key_count = 0;
    const ares_dns_rr_key_t* keys = ares_dns_rr_get_keys(fuzzed_rec_type, &key_count);
    
    // If we got valid keys, test some of them with ares_dns_opt_get_datatype and ares_dns_opt_get_name
    if (keys != NULL && key_count > 0) {
        // Test a few keys (limited to avoid excessive loops)
        size_t keys_to_test = std::min(key_count, (size_t)5);
        for (size_t i = 0; i < keys_to_test; i++) {
            ares_dns_rr_key_t key = keys[i];
            
            // Test ares_dns_opt_get_datatype with fuzzed option values
            unsigned short opt_value = fdp.ConsumeIntegral<uint16_t>();
            ares_dns_opt_datatype_t datatype = ares_dns_opt_get_datatype(key, opt_value);
            (void)datatype; // Use the result
            
            // Test ares_dns_opt_get_name with fuzzed option values
            const char* opt_name = ares_dns_opt_get_name(key, opt_value);
            (void)opt_name; // Use the result
        }
    }
    
    // 4. Test round-trip conversions for record types
    // First, test all known record type strings
    const char* rec_type_strings[] = {
        "A", "NS", "CNAME", "SOA", "PTR", "HINFO", "MX", "TXT",
        "SIG", "AAAA", "SRV", "NAPTR", "OPT", "TLSA", "SVCB", 
        "HTTPS", "URI", "CAA", "ANY"
    };
    
    for (size_t i = 0; i < sizeof(rec_type_strings)/sizeof(rec_type_strings[0]) && fdp.remaining_bytes() > 0; i++) {
        ares_dns_rec_type_t parsed_type;
        ares_bool_t parse_success = ares_dns_rec_type_fromstr(&parsed_type, rec_type_strings[i]);
        
        if (parse_success == ARES_TRUE) {
            // Convert back to string
            const char* type_str = ares_dns_rec_type_tostr(parsed_type);
            (void)type_str; // Use the result
        }
    }
    
    // Also test with fuzzed strings
    if (fdp.remaining_bytes() > 4) {
        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        std::string fuzzed_str = fdp.ConsumeBytesAsString(str_len);
        
        ares_dns_rec_type_t parsed_type;
        ares_bool_t parse_success = ares_dns_rec_type_fromstr(&parsed_type, fuzzed_str.c_str());
        (void)parse_success; // Use the result
    }
    
    // 5. Test round-trip conversions for DNS classes
    // Test all known DNS class strings
    const char* class_strings[] = {
        "IN", "CH", "CHAOS", "HS", "HESIOD", "NONE", "ANY"
    };
    
    for (size_t i = 0; i < sizeof(class_strings)/sizeof(class_strings[0]) && fdp.remaining_bytes() > 0; i++) {
        ares_dns_class_t parsed_class;
        ares_bool_t parse_success = ares_dns_class_fromstr(&parsed_class, class_strings[i]);
        
        if (parse_success == ARES_TRUE) {
            // Convert back to string
            const char* class_str = ares_dns_class_tostr(parsed_class);
            (void)class_str; // Use the result
        }
    }
    
    // Also test with fuzzed strings
    if (fdp.remaining_bytes() > 4) {
        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        std::string fuzzed_str = fdp.ConsumeBytesAsString(str_len);
        
        ares_dns_class_t parsed_class;
        ares_bool_t parse_success = ares_dns_class_fromstr(&parsed_class, fuzzed_str.c_str());
        (void)parse_success; // Use the result
    }
    
    // 6. Test ares_dns_rcode_tostr for all valid response codes
    ares_dns_rcode_t rcodes[] = {
        ARES_RCODE_NOERROR,
        ARES_RCODE_FORMERR,
        ARES_RCODE_SERVFAIL,
        ARES_RCODE_NXDOMAIN,
        ARES_RCODE_NOTIMP,
        ARES_RCODE_REFUSED,
        ARES_RCODE_YXDOMAIN,
        ARES_RCODE_YXRRSET,
        ARES_RCODE_NXRRSET,
        ARES_RCODE_NOTAUTH,
        ARES_RCODE_NOTZONE,
        ARES_RCODE_DSOTYPEI,
        ARES_RCODE_BADSIG,
        ARES_RCODE_BADKEY,
        ARES_RCODE_BADTIME,
        ARES_RCODE_BADMODE,
        ARES_RCODE_BADNAME,
        ARES_RCODE_BADALG,
        ARES_RCODE_BADTRUNC,
        ARES_RCODE_BADCOOKIE
    };
    
    for (size_t i = 0; i < sizeof(rcodes)/sizeof(rcodes[0]); i++) {
        const char* rcode_str = ares_dns_rcode_tostr(rcodes[i]);
        (void)rcode_str; // Use the result
    }
    
    // Also test with fuzzed rcode values
    if (fdp.remaining_bytes() > 2) {
        uint16_t fuzzed_rcode = fdp.ConsumeIntegral<uint16_t>();
        const char* rcode_str = ares_dns_rcode_tostr((ares_dns_rcode_t)fuzzed_rcode);
        (void)rcode_str; // Use the result
    }
    
    // 7. Test ares_dns_section_tostr for all section types
    ares_dns_section_t sections[] = {
        ARES_SECTION_ANSWER,
        ARES_SECTION_AUTHORITY,
        ARES_SECTION_ADDITIONAL
    };
    
    for (size_t i = 0; i < sizeof(sections)/sizeof(sections[0]); i++) {
        const char* section_str = ares_dns_section_tostr(sections[i]);
        (void)section_str; // Use the result
    }
    
    // Also test with fuzzed section values
    if (fdp.remaining_bytes() > 1) {
        uint8_t fuzzed_section = fdp.ConsumeIntegral<uint8_t>();
        const char* section_str = ares_dns_section_tostr((ares_dns_section_t)fuzzed_section);
        (void)section_str; // Use the result
    }
    
    // 8. Test other mapping functions for completeness
    // Test ares_dns_opcode_tostr
    if (fdp.remaining_bytes() > 1) {
        uint8_t fuzzed_opcode = fdp.ConsumeIntegral<uint8_t>();
        const char* opcode_str = ares_dns_opcode_tostr((ares_dns_opcode_t)fuzzed_opcode);
        (void)opcode_str; // Use the result
    }
    
    // Test ares_dns_rr_key_datatype
    if (keys != NULL && key_count > 0 && fdp.remaining_bytes() > 0) {
        size_t key_idx = fdp.ConsumeIntegralInRange<size_t>(0, key_count - 1);
        ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(keys[key_idx]);
        (void)datatype; // Use the result
    }
    
    // Test ares_dns_rr_key_to_rec_type
    if (keys != NULL && key_count > 0 && fdp.remaining_bytes() > 0) {
        size_t key_idx = fdp.ConsumeIntegralInRange<size_t>(0, key_count - 1);
        ares_dns_rec_type_t rec_type = ares_dns_rr_key_to_rec_type(keys[key_idx]);
        (void)rec_type; // Use the result
    }
    
    // 9. Clean up
    ares_dns_record_destroy(dns_record);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_008

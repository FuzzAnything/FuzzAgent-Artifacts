#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include "ares.h"
#include "ares_dns_record.h"
#include "ares_nameser.h"

namespace harness_011 {


// Custom memory functions for ares_library_init_mem
static void *custom_malloc(size_t size) {
    return malloc(size);
}

static void custom_free(void *ptr) {
    free(ptr);
}

static void *custom_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

// Simple query callback for DNS queries
static void query_callback(void *arg, int status, int timeouts, 
                          const unsigned char *abuf, int alen) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
    // Simple callback - does nothing, just satisfies API requirements
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for initialization and testing
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize c-ares library with custom memory functions
    // First try standard initialization
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // Then try with custom memory functions
    ares_library_init_mem(ARES_LIB_INIT_ALL, custom_malloc, custom_free, custom_realloc);
    
    // Step 2: Initialize ares channel for threading tests
    ares_channel_t* channel = NULL;
    int init_result = ares_init(&channel);
    if (init_result != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Step 3: Create a DNS record for testing mapping functions
    ares_dns_record_t *dns_record = NULL;
    ares_dns_record_create(&dns_record, 0, ARES_FLAG_QR, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (dns_record == NULL) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }
    
    // Step 4: Add query sections to the DNS record
    // Consume domain names for queries
    std::string query_name1 = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
    std::string query_name2 = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
    
    ares_dns_record_query_add(dns_record, query_name1.c_str(), ARES_REC_TYPE_A, ARES_CLASS_IN);
    ares_dns_record_query_add(dns_record, query_name2.c_str(), ARES_REC_TYPE_AAAA, ARES_CLASS_IN);
    
    // Step 5: Add resource records to different sections
    // Answer section
    std::string answer_name = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
    ares_dns_rr_t *answer_rr = NULL;
    ares_dns_record_rr_add(&answer_rr, dns_record, ARES_SECTION_ANSWER, 
                          answer_name.c_str(), ARES_REC_TYPE_A, ARES_CLASS_IN, 300);
    
    // Authority section  
    std::string authority_name = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
    ares_dns_rr_t *authority_rr = NULL;
    ares_dns_record_rr_add(&authority_rr, dns_record, ARES_SECTION_AUTHORITY,
                          authority_name.c_str(), ARES_REC_TYPE_NS, ARES_CLASS_IN, 3600);
    
    // Additional section
    std::string additional_name = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
    ares_dns_rr_t *additional_rr = NULL;
    ares_dns_record_rr_add(&additional_rr, dns_record, ARES_SECTION_ADDITIONAL,
                          additional_name.c_str(), ARES_REC_TYPE_OPT, ARES_CLASS_IN, 0);
    
    // Step 6: Test DNS string conversion functions
    
    // Test ares_dns_section_tostr with all section types
    const char *section_str_answer = ares_dns_section_tostr(ARES_SECTION_ANSWER);
    const char *section_str_authority = ares_dns_section_tostr(ARES_SECTION_AUTHORITY);
    const char *section_str_additional = ares_dns_section_tostr(ARES_SECTION_ADDITIONAL);
    
    // Test with invalid section value (edge case)
    const char *section_str_invalid = ares_dns_section_tostr((ares_dns_section_t)999);
    (void)section_str_invalid;
    
    // Test ares_dns_class_fromstr with various class strings
    ares_dns_class_t class_result;
    const char *class_strings[] = {"IN", "CHAOS", "HESIOD", "HESOID", "NONE", "ANY", "invalid"};
    
    for (size_t i = 0; i < sizeof(class_strings)/sizeof(class_strings[0]); i++) {
        if (fdp.remaining_bytes() > 0) {
            ares_dns_class_fromstr(&class_result, class_strings[i]);
        }
    }
    
    // Also test with fuzzer-generated strings
    if (fdp.remaining_bytes() > 10) {
        std::string fuzz_class_str = fdp.ConsumeRandomLengthString(20);
        ares_dns_class_fromstr(&class_result, fuzz_class_str.c_str());
    }
    
    // Test ares_dns_rec_type_fromstr with various type strings
    ares_dns_rec_type_t type_result;
    const char *type_strings[] = {"A", "AAAA", "MX", "TXT", "CNAME", "NS", "SOA", "PTR", 
                                 "SRV", "CAA", "URI", "TLSA", "SVCB", "HTTPS", "ANY", 
                                 "RAW_RR", "invalid"};
    
    for (size_t i = 0; i < sizeof(type_strings)/sizeof(type_strings[0]); i++) {
        if (fdp.remaining_bytes() > 0) {
            ares_dns_rec_type_fromstr(&type_result, type_strings[i]);
        }
    }
    
    // Also test with fuzzer-generated strings
    if (fdp.remaining_bytes() > 10) {
        std::string fuzz_type_str = fdp.ConsumeRandomLengthString(20);
        ares_dns_rec_type_fromstr(&type_result, fuzz_type_str.c_str());
    }
    
    // Step 7: Test option getter functions
    
    // Test ares_dns_opt_get_datatype with various RR keys and option values
    // First get keys for OPT record type
    size_t opt_key_count = 0;
    const ares_dns_rr_key_t *opt_keys = ares_dns_rr_get_keys(ARES_REC_TYPE_OPT, &opt_key_count);
    
    if (opt_keys != NULL && opt_key_count > 0) {
        // Test with various option values
        unsigned short option_values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
                                         ARES_OPT_PARAM_LLQ, ARES_OPT_PARAM_UL,
                                         ARES_OPT_PARAM_NSID, ARES_OPT_PARAM_DAU,
                                         ARES_OPT_PARAM_DHU, ARES_OPT_PARAM_N3U,
                                         ARES_OPT_PARAM_EDNS_CLIENT_SUBNET,
                                         ARES_OPT_PARAM_EDNS_EXPIRE,
                                         ARES_OPT_PARAM_COOKIE,
                                         ARES_OPT_PARAM_EDNS_TCP_KEEPALIVE,
                                         ARES_OPT_PARAM_PADDING,
                                         ARES_OPT_PARAM_CHAIN,
                                         ARES_OPT_PARAM_EDNS_KEY_TAG,
                                         ARES_OPT_PARAM_EXTENDED_DNS_ERROR};
        
        for (size_t i = 0; i < opt_key_count && i < 5; i++) { // Limit to 5 keys for safety
            for (size_t j = 0; j < sizeof(option_values)/sizeof(option_values[0]) && j < 10; j++) {
                ares_dns_opt_get_datatype(opt_keys[i], option_values[j]);
            }
        }
    }
    
    // Test with SVCB/HTTPS record types as well
    size_t svcb_key_count = 0;
    const ares_dns_rr_key_t *svcb_keys = ares_dns_rr_get_keys(ARES_REC_TYPE_SVCB, &svcb_key_count);
    
    if (svcb_keys != NULL && svcb_key_count > 0) {
        unsigned short svcb_options[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                                        ARES_SVCB_PARAM_NO_DEFAULT_ALPN,
                                        ARES_SVCB_PARAM_ECH,
                                        ARES_SVCB_PARAM_MANDATORY,
                                        ARES_SVCB_PARAM_ALPN,
                                        ARES_SVCB_PARAM_PORT,
                                        ARES_SVCB_PARAM_IPV4HINT,
                                        ARES_SVCB_PARAM_IPV6HINT,
                                        ARES_SVCB_PARAM_DOHPATH,
                                        ARES_SVCB_PARAM_OHTTP};
        
        for (size_t i = 0; i < svcb_key_count && i < 5; i++) {
            for (size_t j = 0; j < sizeof(svcb_options)/sizeof(svcb_options[0]) && j < 10; j++) {
                ares_dns_opt_get_datatype(svcb_keys[i], svcb_options[j]);
            }
        }
    }
    
    // Test ares_dns_opt_get_name with various RR keys and option values
    if (opt_keys != NULL && opt_key_count > 0) {
        for (size_t i = 0; i < opt_key_count && i < 5; i++) {
            for (unsigned short opt_val = 0; opt_val < 20; opt_val++) {
                if (fdp.remaining_bytes() > 0) {
                    const char *opt_name = ares_dns_opt_get_name(opt_keys[i], opt_val);
                    (void)opt_name;
                }
            }
        }
    }
    
    // Test with random option values from fuzzer input
    if (fdp.remaining_bytes() > 10) {
        for (int i = 0; i < 10 && fdp.remaining_bytes() > 2; i++) {
            unsigned short random_opt = fdp.ConsumeIntegral<uint16_t>();
            if (opt_keys != NULL && opt_key_count > 0) {
                const char *random_opt_name = ares_dns_opt_get_name(opt_keys[0], random_opt);
                (void)random_opt_name;
            }
        }
    }
    
    // Step 8: Test threading function ares_queue_wait_empty
    // First, create some queries to potentially have something in the queue
    if (fdp.remaining_bytes() > 20) {
        // Consume hostnames for queries
        std::string query_host1 = fdp.ConsumeRandomLengthString(100);
        std::string query_host2 = fdp.ConsumeRandomLengthString(100);
        
        // Perform some queries (they'll be async)
        ares_query(channel, query_host1.c_str(), C_IN, T_A, query_callback, NULL);
        ares_query(channel, query_host2.c_str(), C_IN, T_AAAA, query_callback, NULL);
        
        // Test ares_queue_wait_empty with various timeout values
        int timeout_values[] = {-1, 0, 1, 10, 100, 1000};
        
        for (size_t i = 0; i < sizeof(timeout_values)/sizeof(timeout_values[0]); i++) {
            ares_queue_wait_empty(channel, timeout_values[i]);
        }
        
        // Also test with random timeout from fuzzer
        if (fdp.remaining_bytes() > 4) {
            int random_timeout = fdp.ConsumeIntegral<int>();
            ares_queue_wait_empty(channel, random_timeout);
        }
        
        // Cancel queries to clean up
        ares_cancel(channel);
    }
    
    // Step 9: Test ares_dns_parse and ares_dns_write with fuzzer data
    if (fdp.remaining_bytes() > 50) {
        // Consume some bytes for DNS wire format testing
        size_t wire_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
        std::vector<uint8_t> wire_data = fdp.ConsumeBytes<uint8_t>(wire_size);
        
        // Try to parse the wire data as a DNS record
        ares_dns_record_t *parsed_record = NULL;
        // Use 0 for flags (no special parsing options) or combine flags from ares_dns_parse_flags_t
        unsigned int parse_flags = 0;
        if (fdp.ConsumeBool()) {
            parse_flags |= ARES_DNS_PARSE_AN_BASE_RAW;
        }
        if (fdp.ConsumeBool()) {
            parse_flags |= ARES_DNS_PARSE_NS_BASE_RAW;
        }
        if (fdp.ConsumeBool()) {
            parse_flags |= ARES_DNS_PARSE_AR_BASE_RAW;
        }
        
        ares_dns_parse(wire_data.data(), wire_size, parse_flags, &parsed_record);
        
        if (parsed_record != NULL) {
            // If parsing succeeded, try to write it back
            size_t output_size = 0;
            unsigned char *output_buf = NULL;
            ares_dns_write(parsed_record, &output_buf, &output_size);
            
            // Clean up output buffer if allocated
            if (output_buf != NULL) {
                ares_free_data(output_buf);
            }
            
            // Destroy the parsed record
            ares_dns_record_destroy(parsed_record);
        }
    }
    
    // Step 10: Test ares_queue_active_queries (mentioned in guidance as low-complexity)
    if (channel != NULL) {
        size_t active_queries = ares_queue_active_queries(channel);
        (void)active_queries;
    }
    
    // Step 11: Test ares_version (mentioned in guidance)
    const char *version_str = ares_version(NULL);
    (void)version_str;
    
    // Step 12: Clean up
    ares_dns_record_destroy(dns_record);
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_011

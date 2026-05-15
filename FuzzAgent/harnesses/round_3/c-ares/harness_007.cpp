#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ares.h"
#include "ares_nameser.h"

namespace harness_007 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for initialization and basic parameters
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // Step 2: Test ares_create_query with various parameters
    // Consume DNS query name (domain name)
    std::string query_name = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
    
    // Consume DNS class (common values: IN, CHAOS, HS)
    int dns_class_vals[] = {ns_c_in, ns_c_chaos, ns_c_hs, ns_c_any, ns_c_none};
    int dns_class = fdp.PickValueInArray(dns_class_vals);
    
    // Consume DNS type (common record types)
    int dns_type_vals[] = {ns_t_a, ns_t_aaaa, ns_t_mx, ns_t_txt, ns_t_ptr, 
                          ns_t_cname, ns_t_ns, ns_t_soa, ns_t_srv, ns_t_caa};
    int dns_type = fdp.PickValueInArray(dns_type_vals);
    
    // Consume query ID
    unsigned short query_id = fdp.ConsumeIntegral<unsigned short>();
    
    // Consume recursion desired flag
    int rd_flag = fdp.ConsumeBool() ? 1 : 0;
    
    // Consume max UDP size (0 = use default, >0 = specific size)
    int max_udp_size = fdp.ConsumeIntegralInRange<int>(0, 4096);
    
    // Test ares_create_query
    unsigned char* query_buf = NULL;
    int query_buflen = 0;
    
    int create_query_result = ares_create_query(
        query_name.c_str(), 
        dns_class, 
        dns_type,
        query_id,
        rd_flag,
        &query_buf,
        &query_buflen,
        max_udp_size
    );
    
    // Use ares_strerror to convert result to string
    const char* error_str = ares_strerror(create_query_result);
    (void)error_str; // Avoid unused variable warning
    
    // Free the buffer if allocated
    if (query_buf != NULL) {
        free(query_buf);
        query_buf = NULL;
    }
    
    // Step 3: Test ares_mkquery (similar to ares_create_query but without max_udp_size)
    if (fdp.remaining_bytes() > 32) {
        // Consume new parameters for mkquery
        std::string mkquery_name = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
        int mkquery_class = fdp.PickValueInArray(dns_class_vals);
        int mkquery_type = fdp.PickValueInArray(dns_type_vals);
        unsigned short mkquery_id = fdp.ConsumeIntegral<unsigned short>();
        int mkquery_rd = fdp.ConsumeBool() ? 1 : 0;
        
        unsigned char* mkquery_buf = NULL;
        int mkquery_buflen = 0;
        
        int mkquery_result = ares_mkquery(
            mkquery_name.c_str(),
            mkquery_class,
            mkquery_type,
            mkquery_id,
            mkquery_rd,
            &mkquery_buf,
            &mkquery_buflen
        );
        
        // Test ares_strerror with mkquery result
        const char* mkquery_error_str = ares_strerror(mkquery_result);
        (void)mkquery_error_str;
        
        // Test error path: call ares_strerror with various error codes
        int error_codes[] = {ARES_SUCCESS, ARES_ENODATA, ARES_EFORMERR, 
                            ARES_ESERVFAIL, ARES_ENOTFOUND, ARES_ENOMEM,
                            ARES_ECONNREFUSED, ARES_ETIMEOUT, ARES_EOF};
        int test_error_code = fdp.PickValueInArray(error_codes);
        const char* test_error_str = ares_strerror(test_error_code);
        (void)test_error_str;
        
        if (mkquery_buf != NULL) {
            free(mkquery_buf);
            mkquery_buf = NULL;
        }
    }
    
    // Step 4: Test ares_expand_name with the query buffer if we created one
    // First create a simple DNS query to test expansion
    if (fdp.remaining_bytes() > 100) {
        // Create a test query for expansion functions
        unsigned char* test_buf = NULL;
        int test_buflen = 0;
        
        // Use a simple domain name for testing
        std::string test_name = "www.example.com";
        
        int test_result = ares_create_query(
            test_name.c_str(),
            ns_c_in,
            ns_t_a,
            0x1234,
            1,
            &test_buf,
            &test_buflen,
            512
        );
        
        if (test_result == ARES_SUCCESS && test_buf != NULL && test_buflen > 0) {
            // Test ares_expand_name with the query buffer
            // The encoded name starts at offset 12 (after header)
            if (test_buflen > 20) {
                const unsigned char* encoded = test_buf + 12; // Start of question section
                const unsigned char* abuf = test_buf;
                int alen = test_buflen;
                char* expanded_name = NULL;
                long enclen = 0;
                
                // Try to expand the name from the query
                int expand_result = ares_expand_name(encoded, abuf, alen, &expanded_name, &enclen);
                
                // Check ares_strerror for expansion result
                const char* expand_error_str = ares_strerror(expand_result);
                (void)expand_error_str;
                
                if (expanded_name != NULL) {
                    free(expanded_name);
                    expanded_name = NULL;
                }
                
                // Test ares_expand_name with malformed/truncated data
                if (fdp.remaining_bytes() > 50) {
                    // Create malformed data for testing edge cases
                    size_t malformed_size = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                    std::vector<uint8_t> malformed_data = fdp.ConsumeBytes<uint8_t>(malformed_size);
                    
                    if (malformed_data.size() > 0) {
                        char* malformed_expanded = NULL;
                        long malformed_enclen = 0;
                        
                        // Test with truncated/random data
                        int malformed_result = ares_expand_name(
                            malformed_data.data(),
                            malformed_data.data(),
                            malformed_data.size(),
                            &malformed_expanded,
                            &malformed_enclen
                        );
                        
                        // Check error string
                        const char* malformed_error_str = ares_strerror(malformed_result);
                        (void)malformed_error_str;
                        
                        if (malformed_expanded != NULL) {
                            free(malformed_expanded);
                            malformed_expanded = NULL;
                        }
                    }
                }
            }
            
            // Test ares_expand_string with the query buffer
            // Try to expand strings from various offsets in the buffer
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                int offset = fdp.ConsumeIntegralInRange<int>(0, test_buflen - 1);
                unsigned char* expanded_string = NULL;
                long string_enclen = 0;
                
                int string_result = ares_expand_string(
                    test_buf + offset,
                    test_buf,
                    test_buflen,
                    &expanded_string,
                    &string_enclen
                );
                
                // Check error string
                const char* string_error_str = ares_strerror(string_result);
                (void)string_error_str;
                
                if (expanded_string != NULL) {
                    free(expanded_string);
                    expanded_string = NULL;
                }
            }
            
            free(test_buf);
            test_buf = NULL;
        }
    }
    
    // Step 5: Test ares_expand_name and ares_expand_string with compressed DNS data
    // Create compressed name data for testing
    if (fdp.remaining_bytes() > 100) {
        // Build a simple DNS message with compression
        std::vector<unsigned char> dns_message;
        
        // DNS header (12 bytes)
        for (int i = 0; i < 12; i++) {
            dns_message.push_back(fdp.ConsumeIntegral<uint8_t>());
        }
        
        // Question section with possible compression
        // Add a label: "www" (3 bytes + length)
        dns_message.push_back(3); // Length of "www"
        std::string label1 = "www";
        dns_message.insert(dns_message.end(), label1.begin(), label1.end());
        
        // Add compression pointer (0xc0 0x0c) pointing to offset 12
        dns_message.push_back(0xc0);
        dns_message.push_back(0x0c);
        
        // Type and class
        dns_message.push_back(0x00); // Type high byte
        dns_message.push_back(0x01); // Type low byte (A record)
        dns_message.push_back(0x00); // Class high byte
        dns_message.push_back(0x01); // Class low byte (IN)
        
        if (dns_message.size() > 20) {
            char* compressed_expanded = NULL;
            long compressed_enclen = 0;
            
            // Try to expand the compressed name
            int compressed_result = ares_expand_name(
                dns_message.data() + 12, // Start of question name
                dns_message.data(),
                dns_message.size(),
                &compressed_expanded,
                &compressed_enclen
            );
            
            // Check error string
            const char* compressed_error_str = ares_strerror(compressed_result);
            (void)compressed_error_str;
            
            if (compressed_expanded != NULL) {
                free(compressed_expanded);
                compressed_expanded = NULL;
            }
            
            // Test ares_expand_string with the same data
            unsigned char* string_result_buf = NULL;
            long string_enclen = 0;
            
            int string_expand_result = ares_expand_string(
                dns_message.data() + 12,
                dns_message.data(),
                dns_message.size(),
                &string_result_buf,
                &string_enclen
            );
            
            // Check error string
            const char* string_expand_error_str = ares_strerror(string_expand_result);
            (void)string_expand_error_str;
            
            if (string_result_buf != NULL) {
                free(string_result_buf);
                string_result_buf = NULL;
            }
        }
    }
    
    // Step 6: Test error paths with invalid parameters
    // Test ares_create_query with NULL parameters
    if (fdp.ConsumeBool()) {
        unsigned char* null_buf = NULL;
        int null_buflen = 0;
        
        // Should fail with ARES_EFORMERR
        int null_result = ares_create_query(
            NULL, // Invalid: NULL name
            ns_c_in,
            ns_t_a,
            0x1234,
            1,
            &null_buf,
            &null_buflen,
            512
        );
        
        const char* null_error_str = ares_strerror(null_result);
        (void)null_error_str;
        
        // Test with NULL buffer pointer
        int null_buf_result = ares_create_query(
            "test.example.com",
            ns_c_in,
            ns_t_a,
            0x1234,
            1,
            NULL, // Invalid: NULL buffer pointer
            &null_buflen,
            512
        );
        
        const char* null_buf_error_str = ares_strerror(null_buf_result);
        (void)null_buf_error_str;
        
        // Test with NULL buffer length pointer
        int null_len_result = ares_create_query(
            "test.example.com",
            ns_c_in,
            ns_t_a,
            0x1234,
            1,
            &null_buf,
            NULL, // Invalid: NULL buffer length pointer
            512
        );
        
        const char* null_len_error_str = ares_strerror(null_len_result);
        (void)null_len_error_str;
    }
    
    // Step 7: Test boundary conditions for expansion functions
    if (fdp.remaining_bytes() > 20) {
        // Test with zero-length buffer
        unsigned char zero_buf[1] = {0};
        char* zero_expanded = NULL;
        long zero_enclen = 0;
        
        int zero_result = ares_expand_name(
            zero_buf,
            zero_buf,
            0, // Zero length
            &zero_expanded,
            &zero_enclen
        );
        
        const char* zero_error_str = ares_strerror(zero_result);
        (void)zero_error_str;
        
        // Test with single byte buffer (invalid)
        unsigned char single_buf[1] = {0x01}; // Length byte without data
        char* single_expanded = NULL;
        long single_enclen = 0;
        
        int single_result = ares_expand_name(
            single_buf,
            single_buf,
            1,
            &single_expanded,
            &single_enclen
        );
        
        const char* single_error_str = ares_strerror(single_result);
        (void)single_error_str;
    }
    
    // Step 8: Test ares_strerror with all possible error codes
    // Test a wide range of error codes to cover the function
    int all_error_codes[] = {
        ARES_SUCCESS,
        ARES_ENODATA,
        ARES_EFORMERR,
        ARES_ESERVFAIL,
        ARES_ENOTFOUND,
        ARES_ENOTIMP,
        ARES_EREFUSED,
        ARES_EBADQUERY,
        ARES_EBADNAME,
        ARES_EBADFAMILY,
        ARES_EBADRESP,
        ARES_ECONNREFUSED,
        ARES_ETIMEOUT,
        ARES_EOF,
        ARES_EFILE,
        ARES_ENOMEM,
        ARES_EDESTRUCTION,
        ARES_EBADSTR,
        ARES_EBADFLAGS,
        ARES_ENONAME,
        ARES_EBADHINTS,
        ARES_ENOTINITIALIZED,
        ARES_ELOADIPHLPAPI,
        ARES_EADDRGETNETWORKPARAMS,
        ARES_ECANCELLED,
        ARES_ESERVICE,
        ARES_ENOSERVER
    };
    
    // Test a few random error codes from the fuzzer input
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 4; i++) {
        int array_size = sizeof(all_error_codes) / sizeof(all_error_codes[0]);
        int error_idx = fdp.ConsumeIntegralInRange<int>(0, array_size - 1);
        const char* error_msg = ares_strerror(all_error_codes[error_idx]);
        (void)error_msg;
    }
    
    return 0;
}

} // namespace harness_007

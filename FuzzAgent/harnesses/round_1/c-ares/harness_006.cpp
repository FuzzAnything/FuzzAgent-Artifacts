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

namespace harness_006 {


// Fuzzer entry point for legacy DNS reply parsing functions
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for meaningful testing
    // We need enough for DNS header (12 bytes) plus some data
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize library (required for memory allocation functions)
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // Test all parsing functions with the raw input data first
    // This tests the error handling paths for malformed data
    
    // 1. Test ares_parse_caa_reply with raw data
    struct ares_caa_reply *caa_out = NULL;
    int caa_result = ares_parse_caa_reply(data, (int)size, &caa_out);
    if (caa_out != NULL) {
        ares_free_data(caa_out);
    }
    
    // 2. Test ares_parse_naptr_reply with raw data  
    struct ares_naptr_reply *naptr_out = NULL;
    int naptr_result = ares_parse_naptr_reply(data, (int)size, &naptr_out);
    if (naptr_out != NULL) {
        ares_free_data(naptr_out);
    }
    
    // 3. Test ares_parse_uri_reply with raw data
    struct ares_uri_reply *uri_out = NULL;
    int uri_result = ares_parse_uri_reply(data, (int)size, &uri_out);
    if (uri_out != NULL) {
        ares_free_data(uri_out);
    }
    
    // 4. Test ares_parse_txt_reply_ext with raw data
    struct ares_txt_ext *txt_ext_out = NULL;
    int txt_ext_result = ares_parse_txt_reply_ext(data, (int)size, &txt_ext_out);
    if (txt_ext_out != NULL) {
        ares_free_data(txt_ext_out);
    }
    
    // 5. Test ares_parse_txt_reply with raw data
    struct ares_txt_reply *txt_out = NULL;
    int txt_result = ares_parse_txt_reply(data, (int)size, &txt_out);
    if (txt_out != NULL) {
        ares_free_data(txt_out);
    }
    
    // Now create modified buffers with proper DNS structure to test successful parsing paths
    // Split remaining input for constructing DNS-like buffers
    
    if (fdp.remaining_bytes() > 64) {
        // Create a buffer that looks more like a valid DNS response
        size_t dns_buf_size = fdp.ConsumeIntegralInRange<size_t>(32, 512);
        if (dns_buf_size <= fdp.remaining_bytes()) {
            std::vector<uint8_t> dns_buffer = fdp.ConsumeBytes<uint8_t>(dns_buf_size);
            
            // Ensure minimum DNS header size
            if (dns_buffer.size() >= 12) {
                // Set response flag (bit 7) and authoritative answer (bit 5)
                dns_buffer[2] = 0x81;  // Response + authoritative
                dns_buffer[3] = 0x80;  // Recursion available
                
                // Set question count to 1
                dns_buffer[4] = 0x00;
                dns_buffer[5] = 0x01;
                
                // Set answer count based on fuzzed data
                uint8_t answer_count = fdp.ConsumeIntegral<uint8_t>() % 4;
                dns_buffer[6] = 0x00;
                dns_buffer[7] = answer_count;
                
                // Try parsing with modified buffer
                struct ares_caa_reply *caa_modified = NULL;
                ares_parse_caa_reply(dns_buffer.data(), (int)dns_buffer.size(), &caa_modified);
                if (caa_modified != NULL) {
                    ares_free_data(caa_modified);
                }
                
                // Also try with other parsers
                struct ares_naptr_reply *naptr_modified = NULL;
                ares_parse_naptr_reply(dns_buffer.data(), (int)dns_buffer.size(), &naptr_modified);
                if (naptr_modified != NULL) {
                    ares_free_data(naptr_modified);
                }
                
                struct ares_uri_reply *uri_modified = NULL;
                ares_parse_uri_reply(dns_buffer.data(), (int)dns_buffer.size(), &uri_modified);
                if (uri_modified != NULL) {
                    ares_free_data(uri_modified);
                }
            }
        }
    }
    
    // Test edge cases with various buffer sizes
    
    // Test with size = 0
    struct ares_caa_reply *caa_zero = NULL;
    ares_parse_caa_reply(data, 0, &caa_zero);
    
    // Test with negative size (should fail gracefully)
    struct ares_naptr_reply *naptr_neg = NULL;
    ares_parse_naptr_reply(data, -1, &naptr_neg);
    
    // Test with NULL buffer
    struct ares_uri_reply *uri_null = NULL;
    ares_parse_uri_reply(NULL, 0, &uri_null);
    
    // Test with small buffers
    if (size > 4) {
        uint8_t small_buf[4];
        memcpy(small_buf, data, 4);
        struct ares_txt_ext *txt_small = NULL;
        ares_parse_txt_reply_ext(small_buf, 4, &txt_small);
    }
    
    // Test with exactly DNS header size
    if (size >= 12) {
        uint8_t header_buf[12];
        memcpy(header_buf, data, 12);
        struct ares_caa_reply *caa_header = NULL;
        ares_parse_caa_reply(header_buf, 12, &caa_header);
    }
    
    // Clean up library
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_006

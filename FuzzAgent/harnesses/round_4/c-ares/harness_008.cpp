#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include "ares.h"
#include "ares_nameser.h"

namespace harness_008 {
/* Fuzzing harness for c-ares library targeting legacy DNS query creation and name expansion functions
 * This harness tests legacy DNS query creation APIs and name/string expansion functions
 * Target APIs: ares_mkquery (111 undiscovered branches), ares_create_query (111),
 *              ares_expand_name (8), ares_expand_string (8)
 * Required helper APIs: ares_library_init (initialization), ares_library_cleanup (cleanup)
 * Following invocation sequence from coverage guidance:
 * 1. ares_library_init() to initialize the c-ares library.
 * 2. ares_mkquery() with various DNS names, classes, types, and IDs to create legacy DNS queries.
 * 3. ares_create_query() with additional max_udp_size parameter to test UDP size limits.
 * 4. Use ares_expand_name() to parse and expand DNS names from the created query buffers.
 * 5. Use ares_expand_string() to parse and expand DNS strings from query buffers.
 * 6. ares_library_cleanup() to properly clean up library resources.
 * Differentiates from previous harnesses by focusing on legacy DNS query creation and expansion.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    // We need enough data for: DNS names, query parameters, and buffer sizes
    if (size < 50) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Initialize c-ares library
    int init_result = ares_library_init(ARES_LIB_INIT_ALL);
    if (init_result != ARES_SUCCESS) {
        return 0;
    }

    // Generate DNS name from fuzzer input
    std::string dns_name = fdp.ConsumeRandomLengthString(255);
    if (dns_name.empty()) {
        // If empty, generate a small name from remaining bytes
        dns_name = fdp.ConsumeBytesAsString(10);
        if (dns_name.empty()) {
            // If still empty, use a single character
            dns_name = "a";
        }
    }

    // Consume query parameters from fuzzer input
    int dns_class = fdp.ConsumeIntegralInRange<int>(C_IN, C_HS);
    int query_type = fdp.ConsumeIntegralInRange<int>(T_A, T_URI);
    unsigned short query_id = fdp.ConsumeIntegral<unsigned short>();
    int recursion_desired = fdp.ConsumeBool() ? 1 : 0;

    unsigned char* query_buf_mkquery = NULL;
    int query_buflen_mkquery = 0;

    // Call ares_mkquery with fuzzer-generated parameters
    int mkquery_result = ares_mkquery(
        dns_name.c_str(),
        dns_class,
        query_type,
        query_id,
        recursion_desired,
        &query_buf_mkquery,
        &query_buflen_mkquery
    );

    // Generate another DNS name for create_query
    std::string dns_name2 = fdp.ConsumeRandomLengthString(255);
    if (dns_name2.empty()) {
        // If empty, generate a small name from remaining bytes
        dns_name2 = fdp.ConsumeBytesAsString(10);
        if (dns_name2.empty()) {
            // If still empty, use a single character
            dns_name2 = "b";
        }
    }

    // Consume additional parameters for create_query
    int dns_class2 = fdp.ConsumeIntegralInRange<int>(C_IN, C_HS);
    int query_type2 = fdp.ConsumeIntegralInRange<int>(T_A, T_URI);
    unsigned short query_id2 = fdp.ConsumeIntegral<unsigned short>();
    int recursion_desired2 = fdp.ConsumeBool() ? 1 : 0;
    int max_udp_size = fdp.ConsumeIntegralInRange<int>(0, 65535);

    unsigned char* query_buf_create = NULL;
    int query_buflen_create = 0;

    // Call ares_create_query with fuzzer-generated parameters including max_udp_size
    int create_query_result = ares_create_query(
        dns_name2.c_str(),
        dns_class2,
        query_type2,
        query_id2,
        recursion_desired2,
        &query_buf_create,
        &query_buflen_create,
        max_udp_size
    );

    // Step 4: Use ares_expand_name() to parse and expand DNS names from query buffers
    if (mkquery_result == ARES_SUCCESS && query_buf_mkquery != NULL && query_buflen_mkquery > 12) {
        // Skip the DNS header (12 bytes) to get to the question section
        const unsigned char* encoded_name = query_buf_mkquery + 12;
        
        char* expanded_name = NULL;
        long enclen = 0;
        
        // Try to expand the name from the query buffer
        int expand_name_result = ares_expand_name(
            encoded_name,
            query_buf_mkquery,
            query_buflen_mkquery,
            &expanded_name,
            &enclen
        );
        
        if (expand_name_result == ARES_SUCCESS && expanded_name != NULL) {
            // Name was successfully expanded - clean it up
            ares_free_string(expanded_name);
        }
    }

    if (create_query_result == ARES_SUCCESS && query_buf_create != NULL && query_buflen_create > 12) {
        // Skip the DNS header (12 bytes) to get to the question section
        const unsigned char* encoded_name2 = query_buf_create + 12;
        
        char* expanded_name2 = NULL;
        long enclen2 = 0;
        
        // Try to expand the name from the create_query buffer
        int expand_name_result2 = ares_expand_name(
            encoded_name2,
            query_buf_create,
            query_buflen_create,
            &expanded_name2,
            &enclen2
        );
        
        if (expand_name_result2 == ARES_SUCCESS && expanded_name2 != NULL) {
            // Name was successfully expanded - clean it up
            ares_free_string(expanded_name2);
        }
    }

    // Step 5: Use ares_expand_string() to parse and expand DNS strings from query buffers
    // For string expansion, we need to find a suitable location in the buffer
    // Try expanding from various positions in the buffer
    if (mkquery_result == ARES_SUCCESS && query_buf_mkquery != NULL && query_buflen_mkquery > 0) {
        // Try expanding from different positions in the buffer
        for (int pos = 0; pos < query_buflen_mkquery && pos < 20; pos++) {
            const unsigned char* encoded_string = query_buf_mkquery + pos;
            unsigned char* expanded_string = NULL;
            long string_enclen = 0;
            
            int expand_string_result = ares_expand_string(
                encoded_string,
                query_buf_mkquery,
                query_buflen_mkquery,
                &expanded_string,
                &string_enclen
            );
            
            if (expand_string_result == ARES_SUCCESS && expanded_string != NULL) {
                // String was successfully expanded - clean it up
                ares_free_string(expanded_string);
                break; // Found a valid string, move on
            }
        }
    }

    if (create_query_result == ARES_SUCCESS && query_buf_create != NULL && query_buflen_create > 0) {
        // Try expanding from different positions in the create_query buffer
        for (int pos = 0; pos < query_buflen_create && pos < 20; pos++) {
            const unsigned char* encoded_string2 = query_buf_create + pos;
            unsigned char* expanded_string2 = NULL;
            long string_enclen2 = 0;
            
            int expand_string_result2 = ares_expand_string(
                encoded_string2,
                query_buf_create,
                query_buflen_create,
                &expanded_string2,
                &string_enclen2
            );
            
            if (expand_string_result2 == ARES_SUCCESS && expanded_string2 != NULL) {
                // String was successfully expanded - clean it up
                ares_free_string(expanded_string2);
                break; // Found a valid string, move on
            }
        }
    }

    // Clean up allocated query buffers
    if (query_buf_mkquery != NULL) {
        ares_free_string(query_buf_mkquery);
    }
    
    if (query_buf_create != NULL) {
        ares_free_string(query_buf_create);
    }

    // Step 6: Clean up library resources
    ares_library_cleanup();

    return 0;
}

} // namespace harness_008

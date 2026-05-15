#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/urlapi.h>

namespace harness_020 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for basic configuration
    if (size < 32) {
        return 0; // Need minimum input for meaningful URL testing
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Initialize curl globally
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Step 2: Create URL handle using curl_url()
    CURLU* url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Consume some data for general configuration
    (void)fdp.ConsumeIntegral<uint8_t>(); // Previously url_config, now unused
    
    // Array of all URL parts to test
    CURLUPart url_parts[] = {
        CURLUPART_URL,
        CURLUPART_SCHEME,
        CURLUPART_USER,
        CURLUPART_PASSWORD,
        CURLUPART_OPTIONS,
        CURLUPART_HOST,
        CURLUPART_PORT,
        CURLUPART_PATH,
        CURLUPART_QUERY,
        CURLUPART_FRAGMENT,
        CURLUPART_ZONEID
    };
    
    const size_t num_parts = sizeof(url_parts) / sizeof(url_parts[0]);
    
    // Step 3: Set URL parts based on fuzzed data
    // Test various schemes: http, https, ftp, file
    const char* schemes[] = {"http", "https", "ftp", "file"};
    size_t scheme_idx = fdp.ConsumeIntegralInRange<size_t>(0, 3);
    
    // Set scheme
    if (fdp.remaining_bytes() > 4) {
        unsigned int flags = 0;
        if (fdp.ConsumeBool()) flags |= CURLU_URLENCODE;
        if (fdp.ConsumeBool()) flags |= CURLU_NON_SUPPORT_SCHEME;
        
        CURLUcode set_result = curl_url_set(url_handle, CURLUPART_SCHEME, 
                                           schemes[scheme_idx], flags);
        (void)set_result; // Result may be success or error
    }
    
    // Set host with various inputs
    if (fdp.remaining_bytes() > 8) {
        std::string host = fdp.ConsumeRandomLengthString(50);
        unsigned int host_flags = 0;
        if (fdp.ConsumeBool()) host_flags |= CURLU_URLENCODE;
        if (fdp.ConsumeBool()) host_flags |= CURLU_PUNYCODE;
        
        CURLUcode host_result = curl_url_set(url_handle, CURLUPART_HOST, 
                                            host.c_str(), host_flags);
        (void)host_result;
    }
    
    // Set port (valid and invalid)
    if (fdp.remaining_bytes() > 4) {
        // Generate port string, could be valid or invalid
        std::string port_str;
        if (fdp.ConsumeBool()) {
            // Valid port number
            uint16_t port = fdp.ConsumeIntegralInRange<uint16_t>(1, 65535);
            port_str = std::to_string(port);
        } else {
            // Random string that might not be a valid port
            port_str = fdp.ConsumeRandomLengthString(10);
        }
        
        unsigned int port_flags = 0;
        if (fdp.ConsumeBool()) port_flags |= CURLU_URLENCODE;
        
        CURLUcode port_result = curl_url_set(url_handle, CURLUPART_PORT, 
                                            port_str.c_str(), port_flags);
        (void)port_result;
    }
    
    // Set path with various flags
    if (fdp.remaining_bytes() > 8) {
        std::string path = fdp.ConsumeRandomLengthString(100);
        unsigned int path_flags = 0;
        if (fdp.ConsumeBool()) path_flags |= CURLU_URLENCODE;
        if (fdp.ConsumeBool()) path_flags |= CURLU_PATH_AS_IS;
        if (fdp.ConsumeBool()) path_flags |= CURLU_ALLOW_SPACE;
        
        CURLUcode path_result = curl_url_set(url_handle, CURLUPART_PATH, 
                                            path.c_str(), path_flags);
        (void)path_result;
    }
    
    // Set query with append option
    if (fdp.remaining_bytes() > 8) {
        std::string query = fdp.ConsumeRandomLengthString(100);
        unsigned int query_flags = 0;
        if (fdp.ConsumeBool()) query_flags |= CURLU_URLENCODE;
        if (fdp.ConsumeBool()) query_flags |= CURLU_APPENDQUERY;
        
        CURLUcode query_result = curl_url_set(url_handle, CURLUPART_QUERY, 
                                             query.c_str(), query_flags);
        (void)query_result;
    }
    
    // Set fragment
    if (fdp.remaining_bytes() > 8) {
        std::string fragment = fdp.ConsumeRandomLengthString(50);
        unsigned int fragment_flags = 0;
        if (fdp.ConsumeBool()) fragment_flags |= CURLU_URLENCODE;
        
        CURLUcode fragment_result = curl_url_set(url_handle, CURLUPART_FRAGMENT, 
                                                fragment.c_str(), fragment_flags);
        (void)fragment_result;
    }
    
    // Set user and password (for auth)
    if (fdp.remaining_bytes() > 8) {
        std::string user = fdp.ConsumeRandomLengthString(30);
        unsigned int user_flags = 0;
        if (fdp.ConsumeBool()) user_flags |= CURLU_URLENCODE;
        
        CURLUcode user_result = curl_url_set(url_handle, CURLUPART_USER, 
                                            user.c_str(), user_flags);
        (void)user_result;
        
        if (fdp.remaining_bytes() > 8) {
            std::string password = fdp.ConsumeRandomLengthString(30);
            CURLUcode pass_result = curl_url_set(url_handle, CURLUPART_PASSWORD, 
                                                password.c_str(), user_flags);
            (void)pass_result;
        }
    }
    
    // Step 4: Test curl_url_get() for various parts
    // Get full URL
    if (fdp.remaining_bytes() > 4) {
        char* url_str = nullptr;
        unsigned int get_flags = 0;
        if (fdp.ConsumeBool()) get_flags |= CURLU_DEFAULT_PORT;
        if (fdp.ConsumeBool()) get_flags |= CURLU_DEFAULT_SCHEME;
        if (fdp.ConsumeBool()) get_flags |= CURLU_URLDECODE;
        if (fdp.ConsumeBool()) get_flags |= CURLU_NO_DEFAULT_PORT;
        
        CURLUcode get_result = curl_url_get(url_handle, CURLUPART_URL, 
                                           &url_str, get_flags);
        if (get_result == CURLUE_OK && url_str) {
            // Got URL successfully, free it
            curl_free(url_str);
        }
    }
    
    // Get individual components
    for (size_t i = 0; i < num_parts && fdp.remaining_bytes() > 4; i++) {
        char* part_str = nullptr;
        unsigned int part_flags = 0;
        if (fdp.ConsumeBool()) part_flags |= CURLU_URLDECODE;
        if (url_parts[i] == CURLUPART_PORT && fdp.ConsumeBool()) {
            part_flags |= CURLU_DEFAULT_PORT;
        }
        
        CURLUcode part_result = curl_url_get(url_handle, url_parts[i], 
                                            &part_str, part_flags);
        if (part_result == CURLUE_OK && part_str) {
            curl_free(part_str);
        }
    }
    
    // Step 5: Test curl_url_dup() - duplicate URL handle
    CURLU* dup_url_handle = curl_url_dup(url_handle);
    if (dup_url_handle) {
        // Test getting from duplicated handle
        if (fdp.remaining_bytes() > 4) {
            char* dup_url_str = nullptr;
            CURLUcode dup_result = curl_url_get(dup_url_handle, CURLUPART_URL, 
                                               &dup_url_str, 0);
            if (dup_result == CURLUE_OK && dup_url_str) {
                curl_free(dup_url_str);
            }
        }
        
        // Modify duplicated handle
        if (fdp.remaining_bytes() > 8) {
            std::string new_path = fdp.ConsumeRandomLengthString(50);
            curl_url_set(dup_url_handle, CURLUPART_PATH, new_path.c_str(), 0);
        }
        
        // Clean up duplicated handle
        curl_url_cleanup(dup_url_handle);
    }
    
    // Step 6: Test setting full URL string with more dynamic generation
    if (fdp.remaining_bytes() > 16) {
        std::string full_url;
        
        // Choose URL type based on fuzzed data
        uint8_t url_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        // Build URL dynamically using fuzzed data
        switch (url_type) {
            case 0: // HTTP URL
                full_url = "http://";
                if (fdp.remaining_bytes() > 8) {
                    full_url += fdp.ConsumeRandomLengthString(50);
                }
                full_url += "/";
                if (fdp.remaining_bytes() > 8) {
                    full_url += fdp.ConsumeRandomLengthString(50);
                }
                break;
            case 1: // HTTPS URL - use fuzzed domain
                full_url = "https://";
                if (fdp.remaining_bytes() > 8) {
                    full_url += fdp.ConsumeRandomLengthString(50);
                }
                full_url += "/";
                if (fdp.remaining_bytes() > 8) {
                    full_url += fdp.ConsumeRandomLengthString(50);
                }
                full_url += "?";
                if (fdp.remaining_bytes() > 8) {
                    full_url += fdp.ConsumeRandomLengthString(50);
                }
                break;
            case 2: // FTP URL - use fuzzed domain
                full_url = "ftp://";
                if (fdp.remaining_bytes() > 8) {
                    full_url += fdp.ConsumeRandomLengthString(50);
                }
                full_url += "/";
                if (fdp.remaining_bytes() > 8) {
                    full_url += fdp.ConsumeRandomLengthString(50);
                }
                break;
            case 3: // File URL
                full_url = "file:///";
                if (fdp.remaining_bytes() > 8) {
                    full_url += fdp.ConsumeRandomLengthString(100);
                }
                break;
        }
        
        // Add fragment if we have enough data
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
            full_url += "#";
            full_url += fdp.ConsumeRandomLengthString(30);
        }
        
        unsigned int parse_flags = 0;
        if (fdp.ConsumeBool()) parse_flags |= CURLU_URLENCODE;
        if (fdp.ConsumeBool()) parse_flags |= CURLU_NON_SUPPORT_SCHEME;
        if (fdp.ConsumeBool()) parse_flags |= CURLU_GUESS_SCHEME;
        if (fdp.ConsumeBool()) parse_flags |= CURLU_NO_AUTHORITY;
        
        CURLUcode parse_result = curl_url_set(url_handle, CURLUPART_URL, 
                                             full_url.c_str(), parse_flags);
        (void)parse_result;
    }
    
    // Step 7: Test error cases and edge conditions
    // Test with NULL pointers
    if (fdp.ConsumeBool()) {
        // Try setting with NULL value
        curl_url_set(url_handle, CURLUPART_PATH, nullptr, 0);
    }
    
    // Test with empty strings
    if (fdp.ConsumeBool()) {
        curl_url_set(url_handle, CURLUPART_QUERY, "", 0);
    }
    
    // Step 8: Clean up
    curl_url_cleanup(url_handle);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_020

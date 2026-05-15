#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_002 {
/*
 * Fuzzing harness for libcurl's URL API
 * Targets: curl_url_init, curl_url_set, curl_url_get, curl_url_dup, curl_url_cleanup
 * Focus on URL parsing, manipulation, and validation
 */


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum input for meaningful fuzzing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize URL handle
    CURLU *urlp = curl_url();
    if (!urlp) {
        return 0;
    }
    
    // Test various URL operations based on fuzzed input
    
    // 1. Parse a complete URL from input
    std::string full_url = fdp.ConsumeRandomLengthString(512);
    if (!full_url.empty()) {
        CURLUcode uc = curl_url_set(urlp, CURLUPART_URL, full_url.c_str(), 0);
        // Don't check result - we want to test error paths too
    }
    
    // 2. Set individual URL parts based on fuzzed input
    uint8_t num_parts = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    for (uint8_t i = 0; i < num_parts && fdp.remaining_bytes() > 10; i++) {
        CURLUPart part = static_cast<CURLUPart>(fdp.ConsumeIntegralInRange<int>(0, 10));
        std::string part_value = fdp.ConsumeRandomLengthString(128);
        unsigned int flags = 0;
        
        // Randomly set some flags
        if (fdp.ConsumeBool()) flags |= CURLU_URLDECODE;
        if (fdp.ConsumeBool()) flags |= CURLU_URLENCODE;
        if (fdp.ConsumeBool()) flags |= CURLU_APPENDQUERY;
        if (fdp.ConsumeBool()) flags |= CURLU_PATH_AS_IS;
        if (fdp.ConsumeBool()) flags |= CURLU_DISALLOW_USER;
        if (fdp.ConsumeBool()) flags |= CURLU_ALLOW_SPACE;
        
        curl_url_set(urlp, part, part_value.c_str(), flags);
    }
    
    // 3. Get URL parts back
    for (uint8_t i = 0; i < 3 && fdp.remaining_bytes() > 5; i++) {
        CURLUPart part = static_cast<CURLUPart>(fdp.ConsumeIntegralInRange<int>(0, 10));
        char *value = NULL;
        unsigned int flags = 0;
        
        // Randomly set some flags for get
        if (fdp.ConsumeBool()) flags |= CURLU_DEFAULT_PORT;
        if (fdp.ConsumeBool()) flags |= CURLU_DEFAULT_SCHEME;
        if (fdp.ConsumeBool()) flags |= CURLU_PUNYCODE;
        if (fdp.ConsumeBool()) flags |= CURLU_NO_DEFAULT_PORT;
        
        CURLUcode uc = curl_url_get(urlp, part, &value, flags);
        if (uc == CURLUE_OK && value) {
            curl_free(value);
        }
    }
    
    // 4. Get the full URL back
    char *full_url_result = NULL;
    CURLUcode uc = curl_url_get(urlp, CURLUPART_URL, &full_url_result, 0);
    if (uc == CURLUE_OK && full_url_result) {
        curl_free(full_url_result);
    }
    
    // 5. Duplicate the URL handle
    CURLU *urlp2 = curl_url_dup(urlp);
    if (urlp2) {
        // Set some additional parts on the duplicate
        if (fdp.remaining_bytes() > 20) {
            std::string query = fdp.ConsumeRandomLengthString(100);
            if (!query.empty()) {
                curl_url_set(urlp2, CURLUPART_QUERY, query.c_str(), CURLU_APPENDQUERY);
            }
        }
        
        // Get scheme from duplicate
        char *scheme = NULL;
        curl_url_get(urlp2, CURLUPART_SCHEME, &scheme, 0);
        if (scheme) {
            curl_free(scheme);
        }
        
        curl_url_cleanup(urlp2);
    }
    
    // 6. Test with various schemes
    const char* schemes[] = {
        "http", "https", "ftp", "ftps", "sftp", "scp", 
        "file", "ldap", "ldaps", "imap", "imaps", "pop3", 
        "pop3s", "smtp", "smtps", "rtsp", "rtmp", "rtmps",
        "gopher", "dict", "telnet", "ssh"
    };
    
    if (fdp.remaining_bytes() > 10) {
        uint8_t scheme_idx = fdp.ConsumeIntegral<uint8_t>() % (sizeof(schemes)/sizeof(schemes[0]));
        std::string host = fdp.ConsumeRandomLengthString(100);
        
        // Create a new URL for scheme testing
        CURLU *test_url = curl_url();
        if (test_url) {
            // Set scheme
            curl_url_set(test_url, CURLUPART_SCHEME, schemes[scheme_idx], 0);
            
            // Set host if available
            if (!host.empty()) {
                curl_url_set(test_url, CURLUPART_HOST, host.c_str(), 0);
            }
            
            // Set path if available
            if (fdp.remaining_bytes() > 10) {
                std::string path = fdp.ConsumeRandomLengthString(100);
                if (!path.empty()) {
                    curl_url_set(test_url, CURLUPART_PATH, path.c_str(), 0);
                }
            }
            
            // Try to get the full URL
            char *test_full = NULL;
            curl_url_get(test_url, CURLUPART_URL, &test_full, 0);
            if (test_full) {
                curl_free(test_full);
            }
            
            curl_url_cleanup(test_url);
        }
    }
    
    // 7. Test URL with query parameters
    if (fdp.remaining_bytes() > 30) {
        std::string query1 = fdp.ConsumeRandomLengthString(50);
        std::string query2 = fdp.ConsumeRandomLengthString(50);
        
        if (!query1.empty()) {
            curl_url_set(urlp, CURLUPART_QUERY, query1.c_str(), CURLU_APPENDQUERY);
        }
        
        if (!query2.empty()) {
            curl_url_set(urlp, CURLUPART_QUERY, query2.c_str(), CURLU_APPENDQUERY);
        }
        
        // Get query back
        char *final_query = NULL;
        curl_url_get(urlp, CURLUPART_QUERY, &final_query, 0);
        if (final_query) {
            curl_free(final_query);
        }
    }
    
    // 8. Test with port numbers
    if (fdp.remaining_bytes() > 5) {
        std::string port = fdp.ConsumeRandomLengthString(10);
        if (!port.empty()) {
            curl_url_set(urlp, CURLUPART_PORT, port.c_str(), 0);
        }
    }
    
    // 9. Test with userinfo (user:password)
    if (fdp.remaining_bytes() > 20) {
        std::string user = fdp.ConsumeRandomLengthString(50);
        std::string password = fdp.ConsumeRandomLengthString(50);
        
        if (!user.empty()) {
            curl_url_set(urlp, CURLUPART_USER, user.c_str(), 0);
        }
        
        if (!password.empty()) {
            curl_url_set(urlp, CURLUPART_PASSWORD, password.c_str(), 0);
        }
    }
    
    // 10. Test with fragment
    if (fdp.remaining_bytes() > 10) {
        std::string fragment = fdp.ConsumeRandomLengthString(50);
        if (!fragment.empty()) {
            curl_url_set(urlp, CURLUPART_FRAGMENT, fragment.c_str(), 0);
        }
    }
    
    // 11. Get all parts one more time to test state
    CURLUPart parts_to_get[] = {
        CURLUPART_SCHEME, CURLUPART_USER, CURLUPART_PASSWORD,
        CURLUPART_HOST, CURLUPART_PORT, CURLUPART_PATH,
        CURLUPART_QUERY, CURLUPART_FRAGMENT
    };
    
    for (CURLUPart part : parts_to_get) {
        if (fdp.remaining_bytes() > 2) {
            char *value = NULL;
            unsigned int flags = 0;
            if (fdp.ConsumeBool()) flags |= CURLU_DEFAULT_PORT;
            if (fdp.ConsumeBool()) flags |= CURLU_DEFAULT_SCHEME;
            
            curl_url_get(urlp, part, &value, flags);
            if (value) {
                curl_free(value);
            }
        }
    }
    
    // Clean up
    curl_url_cleanup(urlp);
    
    return 0;
}

} // namespace harness_002

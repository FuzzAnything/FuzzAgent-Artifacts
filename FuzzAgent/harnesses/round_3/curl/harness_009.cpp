#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/urlapi.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_009 {
/*
 * Fuzzing harness for libcurl's URL parsing and manipulation API
 * Targets: curl_url_set (126 blocked branches, top blocker), curl_url_get, 
 *          curl_url_dup, curl_url_strerror, curl_url, curl_url_cleanup
 * Focus on URL API functions which have 300+ accumulated branches with 200+ undiscovered branches
 * Test scenarios: complete URL parsing, individual component setting/getting,
 *                error conditions, flags testing, memory lifecycle
 */


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum input for meaningful fuzzing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl (required for URL API)
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }
    
    // Create URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Consume fixed-size data first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint32_t flags = fdp.ConsumeIntegral<uint32_t>();
    
    // Consume various URL components from fuzzed input
    std::string full_url = fdp.ConsumeRandomLengthString(512);
    std::string scheme = fdp.ConsumeRandomLengthString(32);
    std::string user = fdp.ConsumeRandomLengthString(64);
    std::string password = fdp.ConsumeRandomLengthString(64);
    std::string host = fdp.ConsumeRandomLengthString(256);
    std::string port_str = fdp.ConsumeRandomLengthString(8);
    std::string path = fdp.ConsumeRandomLengthString(256);
    std::string query = fdp.ConsumeRandomLengthString(256);
    std::string fragment = fdp.ConsumeRandomLengthString(256);
    std::string zoneid = fdp.ConsumeRandomLengthString(64);
    
    // Test different scenarios based on fuzzed input
    switch (test_scenario) {
        case 0: {
            // Scenario 1: Complete URL parsing and manipulation
            
            // Set full URL first
            CURLUcode uc = curl_url_set(url_handle, CURLUPART_URL, 
                                        full_url.empty() ? NULL : full_url.c_str(), 
                                        flags & 0xFFFF);
            
            // If successful, try to get individual components
            if (uc == CURLUE_OK) {
                char *retrieved_part = NULL;
                
                // Try to get various parts with different flags
                for (int part = CURLUPART_SCHEME; part <= CURLUPART_ZONEID; part++) {
                    CURLUcode get_uc = curl_url_get(url_handle, 
                                                   static_cast<CURLUPart>(part), 
                                                   &retrieved_part, 
                                                   fdp.ConsumeIntegral<uint8_t>() & 0x7F);
                    if (get_uc == CURLUE_OK && retrieved_part) {
                        curl_free(retrieved_part);
                        retrieved_part = NULL;
                    }
                }
            }
            
            // Exercise error paths - try invalid operations
            if (fdp.ConsumeBool()) {
                // Try setting invalid part
                CURLUcode err_uc = curl_url_set(url_handle, 
                                               static_cast<CURLUPart>(999), 
                                               "invalid", 
                                               0);
                if (err_uc != CURLUE_OK) {
                    const char *err_str = curl_url_strerror(err_uc);
                    (void)err_str; // Use error string
                }
            }
            break;
        }
        
        case 1: {
            // Scenario 2: Individual component setting and getting
            
            // Set individual components in sequence
            CURLUPart parts[] = {
                CURLUPART_SCHEME,
                CURLUPART_USER,
                CURLUPART_PASSWORD,
                CURLUPART_HOST,
                CURLUPART_PORT,
                CURLUPART_PATH,
                CURLUPART_QUERY,
                CURLUPART_FRAGMENT,
                CURLUPART_ZONEID
            };
            
            const char *part_values[] = {
                scheme.empty() ? NULL : scheme.c_str(),
                user.empty() ? NULL : user.c_str(),
                password.empty() ? NULL : password.c_str(),
                host.empty() ? NULL : host.c_str(),
                port_str.empty() ? NULL : port_str.c_str(),
                path.empty() ? NULL : path.c_str(),
                query.empty() ? NULL : query.c_str(),
                fragment.empty() ? NULL : fragment.c_str(),
                zoneid.empty() ? NULL : zoneid.c_str()
            };
            
            for (size_t i = 0; i < sizeof(parts)/sizeof(parts[0]); i++) {
                if (part_values[i]) {
                    unsigned int part_flags = fdp.ConsumeIntegral<uint8_t>() & 0x7F;
                    CURLUcode set_uc = curl_url_set(url_handle, parts[i], 
                                                   part_values[i], part_flags);
                    
                    // Try to get the same part
                    if (set_uc == CURLUE_OK) {
                        char *retrieved = NULL;
                        CURLUcode get_uc = curl_url_get(url_handle, parts[i], 
                                                       &retrieved, part_flags);
                        if (get_uc == CURLUE_OK && retrieved) {
                            curl_free(retrieved);
                        }
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Scenario 3: Duplication and memory lifecycle
            
            // Set some initial values
            if (!scheme.empty()) {
                curl_url_set(url_handle, CURLUPART_SCHEME, scheme.c_str(), 0);
            }
            if (!host.empty()) {
                curl_url_set(url_handle, CURLUPART_HOST, host.c_str(), 0);
            }
            if (!path.empty()) {
                curl_url_set(url_handle, CURLUPART_PATH, path.c_str(), 0);
            }
            
            // Duplicate the URL handle
            CURLU *dup_handle = curl_url_dup(url_handle);
            if (dup_handle) {
                // Modify the duplicated handle
                if (!query.empty()) {
                    curl_url_set(dup_handle, CURLUPART_QUERY, query.c_str(), 
                                CURLU_APPENDQUERY);
                }
                
                // Get values from both handles
                char *orig_path = NULL;
                char *dup_path = NULL;
                
                curl_url_get(url_handle, CURLUPART_PATH, &orig_path, 0);
                curl_url_get(dup_handle, CURLUPART_PATH, &dup_path, 0);
                
                if (orig_path) curl_free(orig_path);
                if (dup_path) curl_free(dup_path);
                
                // Clean up duplicated handle
                curl_url_cleanup(dup_handle);
            }
            break;
        }
        
        case 3: {
            // Scenario 4: Error conditions and flags testing
            
            // Test various flag combinations
            unsigned int test_flags[] = {
                0,
                CURLU_URLENCODE,
                CURLU_URLDECODE,
                CURLU_APPENDQUERY,
                CURLU_PATH_AS_IS,
                CURLU_DISALLOW_USER,
                CURLU_DEFAULT_PORT,
                CURLU_NO_DEFAULT_PORT,
                CURLU_DEFAULT_SCHEME,
                CURLU_NON_SUPPORT_SCHEME,
                CURLU_GUESS_SCHEME,
                CURLU_NO_AUTHORITY,
                CURLU_ALLOW_SPACE,
                CURLU_PUNYCODE,
                CURLU_PUNY2IDN,
                CURLU_GET_EMPTY,
                CURLU_NO_GUESS_SCHEME
            };
            
            // Test setting with different flags
            for (unsigned int test_flag : test_flags) {
                if (fdp.ConsumeBool() && !host.empty()) {
                    CURLUcode flag_uc = curl_url_set(url_handle, CURLUPART_HOST, 
                                                    host.c_str(), test_flag);
                    
                    // If error, get error string
                    if (flag_uc != CURLUE_OK) {
                        const char *err_msg = curl_url_strerror(flag_uc);
                        (void)err_msg;
                    }
                }
            }
            
            // Test NULL and empty string cases
            if (fdp.ConsumeBool()) {
                curl_url_set(url_handle, CURLUPART_QUERY, NULL, 0);
            }
            if (fdp.ConsumeBool()) {
                curl_url_set(url_handle, CURLUPART_FRAGMENT, "", 0);
            }
            
            // Test invalid port numbers
            if (fdp.ConsumeBool()) {
                curl_url_set(url_handle, CURLUPART_PORT, "99999", 0);
            }
            break;
        }
    }
    
    // Final cleanup
    curl_url_cleanup(url_handle);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_009

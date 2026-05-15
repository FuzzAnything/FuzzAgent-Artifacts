#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/urlapi.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_016 {
/*
 * Fuzzing harness for libcurl's URL parsing functions integrated with HTTP protocol operations
 * Targets: curl_url_set (145 blocked branches - single most critical coverage gap)
 *          HTTP protocol implementation (0.33% coverage)
 * Focus on comprehensive URL parsing and HTTP protocol operations integration
 * Test scenarios: URL parsing with various components, using parsed URLs in HTTP operations,
 *                 HTTP-specific options, semantic diversity from existing harnesses
 */


// Write callback for handling HTTP response data
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    // Just discard the data for fuzzing purposes
    (void)contents;
    (void)userp;
    return realsize;
}

// Header callback for HTTP headers
static size_t header_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    // Just discard headers for fuzzing
    (void)contents;
    (void)userp;
    return realsize;
}

// Progress callback
static int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp;
    (void)dltotal;
    (void)dlnow;
    (void)ultotal;
    (void)ulnow;
    return 0; // Continue
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum input for meaningful fuzzing - enough for URL parsing and HTTP operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }
    
    // Create URL handle for URL parsing operations
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create easy handle for HTTP operations
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Set basic callbacks for HTTP operations
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 0L); // Enable progress callback
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, progress_callback);
    
    // Consume fixed-size data first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume URL components from fuzzed input
    std::string scheme = fdp.ConsumeRandomLengthString(32);
    std::string user = fdp.ConsumeRandomLengthString(64);
    std::string password = fdp.ConsumeRandomLengthString(64);
    std::string host = fdp.ConsumeRandomLengthString(256);
    std::string port_str = fdp.ConsumeRandomLengthString(8);
    std::string path = fdp.ConsumeRandomLengthString(256);
    std::string query = fdp.ConsumeRandomLengthString(256);
    std::string fragment = fdp.ConsumeRandomLengthString(256);
    
    // Consume HTTP-specific parameters
    std::string custom_header = fdp.ConsumeRandomLengthString(128);
    std::string post_data = fdp.ConsumeRandomLengthString(1024);
    
    // Variables for retrieved URL parts
    char *retrieved_url = NULL;
    char *retrieved_scheme = NULL;
    char *retrieved_host = NULL;
    char *retrieved_path = NULL;
    
    switch (test_scenario) {
        case 0: {
            // Scenario 1: Comprehensive URL parsing with all components and HTTP integration
            
            // Set individual URL components with various flags
            CURLUPart parts[] = {
                CURLUPART_SCHEME,
                CURLUPART_USER,
                CURLUPART_PASSWORD,
                CURLUPART_HOST,
                CURLUPART_PORT,
                CURLUPART_PATH,
                CURLUPART_QUERY,
                CURLUPART_FRAGMENT
            };
            
            const char *part_values[] = {
                scheme.empty() ? NULL : scheme.c_str(),
                user.empty() ? NULL : user.c_str(),
                password.empty() ? NULL : password.c_str(),
                host.empty() ? NULL : host.c_str(),
                port_str.empty() ? NULL : port_str.c_str(),
                path.empty() ? NULL : path.c_str(),
                query.empty() ? NULL : query.c_str(),
                fragment.empty() ? NULL : fragment.c_str()
            };
            
            unsigned int flag_options[] = {
                CURLU_URLENCODE,
                CURLU_URLDECODE,
                CURLU_APPENDQUERY,
                CURLU_PATH_AS_IS,
                CURLU_DISALLOW_USER,
                CURLU_ALLOW_SPACE,
                CURLU_GUESS_SCHEME,
                CURLU_NO_AUTHORITY
            };
            
            // Set URL parts with various flag combinations
            for (size_t i = 0; i < sizeof(parts)/sizeof(parts[0]); i++) {
                if (part_values[i] && fdp.remaining_bytes() > 0) {
                    unsigned int flags = 0;
                    uint8_t flag_mask = fdp.ConsumeIntegral<uint8_t>();
                    
                    // Apply random flags
                    for (size_t j = 0; j < sizeof(flag_options)/sizeof(flag_options[0]); j++) {
                        if (flag_mask & (1 << (j % 8))) {
                            flags |= flag_options[j];
                        }
                    }
                    
                    // Primary target: curl_url_set with 145 blocked branches
                    CURLUcode set_result = curl_url_set(url_handle, parts[i], part_values[i], flags);
                    
                    // Test error handling
                    if (set_result != CURLUE_OK) {
                        const char *err_str = curl_url_strerror(set_result);
                        (void)err_str; // Use error string
                    }
                }
            }
            
            // Get complete URL and use it for HTTP operations
            CURLUcode get_result = curl_url_get(url_handle, CURLUPART_URL, &retrieved_url, 0);
            if (get_result == CURLUE_OK && retrieved_url) {
                // Use parsed URL for HTTP operation
                curl_easy_setopt(easy_handle, CURLOPT_URL, retrieved_url);
                
                // Set HTTP method based on fuzzed input
                uint8_t http_method = fdp.ConsumeIntegral<uint8_t>() % 5;
                switch (http_method) {
                    case 0:
                        curl_easy_setopt(easy_handle, CURLOPT_HTTPGET, 1L);
                        break;
                    case 1:
                        curl_easy_setopt(easy_handle, CURLOPT_POST, 1L);
                        if (!post_data.empty()) {
                            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, post_data.c_str());
                            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, post_data.length());
                        }
                        break;
                    case 2:
                        curl_easy_setopt(easy_handle, CURLOPT_PUT, 1L);
                        break;
                    case 3:
                        curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, "DELETE");
                        break;
                    case 4:
                        curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, "HEAD");
                        break;
                }
                
                // Set HTTP version
                uint8_t http_version = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (http_version) {
                    case 0:
                        curl_easy_setopt(easy_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
                        break;
                    case 1:
                        curl_easy_setopt(easy_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
                        break;
                    case 2:
                        curl_easy_setopt(easy_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
                        break;
                    case 3:
                        curl_easy_setopt(easy_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);
                        break;
                }
                
                // Set HTTP headers if available
                if (!custom_header.empty()) {
                    struct curl_slist *headers = NULL;
                    headers = curl_slist_append(headers, custom_header.c_str());
                    if (headers) {
                        curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
                        curl_slist_free_all(headers);
                    }
                }
                
                // Set HTTP-specific options
                uint8_t http_options = fdp.ConsumeIntegral<uint8_t>();
                if (http_options & 0x01) {
                    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, fdp.ConsumeBool() ? 1L : 0L);
                }
                if (http_options & 0x02) {
                    curl_easy_setopt(easy_handle, CURLOPT_MAXREDIRS, fdp.ConsumeIntegralInRange<long>(0, 10));
                }
                if (http_options & 0x04) {
                    curl_easy_setopt(easy_handle, CURLOPT_HTTPAUTH, 
                                    fdp.ConsumeBool() ? CURLAUTH_BASIC : CURLAUTH_ANY);
                }
                if (http_options & 0x08) {
                    curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, "fuzzer/1.0");
                }
                
                // Try to perform HTTP operation (may fail without network, but that's OK)
                CURLcode perform_result = curl_easy_perform(easy_handle);
                (void)perform_result; // Result may be error, but we continue testing
                
                curl_free(retrieved_url);
            }
            break;
        }
        
        case 1: {
            // Scenario 2: Complex URL parsing with query manipulation and HTTP POST
            
            // Build a complex URL with query parameters
            if (!scheme.empty()) {
                curl_url_set(url_handle, CURLUPART_SCHEME, scheme.c_str(), CURLU_URLENCODE);
            }
            if (!host.empty()) {
                curl_url_set(url_handle, CURLUPART_HOST, host.c_str(), CURLU_URLENCODE);
            }
            if (!path.empty()) {
                curl_url_set(url_handle, CURLUPART_PATH, path.c_str(), CURLU_PATH_AS_IS);
            }
            
            // Append multiple query parameters
            if (!query.empty()) {
                // First query parameter
                curl_url_set(url_handle, CURLUPART_QUERY, query.c_str(), CURLU_APPENDQUERY);
                
                // Second query parameter (if enough input)
                if (fdp.remaining_bytes() > 10) {
                    std::string query2 = fdp.ConsumeRandomLengthString(100);
                    if (!query2.empty()) {
                        curl_url_set(url_handle, CURLUPART_QUERY, query2.c_str(), CURLU_APPENDQUERY);
                    }
                }
            }
            
            // Get individual components
            curl_url_get(url_handle, CURLUPART_SCHEME, &retrieved_scheme, 0);
            curl_url_get(url_handle, CURLUPART_HOST, &retrieved_host, CURLU_PUNYCODE);
            curl_url_get(url_handle, CURLUPART_PATH, &retrieved_path, 0);
            
            // Use URL for HTTP POST operation
            curl_url_get(url_handle, CURLUPART_URL, &retrieved_url, 0);
            if (retrieved_url) {
                curl_easy_setopt(easy_handle, CURLOPT_URL, retrieved_url);
                curl_easy_setopt(easy_handle, CURLOPT_POST, 1L);
                
                if (!post_data.empty()) {
                    curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, post_data.c_str());
                    curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, post_data.length());
                }
                
                // Set content type for POST
                if (fdp.ConsumeBool()) {
                    struct curl_slist *headers = NULL;
                    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
                    if (headers) {
                        curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
                        curl_slist_free_all(headers);
                    }
                }
                
                // Perform HTTP POST
                CURLcode perform_result = curl_easy_perform(easy_handle);
                (void)perform_result;
                
                curl_free(retrieved_url);
            }
            
            if (retrieved_scheme) curl_free(retrieved_scheme);
            if (retrieved_host) curl_free(retrieved_host);
            if (retrieved_path) curl_free(retrieved_path);
            break;
        }
        
        case 2: {
            // Scenario 3: URL with authentication and HTTP authentication testing
            
            // Build URL with userinfo
            if (!scheme.empty()) {
                curl_url_set(url_handle, CURLUPART_SCHEME, scheme.c_str(), 0);
            }
            if (!user.empty()) {
                curl_url_set(url_handle, CURLUPART_USER, user.c_str(), CURLU_URLENCODE);
            }
            if (!password.empty()) {
                curl_url_set(url_handle, CURLUPART_PASSWORD, password.c_str(), CURLU_URLENCODE);
            }
            if (!host.empty()) {
                curl_url_set(url_handle, CURLUPART_HOST, host.c_str(), 0);
            }
            if (!port_str.empty()) {
                curl_url_set(url_handle, CURLUPART_PORT, port_str.c_str(), 0);
            }
            if (!path.empty()) {
                curl_url_set(url_handle, CURLUPART_PATH, path.c_str(), CURLU_PATH_AS_IS);
            }
            
            // Get full URL with authentication
            curl_url_get(url_handle, CURLUPART_URL, &retrieved_url, 0);
            if (retrieved_url) {
                curl_easy_setopt(easy_handle, CURLOPT_URL, retrieved_url);
                
                // Set HTTP authentication methods
                uint8_t auth_methods = fdp.ConsumeIntegral<uint8_t>() % 8;
                long auth_mask = 0;
                
                if (auth_methods & 0x01) auth_mask |= CURLAUTH_BASIC;
                if (auth_methods & 0x02) auth_mask |= CURLAUTH_DIGEST;
                if (auth_methods & 0x04) auth_mask |= CURLAUTH_NEGOTIATE;
                if (auth_methods & 0x08) auth_mask |= CURLAUTH_NTLM;
                
                curl_easy_setopt(easy_handle, CURLOPT_HTTPAUTH, auth_mask);
                
                // Set username and password if available
                if (!user.empty()) {
                    curl_easy_setopt(easy_handle, CURLOPT_USERNAME, user.c_str());
                }
                if (!password.empty()) {
                    curl_easy_setopt(easy_handle, CURLOPT_PASSWORD, password.c_str());
                }
                
                // Set HTTP method to GET
                curl_easy_setopt(easy_handle, CURLOPT_HTTPGET, 1L);
                
                // Set various HTTP options
                curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, fdp.ConsumeBool() ? 1L : 0L);
                curl_easy_setopt(easy_handle, CURLOPT_MAXREDIRS, fdp.ConsumeIntegralInRange<long>(0, 5));
                curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, fdp.ConsumeIntegralInRange<long>(1, 10));
                
                // Perform HTTP request with authentication
                CURLcode perform_result = curl_easy_perform(easy_handle);
                (void)perform_result;
                
                curl_free(retrieved_url);
            }
            break;
        }
    }
    
    // Clean up
    curl_easy_cleanup(easy_handle);
    curl_url_cleanup(url_handle);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_016

#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/urlapi.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_012 {

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size for meaningful URL testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Create initial URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        return 0;
    }
    
    // Consume operation type to determine URL scheme and structure
    uint8_t url_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume various URL components from fuzzer input
    std::string scheme;
    std::string host;
    std::string port_str;
    std::string user;
    std::string password;
    std::string path;
    std::string query;
    std::string fragment;
    
    // Determine URL scheme based on url_type
    switch (url_type) {
        case 0: scheme = "http://"; break;
        case 1: scheme = "https://"; break;
        case 2: scheme = "ftp://"; break;
        case 3: scheme = "file://"; break;
        case 4: scheme = "sftp://"; break;
        case 5: scheme = "ws://"; break;
        case 6: scheme = "wss://"; break;
        case 7: scheme = ""; break; // No scheme - use GUESS_SCHEME
    }
    
    // Consume hostname with various patterns
    uint8_t host_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    switch (host_type) {
        case 0: // Simple domain
            host = fdp.ConsumeRandomLengthString(64);
            break;
        case 1: // Domain with subdomain
            host = fdp.ConsumeRandomLengthString(32) + "." + 
                   fdp.ConsumeRandomLengthString(32);
            break;
        case 2: // IP address
            host = std::to_string(fdp.ConsumeIntegralInRange<int>(0, 255)) + "." +
                   std::to_string(fdp.ConsumeIntegralInRange<int>(0, 255)) + "." +
                   std::to_string(fdp.ConsumeIntegralInRange<int>(0, 255)) + "." +
                   std::to_string(fdp.ConsumeIntegralInRange<int>(0, 255));
            break;
        case 3: // IPv6 address (simplified)
            host = "[" + fdp.ConsumeRandomLengthString(32) + "]";
            break;
        case 4: // Localhost
            host = "localhost";
            break;
        case 5: // Empty host
            host = "";
            break;
    }
    
    // Consume port if needed
    bool include_port = fdp.ConsumeBool();
    if (include_port) {
        int port = fdp.ConsumeIntegralInRange<int>(1, 65535);
        port_str = ":" + std::to_string(port);
    }
    
    // Consume user info
    bool include_userinfo = fdp.ConsumeBool();
    if (include_userinfo) {
        user = fdp.ConsumeRandomLengthString(32);
        bool include_password = fdp.ConsumeBool();
        if (include_password) {
            password = fdp.ConsumeRandomLengthString(32);
        }
    }
    
    // Consume path with various patterns
    uint8_t path_type = fdp.ConsumeIntegral<uint8_t>() % 5;
    switch (path_type) {
        case 0: // Simple path
            path = "/" + fdp.ConsumeRandomLengthString(64);
            break;
        case 1: // Deep path
            path = "/" + fdp.ConsumeRandomLengthString(16) + "/" +
                   fdp.ConsumeRandomLengthString(16) + "/" +
                   fdp.ConsumeRandomLengthString(16);
            break;
        case 2: // Path with file extension
            path = "/" + fdp.ConsumeRandomLengthString(32) + "." +
                   fdp.ConsumeRandomLengthString(8);
            break;
        case 3: // Path with special characters
            path = "/" + fdp.ConsumeRandomLengthString(48) + 
                   "?" + fdp.ConsumeRandomLengthString(16) +
                   "#" + fdp.ConsumeRandomLengthString(16);
            break;
        case 4: // Empty path
            path = "";
            break;
    }
    
    // Consume query string
    bool include_query = fdp.ConsumeBool();
    if (include_query) {
        uint8_t query_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (query_type) {
            case 0: // Simple query
                query = "?" + fdp.ConsumeRandomLengthString(32);
                break;
            case 1: // Multiple parameters
                query = "?" + fdp.ConsumeRandomLengthString(16) + "=" +
                        fdp.ConsumeRandomLengthString(16) + "&" +
                        fdp.ConsumeRandomLengthString(16) + "=" +
                        fdp.ConsumeRandomLengthString(16);
                break;
            case 2: // URL encoded query
                query = "?" + fdp.ConsumeRandomLengthString(48);
                break;
        }
    }
    
    // Consume fragment
    bool include_fragment = fdp.ConsumeBool();
    if (include_fragment) {
        fragment = "#" + fdp.ConsumeRandomLengthString(32);
    }
    
    // Build full URL string for initial set
    std::string full_url;
    if (!scheme.empty() && scheme.back() == ':') {
        full_url = scheme + "//";
    } else if (!scheme.empty()) {
        full_url = scheme;
    }
    
    if (!user.empty()) {
        full_url += user;
        if (!password.empty()) {
            full_url += ":" + password;
        }
        full_url += "@";
    }
    
    if (!host.empty()) {
        full_url += host;
    }
    
    if (!port_str.empty()) {
        full_url += port_str;
    }
    
    if (!path.empty()) {
        // Ensure path starts with / if not empty and doesn't already
        if (path[0] != '/' && !path.empty()) {
            full_url += "/" + path;
        } else {
            full_url += path;
        }
    }
    
    if (!query.empty()) {
        full_url += query;
    }
    
    if (!fragment.empty()) {
        full_url += fragment;
    }
    
    // 2. Set URL using curl_url_set() with various flags
    unsigned int flags = 0;
    uint8_t flags_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (flags_type) {
        case 0: flags = CURLU_GUESS_SCHEME; break;
        case 1: flags = CURLU_DEFAULT_SCHEME; break;
        case 2: flags = CURLU_URLENCODE; break;
        case 3: flags = CURLU_URLDECODE; break;
        case 4: flags = CURLU_PATH_AS_IS; break;
        case 5: flags = CURLU_NON_SUPPORT_SCHEME; break;
        case 6: flags = CURLU_ALLOW_SPACE; break;
        case 7: flags = CURLU_APPENDQUERY; break;
    }
    
    // Set full URL first
    CURLUcode result = curl_url_set(url_handle, CURLUPART_URL, full_url.c_str(), flags);
    
    // Store error for potential strerror use
    CURLUcode last_error = result;
    
    // 3. Set individual URL parts using curl_url_set()
    if (result == CURLUE_OK || fdp.ConsumeBool()) {
        // Try setting individual parts regardless of full URL success
        if (!scheme.empty() && scheme.find("://") != std::string::npos) {
            std::string clean_scheme = scheme.substr(0, scheme.find("://"));
            curl_url_set(url_handle, CURLUPART_SCHEME, clean_scheme.c_str(), 0);
        }
        
        if (!host.empty()) {
            curl_url_set(url_handle, CURLUPART_HOST, host.c_str(), 0);
        }
        
        if (!path.empty()) {
            curl_url_set(url_handle, CURLUPART_PATH, path.c_str(), CURLU_PATH_AS_IS);
        }
        
        if (!query.empty() && query[0] == '?') {
            std::string clean_query = query.substr(1);
            curl_url_set(url_handle, CURLUPART_QUERY, clean_query.c_str(), CURLU_APPENDQUERY);
        }
        
        if (!fragment.empty() && fragment[0] == '#') {
            std::string clean_fragment = fragment.substr(1);
            curl_url_set(url_handle, CURLUPART_FRAGMENT, clean_fragment.c_str(), 0);
        }
        
        if (include_port && !port_str.empty()) {
            std::string clean_port = port_str.substr(1);
            curl_url_set(url_handle, CURLUPART_PORT, clean_port.c_str(), 0);
        }
        
        if (!user.empty()) {
            curl_url_set(url_handle, CURLUPART_USER, user.c_str(), 0);
        }
        
        if (!password.empty()) {
            curl_url_set(url_handle, CURLUPART_PASSWORD, password.c_str(), 0);
        }
    }
    
    // 4. Get URL parts using curl_url_get() to validate
    char *retrieved_scheme = NULL;
    char *retrieved_host = NULL;
    char *retrieved_path = NULL;
    char *retrieved_query = NULL;
    char *retrieved_fragment = NULL;
    char *retrieved_port = NULL;
    char *retrieved_user = NULL;
    char *retrieved_password = NULL;
    char *retrieved_url = NULL;
    
    // Try to get various parts with different flags
    unsigned int get_flags = 0;
    if (fdp.ConsumeBool()) {
        get_flags |= CURLU_DEFAULT_PORT;
    }
    if (fdp.ConsumeBool()) {
        get_flags |= CURLU_URLDECODE;
    }
    if (fdp.ConsumeBool()) {
        get_flags |= CURLU_PUNYCODE;
    }
    
    curl_url_get(url_handle, CURLUPART_URL, &retrieved_url, get_flags);
    curl_url_get(url_handle, CURLUPART_SCHEME, &retrieved_scheme, 0);
    curl_url_get(url_handle, CURLUPART_HOST, &retrieved_host, 0);
    curl_url_get(url_handle, CURLUPART_PATH, &retrieved_path, 0);
    curl_url_get(url_handle, CURLUPART_QUERY, &retrieved_query, 0);
    curl_url_get(url_handle, CURLUPART_FRAGMENT, &retrieved_fragment, 0);
    
    // Also try getting user info if it was set
    if (!user.empty() || !password.empty()) {
        curl_url_get(url_handle, CURLUPART_USER, &retrieved_user, 0);
        curl_url_get(url_handle, CURLUPART_PASSWORD, &retrieved_password, 0);
    }
    
    // Try getting port
    curl_url_get(url_handle, CURLUPART_PORT, &retrieved_port, CURLU_DEFAULT_PORT);
    
    // 5. Duplicate the URL handle using curl_url_dup()
    CURLU *dup_handle = curl_url_dup(url_handle);
    if (dup_handle) {
        // 6. Get URL parts from duplicated handle
        char *dup_scheme = NULL;
        char *dup_host = NULL;
        char *dup_path = NULL;
        
        curl_url_get(dup_handle, CURLUPART_SCHEME, &dup_scheme, 0);
        curl_url_get(dup_handle, CURLUPART_HOST, &dup_host, 0);
        curl_url_get(dup_handle, CURLUPART_PATH, &dup_path, 0);
        
        // Test setting a new value on the duplicate
        if (fdp.ConsumeBool()) {
            std::string new_path = "/" + fdp.ConsumeRandomLengthString(32);
            curl_url_set(dup_handle, CURLUPART_PATH, new_path.c_str(), CURLU_PATH_AS_IS);
        }
        
        // Test appending to query on duplicate
        if (fdp.ConsumeBool()) {
            std::string append_query = fdp.ConsumeRandomLengthString(16) + "=" +
                                      fdp.ConsumeRandomLengthString(16);
            curl_url_set(dup_handle, CURLUPART_QUERY, append_query.c_str(), CURLU_APPENDQUERY);
        }
        
        // Free duplicate retrieved values
        if (dup_scheme) curl_free(dup_scheme);
        if (dup_host) curl_free(dup_host);
        if (dup_path) curl_free(dup_path);
        
        // 7. Clean up duplicate handle
        curl_url_cleanup(dup_handle);
    }
    
    // 7. Use curl_url_strerror() for error translation
    if (last_error != CURLUE_OK) {
        const char *error_str = curl_url_strerror(last_error);
        // error_str can be used for logging or validation
        (void)error_str; // Prevent unused variable warning
    }
    
    // Also test strerror on random error codes
    if (fdp.ConsumeBool()) {
        CURLUcode test_error = static_cast<CURLUcode>(fdp.ConsumeIntegralInRange<int>(0, CURLUE_LAST - 1));
        const char *test_error_str = curl_url_strerror(test_error);
        (void)test_error_str;
    }
    
    // Free all allocated strings from curl_url_get
    if (retrieved_url) curl_free(retrieved_url);
    if (retrieved_scheme) curl_free(retrieved_scheme);
    if (retrieved_host) curl_free(retrieved_host);
    if (retrieved_path) curl_free(retrieved_path);
    if (retrieved_query) curl_free(retrieved_query);
    if (retrieved_fragment) curl_free(retrieved_fragment);
    if (retrieved_port) curl_free(retrieved_port);
    if (retrieved_user) curl_free(retrieved_user);
    if (retrieved_password) curl_free(retrieved_password);
    
    // 6. Clean up original handle
    curl_url_cleanup(url_handle);
    
    return 0;
}

} // namespace harness_012

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_017 {
/*
 * Comprehensive fuzzing harness for libcurl's HTTP protocol operations
 * Coverage analysis identified HTTP protocol as the single most critical coverage gap (0.48% branch coverage in http.c)
 * 
 * Primary targets:
 *   - HTTP/1.1 protocol implementation
 *   - HTTP/2 protocol features  
 *   - HTTP proxy operations (HTTP, HTTPS, SOCKS proxies)
 *   - Various HTTP methods (GET, POST, PUT, DELETE, HEAD, OPTIONS, PATCH)
 *   - HTTP header manipulation and authentication
 *   - Response handling and status code processing
 *   - Connection reuse and keep-alive
 *   - Chunked transfer encoding
 *   - HTTP version negotiation
 * 
 * Semantic differentiation from existing harnesses:
 *   - harness_000: Basic HTTP operations with minimal configuration
 *   - harness_001: HTTP header manipulation APIs
 *   - harness_015: WebSocket protocol operations  
 *   - harness_016: WebSocket operations (recompiled)
 *   - harness_017: Comprehensive HTTP protocol testing focusing on HTTP/1.1, HTTP/2, and proxy operations
 * 
 * Key coverage objectives for http.c (0.48% branch coverage):
 *   - HTTP protocol state machine transitions
 *   - Request/response parsing and validation
 *   - Proxy connection establishment and tunneling
 *   - HTTP/2 frame processing and stream management
 *   - Authentication mechanisms (Basic, Digest, NTLM, Negotiate)
 *   - Chunked encoding parsing
 *   - Keep-alive connection management
 *   - HTTP method-specific processing
 * 
 * Invocation sequence:
 *   1. curl_global_init(CURL_GLOBAL_ALL)
 *   2. curl_easy_init()
 *   3. curl_easy_setopt with comprehensive HTTP/proxy configuration
 *   4. curl_easy_perform() for request execution
 *   5. Response processing and error handling
 *   6. curl_easy_cleanup() and curl_global_cleanup()
 */


// Simple write callback that discards data
static size_t discard_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;  // Always report success
}

// Header callback that discards headers
static size_t discard_header_cb(char *buffer, size_t size, size_t nitems, void *userdata) {
    (void)buffer;
    (void)userdata;
    return size * nitems;  // Always report success
}

// HTTP method enumeration
enum http_method {
    HTTP_GET = 0,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_HEAD,
    HTTP_OPTIONS,
    HTTP_PATCH,
    HTTP_METHOD_MAX
};

// HTTP version enumeration  
enum http_version {
    HTTP_VERSION_1_0 = 0,
    HTTP_VERSION_1_1,
    HTTP_VERSION_2_0,
    HTTP_VERSION_2TLS,
    HTTP_VERSION_2_PRIOR_KNOWLEDGE,
    HTTP_VERSION_3,
    HTTP_VERSION_3ONLY,
    HTTP_VERSION_MAX
};

// Proxy type enumeration
enum proxy_type {
    PROXY_HTTP = 0,
    PROXY_HTTPS,
    PROXY_SOCKS4,
    PROXY_SOCKS5,
    PROXY_SOCKS5_HOSTNAME,
    PROXY_TYPE_MAX
};

// Authentication type enumeration
enum auth_type {
    AUTH_NONE = 0,
    AUTH_BASIC,
    AUTH_DIGEST,
    AUTH_NTLM,
    AUTH_NEGOTIATE,
    AUTH_BEARER,
    AUTH_TYPE_MAX
};

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for comprehensive HTTP protocol fuzzing
    // Need enough for all configuration parameters, URLs, headers, and data
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size configuration data first
    uint8_t http_method = fdp.ConsumeIntegral<uint8_t>() % HTTP_METHOD_MAX;
    uint8_t http_version = fdp.ConsumeIntegral<uint8_t>() % HTTP_VERSION_MAX;
    uint8_t proxy_type = fdp.ConsumeIntegral<uint8_t>() % PROXY_TYPE_MAX;
    uint8_t auth_type = fdp.ConsumeIntegral<uint8_t>() % AUTH_TYPE_MAX;
    uint8_t use_proxy = fdp.ConsumeBool();
    uint8_t use_auth = fdp.ConsumeBool();
    uint8_t use_chunked = fdp.ConsumeBool();
    uint8_t use_keepalive = fdp.ConsumeBool();
    uint8_t use_followlocation = fdp.ConsumeBool();
    uint8_t use_compression = fdp.ConsumeBool();
    uint8_t use_http09 = fdp.ConsumeBool();
    uint8_t use_verbose = fdp.ConsumeBool();
    
    // Numeric parameters
    uint16_t timeout = fdp.ConsumeIntegralInRange<uint16_t>(1, 30);
    uint16_t max_redirects = fdp.ConsumeIntegralInRange<uint16_t>(0, 10);
    uint32_t post_data_size = fdp.ConsumeIntegralInRange<uint32_t>(0, 65536);
    uint16_t num_headers = fdp.ConsumeIntegralInRange<uint16_t>(0, 20);
    
    // String length parameters
    uint16_t url_length = fdp.ConsumeIntegralInRange<uint16_t>(8, 256);
    uint16_t proxy_url_length = fdp.ConsumeIntegralInRange<uint16_t>(0, 256);
    uint16_t username_length = fdp.ConsumeIntegralInRange<uint16_t>(0, 128);
    uint16_t password_length = fdp.ConsumeIntegralInRange<uint16_t>(0, 128);
    uint16_t user_agent_length = fdp.ConsumeIntegralInRange<uint16_t>(0, 128);
    uint16_t referer_length = fdp.ConsumeIntegralInRange<uint16_t>(0, 128);
    
    // Consume string data
    std::string url = fdp.ConsumeBytesAsString(url_length);
    std::string proxy_url = fdp.ConsumeBytesAsString(proxy_url_length);
    std::string username = fdp.ConsumeBytesAsString(username_length);
    std::string password = fdp.ConsumeBytesAsString(password_length);
    std::string user_agent = fdp.ConsumeBytesAsString(user_agent_length);
    std::string referer = fdp.ConsumeBytesAsString(referer_length);
    
    // Consume POST data if needed
    std::vector<uint8_t> post_data;
    if (post_data_size > 0 && fdp.remaining_bytes() >= post_data_size) {
        post_data = fdp.ConsumeBytes<uint8_t>(post_data_size);
    }
    
    // Consume header data
    std::vector<std::string> headers;
    for (uint16_t i = 0; i < num_headers && fdp.remaining_bytes() > 4; i++) {
        uint16_t header_length = fdp.ConsumeIntegralInRange<uint16_t>(1, 256);
        if (fdp.remaining_bytes() >= header_length) {
            headers.push_back(fdp.ConsumeBytesAsString(header_length));
        }
    }
    
    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    CURLcode res;
    
    // 1. Set URL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    // 2. Set write callback to discard response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, nullptr);
    
    // 3. Set header callback to discard headers
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, discard_header_cb);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, nullptr);
    
    // 4. Set HTTP version
    long curl_http_version = CURL_HTTP_VERSION_1_1; // Default
    switch (http_version) {
        case HTTP_VERSION_1_0:
            curl_http_version = CURL_HTTP_VERSION_1_0;
            break;
        case HTTP_VERSION_1_1:
            curl_http_version = CURL_HTTP_VERSION_1_1;
            break;
        case HTTP_VERSION_2_0:
            curl_http_version = CURL_HTTP_VERSION_2_0;
            break;
        case HTTP_VERSION_2TLS:
            curl_http_version = CURL_HTTP_VERSION_2TLS;
            break;
        case HTTP_VERSION_2_PRIOR_KNOWLEDGE:
            curl_http_version = CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE;
            break;
        case HTTP_VERSION_3:
            curl_http_version = CURL_HTTP_VERSION_3;
            break;
        case HTTP_VERSION_3ONLY:
            curl_http_version = CURL_HTTP_VERSION_3ONLY;
            break;
    }
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, curl_http_version);
    
    // 5. Set HTTP method
    switch (http_method) {
        case HTTP_GET:
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            break;
        case HTTP_POST:
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            if (!post_data.empty()) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.data());
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data.size());
            }
            break;
        case HTTP_PUT:
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            if (!post_data.empty()) {
                curl_easy_setopt(curl, CURLOPT_READDATA, nullptr);
                curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)post_data.size());
            }
            break;
        case HTTP_DELETE:
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
            break;
        case HTTP_HEAD:
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            break;
        case HTTP_OPTIONS:
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "OPTIONS");
            break;
        case HTTP_PATCH:
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
            if (!post_data.empty()) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.data());
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data.size());
            }
            break;
    }
    
    // 6. Set proxy configuration if enabled
    if (use_proxy && !proxy_url.empty()) {
        curl_easy_setopt(curl, CURLOPT_PROXY, proxy_url.c_str());
        
        // Set proxy type
        long curl_proxy_type = CURLPROXY_HTTP; // Default
        switch (proxy_type) {
            case PROXY_HTTP:
                curl_proxy_type = CURLPROXY_HTTP;
                break;
            case PROXY_HTTPS:
                curl_proxy_type = CURLPROXY_HTTPS;
                break;
            case PROXY_SOCKS4:
                curl_proxy_type = CURLPROXY_SOCKS4;
                break;
            case PROXY_SOCKS5:
                curl_proxy_type = CURLPROXY_SOCKS5;
                break;
            case PROXY_SOCKS5_HOSTNAME:
                curl_proxy_type = CURLPROXY_SOCKS5_HOSTNAME;
                break;
        }
        curl_easy_setopt(curl, CURLOPT_PROXYTYPE, curl_proxy_type);
        
        // Set proxy authentication if credentials provided
        if (!username.empty() || !password.empty()) {
            std::string proxy_auth = username + ":" + password;
            curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, proxy_auth.c_str());
        }
    }
    
    // 7. Set authentication if enabled
    if (use_auth && (!username.empty() || !password.empty())) {
        std::string auth = username + ":" + password;
        curl_easy_setopt(curl, CURLOPT_USERPWD, auth.c_str());
        
        // Set authentication method
        long curl_auth = CURLAUTH_ANY; // Default
        switch (auth_type) {
            case AUTH_BASIC:
                curl_auth = CURLAUTH_BASIC;
                break;
            case AUTH_DIGEST:
                curl_auth = CURLAUTH_DIGEST;
                break;
            case AUTH_NTLM:
                curl_auth = CURLAUTH_NTLM;
                break;
            case AUTH_NEGOTIATE:
                curl_auth = CURLAUTH_NEGOTIATE;
                break;
            case AUTH_BEARER:
                curl_easy_setopt(curl, CURLOPT_XOAUTH2_BEARER, username.c_str());
                break;
        }
        if (auth_type != AUTH_BEARER) {
            curl_easy_setopt(curl, CURLOPT_HTTPAUTH, curl_auth);
        }
    }
    
    // 8. Set additional HTTP options
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)timeout);
    
    if (use_followlocation) {
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, (long)max_redirects);
    }
    
    if (use_chunked) {
        curl_easy_setopt(curl, CURLOPT_HTTP_TRANSFER_DECODING, 1L);
    }
    
    if (use_keepalive) {
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, 5L);
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, 5L);
    }
    
    if (use_compression) {
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
    }
    
    if (use_http09) {
        curl_easy_setopt(curl, CURLOPT_HTTP09_ALLOWED, 1L);
    }
    
    if (use_verbose) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    }
    
    // 9. Set custom headers if any
    struct curl_slist *header_list = nullptr;
    if (!headers.empty()) {
        for (const auto& header : headers) {
            header_list = curl_slist_append(header_list, header.c_str());
        }
        if (header_list) {
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
        }
    }
    
    // 10. Set User-Agent if provided
    if (!user_agent.empty()) {
        curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str());
    }
    
    // 11. Set Referer if provided
    if (!referer.empty()) {
        curl_easy_setopt(curl, CURLOPT_REFERER, referer.c_str());
    }
    
    // 12. Set SSL options for HTTPS testing
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    
    // 13. Perform the request (ignore result, we're fuzzing)
    res = curl_easy_perform(curl);
    
    // 14. Try to get some response info for coverage
    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    
    double total_time = 0;
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    
    // 15. Clean up header list if we created one
    if (header_list) {
        curl_slist_free_all(header_list);
    }
    
    // 16. Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_017

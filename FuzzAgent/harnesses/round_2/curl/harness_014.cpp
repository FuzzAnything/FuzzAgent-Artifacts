#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/options.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_014 {

// Simple write callback that discards data
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Just discard the received data
    return size * nmemb;
}

// Progress callback
static int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    // Always continue
    return 0;
}

// Header callback
static size_t header_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Just discard the headers
    return size * nmemb;
}

// Debug callback
static int debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    // Always return 0 for success
    return 0;
}

// Sockopt callback
static int sockopt_callback(void *clientp, curl_socket_t curlfd, curlsocktype purpose) {
    // Always return 0 for success
    return 0;
}

// Seek callback
static int seek_callback(void *clientp, curl_off_t offset, int origin) {
    // Always return CURL_SEEKFUNC_OK
    return CURL_SEEKFUNC_OK;
}

// SSL context callback
static CURLcode ssl_ctx_callback(CURL *curl, void *ssl_ctx, void *userptr) {
    // Always return CURLE_OK
    return CURLE_OK;
}

// Chunk begin callback
static long chunk_bgn_callback(const void *transfer_info, void *ptr, int remains) {
    // Return CURL_CHUNK_BGN_FUNC_OK
    return CURL_CHUNK_BGN_FUNC_OK;
}

// Chunk end callback
static long chunk_end_callback(void *ptr) {
    // Return CURL_CHUNK_END_FUNC_OK
    return CURL_CHUNK_END_FUNC_OK;
}

// FN match callback
static int fnmatch_callback(void *ptr, const char *pattern, const char *string) {
    // Return CURL_FNMATCHFUNC_MATCH
    return CURL_FNMATCHFUNC_MATCH;
}

// Trailer callback
static int trailer_callback(struct curl_slist **list, void *userdata) {
    // Return 0 for success
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum size for comprehensive option testing
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ============================================
    // PHASE 1: INITIALIZATION (as per guidance)
    // ============================================
    
    // Initialize curl library - must be called once
    CURLcode global_init_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_result != CURLE_OK) {
        return 0;
    }
    
    // 1. Call curl_easy_init() to create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // ============================================
    // PHASE 2: OPTION LOOKUP (as per guidance)
    // ============================================
    
    // 2. Call curl_easy_option_by_name for option lookup
    // Test looking up various option names based on fuzzed input
    std::vector<std::string> option_names_to_test = {
        "URL", "VERBOSE", "TIMEOUT", "FOLLOWLOCATION", "WRITEFUNCTION",
        "HEADERFUNCTION", "PROGRESSFUNCTION", "DEBUGFUNCTION",
        "SSL_VERIFYPEER", "SSL_VERIFYHOST", "USERAGENT", "REFERER",
        "COOKIE", "CUSTOMREQUEST", "HTTP_VERSION", "PROXY", "NOPROXY"
    };
    
    for (const auto& option_name : option_names_to_test) {
        const struct curl_easyoption *opt = curl_easy_option_by_name(option_name.c_str());
        // Option may be NULL if not found - that's okay for testing
        (void)opt;
    }
    
    // Also test lookup by ID
    std::vector<CURLoption> option_ids_to_test = {
        CURLOPT_URL, CURLOPT_VERBOSE, CURLOPT_TIMEOUT, CURLOPT_FOLLOWLOCATION,
        CURLOPT_WRITEFUNCTION, CURLOPT_HEADERFUNCTION, CURLOPT_PROGRESSFUNCTION,
        CURLOPT_DEBUGFUNCTION, CURLOPT_SSL_VERIFYPEER, CURLOPT_SSL_VERIFYHOST
    };
    
    for (CURLoption opt_id : option_ids_to_test) {
        const struct curl_easyoption *opt = curl_easy_option_by_id(opt_id);
        // Option may be NULL if not found - that's okay for testing
        (void)opt;
    }
    
    // ============================================
    // PHASE 3: SETOPT WITH VARIOUS OPTION TYPES (as per guidance)
    // ============================================
    
    // Consume fuzzed data for various parameters
    std::string url = fdp.ConsumeRandomLengthString(256);
    std::string user_agent = fdp.ConsumeRandomLengthString(100);
    std::string referer = fdp.ConsumeRandomLengthString(100);
    std::string cookie_data = fdp.ConsumeRandomLengthString(512);
    std::string custom_request = fdp.ConsumeRandomLengthString(50);
    std::string proxy_url = fdp.ConsumeRandomLengthString(200);
    
    // Test different data types for curl_easy_setopt:
    
    // 1. STRINGPOINT options (char*)
    curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
    if (!user_agent.empty()) {
        curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, user_agent.c_str());
    }
    if (!referer.empty()) {
        curl_easy_setopt(easy_handle, CURLOPT_REFERER, referer.c_str());
    }
    if (!cookie_data.empty()) {
        curl_easy_setopt(easy_handle, CURLOPT_COOKIE, cookie_data.c_str());
    }
    if (!custom_request.empty()) {
        curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, custom_request.c_str());
    }
    if (!proxy_url.empty()) {
        curl_easy_setopt(easy_handle, CURLOPT_PROXY, proxy_url.c_str());
    }
    
    // 2. LONG options
    bool verbose = fdp.ConsumeBool();
    bool follow_location = fdp.ConsumeBool();
    bool verify_peer = fdp.ConsumeBool();
    bool verify_host = fdp.ConsumeBool();
    int timeout = fdp.ConsumeIntegralInRange<int>(1, 60);
    int connect_timeout = fdp.ConsumeIntegralInRange<int>(1, 30);
    int max_redirects = fdp.ConsumeIntegralInRange<int>(0, 10);
    
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, verbose ? 1L : 0L);
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, follow_location ? 1L : 0L);
    curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, verify_peer ? 1L : 0L);
    curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYHOST, verify_host ? 2L : 0L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, timeout);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, connect_timeout);
    curl_easy_setopt(easy_handle, CURLOPT_MAXREDIRS, max_redirects);
    
    // 3. FUNCTIONPOINT options (callback functions)
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, progress_callback);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, debug_callback);
    
    // 4. CBPOINT options (callback data)
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, nullptr);
    curl_easy_setopt(easy_handle, CURLOPT_HEADERDATA, nullptr);
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSDATA, nullptr);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGDATA, nullptr);
    
    // 5. Test some VALUES options (enums)
    int http_version = fdp.ConsumeIntegralInRange<int>(0, 4);
    switch (http_version) {
        case 0: curl_easy_setopt(easy_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_NONE); break;
        case 1: curl_easy_setopt(easy_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0); break;
        case 2: curl_easy_setopt(easy_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1); break;
        case 3: curl_easy_setopt(easy_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0); break;
        case 4: curl_easy_setopt(easy_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS); break;
    }
    
    // 6. Test OFF_T options
    curl_off_t max_filesize = fdp.ConsumeIntegral<curl_off_t>();
    curl_easy_setopt(easy_handle, CURLOPT_MAXFILESIZE_LARGE, max_filesize);
    
    // 7. Test SLISTPOINT option (create a simple list)
    struct curl_slist *headers = nullptr;
    std::string header1 = fdp.ConsumeRandomLengthString(50);
    std::string header2 = fdp.ConsumeRandomLengthString(50);
    
    if (!header1.empty()) {
        std::string full_header = "X-Test-Header: " + header1;
        headers = curl_slist_append(headers, full_header.c_str());
    }
    if (!header2.empty()) {
        std::string full_header = "X-Another-Header: " + header2;
        headers = curl_slist_append(headers, full_header.c_str());
    }
    
    if (headers) {
        curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
    }
    
    // 8. Test OBJECTPOINT option (using share handle)
    CURLSH *share_handle = curl_share_init();
    if (share_handle) {
        curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
        curl_easy_setopt(easy_handle, CURLOPT_SHARE, share_handle);
    }
    
    // ============================================
    // PHASE 4: MINIMAL OPERATION SETUP (as per guidance)
    // ============================================
    
    // We don't actually perform network operations in fuzzing,
    // but we set up everything as if we would
    
    // ============================================
    // PHASE 5: GETINFO WITH VARIOUS INFO TYPES (as per guidance)
    // ============================================
    
    // Test retrieving various info types even without performing transfer
    
    // 1. STRING info types
    char *effective_url = nullptr;
    char *content_type = nullptr;
    char *primary_ip = nullptr;
    char *local_ip = nullptr;
    
    curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &effective_url);
    curl_easy_getinfo(easy_handle, CURLINFO_CONTENT_TYPE, &content_type);
    curl_easy_getinfo(easy_handle, CURLINFO_PRIMARY_IP, &primary_ip);
    curl_easy_getinfo(easy_handle, CURLINFO_LOCAL_IP, &local_ip);
    
    // 2. LONG info types
    long response_code = 0;
    long header_size = 0;
    long request_size = 0;
    long ssl_verify_result = 0;
    long redirect_count = 0;
    long http_connect_code = 0;
    long os_errno = 0;
    long num_connects = 0;
    
    curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_getinfo(easy_handle, CURLINFO_HEADER_SIZE, &header_size);
    curl_easy_getinfo(easy_handle, CURLINFO_REQUEST_SIZE, &request_size);
    curl_easy_getinfo(easy_handle, CURLINFO_SSL_VERIFYRESULT, &ssl_verify_result);
    curl_easy_getinfo(easy_handle, CURLINFO_REDIRECT_COUNT, &redirect_count);
    curl_easy_getinfo(easy_handle, CURLINFO_HTTP_CONNECTCODE, &http_connect_code);
    curl_easy_getinfo(easy_handle, CURLINFO_OS_ERRNO, &os_errno);
    curl_easy_getinfo(easy_handle, CURLINFO_NUM_CONNECTS, &num_connects);
    
    // 3. DOUBLE info types
    double total_time = 0.0;
    double namelookup_time = 0.0;
    double connect_time = 0.0;
    double pretransfer_time = 0.0;
    double starttransfer_time = 0.0;
    double speed_download = 0.0;
    double speed_upload = 0.0;
    double content_length_download = 0.0;
    double content_length_upload = 0.0;
    
    curl_easy_getinfo(easy_handle, CURLINFO_TOTAL_TIME, &total_time);
    curl_easy_getinfo(easy_handle, CURLINFO_NAMELOOKUP_TIME, &namelookup_time);
    curl_easy_getinfo(easy_handle, CURLINFO_CONNECT_TIME, &connect_time);
    curl_easy_getinfo(easy_handle, CURLINFO_PRETRANSFER_TIME, &pretransfer_time);
    curl_easy_getinfo(easy_handle, CURLINFO_STARTTRANSFER_TIME, &starttransfer_time);
    curl_easy_getinfo(easy_handle, CURLINFO_SPEED_DOWNLOAD, &speed_download);
    curl_easy_getinfo(easy_handle, CURLINFO_SPEED_UPLOAD, &speed_upload);
    curl_easy_getinfo(easy_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &content_length_download);
    curl_easy_getinfo(easy_handle, CURLINFO_CONTENT_LENGTH_UPLOAD, &content_length_upload);
    
    // 4. OFF_T info types (large integer types)
    curl_off_t size_upload_t = 0;
    curl_off_t size_download_t = 0;
    curl_off_t speed_download_t = 0;
    curl_off_t speed_upload_t = 0;
    
    curl_easy_getinfo(easy_handle, CURLINFO_SIZE_UPLOAD_T, &size_upload_t);
    curl_easy_getinfo(easy_handle, CURLINFO_SIZE_DOWNLOAD_T, &size_download_t);
    curl_easy_getinfo(easy_handle, CURLINFO_SPEED_DOWNLOAD_T, &speed_download_t);
    curl_easy_getinfo(easy_handle, CURLINFO_SPEED_UPLOAD_T, &speed_upload_t);
    
    // 5. SLIST info type
    struct curl_slist *cookie_list = nullptr;
    curl_easy_getinfo(easy_handle, CURLINFO_COOKIELIST, &cookie_list);
    
    // 6. SOCKET info type
    curl_socket_t active_socket = CURL_SOCKET_BAD;
    curl_easy_getinfo(easy_handle, CURLINFO_ACTIVESOCKET, &active_socket);
    
    // ============================================
    // PHASE 6: CLEANUP (as per guidance)
    // ============================================
    
    // Clean up headers list if created
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    // Clean up share handle if created
    if (share_handle) {
        curl_share_cleanup(share_handle);
    }
    
    // 6. Call curl_easy_cleanup to free resources
    curl_easy_cleanup(easy_handle);
    
    // Clean up global state
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_014

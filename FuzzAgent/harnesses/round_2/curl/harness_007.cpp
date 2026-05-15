#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_007 {

// Callback functions for various callback types
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Discard received data
    return size * nmemb;
}

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp) {
    // Provide data from fuzzer input
    FuzzedDataProvider *fdp = static_cast<FuzzedDataProvider*>(userp);
    if (fdp == nullptr || fdp->remaining_bytes() == 0) {
        return 0;
    }
    
    size_t bytes_to_read = size * nmemb;
    if (bytes_to_read > fdp->remaining_bytes()) {
        bytes_to_read = fdp->remaining_bytes();
    }
    
    auto data = fdp->ConsumeBytes<uint8_t>(bytes_to_read);
    memcpy(ptr, data.data(), data.size());
    return data.size() / size;
}

static int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    // Always continue
    return 0;
}

static int debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userp) {
    // Ignore debug output
    return 0;
}

static size_t header_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Discard headers
    return size * nmemb;
}

static curl_socket_t opensocket_callback(void *clientp, curlsocktype purpose, struct curl_sockaddr *address) {
    // Return invalid socket to prevent actual network operations
    return CURL_SOCKET_BAD;
}

static int sockopt_callback(void *clientp, curl_socket_t curlfd, curlsocktype purpose) {
    // Do nothing
    return CURL_SOCKOPT_OK;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum size for comprehensive testing
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize curl library
    CURLcode global_init_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_result != CURLE_OK) {
        return 0;
    }
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // Consume fuzzed data for various parameters
    std::string url = fdp.ConsumeRandomLengthString(256);
    std::string proxy = fdp.ConsumeRandomLengthString(256);
    std::string user_agent = fdp.ConsumeRandomLengthString(100);
    std::string referer = fdp.ConsumeRandomLengthString(100);
    std::string cookie = fdp.ConsumeRandomLengthString(100);
    std::string post_data = fdp.ConsumeRandomLengthString(1024);
    
    // Consume numeric parameters
    long timeout = fdp.ConsumeIntegralInRange<long>(1, 60);
    long connect_timeout = fdp.ConsumeIntegralInRange<long>(1, 30);
    long low_speed_limit = fdp.ConsumeIntegralInRange<long>(1, 1024);
    long low_speed_time = fdp.ConsumeIntegralInRange<long>(1, 60);
    long buffer_size = fdp.ConsumeIntegralInRange<long>(1, 65536);
    long dns_cache_timeout = fdp.ConsumeIntegralInRange<long>(0, 3600);
    
    // Consume boolean parameters
    bool verbose = fdp.ConsumeBool();
    bool follow_location = fdp.ConsumeBool();
    bool no_signal = fdp.ConsumeBool();
    bool tcp_nodelay = fdp.ConsumeBool();
    bool ssl_verifypeer = fdp.ConsumeBool();
    bool ssl_verifyhost = fdp.ConsumeBool();
    bool forbid_reuse = fdp.ConsumeBool();
    bool fresh_connect = fdp.ConsumeBool();
    bool http_proxy_tunnel = fdp.ConsumeBool();
    bool ssl_sessionid_cache = fdp.ConsumeBool();
    bool http_transfer_decoding = fdp.ConsumeBool();
    bool http_content_decoding = fdp.ConsumeBool();
    bool tcp_keepalive = fdp.ConsumeBool();
    
    // Consume other parameters
    long http_version = fdp.ConsumeIntegralInRange<long>(0, 31);
    long ip_resolve = fdp.ConsumeIntegralInRange<long>(0, 2);
    long ftp_method = fdp.ConsumeIntegralInRange<long>(0, 2);
    long ftp_use_eprt = fdp.ConsumeIntegralInRange<long>(0, 1);
    long ftp_use_pret = fdp.ConsumeIntegralInRange<long>(0, 1);
    long use_ssl = fdp.ConsumeIntegralInRange<long>(0, 3);
    long ftpsslauth = fdp.ConsumeIntegralInRange<long>(0, 2);
    long ftp_create_missing_dirs = fdp.ConsumeIntegralInRange<long>(0, 2);
    long ftp_ssl_ccc = fdp.ConsumeIntegralInRange<long>(0, 2);
    long ssh_auth_types = fdp.ConsumeIntegral<long>();
    long post_redir = fdp.ConsumeIntegralInRange<long>(0, 3);
    
    // Set a comprehensive set of options to exercise typecheck macros
    
    // String options (curlcheck_string_option)
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if (!proxy.empty()) {
        curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    }
    if (!user_agent.empty()) {
        curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str());
    }
    if (!referer.empty()) {
        curl_easy_setopt(curl, CURLOPT_REFERER, referer.c_str());
    }
    if (!cookie.empty()) {
        curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.c_str());
    }
    
    // Long options (curlcheck_long_option)
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connect_timeout);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, low_speed_limit);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, low_speed_time);
    curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, buffer_size);
    curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, dns_cache_timeout);
    
    // Boolean options (as long)
    curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, follow_location ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, no_signal ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, tcp_nodelay ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, ssl_verifypeer ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, ssl_verifyhost ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, forbid_reuse ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_FRESH_CONNECT, fresh_connect ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_HTTPPROXYTUNNEL, http_proxy_tunnel ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_SESSIONID_CACHE, ssl_sessionid_cache ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_HTTP_TRANSFER_DECODING, http_transfer_decoding ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_HTTP_CONTENT_DECODING, http_content_decoding ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, tcp_keepalive ? 1L : 0L);
    
    // Value options (curlcheck_long_option with specific values)
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, http_version);
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, ip_resolve);
    curl_easy_setopt(curl, CURLOPT_FTP_FILEMETHOD, ftp_method);
    curl_easy_setopt(curl, CURLOPT_FTP_USE_EPRT, ftp_use_eprt);
    curl_easy_setopt(curl, CURLOPT_FTP_USE_PRET, ftp_use_pret);
    curl_easy_setopt(curl, CURLOPT_USE_SSL, use_ssl);
    curl_easy_setopt(curl, CURLOPT_FTPSSLAUTH, ftpsslauth);
    curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, ftp_create_missing_dirs);
    curl_easy_setopt(curl, CURLOPT_FTP_SSL_CCC, ftp_ssl_ccc);
    curl_easy_setopt(curl, CURLOPT_SSH_AUTH_TYPES, ssh_auth_types);
    curl_easy_setopt(curl, CURLOPT_POSTREDIR, post_redir);
    
    // Callback options (curlcheck_write_cb_option, curlcheck_read_cb, etc.)
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &fdp);
    
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_READDATA, &fdp);
    
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &fdp);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L); // Enable progress callback
    
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &fdp);
    
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, debug_callback);
    curl_easy_setopt(curl, CURLOPT_DEBUGDATA, &fdp);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // Enable debug output for callback
    
    // Socket and network options
    curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, opensocket_callback);
    curl_easy_setopt(curl, CURLOPT_OPENSOCKETDATA, &fdp);
    
    curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, sockopt_callback);
    curl_easy_setopt(curl, CURLOPT_SOCKOPTDATA, &fdp);
    
    // POST data options (curlcheck_postfields_option)
    if (!post_data.empty()) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data.size());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
    }
    
    // Off_t options (curlcheck_off_t_option) - need to use curl_off_t
    curl_off_t max_filesize = fdp.ConsumeIntegral<curl_off_t>();
    curl_easy_setopt(curl, CURLOPT_MAXFILESIZE_LARGE, max_filesize);
    
    // Object pointer options
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, nullptr); // Will allocate if needed
    
    // Try to perform the request - it will fail due to network constraints
    // but will still exercise the setup and validation code
    CURLcode result = curl_easy_perform(curl);
    
    // Get various info types to exercise curl_easy_getinfo typecheck macros
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    
    double total_time = 0.0;
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    
    double namelookup_time = 0.0;
    curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &namelookup_time);
    
    double connect_time = 0.0;
    curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &connect_time);
    
    double pretransfer_time = 0.0;
    curl_easy_getinfo(curl, CURLINFO_PRETRANSFER_TIME, &pretransfer_time);
    
    double starttransfer_time = 0.0;
    curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &starttransfer_time);
    
    double size_upload = 0.0;
    curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD, &size_upload);
    
    double size_download = 0.0;
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &size_download);
    
    double speed_upload = 0.0;
    curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
    
    double speed_download = 0.0;
    curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &speed_download);
    
    long redirect_count = 0;
    curl_easy_getinfo(curl, CURLINFO_REDIRECT_COUNT, &redirect_count);
    
    char *effective_url = nullptr;
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
    
    char *content_type = nullptr;
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
    
    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_007

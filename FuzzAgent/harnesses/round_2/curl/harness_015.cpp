#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_015 {

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

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum size for comprehensive FTP testing
    // We need enough data for URL, FTP port, timeout values, and operation selection
    if (size < 64) {
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
    // PHASE 2: SET FTP URL (as per guidance)
    // ============================================
    
    // 2. Call curl_easy_setopt with CURLOPT_URL for FTP URL
    // Generate FTP URL with fuzzed data
    std::string ftp_host = fdp.ConsumeRandomLengthString(100);
    std::string ftp_path = fdp.ConsumeRandomLengthString(200);
    std::string ftp_user = fdp.ConsumeRandomLengthString(50);
    std::string ftp_pass = fdp.ConsumeRandomLengthString(50);
    
    // Construct FTP URL with various formats
    std::string ftp_url;
    if (fdp.ConsumeBool()) {
        // Simple FTP URL
        ftp_url = "ftp://" + ftp_host + "/" + ftp_path;
    } else {
        // FTP URL with credentials
        ftp_url = "ftp://" + ftp_user + ":" + ftp_pass + "@" + ftp_host + "/" + ftp_path;
    }
    
    curl_easy_setopt(easy_handle, CURLOPT_URL, ftp_url.c_str());
    
    // ============================================
    // PHASE 3: FTP-SPECIFIC CONFIGURATION (as per guidance)
    // ============================================
    
    // Consume fuzzed data for various FTP parameters
    std::string ftp_port = fdp.ConsumeRandomLengthString(50);
    int ftp_response_timeout = fdp.ConsumeIntegralInRange<int>(1, 300);
    bool use_epsv = fdp.ConsumeBool();
    bool use_eprt = fdp.ConsumeBool();
    bool skip_pasv_ip = fdp.ConsumeBool();
    bool use_pret = fdp.ConsumeBool();
    int ftp_filemethod = fdp.ConsumeIntegralInRange<int>(0, 3);
    int ftp_ssl_auth = fdp.ConsumeIntegralInRange<int>(0, 2);
    int ftp_ssl_ccc = fdp.ConsumeIntegralInRange<int>(0, 2);
    int ftp_create_dirs = fdp.ConsumeIntegralInRange<int>(0, 2);
    
    // 3. Call FTP-specific curl_easy_setopt() options with fuzzed parameters
    
    // a) CURLOPT_FTPPORT - Active mode port specification
    if (!ftp_port.empty()) {
        curl_easy_setopt(easy_handle, CURLOPT_FTPPORT, ftp_port.c_str());
    }
    
    // b) CURLOPT_FTP_RESPONSE_TIMEOUT (aliased as CURLOPT_SERVER_RESPONSE_TIMEOUT)
    curl_easy_setopt(easy_handle, CURLOPT_SERVER_RESPONSE_TIMEOUT, ftp_response_timeout);
    
    // c) CURLOPT_FTP_CREATE_MISSING_DIRS - Directory creation option
    curl_easy_setopt(easy_handle, CURLOPT_FTP_CREATE_MISSING_DIRS, ftp_create_dirs);
    curl_easy_setopt(easy_handle, CURLOPT_FTP_USE_EPSV, use_epsv ? 1L : 0L);
    
    // e) CURLOPT_FTP_USE_EPRT - Use EPRT for active mode
    curl_easy_setopt(easy_handle, CURLOPT_FTP_USE_EPRT, use_eprt ? 1L : 0L);
    
    // f) CURLOPT_FTP_SKIP_PASV_IP - Skip the IP address in PASV response
    curl_easy_setopt(easy_handle, CURLOPT_FTP_SKIP_PASV_IP, skip_pasv_ip ? 1L : 0L);
    
    // g) CURLOPT_FTP_USE_PRET - Use PRET before PASV/EPSV
    curl_easy_setopt(easy_handle, CURLOPT_FTP_USE_PRET, use_pret ? 1L : 0L);
    
    // h) CURLOPT_FTP_FILEMETHOD - FTP file transfer method
    switch (ftp_filemethod) {
        case 0: curl_easy_setopt(easy_handle, CURLOPT_FTP_FILEMETHOD, CURLFTPMETHOD_DEFAULT); break;
        case 1: curl_easy_setopt(easy_handle, CURLOPT_FTP_FILEMETHOD, CURLFTPMETHOD_MULTICWD); break;
        case 2: curl_easy_setopt(easy_handle, CURLOPT_FTP_FILEMETHOD, CURLFTPMETHOD_NOCWD); break;
        case 3: curl_easy_setopt(easy_handle, CURLOPT_FTP_FILEMETHOD, CURLFTPMETHOD_SINGLECWD); break;
    }
    
    // i) CURLOPT_FTPSSLAUTH - FTP SSL authentication method
    switch (ftp_ssl_auth) {
        case 0: curl_easy_setopt(easy_handle, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_DEFAULT); break;
        case 1: curl_easy_setopt(easy_handle, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_SSL); break;
        case 2: curl_easy_setopt(easy_handle, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_TLS); break;
    }
    
    // j) CURLOPT_FTP_SSL_CCC - FTP SSL Clear Command Channel
    switch (ftp_ssl_ccc) {
        case 0: curl_easy_setopt(easy_handle, CURLOPT_FTP_SSL_CCC, CURLFTPSSL_CCC_NONE); break;
        case 1: curl_easy_setopt(easy_handle, CURLOPT_FTP_SSL_CCC, CURLFTPSSL_CCC_PASSIVE); break;
        case 2: curl_easy_setopt(easy_handle, CURLOPT_FTP_SSL_CCC, CURLFTPSSL_CCC_ACTIVE); break;
    }
    
    // k) CURLOPT_USE_SSL - Enable SSL for FTP
    int use_ssl = fdp.ConsumeIntegralInRange<int>(0, 3);
    switch (use_ssl) {
        case 0: curl_easy_setopt(easy_handle, CURLOPT_USE_SSL, CURLUSESSL_NONE); break;
        case 1: curl_easy_setopt(easy_handle, CURLOPT_USE_SSL, CURLUSESSL_TRY); break;
        case 2: curl_easy_setopt(easy_handle, CURLOPT_USE_SSL, CURLUSESSL_CONTROL); break;
        case 3: curl_easy_setopt(easy_handle, CURLOPT_USE_SSL, CURLUSESSL_ALL); break;
    }
    
    // l) FTP account (if available)
    std::string ftp_account = fdp.ConsumeRandomLengthString(50);
    if (!ftp_account.empty()) {
        curl_easy_setopt(easy_handle, CURLOPT_FTP_ACCOUNT, ftp_account.c_str());
    }
    
    // m) FTP alternative to USER command
    std::string ftp_alternative = fdp.ConsumeRandomLengthString(50);
    if (!ftp_alternative.empty()) {
        curl_easy_setopt(easy_handle, CURLOPT_FTP_ALTERNATIVE_TO_USER, ftp_alternative.c_str());
    }
    
    // ============================================
    // PHASE 4: ADDITIONAL FTP-SPECIFIC OPTIONS
    // ============================================
    
    // Set up callbacks to avoid actual network operations
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, progress_callback);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGFUNCTION, debug_callback);
    
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, nullptr);
    curl_easy_setopt(easy_handle, CURLOPT_HEADERDATA, nullptr);
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSDATA, nullptr);
    curl_easy_setopt(easy_handle, CURLOPT_DEBUGDATA, nullptr);
    
    // Set connection timeout to prevent hanging
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 2L);
    
    // Disable SSL verification for fuzzing
    curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYHOST, 0L);
    
    // ============================================
    // PHASE 5: FTP COMMAND QUOTES (advanced FTP feature)
    // ============================================
    
    // Test FTP quote commands if we have enough data
    if (fdp.remaining_bytes() > 50) {
        struct curl_slist *quote_commands = nullptr;
        std::string quote1 = fdp.ConsumeRandomLengthString(50);
        std::string quote2 = fdp.ConsumeRandomLengthString(50);
        
        if (!quote1.empty()) {
            quote_commands = curl_slist_append(quote_commands, quote1.c_str());
        }
        if (!quote2.empty()) {
            quote_commands = curl_slist_append(quote_commands, quote2.c_str());
        }
        
        if (quote_commands) {
            curl_easy_setopt(easy_handle, CURLOPT_QUOTE, quote_commands);
        }
        
        // Also test post-quote commands
        struct curl_slist *postquote_commands = nullptr;
        std::string postquote = fdp.ConsumeRandomLengthString(50);
        
        if (!postquote.empty()) {
            postquote_commands = curl_slist_append(postquote_commands, postquote.c_str());
        }
        
        if (postquote_commands) {
            curl_easy_setopt(easy_handle, CURLOPT_POSTQUOTE, postquote_commands);
        }
        
        // Clean up quote lists after setting options
        // Note: libcurl takes ownership of the lists, so we don't free them here
    }
    
    // ============================================
    // PHASE 6: EXECUTION (as per guidance)
    // ============================================
    
    // 4. Call curl_easy_perform(handle) to execute FTP operations
    // Note: This will fail due to network connectivity, but will exercise the FTP state machine
    CURLcode perform_result = curl_easy_perform(easy_handle);
    
    // Try to get FTP-specific info even if perform failed
    char *effective_url = nullptr;
    curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &effective_url);
    
    long response_code = 0;
    curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &response_code);
    
    double total_time = 0.0;
    curl_easy_getinfo(easy_handle, CURLINFO_TOTAL_TIME, &total_time);
    
    // FTP-specific info
    long ftp_entry_path = 0;
    curl_easy_getinfo(easy_handle, CURLINFO_FTP_ENTRY_PATH, &ftp_entry_path);
    
    // ============================================
    // PHASE 7: CLEANUP (as per guidance)
    // ============================================
    
    // 5. Call curl_easy_cleanup(handle) to release resources
    curl_easy_cleanup(easy_handle);
    
    // Clean up global state
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_015

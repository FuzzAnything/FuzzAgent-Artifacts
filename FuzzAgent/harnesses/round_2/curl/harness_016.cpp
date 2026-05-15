#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_016 {

// ============================================
// CALLBACK FUNCTIONS FOR COMPREHENSIVE FTP TESTING
// ============================================

// Write callback: handles data received from FTP server
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    // For fuzzing, we can discard data or simulate processing
    // Return CURL_WRITEFUNC_PAUSE or CURL_WRITEFUNC_ERROR based on fuzzed input
    uint8_t *fuzz_decision = static_cast<uint8_t*>(userp);
    if (fuzz_decision && *fuzz_decision % 100 < 5) {
        // Occasionally return PAUSE (5% chance)
        return CURL_WRITEFUNC_PAUSE;
    } else if (fuzz_decision && *fuzz_decision % 100 < 10) {
        // Occasionally return ERROR (5% chance)
        return CURL_WRITEFUNC_ERROR;
    }
    // Normally, accept all data
    return total_size;
}

// Read callback: provides data to upload to FTP server
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp) {
    size_t buffer_size = size * nmemb;
    FuzzedDataProvider *fdp = static_cast<FuzzedDataProvider*>(userp);
    
    if (!fdp || fdp->remaining_bytes() == 0) {
        return 0; // EOF
    }
    
    // Determine how much data to return based on fuzzed input
    size_t bytes_to_return = fdp->ConsumeIntegralInRange<size_t>(0, buffer_size);
    if (bytes_to_return == 0) {
        return 0; // EOF
    }
    
    // Get data from fuzzer
    std::vector<uint8_t> data = fdp->ConsumeBytes<uint8_t>(bytes_to_return);
    memcpy(ptr, data.data(), data.size());
    
    return data.size();
}

// Xferinfo callback (replaces old progress callback): provides detailed transfer info
static int xferinfo_callback(void *clientp, 
                             curl_off_t dltotal, curl_off_t dlnow,
                             curl_off_t ultotal, curl_off_t ulnow) {
    // For fuzzing, we can return different values based on input
    uint8_t *fuzz_decision = static_cast<uint8_t*>(clientp);
    if (fuzz_decision) {
        int decision = *fuzz_decision % 100;
        if (decision < 10) {
            return 1; // Abort transfer (10% chance)
        } else if (decision < 20) {
            return CURL_PROGRESSFUNC_CONTINUE; // Continue with default (10% chance)
        }
        // Otherwise continue normally (80% chance)
    }
    return 0; // Continue normally
}

// ============================================
// MAIN FUZZER FUNCTION
// ============================================

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size for comprehensive FTP testing with callbacks
    // Need data for: URL, FTP options, callback decisions, upload data
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ============================================
    // PHASE 1: INITIALIZATION (as per guidance)
    // ============================================
    
    // 1. Call curl_global_init(CURL_GLOBAL_ALL) for global initialization
    CURLcode global_init_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_result != CURLE_OK) {
        return 0;
    }
    
    // 2. Call curl_easy_init() to create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // ============================================
    // PHASE 2: SET FTP URL (as per guidance)
    // ============================================
    
    // Generate FTP URL with fuzzed data
    std::string ftp_host = fdp.ConsumeRandomLengthString(100);
    std::string ftp_path = fdp.ConsumeRandomLengthString(200);
    std::string ftp_user = fdp.ConsumeRandomLengthString(50);
    std::string ftp_pass = fdp.ConsumeRandomLengthString(50);
    
    // Construct FTP URL with various formats
    std::string ftp_url;
    int url_format = fdp.ConsumeIntegralInRange<int>(0, 3);
    switch (url_format) {
        case 0:
            // Simple FTP URL
            ftp_url = "ftp://" + ftp_host + "/" + ftp_path;
            break;
        case 1:
            // FTP URL with credentials
            ftp_url = "ftp://" + ftp_user + ":" + ftp_pass + "@" + ftp_host + "/" + ftp_path;
            break;
        case 2: {
            // FTP URL with port
            int port = fdp.ConsumeIntegralInRange<int>(1, 65535);
            ftp_url = "ftp://" + ftp_host + ":" + std::to_string(port) + "/" + ftp_path;
            break;
        }
        case 3:
            // FTP URL with file upload (PUT)
            ftp_url = "ftp://" + ftp_host + "/" + ftp_path + ";type=i";
            break;
    }
    
    curl_easy_setopt(easy_handle, CURLOPT_URL, ftp_url.c_str());
    
    // ============================================
    // PHASE 3: FTP-SPECIFIC OPTIONS (as per guidance)
    // ============================================
    
    // a) CURLOPT_FTPPORT - Active mode port specification
    std::string ftp_port = fdp.ConsumeRandomLengthString(30);
    if (!ftp_port.empty()) {
        curl_easy_setopt(easy_handle, CURLOPT_FTPPORT, ftp_port.c_str());
    }
    
    // b) CURLOPT_FTP_CREATE_MISSING_DIRS - Directory creation option
    int ftp_create_dirs = fdp.ConsumeIntegralInRange<int>(0, 2);
    curl_easy_setopt(easy_handle, CURLOPT_FTP_CREATE_MISSING_DIRS, ftp_create_dirs);
    
    // c) CURLOPT_FTP_FILEMETHOD - FTP file transfer method
    int ftp_filemethod = fdp.ConsumeIntegralInRange<int>(0, 3);
    switch (ftp_filemethod) {
        case 0:
            curl_easy_setopt(easy_handle, CURLOPT_FTP_FILEMETHOD, CURLFTPMETHOD_DEFAULT);
            break;
        case 1:
            curl_easy_setopt(easy_handle, CURLOPT_FTP_FILEMETHOD, CURLFTPMETHOD_MULTICWD);
            break;
        case 2:
            curl_easy_setopt(easy_handle, CURLOPT_FTP_FILEMETHOD, CURLFTPMETHOD_NOCWD);
            break;
        case 3:
            curl_easy_setopt(easy_handle, CURLOPT_FTP_FILEMETHOD, CURLFTPMETHOD_SINGLECWD);
            break;
    }
    
    // d) CURLOPT_FTP_RESPONSE_TIMEOUT (aliased as CURLOPT_SERVER_RESPONSE_TIMEOUT)
    long ftp_response_timeout = fdp.ConsumeIntegralInRange<long>(1, 300);
    curl_easy_setopt(easy_handle, CURLOPT_SERVER_RESPONSE_TIMEOUT, ftp_response_timeout);
    
    // Additional FTP-specific options
    bool use_epsv = fdp.ConsumeBool();
    bool use_eprt = fdp.ConsumeBool();
    bool skip_pasv_ip = fdp.ConsumeBool();
    bool use_pret = fdp.ConsumeBool();
    
    curl_easy_setopt(easy_handle, CURLOPT_FTP_USE_EPSV, use_epsv ? 1L : 0L);
    curl_easy_setopt(easy_handle, CURLOPT_FTP_USE_EPRT, use_eprt ? 1L : 0L);
    curl_easy_setopt(easy_handle, CURLOPT_FTP_SKIP_PASV_IP, skip_pasv_ip ? 1L : 0L);
    curl_easy_setopt(easy_handle, CURLOPT_FTP_USE_PRET, use_pret ? 1L : 0L);
    
    // FTP SSL options
    int ftp_ssl_auth = fdp.ConsumeIntegralInRange<int>(0, 2);
    switch (ftp_ssl_auth) {
        case 0:
            curl_easy_setopt(easy_handle, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_DEFAULT);
            break;
        case 1:
            curl_easy_setopt(easy_handle, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_SSL);
            break;
        case 2:
            curl_easy_setopt(easy_handle, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_TLS);
            break;
    }
    
    // FTP account
    std::string ftp_account = fdp.ConsumeRandomLengthString(50);
    if (!ftp_account.empty()) {
        curl_easy_setopt(easy_handle, CURLOPT_FTP_ACCOUNT, ftp_account.c_str());
    }
    
    // ============================================
    // PHASE 4: CALLBACK CONFIGURATION (primary focus)
    // ============================================
    
    // Create fuzzing decision variables for callbacks
    uint8_t write_decision = fdp.ConsumeIntegral<uint8_t>();
    uint8_t xferinfo_decision = fdp.ConsumeIntegral<uint8_t>();
    
    // 3. Call curl_easy_setopt() with callback options as per guidance:
    
    // a) CURLOPT_WRITEFUNCTION - Write callback
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &write_decision);
    
    // b) CURLOPT_READFUNCTION - Read callback (for uploads)
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, &fdp);
    
    // c) CURLOPT_XFERINFOFUNCTION - Xferinfo callback (modern replacement for progress)
    curl_easy_setopt(easy_handle, CURLOPT_XFERINFOFUNCTION, xferinfo_callback);
    curl_easy_setopt(easy_handle, CURLOPT_XFERINFODATA, &xferinfo_decision);
    
    // Enable progress/xferinfo callback
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 0L);
    
    // Also set the old PROGRESSFUNCTION for compatibility testing
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, xferinfo_callback);
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSDATA, &xferinfo_decision);
    
    // ============================================
    // PHASE 5: TRANSFER TYPE CONFIGURATION
    // ============================================
    
    // Determine transfer type: download or upload
    bool is_upload = fdp.ConsumeBool();
    if (is_upload && fdp.remaining_bytes() > 100) {
        // Configure for upload
        curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
        
        // Set upload size from remaining data
        size_t upload_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
        curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE_LARGE, static_cast<curl_off_t>(upload_size));
        
        // Consume some data for the upload
        std::vector<uint8_t> upload_data = fdp.ConsumeBytes<uint8_t>(
            fdp.ConsumeIntegralInRange<size_t>(0, std::min<size_t>(fdp.remaining_bytes(), 1024))
        );
        // Note: The read callback will provide this data
    } else {
        // Configure for download
        curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 0L);
    }
    
    // ============================================
    // PHASE 6: NETWORK SAFETY CONFIGURATION
    // ============================================
    
    // Set timeouts to prevent hanging
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 2L);
    
    // Disable SSL verification for fuzzing
    curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYHOST, 0L);
    
    // ============================================
    // PHASE 7: EXECUTION (as per guidance)
    // ============================================
    
    // 4. Call curl_easy_perform(handle) to execute FTP operations
    // This will exercise the FTP state machine (ftp_pp_statemachine, ftp_do, ftp_perform)
    CURLcode perform_result = curl_easy_perform(easy_handle);
    
    // Even if perform fails, try to get FTP-specific information
    char *effective_url = nullptr;
    curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &effective_url);
    
    long response_code = 0;
    curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &response_code);
    
    double total_time = 0.0;
    curl_easy_getinfo(easy_handle, CURLINFO_TOTAL_TIME, &total_time);
    
    // FTP-specific info
    char *ftp_entry_path = nullptr;
    curl_easy_getinfo(easy_handle, CURLINFO_FTP_ENTRY_PATH, &ftp_entry_path);
    
    long ftp_retry_after = 0;
    curl_easy_getinfo(easy_handle, CURLINFO_RETRY_AFTER, &ftp_retry_after);
    
    // ============================================
    // PHASE 8: CLEANUP (as per guidance)
    // ============================================
    
    // 5. Call curl_easy_cleanup(handle) to release resources
    curl_easy_cleanup(easy_handle);
    
    // 6. Call curl_global_cleanup() for global cleanup
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_016

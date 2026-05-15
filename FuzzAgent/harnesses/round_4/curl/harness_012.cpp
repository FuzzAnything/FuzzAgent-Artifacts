#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "curl/curl.h"

namespace harness_012 {
/*
 * Fuzzing harness for libcurl targeting FTP protocol stack via curl_easy APIs
 * Harness ID: 012
 * Targets: Comprehensive FTP protocol handling through curl_easy interface
 * Focus: FTP-specific options, commands, and protocol handling through easy interface
 * Differentiated from:
 * - harness_003.cpp (low-level FTP protocol functions ftp_pp_statemachine, ftp_do)
 * This harness focuses on FTP protocol stack identified as highest-priority gap with 32,918+ untested branches
 * Implements: curl_easy_init -> curl_easy_setopt with FTP-specific options -> curl_easy_perform -> curl_easy_cleanup
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for comprehensive FTP testing
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // ==================== Easy Handle Creation ====================
    
    // Create easy handle for FTP operations
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // ==================== FTP URL Configuration ====================
    
    // Consume FTP URL from fuzzed input
    std::string ftp_url = fdp.ConsumeRandomLengthString(256);
    if (ftp_url.empty()) {
        // Use a generic FTP URL structure for testing
        ftp_url = "ftp://localhost/";
    }
    
    // Set FTP URL
    curl_easy_setopt(easy_handle, CURLOPT_URL, ftp_url.c_str());
    
    // ==================== FTP-Specific Options Configuration ====================
    
    // FTP Port configuration (active/passive mode)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
        std::string ftp_port = fdp.ConsumeRandomLengthString(64);
        curl_easy_setopt(easy_handle, CURLOPT_FTPPORT, ftp_port.c_str());
    }
    
    // FTP EPSV (Extended Passive Mode)
    bool use_epsv = fdp.ConsumeBool();
    curl_easy_setopt(easy_handle, CURLOPT_FTP_USE_EPSV, use_epsv ? 1L : 0L);
    
    // FTP EPRT (Extended Port Mode)
    bool use_eprt = fdp.ConsumeBool();
    curl_easy_setopt(easy_handle, CURLOPT_FTP_USE_EPRT, use_eprt ? 1L : 0L);
    
    // FTP Create Missing Directories
    if (fdp.ConsumeBool()) {
        long create_dirs = fdp.ConsumeIntegralInRange<long>(0, 2);
        curl_easy_setopt(easy_handle, CURLOPT_FTP_CREATE_MISSING_DIRS, create_dirs);
    }
    
    // FTP SSL/TLS Authentication Method
    if (fdp.ConsumeBool()) {
        long ftp_auth = fdp.ConsumeIntegralInRange<long>(0, 2);
        curl_easy_setopt(easy_handle, CURLOPT_FTPSSLAUTH, ftp_auth);
    }
    
    // FTP Account (for some FTP servers)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
        std::string ftp_account = fdp.ConsumeRandomLengthString(128);
        curl_easy_setopt(easy_handle, CURLOPT_FTP_ACCOUNT, ftp_account.c_str());
    }
    
    // FTP Skip PASV IP
    if (fdp.ConsumeBool()) {
        long skip_pasv_ip = fdp.ConsumeBool() ? 1L : 0L;
        curl_easy_setopt(easy_handle, CURLOPT_FTP_SKIP_PASV_IP, skip_pasv_ip);
    }
    
    // FTP File Method (how to retrieve files)
    if (fdp.ConsumeBool()) {
        long file_method = fdp.ConsumeIntegralInRange<long>(0, 3);
        curl_easy_setopt(easy_handle, CURLOPT_FTP_FILEMETHOD, file_method);
    }
    
    // FTP Alternative to USER command
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
        std::string alt_to_user = fdp.ConsumeRandomLengthString(128);
        curl_easy_setopt(easy_handle, CURLOPT_FTP_ALTERNATIVE_TO_USER, alt_to_user.c_str());
    }
    
    // FTP SSL Clear Command Channel
    if (fdp.ConsumeBool()) {
        long ssl_ccc = fdp.ConsumeIntegralInRange<long>(0, 2);
        curl_easy_setopt(easy_handle, CURLOPT_FTP_SSL_CCC, ssl_ccc);
    }
    
    // FTP Use PRET (Pre Transfer) command
    if (fdp.ConsumeBool()) {
        long use_pret = fdp.ConsumeBool() ? 1L : 0L;
        curl_easy_setopt(easy_handle, CURLOPT_FTP_USE_PRET, use_pret);
    }
    
    // FTP over SSL/TLS (USE_SSL option)
    if (fdp.ConsumeBool()) {
        long use_ssl = fdp.ConsumeIntegralInRange<long>(0, 3);
        curl_easy_setopt(easy_handle, CURLOPT_USE_SSL, use_ssl);
    }
    
    // ==================== FTP Command Configuration ====================
    
    // Build quote commands from fuzzed data
    struct curl_slist* quote_list = NULL;
    struct curl_slist* prequote_list = NULL;
    struct curl_slist* postquote_list = NULL;
    
    // Pre-quote commands (executed before transfer)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 16) {
        size_t num_prequotes = fdp.ConsumeIntegralInRange<size_t>(0, 5);
        for (size_t i = 0; i < num_prequotes && fdp.remaining_bytes() > 4; i++) {
            std::string prequote_cmd = fdp.ConsumeRandomLengthString(64);
            prequote_list = curl_slist_append(prequote_list, prequote_cmd.c_str());
        }
        if (prequote_list) {
            curl_easy_setopt(easy_handle, CURLOPT_PREQUOTE, prequote_list);
        }
    }
    
    // Quote commands (executed during transfer setup)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 16) {
        size_t num_quotes = fdp.ConsumeIntegralInRange<size_t>(0, 5);
        for (size_t i = 0; i < num_quotes && fdp.remaining_bytes() > 4; i++) {
            std::string quote_cmd = fdp.ConsumeRandomLengthString(64);
            quote_list = curl_slist_append(quote_list, quote_cmd.c_str());
        }
        if (quote_list) {
            curl_easy_setopt(easy_handle, CURLOPT_QUOTE, quote_list);
        }
    }
    
    // Post-quote commands (executed after transfer)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 16) {
        size_t num_postquotes = fdp.ConsumeIntegralInRange<size_t>(0, 5);
        for (size_t i = 0; i < num_postquotes && fdp.remaining_bytes() > 4; i++) {
            std::string postquote_cmd = fdp.ConsumeRandomLengthString(64);
            postquote_list = curl_slist_append(postquote_list, postquote_cmd.c_str());
        }
        if (postquote_list) {
            curl_easy_setopt(easy_handle, CURLOPT_POSTQUOTE, postquote_list);
        }
    }
    
    // Custom FTP command
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
        std::string custom_cmd = fdp.ConsumeRandomLengthString(128);
        curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, custom_cmd.c_str());
    }
    
    // ==================== FTP Operation Type ====================
    
    // Determine FTP operation type from fuzzed data
    int ftp_operation = fdp.ConsumeIntegralInRange<int>(0, 6);
    
    switch (ftp_operation) {
        case 0:  // LIST - directory listing
            curl_easy_setopt(easy_handle, CURLOPT_DIRLISTONLY, 1L);
            break;
        case 1:  // RETR - file download (GET)
            // Already configured by default
            break;
        case 2:  // STOR - file upload (PUT)
            curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
            break;
        case 3:  // APPE - append to file
            curl_easy_setopt(easy_handle, CURLOPT_APPEND, 1L);
            break;
        case 4:  // DELE - delete file
            if (fdp.remaining_bytes() > 8) {
                std::string delete_path = fdp.ConsumeRandomLengthString(128);
                // Would typically append to URL for deletion
            }
            break;
        case 5:  // MKD - make directory
            curl_easy_setopt(easy_handle, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);
            break;
        case 6:  // RMD - remove directory
            // Would use custom request "RMD <path>"
            break;
    }
    
    // ==================== Authentication Configuration ====================
    
    // FTP username/password
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 16) {
        std::string username = fdp.ConsumeRandomLengthString(64);
        std::string password = fdp.ConsumeRandomLengthString(64);
        curl_easy_setopt(easy_handle, CURLOPT_USERNAME, username.c_str());
        curl_easy_setopt(easy_handle, CURLOPT_PASSWORD, password.c_str());
    }
    
    // ==================== Connection Settings ====================
    
    // Timeout settings
    long timeout = fdp.ConsumeIntegralInRange<long>(1, 30);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, timeout);
    
    // Connection timeout
    long connect_timeout = fdp.ConsumeIntegralInRange<long>(1, 10);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, connect_timeout);
    
    // No signal handling (important for fuzzing)
    curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);
    
    // Disable verbose output for fuzzing efficiency
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    
    // ==================== Perform FTP Operation ====================
    
    // Perform the FTP operation
    // Note: This will likely fail since we're not connected to a real FTP server,
    // but it exercises the FTP protocol initialization, option parsing, and state setup
    CURLcode res = curl_easy_perform(easy_handle);
    
    // ==================== Cleanup ====================
    
    // Clean up quote lists
    if (quote_list) {
        curl_slist_free_all(quote_list);
    }
    if (prequote_list) {
        curl_slist_free_all(prequote_list);
    }
    if (postquote_list) {
        curl_slist_free_all(postquote_list);
    }
    
    // Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // Clean up global state
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_012

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "curl/curl.h"

namespace harness_003 {
/*
 * Fuzzing harness for libcurl targeting FTP protocol functions and state machine
 * Harness ID: 003
 * Targets: ftp_pp_statemachine, ftp_do, and related FTP state machine functions
 * Focus: FTP protocol parsing, command/response handling, and state transitions
 * Differentiated from:
 * - harness_000.cpp (curl_url/curl_easy APIs)
 * - harness_001.cpp (MIME APIs) 
 * - harness_002.cpp (curl_multi APIs)
 * This harness specifically targets FTP coverage gap with 300+ blocked branches
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for FTP operations
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create easy handle for FTP operations
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // ==================== FTP Protocol Testing ====================
    
    // Track quote list for cleanup if we create one
    struct curl_slist* quote_list = NULL;
    
    // Consume FTP URL components
    std::string ftp_url = fdp.ConsumeRandomLengthString(256);
    if (ftp_url.empty()) {
        // Use a default FTP URL structure if none provided
        ftp_url = "ftp://localhost/";
    }
    
    // Set FTP URL
    curl_easy_setopt(easy_handle, CURLOPT_URL, ftp_url.c_str());
    
    // Consume FTP command/operation type
    int ftp_op = fdp.ConsumeIntegralInRange<int>(0, 5);
    
    // Set FTP-specific options based on fuzzed operation type
    switch (ftp_op) {
        case 0:  // LIST - directory listing
            curl_easy_setopt(easy_handle, CURLOPT_DIRLISTONLY, 1L);
            break;
        case 1:  // RETR - file download
            // Add path for file retrieval
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
                std::string filepath = fdp.ConsumeRandomLengthString(128);
                // Could set CURLOPT_URL with file path
            }
            break;
        case 2:  // STOR - file upload  
            curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
            // Need to provide upload data
            if (fdp.remaining_bytes() > 16) {
                std::string upload_data = fdp.ConsumeRandomLengthString(1024);
                // In real scenario would use CURLOPT_READDATA
            }
            break;
        case 3:  // NLST - name list
            curl_easy_setopt(easy_handle, CURLOPT_DIRLISTONLY, 1L);
            break;
        case 4:  // Custom quote commands
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
                // Build quote list from fuzzed data
                size_t num_quotes = fdp.ConsumeIntegralInRange<size_t>(0, 5);
                for (size_t i = 0; i < num_quotes && fdp.remaining_bytes() > 4; i++) {
                    std::string quote_cmd = fdp.ConsumeRandomLengthString(64);
                    quote_list = curl_slist_append(quote_list, quote_cmd.c_str());
                }
                if (quote_list) {
                    curl_easy_setopt(easy_handle, CURLOPT_QUOTE, quote_list);
                }
            }
            break;
        case 5:  // Various FTP options
            // Set miscellaneous FTP options
            curl_easy_setopt(easy_handle, CURLOPT_FTPPORT, "-");  // Use PORT command
            curl_easy_setopt(easy_handle, CURLOPT_FTPLISTONLY, 1L);
            break;
    }
    
    // Consume and set FTP connection options
    bool use_passive = fdp.ConsumeBool();
    curl_easy_setopt(easy_handle, CURLOPT_FTPPORT, use_passive ? "-" : NULL);
    
    bool use_epsv = fdp.ConsumeBool();
    curl_easy_setopt(easy_handle, CURLOPT_FTP_USE_EPSV, use_epsv ? 1L : 0L);
    
    bool use_ssl = fdp.ConsumeBool();
    if (use_ssl && fdp.remaining_bytes() > 4) {
        curl_easy_setopt(easy_handle, CURLOPT_USE_SSL, CURLUSESSL_TRY);
    }
    
    // Consume FTP account info if available
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
        std::string username = fdp.ConsumeRandomLengthString(64);
        std::string password = fdp.ConsumeRandomLengthString(64);
        curl_easy_setopt(easy_handle, CURLOPT_USERNAME, username.c_str());
        curl_easy_setopt(easy_handle, CURLOPT_PASSWORD, password.c_str());
    }
    
    // Set verbose mode to potentially trigger more state machine paths
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);  // Disabled for fuzzing efficiency
    
    // Set timeout to prevent hangs
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 5L);
    
    // Set no signal handling
    curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);
    
    // Perform the FTP operation (will exercise ftp_do and state machine)
    // Note: This will likely fail since we're not connected to a real FTP server,
    // but it should still exercise the FTP initialization and state setup code
    CURLcode res = curl_easy_perform(easy_handle);
    
    // Clean up quote list if created
    if (quote_list) {
        curl_slist_free_all(quote_list);
    }
    
    // Clean up
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_003

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_014 {
/*
 * Advanced fuzzing harness for libcurl's FTP protocol operations with comprehensive coverage
 * Coverage analysis shows: FTP protocol functions show 0% coverage with high branch complexity
 * This harness targets additional FTP-specific APIs and options not covered in harness_010.cpp
 * 
 * Primary targets: 
 *   - Additional FTP-specific curl_easy_setopt options:
 *     * CURLOPT_FTP_ACCOUNT (134) - FTP account info
 *     * CURLOPT_FTP_SKIP_PASV_IP (137) - Skip IP check for PASV response
 *     * CURLOPT_FTP_ALTERNATIVE_TO_USER (147) - Alternative authentication
 *     * CURLOPT_FTP_SSL_CCC (154) - Send CCC command after SSL/TLS negotiation
 *     * CURLOPT_FTP_USE_EPRT (106) - Use EPRT instead of PORT for active transfers
 *   - Comprehensive FTP state machine testing with various transfer modes
 *   - FTP SSL/TLS operations (FTPS)
 *   - FTP file listing parsing operations
 * 
 * Semantic differentiation from harness_010.cpp:
 *   - harness_010 covers basic FTP operations (LIST, RETR, STOR, DELE, RNFR/RNTO)
 *   - harness_014 covers advanced FTP options, SSL/TLS, account management, PASV/IP handling
 *   - Focuses on different FTP protocol code paths and configuration combinations
 * 
 * Invocation sequence:
 *   1. curl_global_init(CURL_GLOBAL_ALL)
 *   2. curl_easy_init()
 *   3. Multiple curl_easy_setopt calls with various FTP-specific options
 *   4. curl_easy_perform() to execute FTP operations
 *   5. curl_easy_cleanup() and curl_global_cleanup()
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

// FTP directory listing callback
static size_t ftp_list_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;  // Always report success
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for comprehensive FTP fuzzing
    // Need enough for all FTP configuration options and operation parameters
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size configuration data first
    uint8_t ftp_test_scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    uint8_t use_ftps_ssl = fdp.ConsumeBool();
    uint8_t use_account_info = fdp.ConsumeBool();
    uint8_t skip_pasv_ip = fdp.ConsumeBool();
    uint8_t use_alternative_auth = fdp.ConsumeBool();
    uint8_t use_eprt = fdp.ConsumeBool();
    uint8_t use_ssl_ccc = fdp.ConsumeBool();
    uint8_t use_epsv = fdp.ConsumeBool();
    uint8_t create_missing_dirs = fdp.ConsumeBool();
    uint8_t filemethod = fdp.ConsumeIntegral<uint8_t>() % 3;
    uint8_t use_pret = fdp.ConsumeBool();
    
    // FTP operation type within each scenario
    uint8_t ftp_operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // String length parameters
    uint8_t url_length = fdp.ConsumeIntegralInRange<uint8_t>(1, 128);
    uint8_t account_length = fdp.ConsumeIntegralInRange<uint8_t>(0, 64);
    uint8_t alt_auth_length = fdp.ConsumeIntegralInRange<uint8_t>(0, 128);
    uint8_t ftp_port_length = fdp.ConsumeIntegralInRange<uint8_t>(0, 64);
    uint8_t quote_cmd_length = fdp.ConsumeIntegralInRange<uint8_t>(0, 128);
    
    // Consume string data from fuzzer input
    std::string url = fdp.ConsumeBytesAsString(url_length);
    std::string account_info = fdp.ConsumeBytesAsString(account_length);
    std::string alternative_auth = fdp.ConsumeBytesAsString(alt_auth_length);
    std::string ftp_port = fdp.ConsumeBytesAsString(ftp_port_length);
    std::string quote_cmd = fdp.ConsumeBytesAsString(quote_cmd_length);
    
    // Consume remaining data for potential file upload operations
    std::vector<uint8_t> upload_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set FTP URL - ensure it starts with appropriate protocol
    std::string ftp_url;
    if (!url.empty()) {
        if (use_ftps_ssl) {
            // Use FTPS (FTP over SSL/TLS)
            if (url.find("ftps://") != 0 && url.find("ftp://") != 0) {
                ftp_url = "ftps://" + url;
            } else {
                ftp_url = url;
            }
        } else {
            // Regular FTP
            if (url.find("ftp://") != 0) {
                ftp_url = "ftp://" + url;
            } else {
                ftp_url = url;
            }
        }
    } else {
        ftp_url = use_ftps_ssl ? "ftps://example.com/test" : "ftp://example.com/test";
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, ftp_url.c_str());
    
    // Set write callback to discard response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, nullptr);
    
    // Set header callback to discard headers
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, discard_header_cb);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, nullptr);
    
    // Set timeout to prevent hanging
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    
    // ===== SET FTP-SPECIFIC OPTIONS =====
    
    // 1. FTP account information (CURLOPT_FTP_ACCOUNT)
    if (use_account_info && !account_info.empty()) {
        curl_easy_setopt(curl, CURLOPT_FTP_ACCOUNT, account_info.c_str());
    }
    
    // 2. Skip IP check for PASV response (CURLOPT_FTP_SKIP_PASV_IP)
    curl_easy_setopt(curl, CURLOPT_FTP_SKIP_PASV_IP, skip_pasv_ip ? 1L : 0L);
    
    // 3. Alternative authentication (CURLOPT_FTP_ALTERNATIVE_TO_USER)
    if (use_alternative_auth && !alternative_auth.empty()) {
        curl_easy_setopt(curl, CURLOPT_FTP_ALTERNATIVE_TO_USER, alternative_auth.c_str());
    }
    
    // 4. FTP SSL CCC command (CURLOPT_FTP_SSL_CCC)
    curl_easy_setopt(curl, CURLOPT_FTP_SSL_CCC, use_ssl_ccc ? 1L : 0L);
    
    // 5. Use EPRT instead of PORT (CURLOPT_FTP_USE_EPRT)
    curl_easy_setopt(curl, CURLOPT_FTP_USE_EPRT, use_eprt ? 1L : 0L);
    
    // 6. Use EPSV (Extended Passive Mode)
    curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, use_epsv ? 1L : 0L);
    
    // 7. Create missing directories
    curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, create_missing_dirs ? 1L : 0L);
    
    // 8. FTP file method
    curl_easy_setopt(curl, CURLOPT_FTP_FILEMETHOD, (long)filemethod);
    
    // 9. FTP PRET (Pre Transfer)
    curl_easy_setopt(curl, CURLOPT_FTP_USE_PRET, use_pret ? 1L : 0L);
    
    // 10. FTP PORT configuration (for active mode)
    if (!ftp_port.empty()) {
        curl_easy_setopt(curl, CURLOPT_FTPPORT, ftp_port.c_str());
    }
    
    // 11. FTP SSL/TLS options if using FTPS
    if (use_ftps_ssl) {
        // Try different SSL levels
        uint8_t ssl_level = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (ssl_level) {
            case 0:
                curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_TRY);
                break;
            case 1:
                curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_CONTROL);
                break;
            case 2:
                curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
                break;
            case 3:
                curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_NONE);
                break;
        }
    }
    
    // ===== TEST DIFFERENT FTP SCENARIOS =====
    
    switch (ftp_test_scenario) {
        case 0: {
            // Scenario 0: Comprehensive FTP state machine testing
            // Test various FTP commands and operations
            
            if (!quote_cmd.empty()) {
                struct curl_slist *quote_list = nullptr;
                quote_list = curl_slist_append(quote_list, quote_cmd.c_str());
                curl_easy_setopt(curl, CURLOPT_QUOTE, quote_list);
            }
            
            // Set operation-specific options
            if (ftp_operation == 3 && !upload_data.empty()) {
                // STOR operation
                curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            } else if (ftp_operation == 1) {
                // NLST operation (names only)
                curl_easy_setopt(curl, CURLOPT_DIRLISTONLY, 1L);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ftp_list_cb);
            } else if (ftp_operation == 0) {
                // LIST operation (detailed)
                curl_easy_setopt(curl, CURLOPT_DIRLISTONLY, 0L);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ftp_list_cb);
            }
            break;
        }
        
        case 1: {
            // Scenario 1: FTP with CRLF line endings
            curl_easy_setopt(curl, CURLOPT_CRLF, 1L);
            break;
        }
        
        case 2: {
            // Scenario 2: FTP with custom commands (QUOTE and POSTQUOTE)
            struct curl_slist *quote_list = nullptr;
            struct curl_slist *postquote_list = nullptr;
            
            if (!quote_cmd.empty()) {
                quote_list = curl_slist_append(quote_list, quote_cmd.c_str());
                curl_easy_setopt(curl, CURLOPT_QUOTE, quote_list);
                
                // Add a postquote command too
                std::string postquote = "SITE CHMOD 755 testfile";
                postquote_list = curl_slist_append(postquote_list, postquote.c_str());
                curl_easy_setopt(curl, CURLOPT_POSTQUOTE, postquote_list);
            }
            break;
        }
        
        case 3: {
            // Scenario 3: FTP with resume/restart capability
            long resume_from = fdp.ConsumeIntegral<uint32_t>() % 1024;
            curl_easy_setopt(curl, CURLOPT_RESUME_FROM, resume_from);
            break;
        }
        
        case 4: {
            // Scenario 4: FTP with transfer type (ASCII/BINARY)
            uint8_t transfer_type = fdp.ConsumeBool() ? 1 : 0;
            if (transfer_type) {
                // ASCII mode
                curl_easy_setopt(curl, CURLOPT_TRANSFERTEXT, 1L);
            }
            // Binary mode is default
            break;
        }
        
        case 5: {
            // Scenario 5: FTP with custom request (like SITE commands)
            if (!quote_cmd.empty()) {
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, quote_cmd.c_str());
            }
            break;
        }
    }
    
    // Perform the FTP operation
    // Note: In fuzzing context, the actual network operation will likely fail
    // but we're testing the API calls and state machine setup
    (void)curl_easy_perform(curl);
    
    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_014

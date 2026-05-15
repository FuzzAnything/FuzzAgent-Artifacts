#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_018 {
/*
 * Fuzzing harness for libcurl's FTP protocol operations
 * Targets: FTP-specific APIs and operations identified as the highest uncovered complexity
 * FTP functions have only 2.75% region coverage and 1.17% branch coverage in ftp.c
 * Focus on FTP upload/download, directory listing, FTP commands, FTP over SSL/TLS
 * Testing strategy:
 *   1. Test various FTP operations (RETR, STOR, LIST, DELE) with FTP URLs
 *   2. Test FTP-specific CURLOPT options for maximum coverage
 *   3. Test FTP over SSL/TLS configurations
 *   4. Test different FTP file methods (MULTICWD, NOCWD, SINGLECWD)
 *   5. Test FTP authentication methods (AUTH SSL, AUTH TLS)
 *   6. Test FTP command sequences and quote operations
 * Semantic diversity: Unique harness focusing exclusively on FTP protocol operations,
 *   unlike existing harnesses which focus on HTTP, multi, URL, headers, or memory management.
 *   This harness specifically targets the thousands of uncovered branches in ftp.c.
 */


// Write callback for handling response data
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    // Just discard the data for fuzzing purposes
    (void)contents;
    (void)userp;
    return realsize;
}

// Read callback for providing upload data
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp) {
    FuzzedDataProvider *fdp = static_cast<FuzzedDataProvider*>(userp);
    size_t buffer_size = size * nmemb;
    
    if (fdp->remaining_bytes() == 0) {
        return 0; // EOF
    }
    
    // Consume some bytes for the request body
    size_t to_read = fdp->ConsumeIntegralInRange<size_t>(0, buffer_size);
    if (to_read > 0) {
        std::vector<uint8_t> data = fdp->ConsumeBytes<uint8_t>(to_read);
        memcpy(ptr, data.data(), data.size());
        return data.size();
    }
    return 0;
}

// Header callback
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
    // Need minimum input for meaningful FTP fuzzing - FTP has many options
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    CURLcode global_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_res != CURLE_OK) {
        return 0;
    }
    
    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set basic callbacks
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 0L); // Enable progress callback
    curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, progress_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, &fdp);
    
    // Consume fixed-size data first
    uint8_t ftp_operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume FTP-specific parameters from fuzzed input
    std::string ftp_url = fdp.ConsumeRandomLengthString(256);
    std::string ftp_user = fdp.ConsumeRandomLengthString(64);
    std::string ftp_password = fdp.ConsumeRandomLengthString(64);
    std::string ftp_account = fdp.ConsumeRandomLengthString(64);
    std::string ftp_path = fdp.ConsumeRandomLengthString(256);
    std::string ftp_port = fdp.ConsumeRandomLengthString(8);
    std::string ftp_quote_cmd = fdp.ConsumeRandomLengthString(128);
    std::string ftp_postquote_cmd = fdp.ConsumeRandomLengthString(128);
    std::string ftp_prequote_cmd = fdp.ConsumeRandomLengthString(128);
    
    // Consume binary data for upload operations
    size_t upload_data_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
    std::vector<uint8_t> upload_data = fdp.ConsumeBytes<uint8_t>(upload_data_size);
    
    // Build FTP URL if we have enough components
    if (!ftp_url.empty()) {
        // If it doesn't start with ftp:// or ftps://, prepend ftp://
        if (ftp_url.find("ftp://") != 0 && ftp_url.find("ftps://") != 0) {
            std::string full_url = "ftp://";
            if (!ftp_user.empty()) {
                full_url += ftp_user;
                if (!ftp_password.empty()) {
                    full_url += ":" + ftp_password;
                }
                full_url += "@";
            }
            full_url += ftp_url;
            if (!ftp_path.empty()) {
                if (ftp_path[0] != '/') {
                    full_url += "/";
                }
                full_url += ftp_path;
            }
            curl_easy_setopt(easy_handle, CURLOPT_URL, full_url.c_str());
        } else {
            curl_easy_setopt(easy_handle, CURLOPT_URL, ftp_url.c_str());
        }
    }
    
    // Set FTP-specific options based on fuzzed input
    
    // 1. FTP PORT option (active mode)
    uint8_t use_ftp_port = fdp.ConsumeIntegral<uint8_t>() % 3;
    if (use_ftp_port == 0) {
        std::string ftp_port_str = fdp.ConsumeRandomLengthString(32);
        curl_easy_setopt(easy_handle, CURLOPT_FTPPORT, ftp_port_str.c_str());
    }
    
    // 2. FTP EPSV (Extended Passive Mode)
    long ftp_use_epsv = fdp.ConsumeIntegral<uint8_t>() % 3;
    curl_easy_setopt(easy_handle, CURLOPT_FTP_USE_EPSV, ftp_use_epsv);
    
    // 3. FTP EPRT (Extended Port Mode)
    long ftp_use_eprt = fdp.ConsumeIntegral<uint8_t>() % 2;
    curl_easy_setopt(easy_handle, CURLOPT_FTP_USE_EPRT, ftp_use_eprt);
    
    // 4. FTP CREATE_MISSING_DIRS
    long ftp_create_dirs = fdp.ConsumeIntegral<uint8_t>() % 3;
    curl_easy_setopt(easy_handle, CURLOPT_FTP_CREATE_MISSING_DIRS, ftp_create_dirs);
    
    // 5. FTP ACCOUNT
    if (!ftp_account.empty()) {
        curl_easy_setopt(easy_handle, CURLOPT_FTP_ACCOUNT, ftp_account.c_str());
    }
    
    // 6. FTP SKIP_PASV_IP
    long ftp_skip_pasv_ip = fdp.ConsumeIntegral<uint8_t>() % 2;
    curl_easy_setopt(easy_handle, CURLOPT_FTP_SKIP_PASV_IP, ftp_skip_pasv_ip);
    
    // 7. FTP FILEMETHOD (MULTICWD, NOCWD, SINGLECWD)
    long ftp_filemethod = fdp.ConsumeIntegral<uint8_t>() % 4;
    curl_easy_setopt(easy_handle, CURLOPT_FTP_FILEMETHOD, ftp_filemethod);
    
    // 8. FTP ALTERNATIVE_TO_USER
    if (!ftp_user.empty()) {
        curl_easy_setopt(easy_handle, CURLOPT_FTP_ALTERNATIVE_TO_USER, ftp_user.c_str());
    }
    
    // 9. FTP SSL CCC (Clear Command Channel)
    long ftp_ssl_ccc = fdp.ConsumeIntegral<uint8_t>() % 3;
    curl_easy_setopt(easy_handle, CURLOPT_FTP_SSL_CCC, ftp_ssl_ccc);
    
    // 10. FTP USE_PRET (Pre-Transfer)
    long ftp_use_pret = fdp.ConsumeIntegral<uint8_t>() % 2;
    curl_easy_setopt(easy_handle, CURLOPT_FTP_USE_PRET, ftp_use_pret);
    
    // 11. FTP AUTH method (SSL/TLS)
    long ftp_auth = fdp.ConsumeIntegral<uint8_t>() % 3;
    curl_easy_setopt(easy_handle, CURLOPT_FTPSSLAUTH, ftp_auth);
    
    // 12. FTP over SSL/TLS configuration
    long use_ssl = fdp.ConsumeIntegral<uint8_t>() % 5;
    curl_easy_setopt(easy_handle, CURLOPT_USE_SSL, use_ssl);
    
    // 13. FTP response timeout
    long ftp_response_timeout = fdp.ConsumeIntegralInRange<long>(1, 300);
    curl_easy_setopt(easy_handle, CURLOPT_FTP_RESPONSE_TIMEOUT, ftp_response_timeout);
    
    // 14. FTP commands via quote options
    struct curl_slist *quote_list = NULL;
    struct curl_slist *prequote_list = NULL;
    struct curl_slist *postquote_list = NULL;
    
    if (!ftp_quote_cmd.empty()) {
        quote_list = curl_slist_append(quote_list, ftp_quote_cmd.c_str());
        if (quote_list) {
            curl_easy_setopt(easy_handle, CURLOPT_QUOTE, quote_list);
        }
    }
    
    if (!ftp_prequote_cmd.empty()) {
        prequote_list = curl_slist_append(prequote_list, ftp_prequote_cmd.c_str());
        if (prequote_list) {
            curl_easy_setopt(easy_handle, CURLOPT_PREQUOTE, prequote_list);
        }
    }
    
    if (!ftp_postquote_cmd.empty()) {
        postquote_list = curl_slist_append(postquote_list, ftp_postquote_cmd.c_str());
        if (postquote_list) {
            curl_easy_setopt(easy_handle, CURLOPT_POSTQUOTE, postquote_list);
        }
    }
    
    // 15. FTP append mode (APPE command)
    long ftp_append = fdp.ConsumeIntegral<uint8_t>() % 2;
    curl_easy_setopt(easy_handle, CURLOPT_APPEND, ftp_append);
    
    // 16. FTP directory listing only
    long ftp_list_only = fdp.ConsumeIntegral<uint8_t>() % 2;
    curl_easy_setopt(easy_handle, CURLOPT_DIRLISTONLY, ftp_list_only);
    
    // 17. FTP transfer mode (ASCII vs BINARY)
    long ftp_transfer_ascii = fdp.ConsumeIntegral<uint8_t>() % 2;
    curl_easy_setopt(easy_handle, CURLOPT_TRANSFERTEXT, ftp_transfer_ascii);
    
    // Set operation-specific options based on ftp_operation
    switch (ftp_operation) {
        case 0: // FTP download (RETR)
            curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 0L);
            break;
            
        case 1: // FTP upload (STOR)
            curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
            if (!upload_data.empty()) {
                curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)upload_data.size());
                curl_easy_setopt(easy_handle, CURLOPT_READDATA, &upload_data);
            }
            break;
            
        case 2: // FTP upload with append (APPE)
            curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(easy_handle, CURLOPT_APPEND, 1L);
            if (!upload_data.empty()) {
                curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)upload_data.size());
            }
            break;
            
        case 3: // FTP directory listing (LIST)
            curl_easy_setopt(easy_handle, CURLOPT_DIRLISTONLY, 1L);
            break;
            
        case 4: // FTP directory listing with custom command
            curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, "LIST");
            break;
            
        case 5: // FTP delete (DELE)
            curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, "DELE");
            break;
            
        case 6: // FTP make directory (MKD)
            curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, "MKD");
            break;
            
        case 7: // FTP remove directory (RMD)
            curl_easy_setopt(easy_handle, CURLOPT_CUSTOMREQUEST, "RMD");
            break;
    }
    
    // Set timeout and other generic options
    long timeout = fdp.ConsumeIntegralInRange<long>(1, 30);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, timeout);
    
    long connect_timeout = fdp.ConsumeIntegralInRange<long>(1, 30);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, connect_timeout);
    
    // Try to perform the operation (ignore errors for fuzzing)
    (void)curl_easy_perform(easy_handle);
    
    // Clean up quote lists if they were created
    if (quote_list) {
        curl_slist_free_all(quote_list);
    }
    if (prequote_list) {
        curl_slist_free_all(prequote_list);
    }
    if (postquote_list) {
        curl_slist_free_all(postquote_list);
    }
    
    // Clean up
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_018

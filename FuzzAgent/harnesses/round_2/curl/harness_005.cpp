#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sys/stat.h>

namespace harness_005 {

// Simple write callback that discards data
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Just discard the received data
    return size * nmemb;
}

// Progress callback for share interface testing
static int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    // Always continue
    return 0;
}

// Header callback for share interface testing
static size_t header_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Just discard the headers
    return size * nmemb;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum size for comprehensive operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize curl library - must be called once
    CURLcode global_init_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_result != CURLE_OK) {
        return 0;
    }
    
    // ============================================
    // TEST 1: SHARE INTERFACE (Primary Target)
    // ============================================
    {
        // Create share handle
        CURLSH *share_handle = curl_share_init();
        if (share_handle) {
            // Consume share options from fuzzed data
            uint8_t share_options = fdp.ConsumeIntegral<uint8_t>();
            
            // Set share options based on consumed bits
            if (share_options & 0x01) {
                curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
            }
            if (share_options & 0x02) {
                curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
            }
            if (share_options & 0x04) {
                curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
            }
            if (share_options & 0x08) {
                curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_CONNECT);
            }
            if (share_options & 0x10) {
                curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_PSL);
            }
            if (share_options & 0x20) {
                curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_HSTS);
            }
            
            // Test unlocking data types
            if (share_options & 0x40) {
                curl_share_setopt(share_handle, CURLSHOPT_UNSHARE, CURL_LOCK_DATA_COOKIE);
            }
            if (share_options & 0x80) {
                curl_share_setopt(share_handle, CURLSHOPT_UNSHARE, CURL_LOCK_DATA_DNS);
            }
            
            // Create easy handle and associate with share
            CURL *easy_handle = curl_easy_init();
            if (easy_handle) {
                // Set basic options
                std::string url = fdp.ConsumeRandomLengthString(128);
                curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
                
                // Associate share handle with easy handle
                curl_easy_setopt(easy_handle, CURLOPT_SHARE, share_handle);
                
                // Set write callback
                curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
                
                // Try to perform (will likely fail without network, but that's OK)
                curl_easy_perform(easy_handle);
                
                // Clean up easy handle
                curl_easy_cleanup(easy_handle);
            }
            
            // Clean up share handle
            curl_share_cleanup(share_handle);
        }
    }
    
    // ============================================
    // TEST 2: COMPREHENSIVE MIME API BEYOND HEADERS
    // ============================================
    {
        // Create easy handle for MIME operations
        CURL *easy_handle = curl_easy_init();
        if (easy_handle) {
            // Create MIME handle
            curl_mime *mime = curl_mime_init(easy_handle);
            if (mime) {
                // Create multiple MIME parts with different configurations
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 32; i++) {
                    curl_mimepart *part = curl_mime_addpart(mime);
                    if (part) {
                        // Consume MIME data type decision
                        uint8_t mime_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                        
                        switch (mime_type) {
                            case 0: {
                                // Test curl_mime_data with random string data
                                std::string mime_data = fdp.ConsumeRandomLengthString(256);
                                curl_mime_data(part, mime_data.c_str(), mime_data.size());
                                break;
                            }
                            case 1: {
                                // Test curl_mime_filedata with fuzzed filename (API will fail but exercise code)
                                std::string filename = fdp.ConsumeRandomLengthString(64);
                                // Just test the API call - file won't exist, but that's OK for fuzzing
                                // This exercises the parameter validation and error handling in curl_mime_filedata
                                curl_mime_filedata(part, filename.c_str());
                                
                                // Also set data as fallback to ensure we test some MIME functionality
                                std::string mime_data = fdp.ConsumeRandomLengthString(128);
                                curl_mime_data(part, mime_data.c_str(), mime_data.size());
                                break;
                            }
                            case 2: {
                                // Test curl_mime_type with various MIME types
                                const char* mime_types[] = {
                                    "text/plain",
                                    "text/html", 
                                    "application/json",
                                    "image/jpeg",
                                    "application/octet-stream"
                                };
                                uint8_t type_idx = fdp.ConsumeIntegral<uint8_t>() % 5;
                                curl_mime_type(part, mime_types[type_idx]);
                                
                                // Also set some data
                                std::string mime_data = fdp.ConsumeRandomLengthString(128);
                                curl_mime_data(part, mime_data.c_str(), mime_data.size());
                                break;
                            }
                            case 3: {
                                // Test curl_mime_encoder with various encodings
                                std::string mime_data = fdp.ConsumeRandomLengthString(128);
                                curl_mime_data(part, mime_data.c_str(), mime_data.size());
                                
                                // Test different encoders
                                uint8_t encoder_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                                switch (encoder_type) {
                                    case 0:
                                        curl_mime_encoder(part, "base64");
                                        break;
                                    case 1:
                                        curl_mime_encoder(part, "quoted-printable");
                                        break;
                                    case 2:
                                        curl_mime_encoder(part, "8bit");
                                        break;
                                }
                                break;
                            }
                        }
                        
                        // Set MIME part name
                        std::string part_name = "part_" + std::to_string(i);
                        curl_mime_name(part, part_name.c_str());
                        
                        // Set MIME filename
                        std::string part_filename = "file_" + std::to_string(i) + ".dat";
                        curl_mime_filename(part, part_filename.c_str());
                    }
                }
                
                // Set MIME data on easy handle
                curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
                
                // Clean up MIME handle
                curl_mime_free(mime);
            }
            
            // Clean up easy handle
            curl_easy_cleanup(easy_handle);
        }
    }
    
    // ============================================
    // TEST 3: COOKIE ENGINE FUNCTIONS
    // ============================================
    {
        CURL *easy_handle = curl_easy_init();
        if (easy_handle) {
            // Test cookie engine setup
            bool enable_cookies = fdp.ConsumeBool();
            if (enable_cookies) {
                // Set cookie file
                std::string cookie_file = fdp.ConsumeRandomLengthString(64);
                curl_easy_setopt(easy_handle, CURLOPT_COOKIEFILE, cookie_file.c_str());
                curl_easy_setopt(easy_handle, CURLOPT_COOKIEJAR, cookie_file.c_str());
                
                // Set cookie list
                std::string cookie_list = fdp.ConsumeRandomLengthString(128);
                curl_easy_setopt(easy_handle, CURLOPT_COOKIELIST, cookie_list.c_str());
                
                // Enable cookie engine
                curl_easy_setopt(easy_handle, CURLOPT_COOKIESESSION, 1L);
            }
            
            // Set basic URL
            std::string url = fdp.ConsumeRandomLengthString(128);
            curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
            curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
            
            // Clean up
            curl_easy_cleanup(easy_handle);
        }
    }
    
    // ============================================
    // TEST 4: PROTOCOL-SPECIFIC OPTIONS (FTP, SMTP)
    // ============================================
    {
        CURL *easy_handle = curl_easy_init();
        if (easy_handle) {
            // Consume protocol type
            uint8_t protocol_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            
            std::string url = fdp.ConsumeRandomLengthString(128);
            curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
            
            switch (protocol_type) {
                case 0: {
                    // FTP-specific options
                    bool use_ftp = fdp.ConsumeBool();
                    if (use_ftp) {
                        // FTP options
                        curl_easy_setopt(easy_handle, CURLOPT_FTPPORT, "-");
                        curl_easy_setopt(easy_handle, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);
                        curl_easy_setopt(easy_handle, CURLOPT_FTP_FILEMETHOD, CURLFTPMETHOD_MULTICWD);
                        
                        // FTP authentication
                        std::string ftp_user = fdp.ConsumeRandomLengthString(32);
                        std::string ftp_pass = fdp.ConsumeRandomLengthString(32);
                        if (!ftp_user.empty()) {
                            curl_easy_setopt(easy_handle, CURLOPT_USERNAME, ftp_user.c_str());
                        }
                        if (!ftp_pass.empty()) {
                            curl_easy_setopt(easy_handle, CURLOPT_PASSWORD, ftp_pass.c_str());
                        }
                    }
                    break;
                }
                case 1: {
                    // SMTP-specific options
                    bool use_smtp = fdp.ConsumeBool();
                    if (use_smtp) {
                        // SMTP options
                        std::string mail_from = fdp.ConsumeRandomLengthString(64);
                        std::string mail_rcpt = fdp.ConsumeRandomLengthString(64);
                        
                        if (!mail_from.empty()) {
                            curl_easy_setopt(easy_handle, CURLOPT_MAIL_FROM, mail_from.c_str());
                        }
                        if (!mail_rcpt.empty()) {
                            struct curl_slist *recipients = NULL;
                            recipients = curl_slist_append(recipients, mail_rcpt.c_str());
                            curl_easy_setopt(easy_handle, CURLOPT_MAIL_RCPT, recipients);
                            if (recipients) {
                                curl_slist_free_all(recipients);
                            }
                        }
                        
                        // SMTP authentication
                        curl_easy_setopt(easy_handle, CURLOPT_USE_SSL, CURLUSESSL_TRY);
                    }
                    break;
                }
                case 2: {
                    // Generic options
                    break;
                }
            }
            
            curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_cleanup(easy_handle);
        }
    }
    
    // ============================================
    // TEST 5: DEBUG AND PROGRESS CALLBACKS
    // ============================================
    {
        CURL *easy_handle = curl_easy_init();
        if (easy_handle) {
            // Set debug callback if enabled
            bool enable_debug = fdp.ConsumeBool();
            if (enable_debug) {
                // Debug function prototype would be needed
                // For now, just enable verbose mode
                curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 1L);
            }
            
            // Set progress callback
            bool enable_progress = fdp.ConsumeBool();
            if (enable_progress) {
                curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 0L);
                curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, progress_callback);
            }
            
            // Set header callback
            bool enable_headers = fdp.ConsumeBool();
            if (enable_headers) {
                curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, header_callback);
            }
            
            std::string url = fdp.ConsumeRandomLengthString(128);
            curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
            curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
            
            curl_easy_cleanup(easy_handle);
        }
    }
    
    // Clean up global state
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_005

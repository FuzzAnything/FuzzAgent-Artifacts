#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_022 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // FTP operations require significant input for comprehensive testing
    if (size < 64) {
        return 0; // Need minimum input for FTP operations
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Initialize curl globally
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Step 2: Create curl easy handle
    CURL* curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Step 3: Construct FTP URL from fuzzer input
    // Consume parts for FTP URL
    std::string ftp_host = fdp.ConsumeRandomLengthString(64);
    if (ftp_host.empty()) {
        ftp_host = "localhost";
    }
    
    // Consume port (0 means default port)
    uint16_t ftp_port = fdp.ConsumeIntegral<uint16_t>();
    std::string ftp_port_str = "";
    if (ftp_port > 0 && ftp_port != 21) {
        ftp_port_str = ":" + std::to_string(ftp_port);
    }
    
    // Consume username and password
    std::string ftp_user = fdp.ConsumeRandomLengthString(32);
    std::string ftp_pass = fdp.ConsumeRandomLengthString(32);
    std::string ftp_credentials = "";
    if (!ftp_user.empty()) {
        ftp_credentials = ftp_user;
        if (!ftp_pass.empty()) {
            ftp_credentials += ":" + ftp_pass;
        }
        ftp_credentials += "@";
    }
    
    // Consume file path - test FTP wildcard patterns
    std::string ftp_path = fdp.ConsumeRandomLengthString(128);
    if (ftp_path.empty()) {
        // Test FTP wildcard patterns: *, ?, [range], {pattern1,pattern2}
        std::string wildcards[] = {
            "/test*.txt",
            "/file[0-9].dat",
            "/{backup,archive}/*.zip",
            "/image?.jpg",
            "/log_*.gz"
        };
        size_t wildcard_idx = fdp.ConsumeIntegralInRange<size_t>(0, 4);
        ftp_path = wildcards[wildcard_idx];
    }
    
    // Build complete FTP URL with protocol selection
    std::string ftp_protocol = "ftp://";
    // Test FTPS (FTP over SSL/TLS)
    if (fdp.ConsumeBool()) {
        ftp_protocol = "ftps://";
    }
    
    std::string ftp_url = ftp_protocol + ftp_credentials + ftp_host + ftp_port_str + ftp_path;
    
    // Set FTP URL
    curl_easy_setopt(curl, CURLOPT_URL, ftp_url.c_str());

    // Step 4: Test FTP SSL/TLS specific options
    // CURLOPT_USE_SSL - Enable SSL/TLS for FTP
    if (fdp.remaining_bytes() > 1) {
        long use_ssl = fdp.ConsumeIntegralInRange<long>(0, 3); // CURLUSESSL_NONE to CURLUSESSL_ALL
        curl_easy_setopt(curl, CURLOPT_USE_SSL, use_ssl);
    }
    
    // CURLOPT_FTPSSLAUTH - FTP authentication method for SSL/TLS
    if (fdp.remaining_bytes() > 1) {
        long ftpssl_auth = fdp.ConsumeIntegralInRange<long>(0, 2); // CURLFTPAUTH_DEFAULT to CURLFTPAUTH_TLS
        curl_easy_setopt(curl, CURLOPT_FTPSSLAUTH, ftpssl_auth);
    }
    
    // CURLOPT_FTP_SSL_CCC - Clear Command Channel after authentication
    if (fdp.remaining_bytes() > 1) {
        long ssl_ccc = fdp.ConsumeIntegralInRange<long>(0, 2); // CURLFTPSSL_CCC_NONE to CURLFTPSSL_CCC_ACTIVE
        curl_easy_setopt(curl, CURLOPT_FTP_SSL_CCC, ssl_ccc);
    }
    
    // CURLOPT_FTP_ALTERNATIVE_TO_USER - Alternative command if USER/PASS fails
    if (fdp.remaining_bytes() > 4) {
        std::string alt_user = fdp.ConsumeRandomLengthString(50);
        curl_easy_setopt(curl, CURLOPT_FTP_ALTERNATIVE_TO_USER, alt_user.c_str());
    }
    
    // CURLOPT_PROXY_TRANSFER_MODE - transfer mode when doing FTP via HTTP proxy
    if (fdp.remaining_bytes() > 1) {
        long proxy_mode = fdp.ConsumeIntegral<long>() % 3; // 0, 1, or other values
        curl_easy_setopt(curl, CURLOPT_PROXY_TRANSFER_MODE, proxy_mode);
    }
    
    // CURLOPT_ACCEPTTIMEOUT_MS - Time-out accept operations (FTP only)
    if (fdp.remaining_bytes() > 4) {
        long accept_timeout = fdp.ConsumeIntegralInRange<long>(0, 10000); // 0-10 seconds in ms
        curl_easy_setopt(curl, CURLOPT_ACCEPTTIMEOUT_MS, accept_timeout);
    }
    
    // CURLOPT_FTP_RESPONSE_TIMEOUT (CURLOPT_SERVER_RESPONSE_TIMEOUT) - FTP response timeout
    if (fdp.remaining_bytes() > 4) {
        long response_timeout = fdp.ConsumeIntegralInRange<long>(0, 300); // 0-300 seconds
        curl_easy_setopt(curl, CURLOPT_SERVER_RESPONSE_TIMEOUT, response_timeout);
    }
    
    // Test FTP resume/restart functionality
    // CURLOPT_RESUME_FROM_LARGE - continuation offset for FTP transfers
    if (fdp.remaining_bytes() > 8) {
        curl_off_t resume_from = fdp.ConsumeIntegral<curl_off_t>();
        curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, resume_from);
    }
    
    // CURLOPT_MAXFILESIZE_LARGE - maximum size for FTP downloads
    if (fdp.remaining_bytes() > 8) {
        curl_off_t max_filesize = fdp.ConsumeIntegral<curl_off_t>();
        curl_easy_setopt(curl, CURLOPT_MAXFILESIZE_LARGE, max_filesize);
    }
    
    // Test FTP-specific protocol behaviors
    // CURLOPT_TRANSFERTEXT - use ASCII transfer for FTP
    if (fdp.remaining_bytes() > 1) {
        bool transfer_text = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_TRANSFERTEXT, transfer_text ? 1L : 0L);
    }
    
    // CURLOPT_PROTOCOLS - restrict to FTP/FTPS protocols only
    if (fdp.remaining_bytes() > 4) {
        long protocols = CURLPROTO_FTP | CURLPROTO_FTPS;
        if (fdp.ConsumeBool()) {
            protocols |= CURLPROTO_TFTP; // Include TFTP sometimes
        }
        curl_easy_setopt(curl, CURLOPT_PROTOCOLS, protocols);
    }
    
    // Test FTP directory listing options
    // CURLOPT_DIRLISTONLY - only list directory contents (FTP)
    if (fdp.remaining_bytes() > 1) {
        bool dirlist_only = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_DIRLISTONLY, dirlist_only ? 1L : 0L);
    }
    
    // Test FTP file upload options
    // CURLOPT_INFILESIZE_LARGE - size of file being uploaded (FTP)
    if (fdp.remaining_bytes() > 8) {
        curl_off_t infilesize = fdp.ConsumeIntegral<curl_off_t>();
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, infilesize);
    }
    
    // Test FTP connection options
    // CURLOPT_FTPPORT - FTP PORT string for active mode
    if (fdp.remaining_bytes() > 4) {
        std::string ftp_port_active = fdp.ConsumeRandomLengthString(50);
        curl_easy_setopt(curl, CURLOPT_FTPPORT, ftp_port_active.c_str());
    }
    
    // CURLOPT_FTP_USE_EPSV - use EPSV command
    if (fdp.remaining_bytes() > 1) {
        bool use_epsv = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, use_epsv ? 1L : 0L);
    }
    
    // CURLOPT_FTP_USE_EPRT - use EPRT command
    if (fdp.remaining_bytes() > 1) {
        bool use_eprt = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_FTP_USE_EPRT, use_eprt ? 1L : 0L);
    }
    
    // CURLOPT_FTP_USE_PRET - send PRET before PASV
    if (fdp.remaining_bytes() > 1) {
        bool use_pret = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_FTP_USE_PRET, use_pret ? 1L : 0L);
    }
    
    // Set common options for safe fuzzing
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L); // Disable signal handling
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10L); // Very short timeout for fuzzing
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 10L); // Short connect timeout
    
    // Don't actually perform the transfer to avoid network I/O
    // Just test option setting and validation
    
    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_022

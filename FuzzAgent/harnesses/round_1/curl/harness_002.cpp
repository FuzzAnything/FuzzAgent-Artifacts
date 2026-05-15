#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_002 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0; // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize curl globally
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Create an easy handle
    CURL* curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Consume data for FTP configuration
    // Generate FTP URL - use a dummy FTP URL to avoid actual network connections
    // We'll use ftp://localhost/ with some random path
    std::string url_base = "ftp://localhost/";
    std::string url_path = fdp.ConsumeRandomLengthString(100);
    std::string full_url = url_base + url_path;
    
    // Set FTP URL
    curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
    
    // Set FTP-specific options based on fuzzed input
    
    // CURLOPT_FTPPORT - FTP PORT string (interface name, IP address)
    if (fdp.remaining_bytes() > 4) {
        std::string ftp_port = fdp.ConsumeRandomLengthString(50);
        curl_easy_setopt(curl, CURLOPT_FTPPORT, ftp_port.c_str());
    }
    
    // CURLOPT_FTPLISTONLY (CURLOPT_DIRLISTONLY) - only list directory contents
    if (fdp.remaining_bytes() > 1) {
        bool list_only = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_DIRLISTONLY, list_only ? 1L : 0L);
    }
    
    // CURLOPT_FTP_USE_EPSV - use EPSV command for data connections
    if (fdp.remaining_bytes() > 1) {
        bool use_epsv = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, use_epsv ? 1L : 0L);
    }
    
    // CURLOPT_FTP_USE_EPRT - use EPRT command for data connections
    if (fdp.remaining_bytes() > 1) {
        bool use_eprt = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_FTP_USE_EPRT, use_eprt ? 1L : 0L);
    }
    
    // CURLOPT_FTP_CREATE_MISSING_DIRS - create missing directories on remote server
    if (fdp.remaining_bytes() > 1) {
        bool create_dirs = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, create_dirs ? CURLFTP_CREATE_DIR : CURLFTP_CREATE_DIR_NONE);
    }
    
    // CURLOPT_FTP_FILEMETHOD - method to reach files on FTP server
    if (fdp.remaining_bytes() > 1) {
        int ftp_method = fdp.ConsumeIntegralInRange<int>(0, 3);
        curl_easy_setopt(curl, CURLOPT_FTP_FILEMETHOD, ftp_method);
    }
    
    // CURLOPT_FTP_SKIP_PASV_IP - skip IP address in PASV response
    if (fdp.remaining_bytes() > 1) {
        bool skip_pasv_ip = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_FTP_SKIP_PASV_IP, skip_pasv_ip ? 1L : 0L);
    }
    
    // CURLOPT_FTP_ACCOUNT - account info for FTP server
    if (fdp.remaining_bytes() > 4) {
        std::string ftp_account = fdp.ConsumeRandomLengthString(50);
        curl_easy_setopt(curl, CURLOPT_FTP_ACCOUNT, ftp_account.c_str());
    }
    
    // Set other options that don't require network but affect FTP behavior
    bool verbose = fdp.ConsumeBool();
    curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose ? 1L : 0L);
    
    // Disable signal handling for fuzzing safety
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    
    // Set timeout to prevent hanging (very short for fuzzing)
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10L);
    
    // Set connection timeout (very short for fuzzing)
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 10L);
    
    // Target API: curl_easy_perform - execute the FTP transfer
    // This will likely fail since there's no real FTP server, but that's okay
    // We want to exercise the FTP protocol handling code
    CURLcode perform_result = curl_easy_perform(curl);
    
    // The result doesn't matter for fuzzing - we're interested in code coverage
    
    // Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}

} // namespace harness_002

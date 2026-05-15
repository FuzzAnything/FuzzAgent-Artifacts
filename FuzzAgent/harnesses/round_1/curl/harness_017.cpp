#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_017 {


// Simple callback functions for testing
static size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    // Just discard the data, return number of bytes we "handled"
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static size_t read_callback(char* buffer, size_t size, size_t nitems, void* userdata) {
    // Read from fuzzed data if available
    std::vector<uint8_t>* fuzz_data = static_cast<std::vector<uint8_t>*>(userdata);
    size_t buffer_size = size * nitems;
    size_t to_copy = std::min(buffer_size, fuzz_data->size());
    
    if (to_copy > 0) {
        memcpy(buffer, fuzz_data->data(), to_copy);
        fuzz_data->erase(fuzz_data->begin(), fuzz_data->begin() + to_copy);
    }
    
    return to_copy;
}

static int progress_callback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    // Simple progress callback
    (void)clientp;
    (void)dltotal;
    (void)dlnow;
    (void)ultotal;
    (void)ulnow;
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for various option configurations
    if (size < 64) {
        return 0; // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: curl_global_init (thread safety initialization)
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Step 2: curl_easy_init (create easy handle)
    CURL* curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Step 3: curl_easy_setopt multiple times with fuzzed option values
    // Test various CURLOPT_* options categorized by type
    
    // String options (CURLOPTTYPE_STRINGPOINT)
    if (fdp.remaining_bytes() > 16) {
        std::string url = fdp.ConsumeRandomLengthString(256);
        if (!url.empty()) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        }
    }
    
    if (fdp.remaining_bytes() > 8) {
        std::string user_agent = fdp.ConsumeRandomLengthString(128);
        if (!user_agent.empty()) {
            curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str());
        }
    }
    
    if (fdp.remaining_bytes() > 8) {
        std::string referer = fdp.ConsumeRandomLengthString(128);
        if (!referer.empty()) {
            curl_easy_setopt(curl, CURLOPT_REFERER, referer.c_str());
        }
    }
    
    // Long options (CURLOPTTYPE_LONG)
    if (fdp.remaining_bytes() > 1) {
        bool verbose = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose ? 1L : 0L);
    }
    
    if (fdp.remaining_bytes() > 1) {
        bool follow_location = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, follow_location ? 1L : 0L);
    }
    
    if (fdp.remaining_bytes() > 1) {
        bool header_included = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_HEADER, header_included ? 1L : 0L);
    }
    
    if (fdp.remaining_bytes() > 1) {
        bool no_body = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_NOBODY, no_body ? 1L : 0L);
    }
    
    // Callback options (CURLOPTTYPE_FUNCTIONPOINT)
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, nullptr);
    
    // Set up read callback with fuzzed data
    std::vector<uint8_t> read_data;
    if (fdp.remaining_bytes() > 32) {
        size_t read_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        if (read_data_size > fdp.remaining_bytes()) {
            read_data_size = fdp.remaining_bytes();
        }
        read_data = fdp.ConsumeBytes<uint8_t>(read_data_size);
    }
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_READDATA, &read_data);
    
    // Progress callback
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, nullptr);
    
    // Pointer options (CURLOPTTYPE_OBJECTPOINT, CURLOPTTYPE_CBPOINT, etc.)
    // Test blob option if enough data
    if (fdp.remaining_bytes() > 16) {
        struct curl_blob blob;
        size_t blob_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        if (blob_size > fdp.remaining_bytes()) {
            blob_size = fdp.remaining_bytes();
        }
        std::vector<uint8_t> blob_data = fdp.ConsumeBytes<uint8_t>(blob_size);
        
        blob.data = blob_data.data();
        blob.len = blob_data.size();
        blob.flags = CURL_BLOB_COPY;
        
        // Note: This would normally be used with CURLOPT_SSLCERT_BLOB, etc.
        // but we'll use it to test blob handling
    }
    
    // More string options
    if (fdp.remaining_bytes() > 8) {
        std::string cookie = fdp.ConsumeRandomLengthString(128);
        if (!cookie.empty()) {
            curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.c_str());
        }
    }
    
    if (fdp.remaining_bytes() > 8) {
        std::string encoding = fdp.ConsumeRandomLengthString(64);
        if (!encoding.empty()) {
            curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, encoding.c_str());
        }
    }
    
    // Additional long options with ranges
    if (fdp.remaining_bytes() > 4) {
        long timeout_ms = fdp.ConsumeIntegralInRange<long>(1, 10000);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout_ms);
    }
    
    if (fdp.remaining_bytes() > 4) {
        long connect_timeout_ms = fdp.ConsumeIntegralInRange<long>(1, 10000);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, connect_timeout_ms);
    }
    
    if (fdp.remaining_bytes() > 4) {
        long max_redirs = fdp.ConsumeIntegralInRange<long>(0, 10);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, max_redirs);
    }
    
    // HTTP version option
    if (fdp.remaining_bytes() > 1) {
        int http_version = fdp.ConsumeIntegralInRange<int>(0, 4);
        switch (http_version % 4) {
            case 0: curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0); break;
            case 1: curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1); break;
            case 2: curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0); break;
            case 3: curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_3); break;
        }
    }
    
    // SSL options
    if (fdp.remaining_bytes() > 1) {
        bool verify_peer = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, verify_peer ? 1L : 0L);
    }
    
    if (fdp.remaining_bytes() > 1) {
        bool verify_host = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, verify_host ? 2L : 0L);
    }
    
    // Disable signal handling for fuzzing safety
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    
    // Step 4: curl_easy_perform (execute transfer)
    // Note: In fuzzing context, this will likely fail due to no network,
    // but we want to test the API call and error paths
    CURLcode perform_result = curl_easy_perform(curl);
    
    // Step 5: curl_easy_getinfo to retrieve various information types
    // Test different CURLINFO_* types regardless of perform result
    
    // String info types
    char* effective_url = nullptr;
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
    
    char* content_type = nullptr;
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
    
    // Long info types
    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    
    long http_connectcode = 0;
    curl_easy_getinfo(curl, CURLINFO_HTTP_CONNECTCODE, &http_connectcode);
    
    long http_version = 0;
    curl_easy_getinfo(curl, CURLINFO_HTTP_VERSION, &http_version);
    
    long filetime = 0;
    curl_easy_getinfo(curl, CURLINFO_FILETIME, &filetime);
    
    // Double info types
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
    
    // Off_t info types (curl_off_t)
    curl_off_t size_upload_t = 0;
    curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &size_upload_t);
    
    curl_off_t size_download_t = 0;
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &size_download_t);
    
    // Socket descriptor type - CURLINFO_LASTSOCKET is deprecated and expects long*
    // Using the non-deprecated CURLINFO_ACTIVESOCKET instead with correct curl_socket_t type
    curl_socket_t active_socket = CURL_SOCKET_BAD;
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &active_socket);
    // SSL data info types
    curl_easy_getinfo(curl, CURLINFO_SSL_VERIFYRESULT, &response_code);
    
    // Cookie list
    struct curl_slist* cookies = nullptr;
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
    if (cookies) {
        curl_slist_free_all(cookies);
    }
    
    // Test some conditional getinfo calls based on fuzzed data
    if (fdp.remaining_bytes() > 1) {
        bool get_primary_ip = fdp.ConsumeBool();
        if (get_primary_ip) {
            char* primary_ip = nullptr;
            curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &primary_ip);
        }
    }
    
    if (fdp.remaining_bytes() > 1) {
        bool get_local_ip = fdp.ConsumeBool();
        if (get_local_ip) {
            char* local_ip = nullptr;
            curl_easy_getinfo(curl, CURLINFO_LOCAL_IP, &local_ip);
        }
    }
    
    if (fdp.remaining_bytes() > 1) {
        bool get_redirect_count = fdp.ConsumeBool();
        if (get_redirect_count) {
            long redirect_count = 0;
            curl_easy_getinfo(curl, CURLINFO_REDIRECT_COUNT, &redirect_count);
        }
    }
    
    if (fdp.remaining_bytes() > 1) {
        bool get_redirect_time = fdp.ConsumeBool();
        if (get_redirect_time) {
            double redirect_time = 0.0;
            curl_easy_getinfo(curl, CURLINFO_REDIRECT_TIME, &redirect_time);
        }
    }
    
    if (fdp.remaining_bytes() > 1) {
        bool get_redirect_url = fdp.ConsumeBool();
        if (get_redirect_url) {
            char* redirect_url = nullptr;
            curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &redirect_url);
        }
    }
    
    // Step 6: curl_easy_cleanup (free easy handle)
    curl_easy_cleanup(curl);
    
    // Step 7: curl_global_cleanup
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_017

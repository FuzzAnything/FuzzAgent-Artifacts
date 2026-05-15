#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_020 {
/*
 * Fuzzing harness for libcurl's SSL/TLS session import functionality
 * Targets: curl_easy_ssls_import (0% coverage, 3000+ undiscovered branches)
 * Focus on SSL/TLS module identified as most critical coverage gap.
 * Implementation: curl_easy_init -> SSL context configuration with share handle ->
 * curl_easy_ssls_import with various session data -> validation -> cleanup.
 * Semantic diversity: Unique harness focusing exclusively on SSL/TLS session import/export,
 * unlike existing harnesses which focus on HTTP, multi, URL, headers, or memory management.
 * This harness specifically targets the SSL session cache management APIs.
 */


// Write callback for handling response data
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    // Just discard the data for fuzzing purposes
    (void)contents;
    (void)userp;
    return realsize;
}

// Lock callback for share API (required for SSL session sharing)
static void lock_callback(CURL *handle, curl_lock_data data,
                         curl_lock_access access, void *userptr) {
    (void)handle;
    (void)data;
    (void)access;
    (void)userptr;
    // Minimal lock implementation for fuzzing
}

// Unlock callback for share API (required for SSL session sharing)
static void unlock_callback(CURL *handle, curl_lock_data data, void *userptr) {
    (void)handle;
    (void)data;
    (void)userptr;
    // Minimal unlock implementation for fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum input for meaningful SSL session import testing
    // We need: session_key (optional), shmac data, sdata, plus configuration options
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    CURLcode global_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_res != CURLE_OK) {
        return 0;
    }
    
    // Step 1: Create share handle for SSL session sharing
    CURLSH *share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set lock/unlock callbacks for the share handle
    curl_share_setopt(share, CURLSHOPT_LOCKFUNC, lock_callback);
    curl_share_setopt(share, CURLSHOPT_UNLOCKFUNC, unlock_callback);
    
    // Enable SSL session sharing - REQUIRED for curl_easy_ssls_import to work
    CURLSHcode share_res = curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
    (void)share_res; // Ignore result for fuzzing
    
    // Step 2: Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Configure easy handle to use the share
    curl_easy_setopt(easy_handle, CURLOPT_SHARE, share);
    
    // Set basic callbacks
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
    
    // Consume SSL/TLS configuration options from fuzzed input
    uint8_t ssl_config = fdp.ConsumeIntegral<uint8_t>();
    
    // Set SSL verification options based on fuzzed input
    if (ssl_config & 0x01) {
        bool verify_peer = fdp.ConsumeBool();
        curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, verify_peer ? 1L : 0L);
    }
    
    if (ssl_config & 0x02 && fdp.remaining_bytes() > 0) {
        bool verify_host = fdp.ConsumeBool();
        curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYHOST, verify_host ? 1L : 0L);
    }
    
    // Consume session import parameters from fuzzed input
    bool use_session_key = fdp.ConsumeBool();
    const char *session_key_ptr = nullptr;
    std::string session_key_str;
    
    if (use_session_key && fdp.remaining_bytes() > 1) {
        // Consume session key as a string
        session_key_str = fdp.ConsumeRandomLengthString(128);
        if (!session_key_str.empty()) {
            session_key_ptr = session_key_str.c_str();
        }
    }
    
    // Consume shmac (session HMAC) data
    size_t shmac_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::vector<unsigned char> shmac_data;
    if (shmac_len > 0 && fdp.remaining_bytes() >= shmac_len) {
        shmac_data = fdp.ConsumeBytes<unsigned char>(shmac_len);
    }
    
    // Consume sdata (session data) - this is the main session ticket data
    size_t sdata_len = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    std::vector<unsigned char> sdata_data;
    if (sdata_len > 0 && fdp.remaining_bytes() >= sdata_len) {
        sdata_data = fdp.ConsumeBytes<unsigned char>(sdata_len);
    }
    
    // Step 3: Main target - curl_easy_ssls_import
    // According to documentation: sdata and sdata_len must always be provided.
    // If session_key is NULL, then shmac and shmac_len must be given as received during export.
    CURLcode import_result = CURLE_OK;
    
    if (sdata_data.empty()) {
        // Need at least some session data
        curl_easy_cleanup(easy_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Test different import scenarios based on consumed data
    if (session_key_ptr != nullptr) {
        // Import with session key
        import_result = curl_easy_ssls_import(easy_handle,
                                             session_key_ptr,
                                             shmac_data.empty() ? nullptr : shmac_data.data(),
                                             shmac_data.size(),
                                             sdata_data.data(),
                                             sdata_data.size());
    } else {
        // Import without session key (requires shmac)
        import_result = curl_easy_ssls_import(easy_handle,
                                             nullptr,
                                             shmac_data.empty() ? nullptr : shmac_data.data(),
                                             shmac_data.size(),
                                             sdata_data.data(),
                                             sdata_data.size());
    }
    
    // The result may be CURLE_NOT_BUILT_IN if SSL or USE_SSLS_EXPORT not defined,
    // or other errors for invalid session data. That's OK for fuzzing.
    (void)import_result;
    
    // Step 4: Test complementary SSL session export (optional)
    // Export callback prototype
    auto export_callback = [](CURL *handle, void *userptr,
                              const char *session_key,
                              const unsigned char *shmac, size_t shmac_len,
                              const unsigned char *sdata, size_t sdata_len,
                              curl_off_t valid_until,
                              int ietf_tls_id,
                              const char *alpn,
                              size_t earlydata_max) -> CURLcode {
        (void)handle;
        (void)userptr;
        (void)session_key;
        (void)shmac;
        (void)shmac_len;
        (void)sdata;
        (void)sdata_len;
        (void)valid_until;
        (void)ietf_tls_id;
        (void)alpn;
        (void)earlydata_max;
        return CURLE_OK;
    };
    
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        CURLcode export_result = curl_easy_ssls_export(easy_handle, export_callback, nullptr);
        (void)export_result;
    }
    
    // Step 5: Test with URL setup (optional, to potentially trigger SSL handshake)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        std::string test_url = fdp.ConsumeRandomLengthString(256);
        if (!test_url.empty()) {
            curl_easy_setopt(easy_handle, CURLOPT_URL, test_url.c_str());
            
            // Try to perform (will fail without network, but tests SSL setup paths)
            CURLcode perform_result = curl_easy_perform(easy_handle);
            (void)perform_result;
        }
    }
    
    // Step 6: Cleanup in reverse order
    curl_easy_cleanup(easy_handle);
    curl_share_cleanup(share);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_020

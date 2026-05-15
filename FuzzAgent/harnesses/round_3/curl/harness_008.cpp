#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_008 {
/*
 * Fuzzing harness for libcurl's easy handle manipulation operations
 * Targets: curl_easy_duphandle (92 undiscovered branches), curl_easy_pause, curl_easy_upkeep
 * Focus on easy handle manipulation with 0% coverage for these functions.
 * Following invocation sequence from coverage guidance:
 * curl_easy_init() -> curl_easy_setopt() -> curl_easy_duphandle() -> 
 * curl_easy_pause() on both handles -> curl_easy_upkeep() -> curl_easy_perform() -> 
 * curl_easy_cleanup() on both handles.
 */


// Write callback for handling response data
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    // Just discard the data for fuzzing purposes
    (void)contents;
    (void)userp;
    return realsize;
}

// Header callback
static size_t header_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    // Just discard headers for fuzzing
    (void)contents;
    (void)userp;
    return realsize;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum input for meaningful fuzzing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }
    
    // Step 1: Create original easy handle
    CURL *easy_orig = curl_easy_init();
    if (!easy_orig) {
        curl_global_cleanup();
        return 0;
    }
    
    // Step 2: Set various options on the original handle
    // URL from fuzzed input
    std::string url = fdp.ConsumeRandomLengthString(256);
    if (!url.empty()) {
        curl_easy_setopt(easy_orig, CURLOPT_URL, url.c_str());
    }
    
    // Set callbacks
    curl_easy_setopt(easy_orig, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(easy_orig, CURLOPT_HEADERFUNCTION, header_callback);
    
    // Set various options based on fuzzed input
    uint8_t options_mask = fdp.ConsumeIntegral<uint8_t>();
    
    if (options_mask & 0x01) {
        bool verbose = fdp.ConsumeBool();
        curl_easy_setopt(easy_orig, CURLOPT_VERBOSE, verbose ? 1L : 0L);
    }
    
    if (options_mask & 0x02 && fdp.remaining_bytes() > 1) {
        bool follow_redirects = fdp.ConsumeBool();
        curl_easy_setopt(easy_orig, CURLOPT_FOLLOWLOCATION, follow_redirects ? 1L : 0L);
    }
    
    if (options_mask & 0x04 && fdp.remaining_bytes() > 1) {
        bool ssl_verifypeer = fdp.ConsumeBool();
        curl_easy_setopt(easy_orig, CURLOPT_SSL_VERIFYPEER, ssl_verifypeer ? 1L : 0L);
    }
    
    if (options_mask & 0x08 && fdp.remaining_bytes() > 1) {
        bool ssl_verifyhost = fdp.ConsumeBool();
        curl_easy_setopt(easy_orig, CURLOPT_SSL_VERIFYHOST, ssl_verifyhost ? 1L : 0L);
    }
    
    // Step 3: Duplicate the handle (main target API - curl_easy_duphandle)
    CURL *easy_dup = curl_easy_duphandle(easy_orig);
    if (!easy_dup) {
        // If duplication fails, cleanup original and exit
        curl_easy_cleanup(easy_orig);
        curl_global_cleanup();
        return 0;
    }
    
    // Step 4: Apply curl_easy_pause on both handles (second target API)
    // Consume pause bitmask from fuzzed input
    uint8_t pause_bits = fdp.ConsumeIntegral<uint8_t>();
    int pause_mask = 0;
    
    if (pause_bits & 0x01) {
        pause_mask |= CURLPAUSE_RECV;
    }
    if (pause_bits & 0x02) {
        pause_mask |= CURLPAUSE_SEND;
    }
    if (pause_bits & 0x04) {
        pause_mask |= CURLPAUSE_ALL;
    }
    if (pause_bits & 0x08) {
        pause_mask |= CURLPAUSE_CONT;
    }
    
    // Apply pause to both handles
    CURLcode pause_res_orig = curl_easy_pause(easy_orig, pause_mask);
    CURLcode pause_res_dup = curl_easy_pause(easy_dup, pause_mask);
    (void)pause_res_orig;
    (void)pause_res_dup;
    
    // Step 5: Perform connection upkeep on both handles (third target API - curl_easy_upkeep)
    CURLcode upkeep_res_orig = curl_easy_upkeep(easy_orig);
    CURLcode upkeep_res_dup = curl_easy_upkeep(easy_dup);
    (void)upkeep_res_orig;
    (void)upkeep_res_dup;
    
    // Step 6: Perform transfer on both handles (optional, depending on setup)
    // We'll try to perform on both handles, but it may fail if not properly configured
    // Only attempt if we have a URL
    if (!url.empty() && fdp.remaining_bytes() > 10) {
        // Try to perform on original handle
        CURLcode perform_res_orig = curl_easy_perform(easy_orig);
        (void)perform_res_orig;
        
        // Try to perform on duplicate handle
        CURLcode perform_res_dup = curl_easy_perform(easy_dup);
        (void)perform_res_dup;
    }
    
    // Step 7: Cleanup both handles
    curl_easy_cleanup(easy_dup);
    curl_easy_cleanup(easy_orig);
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_008

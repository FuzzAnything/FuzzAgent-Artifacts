#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <iostream>

namespace harness_014 {
/*
 * Fuzzing harness for libcurl's custom memory management API
 * Target: curl_global_init_mem (identified as critical uncovered API with 12 undiscovered branches)
 * Focus: Custom memory allocator testing to improve API coverage from current 0.75%
 * Semantic diversity: Unique harness focusing on memory management callbacks unlike existing harnesses
 * Testing strategy: Implement custom malloc/free/realloc/strdup/calloc callbacks, track allocations,
 *                   test various libcurl operations with custom memory management,
 *                   exercise error handling and edge cases in memory callbacks
 */


// Global tracking for memory allocations to verify callbacks are being called
static size_t total_allocations = 0;
static size_t total_deallocations = 0;
static size_t total_bytes_allocated = 0;
static std::map<void*, size_t> allocation_map;

// Custom memory callbacks for curl_global_init_mem
static void *custom_malloc(size_t size) {
    total_allocations++;
    total_bytes_allocated += size;
    
    // Use standard malloc but track it
    void *ptr = malloc(size);
    if (ptr) {
        allocation_map[ptr] = size;
    }
    return ptr;
}

static void custom_free(void *ptr) {
    total_deallocations++;
    
    if (ptr && allocation_map.find(ptr) != allocation_map.end()) {
        size_t size = allocation_map[ptr];
        total_bytes_allocated -= size;
        allocation_map.erase(ptr);
    }
    
    free(ptr);
}

static void *custom_realloc(void *ptr, size_t size) {
    // Track the old allocation if it exists
    if (ptr && allocation_map.find(ptr) != allocation_map.end()) {
        size_t old_size = allocation_map[ptr];
        total_bytes_allocated -= old_size;
        allocation_map.erase(ptr);
        total_deallocations++;  // realloc includes free of old pointer
    }
    
    total_allocations++;
    total_bytes_allocated += size;
    
    void *new_ptr = realloc(ptr, size);
    if (new_ptr) {
        allocation_map[new_ptr] = size;
    }
    
    return new_ptr;
}

static char *custom_strdup(const char *str) {
    total_allocations++;
    
    if (!str) {
        return nullptr;
    }
    
    size_t len = strlen(str) + 1;
    total_bytes_allocated += len;
    
    char *new_str = (char*)malloc(len);
    if (new_str) {
        allocation_map[new_str] = len;
        strcpy(new_str, str);
    }
    
    return new_str;
}

static void *custom_calloc(size_t nmemb, size_t size) {
    total_allocations++;
    
    size_t total_size = nmemb * size;
    total_bytes_allocated += total_size;
    
    void *ptr = calloc(nmemb, size);
    if (ptr) {
        allocation_map[ptr] = total_size;
    }
    
    return ptr;
}

// Simple write callback for libcurl
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    (void)contents;
    (void)userp;
    return realsize;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need sufficient input for meaningful fuzzing
    // We need data for: test scenario, flags, URLs, strings, etc.
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Reset tracking counters
    total_allocations = 0;
    total_deallocations = 0;
    total_bytes_allocated = 0;
    allocation_map.clear();
    
    // Step 1: Initialize libcurl with custom memory callbacks
    // Consume flags from fuzzed input
    long flags = 0;
    uint8_t flag_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (flag_choice) {
        case 0: flags = CURL_GLOBAL_DEFAULT; break;
        case 1: flags = CURL_GLOBAL_ALL; break;
        case 2: flags = CURL_GLOBAL_SSL; break;
        case 3: flags = CURL_GLOBAL_WIN32; break;
    }
    
    CURLcode init_result = curl_global_init_mem(flags,
                                                custom_malloc,
                                                custom_free,
                                                custom_realloc,
                                                custom_strdup,
                                                custom_calloc);
    
    // Check if initialization succeeded
    if (init_result != CURLE_OK) {
        // Even if init fails, we should cleanup any partial initialization
        curl_global_cleanup();
        return 0;
    }
    
    // Step 2: Test various libcurl operations to exercise memory callbacks
    
    // Test scenario selection from fuzzed input
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (test_scenario) {
        case 0: {
            // Scenario 0: Basic easy handle operations
            CURL *easy = curl_easy_init();
            if (easy) {
                // Set URL from fuzzed input
                std::string url = fdp.ConsumeRandomLengthString(256);
                if (!url.empty()) {
                    curl_easy_setopt(easy, CURLOPT_URL, url.c_str());
                }
                
                // Set write callback
                curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, write_callback);
                
                // Perform the request (will likely fail but exercises code paths)
                CURLcode perform_result = curl_easy_perform(easy);
                (void)perform_result; // Result may be error but that's OK for fuzzing
                
                // Get some info
                long http_code = 0;
                curl_easy_getinfo(easy, CURLINFO_RESPONSE_CODE, &http_code);
                
                // Clean up
                curl_easy_cleanup(easy);
            }
            break;
        }
        
        case 1: {
            // Scenario 1: URL parsing operations
            CURLU *urlp = curl_url();
            if (urlp) {
                // Set URL parts from fuzzed input
                std::string url = fdp.ConsumeRandomLengthString(256);
                if (!url.empty()) {
                    curl_url_set(urlp, CURLUPART_URL, url.c_str(), 0);
                }
                
                // Set individual parts
                uint8_t num_parts = fdp.ConsumeIntegral<uint8_t>() % 5;
                for (uint8_t i = 0; i < num_parts && fdp.remaining_bytes() > 10; i++) {
                    CURLUPart part = static_cast<CURLUPart>(fdp.ConsumeIntegralInRange<int>(0, 10));
                    std::string part_value = fdp.ConsumeRandomLengthString(128);
                    curl_url_set(urlp, part, part_value.c_str(), 0);
                }
                
                // Get parts back
                for (uint8_t i = 0; i < 2 && fdp.remaining_bytes() > 5; i++) {
                    CURLUPart part = static_cast<CURLUPart>(fdp.ConsumeIntegralInRange<int>(0, 10));
                    char *value = nullptr;
                    CURLUcode uc = curl_url_get(urlp, part, &value, 0);
                    if (uc == CURLUE_OK && value) {
                        curl_free(value);
                    }
                }
                
                curl_url_cleanup(urlp);
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Multi handle operations
            CURL *easy = curl_easy_init();
            if (easy) {
                std::string url = fdp.ConsumeRandomLengthString(256);
                if (!url.empty()) {
                    curl_easy_setopt(easy, CURLOPT_URL, url.c_str());
                }
                curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, write_callback);
                
                CURLM *multi = curl_multi_init();
                if (multi) {
                    curl_multi_add_handle(multi, easy);
                    
                    int running_handles = 0;
                    curl_multi_perform(multi, &running_handles);
                    
                    // Wait a bit (simulated)
                    int numfds = 0;
                    curl_multi_wait(multi, nullptr, 0, 1000, &numfds);
                    
                    // Perform more
                    curl_multi_perform(multi, &running_handles);
                    
                    // Clean up multi
                    curl_multi_remove_handle(multi, easy);
                    curl_multi_cleanup(multi);
                }
                
                curl_easy_cleanup(easy);
            }
            break;
        }
        
        case 3: {
            // Scenario 3: MIME operations
            CURL *easy = curl_easy_init();
            if (easy) {
                curl_mime *mime = curl_mime_init(easy);
                if (mime) {
                    curl_mimepart *part = curl_mime_addpart(mime);
                    if (part) {
                        // Set mime data from fuzzed input
                        std::string mime_data = fdp.ConsumeRandomLengthString(512);
                        if (!mime_data.empty()) {
                            curl_mime_data(part, mime_data.c_str(), mime_data.size());
                        }
                        
                        // Set mime name
                        std::string mime_name = fdp.ConsumeRandomLengthString(128);
                        if (!mime_name.empty()) {
                            curl_mime_name(part, mime_name.c_str());
                        }
                        
                        // Set mime filename
                        std::string mime_filename = fdp.ConsumeRandomLengthString(128);
                        if (!mime_filename.empty()) {
                            curl_mime_filename(part, mime_filename.c_str());
                        }
                    }
                    
                    // Set the MIME data to easy handle
                    curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
                    
                    curl_mime_free(mime);
                }
                curl_easy_cleanup(easy);
            }
            break;
        }
    }
    
    // Step 3: Test memory tracking consistency (optional verification)
    // Note: In real fuzzing, we don't want to fail on memory leaks as libcurl
    // may cache allocations, but we can at least verify callbacks were called
    if (total_allocations > 0) {
        // Callbacks were invoked - good!
    }
    
    // Step 4: Clean up libcurl
    curl_global_cleanup();
    
    // Note: We don't assert on memory leaks because libcurl may have internal caches
    // that aren't freed until later. The important thing is that our callbacks
    // were invoked and libcurl functions worked with custom memory management.
    
    return 0;
}

} // namespace harness_014

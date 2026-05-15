#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include "ares.h"

namespace harness_011 {


// Custom memory allocation functions for ares_library_init_mem
static void* custom_malloc(size_t size) {
    return malloc(size);
}

static void custom_free(void* ptr) {
    free(ptr);
}

static void* custom_realloc(void* ptr, size_t size) {
    return realloc(ptr, size);
}

// Fuzzer entry point targeting local file-based hostname resolution
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Initialize c-ares library with custom memory allocators
    int lib_init_result = ares_library_init_mem(ARES_LIB_INIT_ALL, 
                                                custom_malloc, 
                                                custom_free, 
                                                custom_realloc);
    // Note: We continue even if initialization fails, as we want to test
    // error paths as well
    
    // 2. Create a channel for testing
    ares_channel_t* channel = NULL;
    int channel_result = ares_init(&channel);
    if (channel_result != ARES_SUCCESS) {
        // If channel creation fails, cleanup and exit
        ares_library_cleanup();
        return 0;
    }
    
    // 3. Test ares_gethostbyname_file with various hostname cases
    // Consume data for hostname testing
    
    // Determine how many hostname tests to perform (1-10)
    size_t num_hostname_tests = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t i = 0; i < num_hostname_tests && fdp.remaining_bytes() > 4; i++) {
        // Consume hostname length and data
        size_t hostname_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
        std::string hostname;
        
        if (hostname_len > 0 && fdp.remaining_bytes() >= hostname_len) {
            hostname = fdp.ConsumeBytesAsString(hostname_len);
        }
        
        // Consume address family (AF_INET or AF_INET6)
        int family = fdp.ConsumeIntegralInRange<int>(AF_INET, AF_INET6);
        
        struct hostent* host_result = NULL;
        int result = 0;
        
        // Test different scenarios based on consumed data
        uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>();
        
        switch (test_scenario % 6) {
            case 0:
                // Test with valid hostname from fuzzer input
                if (!hostname.empty()) {
                    result = ares_gethostbyname_file(channel, hostname.c_str(), family, &host_result);
                    if (host_result) {
                        ares_free_hostent(host_result);
                        host_result = NULL;
                    }
                }
                break;
                
            case 1:
                // Test with NULL hostname pointer (should fail)
                result = ares_gethostbyname_file(channel, NULL, family, &host_result);
                break;
                
            case 2:
                // Test with .onion domain (should return ARES_ENOTFOUND per RFC 7686)
                if (!hostname.empty()) {
                    // Append .onion to test the special case
                    std::string onion_name = hostname + ".onion";
                    result = ares_gethostbyname_file(channel, onion_name.c_str(), family, &host_result);
                    if (host_result) {
                        ares_free_hostent(host_result);
                        host_result = NULL;
                    }
                }
                break;
                
            case 3:
                // Test with localhost (special handling per RFC6761)
                result = ares_gethostbyname_file(channel, "localhost", family, &host_result);
                if (host_result) {
                    ares_free_hostent(host_result);
                    host_result = NULL;
                }
                break;
                
            case 4:
                // Test with empty string
                result = ares_gethostbyname_file(channel, "", family, &host_result);
                if (host_result) {
                    ares_free_hostent(host_result);
                    host_result = NULL;
                }
                break;
                
            case 5:
                // Test with very long hostname
                if (fdp.remaining_bytes() > 512) {
                    size_t long_len = fdp.ConsumeIntegralInRange<size_t>(256, 512);
                    std::string long_hostname = fdp.ConsumeBytesAsString(long_len);
                    result = ares_gethostbyname_file(channel, long_hostname.c_str(), family, &host_result);
                    if (host_result) {
                        ares_free_hostent(host_result);
                        host_result = NULL;
                    }
                }
                break;
        }
    }
    
    // 4. Test ares_queue_wait_empty for thread synchronization
    // Consume timeout value from fuzzer input
    if (fdp.remaining_bytes() >= sizeof(int)) {
        int timeout_ms = fdp.ConsumeIntegral<int>();
        
        // Test with the consumed timeout
        ares_status_t queue_status = ares_queue_wait_empty(channel, timeout_ms);
        (void)queue_status; // Consume result
        
        // Also test with -1 (infinite timeout) and 0 (no wait)
        queue_status = ares_queue_wait_empty(channel, -1);
        (void)queue_status;
        
        queue_status = ares_queue_wait_empty(channel, 0);
        (void)queue_status;
    }
    
    // 5. Test with NULL channel (error case)
    if (fdp.ConsumeBool()) {
        struct hostent* dummy_host = NULL;
        int null_result = ares_gethostbyname_file(NULL, "test", AF_INET, &dummy_host);
        (void)null_result;
    }
    
    // 6. Test ares_queue_active_queries (related function)
    if (channel) {
        size_t active_queries = ares_queue_active_queries(channel);
        (void)active_queries;
    }
    
    // 7. Cleanup
    if (channel) {
        ares_destroy(channel);
    }
    
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_011

#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <climits>
#include "ares.h"

namespace harness_010 {

// Fuzzer entry point targeting ares_strerror function
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 4) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Initialize c-ares library
    int init_result = ares_library_init(ARES_LIB_INIT_ALL);
    if (init_result != ARES_SUCCESS) {
        // Even if initialization fails, we can still test ares_strerror
        // with fuzzed error codes
    }
    
    // 2. Create a channel to potentially get real error codes from operations
    ares_channel_t *channel = NULL;
    int channel_result = ARES_SUCCESS;
    
    // Try to create a channel - this may fail and give us error codes to test
    channel_result = ares_init(&channel);
    
    // 3. Test ares_strerror with various error codes
    
    // First, test all known ARES_* error code constants
    const int known_error_codes[] = {
        ARES_SUCCESS,           // 0
        ARES_ENODATA,           // 1
        ARES_EFORMERR,          // 2
        ARES_ESERVFAIL,         // 3
        ARES_ENOTFOUND,         // 4
        ARES_ENOTIMP,           // 5
        ARES_EREFUSED,          // 6
        ARES_EBADQUERY,         // 7
        ARES_EBADNAME,          // 8
        ARES_EBADFAMILY,        // 9
        ARES_EBADRESP,          // 10
        ARES_ECONNREFUSED,      // 11
        ARES_ETIMEOUT,          // 12
        ARES_EOF,               // 13
        ARES_EFILE,             // 14
        ARES_ENOMEM,            // 15
        ARES_EDESTRUCTION,      // 16
        ARES_EBADSTR,           // 17
        ARES_EBADFLAGS,         // 18
        ARES_ENONAME,           // 19
        ARES_EBADHINTS,         // 20
        ARES_ENOTINITIALIZED,   // 21
        ARES_ELOADIPHLPAPI,     // 22
        ARES_EADDRGETNETWORKPARAMS, // 23
        ARES_ECANCELLED,        // 24
        ARES_ESERVICE,          // 25
        ARES_ENOSERVER          // 26
    };
    
    // Test each known error code
    for (size_t i = 0; i < sizeof(known_error_codes)/sizeof(known_error_codes[0]); i++) {
        const char* error_str = ares_strerror(known_error_codes[i]);
        (void)error_str; // Consume result to prevent optimization
    }
    
    // 4. Test boundary values around the known error range
    // Test values just below and above the known range
    const int boundary_values[] = {
        -1,                     // Just below ARES_SUCCESS
        -100,                   // Far below
        -1000,                  // Very far below
        27,                     // Just above ARES_ENOSERVER (26)
        50,                     // Above known range
        100,                    // Far above
        1000,                   // Very far above
        INT_MIN,                // Minimum possible integer
        INT_MAX                 // Maximum possible integer
    };
    
    for (size_t i = 0; i < sizeof(boundary_values)/sizeof(boundary_values[0]); i++) {
        const char* error_str = ares_strerror(boundary_values[i]);
        (void)error_str;
    }
    
    // 5. Test random integers from fuzzer input for "unknown" case
    // Consume multiple random error codes from fuzzer input
    size_t num_random_tests = fdp.ConsumeIntegralInRange<size_t>(1, 20);
    for (size_t i = 0; i < num_random_tests && fdp.remaining_bytes() >= sizeof(int); i++) {
        int random_error = fdp.ConsumeIntegral<int>();
        const char* error_str = ares_strerror(random_error);
        (void)error_str;
    }
    

    
    // If we have a channel, try some operations that might fail
    if (channel != NULL) {
        // Try to set invalid servers to get error codes
        // Use fuzzer input to generate server string
        if (fdp.remaining_bytes() > 0) {
            size_t server_str_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            std::string server_str = fdp.ConsumeBytesAsString(server_str_len);
            int server_result = ares_set_servers_csv(channel, server_str.c_str());
            
            // Test the error code from the operation
            const char* server_error_str = ares_strerror(server_result);
            (void)server_error_str;
        }
        // Try other operations that might fail
        // Example: Try to parse invalid DNS data
        if (fdp.remaining_bytes() > 0) {
            size_t dns_data_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
            std::vector<uint8_t> dns_data = fdp.ConsumeBytes<uint8_t>(dns_data_size);
            
            // Note: We can't easily call parse functions without proper setup,
            // but we can use the error codes from channel creation or other operations
        }
    }
    
    // 7. Test with error codes from library initialization
    if (init_result != ARES_SUCCESS) {
        const char* init_error_str = ares_strerror(init_result);
        (void)init_error_str;
    }
    
    // 8. Test with error codes from channel creation
    if (channel_result != ARES_SUCCESS) {
        const char* channel_error_str = ares_strerror(channel_result);
        (void)channel_error_str;
    }
    
    // 9. Test with sequential values covering a range
    // This helps test the switch statement thoroughly
    int start_range = fdp.ConsumeIntegralInRange<int>(-50, 50);
    int end_range = start_range + fdp.ConsumeIntegralInRange<int>(0, 100);
    
    for (int error_code = start_range; error_code <= end_range; error_code++) {
        const char* error_str = ares_strerror(error_code);
        (void)error_str;
    }
    
    // 10. Clean up
    if (channel != NULL) {
        ares_destroy(channel);
    }
    
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_010

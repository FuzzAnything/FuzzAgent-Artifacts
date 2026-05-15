#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "curl/curl.h"

namespace harness_011 {
/*
 * Fuzzing harness for libcurl targeting curl_easy_pause and related transfer control functions
 * Harness ID: 011
 * Targets: curl_easy_pause, CURLPAUSE_* macros, and related state management APIs
 * Focus: Transfer pausing/resuming functionality, state transitions, edge cases in pause control
 * Differentiated from:
 * - harness_000.cpp (basic curl_url/curl_easy APIs)
 * - harness_001.cpp (MIME APIs)
 * - harness_002.cpp (curl_multi APIs)
 * - harness_003.cpp (FTP protocol)
 * - harness_004.cpp (Form APIs)
 * - harness_005.cpp (HTTP Headers Management)
 * - harness_006.cpp (Share APIs)
 * - harness_007.cpp (WebSocket APIs)
 * - harness_008.cpp (Advanced URL parsing)
 * - harness_009.cpp (Advanced Easy/Multi APIs)
 * - harness_010.cpp (Advanced Multi interfaces)
 * This harness specifically targets curl_easy_pause coverage gap identified as next highest-value
 * Focuses on: Complex pause/resume scenarios, state transitions, error conditions, bitmask combinations
 */



// Simple write callback that can return PAUSE based on fuzzed input
static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb;
    bool* should_pause = static_cast<bool*>(userdata);
    
    if (*should_pause) {
        *should_pause = false; // Only pause once per trigger
        return CURL_WRITEFUNC_PAUSE;
    }
    
    // Accept the data
    return realsize;
}

// Simple read callback that can return PAUSE based on fuzzed input
static size_t read_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
    size_t bufsize = size * nitems;
    bool* should_pause = static_cast<bool*>(userdata);
    
    if (*should_pause) {
        *should_pause = false; // Only pause once per trigger
        return CURL_READFUNC_PAUSE;
    }
    
    // Provide some data (zeros for simplicity)
    memset(buffer, 0, bufsize);
    return bufsize;
}

// Progress callback that can trigger pausing
static int progress_callback(void *clientp, 
                           curl_off_t dltotal, 
                           curl_off_t dlnow,
                           curl_off_t ultotal,
                           curl_off_t ulnow) {
    bool* should_pause = static_cast<bool*>(clientp);
    
    if (*should_pause) {
        *should_pause = false;
        // Return non-zero to pause the transfer
        return 1;
    }
    
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for pause operations
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // ==================== Easy Handle Creation ====================
    
    // Create easy handle for pause testing
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Consume URL for testing (use dummy URL since we're not establishing real connections)
    std::string url = fdp.ConsumeRandomLengthString(256);
    if (url.empty()) {
        url = "http://localhost/";
    }
    curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
    
    // Consume various configuration options from fuzzed data
    long connect_timeout = fdp.ConsumeIntegralInRange<long>(1, 10);
    long timeout = fdp.ConsumeIntegralInRange<long>(1, 30);
    
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, connect_timeout);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, timeout);
    
    // Consume flags for whether to test read/write/progress callbacks
    bool test_write_callback = fdp.ConsumeBool();
    bool test_read_callback = fdp.ConsumeBool();
    bool test_progress_callback = fdp.ConsumeBool();
    
    // State variables for callbacks
    bool write_should_pause = false;
    bool read_should_pause = false;
    bool progress_should_pause = false;
    
    if (test_write_callback) {
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &write_should_pause);
    }
    
    if (test_read_callback) {
        curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(easy_handle, CURLOPT_READDATA, &read_should_pause);
    }
    
    if (test_progress_callback) {
        curl_easy_setopt(easy_handle, CURLOPT_XFERINFOFUNCTION, progress_callback);
        curl_easy_setopt(easy_handle, CURLOPT_XFERINFODATA, &progress_should_pause);
        curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 0L);
    }
    
    // ==================== Multi Handle for Advanced Pause Testing ====================
    
    // Consume whether to use multi handle for more complex pause scenarios
    bool use_multi_handle = fdp.ConsumeBool();
    CURLM* multi_handle = nullptr;
    
    if (use_multi_handle) {
        multi_handle = curl_multi_init();
        if (multi_handle) {
            curl_multi_add_handle(multi_handle, easy_handle);
        }
    }
    
    // ==================== Pause/Resume Testing ====================
    
    // Determine number of pause operations to perform
    size_t num_pause_operations = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t i = 0; i < num_pause_operations && fdp.remaining_bytes() > 4; i++) {
        // Consume pause bitmask from fuzzed data
        int pause_bitmask = 0;
        
        // Build bitmask based on fuzzed input
        if (fdp.ConsumeBool()) pause_bitmask |= CURLPAUSE_RECV;
        if (fdp.ConsumeBool()) pause_bitmask |= CURLPAUSE_SEND;
        
        // For some operations, use predefined combinations
        int operation_type = fdp.ConsumeIntegralInRange<int>(0, 7);
        switch (operation_type) {
            case 0:
                // Use custom bitmask built above
                break;
            case 1:
                pause_bitmask = CURLPAUSE_ALL;  // Pause both recv and send
                break;
            case 2:
                pause_bitmask = CURLPAUSE_RECV;  // Pause receive only
                break;
            case 3:
                pause_bitmask = CURLPAUSE_SEND;  // Pause send only
                break;
            case 4:
                pause_bitmask = CURLPAUSE_CONT;  // Resume both
                break;
            case 5:
                pause_bitmask = CURLPAUSE_RECV_CONT;  // Resume receive
                break;
            case 6:
                pause_bitmask = CURLPAUSE_SEND_CONT;  // Resume send
                break;
            case 7:
                // Invalid combination test - mix pause and continue flags
                pause_bitmask = CURLPAUSE_RECV | CURLPAUSE_SEND_CONT;
                break;
        }
        
        // Consume whether to trigger callback pausing before curl_easy_pause
        if (test_write_callback && fdp.ConsumeBool()) {
            write_should_pause = true;
        }
        if (test_read_callback && fdp.ConsumeBool()) {
            read_should_pause = true;
        }
        if (test_progress_callback && fdp.ConsumeBool()) {
            progress_should_pause = true;
        }
        
        // Perform the pause operation
        CURLcode pause_result = curl_easy_pause(easy_handle, pause_bitmask);
        (void)pause_result; // All results are valid for fuzzing
        
        // If using multi handle, perform some multi operations
        if (use_multi_handle && multi_handle) {
            int still_running = 0;
            if (fdp.ConsumeBool()) {
                curl_multi_perform(multi_handle, &still_running);
            }
            
            // Test multi poll with timeout
            int numfds = 0;
            if (fdp.ConsumeBool()) {
                curl_multi_poll(multi_handle, NULL, 0, 100, &numfds);
            }
        }
        
        // Consume a small delay between operations (simulated with data consumption)
        if (fdp.remaining_bytes() > 1) {
            fdp.ConsumeIntegral<uint8_t>(); // Simulate time passing
        }
    }
    
    // ==================== Advanced Pause Scenarios ====================
    
    // Test complex state transitions
    if (fdp.remaining_bytes() > 8) {
        // Test sequence: pause recv -> pause send -> resume all
        curl_easy_pause(easy_handle, CURLPAUSE_RECV);
        if (fdp.ConsumeBool()) {
            curl_easy_pause(easy_handle, CURLPAUSE_SEND);
        }
        if (fdp.ConsumeBool()) {
            curl_easy_pause(easy_handle, CURLPAUSE_CONT);
        }
        
        // Test sequence: pause all -> resume recv only -> resume send only
        if (fdp.ConsumeBool()) {
            curl_easy_pause(easy_handle, CURLPAUSE_ALL);
            curl_easy_pause(easy_handle, CURLPAUSE_RECV_CONT);
            curl_easy_pause(easy_handle, CURLPAUSE_SEND_CONT);
        }
        
        // Test invalid/edge case bitmasks
        if (fdp.ConsumeBool()) {
            // Zero bitmask (should be a no-op)
            curl_easy_pause(easy_handle, 0);
        }
        if (fdp.ConsumeBool()) {
            // Bitmask with unused bits set
            curl_easy_pause(easy_handle, 0xFF00);
        }
    }
    
    // ==================== Cleanup ====================
    
    if (use_multi_handle && multi_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_multi_cleanup(multi_handle);
    }
    
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_011

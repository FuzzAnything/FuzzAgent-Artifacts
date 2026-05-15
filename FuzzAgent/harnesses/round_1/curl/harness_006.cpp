#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_006 {


// Write callback function that can be paused
static size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    // Simply discard the data, return number of bytes we "handled"
    return size * nmemb;
}

// Progress callback that can trigger pause/unpause
static int progress_callback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    // This callback can be used to call curl_easy_pause if needed
    // For now, just return 0 to continue
    return 0;
}

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

    // Set up a dummy URL - use localhost to avoid actual network connections
    // Consume a URL path from fuzzer input
    std::string url_path = fdp.ConsumeRandomLengthString(100);
    std::string full_url = "http://localhost/" + url_path;
    
    // Set the URL
    curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
    
    // Set write callback and data
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, nullptr);
    
    // Set progress callback for potential pause control
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
    
    // Set some basic options for testing
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L); // Short timeout
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 50L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);
    
    // Consume data to determine pause sequence strategy
    uint8_t pause_strategy = fdp.ConsumeIntegral<uint8_t>() % 8;
    uint8_t num_pause_ops = fdp.ConsumeIntegralInRange<uint8_t>(1, 5);
    
    // Test different pause sequences based on strategy
    for (uint8_t i = 0; i < num_pause_ops && fdp.remaining_bytes() > 1; i++) {
        // Consume pause flags
        uint8_t pause_flags = fdp.ConsumeIntegral<uint8_t>() % 16;
        
        // Map to valid pause flags
        int bitmask = 0;
        if (pause_flags & 0x01) bitmask |= CURLPAUSE_RECV;
        if (pause_flags & 0x02) bitmask |= CURLPAUSE_SEND;
        if (pause_flags & 0x04) bitmask |= CURLPAUSE_ALL;
        if (pause_flags & 0x08) bitmask |= CURLPAUSE_CONT;
        
        // Special case: if both RECV and SEND are set, use ALL
        if ((bitmask & CURLPAUSE_RECV) && (bitmask & CURLPAUSE_SEND)) {
            bitmask = CURLPAUSE_ALL;
        }
        
        // Special case: if CONT is set with others, prioritize CONT
        if (bitmask & CURLPAUSE_CONT) {
            bitmask = CURLPAUSE_CONT;
        }
        
        // Apply the pause
        CURLcode pause_result = curl_easy_pause(curl, bitmask);
        (void)pause_result; // Result may be CURLE_OK or error
        
        // Consume a boolean to decide whether to perform a transfer now
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 1) {
            // Start a transfer (non-blocking)
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
            CURLcode perform_result = curl_easy_perform(curl);
            (void)perform_result; // Result may be CURLE_OK or error
            
            // Possibly pause during transfer
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 1) {
                uint8_t inner_pause = fdp.ConsumeIntegral<uint8_t>() % 4;
                int inner_bitmask = 0;
                switch (inner_pause) {
                    case 0: inner_bitmask = CURLPAUSE_RECV; break;
                    case 1: inner_bitmask = CURLPAUSE_SEND; break;
                    case 2: inner_bitmask = CURLPAUSE_ALL; break;
                    case 3: inner_bitmask = CURLPAUSE_CONT; break;
                }
                curl_easy_pause(curl, inner_bitmask);
            }
        }
    }
    
    // Test a full transfer sequence with pauses
    // Strategy 0: Perform -> Pause RECV -> Resume -> Pause SEND -> Resume -> Cleanup
    // Strategy 1: Pause ALL -> Perform (should not transfer) -> Resume -> Perform
    // Strategy 2: Multiple rapid pause/unpause sequences
    // etc.
    
    switch (pause_strategy) {
        case 0: {
            // Simple sequence: Perform, pause RECV, resume, cleanup
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
            curl_easy_perform(curl); // May fail due to network, that's OK
            
            curl_easy_pause(curl, CURLPAUSE_RECV);
            curl_easy_pause(curl, CURLPAUSE_CONT);
            
            break;
        }
        case 1: {
            // Pause before performing
            curl_easy_pause(curl, CURLPAUSE_ALL);
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
            curl_easy_perform(curl); // Should not transfer while paused
            
            curl_easy_pause(curl, CURLPAUSE_CONT);
            curl_easy_perform(curl); // Now should try to transfer
            
            break;
        }
        case 2: {
            // Multiple direction-specific pauses
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
            
            curl_easy_pause(curl, CURLPAUSE_RECV);
            curl_easy_perform(curl);
            
            curl_easy_pause(curl, CURLPAUSE_CONT); // Resume RECV
            curl_easy_pause(curl, CURLPAUSE_SEND);
            curl_easy_perform(curl);
            
            curl_easy_pause(curl, CURLPAUSE_CONT); // Resume SEND
            
            break;
        }
        case 3: {
            // Rapid pause/unpause sequence
            for (int j = 0; j < 3; j++) {
                curl_easy_pause(curl, CURLPAUSE_RECV);
                curl_easy_pause(curl, CURLPAUSE_CONT);
                curl_easy_pause(curl, CURLPAUSE_SEND);
                curl_easy_pause(curl, CURLPAUSE_CONT);
            }
            break;
        }
        case 4: {
            // Test pause during active transfer simulation
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
            // Start transfer in separate "thread" simulation (just call perform)
            curl_easy_perform(curl);
            
            // Try to pause during transfer
            curl_easy_pause(curl, CURLPAUSE_ALL);
            curl_easy_pause(curl, CURLPAUSE_CONT);
            
            break;
        }
        case 5: {
            // Test combined flags
            curl_easy_pause(curl, CURLPAUSE_RECV | CURLPAUSE_SEND); // Should be same as ALL
            curl_easy_pause(curl, CURLPAUSE_CONT);
            
            // Test individual resume
            curl_easy_pause(curl, CURLPAUSE_RECV);
            curl_easy_pause(curl, CURLPAUSE_RECV_CONT); // Resume only RECV
            curl_easy_pause(curl, CURLPAUSE_SEND);
            curl_easy_pause(curl, CURLPAUSE_SEND_CONT); // Resume only SEND
            
            break;
        }
        case 6: {
            // Complex state machine testing
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
            
            // State 1: Pause RECV, try transfer
            curl_easy_pause(curl, CURLPAUSE_RECV);
            curl_easy_perform(curl);
            
            // State 2: Resume RECV, pause SEND
            curl_easy_pause(curl, CURLPAUSE_CONT);
            curl_easy_pause(curl, CURLPAUSE_SEND);
            curl_easy_perform(curl);
            
            // State 3: Resume everything
            curl_easy_pause(curl, CURLPAUSE_CONT);
            curl_easy_perform(curl);
            
            break;
        }
        case 7: {
            // Edge case: multiple pause calls with same flags
            curl_easy_pause(curl, CURLPAUSE_ALL);
            curl_easy_pause(curl, CURLPAUSE_ALL); // Same flag again
            curl_easy_pause(curl, CURLPAUSE_ALL); // And again
            
            curl_easy_pause(curl, CURLPAUSE_CONT);
            curl_easy_pause(curl, CURLPAUSE_CONT); // Resume multiple times
            
            break;
        }
    }
    
    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_006

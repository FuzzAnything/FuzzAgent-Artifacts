#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_003 {

// Simple write callback that discards data
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Just discard the received data
    return size * nmemb;
}

// Progress callback to test pause functionality
static int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    // Return 1 to pause the transfer (test pause functionality)
    // We'll use the clientp to determine whether to pause
    int* should_pause = (int*)clientp;
    if (should_pause && *should_pause) {
        return 1; // Return non-zero to pause
    }
    return 0; // Continue normally
}

// Header callback for mime testing
static size_t header_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Just discard the headers
    return size * nmemb;
}

// Notify callback for multi interface
static void notify_callback(CURLM *multi, unsigned int notification, CURL *easy, void *user_data) {
    // Just a placeholder callback for testing notify functionality
    (void)multi;
    (void)notification;
    (void)easy;
    (void)user_data;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum size for basic operations
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize curl library - must be called once
    CURLcode global_init_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_result != CURLE_OK) {
        return 0;
    }
    
    // 1. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // 2. Configure with curl_easy_setopt for various options
    std::string url = fdp.ConsumeRandomLengthString(256);
    bool follow_location = fdp.ConsumeBool();
    bool verbose = fdp.ConsumeBool();
    bool use_progress = fdp.ConsumeBool();
    int should_pause_flag = fdp.ConsumeBool() ? 1 : 0;
    
    // Set basic options
    curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, follow_location ? 1L : 0L);
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, verbose ? 1L : 0L);
    
    // Set write callback to discard data
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
    
    // Set progress callback if requested (for pause testing)
    if (use_progress) {
        curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, progress_callback);
        curl_easy_setopt(easy_handle, CURLOPT_PROGRESSDATA, &should_pause_flag);
    }
    
    // Set header callback
    curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, header_callback);
    
    // 3. Create mime handle and use curl_mime_headers for MIME operations
    // PRIMARY TARGET: curl_mime_headers (10 branches, 0% covered)
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Set some mime data
            std::string mime_data = fdp.ConsumeRandomLengthString(128);
            curl_mime_data(part, mime_data.c_str(), mime_data.size());
            
            // Create headers for mime part
            struct curl_slist *headers = NULL;
            std::string header1 = "Content-Type: " + fdp.ConsumeRandomLengthString(50);
            std::string header2 = "Content-Disposition: " + fdp.ConsumeRandomLengthString(50);
            
            headers = curl_slist_append(headers, header1.c_str());
            headers = curl_slist_append(headers, header2.c_str());
            
            // Call curl_mime_headers - PRIMARY TARGET
            curl_mime_headers(part, headers, 1); // take_ownership = 1
            
            // Set mime data to easy handle
            curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
        }
    }
    
    // 4. Perform transfer with curl_easy_perform
    // Start the transfer in a way that we can pause it
    CURLcode perform_result = curl_easy_perform(easy_handle);
    
    // 5. Test pause/resume functionality with curl_easy_pause during transfer
    // PRIMARY TARGET: curl_easy_pause (38 branches, 0% covered)
    if (perform_result == CURLE_OK || perform_result == CURLE_ABORTED_BY_CALLBACK) {
        // Test different pause bitmasks
        int pause_bitmask = 0;
        bool pause_recv = fdp.ConsumeBool();
        bool pause_send = fdp.ConsumeBool();
        
        if (pause_recv) pause_bitmask |= CURLPAUSE_RECV;
        if (pause_send) pause_bitmask |= CURLPAUSE_SEND;
        
        // Also test combined flags
        if (fdp.ConsumeBool()) pause_bitmask |= CURLPAUSE_ALL;
        if (fdp.ConsumeBool()) pause_bitmask |= CURLPAUSE_CONT;
        
        // Call curl_easy_pause with various bitmasks
        CURLcode pause_result = curl_easy_pause(easy_handle, pause_bitmask);
        (void)pause_result; // Result may vary depending on transfer state
    }
    
    // 6. Integrate with multi interface for advanced testing
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle) {
        // Add easy handle to multi handle
        CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle);
        
        if (add_result == CURLM_OK) {
            // PRIMARY TARGET: curl_multi_notify_enable (4 branches, 0% covered)
            // Enable notifications
            curl_multi_notify_enable(multi_handle, CURLMNOTIFY_INFO_READ);
            curl_multi_notify_enable(multi_handle, CURLMNOTIFY_EASY_DONE);
            
            // Set notify callback
            curl_multi_setopt(multi_handle, CURLMOPT_NOTIFYFUNCTION, notify_callback);
            
            // PRIMARY TARGET: curl_multi_wait (6 branches, 0% covered)
            // Wait for activity
            int numfds = 0;
            CURLMcode wait_result = curl_multi_wait(multi_handle, NULL, 0, 1000, &numfds);
            (void)wait_result;
            
            // PRIMARY TARGET: curl_multi_wakeup (6 branches, 50% covered, 3 undiscovered branches)
            // Try to wake up the multi handle
            CURLMcode wakeup_result = curl_multi_wakeup(multi_handle);
            (void)wakeup_result;
            
            // Perform some multi operations
            int running_handles = 0;
            CURLMcode perform_multi_result = curl_multi_perform(multi_handle, &running_handles);
            (void)perform_multi_result;
            
            // Test pause with multi interface
            int multi_pause_bitmask = 0;
            if (fdp.ConsumeBool()) multi_pause_bitmask |= CURLPAUSE_RECV;
            if (fdp.ConsumeBool()) multi_pause_bitmask |= CURLPAUSE_SEND;
            
            CURLcode multi_pause_result = curl_easy_pause(easy_handle, multi_pause_bitmask);
            (void)multi_pause_result;
            
            // Remove easy handle from multi
            curl_multi_remove_handle(multi_handle, easy_handle);
        }
        
        // Clean up multi handle
        curl_multi_cleanup(multi_handle);
    }
    
    // 7. Test pause/resume again after multi operations
    int final_pause_bitmask = fdp.ConsumeIntegralInRange<int>(0, 15);
    CURLcode final_pause_result = curl_easy_pause(easy_handle, final_pause_bitmask);
    (void)final_pause_result;
    
    // 8. Clean up mime handle if created
    if (mime) {
        curl_mime_free(mime);
    }
    
    // 9. Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // 10. Clean up global curl
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_003

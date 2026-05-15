#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace harness_021 {
/*
 * Fuzzing harness for libcurl's curl_multi_waitfds API 
 * Targets: curl_multi_waitfds API identified by CoverageAnalyzerAgent as the most critical 
 * uncovered API with 26 branches and 28 undiscovered accumulated branches, and extremely 
 * low API coverage (0.87%).
 * Focus on multi interface wait operations: curl_multi_init -> curl_multi_add_handle 
 * curl_multi_waitfds with various descriptor configurations -> curl_multi_perform -> cleanup.
 * Semantic diversity: This harness specifically targets curl_multi_waitfds which is NOT 
 * used in existing harness_001.cpp (which uses curl_multi_wait, curl_multi_poll but NOT waitfds).
 * We create unique harness focusing on descriptor polling API for advanced socket management.
 */

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    // Just discard the data for fuzzing purposes
    (void)contents;
    (void)userp;
    return realsize;
}

// Create dummy socket pairs for testing curl_multi_waitfds
static int create_dummy_socket_pair(int fd[2]) {
    // Create a socket pair for testing
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) == 0) {
        return 0;
    }
    return -1;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum input for meaningful fuzzing of curl_multi_waitfds
    // We need to consume: number of handles, URLs, waitfd configurations
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    CURLcode global_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_res != CURLE_OK) {
        return 0;
    }
    
    // Step 1: Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_global_cleanup();
        return 0;
    }
    
    // Configure multi handle options based on fuzzed input
    uint8_t multi_config = fdp.ConsumeIntegral<uint8_t>();
    
    if (multi_config & 0x01) {
        long pipelining = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (pipelining) {
            case 0:
                curl_multi_setopt(multi, CURLMOPT_PIPELINING, CURLPIPE_NOTHING);
                break;
            case 1:
                curl_multi_setopt(multi, CURLMOPT_PIPELINING, CURLPIPE_HTTP1);
                break;
            case 2:
                curl_multi_setopt(multi, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
                break;
        }
    }
    
    if (multi_config & 0x02 && fdp.remaining_bytes() > 4) {
        long max_connects = fdp.ConsumeIntegralInRange<long>(1, 10);
        curl_multi_setopt(multi, CURLMOPT_MAX_TOTAL_CONNECTIONS, max_connects);
    }
    
    // Step 2: Create easy handles and add them to multi
    int num_easy_handles = fdp.ConsumeIntegralInRange<int>(1, 3); // Keep small for fuzzing
    std::vector<CURL*> easy_handles;
    
    for (int i = 0; i < num_easy_handles && fdp.remaining_bytes() > 20; i++) {
        CURL *easy = curl_easy_init();
        if (!easy) {
            continue;
        }
        
        // Consume URL from fuzzed input
        std::string url = fdp.ConsumeRandomLengthString(128);
        if (!url.empty()) {
            curl_easy_setopt(easy, CURLOPT_URL, url.c_str());
        }
        
        // Set write callback
        curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, write_callback);
        
        // Set some basic options based on fuzzed input
        if (fdp.remaining_bytes() > 2) {
            bool verbose = fdp.ConsumeBool();
            curl_easy_setopt(easy, CURLOPT_VERBOSE, verbose ? 1L : 0L);
        }
        
        if (fdp.remaining_bytes() > 2) {
            bool follow_redirects = fdp.ConsumeBool();
            curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, follow_redirects ? 1L : 0L);
        }
        
        // Add handle to multi
        CURLMcode add_res = curl_multi_add_handle(multi, easy);
        if (add_res == CURLM_OK) {
            easy_handles.push_back(easy);
        } else {
            curl_easy_cleanup(easy);
        }
    }
    
    if (easy_handles.empty()) {
        curl_multi_cleanup(multi);
        curl_global_cleanup();
        return 0;
    }
    
    // Step 3: PRIMARY TARGET - Test curl_multi_waitfds with various configurations
    // This is the main focus of this harness as identified by CoverageAnalyzerAgent
    
    // First, call curl_multi_waitfds with NULL ufds to get fd count
    unsigned int fd_count = 0;
    CURLMcode waitfds_res1 = curl_multi_waitfds(multi, NULL, 0, &fd_count);
    (void)waitfds_res1; // Result can vary, ignore for fuzzing
    
    // Now test with actual waitfd structures if we have enough input
    if (fdp.remaining_bytes() > 32) {
        // Determine how many waitfd structures to create (0-5 for fuzzing efficiency)
        unsigned int num_waitfds = fdp.ConsumeIntegralInRange<unsigned int>(0, 5);
        
        if (num_waitfds > 0) {
            // Allocate waitfd array
            struct curl_waitfd *waitfds = (struct curl_waitfd*)malloc(
                num_waitfds * sizeof(struct curl_waitfd));
            
            if (waitfds) {
                // Initialize waitfd structures with fuzzed data
                for (unsigned int i = 0; i < num_waitfds && fdp.remaining_bytes() > 4; i++) {
                    // Consume events configuration
                    short events = 0;
                    uint8_t event_config = fdp.ConsumeIntegral<uint8_t>();
                    
                    if (event_config & 0x01) {
                        events |= CURL_WAIT_POLLIN;
                    }
                    if (event_config & 0x02) {
                        events |= CURL_WAIT_POLLOUT;
                    }
                    if (event_config & 0x04) {
                        events |= CURL_WAIT_POLLPRI;
                    }
                    
                    waitfds[i].fd = CURL_SOCKET_BAD; // Default to invalid socket
                    waitfds[i].events = events;
                    waitfds[i].revents = 0; // Will be filled by curl_multi_waitfds
                    
                    // Sometimes create a dummy socket for testing
                    if ((event_config & 0x08) && fdp.remaining_bytes() > 0) {
                        int sockpair[2];
                        if (create_dummy_socket_pair(sockpair) == 0) {
                            waitfds[i].fd = sockpair[0];
                            // Close the other end to avoid leaks
                            close(sockpair[1]);
                        }
                    }
                }
                
                // Call curl_multi_waitfds with the waitfd array
                unsigned int actual_fd_count = 0;
                CURLMcode waitfds_res2 = curl_multi_waitfds(multi, waitfds, num_waitfds, &actual_fd_count);
                (void)waitfds_res2; // Result can vary
                
                // Clean up any sockets we created
                for (unsigned int i = 0; i < num_waitfds; i++) {
                    if (waitfds[i].fd != CURL_SOCKET_BAD && waitfds[i].fd != -1) {
                        close(waitfds[i].fd);
                    }
                }
                
                free(waitfds);
            }
        }
    }
    
    // Step 4: Test curl_multi_waitfds with single waitfd (common use case)
    if (fdp.remaining_bytes() > 16) {
        // Create a single waitfd structure
        struct curl_waitfd single_waitfd;
        single_waitfd.fd = CURL_SOCKET_BAD;
        single_waitfd.events = CURL_WAIT_POLLIN | CURL_WAIT_POLLOUT;
        single_waitfd.revents = 0;
        
        unsigned int single_fd_count = 0;
        CURLMcode waitfds_res4 = curl_multi_waitfds(multi, &single_waitfd, 1, &single_fd_count);
        (void)waitfds_res4;
    }
    
    // Step 5: Test edge cases for curl_multi_waitfds
    
    // Test with size=0 but ufds!=NULL (should work according to docs)
    if (fdp.remaining_bytes() > 8) {
        struct curl_waitfd dummy_waitfd;
        dummy_waitfd.fd = CURL_SOCKET_BAD;
        dummy_waitfd.events = CURL_WAIT_POLLIN;
        dummy_waitfd.revents = 0;
        
        unsigned int edge_fd_count = 0;
        CURLMcode waitfds_res5 = curl_multi_waitfds(multi, &dummy_waitfd, 0, &edge_fd_count);
        (void)waitfds_res5;
    }
    
    // Test with fd_count=NULL (should work according to function signature)
    if (fdp.remaining_bytes() > 4) {
        CURLMcode waitfds_res6 = curl_multi_waitfds(multi, NULL, 0, NULL);
        (void)waitfds_res6;
    }
    
    // Step 6: Cleanup process
    // Remove and cleanup easy handles
    for (CURL *easy : easy_handles) {
        curl_multi_remove_handle(multi, easy);
        curl_easy_cleanup(easy);
    }
    
    // Test curl_multi_waitfds after cleanup (empty multi handle)
    unsigned int final_fd_count = 0;
    CURLMcode waitfds_res7 = curl_multi_waitfds(multi, NULL, 0, &final_fd_count);
    (void)waitfds_res7;
    
    // Cleanup multi handle
    curl_multi_cleanup(multi);
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_021

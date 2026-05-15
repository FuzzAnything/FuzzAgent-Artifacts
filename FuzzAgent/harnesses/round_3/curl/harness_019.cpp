#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_019 {
/*
 * Fuzzing harness for libcurl's curl_easy_recv API (0% coverage, 12 undiscovered branches)
 * Targets: curl_easy_recv, curl_easy_send, with proper CONNECT_ONLY mode setup
 * Focus on data transfer operations including partial reads, blocking/non-blocking modes,
 * and error conditions for raw socket communication.
 * Following dependency chain: curl_easy_init -> curl_easy_setopt -> curl_easy_perform -> 
 * curl_easy_recv/send/pause -> curl_easy_cleanup.
 * Semantic diversity: Unique harness focusing on raw socket data transfer operations,
 * unlike existing harnesses which focus on HTTP, WebSockets, or other protocols.
 * This harness specifically targets the 12 undiscovered branches in curl_easy_recv.
 */


// Simple socket wait function for simulating network I/O conditions
static int simulate_wait(int for_recv, long timeout_ms) {
    (void)for_recv;
    (void)timeout_ms;
    // For fuzzing, we simulate immediate availability
    // In real usage, this would use select() or poll() on the socket
    return 1; // Always ready for fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum input for meaningful data transfer testing
    // We need: URL, operation type, buffer sizes, flags, and data
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize libcurl globally
    CURLcode global_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_res != CURLE_OK) {
        return 0;
    }
    
    // Step 2: Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Step 3: Consume URL from fuzzed input
    std::string url = fdp.ConsumeRandomLengthString(256);
    if (url.empty()) {
        // Provide default URL for testing (will fail without network, but that's OK)
        url = "http://example.com";
    }
    curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
    
    // Step 4: Set CONNECT_ONLY mode for raw socket operations
    curl_easy_setopt(easy_handle, CURLOPT_CONNECT_ONLY, 1L);
    
    // Step 5: Set other options based on fuzzed input
    // Timeout values
    long timeout_secs = fdp.ConsumeIntegralInRange<long>(1, 30);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, timeout_secs);
    
    long connect_timeout_secs = fdp.ConsumeIntegralInRange<long>(1, 10);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, connect_timeout_secs);
    
    // TCP no delay option (Nagle's algorithm)
    if (fdp.ConsumeBool()) {
        curl_easy_setopt(easy_handle, CURLOPT_TCP_NODELAY, 1L);
    }
    
    // TCP keepalive options
    if (fdp.ConsumeBool()) {
        curl_easy_setopt(easy_handle, CURLOPT_TCP_KEEPALIVE, 1L);
        long keepidle = fdp.ConsumeIntegralInRange<long>(1, 60);
        curl_easy_setopt(easy_handle, CURLOPT_TCP_KEEPIDLE, keepidle);
        long keepintvl = fdp.ConsumeIntegralInRange<long>(1, 10);
        curl_easy_setopt(easy_handle, CURLOPT_TCP_KEEPINTVL, keepintvl);
    }
    
    // Step 6: Try to perform the connection
    // Note: This will likely fail without network, but we still want to test the APIs
    CURLcode perform_res = curl_easy_perform(easy_handle);
    
    // Step 7: Determine operation type from fuzzed input
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Step 8: Get socket information (if connection was successful)
    curl_socket_t sockfd = CURL_SOCKET_BAD;
    CURLcode info_res = CURLE_OK;
    
    if (perform_res == CURLE_OK) {
        info_res = curl_easy_getinfo(easy_handle, CURLINFO_ACTIVESOCKET, &sockfd);
    }
    
    // Step 9: Consume buffer sizes and data for send/recv operations
    size_t send_buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    size_t recv_buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    
    std::vector<uint8_t> send_buffer;
    std::vector<uint8_t> recv_buffer(recv_buffer_size);
    
    if (fdp.remaining_bytes() >= send_buffer_size) {
        send_buffer = fdp.ConsumeBytes<uint8_t>(send_buffer_size);
    } else {
        // Use remaining bytes if not enough
        send_buffer = fdp.ConsumeRemainingBytes<uint8_t>();
        send_buffer_size = send_buffer.size();
    }
    
    // Step 10: Execute different data transfer scenarios based on operation_type
    switch (operation_type) {
        case 0: {
            // Test curl_easy_send() - basic send operation
            if (send_buffer_size > 0) {
                size_t sent = 0;
                CURLcode send_res = curl_easy_send(easy_handle, 
                                                   send_buffer.data(), 
                                                   send_buffer_size, 
                                                   &sent);
                
                // Handle CURLE_AGAIN with simulated wait
                if (send_res == CURLE_AGAIN && sockfd != CURL_SOCKET_BAD) {
                    int wait_ready = simulate_wait(0, 1000);
                    if (wait_ready) {
                        // Try sending again
                        send_res = curl_easy_send(easy_handle, 
                                                 send_buffer.data() + sent, 
                                                 send_buffer_size - sent, 
                                                 &sent);
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Test curl_easy_recv() - basic receive operation
            if (recv_buffer_size > 0) {
                size_t received = 0;
                CURLcode recv_res = curl_easy_recv(easy_handle, 
                                                   recv_buffer.data(), 
                                                   recv_buffer_size, 
                                                   &received);
                
                // Handle CURLE_AGAIN with simulated wait
                if (recv_res == CURLE_AGAIN && sockfd != CURL_SOCKET_BAD) {
                    int wait_ready = simulate_wait(1, 1000);
                    if (wait_ready) {
                        // Try receiving again
                        recv_res = curl_easy_recv(easy_handle, 
                                                 recv_buffer.data() + received, 
                                                 recv_buffer_size - received, 
                                                 &received);
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Test combined send and receive
            if (send_buffer_size > 0) {
                size_t sent = 0;
                CURLcode send_res = curl_easy_send(easy_handle, 
                                                   send_buffer.data(), 
                                                   send_buffer_size, 
                                                   &sent);
                (void)send_res; // Result may be error
            }
            
            if (recv_buffer_size > 0) {
                size_t received = 0;
                CURLcode recv_res = curl_easy_recv(easy_handle, 
                                                   recv_buffer.data(), 
                                                   recv_buffer_size, 
                                                   &received);
                (void)recv_res; // Result may be error
            }
            break;
        }
        
        case 3: {
            // Test partial send with pause
            if (send_buffer_size > 0) {
                // Send first half
                size_t first_half = send_buffer_size / 2;
                if (first_half > 0) {
                    size_t sent = 0;
                    CURLcode send_res = curl_easy_send(easy_handle, 
                                                       send_buffer.data(), 
                                                       first_half, 
                                                       &sent);
                    (void)send_res;
                    
                    // Try to send second half
                    if (sent < first_half && (send_buffer_size - sent) > 0) {
                        size_t remaining_sent = 0;
                        send_res = curl_easy_send(easy_handle, 
                                                 send_buffer.data() + sent, 
                                                 send_buffer_size - sent, 
                                                 &remaining_sent);
                    }
                }
            }
            break;
        }
        
        case 4: {
            // Test multiple small sends
            if (send_buffer_size > 0) {
                size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                size_t total_sent = 0;
                
                while (total_sent < send_buffer_size && chunk_size > 0) {
                    size_t to_send = (send_buffer_size - total_sent) < chunk_size ? 
                                    (send_buffer_size - total_sent) : chunk_size;
                    size_t sent = 0;
                    CURLcode send_res = curl_easy_send(easy_handle, 
                                                       send_buffer.data() + total_sent, 
                                                       to_send, 
                                                       &sent);
                    (void)send_res;
                    
                    if (sent == 0) break; // No progress
                    total_sent += sent;
                }
            }
            break;
        }
        
        case 5: {
            // Test receive with zero buffer (edge case)
            size_t received = 0;
            CURLcode recv_res = curl_easy_recv(easy_handle, NULL, 0, &received);
            (void)recv_res;
            
            // Then try actual receive
            if (recv_buffer_size > 0) {
                recv_res = curl_easy_recv(easy_handle, 
                                         recv_buffer.data(), 
                                         recv_buffer_size, 
                                         &received);
            }
            break;
        }
        
        case 6: {
            // Test send with zero buffer (edge case)
            size_t sent = 0;
            CURLcode send_res = curl_easy_send(easy_handle, NULL, 0, &sent);
            (void)send_res;
            
            // Then try actual send
            if (send_buffer_size > 0) {
                send_res = curl_easy_send(easy_handle, 
                                         send_buffer.data(), 
                                         send_buffer_size, 
                                         &sent);
            }
            break;
        }
        
        case 7: {
            // Test error handling scenarios
            // Try operations on invalid handle
            CURL *invalid_handle = NULL;
            size_t dummy = 0;
            CURLcode invalid_send = curl_easy_send(invalid_handle, NULL, 0, &dummy);
            CURLcode invalid_recv = curl_easy_recv(invalid_handle, NULL, 0, &dummy);
            (void)invalid_send;
            (void)invalid_recv;
            
            // Try operations with valid handle but invalid parameters
            if (send_buffer_size > 0) {
                size_t sent = 0;
                CURLcode send_res = curl_easy_send(easy_handle, 
                                                   send_buffer.data(), 
                                                   send_buffer_size, 
                                                   &sent); // Fixed: use valid pointer instead of NULL
                (void)send_res;
            }
            
            if (recv_buffer_size > 0) {
                size_t received = 0;
                CURLcode recv_res = curl_easy_recv(easy_handle, 
                                                   recv_buffer.data(), 
                                                   recv_buffer_size, 
                                                   &received); // Fixed: use valid pointer instead of NULL
                (void)recv_res;
            }
            break;
        }
    }
    
    // Step 11: Test curl_easy_pause() API (related to data transfer control)
    // Note: curl_easy_pause() affects how curl_easy_recv/send behave
    int pause_flags = 0;
    if (fdp.ConsumeBool()) pause_flags |= CURLPAUSE_RECV;
    if (fdp.ConsumeBool()) pause_flags |= CURLPAUSE_SEND;
    if (fdp.ConsumeBool()) pause_flags |= CURLPAUSE_ALL;
    if (fdp.ConsumeBool()) pause_flags |= CURLPAUSE_CONT;
    
    if (pause_flags != 0) {
        CURLcode pause_res = curl_easy_pause(easy_handle, pause_flags);
        (void)pause_res;
    }
    
    // Step 12: Test curl_easy_upkeep() for connection maintenance
    CURLcode upkeep_res = curl_easy_upkeep(easy_handle);
    (void)upkeep_res;
    
    // Step 13: Cleanup
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_019

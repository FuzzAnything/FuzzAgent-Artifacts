#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include "ares.h"
#include "ares_dns.h"
#include "ares_nameser.h"

namespace harness_001 {


// Callback function for async DNS queries
static void query_callback(void *arg, int status, int timeouts, 
                          const unsigned char *abuf, int alen) {
    // Do nothing - just consume the callback
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

// Callback function for host queries
static void host_callback(void *arg, int status, int timeouts, 
                         const struct hostent *hostent) {
    // Do nothing - just consume the callback
    // Note: c-ares automatically frees the hostent structure after callback returns
    // so we should NOT call ares_free_hostent() here
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
}

// Fuzzer entry point for async event processing
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Global library initialization
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // 2. Channel initialization
    ares_channel_t *channel = NULL;
    int init_result = ares_init(&channel);
    if (init_result != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // 3. Consume data for various API parameters
    
    // Determine number of queries to create (1-5)
    size_t num_queries = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    
    // Store query IDs for potential cancellation
    std::vector<unsigned short> query_ids;
    
    // 4. Create and send DNS queries
    for (size_t i = 0; i < num_queries && fdp.remaining_bytes() > 8; i++) {
        // Consume domain name
        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::string domain_name = fdp.ConsumeBytesAsString(name_len);
        
        // Consume DNS class and type
        int dnsclass = fdp.ConsumeIntegralInRange<int>(C_IN, C_HS);
        int type = fdp.ConsumeIntegralInRange<int>(T_A, T_URI);
        
        // Send query using deprecated ares_query (for compatibility)
        ares_query(channel, domain_name.c_str(), dnsclass, type, 
                  query_callback, NULL);
        
        // Also test ares_send with raw buffer (deprecated)
        if (fdp.remaining_bytes() > 16) {
            size_t buf_len = fdp.ConsumeIntegralInRange<size_t>(12, 512);
            std::vector<uint8_t> query_buf = fdp.ConsumeBytes<uint8_t>(buf_len);
            if (query_buf.size() >= 12) { // Minimum DNS query size
                ares_send(channel, query_buf.data(), query_buf.size(), 
                         query_callback, NULL);
            }
        }
    }
    
    // 5. Test ares_gethostbyname for async host resolution
    if (fdp.remaining_bytes() > 4) {
        size_t hostname_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        std::string hostname = fdp.ConsumeBytesAsString(hostname_len);
        int family = fdp.ConsumeIntegralInRange<int>(AF_INET, AF_INET6);
        
        ares_gethostbyname(channel, hostname.c_str(), family, 
                          host_callback, NULL);
    }
    
    // 6. Process events using different event processing APIs
    
    // First, test ares_process_fds with fuzzed file descriptors
    if (fdp.remaining_bytes() > sizeof(ares_fd_events_t)) {
        // Create some fake file descriptor events
        ares_fd_events_t events[2];
        events[0].fd = fdp.ConsumeIntegral<ares_socket_t>();
        events[0].events = fdp.ConsumeIntegral<unsigned int>() & 
                          (ARES_FD_EVENT_READ | ARES_FD_EVENT_WRITE);
        
        events[1].fd = fdp.ConsumeIntegral<ares_socket_t>();
        events[1].events = fdp.ConsumeIntegral<unsigned int>() & 
                          (ARES_FD_EVENT_READ | ARES_FD_EVENT_WRITE);
        
        unsigned int flags = fdp.ConsumeIntegral<unsigned int>() & 
                           ARES_PROCESS_FLAG_SKIP_NON_FD;
        
        ares_process_fds(channel, events, 2, flags);
    }
    
    // Test ares_process_fd with individual file descriptors
    if (fdp.remaining_bytes() > sizeof(ares_socket_t) * 2) {
        ares_socket_t read_fd = fdp.ConsumeIntegral<ares_socket_t>();
        ares_socket_t write_fd = fdp.ConsumeIntegral<ares_socket_t>();
        ares_process_fd(channel, read_fd, write_fd);
    }
    
    // Test deprecated ares_process with fd_sets
    if (fdp.remaining_bytes() > 0) {
        // Create minimal fd_sets (most will be empty)
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        // Add a couple fuzzed fds if we have data
        if (fdp.remaining_bytes() > sizeof(int) * 2) {
            int fd1 = fdp.ConsumeIntegral<int>();
            int fd2 = fdp.ConsumeIntegral<int>();
            
            if (fd1 >= 0 && fd1 < FD_SETSIZE) {
                FD_SET(fd1, &read_fds);
            }
            if (fd2 >= 0 && fd2 < FD_SETSIZE) {
                FD_SET(fd2, &write_fds);
            }
        }
        
        ares_process(channel, &read_fds, &write_fds);
    }
    
    // 7. Test cancellation path
    if (fdp.ConsumeBool() && !query_ids.empty()) {
        // Cancel all pending queries
        ares_cancel(channel);
    }
    
    // 8. Test channel duplication
    ares_channel_t *dup_channel = NULL;
    if (fdp.ConsumeBool()) {
        int dup_result = ares_dup(&dup_channel, channel);
        if (dup_result == ARES_SUCCESS && dup_channel != NULL) {
            // Process some events on duplicated channel
            ares_socket_t read_fd = fdp.ConsumeIntegral<ares_socket_t>();
            ares_socket_t write_fd = fdp.ConsumeIntegral<ares_socket_t>();
            ares_process_fd(dup_channel, read_fd, write_fd);
            
            // Clean up duplicated channel
            ares_destroy(dup_channel);
        }
    }
    
    // 9. Process timeout events (simulate no file descriptors ready)
    if (fdp.ConsumeBool()) {
        // Process with no file descriptors to trigger timeout handling
        ares_process_fds(channel, NULL, 0, ARES_PROCESS_FLAG_NONE);
    }
    
    // 10. Process pending write operations
    if (fdp.ConsumeBool()) {
        ares_process_pending_write(channel);
    }
    
    // 11. Cleanup
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_001

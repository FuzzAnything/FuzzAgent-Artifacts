#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sys/select.h>
#include "ares.h"
#include "ares_dns.h"

namespace harness_003 {
/*
 * Fuzzing harness for c-ares DNS library - Core DNS Client Lifecycle
 * Targets: Complete DNS client initialization, configuration, query processing, and cleanup lifecycle
 * APIs: ares_dup, ares_init, ares_process_fd, ares_process_fds, ares_reinit, ares_send,
 *       ares_process_pending_write, ares_cancel, ares_set_servers family, and related APIs
 * 
 * This harness targets 15+ APIs representing over 3,000+ undiscovered branches
 * focusing on the complete semantic workflow of a DNS client state machine
 */



// Simple callback functions for async operations
static void query_callback(void *arg, int status, int timeouts, 
                          const unsigned char *abuf, int alen) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

static void dnsrec_callback(void *arg, ares_status_t status, size_t timeouts, 
                           const ares_dns_record_t *dnsrec) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for initialization and basic operations
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // PHASE 1: GLOBAL INITIALIZATION
    // 1. Call ares_library_init() for global initialization
    ares_library_init(ARES_LIB_INIT_ALL);

    // PHASE 2: CHANNEL CREATION AND DUPLICATION
    // 2. Call ares_init() or ares_init_options() to create a channel
    ares_channel_t *channel = NULL;
    ares_channel_t *dup_channel = NULL;
    
    // Choose between simple init and init with options
    if (fdp.ConsumeBool()) {
        // Use simple ares_init() - deprecated but still part of API surface
        ares_init(&channel);
    } else {
        // Use ares_init_options() with random options
        struct ares_options options;
        memset(&options, 0, sizeof(options));
        
        int optmask = 0;
        if (fdp.ConsumeBool()) {
            optmask |= ARES_OPT_FLAGS;
            options.flags = fdp.ConsumeIntegral<int>() & (ARES_FLAG_STAYOPEN | ARES_FLAG_NOSEARCH | ARES_FLAG_USEVC);
        }
        if (fdp.ConsumeBool()) {
            optmask |= ARES_OPT_TIMEOUTMS;
            options.timeout = fdp.ConsumeIntegralInRange<int>(1, 10000);
        }
        if (fdp.ConsumeBool()) {
            optmask |= ARES_OPT_TRIES;
            options.tries = fdp.ConsumeIntegralInRange<int>(1, 10);
        }
        if (fdp.ConsumeBool()) {
            optmask |= ARES_OPT_NDOTS;
            options.ndots = fdp.ConsumeIntegralInRange<int>(0, 15);
        }
        
        ares_init_options(&channel, &options, optmask);
    }
    
    if (channel == NULL) {
        ares_library_cleanup();
        return 0;
    }
    
    // 3. Call ares_dup() to test channel duplication
    // This is a critical API with 435 undiscovered branches
    ares_dup(&dup_channel, channel);
    
    // PHASE 3: SERVER CONFIGURATION
    // 4. Call ares_set_servers_csv() or ares_set_servers() to configure DNS servers
    if (fdp.remaining_bytes() > 10) {
        size_t csv_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
        std::string csv_data = fdp.ConsumeBytesAsString(csv_len);
        
        if (!csv_data.empty()) {
            char *csv_copy = strdup(csv_data.c_str());
            if (csv_copy) {
                // Try both CSV variants
                if (fdp.ConsumeBool()) {
                    ares_set_servers_csv(channel, csv_copy);
                } else {
                    ares_set_servers_ports_csv(channel, csv_copy);
                }
                free(csv_copy);
            }
        }
    }
    // Also test the deprecated ares_set_servers_ports API
    if (fdp.remaining_bytes() > 20) {
        struct ares_addr_port_node *servers = NULL;
        struct ares_addr_port_node *last = NULL;
        
        size_t num_servers = fdp.ConsumeIntegralInRange<size_t>(0, 4);
        
        for (size_t i = 0; i < num_servers && fdp.remaining_bytes() >= 20; i++) {
            struct ares_addr_port_node *server = (struct ares_addr_port_node*)malloc(sizeof(struct ares_addr_port_node));
            if (!server) break;
            
            memset(server, 0, sizeof(struct ares_addr_port_node));
            
            if (fdp.ConsumeBool()) {
                // IPv4 server
                server->family = AF_INET;
                server->addr.addr4.s_addr = fdp.ConsumeIntegral<uint32_t>();
                server->udp_port = fdp.ConsumeIntegralInRange<int>(1, 65535);
                server->tcp_port = fdp.ConsumeIntegralInRange<int>(1, 65535);
            } else if (fdp.remaining_bytes() >= 16) {
                // IPv6 server
                server->family = AF_INET6;
                std::vector<uint8_t> ip6_bytes = fdp.ConsumeBytes<uint8_t>(16);
                memcpy(&server->addr.addr6, ip6_bytes.data(), 16);
                server->udp_port = fdp.ConsumeIntegralInRange<int>(1, 65535);
                server->tcp_port = fdp.ConsumeIntegralInRange<int>(1, 65535);
            }
            
            server->next = NULL;
            
            if (last) {
                last->next = server;
                last = server;
            } else {
                servers = server;
                last = server;
            }
        }
        
        if (servers) {
            ares_set_servers_ports(channel, servers);
            
            // Free the server list
            struct ares_addr_port_node *current = servers;
            while (current) {
                struct ares_addr_port_node *next = current->next;
                free(current);
                current = next;
            }
        }
    }
    
    // PHASE 4: DNS QUERY CREATION AND SENDING
    // 5. Call ares_create_query() to create a DNS query
    // 6. Call ares_send() to send the query
    if (fdp.remaining_bytes() > 20) {
        size_t query_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        std::string query_name = fdp.ConsumeBytesAsString(query_name_len);
        
        if (!query_name.empty()) {
            // Create query using legacy API
            unsigned char *query_buf = NULL;
            int query_buflen = 0;
            
            int dns_class = fdp.ConsumeIntegralInRange<int>(1, 4);
            int dns_type = fdp.ConsumeIntegralInRange<int>(1, 100);
            unsigned short query_id = fdp.ConsumeIntegral<unsigned short>();
            
            ares_create_query(query_name.c_str(), dns_class, dns_type, 
                             query_id, fdp.ConsumeBool() ? 1 : 0,
                             &query_buf, &query_buflen, 1024);
            
            if (query_buf) {
                // Send the query using ares_send (274 undiscovered branches)
                ares_send(channel, query_buf, query_buflen, query_callback, NULL);
                free(query_buf);
            }
            
            // Also test ares_query() for comparison
            ares_query(channel, query_name.c_str(), dns_class, dns_type, 
                      query_callback, NULL);
        }
    }
    
    // PHASE 5: RESULT PROCESSING WITH FILE DESCRIPTORS
    // 7. Call ares_process_fd() or ares_process_fds() to process results
    // These have 307 and 301 undiscovered branches respectively
    
    // Test ares_process_fd() with random file descriptors
    ares_socket_t read_fd = fdp.ConsumeBool() ? ARES_SOCKET_BAD : 0;
    ares_socket_t write_fd = fdp.ConsumeBool() ? ARES_SOCKET_BAD : 1;
    ares_process_fd(channel, read_fd, write_fd);
    
    // Test ares_process_fds() with event arrays
    if (fdp.remaining_bytes() > 8) {
        size_t num_events = fdp.ConsumeIntegralInRange<size_t>(0, 4);
        std::vector<ares_fd_events_t> events;
        
        for (size_t i = 0; i < num_events && fdp.remaining_bytes() >= 12; i++) {
            ares_fd_events_t event;
            event.fd = fdp.ConsumeIntegral<ares_socket_t>();
            event.events = fdp.ConsumeIntegral<unsigned int>() & 
                          (ARES_PROCESS_FLAG_SKIP_NON_FD);
            events.push_back(event);
        }
        
        if (!events.empty()) {
            unsigned int flags = fdp.ConsumeIntegral<unsigned int>() & 
                                (ARES_PROCESS_FLAG_SKIP_NON_FD);
            ares_process_fds(channel, events.data(), events.size(), flags);
        } else {
            // Process with no events but possible timeouts
            ares_process_fds(channel, NULL, 0, ARES_PROCESS_FLAG_NONE);
        }
    }
    
    // PHASE 6: WRITE PROCESSING AND TIMEOUT MANAGEMENT
    // 8. Call ares_process_pending_write() for write operations (272 undiscovered branches)
    ares_process_pending_write(channel);
    
    // 9. Call ares_timeout() to check timeouts
    struct timeval tv;
    struct timeval *timeout = ares_timeout(channel, NULL, &tv);
    (void)timeout; // Use timeout if needed
    
    // PHASE 7: QUERY CANCELLATION AND RE-INITIALIZATION
    // 10. Call ares_cancel() to test query cancellation (270 undiscovered branches)
    ares_cancel(channel);
    
    // 11. Call ares_reinit() to test re-initialization (274 undiscovered branches)
    // Reinit requires saving options first
    struct ares_options saved_options;
    memset(&saved_options, 0, sizeof(saved_options));
    int saved_optmask = 0;
    
    if (ares_save_options(channel, &saved_options, &saved_optmask) == ARES_SUCCESS) {
        ares_reinit(channel);
        
        // Restore options if needed
        if (saved_optmask != 0) {
            // Note: ares_init_options would be needed to restore, but we're testing reinit
        }
        ares_destroy_options(&saved_options);
    }
    
    // PHASE 8: ADDITIONAL CONFIGURATION TESTS
    // Test ares_set_sortlist if we have input
    if (fdp.remaining_bytes() > 5) {
        size_t sortlist_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::string sortlist_str = fdp.ConsumeBytesAsString(sortlist_len);
        
        if (!sortlist_str.empty()) {
            ares_set_sortlist(channel, sortlist_str.c_str());
        }
    }
    
    // Test local binding configuration
    if (fdp.remaining_bytes() >= 4) {
        uint32_t local_ip = fdp.ConsumeIntegral<uint32_t>();
        ares_set_local_ip4(channel, local_ip);
    }
    
    if (fdp.remaining_bytes() >= 16) {
        unsigned char local_ip6[16];
        std::vector<uint8_t> ip6_bytes = fdp.ConsumeBytes<uint8_t>(16);
        memcpy(local_ip6, ip6_bytes.data(), 16);
        ares_set_local_ip6(channel, local_ip6);
    }
    
    if (fdp.remaining_bytes() > 5) {
        size_t dev_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
        std::string dev_name = fdp.ConsumeBytesAsString(dev_len);
        
        if (!dev_name.empty()) {
            ares_set_local_dev(channel, dev_name.c_str());
        }
    }
    
    // PHASE 9: CLEANUP
    // 12. Call ares_destroy() to cleanup the channel
    if (dup_channel) {
        ares_destroy(dup_channel);
    }
    ares_destroy(channel);
    
    // 13. Call ares_library_cleanup() for global cleanup
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_003

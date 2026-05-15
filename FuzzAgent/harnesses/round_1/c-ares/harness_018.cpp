#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include "ares.h"

namespace harness_018 {

// Comprehensive harness targeting server configuration operations in ares_update_servers.c
// Goal: Address 470-474+ accumulated branches in server configuration functions
// Targeting: ares_set_servers_ports_csv (115 branches), ares_set_servers_ports (117 branches),
//            ares_set_servers (117 branches), ares_get_servers_ports (13 branches),
//            ares_get_servers (13 branches), ares_get_servers_csv (8 branches),
//            ares_init (175 branches), ares_init_options (175 branches),
//            ares_destroy (131 branches)

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need substantial input for comprehensive server configuration testing
    // Includes: CSV strings, server arrays, port arrays, options structures
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Global library initialization (exact sequence from guidance)
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // Step 2: Create channel using different initialization methods based on fuzzed input
    ares_channel_t *channel = NULL;
    int init_result = ARES_SUCCESS;
    
    // Consume initialization method (0 = ares_init, 1 = ares_init_options)
    uint8_t init_method = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    if (init_method == 0) {
        // Method 1: Simple ares_init (deprecated but still in API)
        init_result = ares_init(&channel);
    } else {
        // Method 2: ares_init_options with fuzzed options
        struct ares_options options;
        memset(&options, 0, sizeof(options));
        
        // Consume various option values from fuzzed data
        int optmask = 0;
        
        // Consume timeout value (if enough data)
        if (fdp.remaining_bytes() >= sizeof(int)) {
            options.timeout = fdp.ConsumeIntegral<int>();
            optmask |= ARES_OPT_TIMEOUTMS;
        }
        
        // Consume tries value
        if (fdp.remaining_bytes() >= sizeof(int)) {
            options.tries = fdp.ConsumeIntegral<int>();
            optmask |= ARES_OPT_TRIES;
        }
        
        // Consume ndots value
        if (fdp.remaining_bytes() >= sizeof(int)) {
            options.ndots = fdp.ConsumeIntegral<int>();
            optmask |= ARES_OPT_NDOTS;
        }
        
        // Consume UDP port
        if (fdp.remaining_bytes() >= sizeof(unsigned short)) {
            options.udp_port = fdp.ConsumeIntegral<unsigned short>();
            optmask |= ARES_OPT_UDP_PORT;
        }
        
        // Consume TCP port
        if (fdp.remaining_bytes() >= sizeof(unsigned short)) {
            options.tcp_port = fdp.ConsumeIntegral<unsigned short>();
            optmask |= ARES_OPT_TCP_PORT;
        }
        
        // Consume flags (bitmask)
        if (fdp.remaining_bytes() >= sizeof(int)) {
            options.flags = fdp.ConsumeIntegral<int>();
            optmask |= ARES_OPT_FLAGS;
        }
        
        // Consume domains (up to 5 domains)
        if (fdp.remaining_bytes() > 16) {
            size_t num_domains = fdp.ConsumeIntegralInRange<size_t>(0, 5);
            char **domains = NULL;
            
            if (num_domains > 0) {
                domains = (char**)malloc((num_domains + 1) * sizeof(char*));
                if (domains) {
                    // Initialize all slots to NULL for safety
                    for (size_t i = 0; i < num_domains + 1; i++) {
                        domains[i] = NULL;
                    }
                    
                    for (size_t i = 0; i < num_domains && fdp.remaining_bytes() > 8; i++) {
                        size_t domain_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                        if (fdp.remaining_bytes() < domain_len) break;
                        
                        std::string domain = fdp.ConsumeBytesAsString(domain_len);
                        domains[i] = strdup(domain.c_str());
                    }
                    // NULL-terminate at the last allocated slot
                    domains[num_domains] = NULL;
                    options.domains = domains;
                    options.ndomains = (int)num_domains;  // CRITICAL: Set ndomains field
                    optmask |= ARES_OPT_DOMAINS;
                }
            }
        }
        
        // Consume lookups (string)
        if (fdp.remaining_bytes() > 8) {
            size_t lookup_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
            if (fdp.remaining_bytes() >= lookup_len) {
                std::string lookup_str = fdp.ConsumeBytesAsString(lookup_len);
                options.lookups = strdup(lookup_str.c_str());
                optmask |= ARES_OPT_LOOKUPS;
            }
        }
        
        // Initialize channel with options
        init_result = ares_init_options(&channel, &options, optmask);
        
        // Clean up allocated strings using proper ndomains field
        if (options.domains) {
            for (int i = 0; i < options.ndomains; i++) {
                free(options.domains[i]);
            }
            free(options.domains);
        }
        if (options.lookups) {
            free((void*)options.lookups);
        }
    }
    
    if (init_result != ARES_SUCCESS || channel == NULL) {
        ares_library_cleanup();
        return 0;
    }
    
    // Step 3: Test ares_set_servers_ports_csv (set servers from CSV string)
    if (fdp.remaining_bytes() > 16) {
        // Generate CSV server string from fuzzed data
        size_t csv_len = fdp.ConsumeIntegralInRange<size_t>(8, 256);
        if (fdp.remaining_bytes() >= csv_len) {
            std::string csv_servers = fdp.ConsumeBytesAsString(csv_len);
            
            // Test various CSV formats:
            // 1. Simple IP addresses: "1.2.3.4,5.6.7.8"
            // 2. IP addresses with ports: "1.2.3.4:53,5.6.7.8:5353"
            // 3. IPv6 addresses: "[2001:db8::1]:53,[2001:db8::2]:5353"
            // 4. Mixed formats: "1.2.3.4,[2001:db8::1]:53"
            // 5. Empty string (should clear servers)
            
            // Also test ares_set_servers_csv (the non-deprecated version)
            ares_set_servers_csv(channel, csv_servers.c_str());
            ares_set_servers_ports_csv(channel, csv_servers.c_str());
        }
    }
    
    // Step 4: Test ares_set_servers_ports (set servers from structured array with ports)
    if (fdp.remaining_bytes() > 32) {
        size_t num_port_servers = fdp.ConsumeIntegralInRange<size_t>(0, 5);
        
        if (num_port_servers > 0) {
            // Calculate bytes needed for worst-case (all IPv6 with ports)
            size_t worst_case_bytes = num_port_servers * (1 + 16 + 4); // family + IPv6 + 2 ports
            
            if (fdp.remaining_bytes() >= worst_case_bytes) {
                struct ares_addr_port_node *port_servers = NULL;
                struct ares_addr_port_node *last_port = NULL;
                
                for (size_t i = 0; i < num_port_servers; i++) {
                    struct ares_addr_port_node *node = 
                        (struct ares_addr_port_node*)malloc(sizeof(struct ares_addr_port_node));
                    if (!node) break;
                    
                    memset(node, 0, sizeof(struct ares_addr_port_node));
                    
                    // Consume address family
                    int family = fdp.ConsumeBool() ? AF_INET : AF_INET6;
                    node->family = family;
                    
                    if (family == AF_INET) {
                        // IPv4 address
                        std::vector<uint8_t> ip_bytes = fdp.ConsumeBytes<uint8_t>(4);
                        memcpy(&node->addr.addr4, ip_bytes.data(), 4);
                    } else {
                        // IPv6 address
                        std::vector<uint8_t> ip_bytes = fdp.ConsumeBytes<uint8_t>(16);
                        memcpy(&node->addr.addr6, ip_bytes.data(), 16);
                    }
                    
                    // Consume port numbers (0-65535, 0 means default)
                    node->udp_port = fdp.ConsumeIntegralInRange<int>(0, 65535);
                    node->tcp_port = fdp.ConsumeIntegralInRange<int>(0, 65535);
                    
                    node->next = NULL;
                    
                    if (!port_servers) {
                        port_servers = node;
                        last_port = node;
                    } else {
                        last_port->next = node;
                        last_port = node;
                    }
                }
                
                // Set servers with ports
                ares_set_servers_ports(channel, port_servers);
                
                // Free allocated list
                struct ares_addr_port_node *current = port_servers;
                while (current) {
                    struct ares_addr_port_node *next = current->next;
                    free(current);
                    current = next;
                }
            }
        }
    }
    
    // Step 5: Test ares_set_servers (set servers from basic array without ports)
    if (fdp.remaining_bytes() > 24) {
        size_t num_servers = fdp.ConsumeIntegralInRange<size_t>(0, 5);
        
        if (num_servers > 0) {
            // Calculate bytes needed for worst-case (all IPv6)
            size_t worst_case_bytes = num_servers * (1 + 16); // family + IPv6
            
            if (fdp.remaining_bytes() >= worst_case_bytes) {
                struct ares_addr_node *servers = NULL;
                struct ares_addr_node *last = NULL;
                
                for (size_t i = 0; i < num_servers; i++) {
                    struct ares_addr_node *node = 
                        (struct ares_addr_node*)malloc(sizeof(struct ares_addr_node));
                    if (!node) break;
                    
                    memset(node, 0, sizeof(struct ares_addr_node));
                    
                    // Consume address family
                    int family = fdp.ConsumeBool() ? AF_INET : AF_INET6;
                    node->family = family;
                    
                    if (family == AF_INET) {
                        // IPv4 address
                        std::vector<uint8_t> ip_bytes = fdp.ConsumeBytes<uint8_t>(4);
                        memcpy(&node->addr.addr4, ip_bytes.data(), 4);
                    } else {
                        // IPv6 address
                        std::vector<uint8_t> ip_bytes = fdp.ConsumeBytes<uint8_t>(16);
                        memcpy(&node->addr.addr6, ip_bytes.data(), 16);
                    }
                    
                    node->next = NULL;
                    
                    if (!servers) {
                        servers = node;
                        last = node;
                    } else {
                        last->next = node;
                        last = node;
                    }
                }
                
                // Set servers (deprecated API)
                ares_set_servers(channel, servers);
                
                // Free allocated list
                struct ares_addr_node *current = servers;
                while (current) {
                    struct ares_addr_node *next = current->next;
                    free(current);
                    current = next;
                }
            }
        }
    }
    
    // Step 6: Test ares_get_servers_ports (retrieve configured servers with ports)
    struct ares_addr_port_node *retrieved_port_servers = NULL;
    ares_get_servers_ports(channel, &retrieved_port_servers);
    
    // Step 7: Test ares_get_servers (retrieve basic server list)
    struct ares_addr_node *retrieved_servers = NULL;
    ares_get_servers(channel, &retrieved_servers);
    
    // Step 8: Test ares_get_servers_csv (retrieve CSV representation)
    char *csv_output = ares_get_servers_csv(channel);
    if (csv_output) {
        // CSV string can be used for further testing
        (void)csv_output; // Mark as used
        ares_free_string(csv_output);
    }
    
    // Step 9: Free retrieved server lists (if any)
    if (retrieved_port_servers) {
        ares_free_data(retrieved_port_servers);
    }
    if (retrieved_servers) {
        ares_free_data(retrieved_servers);
    }
    
    // Step 10: Cleanup - destroy channel
    ares_destroy(channel);
    
    // Step 11: Global cleanup
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_018

#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "ares.h"

namespace harness_003 {


// Fuzzer entry point for server configuration APIs
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for meaningful testing: server count + at least one address
    if (size < 16) {
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
    
    // Determine number of servers to create (0-10, with 0 testing edge case)
    size_t num_servers = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    
    // 3. Test ares_set_servers / ares_get_servers (without ports)
    if (num_servers > 0) {
        // Calculate worst-case bytes needed: all IPv6 (most bytes)
        // Each server: 1 byte for family + 16 bytes for IPv6 address
        size_t worst_case_bytes_needed = num_servers * 17;
        
        if (fdp.remaining_bytes() < worst_case_bytes_needed) {
            // Not enough data for all servers in worst case, skip this test
            ares_destroy(channel);
            ares_library_cleanup();
            return 0;
        }
        
        struct ares_addr_node *servers = NULL;
        struct ares_addr_node *last = NULL;
        
        // Create linked list of server structures
        for (size_t i = 0; i < num_servers; i++) {
            struct ares_addr_node *node = (struct ares_addr_node*)malloc(sizeof(struct ares_addr_node));
            if (!node) break;
            
            memset(node, 0, sizeof(struct ares_addr_node));
            
            // Consume address family (AF_INET or AF_INET6)
            int family = fdp.ConsumeBool() ? AF_INET : AF_INET6;
            node->family = family;
            
            if (family == AF_INET) {
                // IPv4 address (4 bytes)
                std::vector<uint8_t> ip_bytes = fdp.ConsumeBytes<uint8_t>(4);
                memcpy(&node->addr.addr4, ip_bytes.data(), 4);
            } else {
                // IPv6 address (16 bytes)
                std::vector<uint8_t> ip_bytes = fdp.ConsumeBytes<uint8_t>(16);
                memcpy(&node->addr.addr6, ip_bytes.data(), 16);
            }
            
            node->next = NULL;
            
            // Add to linked list
            if (!servers) {
                servers = node;
                last = node;
            } else {
                last->next = node;
                last = node;
            }
        }
        
        // Set servers on channel
        int set_result = ares_set_servers(channel, servers);
        (void)set_result; // Result can be ignored for fuzzing
        
        // Get servers back for verification
        struct ares_addr_node *retrieved_servers = NULL;
        int get_result = ares_get_servers(channel, &retrieved_servers);
        (void)get_result; // Result can be ignored for fuzzing
        
        // Free retrieved servers list
        if (retrieved_servers) {
            ares_free_data(retrieved_servers);
        }
        
        // Free our allocated server list
        struct ares_addr_node *current = servers;
        while (current) {
            struct ares_addr_node *next = current->next;
            free(current);
            current = next;
        }
    }
    
    // 4. Test ares_set_servers_ports / ares_get_servers_ports (with ports)
    // Reset server count for ports version
    if (fdp.remaining_bytes() > 4) {
        size_t num_port_servers = fdp.ConsumeIntegralInRange<size_t>(0, 8);
        
        if (num_port_servers > 0) {
            // Calculate worst-case bytes needed: all IPv6 (most bytes)
            // Each server: 1 byte for family + 16 bytes for IPv6 address + 4 bytes for ports
            size_t worst_case_port_bytes_needed = num_port_servers * 21;
            
            if (fdp.remaining_bytes() < worst_case_port_bytes_needed) {
                // Not enough data for all servers with ports in worst case, skip this test
                ares_destroy(channel);
                ares_library_cleanup();
                return 0;
            }
            
            struct ares_addr_port_node *port_servers = NULL;
            struct ares_addr_port_node *last_port = NULL;
            
            // Create linked list of server structures with ports
            for (size_t i = 0; i < num_port_servers; i++) {
                struct ares_addr_port_node *node = (struct ares_addr_port_node*)malloc(sizeof(struct ares_addr_port_node));
                if (!node) break;
                
                memset(node, 0, sizeof(struct ares_addr_port_node));
                
                // Consume address family (AF_INET or AF_INET6)
                int family = fdp.ConsumeBool() ? AF_INET : AF_INET6;
                node->family = family;
                
                if (family == AF_INET) {
                    // IPv4 address (4 bytes)
                    std::vector<uint8_t> ip_bytes = fdp.ConsumeBytes<uint8_t>(4);
                    memcpy(&node->addr.addr4, ip_bytes.data(), 4);
                } else {
                    // IPv6 address (16 bytes)
                    std::vector<uint8_t> ip_bytes = fdp.ConsumeBytes<uint8_t>(16);
                    memcpy(&node->addr.addr6, ip_bytes.data(), 16);
                }
                
                // Consume port numbers (1-65535)
                node->udp_port = fdp.ConsumeIntegralInRange<int>(1, 65535);
                node->tcp_port = fdp.ConsumeIntegralInRange<int>(1, 65535);
                
                node->next = NULL;
                
                // Add to linked list
                if (!port_servers) {
                    port_servers = node;
                    last_port = node;
                } else {
                    last_port->next = node;
                    last_port = node;
                }
            }
            
            // Set servers with ports on channel
            int set_port_result = ares_set_servers_ports(channel, port_servers);
            (void)set_port_result; // Result can be ignored for fuzzing
            
            // Get servers with ports back for verification
            struct ares_addr_port_node *retrieved_port_servers = NULL;
            int get_port_result = ares_get_servers_ports(channel, &retrieved_port_servers);
            (void)get_port_result; // Result can be ignored for fuzzing
            
            // Free retrieved servers list
            if (retrieved_port_servers) {
                ares_free_data(retrieved_port_servers);
            }
            
            // Free our allocated server list
            struct ares_addr_port_node *current_port = port_servers;
            while (current_port) {
                struct ares_addr_port_node *next = current_port->next;
                free(current_port);
                current_port = next;
            }
        }
    }
    
    // 5. Test CSV-based server configuration (mentioned in guidance)
    if (fdp.remaining_bytes() > 8) {
        // Create CSV string from remaining bytes
        std::string csv_string = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
        
        // Test ares_set_servers_csv
        int csv_result = ares_set_servers_csv(channel, csv_string.c_str());
        (void)csv_result; // Result can be ignored for fuzzing
        
        // Get CSV representation
        char *csv_output = ares_get_servers_csv(channel);
        if (csv_output) {
            free(csv_output);
        }
    }
    
    // 6. Test edge cases: NULL servers, empty configuration
    if (fdp.ConsumeBool()) {
        // Test with NULL servers (should clear configuration)
        ares_set_servers(channel, NULL);
        ares_set_servers_ports(channel, NULL);
        
        // Get servers after clearing
        struct ares_addr_node *empty_servers = NULL;
        ares_get_servers(channel, &empty_servers);
        if (empty_servers) {
            ares_free_data(empty_servers);
        }
    }
    
    // 7. Cleanup
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_003

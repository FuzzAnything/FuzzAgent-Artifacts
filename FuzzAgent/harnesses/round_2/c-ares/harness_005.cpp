#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "ares.h"

namespace harness_005 {
/*
 * Fuzzing harness for c-ares DNS library - DNS Server Configuration Management APIs
 * Targets: 8 completely uncovered server configuration APIs with 766+ undiscovered branches
 * APIs: ares_set_servers, ares_get_servers, ares_set_servers_ports, ares_get_servers_ports,
 *       ares_set_servers_csv, ares_get_servers_csv, ares_set_servers_ports_csv, 
 *       ares_set_server_state_callback
 * 
 * Semantic Focus: DNS server dynamic configuration management for real-world scenarios like
 * DHCP updates, failover scenarios, and network management.
 * Follows exact invocation sequence from coverage guidance for maximum branch coverage.
 */



// Server state callback function for ares_set_server_state_callback
static void server_state_callback(const char *server_string,
                                  ares_bool_t success,
                                  int flags,
                                  void *data) {
    // Simple callback for server state changes
    (void)server_string;
    (void)success;
    (void)flags;
    (void)data;
    // Callback does nothing in fuzzing context - just exercises the callback registration
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for initialization and server configuration
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // PHASE 1: Initialize c-ares library and create channel
    // Step 1: Call ares_init to create channel context
    ares_library_init(ARES_LIB_INIT_ALL);
    
    ares_channel_t *channel = NULL;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    options.timeout = 1000;
    
    int optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS;
    ares_init_options(&channel, &options, optmask);
    
    if (channel == NULL) {
        ares_library_cleanup();
        return 0;
    }

    // PHASE 2: Test server configuration APIs following guidance sequence
    
    // Step 2: Call ares_set_servers to configure initial DNS servers
    struct ares_addr_node *server_list = NULL;
    struct ares_addr_node *last_server = NULL;
    
    // Create 1-4 servers based on fuzzer input
    size_t num_servers = fdp.ConsumeIntegralInRange<size_t>(1, 4);
    
    for (size_t i = 0; i < num_servers && fdp.remaining_bytes() >= 20; i++) {
        struct ares_addr_node *server = (struct ares_addr_node*)malloc(sizeof(struct ares_addr_node));
        if (!server) break;
        
        memset(server, 0, sizeof(struct ares_addr_node));
        server->next = NULL;
        
        // Choose IPv4 or IPv6 based on fuzzer input
        if (fdp.ConsumeBool()) {
            // IPv4 server
            server->family = AF_INET;
            server->addr.addr4.s_addr = fdp.ConsumeIntegral<uint32_t>();
        } else if (fdp.remaining_bytes() >= 16) {
            // IPv6 server
            server->family = AF_INET6;
            std::vector<uint8_t> ip6_bytes = fdp.ConsumeBytes<uint8_t>(16);
            memcpy(&server->addr.addr6, ip6_bytes.data(), 16);
        }
        
        // Add to linked list
        if (last_server) {
            last_server->next = server;
            last_server = server;
        } else {
            server_list = server;
            last_server = server;
        }
    }
    
    if (server_list) {
        ares_set_servers(channel, server_list);
        
        // Step 3: Call ares_get_servers to verify configuration
        struct ares_addr_node *retrieved_servers = NULL;
        ares_get_servers(channel, &retrieved_servers);
        
        // Free retrieved server list if allocated - ares_free_data should be called once on head
        if (retrieved_servers) {
            ares_free_data(retrieved_servers);
        }
        // Free original server list
        struct ares_addr_node *current = server_list;
        while (current) {
            struct ares_addr_node *next = current->next;
            free(current);
            current = next;
        }
    }

    // Step 4: Call ares_set_servers_ports to update with custom ports
    struct ares_addr_port_node *port_server_list = NULL;
    struct ares_addr_port_node *last_port_server = NULL;
    
    size_t num_port_servers = fdp.ConsumeIntegralInRange<size_t>(1, 4);
    
    for (size_t i = 0; i < num_port_servers && fdp.remaining_bytes() >= 24; i++) {
        struct ares_addr_port_node *server = (struct ares_addr_port_node*)malloc(sizeof(struct ares_addr_port_node));
        if (!server) break;
        
        memset(server, 0, sizeof(struct ares_addr_port_node));
        server->next = NULL;
        
        // Choose IPv4 or IPv6
        if (fdp.ConsumeBool()) {
            // IPv4 server with ports
            server->family = AF_INET;
            server->addr.addr4.s_addr = fdp.ConsumeIntegral<uint32_t>();
            server->udp_port = fdp.ConsumeIntegralInRange<int>(1, 65535);
            server->tcp_port = fdp.ConsumeIntegralInRange<int>(1, 65535);
        } else if (fdp.remaining_bytes() >= 20) {
            // IPv6 server with ports
            server->family = AF_INET6;
            std::vector<uint8_t> ip6_bytes = fdp.ConsumeBytes<uint8_t>(16);
            memcpy(&server->addr.addr6, ip6_bytes.data(), 16);
            server->udp_port = fdp.ConsumeIntegralInRange<int>(1, 65535);
            server->tcp_port = fdp.ConsumeIntegralInRange<int>(1, 65535);
        }
        
        // Add to linked list
        if (last_port_server) {
            last_port_server->next = server;
            last_port_server = server;
        } else {
            port_server_list = server;
            last_port_server = server;
        }
    }
    
    if (port_server_list) {
        ares_set_servers_ports(channel, port_server_list);
        
        // Step 5: Call ares_get_servers_ports to verify port configuration
        struct ares_addr_port_node *retrieved_port_servers = NULL;
        ares_get_servers_ports(channel, &retrieved_port_servers);
        
        // Free retrieved port server list if allocated - ares_free_data should be called once on head
        if (retrieved_port_servers) {
            ares_free_data(retrieved_port_servers);
        }
        
        // Free original port server list
        struct ares_addr_port_node *current = port_server_list;
        while (current) {
            struct ares_addr_port_node *next = current->next;
            free(current);
            current = next;
        }
    }

    // Step 6: Call ares_set_servers_csv with CSV-formatted server list
    if (fdp.remaining_bytes() > 20) {
        size_t csv_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
        std::string csv_data = fdp.ConsumeBytesAsString(csv_len);
        
        if (!csv_data.empty()) {
            // Create null-terminated copy for CSV API
            char *csv_copy = strdup(csv_data.c_str());
            if (csv_copy) {
                ares_set_servers_csv(channel, csv_copy);
                free(csv_copy);
            }
        }
    }

    // Step 7: Call ares_get_servers_csv to retrieve CSV representation
    char *csv_output = ares_get_servers_csv(channel);
    if (csv_output) {
        free(csv_output);
    }
    if (fdp.remaining_bytes() > 20) {
        size_t ports_csv_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
        std::string ports_csv_data = fdp.ConsumeBytesAsString(ports_csv_len);
        
        if (!ports_csv_data.empty()) {
            // Create null-terminated copy for CSV API with ports
            char *ports_csv_copy = strdup(ports_csv_data.c_str());
            if (ports_csv_copy) {
                ares_set_servers_ports_csv(channel, ports_csv_copy);
                free(ports_csv_copy);
            }
        }
    }

    // Step 9: Call ares_set_server_state_callback to set state change handler
    ares_set_server_state_callback(channel, server_state_callback, NULL);

    // PHASE 3: Cleanup
    // Step 10: Call ares_destroy to clean up resources
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}

} // namespace harness_005

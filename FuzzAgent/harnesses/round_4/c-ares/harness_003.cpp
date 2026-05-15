#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include "ares.h"

namespace harness_003 {
/* Fuzzing harness for c-ares library targeting server configuration APIs
 * This harness tests server configuration lifecycle APIs with fuzzer-generated input
 * Target APIs: ares_set_servers (210 undiscovered branches), ares_set_servers_ports (210),
 *              ares_get_servers (38), ares_get_servers_ports (38), ares_set_server_state_callback (2)
 * Helper APIs: ares_init/ares_init_options (initialization), ares_destroy (cleanup),
 *              ares_set_servers_csv (already partially covered)
 * Following invocation sequence from coverage guidance:
 * 1. ares_init or ares_init_options to create channel
 * 2. ares_set_servers/ares_set_servers_ports to configure DNS servers
 * 3. ares_get_servers/ares_get_servers_ports to retrieve configuration  
 * 4. ares_set_server_state_callback for state change notifications
 * 5. ares_destroy cleanup
 * Testing edge cases: empty server lists, invalid server addresses (IPv4/IPv6),
 * mixed configurations, port boundaries, multiple calls
 */



// Server state callback for testing ares_set_server_state_callback
static void server_state_callback(const char* server_string,
                                  ares_bool_t success,
                                  int flags,
                                  void* data) {
    // Callback doesn't need to do anything for fuzzing - just consume the notification
    (void)server_string;
    (void)success;
    (void)flags;
    (void)data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    // We need enough data for initialization parameters and server configurations
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Initialize library
    ares_library_init(ARES_LIB_INIT_ALL);

    // Create channel with ares_init_options
    ares_channel_t* channel = NULL;
    ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    
    // Consume fuzzer data for initialization parameters
    int timeout = fdp.ConsumeIntegralInRange<int>(1, 10000);
    int tries = fdp.ConsumeIntegralInRange<int>(1, 10);
    int ndots = fdp.ConsumeIntegralInRange<int>(0, 15);
    bool use_tcp = fdp.ConsumeBool();
    bool use_edns = fdp.ConsumeBool();
    
    options.timeout = timeout;
    optmask |= ARES_OPT_TIMEOUTMS;
    
    options.tries = tries;
    optmask |= ARES_OPT_TRIES;
    
    options.ndots = ndots;
    optmask |= ARES_OPT_NDOTS;
    
    if (use_tcp) {
        options.flags |= ARES_FLAG_USEVC;
        optmask |= ARES_OPT_FLAGS;
    }
    
    if (use_edns) {
        options.flags |= ARES_FLAG_EDNS;
        optmask |= ARES_OPT_FLAGS;
        options.ednspsz = fdp.ConsumeIntegralInRange<int>(0, 4096);
        optmask |= ARES_OPT_EDNSPSZ;
    }
    
    // Try creating channel with ares_init_options
    int status = ares_init_options(&channel, &options, optmask);
    if (status != ARES_SUCCESS) {
        // If ares_init_options fails, try the deprecated ares_init
        status = ares_init(&channel);
        if (status != ARES_SUCCESS) {
            ares_library_cleanup();
            return 0;
        }
    }
    
    // Step 4: Set server state callback (do this before server configuration)
    ares_set_server_state_callback(channel, server_state_callback, NULL);
    
    // Determine which server configuration API to test based on fuzzer input
    uint8_t config_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume number of servers to configure (0-5 to test empty lists and small sets)
    size_t server_count = fdp.ConsumeIntegralInRange<size_t>(0, 5);
    
    if (config_type == 0) {
        // Test ares_set_servers (without ports)
        if (server_count == 0) {
            // Test empty server list
            struct ares_addr_node* servers = NULL;
            ares_set_servers(channel, servers);
        } else {
            // Build server list
            struct ares_addr_node* servers = NULL;
            struct ares_addr_node* prev = NULL;
            
            for (size_t i = 0; i < server_count && fdp.remaining_bytes() >= 6; i++) {
                struct ares_addr_node* node = 
                    (struct ares_addr_node*)malloc(sizeof(struct ares_addr_node));
                if (node == NULL) {
                    break;
                }
                memset(node, 0, sizeof(struct ares_addr_node));
                
                // Consume IP family (IPv4 or IPv6)
                int family = fdp.ConsumeBool() ? AF_INET : AF_INET6;
                node->family = family;
                
                if (family == AF_INET) {
                    // Consume IPv4 address (4 bytes)
                    if (fdp.remaining_bytes() >= 4) {
                        std::vector<uint8_t> ip_bytes = fdp.ConsumeBytes<uint8_t>(4);
                        memcpy(&node->addr.addr4, ip_bytes.data(), 4);
                    } else {
                        // Not enough data for IPv4, use default
                        node->addr.addr4.s_addr = htonl(0x7f000001); // 127.0.0.1
                    }
                } else {
                    // Consume IPv6 address (16 bytes)
                    if (fdp.remaining_bytes() >= 16) {
                        std::vector<uint8_t> ip_bytes = fdp.ConsumeBytes<uint8_t>(16);
                        memcpy(&node->addr.addr6, ip_bytes.data(), 16);
                    } else {
                        // Not enough data for IPv6, use default
                        memset(&node->addr.addr6, 0, sizeof(struct ares_in6_addr));
                        node->addr.addr6._S6_un._S6_u8[15] = 1; // ::1
                    }
                }
                
                node->next = NULL;
                
                if (prev == NULL) {
                    servers = node;
                } else {
                    prev->next = node;
                }
                prev = node;
            }
            
            // Apply server configuration
            ares_set_servers(channel, servers);
            
            // Free the server list
            struct ares_addr_node* curr = servers;
            while (curr != NULL) {
                struct ares_addr_node* next = curr->next;
                free(curr);
                curr = next;
            }
        }
        
        // Retrieve and verify configuration
        struct ares_addr_node* retrieved_servers = NULL;
        ares_get_servers(channel, &retrieved_servers);
        
        // Free retrieved servers
        if (retrieved_servers != NULL) {
            ares_free_data(retrieved_servers);
        }
        
    } else if (config_type == 1) {
        // Test ares_set_servers_ports (with ports)
        if (server_count == 0) {
            // Test empty server list
            struct ares_addr_port_node* servers = NULL;
            ares_set_servers_ports(channel, servers);
        } else {
            // Build server list with ports
            struct ares_addr_port_node* servers = NULL;
            struct ares_addr_port_node* prev = NULL;
            
            for (size_t i = 0; i < server_count && fdp.remaining_bytes() >= 8; i++) {
                struct ares_addr_port_node* node = 
                    (struct ares_addr_port_node*)malloc(sizeof(struct ares_addr_port_node));
                if (node == NULL) {
                    break;
                }
                
                memset(node, 0, sizeof(struct ares_addr_port_node));
                
                // Consume IP family (IPv4 or IPv6)
                int family = fdp.ConsumeBool() ? AF_INET : AF_INET6;
                node->family = family;
                
                if (family == AF_INET) {
                    // Consume IPv4 address (4 bytes)
                    if (fdp.remaining_bytes() >= 4) {
                        std::vector<uint8_t> ip_bytes = fdp.ConsumeBytes<uint8_t>(4);
                        memcpy(&node->addr.addr4, ip_bytes.data(), 4);
                    } else {
                        // Not enough data for IPv4, use default
                        node->addr.addr4.s_addr = htonl(0x7f000001); // 127.0.0.1
                    }
                } else {
                    // Consume IPv6 address (16 bytes)
                    if (fdp.remaining_bytes() >= 16) {
                        std::vector<uint8_t> ip_bytes = fdp.ConsumeBytes<uint8_t>(16);
                        memcpy(&node->addr.addr6, ip_bytes.data(), 16);
                    } else {
                        // Not enough data for IPv6, use default
                        memset(&node->addr.addr6, 0, sizeof(struct ares_in6_addr));
                        node->addr.addr6._S6_un._S6_u8[15] = 1; // ::1
                    }
                }
                
                // Consume port numbers (0-65535, with 0 meaning default)
                node->udp_port = fdp.ConsumeIntegralInRange<int>(0, 65535);
                node->tcp_port = fdp.ConsumeIntegralInRange<int>(0, 65535);
                
                node->next = NULL;
                
                if (prev == NULL) {
                    servers = node;
                } else {
                    prev->next = node;
                }
                prev = node;
            }
            
            // Apply server configuration with ports
            ares_set_servers_ports(channel, servers);
            
            // Free the server list
            struct ares_addr_port_node* curr = servers;
            while (curr != NULL) {
                struct ares_addr_port_node* next = curr->next;
                free(curr);
                curr = next;
            }
        }
        
        // Retrieve and verify configuration with ports
        struct ares_addr_port_node* retrieved_servers = NULL;
        ares_get_servers_ports(channel, &retrieved_servers);
        
        // Free retrieved servers
        if (retrieved_servers != NULL) {
            ares_free_data(retrieved_servers);
        }
        
    } else if (config_type == 2) {
        // Test ares_set_servers_csv (CSV format)
        // Consume CSV string from remaining input
        std::string csv_input;
        if (fdp.remaining_bytes() > 0) {
            // Limit length to prevent excessive memory usage
            size_t max_len = fdp.ConsumeIntegralInRange<size_t>(0, 512);
            csv_input = fdp.ConsumeRandomLengthString(max_len);
        }
        
        // Test with consumed CSV string (could be empty, valid, or invalid)
        ares_set_servers_csv(channel, csv_input.c_str());
        
        // Also test ares_set_servers_ports_csv
        ares_set_servers_ports_csv(channel, csv_input.c_str());
        
        // Test ares_get_servers_csv
        char* csv_output = ares_get_servers_csv(channel);
        if (csv_output != NULL) {
            ares_free_string(csv_output);
        }
        
    } else {
        // config_type == 3: Test multiple configuration calls
        // Perform multiple set/get operations to test state changes
        
        // First configuration
        struct ares_addr_node* servers1 = NULL;
        if (fdp.remaining_bytes() >= 4) {
            struct ares_addr_node* node = 
                (struct ares_addr_node*)malloc(sizeof(struct ares_addr_node));
            if (node != NULL) {
                memset(node, 0, sizeof(struct ares_addr_node));
                node->family = AF_INET;
                node->addr.addr4.s_addr = htonl(0x7f000001); // 127.0.0.1
                node->next = NULL;
                servers1 = node;
                
                ares_set_servers(channel, servers1);
                free(node);
            }
        }
        
        // Retrieve first configuration
        struct ares_addr_node* retrieved1 = NULL;
        ares_get_servers(channel, &retrieved1);
        if (retrieved1 != NULL) {
            ares_free_data(retrieved1);
        }
        
        // Second configuration (different servers)
        struct ares_addr_port_node* servers2 = NULL;
        if (fdp.remaining_bytes() >= 6) {
            struct ares_addr_port_node* node = 
                (struct ares_addr_port_node*)malloc(sizeof(struct ares_addr_port_node));
            if (node != NULL) {
                memset(node, 0, sizeof(struct ares_addr_port_node));
                node->family = AF_INET6;
                memset(&node->addr.addr6, 0, sizeof(struct ares_in6_addr));
                node->addr.addr6._S6_un._S6_u8[15] = 1; // ::1
                node->udp_port = 53;
                node->tcp_port = 53;
                node->next = NULL;
                servers2 = node;
                
                ares_set_servers_ports(channel, servers2);
                free(node);
            }
        }
        
        // Retrieve second configuration
        struct ares_addr_port_node* retrieved2 = NULL;
        ares_get_servers_ports(channel, &retrieved2);
        if (retrieved2 != NULL) {
            ares_free_data(retrieved2);
        }
        
        // Third configuration (empty)
        ares_set_servers(channel, NULL);
        
        // Retrieve empty configuration
        struct ares_addr_node* retrieved3 = NULL;
        ares_get_servers(channel, &retrieved3);
        if (retrieved3 != NULL) {
            ares_free_data(retrieved3);
        }
    }
    
    // Step 5: Cleanup
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_003

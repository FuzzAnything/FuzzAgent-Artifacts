#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "ares.h"
#include "ares_nameser.h"

namespace harness_005 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // Initialize channel - also test ares_init_options for coverage
    ares_channel_t* channel = NULL;
    int init_result;
    
    // Randomly choose between ares_init and ares_init_options
    bool use_options = fdp.ConsumeBool();
    if (use_options && fdp.remaining_bytes() > 4) {
        // Test ares_init_options
        ares_options options;
        memset(&options, 0, sizeof(options));
        options.flags = fdp.ConsumeIntegral<int>();
        options.timeout = fdp.ConsumeIntegralInRange<int>(1000, 10000);
        options.tries = fdp.ConsumeIntegralInRange<int>(1, 10);
        
        int optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
        init_result = ares_init_options(&channel, &options, optmask);
    } else {
        // Test regular ares_init
        init_result = ares_init(&channel);
    }
    
    if (init_result != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Test 1: ares_set_servers_csv (156 branches) and ares_get_servers_csv (25 branches)
    if (fdp.remaining_bytes() > 20) {
        // Generate server CSV string from fuzzer input
        uint8_t server_count = fdp.ConsumeIntegralInRange<uint8_t>(1, 8);
        std::string csv_string;
        
        for (uint8_t i = 0; i < server_count && fdp.remaining_bytes() > 10; i++) {
            if (i > 0) {
                csv_string += ",";
            }
            
            // Decide server type from fuzzer input
            uint8_t server_type = fdp.ConsumeIntegral<uint8_t>() % 6;
            
            switch (server_type) {
                case 0: { // Simple IPv4
                    uint8_t ip[4];
                    for (int j = 0; j < 4; j++) {
                        ip[j] = fdp.ConsumeIntegral<uint8_t>();
                    }
                    char ip_str[16];
                    snprintf(ip_str, sizeof(ip_str), "%d.%d.%d.%d", 
                             ip[0], ip[1], ip[2], ip[3]);
                    csv_string += ip_str;
                    break;
                }
                case 1: { // IPv4 with port
                    uint8_t ip[4];
                    for (int j = 0; j < 4; j++) {
                        ip[j] = fdp.ConsumeIntegral<uint8_t>();
                    }
                    uint16_t port = fdp.ConsumeIntegralInRange<uint16_t>(1, 65535);
                    char server_str[32];
                    snprintf(server_str, sizeof(server_str), "%d.%d.%d.%d:%d", 
                             ip[0], ip[1], ip[2], ip[3], port);
                    csv_string += server_str;
                    break;
                }
                case 2: { // IPv6 (full format from fuzzer)
                    // Generate 8 groups of 2 hex chars each
                    std::string ipv6;
                    for (int j = 0; j < 8 && fdp.remaining_bytes() > 2; j++) {
                        if (j > 0) ipv6 += ":";
                        uint16_t group = fdp.ConsumeIntegral<uint16_t>() % 65536;
                        char group_str[5];
                        snprintf(group_str, sizeof(group_str), "%04x", group);
                        // Remove leading zeros for realistic IPv6 format
                        int k = 0;
                        while (group_str[k] == '0' && k < 3) k++;
                        ipv6 += &group_str[k];
                    }
                    csv_string += "[" + ipv6 + "]";
                    break;
                }
                case 3: { // IPv6 with port
                    std::string ipv6;
                    for (int j = 0; j < 8 && fdp.remaining_bytes() > 2; j++) {
                        if (j > 0) ipv6 += ":";
                        uint16_t group = fdp.ConsumeIntegral<uint16_t>() % 65536;
                        char group_str[5];
                        snprintf(group_str, sizeof(group_str), "%04x", group);
                        int k = 0;
                        while (group_str[k] == '0' && k < 3) k++;
                        ipv6 += &group_str[k];
                    }
                    uint16_t port = fdp.ConsumeIntegralInRange<uint16_t>(1, 65535);
                    csv_string += "[" + ipv6 + "]:" + std::to_string(port);
                    break;
                }
                case 4: { // IPv6 compressed format (::)
                    std::string ipv6;
                    int zero_pos = fdp.ConsumeIntegralInRange<int>(0, 7);
                    int zero_len = fdp.ConsumeIntegralInRange<int>(1, 6);
                    
                    for (int j = 0; j < 8 && fdp.remaining_bytes() > 2; j++) {
                        if (j == zero_pos) {
                            ipv6 += ":";
                            if (j == 0) ipv6 += ":";
                            j += zero_len - 1;
                            continue;
                        }
                        if (j > 0 && (j != zero_pos + zero_len)) ipv6 += ":";
                        uint16_t group = fdp.ConsumeIntegral<uint16_t>() % 65536;
                        if (group != 0 || (j == 7 && ipv6.find("::") == std::string::npos)) {
                            char group_str[5];
                            snprintf(group_str, sizeof(group_str), "%04x", group);
                            int k = 0;
                            while (group_str[k] == '0' && k < 3) k++;
                            ipv6 += &group_str[k];
                        }
                    }
                    csv_string += ipv6;
                    break;
                }
                case 5: { // Invalid format (to test error paths)
                    size_t invalid_len = fdp.ConsumeIntegralInRange<size_t>(1, 15);
                    std::string invalid = fdp.ConsumeRandomLengthString(invalid_len);
                    // Replace commas to keep CSV structure but allow other invalid chars
                    for (char& c : invalid) {
                        if (c == ',') c = '.';
                    }
                    csv_string += invalid;
                    break;
                }
            }
        }
        
        // Test ares_set_servers_csv
        int set_result = ares_set_servers_csv(channel, csv_string.c_str());
        (void)set_result; // Result can be success or error depending on input
        
        // Test ares_get_servers_csv verification
        char* retrieved_csv = ares_get_servers_csv(channel);
        if (retrieved_csv != NULL) {
            // Store length for coverage
            size_t csv_len = strlen(retrieved_csv);
            (void)csv_len;
            ares_free_string(retrieved_csv);
        }
    }
    
    // Test 2: ares_set_servers_ports_csv (156 branches) and verification
    if (fdp.remaining_bytes() > 20) {
        // Build ports CSV string with always port specification
        uint8_t server_count = fdp.ConsumeIntegralInRange<uint8_t>(1, 5);
        std::string ports_csv_string;
        
        for (uint8_t i = 0; i < server_count && fdp.remaining_bytes() > 8; i++) {
            if (i > 0) {
                ports_csv_string += ",";
            }
            
            uint8_t addr_type = fdp.ConsumeIntegral<uint8_t>() % 3; // 0=IPv4, 1=IPv6, 2=IPv6 compressed
            
            if (addr_type == 0) { // IPv4 with port
                uint8_t ip[4];
                for (int j = 0; j < 4; j++) {
                    ip[j] = fdp.ConsumeIntegral<uint8_t>();
                }
                uint16_t udp_port = fdp.ConsumeIntegralInRange<uint16_t>(1, 65535);
                uint16_t tcp_port = fdp.ConsumeIntegralInRange<uint16_t>(1, 65535);
                char server_str[64];
                snprintf(server_str, sizeof(server_str), "%d.%d.%d.%d:%d/%d", 
                         ip[0], ip[1], ip[2], ip[3], udp_port, tcp_port);
                ports_csv_string += server_str;
            } else { // IPv6 with port
                std::string ipv6;
                if (addr_type == 1) { // Full IPv6
                    for (int j = 0; j < 8 && fdp.remaining_bytes() > 2; j++) {
                        if (j > 0) ipv6 += ":";
                        uint16_t group = fdp.ConsumeIntegral<uint16_t>() % 65536;
                        char group_str[5];
                        snprintf(group_str, sizeof(group_str), "%04x", group);
                        int k = 0;
                        while (group_str[k] == '0' && k < 3) k++;
                        ipv6 += &group_str[k];
                    }
                } else { // Compressed IPv6
                    ipv6 = "::";
                    if (fdp.ConsumeBool()) {
                        ipv6 = "2001::";
                    }
                }
                
                uint16_t udp_port = fdp.ConsumeIntegralInRange<uint16_t>(1, 65535);
                uint16_t tcp_port = fdp.ConsumeIntegralInRange<uint16_t>(1, 65535);
                ports_csv_string += "[" + ipv6 + "]:" + std::to_string(udp_port) + 
                                   "/" + std::to_string(tcp_port);
            }
        }
        
        // Test ares_set_servers_ports_csv
        int set_ports_result = ares_set_servers_ports_csv(channel, ports_csv_string.c_str());
        (void)set_ports_result;
        
        // Get servers again to verify
        char* retrieved_ports_csv = ares_get_servers_csv(channel);
        if (retrieved_ports_csv != NULL) {
            size_t csv_len = strlen(retrieved_ports_csv);
            (void)csv_len;
            ares_free_string(retrieved_ports_csv);
        }
    }
    
    // Test 3: Struct-based APIs for coverage (ares_set_servers, ares_set_servers_ports, 
    // ares_get_servers, ares_get_servers_ports)
    if (fdp.remaining_bytes() > 50) {
        // Test struct-based APIs with dynamically created server lists
        
        // Create server list for ares_set_servers (160 branches)
        uint8_t struct_server_count = fdp.ConsumeIntegralInRange<uint8_t>(0, 4);
        struct ares_addr_node* server_list = NULL;
        struct ares_addr_node* prev_node = NULL;
        
        for (uint8_t i = 0; i < struct_server_count && fdp.remaining_bytes() > 6; i++) {
            struct ares_addr_node* node = (struct ares_addr_node*)malloc(sizeof(struct ares_addr_node));
            if (node == NULL) break;
            
            memset(node, 0, sizeof(struct ares_addr_node));
            
            node->family = fdp.ConsumeBool() ? AF_INET : AF_INET6;
            if (node->family == AF_INET) {
                node->addr.addr4.s_addr = fdp.ConsumeIntegral<uint32_t>();
            } else {
                // For IPv6, consume 4 uint32_t for the 128-bit address
                for (int j = 0; j < 4; j++) {
                    ((uint32_t*)&node->addr.addr6)[j] = fdp.ConsumeIntegral<uint32_t>();
                }
            }
            node->next = NULL;
            
            if (prev_node == NULL) {
                server_list = node;
            } else {
                prev_node->next = node;
            }
            prev_node = node;
        }
        
        // Test ares_set_servers
        if (server_list != NULL) {
            int struct_set_result = ares_set_servers(channel, server_list);
            (void)struct_set_result;
            
            // Test ares_get_servers (18 branches)
            struct ares_addr_node* retrieved_servers = NULL;
            int get_struct_result = ares_get_servers(channel, &retrieved_servers);
            if (get_struct_result == ARES_SUCCESS && retrieved_servers != NULL) {
                // Free the retrieved list (allocated by ares_get_servers)
                ares_free_data(retrieved_servers);
            }
            
            // Free our original list (allocated by malloc)
            struct ares_addr_node* current = server_list;
            while (current != NULL) {
                struct ares_addr_node* next = current->next;
                free(current);
                current = next;
            }
        }
        
        // Test ares_set_servers_ports and ares_get_servers_ports if enough data
        if (fdp.remaining_bytes() > 30) {
            uint8_t port_server_count = fdp.ConsumeIntegralInRange<uint8_t>(0, 3);
            struct ares_addr_port_node* port_server_list = NULL;
            struct ares_addr_port_node* port_prev_node = NULL;
            
            for (uint8_t i = 0; i < port_server_count && fdp.remaining_bytes() > 10; i++) {
                struct ares_addr_port_node* node = (struct ares_addr_port_node*)malloc(sizeof(struct ares_addr_port_node));
                if (node == NULL) break;
                
                memset(node, 0, sizeof(struct ares_addr_port_node));
                
                node->family = fdp.ConsumeBool() ? AF_INET : AF_INET6;
                if (node->family == AF_INET) {
                    node->addr.addr4.s_addr = fdp.ConsumeIntegral<uint32_t>();
                } else {
                    for (int j = 0; j < 4; j++) {
                        ((uint32_t*)&node->addr.addr6)[j] = fdp.ConsumeIntegral<uint32_t>();
                    }
                }
                node->udp_port = fdp.ConsumeIntegralInRange<int>(0, 65535);
                node->tcp_port = fdp.ConsumeIntegralInRange<int>(0, 65535);
                node->next = NULL;
                
                if (port_prev_node == NULL) {
                    port_server_list = node;
                } else {
                    port_prev_node->next = node;
                }
                port_prev_node = node;
            }
            
            // Test ares_set_servers_ports (160 branches)
            if (port_server_list != NULL) {
                int set_ports_struct_result = ares_set_servers_ports(channel, port_server_list);
                (void)set_ports_struct_result;
                
                // Test ares_get_servers_ports (18 branches)
                struct ares_addr_port_node* retrieved_port_servers = NULL;
                int get_ports_result = ares_get_servers_ports(channel, &retrieved_port_servers);
                if (get_ports_result == ARES_SUCCESS && retrieved_port_servers != NULL) {
                    // Free the retrieved list (allocated by ares_get_servers_ports)
                    ares_free_data(retrieved_port_servers);
                }
                
                
                // Free our original port server list
                struct ares_addr_port_node* current = port_server_list;
                while (current != NULL) {
                    struct ares_addr_port_node* next = current->next;
                    free(current);
                    current = next;
                }
            }
        }
    }
    
    // Clean up as per invocation sequence
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_005

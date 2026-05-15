// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_free_data at ares_data.c:46:6 in ares.h
// ares_free_data at ares_data.c:46:6 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_set_servers_csv at ares_update_servers.c:1304:5 in ares.h
// ares_get_servers at ares_update_servers.c:1109:5 in ares.h
// ares_set_servers_ports_csv at ares_update_servers.c:1309:5 in ares.h
// ares_get_servers_ports at ares_update_servers.c:1164:5 in ares.h
// ares_set_servers at ares_update_servers.c:1221:5 in ares.h
// ares_set_servers_ports at ares_update_servers.c:1245:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void free_addr_node_list(struct ares_addr_node *list) {
    struct ares_addr_node *current = list;
    while (current) {
        struct ares_addr_node *next = current->next;
        ares_free_data(current);
        current = next;
    }
}

static void free_addr_port_node_list(struct ares_addr_port_node *list) {
    struct ares_addr_port_node *current = list;
    while (current) {
        struct ares_addr_port_node *next = current->next;
        ares_free_data(current);
        current = next;
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize channel
    ares_channel_t *channel = NULL;
    int status = ares_init(&channel);
    if (status != ARES_SUCCESS) {
        return 0;
    }
    
    // Create null-terminated string for CSV functions
    char *csv_str = NULL;
    if (Size > 0) {
        csv_str = (char*)malloc(Size + 1);
        if (csv_str) {
            memcpy(csv_str, Data, Size);
            csv_str[Size] = '\0';
        }
    }
    
    // Test ares_set_servers_csv
    if (csv_str) {
        ares_set_servers_csv(channel, csv_str);
    }
    
    // Test ares_get_servers (deprecated)
    struct ares_addr_node *servers = NULL;
    ares_get_servers(channel, &servers);
    if (servers) {
        free_addr_node_list(servers);
    }
    
    // Test ares_set_servers_ports_csv
    if (csv_str) {
        ares_set_servers_ports_csv(channel, csv_str);
    }
    
    // Test ares_get_servers_ports (deprecated)
    struct ares_addr_port_node *servers_ports = NULL;
    ares_get_servers_ports(channel, &servers_ports);
    if (servers_ports) {
        free_addr_port_node_list(servers_ports);
    }
    
    // Test ares_set_servers (deprecated) - create simple server node
    if (Size >= sizeof(struct in_addr)) {
        struct ares_addr_node *server_node = (struct ares_addr_node*)malloc(sizeof(struct ares_addr_node));
        if (server_node) {
            server_node->next = NULL;
            server_node->family = AF_INET;
            memcpy(&server_node->addr.addr4, Data, sizeof(struct in_addr));
            ares_set_servers(channel, server_node);
            free(server_node);
        }
    }
    
    // Test ares_set_servers_ports (deprecated) - create simple server port node
    if (Size >= sizeof(struct in_addr) + 2*sizeof(int)) {
        struct ares_addr_port_node *server_port_node = (struct ares_addr_port_node*)malloc(sizeof(struct ares_addr_port_node));
        if (server_port_node) {
            server_port_node->next = NULL;
            server_port_node->family = AF_INET;
            memcpy(&server_port_node->addr.addr4, Data, sizeof(struct in_addr));
            server_port_node->udp_port = 53;
            server_port_node->tcp_port = 53;
            ares_set_servers_ports(channel, server_port_node);
            free(server_port_node);
        }
    }
    
    // Cleanup
    if (csv_str) {
        free(csv_str);
    }
    ares_destroy(channel);
    
    return 0;
}
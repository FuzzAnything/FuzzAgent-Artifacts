// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_get_servers at ares_update_servers.c:1109:5 in ares.h
// ares_free_data at ares_data.c:46:6 in ares.h
// ares_get_servers_ports at ares_update_servers.c:1164:5 in ares.h
// ares_free_data at ares_data.c:46:6 in ares.h
// ares_set_servers at ares_update_servers.c:1221:5 in ares.h
// ares_set_servers_ports at ares_update_servers.c:1245:5 in ares.h
// ares_get_servers at ares_update_servers.c:1109:5 in ares.h
// ares_free_data at ares_data.c:46:6 in ares.h
// ares_get_servers_ports at ares_update_servers.c:1164:5 in ares.h
// ares_free_data at ares_data.c:46:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_set_servers_csv at ares_update_servers.c:1304:5 in ares.h
// ares_set_servers_ports_csv at ares_update_servers.c:1309:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void free_addr_list(struct ares_addr_node *list) {
    while (list) {
        struct ares_addr_node *next = list->next;
        free(list);
        list = next;
    }
}

static void free_addr_port_list(struct ares_addr_port_node *list) {
    while (list) {
        struct ares_addr_port_node *next = list->next;
        free(list);
        list = next;
    }
}

static struct ares_addr_node *create_test_addr_list(const uint8_t *data, size_t size) {
    if (size < sizeof(struct ares_addr_node)) return NULL;
    
    struct ares_addr_node *node = malloc(sizeof(struct ares_addr_node));
    if (!node) return NULL;
    
    memset(node, 0, sizeof(struct ares_addr_node));
    node->family = (size % 2) ? AF_INET : AF_INET6;
    
    if (node->family == AF_INET && size >= 4) {
        memcpy(&node->addr.addr4, data, 4);
    } else if (node->family == AF_INET6 && size >= 16) {
        memcpy(&node->addr.addr6, data, 16);
    }
    
    node->next = NULL;
    return node;
}

static struct ares_addr_port_node *create_test_addr_port_list(const uint8_t *data, size_t size) {
    if (size < sizeof(struct ares_addr_port_node)) return NULL;
    
    struct ares_addr_port_node *node = malloc(sizeof(struct ares_addr_port_node));
    if (!node) return NULL;
    
    memset(node, 0, sizeof(struct ares_addr_port_node));
    node->family = (size % 2) ? AF_INET : AF_INET6;
    
    if (node->family == AF_INET && size >= 4) {
        memcpy(&node->addr.addr4, data, 4);
    } else if (node->family == AF_INET6 && size >= 16) {
        memcpy(&node->addr.addr6, data, 16);
    }
    
    if (size >= sizeof(int)) {
        node->udp_port = *(int*)data % 65536;
        node->tcp_port = *(int*)data % 65536;
    }
    
    node->next = NULL;
    return node;
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    ares_channel_t *channel = NULL;
    int status = ares_init(&channel);
    if (status != ARES_SUCCESS || !channel) return 0;
    
    // Test ares_set_servers_csv
    if (Size > 0) {
        char *csv = malloc(Size + 1);
        if (csv) {
            memcpy(csv, Data, Size);
            csv[Size] = '\0';
            ares_set_servers_csv(channel, csv);
            free(csv);
        }
    }
    
    // Test ares_set_servers_ports_csv
    if (Size > 0) {
        char *csv2 = malloc(Size + 1);
        if (csv2) {
            memcpy(csv2, Data, Size);
            csv2[Size] = '\0';
            ares_set_servers_ports_csv(channel, csv2);
            free(csv2);
        }
    }
    
    // Test ares_get_servers
    struct ares_addr_node *servers = NULL;
    ares_get_servers(channel, &servers);
    if (servers) {
        ares_free_data(servers);
    }
    
    // Test ares_get_servers_ports
    struct ares_addr_port_node *servers_ports = NULL;
    ares_get_servers_ports(channel, &servers_ports);
    if (servers_ports) {
        ares_free_data(servers_ports);
    }
    
    // Test ares_set_servers with generated list
    struct ares_addr_node *addr_list = create_test_addr_list(Data, Size);
    if (addr_list) {
        ares_set_servers(channel, addr_list);
        free_addr_list(addr_list);
    }
    
    // Test ares_set_servers_ports with generated list
    struct ares_addr_port_node *addr_port_list = create_test_addr_port_list(Data, Size);
    if (addr_port_list) {
        ares_set_servers_ports(channel, addr_port_list);
        free_addr_port_list(addr_port_list);
    }
    
    // Test getters again after setting
    servers = NULL;
    ares_get_servers(channel, &servers);
    if (servers) {
        ares_free_data(servers);
    }
    
    servers_ports = NULL;
    ares_get_servers_ports(channel, &servers_ports);
    if (servers_ports) {
        ares_free_data(servers_ports);
    }
    
    ares_destroy(channel);
    return 0;
}
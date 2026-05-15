// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_set_servers at ares_update_servers.c:1221:5 in ares.h
// ares_set_servers_csv at ares_update_servers.c:1304:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_get_servers at ares_update_servers.c:1109:5 in ares.h
// ares_free_data at ares_data.c:46:6 in ares.h
// ares_get_servers_ports at ares_update_servers.c:1164:5 in ares.h
// ares_free_data at ares_data.c:46:6 in ares.h
// ares_set_servers_ports at ares_update_servers.c:1245:5 in ares.h
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

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    // Initialize a channel for testing
    ares_channel_t *channel;
    int status = ares_init(&channel);
    if (status != ARES_SUCCESS) {
        return 0;
    }

    // Use first byte to determine which API to test
    if (Size > 0) {
        uint8_t api_selector = Data[0] % 6;
        const uint8_t *remaining_data = Data + 1;
        size_t remaining_size = Size > 1 ? Size - 1 : 0;

        switch (api_selector) {
            case 0: {
                // Test ares_get_servers
                struct ares_addr_node *servers = NULL;
                int result = ares_get_servers(channel, &servers);
                if (result == ARES_SUCCESS && servers != NULL) {
                    // Use ares_free_data to free the linked list
                    struct ares_addr_node *current = servers;
                    while (current != NULL) {
                        struct ares_addr_node *next = current->next;
                        ares_free_data(current);
                        current = next;
                    }
                }
                break;
            }
            case 1: {
                // Test ares_get_servers_ports
                struct ares_addr_port_node *servers = NULL;
                int result = ares_get_servers_ports(channel, &servers);
                if (result == ARES_SUCCESS && servers != NULL) {
                    // Use ares_free_data to free the linked list
                    struct ares_addr_port_node *current = servers;
                    while (current != NULL) {
                        struct ares_addr_port_node *next = current->next;
                        ares_free_data(current);
                        current = next;
                    }
                }
                break;
            }
            case 2: {
                // Test ares_set_servers_ports
                // Build a simple linked list from fuzzer data
                struct ares_addr_port_node *servers = NULL;
                if (remaining_size >= sizeof(struct ares_addr_port_node)) {
                    servers = (struct ares_addr_port_node*)malloc(sizeof(struct ares_addr_port_node));
                    if (servers != NULL) {
                        memset(servers, 0, sizeof(struct ares_addr_port_node));
                        // Use fuzzer data to populate fields (with bounds checking)
                        size_t copy_size = remaining_size < sizeof(struct ares_addr_port_node) ? 
                                          remaining_size : sizeof(struct ares_addr_port_node);
                        memcpy(servers, remaining_data, copy_size);
                        servers->next = NULL;
                        // Ensure family field is valid
                        if (servers->family != AF_INET && servers->family != AF_INET6) {
                            servers->family = AF_INET;
                        }
                        ares_set_servers_ports(channel, servers);
                        free(servers);
                    }
                }
                break;
            }
            case 3: {
                // Test ares_set_servers_ports_csv
                if (remaining_size > 0) {
                    char *csv_str = (char*)malloc(remaining_size + 1);
                    if (csv_str != NULL) {
                        memcpy(csv_str, remaining_data, remaining_size);
                        csv_str[remaining_size] = '\0';
                        ares_set_servers_ports_csv(channel, csv_str);
                        free(csv_str);
                    }
                }
                break;
            }
            case 4: {
                // Test ares_set_servers
                // Build a simple linked list from fuzzer data
                struct ares_addr_node *servers = NULL;
                if (remaining_size >= sizeof(struct ares_addr_node)) {
                    servers = (struct ares_addr_node*)malloc(sizeof(struct ares_addr_node));
                    if (servers != NULL) {
                        memset(servers, 0, sizeof(struct ares_addr_node));
                        // Use fuzzer data to populate fields (with bounds checking)
                        size_t copy_size = remaining_size < sizeof(struct ares_addr_node) ? 
                                          remaining_size : sizeof(struct ares_addr_node);
                        memcpy(servers, remaining_data, copy_size);
                        servers->next = NULL;
                        // Ensure family field is valid
                        if (servers->family != AF_INET && servers->family != AF_INET6) {
                            servers->family = AF_INET;
                        }
                        ares_set_servers(channel, servers);
                        free(servers);
                    }
                }
                break;
            }
            case 5: {
                // Test ares_set_servers_csv
                if (remaining_size > 0) {
                    char *csv_str = (char*)malloc(remaining_size + 1);
                    if (csv_str != NULL) {
                        memcpy(csv_str, remaining_data, remaining_size);
                        csv_str[remaining_size] = '\0';
                        ares_set_servers_csv(channel, csv_str);
                        free(csv_str);
                    }
                }
                break;
            }
        }
    }

    // Clean up
    ares_destroy(channel);
    
    return 0;
}
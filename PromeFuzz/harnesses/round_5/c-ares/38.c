// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_set_servers_csv at ares_update_servers.c:1304:5 in ares.h
// ares_set_sortlist at ares_init.c:581:5 in ares.h
// ares_set_servers_ports at ares_update_servers.c:1245:5 in ares.h
// ares_save_options at ares_options.c:83:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

static int get_int_from_data(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) {
        return 0;
    }
    int value;
    memcpy(&value, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return value;
}

static unsigned short get_ushort_from_data(const uint8_t **data, size_t *size) {
    if (*size < sizeof(unsigned short)) {
        return 0;
    }
    unsigned short value;
    memcpy(&value, *data, sizeof(unsigned short));
    *data += sizeof(unsigned short);
    *size -= sizeof(unsigned short);
    return value;
}

static size_t get_size_t_from_data(const uint8_t **data, size_t *size) {
    if (*size < sizeof(size_t)) {
        return 0;
    }
    size_t value;
    memcpy(&value, *data, sizeof(size_t));
    *data += sizeof(size_t);
    *size -= sizeof(size_t);
    return value;
}

static void create_random_string(char *buffer, size_t max_len, const uint8_t **data, size_t *size) {
    size_t len = *size < max_len ? *size : max_len;
    if (len > 0) {
        memcpy(buffer, *data, len);
        *data += len;
        *size -= len;
    }
    buffer[len] = '\0';
}

static void populate_ares_options(struct ares_options *options, const uint8_t **data, size_t *size) {
    memset(options, 0, sizeof(struct ares_options));
    
    if (*size < sizeof(struct ares_options)) {
        return;
    }
    
    options->flags = get_int_from_data(data, size);
    options->timeout = get_int_from_data(data, size);
    options->tries = get_int_from_data(data, size);
    options->ndots = get_int_from_data(data, size);
    options->udp_port = get_ushort_from_data(data, size);
    options->tcp_port = get_ushort_from_data(data, size);
    options->socket_send_buffer_size = get_int_from_data(data, size);
    options->socket_receive_buffer_size = get_int_from_data(data, size);
    options->nservers = get_int_from_data(data, size);
    options->ndomains = get_int_from_data(data, size);
    options->nsort = get_int_from_data(data, size);
    options->ednspsz = get_int_from_data(data, size);
    options->udp_max_queries = get_int_from_data(data, size);
    options->maxtimeout = get_int_from_data(data, size);
    options->qcache_max_ttl = get_int_from_data(data, size);
    options->evsys = (ares_evsys_t)get_int_from_data(data, size);
    options->server_failover_opts.retry_chance = get_ushort_from_data(data, size);
    options->server_failover_opts.retry_delay = get_size_t_from_data(data, size);
}

static struct ares_addr_port_node *create_server_node(const uint8_t **data, size_t *size) {
    if (*size < sizeof(struct ares_addr_port_node)) {
        return NULL;
    }
    
    struct ares_addr_port_node *node = malloc(sizeof(struct ares_addr_port_node));
    if (!node) {
        return NULL;
    }
    
    memset(node, 0, sizeof(struct ares_addr_port_node));
    node->next = NULL;
    node->family = get_int_from_data(data, size);
    
    if (node->family == AF_INET) {
        if (*size >= sizeof(struct in_addr)) {
            memcpy(&node->addr.addr4, *data, sizeof(struct in_addr));
            *data += sizeof(struct in_addr);
            *size -= sizeof(struct in_addr);
        }
    } else if (node->family == AF_INET6) {
        if (*size >= sizeof(struct ares_in6_addr)) {
            memcpy(&node->addr.addr6, *data, sizeof(struct ares_in6_addr));
            *data += sizeof(struct ares_in6_addr);
            *size -= sizeof(struct ares_in6_addr);
        }
    }
    
    node->udp_port = get_int_from_data(data, size);
    node->tcp_port = get_int_from_data(data, size);
    
    return node;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    const uint8_t *data_ptr = Data;
    size_t remaining_size = Size;
    
    populate_ares_options(&options, &data_ptr, &remaining_size);
    
    optmask = get_int_from_data(&data_ptr, &remaining_size);
    
    int init_result = ares_init_options(&channel, &options, optmask);
    if (init_result != ARES_SUCCESS) {
        return 0;
    }
    
    char servers_csv[256];
    char sortlist_str[256];
    
    create_random_string(servers_csv, sizeof(servers_csv) - 1, &data_ptr, &remaining_size);
    create_random_string(sortlist_str, sizeof(sortlist_str) - 1, &data_ptr, &remaining_size);
    
    ares_set_servers_csv(channel, servers_csv);
    ares_set_sortlist(channel, sortlist_str);
    
    struct ares_addr_port_node *server_node = create_server_node(&data_ptr, &remaining_size);
    if (server_node) {
        ares_set_servers_ports(channel, server_node);
        free(server_node);
    }
    
    struct ares_options saved_options;
    int saved_optmask;
    ares_save_options(channel, &saved_options, &saved_optmask);
    
    ares_destroy(channel);
    
    return 0;
}
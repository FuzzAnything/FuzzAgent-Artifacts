// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init at ares_init.c:67:5 in ares.h
// ares_save_options at ares_options.c:83:5 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_set_servers_csv at ares_update_servers.c:1304:5 in ares.h
// ares_set_servers_ports at ares_update_servers.c:1245:5 in ares.h
// ares_get_servers_ports at ares_update_servers.c:1164:5 in ares.h
// ares_set_socket_functions_ex at ares_set_socket_functions.c:89:3 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_sock_state_cb(void *data, ares_socket_t socket_fd, int readable, int writable) {
    (void)data;
    (void)socket_fd;
    (void)readable;
    (void)writable;
}

static ares_socket_t dummy_asocket(int domain, int type, int protocol, void *user_data) {
    (void)domain;
    (void)type;
    (void)protocol;
    (void)user_data;
    return ARES_SOCKET_BAD;
}

static int dummy_aclose(ares_socket_t sock, void *user_data) {
    (void)sock;
    (void)user_data;
    return -1;
}

static int dummy_aconnect(ares_socket_t sock, const struct sockaddr *address,
                          ares_socklen_t address_len, unsigned int flags, void *user_data) {
    (void)sock;
    (void)address;
    (void)address_len;
    (void)flags;
    (void)user_data;
    return -1;
}

static ares_ssize_t dummy_arecvfrom(ares_socket_t sock, void *buffer, size_t length,
                                    int flags, struct sockaddr *address,
                                    ares_socklen_t *address_len, void *user_data) {
    (void)sock;
    (void)buffer;
    (void)length;
    (void)flags;
    (void)address;
    (void)address_len;
    (void)user_data;
    return -1;
}

static ares_ssize_t dummy_asendto(ares_socket_t sock, const void *buffer, size_t length,
                                  int flags, const struct sockaddr *address,
                                  ares_socklen_t address_len, void *user_data) {
    (void)sock;
    (void)buffer;
    (void)length;
    (void)flags;
    (void)address;
    (void)address_len;
    (void)user_data;
    return -1;
}

static int dummy_asetsockopt(ares_socket_t sock, ares_socket_opt_t opt, const void *val,
                             ares_socklen_t val_size, void *user_data) {
    (void)sock;
    (void)opt;
    (void)val;
    (void)val_size;
    (void)user_data;
    return -1;
}

static int dummy_agetsockname(ares_socket_t sock, struct sockaddr *address,
                              ares_socklen_t *address_len, void *user_data) {
    (void)sock;
    (void)address;
    (void)address_len;
    (void)user_data;
    return -1;
}

static int dummy_abind(ares_socket_t sock, unsigned int flags,
                       const struct sockaddr *address, socklen_t address_len,
                       void *user_data) {
    (void)sock;
    (void)flags;
    (void)address;
    (void)address_len;
    (void)user_data;
    return -1;
}

static unsigned int dummy_aif_nametoindex(const char *ifname, void *user_data) {
    (void)ifname;
    (void)user_data;
    return 0;
}

static const char *dummy_aif_indextoname(unsigned int ifindex, char *ifname_buf,
                                         size_t ifname_buf_len, void *user_data) {
    (void)ifindex;
    (void)ifname_buf;
    (void)ifname_buf_len;
    (void)user_data;
    return NULL;
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    ares_channel_t *channel = NULL;
    ares_channel_t *channel2 = NULL;
    struct ares_options options;
    int optmask = 0;
    int result;
    struct ares_addr_port_node *servers = NULL;
    struct ares_addr_port_node *server_list = NULL;
    struct ares_addr_port_node *tmp_node = NULL;
    char *csv_servers = NULL;
    size_t csv_len;
    uint8_t *fuzz_data = NULL;
    size_t fuzz_size;
    int i;

    /* Test ares_init */
    result = ares_init(&channel);
    if (result != ARES_SUCCESS) {
        channel = NULL;
    }

    /* Test ares_save_options if channel was created */
    if (channel != NULL) {
        (void)ares_save_options(channel, &options, &optmask);
    }

    /* Test ares_init_options with various optmask values */
    for (i = 0; i < 8; i++) {
        int test_optmask = (Data[0] << i) & 0xFF;
        struct ares_options test_options;
        
        memset(&test_options, 0, sizeof(test_options));
        
        if (test_optmask & ARES_OPT_FLAGS) {
            test_options.flags = (Data[1 % Size] << 8) | Data[2 % Size];
        }
        if (test_optmask & ARES_OPT_TIMEOUTMS) {
            test_options.timeout = (Data[3 % Size] << 8) | Data[4 % Size];
        }
        if (test_optmask & ARES_OPT_TRIES) {
            test_options.tries = Data[5 % Size];
        }
        if (test_optmask & ARES_OPT_NDOTS) {
            test_options.ndots = Data[6 % Size];
        }
        if (test_optmask & ARES_OPT_UDP_PORT) {
            test_options.udp_port = (Data[7 % Size] << 8) | Data[8 % Size];
        }
        if (test_optmask & ARES_OPT_TCP_PORT) {
            test_options.tcp_port = (Data[9 % Size] << 8) | Data[10 % Size];
        }
        if (test_optmask & ARES_OPT_SOCK_STATE_CB) {
            test_options.sock_state_cb = dummy_sock_state_cb;
            test_options.sock_state_cb_data = (void *)Data;
        }
        
        result = ares_init_options(&channel2, &test_options, test_optmask);
        if (result == ARES_SUCCESS && channel2 != NULL) {
            ares_destroy(channel2);
            channel2 = NULL;
        }
    }

    /* Test ares_set_servers_csv if we have a channel */
    if (channel != NULL && Size > 0) {
        csv_len = Size > 256 ? 256 : Size;
        csv_servers = (char *)malloc(csv_len + 1);
        if (csv_servers != NULL) {
            memcpy(csv_servers, Data, csv_len);
            csv_servers[csv_len] = '\0';
            
            /* Replace non-printable characters with commas or colons to form a valid-ish CSV */
            for (size_t j = 0; j < csv_len; j++) {
                if (csv_servers[j] < 32 || csv_servers[j] > 126) {
                    if (j % 3 == 0) {
                        csv_servers[j] = ',';
                    } else if (j % 3 == 1) {
                        csv_servers[j] = ':';
                    } else {
                        csv_servers[j] = '.';
                    }
                }
            }
            
            (void)ares_set_servers_csv(channel, csv_servers);
            free(csv_servers);
        }
    }

    /* Test ares_set_servers_ports and ares_get_servers_ports if we have a channel */
    if (channel != NULL && Size >= sizeof(struct ares_addr_port_node)) {
        /* Create a simple server list */
        server_list = (struct ares_addr_port_node *)malloc(sizeof(struct ares_addr_port_node));
        if (server_list != NULL) {
            memset(server_list, 0, sizeof(struct ares_addr_port_node));
            server_list->family = AF_INET;
            server_list->addr.addr4.s_addr = htonl(0x7f000001); /* 127.0.0.1 */
            server_list->udp_port = 53;
            server_list->tcp_port = 53;
            server_list->next = NULL;
            
            /* Try to set servers */
            (void)ares_set_servers_ports(channel, server_list);
            
            /* Try to get servers */
            (void)ares_get_servers_ports(channel, &servers);
            
            /* Free the server list we created */
            free(server_list);
            
            /* Free any servers returned by ares_get_servers_ports */
            if (servers != NULL) {
                tmp_node = servers;
                while (tmp_node != NULL) {
                    struct ares_addr_port_node *next = tmp_node->next;
                    free(tmp_node);
                    tmp_node = next;
                }
            }
        }
    }

    /* Test with custom socket functions */
    if (channel != NULL) {
        struct ares_socket_functions_ex sock_funcs;
        memset(&sock_funcs, 0, sizeof(sock_funcs));
        sock_funcs.version = 1;
        sock_funcs.asocket = dummy_asocket;
        sock_funcs.aclose = dummy_aclose;
        sock_funcs.aconnect = dummy_aconnect;
        sock_funcs.arecvfrom = dummy_arecvfrom;
        sock_funcs.asendto = dummy_asendto;
        sock_funcs.asetsockopt = dummy_asetsockopt;
        sock_funcs.agetsockname = dummy_agetsockname;
        sock_funcs.abind = dummy_abind;
        sock_funcs.aif_nametoindex = dummy_aif_nametoindex;
        sock_funcs.aif_indextoname = dummy_aif_indextoname;
        
        ares_set_socket_functions_ex(channel, &sock_funcs, NULL);
    }

    /* Cleanup */
    if (channel != NULL) {
        ares_destroy(channel);
    }
    if (channel2 != NULL) {
        ares_destroy(channel2);
    }

    return 0;
}
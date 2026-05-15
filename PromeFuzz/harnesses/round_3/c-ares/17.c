// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_set_socket_functions_ex at ares_set_socket_functions.c:89:3 in ares.h
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
// ares_getaddrinfo at ares_getaddrinfo.c:695:6 in ares.h
// ares_getaddrinfo at ares_getaddrinfo.c:695:6 in ares.h
// ares_gethostbyaddr at ares_gethostbyaddr.c:108:6 in ares.h
// ares_gethostbyaddr at ares_gethostbyaddr.c:108:6 in ares.h
// ares_getnameinfo at ares_getnameinfo.c:188:6 in ares.h
// ares_process_fd at ares_process.c:267:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static int pending_callbacks = 0;

static void dummy_addrinfo_callback(void *arg, int status, int timeouts, struct ares_addrinfo *res) {
    (void)arg;
    (void)status;
    (void)timeouts;
    if (res) {
        ares_freeaddrinfo(res);
    }
    pending_callbacks--;
}

static void dummy_host_callback(void *arg, int status, int timeouts, const struct hostent *hostent) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
    pending_callbacks--;
}

static void dummy_nameinfo_callback(void *arg, int status, int timeouts, const char *node, const char *service) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)node;
    (void)service;
    pending_callbacks--;
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
                         ares_socklen_t address_len, unsigned int flags,
                         void *user_data) {
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

static struct ares_socket_functions_ex dummy_socket_functions = {
    .version = 1,
    .flags = 0,
    .asocket = dummy_asocket,
    .aclose = dummy_aclose,
    .asetsockopt = dummy_asetsockopt,
    .aconnect = dummy_aconnect,
    .arecvfrom = dummy_arecvfrom,
    .asendto = dummy_asendto,
    .agetsockname = dummy_agetsockname,
    .abind = dummy_abind,
    .aif_nametoindex = dummy_aif_nametoindex,
    .aif_indextoname = dummy_aif_indextoname
};

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    int status;

    memset(&options, 0, sizeof(options));
    
    options.flags = ARES_FLAG_STAYOPEN;
    optmask |= ARES_OPT_FLAGS;
    
    options.timeout = 1000;
    optmask |= ARES_OPT_TIMEOUTMS;
    
    options.tries = 1;
    optmask |= ARES_OPT_TRIES;

    status = ares_init_options(&channel, &options, optmask);
    if (status != ARES_SUCCESS) {
        return 0;
    }

    ares_set_socket_functions_ex(channel, &dummy_socket_functions, NULL);

    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = ARES_AI_CANONNAME;

    size_t node_len = Size > 0 ? Size / 4 : 0;
    size_t service_len = Size > node_len ? (Size - node_len) / 4 : 0;
    size_t addr_len = Size > (node_len + service_len) ? Size - (node_len + service_len) : 0;

    char *node = NULL;
    char *service = NULL;
    void *addr = NULL;
    int family = AF_INET;
    struct sockaddr_in sa_in;
    struct sockaddr_in6 sa_in6;
    struct sockaddr *sa = NULL;
    ares_socklen_t salen = 0;

    if (node_len > 0) {
        node = (char *)malloc(node_len + 1);
        if (node) {
            memcpy(node, Data, node_len);
            node[node_len] = '\0';
        }
    }

    if (service_len > 0) {
        service = (char *)malloc(service_len + 1);
        if (service) {
            memcpy(service, Data + node_len, service_len);
            service[service_len] = '\0';
        }
    }

    if (addr_len > 0) {
        if (Data[node_len + service_len] & 0x01) {
            family = AF_INET6;
            if (addr_len >= sizeof(struct in6_addr)) {
                addr = malloc(sizeof(struct in6_addr));
                if (addr) {
                    memcpy(addr, Data + node_len + service_len, sizeof(struct in6_addr));
                }
            }
        } else {
            family = AF_INET;
            if (addr_len >= sizeof(struct in_addr)) {
                addr = malloc(sizeof(struct in_addr));
                if (addr) {
                    memcpy(addr, Data + node_len + service_len, sizeof(struct in_addr));
                }
            }
        }
    }

    if (family == AF_INET && addr_len >= sizeof(struct in_addr)) {
        memset(&sa_in, 0, sizeof(sa_in));
        sa_in.sin_family = AF_INET;
        sa_in.sin_port = htons(80);
        if (addr) {
            memcpy(&sa_in.sin_addr, addr, sizeof(struct in_addr));
        } else {
            sa_in.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        }
        sa = (struct sockaddr *)&sa_in;
        salen = sizeof(sa_in);
    } else if (family == AF_INET6 && addr_len >= sizeof(struct in6_addr)) {
        memset(&sa_in6, 0, sizeof(sa_in6));
        sa_in6.sin6_family = AF_INET6;
        sa_in6.sin6_port = htons(80);
        if (addr) {
            memcpy(&sa_in6.sin6_addr, addr, sizeof(struct in6_addr));
        } else {
            sa_in6.sin6_addr = in6addr_loopback;
        }
        sa = (struct sockaddr *)&sa_in6;
        salen = sizeof(sa_in6);
    } else {
        memset(&sa_in, 0, sizeof(sa_in));
        sa_in.sin_family = AF_INET;
        sa_in.sin_port = htons(80);
        sa_in.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        sa = (struct sockaddr *)&sa_in;
        salen = sizeof(sa_in);
    }

    pending_callbacks = 0;
    
    if (node) {
        pending_callbacks++;
        ares_getaddrinfo(channel, node, service, &hints, dummy_addrinfo_callback, NULL);
    } else {
        pending_callbacks++;
        ares_getaddrinfo(channel, "localhost", service, &hints, dummy_addrinfo_callback, NULL);
    }

    if (addr) {
        int addrlen = (family == AF_INET) ? sizeof(struct in_addr) : sizeof(struct in6_addr);
        pending_callbacks++;
        ares_gethostbyaddr(channel, addr, addrlen, family, dummy_host_callback, NULL);
    } else {
        struct in_addr default_addr;
        default_addr.s_addr = htonl(INADDR_LOOPBACK);
        pending_callbacks++;
        ares_gethostbyaddr(channel, &default_addr, sizeof(default_addr), AF_INET, dummy_host_callback, NULL);
    }

    if (sa) {
        pending_callbacks++;
        ares_getnameinfo(channel, sa, salen, 0, dummy_nameinfo_callback, NULL);
    }

    // Process pending callbacks
    while (pending_callbacks > 0) {
        ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    }

    ares_reinit(channel);

    if (node) {
        free(node);
    }
    if (service) {
        free(service);
    }
    if (addr) {
        free(addr);
    }

    ares_destroy(channel);
    return 0;
}
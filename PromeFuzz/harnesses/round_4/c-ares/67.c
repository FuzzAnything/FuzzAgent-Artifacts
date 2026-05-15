// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_threadsafety at ares_threads.c:632:13 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_set_socket_functions_ex at ares_set_socket_functions.c:89:3 in ares.h
// ares_set_socket_callback at ares_socket.c:396:6 in ares.h
// ares_queue_wait_empty at ares_threads.c:768:15 in ares.h
// ares_dup at ares_init.c:455:5 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_reinit at ares_init.c:407:15 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>

#include "ares.h"

static int dummy_socket_create_callback(int sock, int type, void *user_data)
{
    (void)sock;
    (void)type;
    (void)user_data;
    return 0;
}

static ares_socket_t dummy_socket_create(int domain, int type, int protocol, void *user_data)
{
    (void)domain;
    (void)type;
    (void)protocol;
    (void)user_data;
    return ARES_SOCKET_BAD;
}

static int dummy_socket_close(ares_socket_t sock, void *user_data)
{
    (void)sock;
    (void)user_data;
    return -1;
}

static int dummy_socket_connect(ares_socket_t sock, const struct sockaddr *addr,
                                ares_socklen_t addrlen, unsigned int flags, void *user_data)
{
    (void)sock;
    (void)addr;
    (void)addrlen;
    (void)flags;
    (void)user_data;
    return -1;
}

static ares_ssize_t dummy_socket_recvfrom(ares_socket_t sock, void *buf, size_t len,
                                          int flags, struct sockaddr *addr,
                                          ares_socklen_t *addrlen, void *user_data)
{
    (void)sock;
    (void)buf;
    (void)len;
    (void)flags;
    (void)addr;
    (void)addrlen;
    (void)user_data;
    return -1;
}

static ares_ssize_t dummy_socket_sendto(ares_socket_t sock, const void *buf, size_t len,
                                        int flags, const struct sockaddr *addr,
                                        ares_socklen_t addrlen, void *user_data)
{
    (void)sock;
    (void)buf;
    (void)len;
    (void)flags;
    (void)addr;
    (void)addrlen;
    (void)user_data;
    return -1;
}

static int dummy_socket_setsockopt(ares_socket_t sock, ares_socket_opt_t opt,
                                   const void *val, ares_socklen_t val_size, void *user_data)
{
    (void)sock;
    (void)opt;
    (void)val;
    (void)val_size;
    (void)user_data;
    return -1;
}

static int dummy_socket_getsockname(ares_socket_t sock, struct sockaddr *addr,
                                    ares_socklen_t *addrlen, void *user_data)
{
    (void)sock;
    (void)addr;
    (void)addrlen;
    (void)user_data;
    return -1;
}

static int dummy_socket_bind(ares_socket_t sock, unsigned int flags,
                             const struct sockaddr *addr, socklen_t addrlen, void *user_data)
{
    (void)sock;
    (void)flags;
    (void)addr;
    (void)addrlen;
    (void)user_data;
    return -1;
}

static unsigned int dummy_if_nametoindex(const char *ifname, void *user_data)
{
    (void)ifname;
    (void)user_data;
    return 0;
}

static const char *dummy_if_indextoname(unsigned int ifindex, char *ifname_buf,
                                        size_t ifname_buf_len, void *user_data)
{
    (void)ifindex;
    (void)ifname_buf;
    (void)ifname_buf_len;
    (void)user_data;
    return NULL;
}

static struct ares_socket_functions_ex dummy_socket_funcs = {
    .version = 1,
    .flags = 0,
    .asocket = dummy_socket_create,
    .aclose = dummy_socket_close,
    .asetsockopt = dummy_socket_setsockopt,
    .aconnect = dummy_socket_connect,
    .arecvfrom = dummy_socket_recvfrom,
    .asendto = dummy_socket_sendto,
    .agetsockname = dummy_socket_getsockname,
    .abind = dummy_socket_bind,
    .aif_nametoindex = dummy_if_nametoindex,
    .aif_indextoname = dummy_if_indextoname
};

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size)
{
    ares_channel_t *channel = NULL;
    ares_channel_t *dup_channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    if (Size < 1) {
        return 0;
    }
    
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    ares_threadsafety();
    
    int init_flags = ARES_FLAG_STAYOPEN;
    if (selector & 0x01) {
        init_flags |= ARES_FLAG_USEVC;
    }
    if (selector & 0x02) {
        init_flags |= ARES_FLAG_PRIMARY;
    }
    if (selector & 0x04) {
        init_flags |= ARES_FLAG_IGNTC;
    }
    if (selector & 0x08) {
        init_flags |= ARES_FLAG_NORECURSE;
    }
    if (selector & 0x10) {
        init_flags |= ARES_FLAG_EDNS;
    }
    
    memset(&options, 0, sizeof(options));
    
    if (Size > data_offset + sizeof(int)) {
        int timeout = *(const int *)(Data + data_offset);
        timeout = timeout % 10000;
        if (timeout < 0) timeout = -timeout;
        options.timeout = timeout;
        optmask |= ARES_OPT_TIMEOUTMS;
        data_offset += sizeof(int);
    }
    
    if (Size > data_offset + sizeof(int)) {
        int tries = *(const int *)(Data + data_offset);
        tries = tries % 10;
        if (tries < 1) tries = 1;
        options.tries = tries;
        optmask |= ARES_OPT_TRIES;
        data_offset += sizeof(int);
    }
    
    if (Size > data_offset + sizeof(int)) {
        int ndots = *(const int *)(Data + data_offset);
        ndots = ndots % 15;
        options.ndots = ndots;
        optmask |= ARES_OPT_NDOTS;
        data_offset += sizeof(int);
    }
    
    if (Size > data_offset + sizeof(int)) {
        int udp_port = *(const int *)(Data + data_offset);
        udp_port = udp_port % 65536;
        if (udp_port < 1) udp_port = 1;
        options.udp_port = (unsigned short)udp_port;
        optmask |= ARES_OPT_UDP_PORT;
        data_offset += sizeof(int);
    }
    
    if (Size > data_offset + sizeof(int)) {
        int tcp_port = *(const int *)(Data + data_offset);
        tcp_port = tcp_port % 65536;
        if (tcp_port < 1) tcp_port = 1;
        options.tcp_port = (unsigned short)tcp_port;
        optmask |= ARES_OPT_TCP_PORT;
        data_offset += sizeof(int);
    }
    
    int status = ares_init_options(&channel, &options, optmask | init_flags);
    if (status != ARES_SUCCESS) {
        return 0;
    }
    
    if (selector & 0x20) {
        ares_set_socket_functions_ex(channel, &dummy_socket_funcs, NULL);
    }
    
    if (selector & 0x40) {
        ares_set_socket_callback(channel, dummy_socket_create_callback, NULL);
    }
    
    int timeout_ms = -1;
    if (Size > data_offset + sizeof(int)) {
        timeout_ms = *(const int *)(Data + data_offset);
        data_offset += sizeof(int);
    }
    
    ares_queue_wait_empty(channel, timeout_ms);
    
    if (selector & 0x80) {
        ares_dup(&dup_channel, channel);
    }
    
    ares_cancel(channel);
    
    ares_reinit(channel);
    
    if (dup_channel) {
        ares_cancel(dup_channel);
        ares_destroy(dup_channel);
    }
    
    ares_destroy(channel);
    
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_set_socket_functions at ares_set_socket_functions.c:579:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_query at ares_query.c:133:6 in ares.h
// ares_mkquery at ares_create_query.c:74:5 in ares.h
// ares_search at ares_search.c:431:6 in ares.h
// ares_query_dnsrec at ares_query.c:115:15 in ares.h
// ares_create_query at ares_create_query.c:66:5 in ares.h
// ares_send at ares_send.c:247:6 in ares.h
// ares_process_fd at ares_process.c:267:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "ares.h"

static void dummy_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

static void dummy_callback_dnsrec(void *arg, ares_status_t status, 
                                 unsigned long flags, const struct ares_dns_record *dnsrec) {
    (void)arg;
    (void)status;
    (void)flags;
    (void)dnsrec;
}

static ares_socket_t dummy_socket(int domain, int type, int protocol, void *user_data) {
    (void)domain;
    (void)type;
    (void)protocol;
    (void)user_data;
    return ARES_SOCKET_BAD;
}

static int dummy_close(ares_socket_t sock, void *user_data) {
    (void)sock;
    (void)user_data;
    return -1;
}

static int dummy_connect(ares_socket_t sock, const struct sockaddr *address,
                        ares_socklen_t address_len, void *user_data) {
    (void)sock;
    (void)address;
    (void)address_len;
    (void)user_data;
    return -1;
}

static ares_ssize_t dummy_recvfrom(ares_socket_t sock, void *buffer, size_t length,
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

static ares_ssize_t dummy_sendv(ares_socket_t sock, const struct iovec *iov,
                               int iovcnt, void *user_data) {
    (void)sock;
    (void)iov;
    (void)iovcnt;
    (void)user_data;
    return -1;
}

static struct ares_socket_functions g_dummy_funcs = {
    .asocket = dummy_socket,
    .aclose = dummy_close,
    .aconnect = dummy_connect,
    .arecvfrom = dummy_recvfrom,
    .asendv = dummy_sendv
};

static ares_channel_t* init_test_channel(void) {
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    
    options.timeout = 1000;
    optmask |= ARES_OPT_TIMEOUTMS;
    
    options.tries = 1;
    optmask |= ARES_OPT_TRIES;
    
    options.sock_state_cb = NULL;
    options.sock_state_cb_data = NULL;
    
    options.socket_send_buffer_size = 0;
    options.socket_receive_buffer_size = 0;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        return NULL;
    }
    
    ares_set_socket_functions(channel, &g_dummy_funcs, NULL);
    
    return channel;
}

static void cleanup_test_channel(ares_channel_t *channel) {
    if (channel) {
        ares_destroy(channel);
    }
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    
    ssize_t written = write(fd, data, size);
    close(fd);
    
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *data, size_t size) {
    if (size < 1) {
        return 0;
    }
    
    ares_channel_t *channel = init_test_channel();
    if (!channel) {
        return 0;
    }
    
    unsigned char *query_buf = NULL;
    int query_len = 0;
    unsigned short query_id = 0;
    ares_status_t status;
    
    uint8_t func_selector = data[0] % 6;
    
    char *name = NULL;
    int dnsclass = 1;
    int type = 1;
    unsigned short id = 1234;
    int rd = 1;
    int max_udp_size = 512;
    
    if (size > 1) {
        size_t name_len = (size - 1) / 2;
        if (name_len > 0) {
            name = malloc(name_len + 1);
            if (name) {
                size_t copy_len = name_len;
                if (copy_len > size - 1) {
                    copy_len = size - 1;
                }
                memcpy(name, data + 1, copy_len);
                name[copy_len] = '\0';
            }
        }
    }
    
    if (!name) {
        name = strdup("example.com");
    }
    
    if (size > 2) {
        dnsclass = data[2] % 65536;
    }
    
    if (size > 3) {
        type = data[3] % 65536;
    }
    
    if (size > 5) {
        id = (data[4] << 8) | data[5];
    }
    
    if (size > 6) {
        rd = data[6] & 1;
    }
    
    if (size > 7) {
        max_udp_size = 512 + (data[7] % 1024);
    }
    
    switch (func_selector) {
        case 0:
            ares_query(channel, name, dnsclass, type, dummy_callback, NULL);
            break;
            
        case 1:
            ares_mkquery(name, dnsclass, type, id, rd, &query_buf, &query_len);
            if (query_buf) {
                free(query_buf);
                query_buf = NULL;
            }
            break;
            
        case 2:
            ares_search(channel, name, dnsclass, type, dummy_callback, NULL);
            break;
            
        case 3:
            status = ares_query_dnsrec(channel, name, (ares_dns_class_t)dnsclass,
                                      (ares_dns_rec_type_t)type, dummy_callback_dnsrec,
                                      NULL, &query_id);
            (void)status;
            break;
            
        case 4:
            ares_create_query(name, dnsclass, type, id, rd, &query_buf, &query_len, max_udp_size);
            if (query_buf) {
                free(query_buf);
                query_buf = NULL;
            }
            break;
            
        case 5:
            if (size > 8) {
                size_t qlen = size - 8;
                if (qlen > 65535) qlen = 65535;
                ares_send(channel, data + 8, (int)qlen, dummy_callback, NULL);
            }
            break;
    }
    
    free(name);
    
    ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    
    if (write_dummy_file(data, size) == 0) {
        unlink("./dummy_file");
    }
    
    cleanup_test_channel(channel);
    
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_strerror at ares_strerror.c:30:13 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

static struct ares_addrinfo *create_dummy_addrinfo(const uint8_t *data, size_t size) {
    struct ares_addrinfo *ai = (struct ares_addrinfo *)malloc(sizeof(struct ares_addrinfo));
    if (!ai) return NULL;
    
    memset(ai, 0, sizeof(struct ares_addrinfo));
    
    ai->nodes = NULL;
    ai->cnames = NULL;
    ai->name = NULL;
    
    if (size > 0) {
        struct ares_addrinfo_node *node = (struct ares_addrinfo_node *)malloc(sizeof(struct ares_addrinfo_node));
        if (node) {
            memset(node, 0, sizeof(struct ares_addrinfo_node));
            node->ai_ttl = 300;
            node->ai_flags = 0;
            node->ai_family = AF_INET;
            node->ai_socktype = SOCK_STREAM;
            node->ai_protocol = IPPROTO_TCP;
            node->ai_addrlen = sizeof(struct sockaddr_in);
            node->ai_addr = (struct sockaddr *)malloc(sizeof(struct sockaddr_in));
            node->ai_next = NULL;
            
            if (node->ai_addr) {
                struct sockaddr_in *addr = (struct sockaddr_in *)node->ai_addr;
                addr->sin_family = AF_INET;
                addr->sin_port = htons(80);
                size_t copy_size = size > 4 ? 4 : size;
                memcpy(&addr->sin_addr, data, copy_size);
            }
            
            ai->nodes = node;
        }
        
        if (size > 4) {
            struct ares_addrinfo_cname *cname = (struct ares_addrinfo_cname *)malloc(sizeof(struct ares_addrinfo_cname));
            if (cname) {
                memset(cname, 0, sizeof(struct ares_addrinfo_cname));
                cname->ttl = 300;
                cname->alias = strdup("example.com");
                cname->name = strdup("real.example.com");
                cname->next = NULL;
                ai->cnames = cname;
            }
        }
    }
    
    return ai;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    const char *error_str = ares_strerror((int)data[0] % 256);
    (void)error_str;
    
    if (size >= 1) {
        int af = (data[0] & 1) ? AF_INET : AF_INET6;
        char dst[INET6_ADDRSTRLEN];
        const void *src = data;
        ares_socklen_t dst_size = sizeof(dst);
        
        if (af == AF_INET && size >= 4) {
            const char *result = ares_inet_ntop(af, src, dst, dst_size);
            (void)result;
        } else if (af == AF_INET6 && size >= 16) {
            const char *result = ares_inet_ntop(af, src, dst, dst_size);
            (void)result;
        }
    }
    
    struct ares_addrinfo *ai = create_dummy_addrinfo(data, size);
    if (ai) {
        ares_freeaddrinfo(ai);
    }
    
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_strerror at ares_strerror.c:30:13 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

static struct ares_addrinfo* create_dummy_addrinfo(const uint8_t* data, size_t size) {
    struct ares_addrinfo* ai = malloc(sizeof(struct ares_addrinfo));
    if (!ai) return NULL;
    
    ai->cnames = NULL;
    ai->nodes = NULL;
    ai->name = NULL;
    
    if (size > 0) {
        size_t name_len = size > 100 ? 100 : size;
        ai->name = malloc(name_len + 1);
        if (ai->name) {
            memcpy(ai->name, data, name_len);
            ai->name[name_len] = '\0';
        }
        
        if (size > 1) {
            struct ares_addrinfo_cname* cname = malloc(sizeof(struct ares_addrinfo_cname));
            if (cname) {
                cname->ttl = data[0];
                cname->next = NULL;
                
                size_t alias_len = (size > 50 && size < 100) ? 10 : 5;
                cname->alias = malloc(alias_len + 1);
                if (cname->alias) {
                    memcpy(cname->alias, "alias", 5);
                    cname->alias[alias_len] = '\0';
                }
                
                size_t cname_len = (size > 30 && size < 80) ? 8 : 6;
                cname->name = malloc(cname_len + 1);
                if (cname->name) {
                    memcpy(cname->name, "cname", 5);
                    cname->name[cname_len] = '\0';
                }
                
                ai->cnames = cname;
            }
        }
        
        if (size > 2) {
            struct ares_addrinfo_node* node = malloc(sizeof(struct ares_addrinfo_node));
            if (node) {
                node->ai_ttl = data[1];
                node->ai_flags = data[2] & 0x0F;
                node->ai_family = (data[2] & 0x10) ? AF_INET6 : AF_INET;
                node->ai_socktype = SOCK_STREAM;
                node->ai_protocol = IPPROTO_TCP;
                node->ai_next = NULL;
                
                if (node->ai_family == AF_INET) {
                    node->ai_addrlen = sizeof(struct sockaddr_in);
                    node->ai_addr = malloc(node->ai_addrlen);
                    if (node->ai_addr) {
                        memset(node->ai_addr, 0, node->ai_addrlen);
                        ((struct sockaddr_in*)node->ai_addr)->sin_family = AF_INET;
                        if (size > 6) {
                            memcpy(&((struct sockaddr_in*)node->ai_addr)->sin_addr, data + 3, 4);
                        }
                    }
                } else {
                    node->ai_addrlen = sizeof(struct sockaddr_in6);
                    node->ai_addr = malloc(node->ai_addrlen);
                    if (node->ai_addr) {
                        memset(node->ai_addr, 0, node->ai_addrlen);
                        ((struct sockaddr_in6*)node->ai_addr)->sin6_family = AF_INET6;
                        if (size > 18) {
                            memcpy(&((struct sockaddr_in6*)node->ai_addr)->sin6_addr, data + 3, 16);
                        }
                    }
                }
                
                ai->nodes = node;
            }
        }
    }
    
    return ai;
}

int LLVMFuzzerTestOneInput_32(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size > 1024 ? 1024 : Size);
        close(fd);
    }
    
    for (int i = 0; i < 3; i++) {
        int error_code = (int)Data[0];
        if (i == 1) error_code = -error_code;
        if (i == 2) error_code = error_code * 2;
        
        const char* error_str = ares_strerror(error_code);
        (void)error_str;
    }
    
    char dst[INET6_ADDRSTRLEN];
    const char* result = NULL;
    
    if (Size >= 4) {
        uint32_t ipv4_addr;
        memcpy(&ipv4_addr, Data, 4);
        result = ares_inet_ntop(AF_INET, &ipv4_addr, dst, sizeof(dst));
        (void)result;
    }
    
    if (Size >= 16) {
        unsigned char ipv6_addr[16];
        memcpy(ipv6_addr, Data, 16);
        result = ares_inet_ntop(AF_INET6, ipv6_addr, dst, sizeof(dst));
        (void)result;
    }
    
    struct ares_addrinfo* ai1 = create_dummy_addrinfo(Data, Size);
    if (ai1) {
        ares_freeaddrinfo(ai1);
    }
    
    struct ares_addrinfo* ai2 = malloc(sizeof(struct ares_addrinfo));
    if (ai2) {
        ai2->cnames = NULL;
        ai2->nodes = NULL;
        ai2->name = NULL;
        ares_freeaddrinfo(ai2);
    }
    
    if (Size > 0) {
        struct ares_addrinfo* ai3 = malloc(sizeof(struct ares_addrinfo));
        if (ai3) {
            ai3->cnames = NULL;
            ai3->nodes = NULL;
            size_t name_len = Size > 50 ? 50 : Size;
            ai3->name = malloc(name_len + 1);
            if (ai3->name) {
                memcpy(ai3->name, Data, name_len);
                ai3->name[name_len] = '\0';
            }
            ares_freeaddrinfo(ai3);
        }
    }
    
    unlink("./dummy_file");
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
// ares_strerror at ares_strerror.c:30:13 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static struct ares_addrinfo* create_dummy_addrinfo(void) {
    struct ares_addrinfo* ai = malloc(sizeof(struct ares_addrinfo));
    if (!ai) return NULL;
    
    // Create a dummy cname
    struct ares_addrinfo_cname* cname = malloc(sizeof(struct ares_addrinfo_cname));
    if (cname) {
        cname->ttl = 300;
        cname->alias = strdup("dummy.alias");
        cname->name = strdup("dummy.name");
        cname->next = NULL;
    }
    ai->cnames = cname;
    
    // Create a dummy node
    struct ares_addrinfo_node* node = malloc(sizeof(struct ares_addrinfo_node));
    if (node) {
        node->ai_ttl = 300;
        node->ai_flags = 0;
        node->ai_family = AF_INET;
        node->ai_socktype = SOCK_STREAM;
        node->ai_protocol = IPPROTO_TCP;
        node->ai_addrlen = sizeof(struct sockaddr_in);
        node->ai_addr = malloc(node->ai_addrlen);
        if (node->ai_addr) {
            struct sockaddr_in* addr = (struct sockaddr_in*)node->ai_addr;
            addr->sin_family = AF_INET;
            addr->sin_port = htons(80);
            inet_pton(AF_INET, "127.0.0.1", &addr->sin_addr);
        }
        node->ai_next = NULL;
    }
    ai->nodes = node;
    
    ai->name = strdup("dummy.host");
    
    return ai;
}

static void cleanup_addrinfo(struct ares_addrinfo* ai) {
    if (ai) {
        if (ai->cnames) {
            free(ai->cnames->alias);
            free(ai->cnames->name);
            free(ai->cnames);
        }
        if (ai->nodes) {
            if (ai->nodes->ai_addr) {
                free(ai->nodes->ai_addr);
            }
            free(ai->nodes);
        }
        free(ai->name);
        free(ai);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // 1. Test ares_strerror
    int error_code = Data[0] % 100; // Use modulo to get reasonable error codes
    const char* error_str = ares_strerror(error_code);
    
    // 2. Test ares_inet_ntop
    char dst[INET6_ADDRSTRLEN];
    const char* result = NULL;
    
    if (Size >= 5) { // Enough for IPv4 address (4 bytes) + family (1 byte)
        int af = (Data[1] & 0x01) ? AF_INET6 : AF_INET;
        size_t addr_size = (af == AF_INET) ? 4 : 16;
        
        if (Size >= 2 + addr_size) {
            result = ares_inet_ntop(af, Data + 2, dst, sizeof(dst));
        }
    }
    
    // 3. Test ares_freeaddrinfo
    struct ares_addrinfo* ai = create_dummy_addrinfo();
    ares_freeaddrinfo(ai);
    
    // Also test with NULL
    ares_freeaddrinfo(NULL);
    
    // Create another dummy structure to test different program states
    if (Size > 0) {
        struct ares_addrinfo* ai2 = create_dummy_addrinfo();
        if (ai2) {
            // Modify based on input data
            if (Size >= 2 && ai2->cnames) {
                free(ai2->cnames->alias);
                ai2->cnames->alias = strdup("modified.alias");
            }
            ares_freeaddrinfo(ai2);
        }
    }
    
    return 0;
}
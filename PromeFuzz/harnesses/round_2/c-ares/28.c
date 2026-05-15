// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_strerror at ares_strerror.c:30:13 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_inet_ntop at inet_ntop.c:64:20 in ares.h
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

static struct ares_addrinfo* create_dummy_addrinfo(void) {
    struct ares_addrinfo* ai = (struct ares_addrinfo*)malloc(sizeof(struct ares_addrinfo));
    if (!ai) return NULL;
    
    // Initialize ALL fields to NULL first
    ai->nodes = NULL;
    ai->cnames = NULL;
    ai->name = NULL; // Initialize the name field
    
    // Create one dummy node
    struct ares_addrinfo_node* node = (struct ares_addrinfo_node*)malloc(sizeof(struct ares_addrinfo_node));
    if (node) {
        memset(node, 0, sizeof(struct ares_addrinfo_node));
        node->ai_family = AF_INET;
        node->ai_socktype = SOCK_STREAM;
        node->ai_addrlen = sizeof(struct sockaddr_in);
        node->ai_addr = (struct sockaddr*)malloc(sizeof(struct sockaddr_in));
        if (node->ai_addr) {
            memset(node->ai_addr, 0, sizeof(struct sockaddr_in));
        }
        node->ai_next = NULL;
        ai->nodes = node;
    }
    
    // Create one dummy cname - but check if we can allocate safely
    struct ares_addrinfo_cname* cname = (struct ares_addrinfo_cname*)malloc(sizeof(struct ares_addrinfo_cname));
    if (cname) {
        memset(cname, 0, sizeof(struct ares_addrinfo_cname));
        cname->alias = strdup("example.com");
        cname->name = strdup("example.org");
        cname->next = NULL;
        ai->cnames = cname;
    }
    
    // Allocate and set the name field
    ai->name = strdup("dummy.name");
    
    return ai;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // 1. ares_strerror - use first 4 bytes as error code
    int error_code = 0;
    if (Size >= sizeof(int)) {
        error_code = *(const int*)Data;
    } else if (Size > 0) {
        // Use first byte if input is too small
        error_code = Data[0];
    }
    const char* error_str = ares_strerror(error_code);
    (void)error_str; // Use result to avoid unused variable warning
    
    // 2. ares_inet_ntop - convert binary address to string
    char dst[INET6_ADDRSTRLEN];
    memset(dst, 0, sizeof(dst));
    
    // Use different address families based on input
    if (Size >= sizeof(struct in6_addr)) {
        // IPv6 - use first 16 bytes as address
        ares_inet_ntop(AF_INET6, Data, dst, sizeof(dst));
    } else if (Size >= sizeof(struct in_addr)) {
        // IPv4 - use first 4 bytes as address
        ares_inet_ntop(AF_INET, Data, dst, sizeof(dst));
    } else {
        // Use dummy addresses for small inputs
        struct in_addr addr4;
        struct in6_addr addr6;
        memset(&addr4, 0, sizeof(addr4));
        memset(&addr6, 0, sizeof(addr6));
        
        // Try both address families
        ares_inet_ntop(AF_INET, &addr4, dst, sizeof(dst));
        ares_inet_ntop(AF_INET6, &addr6, dst, sizeof(dst));
    }
    
    // 3. ares_freeaddrinfo - free allocated addrinfo structure
    struct ares_addrinfo* ai = create_dummy_addrinfo();
    if (ai) {
        ares_freeaddrinfo(ai);
    }
    
    // Also try with NULL pointer
    ares_freeaddrinfo(NULL);
    
    return 0;
}
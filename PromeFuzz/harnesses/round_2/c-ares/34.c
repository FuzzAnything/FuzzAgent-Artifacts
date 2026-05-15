// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_uri_reply at ares_parse_uri_reply.c:30:5 in ares.h
// ares_parse_a_reply at ares_parse_a_reply.c:48:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_a_reply at ares_parse_a_reply.c:48:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_ns_reply at ares_parse_ns_reply.c:39:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_naptr_reply at ares_parse_naptr_reply.c:29:5 in ares.h
// ares_parse_mx_reply at ares_parse_mx_reply.c:30:5 in ares.h
// ares_parse_ptr_reply at ares_parse_ptr_reply.c:186:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_ptr_reply at ares_parse_ptr_reply.c:186:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_naptr(struct ares_naptr_reply *naptr) {
    while (naptr) {
        struct ares_naptr_reply *next = naptr->next;
        free(naptr->flags);
        free(naptr->service);
        free(naptr->regexp);
        free(naptr->replacement);
        free(naptr);
        naptr = next;
    }
}

static void cleanup_uri(struct ares_uri_reply *uri) {
    while (uri) {
        struct ares_uri_reply *next = uri->next;
        free(uri->uri);
        free(uri);
        uri = next;
    }
}

static void cleanup_mx(struct ares_mx_reply *mx) {
    while (mx) {
        struct ares_mx_reply *next = mx->next;
        free(mx->host);
        free(mx);
        mx = next;
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    int alen = (int)Size;
    const unsigned char *abuf = Data;
    
    // Test ares_parse_ns_reply
    {
        struct hostent *host = NULL;
        int ret = ares_parse_ns_reply(abuf, alen, &host);
        if (ret == ARES_SUCCESS && host != NULL) {
            ares_free_hostent(host);
        }
    }
    
    // Test ares_parse_naptr_reply
    {
        struct ares_naptr_reply *naptr_out = NULL;
        int ret = ares_parse_naptr_reply(abuf, alen, &naptr_out);
        if (ret == ARES_SUCCESS && naptr_out != NULL) {
            cleanup_naptr(naptr_out);
        }
    }
    
    // Test ares_parse_mx_reply
    {
        struct ares_mx_reply *mx_out = NULL;
        int ret = ares_parse_mx_reply(abuf, alen, &mx_out);
        if (ret == ARES_SUCCESS && mx_out != NULL) {
            cleanup_mx(mx_out);
        }
    }
    
    // Test ares_parse_ptr_reply with IPv4
    {
        struct hostent *host = NULL;
        struct in_addr addr;
        addr.s_addr = 0x0100007F; // 127.0.0.1 in network byte order
        int ret = ares_parse_ptr_reply(abuf, alen, &addr, sizeof(addr), AF_INET, &host);
        if (ret == ARES_SUCCESS && host != NULL) {
            ares_free_hostent(host);
        }
    }
    
    // Test ares_parse_ptr_reply with IPv6
    {
        struct hostent *host = NULL;
        struct in6_addr addr6;
        memset(&addr6, 0, sizeof(addr6));
        addr6.s6_addr[15] = 1; // ::1
        int ret = ares_parse_ptr_reply(abuf, alen, &addr6, sizeof(addr6), AF_INET6, &host);
        if (ret == ARES_SUCCESS && host != NULL) {
            ares_free_hostent(host);
        }
    }
    
    // Test ares_parse_uri_reply
    {
        struct ares_uri_reply *uri_out = NULL;
        int ret = ares_parse_uri_reply(abuf, alen, &uri_out);
        if (ret == ARES_SUCCESS && uri_out != NULL) {
            cleanup_uri(uri_out);
        }
    }
    
    // Test ares_parse_a_reply
    {
        struct hostent *host = NULL;
        struct ares_addrttl *addrttls = NULL;
        int naddrttls = 0;
        
        // First call to get required size
        int ret = ares_parse_a_reply(abuf, alen, &host, NULL, &naddrttls);
        if (ret == ARES_SUCCESS) {
            if (host != NULL) {
                ares_free_hostent(host);
                host = NULL;
            }
            
            // Allocate and call again if needed
            if (naddrttls > 0) {
                addrttls = (struct ares_addrttl*)malloc(naddrttls * sizeof(struct ares_addrttl));
                if (addrttls != NULL) {
                    ret = ares_parse_a_reply(abuf, alen, &host, addrttls, &naddrttls);
                    if (ret == ARES_SUCCESS) {
                        if (host != NULL) {
                            ares_free_hostent(host);
                        }
                    }
                    free(addrttls);
                }
            }
        }
    }
    
    return 0;
}
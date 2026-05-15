// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_caa_reply at ares_parse_caa_reply.c:30:5 in ares.h
// ares_parse_ns_reply at ares_parse_ns_reply.c:39:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_uri_reply at ares_parse_uri_reply.c:30:5 in ares.h
// ares_parse_mx_reply at ares_parse_mx_reply.c:30:5 in ares.h
// ares_parse_ptr_reply at ares_parse_ptr_reply.c:186:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_ptr_reply at ares_parse_ptr_reply.c:186:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_a_reply at ares_parse_a_reply.c:48:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_a_reply at ares_parse_a_reply.c:48:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <arpa/inet.h>
#include "ares.h"

static void free_uri_reply(struct ares_uri_reply *uri_out) {
    while (uri_out) {
        struct ares_uri_reply *next = uri_out->next;
        free(uri_out->uri);
        free(uri_out);
        uri_out = next;
    }
}

static void free_mx_reply(struct ares_mx_reply *mx_out) {
    while (mx_out) {
        struct ares_mx_reply *next = mx_out->next;
        free(mx_out->host);
        free(mx_out);
        mx_out = next;
    }
}

static void free_caa_reply(struct ares_caa_reply *caa_out) {
    while (caa_out) {
        struct ares_caa_reply *next = caa_out->next;
        free(caa_out->property);
        free(caa_out->value);
        free(caa_out);
        caa_out = next;
    }
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    const unsigned char *abuf = Data;
    int alen = Size > INT_MAX ? INT_MAX : (int)Size;
    
    // Test ares_parse_ns_reply
    {
        struct hostent *host = NULL;
        int result = ares_parse_ns_reply(abuf, alen, &host);
        if (host) ares_free_hostent(host);
    }
    
    // Test ares_parse_uri_reply
    {
        struct ares_uri_reply *uri_out = NULL;
        int result = ares_parse_uri_reply(abuf, alen, &uri_out);
        if (uri_out) free_uri_reply(uri_out);
    }
    
    // Test ares_parse_mx_reply
    {
        struct ares_mx_reply *mx_out = NULL;
        int result = ares_parse_mx_reply(abuf, alen, &mx_out);
        if (mx_out) free_mx_reply(mx_out);
    }
    
    // Test ares_parse_ptr_reply
    {
        struct hostent *host = NULL;
        // Use dummy address (IPv4)
        struct in_addr addr;
        memset(&addr, 0, sizeof(addr));
        int result1 = ares_parse_ptr_reply(abuf, alen, &addr, sizeof(addr), AF_INET, &host);
        if (host) ares_free_hostent(host);
        
        // Try IPv6
        host = NULL;
        struct in6_addr addr6;
        memset(&addr6, 0, sizeof(addr6));
        int result2 = ares_parse_ptr_reply(abuf, alen, &addr6, sizeof(addr6), AF_INET6, &host);
        if (host) ares_free_hostent(host);
    }
    
    // Test ares_parse_a_reply
    {
        struct hostent *host = NULL;
        struct ares_addrttl *addrttls = NULL;
        int naddrttls = 0;
        int result = ares_parse_a_reply(abuf, alen, &host, addrttls, &naddrttls);
        if (host) ares_free_hostent(host);
        
        // Test with allocated addrttls
        host = NULL;
        addrttls = malloc(sizeof(struct ares_addrttl) * 10);
        naddrttls = 10;
        result = ares_parse_a_reply(abuf, alen, &host, addrttls, &naddrttls);
        if (host) ares_free_hostent(host);
        free(addrttls);
    }
    
    // Test ares_parse_caa_reply
    {
        struct ares_caa_reply *caa_out = NULL;
        int result = ares_parse_caa_reply(abuf, alen, &caa_out);
        if (caa_out) free_caa_reply(caa_out);
    }
    
    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_ptr_reply at ares_parse_ptr_reply.c:186:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_caa_reply at ares_parse_caa_reply.c:30:5 in ares.h
// ares_parse_ns_reply at ares_parse_ns_reply.c:39:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_uri_reply at ares_parse_uri_reply.c:30:5 in ares.h
// ares_parse_mx_reply at ares_parse_mx_reply.c:30:5 in ares.h
// ares_parse_a_reply at ares_parse_a_reply.c:48:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_ptr_reply at ares_parse_ptr_reply.c:186:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

static void free_caa_reply(struct ares_caa_reply *caa) {
    while (caa) {
        struct ares_caa_reply *next = caa->next;
        free(caa->property);
        free(caa->value);
        free(caa);
        caa = next;
    }
}

static void free_uri_reply(struct ares_uri_reply *uri) {
    while (uri) {
        struct ares_uri_reply *next = uri->next;
        free(uri->uri);
        free(uri);
        uri = next;
    }
}

static void free_mx_reply(struct ares_mx_reply *mx) {
    while (mx) {
        struct ares_mx_reply *next = mx->next;
        free(mx->host);
        free(mx);
        mx = next;
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Test ares_parse_caa_reply
    struct ares_caa_reply *caa_out = NULL;
    int ret_caa = ares_parse_caa_reply(Data, (int)Size, &caa_out);
    if (ret_caa == ARES_SUCCESS && caa_out) {
        free_caa_reply(caa_out);
    }

    // Test ares_parse_ns_reply
    struct hostent *ns_host = NULL;
    int ret_ns = ares_parse_ns_reply(Data, (int)Size, &ns_host);
    if (ret_ns == ARES_SUCCESS && ns_host) {
        ares_free_hostent(ns_host);
    }

    // Test ares_parse_uri_reply
    struct ares_uri_reply *uri_out = NULL;
    int ret_uri = ares_parse_uri_reply(Data, (int)Size, &uri_out);
    if (ret_uri == ARES_SUCCESS && uri_out) {
        free_uri_reply(uri_out);
    }

    // Test ares_parse_mx_reply
    struct ares_mx_reply *mx_out = NULL;
    int ret_mx = ares_parse_mx_reply(Data, (int)Size, &mx_out);
    if (ret_mx == ARES_SUCCESS && mx_out) {
        free_mx_reply(mx_out);
    }

    // Test ares_parse_a_reply
    struct hostent *a_host = NULL;
    struct ares_addrttl addrttls[10];
    int naddrttls = 10;
    int ret_a = ares_parse_a_reply(Data, (int)Size, &a_host, addrttls, &naddrttls);
    if (ret_a == ARES_SUCCESS && a_host) {
        ares_free_hostent(a_host);
    }

    // Test ares_parse_ptr_reply
    struct hostent *ptr_host = NULL;
    // Use first 4 bytes as IPv4 address if available, otherwise use dummy
    struct in_addr dummy_addr;
    const void *addr = &dummy_addr;
    int addrlen = sizeof(dummy_addr);
    int family = AF_INET;
    
    if (Size >= 4) {
        addr = Data;
        addrlen = 4;
        family = AF_INET;
    } else {
        dummy_addr.s_addr = 0x0100007F; // 127.0.0.1 in network byte order
    }
    
    int ret_ptr = ares_parse_ptr_reply(Data, (int)Size, addr, addrlen, family, &ptr_host);
    if (ret_ptr == ARES_SUCCESS && ptr_host) {
        ares_free_hostent(ptr_host);
    }

    // Try with IPv6 if we have enough data
    if (Size >= 16) {
        struct hostent *ptr_host6 = NULL;
        int ret_ptr6 = ares_parse_ptr_reply(Data, (int)Size, Data, 16, AF_INET6, &ptr_host6);
        if (ret_ptr6 == ARES_SUCCESS && ptr_host6) {
            ares_free_hostent(ptr_host6);
        }
    }

    return 0;
}
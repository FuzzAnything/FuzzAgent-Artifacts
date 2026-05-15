// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_ptr_reply at ares_parse_ptr_reply.c:186:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_ptr_reply at ares_parse_ptr_reply.c:186:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_caa_reply at ares_parse_caa_reply.c:30:5 in ares.h
// ares_parse_ns_reply at ares_parse_ns_reply.c:39:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_uri_reply at ares_parse_uri_reply.c:30:5 in ares.h
// ares_parse_mx_reply at ares_parse_mx_reply.c:30:5 in ares.h
// ares_parse_a_reply at ares_parse_a_reply.c:48:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_a_reply at ares_parse_a_reply.c:48:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
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

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use the entire input as the DNS reply buffer
    const unsigned char *abuf = Data;
    int alen = (Size > INT_MAX) ? INT_MAX : (int)Size;

    // Test ares_parse_caa_reply
    struct ares_caa_reply *caa_out = NULL;
    int ret_caa = ares_parse_caa_reply(abuf, alen, &caa_out);
    if (ret_caa == ARES_SUCCESS && caa_out != NULL) {
        free_caa_reply(caa_out);
    }

    // Test ares_parse_ns_reply
    struct hostent *host_ns = NULL;
    int ret_ns = ares_parse_ns_reply(abuf, alen, &host_ns);
    if (ret_ns == ARES_SUCCESS && host_ns != NULL) {
        ares_free_hostent(host_ns);
    }

    // Test ares_parse_uri_reply
    struct ares_uri_reply *uri_out = NULL;
    int ret_uri = ares_parse_uri_reply(abuf, alen, &uri_out);
    if (ret_uri == ARES_SUCCESS && uri_out != NULL) {
        free_uri_reply(uri_out);
    }

    // Test ares_parse_mx_reply
    struct ares_mx_reply *mx_out = NULL;
    int ret_mx = ares_parse_mx_reply(abuf, alen, &mx_out);
    if (ret_mx == ARES_SUCCESS && mx_out != NULL) {
        free_mx_reply(mx_out);
    }

    // Test ares_parse_a_reply
    struct hostent *host_a = NULL;
    struct ares_addrttl *addrttls = NULL;
    int naddrttls = 0;
    
    // First call with hostent only
    int ret_a1 = ares_parse_a_reply(abuf, alen, &host_a, NULL, NULL);
    if (ret_a1 == ARES_SUCCESS && host_a != NULL) {
        ares_free_hostent(host_a);
        host_a = NULL;
    }
    
    // Second call with addrttls
    int ret_a2 = ares_parse_a_reply(abuf, alen, &host_a, &addrttls, &naddrttls);
    if (ret_a2 == ARES_SUCCESS) {
        if (host_a != NULL) {
            ares_free_hostent(host_a);
        }
        if (addrttls != NULL) {
            free(addrttls);
        }
    }

    // Test ares_parse_ptr_reply
    struct hostent *host_ptr = NULL;
    
    // Prepare address parameters from fuzzer input
    // Use first 4 bytes for IPv4 address, or first 16 bytes for IPv6 address
    // If input is too small, use default values
    char addr_buf[16] = {0};
    int addrlen = 0;
    int family = AF_INET;
    
    if (Size >= 4) {
        // Use first 4 bytes for IPv4 address
        memcpy(addr_buf, Data, 4);
        addrlen = 4;
        family = AF_INET;
        
        int ret_ptr1 = ares_parse_ptr_reply(abuf, alen, addr_buf, addrlen, family, &host_ptr);
        if (ret_ptr1 == ARES_SUCCESS && host_ptr != NULL) {
            ares_free_hostent(host_ptr);
            host_ptr = NULL;
        }
    }
    
    if (Size >= 16) {
        // Use first 16 bytes for IPv6 address
        memcpy(addr_buf, Data, 16);
        addrlen = 16;
        family = AF_INET6;
        
        int ret_ptr2 = ares_parse_ptr_reply(abuf, alen, addr_buf, addrlen, family, &host_ptr);
        if (ret_ptr2 == ARES_SUCCESS && host_ptr != NULL) {
            ares_free_hostent(host_ptr);
        }
    }

    return 0;
}
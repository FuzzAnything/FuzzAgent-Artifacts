// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_aaaa_reply at ares_parse_aaaa_reply.c:51:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_aaaa_reply at ares_parse_aaaa_reply.c:51:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_txt_reply_ext at ares_parse_txt_reply.c:136:5 in ares.h
// ares_parse_caa_reply at ares_parse_caa_reply.c:30:5 in ares.h
// ares_parse_ns_reply at ares_parse_ns_reply.c:39:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
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

static void free_ares_txt_ext(struct ares_txt_ext *txt) {
    while (txt) {
        struct ares_txt_ext *next = txt->next;
        free(txt->txt);
        free(txt);
        txt = next;
    }
}

static void free_ares_caa_reply(struct ares_caa_reply *caa) {
    while (caa) {
        struct ares_caa_reply *next = caa->next;
        free(caa->property);
        free(caa->value);
        free(caa);
        caa = next;
    }
}

static void free_ares_mx_reply(struct ares_mx_reply *mx) {
    while (mx) {
        struct ares_mx_reply *next = mx->next;
        free(mx->host);
        free(mx);
        mx = next;
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    int alen = (Size > INT_MAX) ? INT_MAX : (int)Size;
    
    // Test ares_parse_txt_reply_ext
    {
        struct ares_txt_ext *txt_out = NULL;
        int ret = ares_parse_txt_reply_ext(Data, alen, &txt_out);
        if (ret == ARES_SUCCESS && txt_out) {
            free_ares_txt_ext(txt_out);
        }
    }
    
    // Test ares_parse_caa_reply
    {
        struct ares_caa_reply *caa_out = NULL;
        int ret = ares_parse_caa_reply(Data, alen, &caa_out);
        if (ret == ARES_SUCCESS && caa_out) {
            free_ares_caa_reply(caa_out);
        }
    }
    
    // Test ares_parse_ns_reply
    {
        struct hostent *host = NULL;
        int ret = ares_parse_ns_reply(Data, alen, &host);
        if (ret == ARES_SUCCESS && host) {
            ares_free_hostent(host);
        }
    }
    
    // Test ares_parse_mx_reply
    {
        struct ares_mx_reply *mx_out = NULL;
        int ret = ares_parse_mx_reply(Data, alen, &mx_out);
        if (ret == ARES_SUCCESS && mx_out) {
            free_ares_mx_reply(mx_out);
        }
    }
    
    // Test ares_parse_a_reply
    {
        struct hostent *host = NULL;
        struct ares_addrttl *addrttls = NULL;
        int naddrttls = 0;
        
        int ret = ares_parse_a_reply(Data, alen, &host, addrttls, &naddrttls);
        if (ret == ARES_SUCCESS) {
            if (host) ares_free_hostent(host);
            if (addrttls) free(addrttls);
        }
        
        // Test with allocated addrttls
        if (Size > 100) {
            host = NULL;
            addrttls = malloc(sizeof(struct ares_addrttl) * 10);
            naddrttls = 10;
            ret = ares_parse_a_reply(Data, alen, &host, addrttls, &naddrttls);
            if (ret == ARES_SUCCESS) {
                if (host) ares_free_hostent(host);
            }
            free(addrttls);
        }
    }
    
    // Test ares_parse_aaaa_reply
    {
        struct hostent *host = NULL;
        struct ares_addr6ttl *addrttls = NULL;
        int naddrttls = 0;
        
        int ret = ares_parse_aaaa_reply(Data, alen, &host, addrttls, &naddrttls);
        if (ret == ARES_SUCCESS) {
            if (host) ares_free_hostent(host);
            if (addrttls) free(addrttls);
        }
        
        // Test with allocated addrttls
        if (Size > 100) {
            host = NULL;
            addrttls = malloc(sizeof(struct ares_addr6ttl) * 10);
            naddrttls = 10;
            ret = ares_parse_aaaa_reply(Data, alen, &host, addrttls, &naddrttls);
            if (ret == ARES_SUCCESS) {
                if (host) ares_free_hostent(host);
            }
            free(addrttls);
        }
    }
    
    return 0;
}
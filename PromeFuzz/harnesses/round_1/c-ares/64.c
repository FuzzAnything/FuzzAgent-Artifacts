// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_caa_reply at ares_parse_caa_reply.c:30:5 in ares.h
// ares_parse_ns_reply at ares_parse_ns_reply.c:39:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_txt_reply_ext at ares_parse_txt_reply.c:136:5 in ares.h
// ares_parse_mx_reply at ares_parse_mx_reply.c:30:5 in ares.h
// ares_parse_aaaa_reply at ares_parse_aaaa_reply.c:51:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_aaaa_reply at ares_parse_aaaa_reply.c:51:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void free_ares_txt_ext(struct ares_txt_ext *txt) {
    while (txt) {
        struct ares_txt_ext *next = txt->next;
        free(txt->txt);
        free(txt);
        txt = next;
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

static void free_ares_caa_reply(struct ares_caa_reply *caa) {
    while (caa) {
        struct ares_caa_reply *next = caa->next;
        free(caa->property);
        free(caa->value);
        free(caa);
        caa = next;
    }
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    int alen = (Size > INT_MAX) ? INT_MAX : (int)Size;
    
    // Test ares_parse_ns_reply
    struct hostent *ns_host = NULL;
    ares_parse_ns_reply(Data, alen, &ns_host);
    if (ns_host) {
        ares_free_hostent(ns_host);
    }
    
    // Test ares_parse_txt_reply_ext
    struct ares_txt_ext *txt_out = NULL;
    ares_parse_txt_reply_ext(Data, alen, &txt_out);
    if (txt_out) {
        free_ares_txt_ext(txt_out);
    }
    
    // Test ares_parse_mx_reply
    struct ares_mx_reply *mx_out = NULL;
    ares_parse_mx_reply(Data, alen, &mx_out);
    if (mx_out) {
        free_ares_mx_reply(mx_out);
    }
    
    // Test ares_parse_aaaa_reply with different parameter combinations
    struct hostent *aaaa_host1 = NULL;
    ares_parse_aaaa_reply(Data, alen, &aaaa_host1, NULL, NULL);
    if (aaaa_host1) {
        ares_free_hostent(aaaa_host1);
    }
    
    struct hostent *aaaa_host2 = NULL;
    struct ares_addr6ttl addr6ttls[4];
    int naddr6ttls = 4;
    ares_parse_aaaa_reply(Data, alen, &aaaa_host2, addr6ttls, &naddr6ttls);
    if (aaaa_host2) {
        ares_free_hostent(aaaa_host2);
    }
    
    // Test ares_parse_a_reply with different parameter combinations
    struct hostent *a_host1 = NULL;
    ares_parse_a_reply(Data, alen, &a_host1, NULL, NULL);
    if (a_host1) {
        ares_free_hostent(a_host1);
    }
    
    struct hostent *a_host2 = NULL;
    struct ares_addrttl addrttls[4];
    int naddrttls = 4;
    ares_parse_a_reply(Data, alen, &a_host2, addrttls, &naddrttls);
    if (a_host2) {
        ares_free_hostent(a_host2);
    }
    
    // Test ares_parse_caa_reply
    struct ares_caa_reply *caa_out = NULL;
    ares_parse_caa_reply(Data, alen, &caa_out);
    if (caa_out) {
        free_ares_caa_reply(caa_out);
    }
    
    return 0;
}
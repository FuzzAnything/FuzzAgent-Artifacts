// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_mx_reply at ares_parse_mx_reply.c:30:5 in ares.h
// ares_parse_a_reply at ares_parse_a_reply.c:48:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_a_reply at ares_parse_a_reply.c:48:5 in ares.h
// ares_parse_caa_reply at ares_parse_caa_reply.c:30:5 in ares.h
// ares_parse_ns_reply at ares_parse_ns_reply.c:39:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_aaaa_reply at ares_parse_aaaa_reply.c:51:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_aaaa_reply at ares_parse_aaaa_reply.c:51:5 in ares.h
// ares_parse_txt_reply_ext at ares_parse_txt_reply.c:136:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include "ares.h"

static void free_mx_reply(struct ares_mx_reply *mx) {
    while (mx) {
        struct ares_mx_reply *next = mx->next;
        free(mx->host);
        free(mx);
        mx = next;
    }
}

static void free_txt_ext(struct ares_txt_ext *txt) {
    while (txt) {
        struct ares_txt_ext *next = txt->next;
        free(txt->txt);
        free(txt);
        txt = next;
    }
}

static void free_caa_reply(struct ares_caa_reply *caa) {
    while (caa) {
        struct ares_caa_reply *next = caa->next;
        free(caa->property);
        free(caa->value);
        free(caa);
        caa = next;
    }
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;

    const unsigned char *abuf = Data;
    int alen = (Size > INT_MAX) ? INT_MAX : (int)Size;

    // Test ares_parse_a_reply
    struct hostent *host_a = NULL;
    struct ares_addrttl *addrttls = NULL;
    int naddrttls = 0;
    int ret_a = ares_parse_a_reply(abuf, alen, &host_a, NULL, &naddrttls);
    if (ret_a == ARES_SUCCESS && host_a) {
        ares_free_hostent(host_a);
    }
    
    // Test with addrttls
    if (Size > 100) {
        int max_ttls = 10;
        addrttls = (struct ares_addrttl *)calloc(max_ttls, sizeof(struct ares_addrttl));
        naddrttls = max_ttls;
        ret_a = ares_parse_a_reply(abuf, alen, NULL, addrttls, &naddrttls);
        free(addrttls);
    }

    // Test ares_parse_caa_reply
    struct ares_caa_reply *caa_out = NULL;
    int ret_caa = ares_parse_caa_reply(abuf, alen, &caa_out);
    if (ret_caa == ARES_SUCCESS && caa_out) {
        free_caa_reply(caa_out);
    }

    // Test ares_parse_ns_reply
    struct hostent *host_ns = NULL;
    int ret_ns = ares_parse_ns_reply(abuf, alen, &host_ns);
    if (ret_ns == ARES_SUCCESS && host_ns) {
        ares_free_hostent(host_ns);
    }

    // Test ares_parse_aaaa_reply
    struct hostent *host_aaaa = NULL;
    struct ares_addr6ttl *addr6ttls = NULL;
    int naddr6ttls = 0;
    int ret_aaaa = ares_parse_aaaa_reply(abuf, alen, &host_aaaa, NULL, &naddr6ttls);
    if (ret_aaaa == ARES_SUCCESS && host_aaaa) {
        ares_free_hostent(host_aaaa);
    }
    
    // Test with addr6ttls
    if (Size > 200) {
        int max_ttls6 = 10;
        addr6ttls = (struct ares_addr6ttl *)calloc(max_ttls6, sizeof(struct ares_addr6ttl));
        naddr6ttls = max_ttls6;
        ret_aaaa = ares_parse_aaaa_reply(abuf, alen, NULL, addr6ttls, &naddr6ttls);
        free(addr6ttls);
    }

    // Test ares_parse_txt_reply_ext
    struct ares_txt_ext *txt_out = NULL;
    int ret_txt = ares_parse_txt_reply_ext(abuf, alen, &txt_out);
    if (ret_txt == ARES_SUCCESS && txt_out) {
        free_txt_ext(txt_out);
    }

    // Test ares_parse_mx_reply
    struct ares_mx_reply *mx_out = NULL;
    int ret_mx = ares_parse_mx_reply(abuf, alen, &mx_out);
    if (ret_mx == ARES_SUCCESS && mx_out) {
        free_mx_reply(mx_out);
    }

    return 0;
}
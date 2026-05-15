// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_txt_reply_ext at ares_parse_txt_reply.c:136:5 in ares.h
// ares_parse_ns_reply at ares_parse_ns_reply.c:39:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_aaaa_reply at ares_parse_aaaa_reply.c:51:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_mx_reply at ares_parse_mx_reply.c:30:5 in ares.h
// ares_parse_caa_reply at ares_parse_caa_reply.c:30:5 in ares.h
// ares_parse_a_reply at ares_parse_a_reply.c:48:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "ares.h"

static void free_txt_ext(struct ares_txt_ext *txt_out) {
    while (txt_out) {
        struct ares_txt_ext *next = txt_out->next;
        free(txt_out->txt);
        free(txt_out);
        txt_out = next;
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

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    // Limit size to avoid excessive memory usage
    if (Size > 65536) {
        return 0;
    }

    int alen = (Size > INT_MAX) ? INT_MAX : (int)Size;

    // Test ares_parse_txt_reply_ext
    struct ares_txt_ext *txt_out = NULL;
    int ret1 = ares_parse_txt_reply_ext(Data, alen, &txt_out);
    if (ret1 == ARES_SUCCESS && txt_out) {
        free_txt_ext(txt_out);
    }

    // Test ares_parse_ns_reply
    struct hostent *ns_host = NULL;
    int ret2 = ares_parse_ns_reply(Data, alen, &ns_host);
    if (ret2 == ARES_SUCCESS && ns_host) {
        ares_free_hostent(ns_host);
    }

    // Test ares_parse_aaaa_reply
    struct hostent *aaaa_host = NULL;
    struct ares_addr6ttl *addr6ttls = NULL;
    int naddr6ttls = 0;
    int ret3 = ares_parse_aaaa_reply(Data, alen, &aaaa_host, &addr6ttls, &naddr6ttls);
    if (ret3 == ARES_SUCCESS) {
        if (aaaa_host) {
            ares_free_hostent(aaaa_host);
        }
        free(addr6ttls);
    }

    // Test ares_parse_mx_reply
    struct ares_mx_reply *mx_out = NULL;
    int ret4 = ares_parse_mx_reply(Data, alen, &mx_out);
    if (ret4 == ARES_SUCCESS && mx_out) {
        free_mx_reply(mx_out);
    }

    // Test ares_parse_caa_reply
    struct ares_caa_reply *caa_out = NULL;
    int ret5 = ares_parse_caa_reply(Data, alen, &caa_out);
    if (ret5 == ARES_SUCCESS && caa_out) {
        free_caa_reply(caa_out);
    }

    // Test ares_parse_a_reply
    struct hostent *a_host = NULL;
    struct ares_addrttl *addrttls = NULL;
    int naddrttls = 0;
    int ret6 = ares_parse_a_reply(Data, alen, &a_host, &addrttls, &naddrttls);
    if (ret6 == ARES_SUCCESS) {
        if (a_host) {
            ares_free_hostent(a_host);
        }
        free(addrttls);
    }

    return 0;
}
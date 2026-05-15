// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_caa_reply at ares_parse_caa_reply.c:30:5 in ares.h
// ares_parse_ns_reply at ares_parse_ns_reply.c:39:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_mx_reply at ares_parse_mx_reply.c:30:5 in ares.h
// ares_parse_a_reply at ares_parse_a_reply.c:48:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_txt_reply_ext at ares_parse_txt_reply.c:136:5 in ares.h
// ares_parse_aaaa_reply at ares_parse_aaaa_reply.c:51:5 in ares.h
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

static void free_caa_reply(struct ares_caa_reply *caa) {
    while (caa) {
        struct ares_caa_reply *next = caa->next;
        free(caa->property);
        free(caa->value);
        free(caa);
        caa = next;
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

static void free_txt_ext(struct ares_txt_ext *txt) {
    while (txt) {
        struct ares_txt_ext *next = txt->next;
        free(txt->txt);
        free(txt);
        txt = next;
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test ares_parse_caa_reply
    struct ares_caa_reply *caa_out = NULL;
    ares_parse_caa_reply(Data, (int)Size, &caa_out);
    if (caa_out) free_caa_reply(caa_out);

    // Test ares_parse_ns_reply
    struct hostent *host_ns = NULL;
    ares_parse_ns_reply(Data, (int)Size, &host_ns);
    if (host_ns) ares_free_hostent(host_ns);

    // Test ares_parse_mx_reply
    struct ares_mx_reply *mx_out = NULL;
    ares_parse_mx_reply(Data, (int)Size, &mx_out);
    if (mx_out) free_mx_reply(mx_out);

    // Test ares_parse_a_reply
    struct hostent *host_a = NULL;
    struct ares_addrttl addrttls[10];
    int naddrttls = 10;
    ares_parse_a_reply(Data, (int)Size, &host_a, addrttls, &naddrttls);
    if (host_a) ares_free_hostent(host_a);

    // Test ares_parse_txt_reply_ext
    struct ares_txt_ext *txt_out = NULL;
    ares_parse_txt_reply_ext(Data, (int)Size, &txt_out);
    if (txt_out) free_txt_ext(txt_out);

    // Test ares_parse_aaaa_reply
    struct hostent *host_aaaa = NULL;
    struct ares_addr6ttl addr6ttls[10];
    int naddr6ttls = 10;
    ares_parse_aaaa_reply(Data, (int)Size, &host_aaaa, addr6ttls, &naddr6ttls);
    if (host_aaaa) ares_free_hostent(host_aaaa);

    return 0;
}
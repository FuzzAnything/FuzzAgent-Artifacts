// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_ns_reply at ares_parse_ns_reply.c:39:5 in ares.h
// ares_parse_txt_reply_ext at ares_parse_txt_reply.c:136:5 in ares.h
// ares_parse_mx_reply at ares_parse_mx_reply.c:30:5 in ares.h
// ares_parse_aaaa_reply at ares_parse_aaaa_reply.c:51:5 in ares.h
// ares_parse_aaaa_reply at ares_parse_aaaa_reply.c:51:5 in ares.h
// ares_parse_a_reply at ares_parse_a_reply.c:48:5 in ares.h
// ares_parse_a_reply at ares_parse_a_reply.c:48:5 in ares.h
// ares_parse_caa_reply at ares_parse_caa_reply.c:30:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

static void cleanup_mx_reply(struct ares_mx_reply *mx_out) {
    while (mx_out != NULL) {
        struct ares_mx_reply *next = mx_out->next;
        free(mx_out->host);
        free(mx_out);
        mx_out = next;
    }
}

static void cleanup_caa_reply(struct ares_caa_reply *caa_out) {
    while (caa_out != NULL) {
        struct ares_caa_reply *next = caa_out->next;
        free(caa_out->property);
        free(caa_out->value);
        free(caa_out);
        caa_out = next;
    }
}

static void cleanup_txt_ext(struct ares_txt_ext *txt_out) {
    while (txt_out != NULL) {
        struct ares_txt_ext *next = txt_out->next;
        free(txt_out->txt);
        free(txt_out);
        txt_out = next;
    }
}

static void cleanup_hostent(struct hostent *host) {
    if (host != NULL) {
        // Use ares_free_hostent if available, otherwise just free the pointer
        #ifdef HAVE_ARES_FREE_HOSTENT
        ares_free_hostent(host);
        #else
        free(host);
        #endif
    }
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test ares_parse_ns_reply
    struct hostent *host_ns = NULL;
    ares_parse_ns_reply(Data, (int)Size, &host_ns);
    cleanup_hostent(host_ns);

    // Test ares_parse_txt_reply_ext
    struct ares_txt_ext *txt_out = NULL;
    ares_parse_txt_reply_ext(Data, (int)Size, &txt_out);
    if (txt_out != NULL) {
        cleanup_txt_ext(txt_out);
    }

    // Test ares_parse_mx_reply
    struct ares_mx_reply *mx_out = NULL;
    ares_parse_mx_reply(Data, (int)Size, &mx_out);
    if (mx_out != NULL) {
        cleanup_mx_reply(mx_out);
    }

    // Test ares_parse_aaaa_reply
    struct hostent *host_aaaa = NULL;
    struct ares_addr6ttl *addr6ttls = NULL;
    int naddr6ttls = 0;
    if (Size > sizeof(struct ares_addr6ttl)) {
        addr6ttls = (struct ares_addr6ttl *)malloc(sizeof(struct ares_addr6ttl));
        naddr6ttls = 1;
        ares_parse_aaaa_reply(Data, (int)Size, &host_aaaa, addr6ttls, &naddr6ttls);
        free(addr6ttls);
    } else {
        ares_parse_aaaa_reply(Data, (int)Size, &host_aaaa, NULL, NULL);
    }
    cleanup_hostent(host_aaaa);

    // Test ares_parse_a_reply
    struct hostent *host_a = NULL;
    struct ares_addrttl *addrttls = NULL;
    int naddrttls = 0;
    if (Size > sizeof(struct ares_addrttl)) {
        addrttls = (struct ares_addrttl *)malloc(sizeof(struct ares_addrttl));
        naddrttls = 1;
        ares_parse_a_reply(Data, (int)Size, &host_a, addrttls, &naddrttls);
        free(addrttls);
    } else {
        ares_parse_a_reply(Data, (int)Size, &host_a, NULL, NULL);
    }
    cleanup_hostent(host_a);

    // Test ares_parse_caa_reply
    struct ares_caa_reply *caa_out = NULL;
    ares_parse_caa_reply(Data, (int)Size, &caa_out);
    if (caa_out != NULL) {
        cleanup_caa_reply(caa_out);
    }

    return 0;
}
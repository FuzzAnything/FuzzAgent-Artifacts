// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_caa_reply at ares_parse_caa_reply.c:30:5 in ares.h
// ares_parse_uri_reply at ares_parse_uri_reply.c:30:5 in ares.h
// ares_parse_srv_reply at ares_parse_srv_reply.c:30:5 in ares.h
// ares_parse_txt_reply_ext at ares_parse_txt_reply.c:136:5 in ares.h
// ares_parse_mx_reply at ares_parse_mx_reply.c:30:5 in ares.h
// ares_parse_soa_reply at ares_parse_soa_reply.c:30:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

static void free_txt_ext(struct ares_txt_ext *txt) {
    while (txt) {
        struct ares_txt_ext *next = txt->next;
        if (txt->txt) {
            free(txt->txt);
        }
        free(txt);
        txt = next;
    }
}

static void free_mx_reply(struct ares_mx_reply *mx) {
    while (mx) {
        struct ares_mx_reply *next = mx->next;
        if (mx->host) {
            free(mx->host);
        }
        free(mx);
        mx = next;
    }
}

static void free_soa_reply(struct ares_soa_reply *soa) {
    if (soa) {
        if (soa->nsname) {
            free(soa->nsname);
        }
        if (soa->hostmaster) {
            free(soa->hostmaster);
        }
        free(soa);
    }
}

static void free_caa_reply(struct ares_caa_reply *caa) {
    while (caa) {
        struct ares_caa_reply *next = caa->next;
        if (caa->property) {
            free(caa->property);
        }
        if (caa->value) {
            free(caa->value);
        }
        free(caa);
        caa = next;
    }
}

static void free_uri_reply(struct ares_uri_reply *uri) {
    while (uri) {
        struct ares_uri_reply *next = uri->next;
        if (uri->uri) {
            free(uri->uri);
        }
        free(uri);
        uri = next;
    }
}

static void free_srv_reply(struct ares_srv_reply *srv) {
    while (srv) {
        struct ares_srv_reply *next = srv->next;
        if (srv->host) {
            free(srv->host);
        }
        free(srv);
        srv = next;
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test ares_parse_txt_reply_ext
    struct ares_txt_ext *txt_out = NULL;
    int ret_txt = ares_parse_txt_reply_ext(Data, (int)Size, &txt_out);
    if (ret_txt == ARES_SUCCESS && txt_out) {
        free_txt_ext(txt_out);
    }

    // Test ares_parse_mx_reply
    struct ares_mx_reply *mx_out = NULL;
    int ret_mx = ares_parse_mx_reply(Data, (int)Size, &mx_out);
    if (ret_mx == ARES_SUCCESS && mx_out) {
        free_mx_reply(mx_out);
    }

    // Test ares_parse_soa_reply
    struct ares_soa_reply *soa_out = NULL;
    int ret_soa = ares_parse_soa_reply(Data, (int)Size, &soa_out);
    if (ret_soa == ARES_SUCCESS && soa_out) {
        free_soa_reply(soa_out);
    }

    // Test ares_parse_caa_reply
    struct ares_caa_reply *caa_out = NULL;
    int ret_caa = ares_parse_caa_reply(Data, (int)Size, &caa_out);
    if (ret_caa == ARES_SUCCESS && caa_out) {
        free_caa_reply(caa_out);
    }

    // Test ares_parse_uri_reply
    struct ares_uri_reply *uri_out = NULL;
    int ret_uri = ares_parse_uri_reply(Data, (int)Size, &uri_out);
    if (ret_uri == ARES_SUCCESS && uri_out) {
        free_uri_reply(uri_out);
    }

    // Test ares_parse_srv_reply
    struct ares_srv_reply *srv_out = NULL;
    int ret_srv = ares_parse_srv_reply(Data, (int)Size, &srv_out);
    if (ret_srv == ARES_SUCCESS && srv_out) {
        free_srv_reply(srv_out);
    }

    return 0;
}
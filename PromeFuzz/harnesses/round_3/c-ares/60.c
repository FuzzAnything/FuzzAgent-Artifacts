// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_txt_reply_ext at ares_parse_txt_reply.c:136:5 in ares.h
// ares_parse_caa_reply at ares_parse_caa_reply.c:30:5 in ares.h
// ares_parse_srv_reply at ares_parse_srv_reply.c:30:5 in ares.h
// ares_parse_naptr_reply at ares_parse_naptr_reply.c:29:5 in ares.h
// ares_parse_soa_reply at ares_parse_soa_reply.c:30:5 in ares.h
// ares_parse_uri_reply at ares_parse_uri_reply.c:30:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "ares.h"

static void free_caa_reply(struct ares_caa_reply *reply) {
    while (reply) {
        struct ares_caa_reply *next = reply->next;
        free(reply->property);
        free(reply->value);
        free(reply);
        reply = next;
    }
}

static void free_srv_reply(struct ares_srv_reply *reply) {
    while (reply) {
        struct ares_srv_reply *next = reply->next;
        free(reply->host);
        free(reply);
        reply = next;
    }
}

static void free_naptr_reply(struct ares_naptr_reply *reply) {
    while (reply) {
        struct ares_naptr_reply *next = reply->next;
        free(reply->flags);
        free(reply->service);
        free(reply->regexp);
        free(reply->replacement);
        free(reply);
        reply = next;
    }
}

static void free_soa_reply(struct ares_soa_reply *reply) {
    if (reply) {
        free(reply->nsname);
        free(reply->hostmaster);
        free(reply);
    }
}

static void free_uri_reply(struct ares_uri_reply *reply) {
    while (reply) {
        struct ares_uri_reply *next = reply->next;
        free(reply->uri);
        free(reply);
        reply = next;
    }
}

static void free_txt_ext(struct ares_txt_ext *reply) {
    while (reply) {
        struct ares_txt_ext *next = reply->next;
        free(reply->txt);
        free(reply);
        reply = next;
    }
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    unsigned char *abuf = (unsigned char *)Data;
    int alen = (Size > INT_MAX) ? INT_MAX : (int)Size;

    struct ares_caa_reply *caa_out = NULL;
    int caa_ret = ares_parse_caa_reply(abuf, alen, &caa_out);
    if (caa_ret == ARES_SUCCESS && caa_out != NULL) {
        free_caa_reply(caa_out);
    }

    struct ares_srv_reply *srv_out = NULL;
    int srv_ret = ares_parse_srv_reply(abuf, alen, &srv_out);
    if (srv_ret == ARES_SUCCESS && srv_out != NULL) {
        free_srv_reply(srv_out);
    }

    struct ares_naptr_reply *naptr_out = NULL;
    int naptr_ret = ares_parse_naptr_reply(abuf, alen, &naptr_out);
    if (naptr_ret == ARES_SUCCESS && naptr_out != NULL) {
        free_naptr_reply(naptr_out);
    }

    struct ares_soa_reply *soa_out = NULL;
    int soa_ret = ares_parse_soa_reply(abuf, alen, &soa_out);
    if (soa_ret == ARES_SUCCESS && soa_out != NULL) {
        free_soa_reply(soa_out);
    }

    struct ares_uri_reply *uri_out = NULL;
    int uri_ret = ares_parse_uri_reply(abuf, alen, &uri_out);
    if (uri_ret == ARES_SUCCESS && uri_out != NULL) {
        free_uri_reply(uri_out);
    }

    struct ares_txt_ext *txt_out = NULL;
    int txt_ret = ares_parse_txt_reply_ext(abuf, alen, &txt_out);
    if (txt_ret == ARES_SUCCESS && txt_out != NULL) {
        free_txt_ext(txt_out);
    }

    return 0;
}
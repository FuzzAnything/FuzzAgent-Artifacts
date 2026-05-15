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

static void free_caa_reply(struct ares_caa_reply *caa) {
    while (caa) {
        struct ares_caa_reply *next = caa->next;
        free(caa->property);
        free(caa->value);
        free(caa);
        caa = next;
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

static void free_uri_reply(struct ares_uri_reply *uri) {
    while (uri) {
        struct ares_uri_reply *next = uri->next;
        free(uri->uri);
        free(uri);
        uri = next;
    }
}

static void free_soa_reply(struct ares_soa_reply *soa) {
    if (soa) {
        free(soa->nsname);
        free(soa->hostmaster);
        free(soa);
    }
}

static void free_naptr_reply(struct ares_naptr_reply *naptr) {
    while (naptr) {
        struct ares_naptr_reply *next = naptr->next;
        free(naptr->flags);
        free(naptr->service);
        free(naptr->regexp);
        free(naptr->replacement);
        free(naptr);
        naptr = next;
    }
}

static void free_srv_reply(struct ares_srv_reply *srv) {
    while (srv) {
        struct ares_srv_reply *next = srv->next;
        free(srv->host);
        free(srv);
        srv = next;
    }
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    const unsigned char *abuf = Data;
    int alen = (Size > INT_MAX) ? INT_MAX : (int)Size;
    
    struct ares_caa_reply *caa_out = NULL;
    int caa_status = ares_parse_caa_reply(abuf, alen, &caa_out);
    if (caa_status == ARES_SUCCESS) {
        free_caa_reply(caa_out);
    }
    
    struct ares_srv_reply *srv_out = NULL;
    int srv_status = ares_parse_srv_reply(abuf, alen, &srv_out);
    if (srv_status == ARES_SUCCESS) {
        free_srv_reply(srv_out);
    }
    
    struct ares_naptr_reply *naptr_out = NULL;
    int naptr_status = ares_parse_naptr_reply(abuf, alen, &naptr_out);
    if (naptr_status == ARES_SUCCESS) {
        free_naptr_reply(naptr_out);
    }
    
    struct ares_soa_reply *soa_out = NULL;
    int soa_status = ares_parse_soa_reply(abuf, alen, &soa_out);
    if (soa_status == ARES_SUCCESS) {
        free_soa_reply(soa_out);
    }
    
    struct ares_uri_reply *uri_out = NULL;
    int uri_status = ares_parse_uri_reply(abuf, alen, &uri_out);
    if (uri_status == ARES_SUCCESS) {
        free_uri_reply(uri_out);
    }
    
    struct ares_txt_ext *txt_out = NULL;
    int txt_status = ares_parse_txt_reply_ext(abuf, alen, &txt_out);
    if (txt_status == ARES_SUCCESS) {
        free_txt_ext(txt_out);
    }
    
    return 0;
}
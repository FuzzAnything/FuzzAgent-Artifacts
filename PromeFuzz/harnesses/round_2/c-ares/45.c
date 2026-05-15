// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_caa_reply at ares_parse_caa_reply.c:30:5 in ares.h
// ares_parse_uri_reply at ares_parse_uri_reply.c:30:5 in ares.h
// ares_parse_txt_reply_ext at ares_parse_txt_reply.c:136:5 in ares.h
// ares_parse_txt_reply at ares_parse_txt_reply.c:126:5 in ares.h
// ares_parse_naptr_reply at ares_parse_naptr_reply.c:29:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_txt_ext(struct ares_txt_ext *txt) {
    struct ares_txt_ext *next;
    while (txt) {
        next = txt->next;
        free(txt->txt);
        free(txt);
        txt = next;
    }
}

static void cleanup_txt_reply(struct ares_txt_reply *txt) {
    struct ares_txt_reply *next;
    while (txt) {
        next = txt->next;
        free(txt->txt);
        free(txt);
        txt = next;
    }
}

static void cleanup_naptr(struct ares_naptr_reply *naptr) {
    struct ares_naptr_reply *next;
    while (naptr) {
        next = naptr->next;
        free(naptr->flags);
        free(naptr->service);
        free(naptr->regexp);
        free(naptr->replacement);
        free(naptr);
        naptr = next;
    }
}

static void cleanup_caa(struct ares_caa_reply *caa) {
    struct ares_caa_reply *next;
    while (caa) {
        next = caa->next;
        free(caa->property);
        free(caa->value);
        free(caa);
        caa = next;
    }
}

static void cleanup_uri(struct ares_uri_reply *uri) {
    struct ares_uri_reply *next;
    while (uri) {
        next = uri->next;
        free(uri->uri);
        free(uri);
        uri = next;
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Make a mutable copy of the data
    unsigned char *buf = (unsigned char *)malloc(Size);
    if (!buf) {
        return 0;
    }
    memcpy(buf, Data, Size);
    int alen = (int)Size;

    // Test ares_parse_txt_reply_ext
    struct ares_txt_ext *txt_ext = NULL;
    int ret = ares_parse_txt_reply_ext(buf, alen, &txt_ext);
    if (ret == ARES_SUCCESS && txt_ext) {
        cleanup_txt_ext(txt_ext);
    }

    // Test ares_parse_txt_reply
    struct ares_txt_reply *txt = NULL;
    ret = ares_parse_txt_reply(buf, alen, &txt);
    if (ret == ARES_SUCCESS && txt) {
        cleanup_txt_reply(txt);
    }

    // Test ares_parse_naptr_reply
    struct ares_naptr_reply *naptr = NULL;
    ret = ares_parse_naptr_reply(buf, alen, &naptr);
    if (ret == ARES_SUCCESS && naptr) {
        cleanup_naptr(naptr);
    }

    // Test ares_parse_caa_reply
    struct ares_caa_reply *caa = NULL;
    ret = ares_parse_caa_reply(buf, alen, &caa);
    if (ret == ARES_SUCCESS && caa) {
        cleanup_caa(caa);
    }

    // Test ares_parse_uri_reply
    struct ares_uri_reply *uri = NULL;
    ret = ares_parse_uri_reply(buf, alen, &uri);
    if (ret == ARES_SUCCESS && uri) {
        cleanup_uri(uri);
    }

    free(buf);
    return 0;
}
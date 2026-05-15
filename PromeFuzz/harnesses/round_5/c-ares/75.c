// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_naptr_reply at ares_parse_naptr_reply.c:29:5 in ares.h
// ares_parse_soa_reply at ares_parse_soa_reply.c:30:5 in ares.h
// ares_parse_uri_reply at ares_parse_uri_reply.c:30:5 in ares.h
// ares_parse_txt_reply_ext at ares_parse_txt_reply.c:136:5 in ares.h
// ares_parse_caa_reply at ares_parse_caa_reply.c:30:5 in ares.h
// ares_parse_txt_reply at ares_parse_txt_reply.c:126:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

static void free_ares_caa_reply(struct ares_caa_reply *caa) {
    while (caa) {
        struct ares_caa_reply *next = caa->next;
        free(caa->property);
        free(caa->value);
        free(caa);
        caa = next;
    }
}

static void free_ares_txt_reply(struct ares_txt_reply *txt) {
    while (txt) {
        struct ares_txt_reply *next = txt->next;
        free(txt->txt);
        free(txt);
        txt = next;
    }
}

static void free_ares_naptr_reply(struct ares_naptr_reply *naptr) {
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

static void free_ares_soa_reply(struct ares_soa_reply *soa) {
    if (soa) {
        free(soa->nsname);
        free(soa->hostmaster);
        free(soa);
    }
}

static void free_ares_uri_reply(struct ares_uri_reply *uri) {
    while (uri) {
        struct ares_uri_reply *next = uri->next;
        free(uri->uri);
        free(uri);
        uri = next;
    }
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a mutable copy of the input data
    unsigned char *abuf = (unsigned char *)malloc(Size);
    if (!abuf) {
        return 0;
    }
    memcpy(abuf, Data, Size);
    int alen = (int)Size;

    // Test ares_parse_txt_reply_ext
    struct ares_txt_ext *txt_ext_out = NULL;
    int ret1 = ares_parse_txt_reply_ext(abuf, alen, &txt_ext_out);
    if (ret1 == ARES_SUCCESS) {
        free_ares_txt_ext(txt_ext_out);
    }

    // Test ares_parse_caa_reply
    struct ares_caa_reply *caa_out = NULL;
    int ret2 = ares_parse_caa_reply(abuf, alen, &caa_out);
    if (ret2 == ARES_SUCCESS) {
        free_ares_caa_reply(caa_out);
    }

    // Test ares_parse_txt_reply
    struct ares_txt_reply *txt_out = NULL;
    int ret3 = ares_parse_txt_reply(abuf, alen, &txt_out);
    if (ret3 == ARES_SUCCESS) {
        free_ares_txt_reply(txt_out);
    }

    // Test ares_parse_naptr_reply
    struct ares_naptr_reply *naptr_out = NULL;
    int ret4 = ares_parse_naptr_reply(abuf, alen, &naptr_out);
    if (ret4 == ARES_SUCCESS) {
        free_ares_naptr_reply(naptr_out);
    }

    // Test ares_parse_soa_reply
    struct ares_soa_reply *soa_out = NULL;
    int ret5 = ares_parse_soa_reply(abuf, alen, &soa_out);
    if (ret5 == ARES_SUCCESS) {
        free_ares_soa_reply(soa_out);
    }

    // Test ares_parse_uri_reply
    struct ares_uri_reply *uri_out = NULL;
    int ret6 = ares_parse_uri_reply(abuf, alen, &uri_out);
    if (ret6 == ARES_SUCCESS) {
        free_ares_uri_reply(uri_out);
    }

    free(abuf);
    return 0;
}
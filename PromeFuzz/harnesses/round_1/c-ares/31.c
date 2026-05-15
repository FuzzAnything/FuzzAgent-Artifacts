// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_txt_reply at ares_parse_txt_reply.c:126:5 in ares.h
// ares_parse_naptr_reply at ares_parse_naptr_reply.c:29:5 in ares.h
// ares_parse_uri_reply at ares_parse_uri_reply.c:30:5 in ares.h
// ares_parse_mx_reply at ares_parse_mx_reply.c:30:5 in ares.h
// ares_parse_caa_reply at ares_parse_caa_reply.c:30:5 in ares.h
// ares_parse_txt_reply_ext at ares_parse_txt_reply.c:136:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void free_txt_ext(struct ares_txt_ext *txt) {
    while (txt) {
        struct ares_txt_ext *next = txt->next;
        free(txt->txt);
        free(txt);
        txt = next;
    }
}

static void free_txt_reply(struct ares_txt_reply *txt) {
    while (txt) {
        struct ares_txt_reply *next = txt->next;
        free(txt->txt);
        free(txt);
        txt = next;
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

static void free_uri_reply(struct ares_uri_reply *uri) {
    while (uri) {
        struct ares_uri_reply *next = uri->next;
        free(uri->uri);
        free(uri);
        uri = next;
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

static void free_caa_reply(struct ares_caa_reply *caa) {
    while (caa) {
        struct ares_caa_reply *next = caa->next;
        free(caa->property);
        free(caa->value);
        free(caa);
        caa = next;
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Limit size to avoid timeouts with very large inputs
    if (Size > 65536) {
        Size = 65536;
    }

    // Create a mutable copy of the data
    unsigned char *buf = (unsigned char *)malloc(Size);
    if (!buf) {
        return 0;
    }
    memcpy(buf, Data, Size);

    int alen = (int)Size;

    // Test ares_parse_txt_reply_ext
    {
        struct ares_txt_ext *txt_ext_out = NULL;
        int ret = ares_parse_txt_reply_ext(buf, alen, &txt_ext_out);
        (void)ret; // Return value ignored for fuzzing
        if (txt_ext_out) {
            free_txt_ext(txt_ext_out);
        }
    }

    // Test ares_parse_txt_reply
    {
        struct ares_txt_reply *txt_out = NULL;
        int ret = ares_parse_txt_reply(buf, alen, &txt_out);
        (void)ret;
        if (txt_out) {
            free_txt_reply(txt_out);
        }
    }

    // Test ares_parse_naptr_reply
    {
        struct ares_naptr_reply *naptr_out = NULL;
        int ret = ares_parse_naptr_reply(buf, alen, &naptr_out);
        (void)ret;
        if (naptr_out) {
            free_naptr_reply(naptr_out);
        }
    }

    // Test ares_parse_uri_reply
    {
        struct ares_uri_reply *uri_out = NULL;
        int ret = ares_parse_uri_reply(buf, alen, &uri_out);
        (void)ret;
        if (uri_out) {
            free_uri_reply(uri_out);
        }
    }

    // Test ares_parse_mx_reply
    {
        struct ares_mx_reply *mx_out = NULL;
        int ret = ares_parse_mx_reply(buf, alen, &mx_out);
        (void)ret;
        if (mx_out) {
            free_mx_reply(mx_out);
        }
    }

    // Test ares_parse_caa_reply
    {
        struct ares_caa_reply *caa_out = NULL;
        int ret = ares_parse_caa_reply(buf, alen, &caa_out);
        (void)ret;
        if (caa_out) {
            free_caa_reply(caa_out);
        }
    }

    free(buf);
    return 0;
}
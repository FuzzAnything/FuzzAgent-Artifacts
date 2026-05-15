// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_txt_reply_ext at ares_parse_txt_reply.c:136:5 in ares.h
// ares_parse_aaaa_reply at ares_parse_aaaa_reply.c:51:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_txt_reply at ares_parse_txt_reply.c:126:5 in ares.h
// ares_parse_naptr_reply at ares_parse_naptr_reply.c:29:5 in ares.h
// ares_parse_soa_reply at ares_parse_soa_reply.c:30:5 in ares.h
// ares_parse_mx_reply at ares_parse_mx_reply.c:30:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void free_txt_reply(struct ares_txt_reply *txt_out) {
    while (txt_out) {
        struct ares_txt_reply *next = txt_out->next;
        free(txt_out->txt);
        free(txt_out);
        txt_out = next;
    }
}

static void free_txt_ext(struct ares_txt_ext *txt_out) {
    while (txt_out) {
        struct ares_txt_ext *next = txt_out->next;
        free(txt_out->txt);
        free(txt_out);
        txt_out = next;
    }
}

static void free_naptr_reply(struct ares_naptr_reply *naptr_out) {
    while (naptr_out) {
        struct ares_naptr_reply *next = naptr_out->next;
        free(naptr_out->flags);
        free(naptr_out->service);
        free(naptr_out->regexp);
        free(naptr_out->replacement);
        free(naptr_out);
        naptr_out = next;
    }
}

static void free_soa_reply(struct ares_soa_reply *soa_out) {
    if (soa_out) {
        free(soa_out->nsname);
        free(soa_out->hostmaster);
        free(soa_out);
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

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use first byte to select function to test
    uint8_t selector = Data[0];
    const unsigned char *abuf = Data + 1;
    int alen = Size - 1;
    if (alen < 0) {
        alen = 0;
    }

    switch (selector % 6) {
        case 0: {
            struct ares_txt_reply *txt_out = NULL;
            int result = ares_parse_txt_reply(abuf, alen, &txt_out);
            (void)result;
            free_txt_reply(txt_out);
            break;
        }
        case 1: {
            struct ares_naptr_reply *naptr_out = NULL;
            int result = ares_parse_naptr_reply(abuf, alen, &naptr_out);
            (void)result;
            free_naptr_reply(naptr_out);
            break;
        }
        case 2: {
            struct ares_soa_reply *soa_out = NULL;
            int result = ares_parse_soa_reply(abuf, alen, &soa_out);
            (void)result;
            free_soa_reply(soa_out);
            break;
        }
        case 3: {
            struct ares_mx_reply *mx_out = NULL;
            int result = ares_parse_mx_reply(abuf, alen, &mx_out);
            (void)result;
            free_mx_reply(mx_out);
            break;
        }
        case 4: {
            struct ares_txt_ext *txt_out = NULL;
            int result = ares_parse_txt_reply_ext(abuf, alen, &txt_out);
            (void)result;
            free_txt_ext(txt_out);
            break;
        }
        case 5: {
            struct hostent *host = NULL;
            struct ares_addr6ttl addrttls[10];
            int naddrttls = 10;
            int result = ares_parse_aaaa_reply(abuf, alen, &host, addrttls, &naddrttls);
            (void)result;
            if (host) {
                ares_free_hostent(host);
            }
            break;
        }
    }

    return 0;
}
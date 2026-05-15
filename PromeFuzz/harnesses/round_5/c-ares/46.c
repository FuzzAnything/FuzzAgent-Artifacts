// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_srv_reply at ares_parse_srv_reply.c:30:5 in ares.h
// ares_parse_uri_reply at ares_parse_uri_reply.c:30:5 in ares.h
// ares_parse_ptr_reply at ares_parse_ptr_reply.c:186:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
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

static void free_soa_reply(struct ares_soa_reply *soa) {
    if (soa) {
        free(soa->nsname);
        free(soa->hostmaster);
        free(soa);
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

static void free_srv_reply(struct ares_srv_reply *srv) {
    while (srv) {
        struct ares_srv_reply *next = srv->next;
        free(srv->host);
        free(srv);
        srv = next;
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test ares_parse_ptr_reply
    {
        struct hostent *host = NULL;
        int family = (Data[0] % 2) ? AF_INET : AF_INET6;
        int addrlen = (family == AF_INET) ? 4 : 16;
        unsigned char addr[16] = {0};
        size_t addr_copy_size = (Size > sizeof(addr)) ? sizeof(addr) : Size;
        memcpy(addr, Data, addr_copy_size);
        
        int result = ares_parse_ptr_reply(Data, (int)Size, addr, addrlen, family, &host);
        if (result == ARES_SUCCESS && host != NULL) {
            ares_free_hostent(host);
        }
    }

    // Test ares_parse_naptr_reply
    {
        struct ares_naptr_reply *naptr_out = NULL;
        int result = ares_parse_naptr_reply(Data, (int)Size, &naptr_out);
        if (result == ARES_SUCCESS && naptr_out != NULL) {
            free_naptr_reply(naptr_out);
        }
    }

    // Test ares_parse_soa_reply
    {
        struct ares_soa_reply *soa_out = NULL;
        int result = ares_parse_soa_reply(Data, (int)Size, &soa_out);
        if (result == ARES_SUCCESS && soa_out != NULL) {
            free_soa_reply(soa_out);
        }
    }

    // Test ares_parse_mx_reply
    {
        struct ares_mx_reply *mx_out = NULL;
        int result = ares_parse_mx_reply(Data, (int)Size, &mx_out);
        if (result == ARES_SUCCESS && mx_out != NULL) {
            free_mx_reply(mx_out);
        }
    }

    // Test ares_parse_srv_reply
    {
        struct ares_srv_reply *srv_out = NULL;
        int result = ares_parse_srv_reply(Data, (int)Size, &srv_out);
        if (result == ARES_SUCCESS && srv_out != NULL) {
            free_srv_reply(srv_out);
        }
    }

    // Test ares_parse_uri_reply
    {
        struct ares_uri_reply *uri_out = NULL;
        int result = ares_parse_uri_reply(Data, (int)Size, &uri_out);
        if (result == ARES_SUCCESS && uri_out != NULL) {
            free_uri_reply(uri_out);
        }
    }

    return 0;
}
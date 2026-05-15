// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_naptr_reply at ares_parse_naptr_reply.c:29:5 in ares.h
// ares_free_data at ares_data.c:46:6 in ares.h
// ares_parse_uri_reply at ares_parse_uri_reply.c:30:5 in ares.h
// ares_free_data at ares_data.c:46:6 in ares.h
// ares_parse_mx_reply at ares_parse_mx_reply.c:30:5 in ares.h
// ares_free_data at ares_data.c:46:6 in ares.h
// ares_parse_ptr_reply at ares_parse_ptr_reply.c:186:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_ptr_reply at ares_parse_ptr_reply.c:186:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_srv_reply at ares_parse_srv_reply.c:30:5 in ares.h
// ares_free_data at ares_data.c:46:6 in ares.h
// ares_parse_soa_reply at ares_parse_soa_reply.c:30:5 in ares.h
// ares_free_data at ares_data.c:46:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

static void test_ares_parse_naptr_reply(const unsigned char *data, size_t size) {
    struct ares_naptr_reply *naptr_out = NULL;
    ares_parse_naptr_reply(data, (int)size, &naptr_out);
    if (naptr_out) {
        ares_free_data(naptr_out);
    }
}

static void test_ares_parse_uri_reply(const unsigned char *data, size_t size) {
    struct ares_uri_reply *uri_out = NULL;
    ares_parse_uri_reply(data, (int)size, &uri_out);
    if (uri_out) {
        ares_free_data(uri_out);
    }
}

static void test_ares_parse_mx_reply(const unsigned char *data, size_t size) {
    struct ares_mx_reply *mx_out = NULL;
    ares_parse_mx_reply(data, (int)size, &mx_out);
    if (mx_out) {
        ares_free_data(mx_out);
    }
}

static void test_ares_parse_ptr_reply(const unsigned char *data, size_t size) {
    struct hostent *host = NULL;
    
    if (size >= 4) {
        // For IPv4 address
        char addr4[4];
        for (int i = 0; i < 4 && i < (int)size; i++) {
            addr4[i] = data[i];
        }
        ares_parse_ptr_reply(data, (int)size, addr4, 4, AF_INET, &host);
        if (host) {
            ares_free_hostent(host);
        }
    }
    
    if (size >= 16) {
        // For IPv6 address
        char addr6[16];
        for (int i = 0; i < 16 && i < (int)size; i++) {
            addr6[i] = data[i];
        }
        ares_parse_ptr_reply(data, (int)size, addr6, 16, AF_INET6, &host);
        if (host) {
            ares_free_hostent(host);
        }
    }
}

static void test_ares_parse_srv_reply(const unsigned char *data, size_t size) {
    struct ares_srv_reply *srv_out = NULL;
    ares_parse_srv_reply(data, (int)size, &srv_out);
    if (srv_out) {
        ares_free_data(srv_out);
    }
}

static void test_ares_parse_soa_reply(const unsigned char *data, size_t size) {
    struct ares_soa_reply *soa_out = NULL;
    ares_parse_soa_reply(data, (int)size, &soa_out);
    if (soa_out) {
        ares_free_data(soa_out);
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Test all the parse functions with the fuzzer input
    test_ares_parse_naptr_reply(Data, Size);
    test_ares_parse_uri_reply(Data, Size);
    test_ares_parse_mx_reply(Data, Size);
    test_ares_parse_ptr_reply(Data, Size);
    test_ares_parse_srv_reply(Data, Size);
    test_ares_parse_soa_reply(Data, Size);
    
    return 0;
}
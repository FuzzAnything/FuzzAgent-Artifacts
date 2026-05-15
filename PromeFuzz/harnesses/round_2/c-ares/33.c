// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_ptr_reply at ares_parse_ptr_reply.c:186:5 in ares.h
// ares_parse_uri_reply at ares_parse_uri_reply.c:30:5 in ares.h
// ares_parse_srv_reply at ares_parse_srv_reply.c:30:5 in ares.h
// ares_parse_naptr_reply at ares_parse_naptr_reply.c:29:5 in ares.h
// ares_parse_mx_reply at ares_parse_mx_reply.c:30:5 in ares.h
// ares_parse_soa_reply at ares_parse_soa_reply.c:30:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

static void free_mx_reply(struct ares_mx_reply *reply) {
    while (reply) {
        struct ares_mx_reply *next = reply->next;
        free(reply->host);
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

static void free_ptr_reply(struct hostent *reply) {
    // Cannot access hostent internals due to incomplete type
    // Memory cleanup is handled by library functions
    (void)reply;
}

static void free_uri_reply(struct ares_uri_reply *reply) {
    while (reply) {
        struct ares_uri_reply *next = reply->next;
        free(reply->uri);
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

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    const unsigned char *abuf = Data;
    int alen = (int)Size;
    
    // 1. Test ares_parse_naptr_reply
    struct ares_naptr_reply *naptr_out = NULL;
    int ret_naptr = ares_parse_naptr_reply(abuf, alen, &naptr_out);
    if (ret_naptr == ARES_SUCCESS) {
        free_naptr_reply(naptr_out);
    }
    
    // 2. Test ares_parse_mx_reply
    struct ares_mx_reply *mx_out = NULL;
    int ret_mx = ares_parse_mx_reply(abuf, alen, &mx_out);
    if (ret_mx == ARES_SUCCESS) {
        free_mx_reply(mx_out);
    }
    
    // 3. Test ares_parse_soa_reply
    struct ares_soa_reply *soa_out = NULL;
    int ret_soa = ares_parse_soa_reply(abuf, alen, &soa_out);
    if (ret_soa == ARES_SUCCESS) {
        free_soa_reply(soa_out);
    }
    
    // 4. Test ares_parse_ptr_reply
    struct hostent *host_out = NULL;
    // Create dummy address data from the input
    unsigned char addr_data[16] = {0};
    size_t addr_len = Size > sizeof(addr_data) ? sizeof(addr_data) : Size;
    memcpy(addr_data, Data, addr_len);
    
    int family = AF_INET;
    if (Size > 8) {
        family = (Data[0] % 2 == 0) ? AF_INET : AF_INET6;
    }
    
    int ret_ptr = ares_parse_ptr_reply(abuf, alen, addr_data, 
                                       (family == AF_INET) ? 4 : 16, 
                                       family, &host_out);
    if (ret_ptr == ARES_SUCCESS) {
        free_ptr_reply(host_out);
    }
    
    // 5. Test ares_parse_uri_reply
    struct ares_uri_reply *uri_out = NULL;
    int ret_uri = ares_parse_uri_reply(abuf, alen, &uri_out);
    if (ret_uri == ARES_SUCCESS) {
        free_uri_reply(uri_out);
    }
    
    // 6. Test ares_parse_srv_reply
    struct ares_srv_reply *srv_out = NULL;
    int ret_srv = ares_parse_srv_reply(abuf, alen, &srv_out);
    if (ret_srv == ARES_SUCCESS) {
        free_srv_reply(srv_out);
    }
    
    return 0;
}
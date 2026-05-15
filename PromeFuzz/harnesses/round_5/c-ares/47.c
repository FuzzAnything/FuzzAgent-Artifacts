// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_soa_reply at ares_parse_soa_reply.c:30:5 in ares.h
// ares_parse_mx_reply at ares_parse_mx_reply.c:30:5 in ares.h
// ares_parse_srv_reply at ares_parse_srv_reply.c:30:5 in ares.h
// ares_parse_uri_reply at ares_parse_uri_reply.c:30:5 in ares.h
// ares_parse_ptr_reply at ares_parse_ptr_reply.c:186:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_ptr_reply at ares_parse_ptr_reply.c:186:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
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

static void free_mx_reply(struct ares_mx_reply *mx_out) {
    while (mx_out) {
        struct ares_mx_reply *next = mx_out->next;
        free(mx_out->host);
        free(mx_out);
        mx_out = next;
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

static void free_uri_reply(struct ares_uri_reply *uri_out) {
    while (uri_out) {
        struct ares_uri_reply *next = uri_out->next;
        free(uri_out->uri);
        free(uri_out);
        uri_out = next;
    }
}

static void free_srv_reply(struct ares_srv_reply *srv_out) {
    while (srv_out) {
        struct ares_srv_reply *next = srv_out->next;
        free(srv_out->host);
        free(srv_out);
        srv_out = next;
    }
}

static void free_soa_reply(struct ares_soa_reply *soa_out) {
    if (soa_out) {
        free(soa_out->nsname);
        free(soa_out->hostmaster);
        free(soa_out);
    }
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to select which function to test
    uint8_t selector = Data[0];
    const unsigned char *abuf = Data + 1;
    int alen = Size - 1;
    
    // Test different functions based on selector
    switch (selector % 6) {
        case 0: {
            // ares_parse_ptr_reply
            struct hostent *host = NULL;
            // Create dummy address data
            char addr4[4] = {192, 168, 1, 1};
            char addr6[16] = {0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x00, 0x00,
                              0x00, 0x00, 0x8a, 0x2e, 0x03, 0x70, 0x73, 0x34};
            
            // Try with IPv4
            ares_parse_ptr_reply(abuf, alen, addr4, 4, AF_INET, &host);
            if (host) {
                ares_free_hostent(host);
                host = NULL;
            }
            
            // Try with IPv6 if we have enough data
            if (alen > 16) {
                ares_parse_ptr_reply(abuf, alen, addr6, 16, AF_INET6, &host);
                if (host) {
                    ares_free_hostent(host);
                }
            }
            break;
        }
        
        case 1: {
            // ares_parse_naptr_reply
            struct ares_naptr_reply *naptr_out = NULL;
            int result = ares_parse_naptr_reply(abuf, alen, &naptr_out);
            if (result == ARES_SUCCESS) {
                free_naptr_reply(naptr_out);
            }
            break;
        }
        
        case 2: {
            // ares_parse_soa_reply
            struct ares_soa_reply *soa_out = NULL;
            int result = ares_parse_soa_reply(abuf, alen, &soa_out);
            if (result == ARES_SUCCESS) {
                free_soa_reply(soa_out);
            }
            break;
        }
        
        case 3: {
            // ares_parse_mx_reply
            struct ares_mx_reply *mx_out = NULL;
            int result = ares_parse_mx_reply(abuf, alen, &mx_out);
            if (result == ARES_SUCCESS) {
                free_mx_reply(mx_out);
            }
            break;
        }
        
        case 4: {
            // ares_parse_srv_reply
            struct ares_srv_reply *srv_out = NULL;
            int result = ares_parse_srv_reply(abuf, alen, &srv_out);
            if (result == ARES_SUCCESS) {
                free_srv_reply(srv_out);
            }
            break;
        }
        
        case 5: {
            // ares_parse_uri_reply
            struct ares_uri_reply *uri_out = NULL;
            int result = ares_parse_uri_reply(abuf, alen, &uri_out);
            if (result == ARES_SUCCESS) {
                free_uri_reply(uri_out);
            }
            break;
        }
    }
    
    return 0;
}
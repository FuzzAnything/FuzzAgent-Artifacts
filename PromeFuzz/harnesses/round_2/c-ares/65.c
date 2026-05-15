// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_parse_mx_reply at ares_parse_mx_reply.c:30:5 in ares.h
// ares_parse_caa_reply at ares_parse_caa_reply.c:30:5 in ares.h
// ares_parse_a_reply at ares_parse_a_reply.c:48:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_a_reply at ares_parse_a_reply.c:48:5 in ares.h
// ares_parse_txt_reply_ext at ares_parse_txt_reply.c:136:5 in ares.h
// ares_parse_ns_reply at ares_parse_ns_reply.c:39:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_aaaa_reply at ares_parse_aaaa_reply.c:51:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_parse_aaaa_reply at ares_parse_aaaa_reply.c:51:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "ares.h"

static void free_txt_ext(struct ares_txt_ext *txt) {
    while (txt) {
        struct ares_txt_ext *next = txt->next;
        if (txt->txt) {
            free(txt->txt);
        }
        free(txt);
        txt = next;
    }
}

static void free_mx_reply(struct ares_mx_reply *mx) {
    while (mx) {
        struct ares_mx_reply *next = mx->next;
        if (mx->host) {
            free(mx->host);
        }
        free(mx);
        mx = next;
    }
}

static void free_caa_reply(struct ares_caa_reply *caa) {
    while (caa) {
        struct ares_caa_reply *next = caa->next;
        if (caa->property) {
            free(caa->property);
        }
        if (caa->value) {
            free(caa->value);
        }
        free(caa);
        caa = next;
    }
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    // Use first byte to decide which function to test (for more diverse exploration)
    uint8_t selector = Size > 0 ? Data[0] : 0;
    const uint8_t *buf = Size > 0 ? Data + 1 : Data;
    int len = Size > 0 ? (int)(Size - 1) : 0;
    
    // Ensure len is non-negative
    if (len < 0) len = 0;
    
    // Test different functions based on selector
    switch (selector % 6) {
        case 0: {
            // Test ares_parse_txt_reply_ext
            struct ares_txt_ext *txt_out = NULL;
            int ret = ares_parse_txt_reply_ext(buf, len, &txt_out);
            if (txt_out) {
                free_txt_ext(txt_out);
            }
            (void)ret;
            break;
        }
        
        case 1: {
            // Test ares_parse_ns_reply
            struct hostent *host = NULL;
            int ret = ares_parse_ns_reply(buf, len, &host);
            if (host) {
                // Use ares_free_hostent if available, otherwise just free the pointer
                // Note: We can't access struct internals due to incomplete type
                ares_free_hostent(host);
            }
            (void)ret;
            break;
        }
        
        case 2: {
            // Test ares_parse_aaaa_reply - test with different parameter combinations
            struct hostent *host = NULL;
            struct ares_addr6ttl addr6ttls[10];
            int naddrttls = 10;
            
            // First call with host output
            int ret1 = ares_parse_aaaa_reply(buf, len, &host, NULL, NULL);
            if (host) {
                ares_free_hostent(host);
                host = NULL;
            }
            
            // Second call with addrttls output
            int ret2 = ares_parse_aaaa_reply(buf, len, NULL, addr6ttls, &naddrttls);
            (void)ret1;
            (void)ret2;
            break;
        }
        
        case 3: {
            // Test ares_parse_mx_reply
            struct ares_mx_reply *mx_out = NULL;
            int ret = ares_parse_mx_reply(buf, len, &mx_out);
            if (mx_out) {
                free_mx_reply(mx_out);
            }
            (void)ret;
            break;
        }
        
        case 4: {
            // Test ares_parse_caa_reply
            struct ares_caa_reply *caa_out = NULL;
            int ret = ares_parse_caa_reply(buf, len, &caa_out);
            if (caa_out) {
                free_caa_reply(caa_out);
            }
            (void)ret;
            break;
        }
        
        case 5: {
            // Test ares_parse_a_reply - test with different parameter combinations
            struct hostent *host = NULL;
            struct ares_addrttl addrttls[10];
            int naddrttls = 10;
            
            // First call with host output
            int ret1 = ares_parse_a_reply(buf, len, &host, NULL, NULL);
            if (host) {
                ares_free_hostent(host);
                host = NULL;
            }
            
            // Second call with addrttls output
            int ret2 = ares_parse_a_reply(buf, len, NULL, addrttls, &naddrttls);
            (void)ret1;
            (void)ret2;
            break;
        }
    }
    
    return 0;
}
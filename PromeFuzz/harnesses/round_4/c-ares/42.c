// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_gethostbyname at ares_gethostbyname.c:99:6 in ares.h
// ares_getaddrinfo at ares_getaddrinfo.c:695:6 in ares.h
// ares_search at ares_search.c:431:6 in ares.h
// ares_gethostbyaddr at ares_gethostbyaddr.c:108:6 in ares.h
// ares_gethostbyname_file at ares_gethostbyname.c:335:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_process_fd at ares_process.c:267:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Dummy callbacks for async functions with correct const qualifiers */
static void dummy_host_callback(void *arg, int status, int timeouts, const struct hostent *host) {
    /* Do nothing */
}

static void dummy_addrinfo_callback(void *arg, int status, int timeouts, struct ares_addrinfo *res) {
    /* Free the allocated addrinfo structure to prevent memory leak */
    if (res != NULL) {
        ares_freeaddrinfo(res);
    }
}

static void dummy_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    /* Do nothing */
}

/* Helper to create a channel with minimal configuration */
static ares_channel_t* create_test_channel(void) {
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    /* Set minimal options */
    options.timeout = 1000; /* 1 second timeout */
    optmask |= ARES_OPT_TIMEOUTMS;
    
    options.tries = 1;
    optmask |= ARES_OPT_TRIES;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        return NULL;
    }
    
    return channel;
}

/* Helper to parse family from fuzzer data */
static int parse_family(uint8_t family_byte) {
    switch (family_byte % 4) {
        case 0: return AF_INET;
        case 1: return AF_INET6;
        case 2: return AF_UNSPEC;
        default: return AF_INET;
    }
}

/* Helper to parse DNS class from fuzzer data */
static int parse_dnsclass(uint8_t class_byte) {
    /* Use standard DNS class values */
    switch (class_byte % 4) {
        case 0: return 1;   /* C_IN - Internet */
        case 1: return 3;   /* C_CHAOS */
        case 2: return 4;   /* C_HS - Hesiod */
        default: return 1;  /* C_IN */
    }
}

/* Helper to parse DNS type from fuzzer data */
static int parse_dnstype(uint8_t type_byte) {
    /* Use standard DNS type values */
    switch (type_byte % 16) {
        case 0: return 1;   /* T_A - Address record */
        case 1: return 2;   /* T_NS - Name server */
        case 2: return 5;   /* T_CNAME - Canonical name */
        case 3: return 6;   /* T_SOA - Start of authority */
        case 4: return 12;  /* T_PTR - Pointer record */
        case 5: return 15;  /* T_MX - Mail exchange */
        case 6: return 16;  /* T_TXT - Text record */
        case 7: return 28;  /* T_AAAA - IPv6 address */
        case 8: return 33;  /* T_SRV - Service record */
        default: return 1;  /* T_A */
    }
}

/* Helper to create address from fuzzer data */
static void create_test_address(const uint8_t *data, size_t size, 
                                const void **addr, int *addrlen, int *family) {
    static struct in_addr ipv4_addr;
    static struct in6_addr ipv6_addr;
    
    *family = parse_family(data[0] % 2); /* Only AF_INET or AF_INET6 for addresses */
    
    if (*family == AF_INET && size >= 5) {
        /* Use bytes 1-4 for IPv4 address */
        memcpy(&ipv4_addr, data + 1, 4);
        *addr = &ipv4_addr;
        *addrlen = 4;
    } else if (*family == AF_INET6 && size >= 17) {
        /* Use bytes 1-16 for IPv6 address */
        memcpy(&ipv6_addr, data + 1, 16);
        *addr = &ipv6_addr;
        *addrlen = 16;
    } else {
        /* Default to IPv4 loopback */
        ipv4_addr.s_addr = htonl(INADDR_LOOPBACK);
        *addr = &ipv4_addr;
        *addrlen = 4;
        *family = AF_INET;
    }
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }
    
    /* Create test channel */
    ares_channel_t *channel = create_test_channel();
    if (!channel) {
        return 0;
    }
    
    /* Create null-terminated string from fuzzer data */
    char *name = NULL;
    if (Size > 0) {
        name = (char*)malloc(Size + 1);
        if (!name) {
            ares_destroy(channel);
            return 0;
        }
        memcpy(name, Data, Size);
        name[Size] = '\0';
    }
    
    /* Parse function selector from first byte */
    uint8_t func_selector = Data[0] % 6;
    
    /* Parse family from second byte */
    int family = parse_family(Data[1]);
    
    /* Test different API functions based on selector */
    switch (func_selector) {
        case 0: /* ares_gethostbyname */
            if (name) {
                ares_gethostbyname(channel, name, family, 
                                  dummy_host_callback, NULL);
            }
            break;
            
        case 1: /* ares_getaddrinfo */
            if (name) {
                struct ares_addrinfo_hints hints;
                memset(&hints, 0, sizeof(hints));
                hints.ai_family = family;
                hints.ai_flags = 0;
                hints.ai_socktype = SOCK_STREAM;
                
                ares_getaddrinfo(channel, name, NULL, &hints,
                                dummy_addrinfo_callback, NULL);
            }
            break;
            
        case 2: /* ares_search */
            if (name && Size >= 4) {
                int dnsclass = parse_dnsclass(Data[2]);
                int type = parse_dnstype(Data[3]);
                ares_search(channel, name, dnsclass, type,
                           dummy_callback, NULL);
            }
            break;
            
        case 3: /* ares_gethostbyaddr */
            if (Size >= 3) {
                const void *addr = NULL;
                int addrlen = 0;
                int addr_family = AF_INET;
                
                create_test_address(Data + 1, Size - 1, &addr, &addrlen, &addr_family);
                ares_gethostbyaddr(channel, addr, addrlen, addr_family,
                                  dummy_host_callback, NULL);
            }
            break;
            
        case 4: /* ares_gethostbyname_file */
            if (name) {
                struct hostent *host = NULL;
                int result = ares_gethostbyname_file(channel, name, family, &host);
                if (result == ARES_SUCCESS && host) {
                    ares_free_hostent(host);
                }
            }
            break;
            
        case 5: /* ares_free_hostent - test with dummy hostent */
            ares_free_hostent(NULL);
            break;
    }
    
    /* Process any pending events */
    ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    
    /* Cleanup */
    if (name) {
        free(name);
    }
    ares_destroy(channel);
    
    return 0;
}
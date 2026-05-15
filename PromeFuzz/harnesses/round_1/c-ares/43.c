// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_gethostbyname at ares_gethostbyname.c:99:6 in ares.h
// ares_search at ares_search.c:431:6 in ares.h
// ares_getaddrinfo at ares_getaddrinfo.c:695:6 in ares.h
// ares_gethostbyaddr at ares_gethostbyaddr.c:108:6 in ares.h
// ares_fds at ares_fds.c:30:5 in ares.h
// ares_process at ares_process.c:334:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_gethostbyname_file at ares_gethostbyname.c:335:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/* DNS constants if not defined in ares.h */
#ifndef C_IN
#define C_IN 1
#endif
#ifndef T_A
#define T_A 1
#endif
#ifndef T_AAAA
#define T_AAAA 28
#endif

/* AI_CANONNAME might not be defined in all environments */
#ifndef AI_CANONNAME
#define AI_CANONNAME 0x0002
#endif

static void dummy_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    /* Dummy callback for ares_search */
}

static void dummy_host_callback(void *arg, int status, int timeouts, const struct hostent *hostent) {
    /* Dummy callback for ares_gethostbyname and ares_gethostbyaddr */
}

static void dummy_addrinfo_callback(void *arg, int status, int timeouts, struct ares_addrinfo *res) {
    /* Dummy callback for ares_getaddrinfo */
    if (res != NULL) {
        ares_freeaddrinfo(res);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create dummy hosts file */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp != NULL) {
        fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
        fclose(fp);
    }

    /* Initialize channel */
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    /* Set hosts file path */
    options.hosts_path = "./dummy_file";
    optmask |= ARES_OPT_HOSTS_FILE;
    
    /* Set timeout to minimal value for fuzzing */
    options.timeout = 1;
    optmask |= ARES_OPT_TIMEOUTMS;
    
    /* Initialize with options */
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        /* Clean up dummy file */
        unlink("./dummy_file");
        return 0;
    }

    /* Use data to generate test parameters */
    size_t data_offset = 0;
    
    /* Prepare name string for various functions */
    char name[256];
    size_t name_len = Size > 255 ? 255 : Size;
    if (name_len > 0) {
        memcpy(name, Data, name_len);
        name[name_len] = '\0';
        data_offset = name_len;
    } else {
        name[0] = 't';
        name[1] = 'e';
        name[2] = 's';
        name[3] = 't';
        name[4] = '\0';
    }

    /* Test ares_gethostbyname_file */
    struct hostent *host = NULL;
    int family = (Size > 0 && (Data[0] & 1)) ? AF_INET : AF_INET6;
    int result = ares_gethostbyname_file(channel, name, family, &host);
    if (host != NULL) {
        ares_free_hostent(host);
        host = NULL;
    }

    /* Test ares_gethostbyname */
    if (Size > 1) {
        family = (Data[1] & 1) ? AF_INET : AF_INET6;
    } else {
        family = AF_INET;
    }
    ares_gethostbyname(channel, name, family, dummy_host_callback, NULL);

    /* Test ares_search */
    int dnsclass = C_IN;
    int type;
    if (Size > 2) {
        type = (Data[2] & 1) ? T_A : T_AAAA;
    } else {
        type = T_A;
    }
    ares_search(channel, name, dnsclass, type, dummy_callback, NULL);

    /* Test ares_getaddrinfo */
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    if (Size > 3) {
        hints.ai_family = (Data[3] & 1) ? AF_INET : AF_UNSPEC;
        hints.ai_flags = (Data[3] & 2) ? AI_CANONNAME : 0;
        hints.ai_socktype = (Data[3] & 4) ? SOCK_STREAM : 0;
    } else {
        hints.ai_family = AF_INET;
    }
    
    char service[16];
    if (Size > 4) {
        snprintf(service, sizeof(service), "%d", Data[4] % 65536);
    } else {
        snprintf(service, sizeof(service), "80");
    }
    
    ares_getaddrinfo(channel, name, service, &hints, dummy_addrinfo_callback, NULL);

    /* Test ares_gethostbyaddr */
    struct in_addr ipv4_addr;
    struct in6_addr ipv6_addr;
    const void *addr = NULL;
    int addrlen = 0;
    
    if (Size > 5 && (Data[5] & 1) && Size >= 4) {
        /* Use IPv4 address - ensure we have at least 4 bytes */
        size_t copy_size = Size - 5;
        if (copy_size > sizeof(struct in_addr)) copy_size = sizeof(struct in_addr);
        memcpy(&ipv4_addr, Data + 5, copy_size);
        /* Fill remaining bytes if needed */
        for (size_t i = copy_size; i < sizeof(struct in_addr); i++) {
            ((uint8_t*)&ipv4_addr)[i] = 0;
        }
        addr = &ipv4_addr;
        addrlen = sizeof(struct in_addr);
        family = AF_INET;
    } else if (Size > 5 && Size >= 16) {
        /* Use IPv6 address - ensure we have at least 16 bytes */
        size_t copy_size = Size - 5;
        if (copy_size > sizeof(struct in6_addr)) copy_size = sizeof(struct in6_addr);
        memcpy(&ipv6_addr, Data + 5, copy_size);
        /* Fill remaining bytes if needed */
        for (size_t i = copy_size; i < sizeof(struct in6_addr); i++) {
            ((uint8_t*)&ipv6_addr)[i] = 0;
        }
        addr = &ipv6_addr;
        addrlen = sizeof(struct in6_addr);
        family = AF_INET6;
    } else {
        /* Fallback to default IPv4 address */
        ipv4_addr.s_addr = htonl(0x7F000001); /* 127.0.0.1 */
        addr = &ipv4_addr;
        addrlen = sizeof(struct in_addr);
        family = AF_INET;
    }
    
    ares_gethostbyaddr(channel, addr, addrlen, family, dummy_host_callback, NULL);

    /* Process any pending requests with a short timeout */
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000; /* 1ms timeout */
    
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    if (nfds > 0) {
        ares_process(channel, &read_fds, &write_fds);
    }

    /* Clean up */
    ares_destroy(channel);
    unlink("./dummy_file");
    
    return 0;
}
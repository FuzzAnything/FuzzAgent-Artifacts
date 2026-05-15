// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_gethostbyname_file at ares_gethostbyname.c:335:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_getaddrinfo at ares_getaddrinfo.c:695:6 in ares.h
// ares_gethostbyaddr at ares_gethostbyaddr.c:108:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_gethostbyname at ares_gethostbyname.c:99:6 in ares.h
// ares_search at ares_search.c:431:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ares.h"

static void dummy_host_callback(void *arg, int status, int timeouts, const struct hostent *hostent) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
}

static void dummy_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

static void dummy_addrinfo_callback(void *arg, int status, int timeouts, struct ares_addrinfo *res) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)res;
}

static void create_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    create_dummy_file(Data, Size);
    
    ares_channel_t *channel = NULL;
    int status = ares_init(&channel);
    if (status != ARES_SUCCESS) {
        unlink("./dummy_file");
        return 0;
    }
    
    if (channel == NULL) {
        unlink("./dummy_file");
        return 0;
    }
    
    struct ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    options.hosts_path = "./dummy_file";
    optmask |= ARES_OPT_HOSTS_FILE;
    
    ares_init_options(&channel, &options, optmask);
    
    size_t str_len = Size > 256 ? 256 : Size;
    char *name = (char *)malloc(str_len + 1);
    if (!name) {
        ares_destroy(channel);
        unlink("./dummy_file");
        return 0;
    }
    memcpy(name, Data, str_len);
    name[str_len] = '\0';
    
    int family = Data[0] % 3;
    if (family == 0) family = AF_INET;
    else if (family == 1) family = AF_INET6;
    else family = AF_UNSPEC;
    
    int dnsclass = Data[0] % 4;
    int type = Data[0] % 16;
    
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = family;
    hints.ai_flags = Data[0] & 0xFF;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    const char *service = "80";
    
    struct hostent *host = NULL;
    
    ares_gethostbyname(channel, name, family, dummy_host_callback, NULL);
    
    ares_search(channel, name, dnsclass, type, dummy_callback, NULL);
    
    ares_gethostbyname_file(channel, name, family, &host);
    if (host) {
        ares_free_hostent(host);
    }
    
    ares_getaddrinfo(channel, name, service, &hints, dummy_addrinfo_callback, NULL);
    
    struct in_addr addr4;
    struct in6_addr addr6;
    const void *addr = NULL;
    int addrlen = 0;
    
    if (family == AF_INET && Size >= 4) {
        memcpy(&addr4, Data, 4);
        addr = &addr4;
        addrlen = 4;
    } else if (family == AF_INET6 && Size >= 16) {
        memcpy(&addr6, Data, 16);
        addr = &addr6;
        addrlen = 16;
    } else if (Size >= 4) {
        memcpy(&addr4, Data, 4);
        addr = &addr4;
        addrlen = 4;
        family = AF_INET;
    }
    
    if (addr) {
        ares_gethostbyaddr(channel, addr, addrlen, family, dummy_host_callback, NULL);
    }
    
    ares_destroy(channel);
    free(name);
    
    unlink("./dummy_file");
    
    return 0;
}
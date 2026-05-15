// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_getaddrinfo at ares_getaddrinfo.c:695:6 in ares.h
// ares_search at ares_search.c:431:6 in ares.h
// ares_gethostbyaddr at ares_gethostbyaddr.c:108:6 in ares.h
// ares_fds at ares_fds.c:30:5 in ares.h
// ares_process at ares_process.c:334:6 in ares.h
// ares_cancel at ares_cancel.c:34:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_gethostbyname_file at ares_gethostbyname.c:335:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_gethostbyname at ares_gethostbyname.c:99:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "ares.h"

#ifdef __cplusplus
}
#endif

static void dummy_host_callback(void *arg, int status, int timeouts, const struct hostent *hostent) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
}

static void dummy_addrinfo_callback(void *arg, int status, int timeouts, struct ares_addrinfo *addrinfo) {
    (void)arg;
    (void)status;
    (void)timeouts;
    if (addrinfo) {
        ares_freeaddrinfo(addrinfo);
    }
}

static void dummy_search_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Initialize channel
    ares_channel_t *channel = NULL;
    int options = 0;
    struct ares_options opts;
    memset(&opts, 0, sizeof(opts));
    
    // Use first byte for channel initialization flags
    opts.flags = Data[0];
    opts.timeout = 1000;
    opts.tries = 1;
    options |= ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    int status = ares_init_options(&channel, &opts, options);
    if (status != ARES_SUCCESS || channel == NULL) {
        return 0;
    }

    // Create null-terminated name string from fuzzer data
    size_t name_len = Size > 256 ? 256 : Size - 1;
    char *name = (char *)malloc(name_len + 1);
    if (!name) {
        ares_destroy(channel);
        return 0;
    }
    memcpy(name, Data + 1, name_len);
    name[name_len] = '\0';

    // Create dummy file for hosts lookup
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
        fclose(fp);
    }

    // Test ares_gethostbyname_file
    struct hostent *host = NULL;
    int family = (Data[0] & 1) ? AF_INET : AF_INET6;
    ares_gethostbyname_file(channel, name, family, &host);
    if (host) {
        ares_free_hostent(host);
    }

    // Test ares_gethostbyname (asynchronous)
    family = (Data[0] & 2) ? AF_INET : AF_INET6;
    ares_gethostbyname(channel, name, family, dummy_host_callback, NULL);

    // Test ares_getaddrinfo (asynchronous)
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = (Data[0] & 4) ? AF_INET : AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    
    const char *service = (Data[0] & 8) ? "http" : NULL;
    ares_getaddrinfo(channel, name, service, &hints, dummy_addrinfo_callback, NULL);

    // Test ares_search (asynchronous)
    int dnsclass = (Data[0] & 16) ? 1 : 255;  // C_IN = 1, C_ANY = 255
    int type = (Data[0] & 32) ? 1 : 255;      // T_A = 1, T_ANY = 255
    ares_search(channel, name, dnsclass, type, dummy_search_callback, NULL);

    // Test ares_gethostbyaddr (asynchronous)
    struct in_addr addr4;
    struct in6_addr addr6;
    void *addr = NULL;
    int addrlen = 0;
    
    if (family == AF_INET && Size >= 6) {
        memcpy(&addr4, Data + 2, 4);
        addr = &addr4;
        addrlen = 4;
    } else if (family == AF_INET6 && Size >= 18) {
        memcpy(&addr6, Data + 2, 16);
        addr = &addr6;
        addrlen = 16;
    }
    
    if (addr) {
        ares_gethostbyaddr(channel, addr, addrlen, family, dummy_host_callback, NULL);
    }

    // Process pending operations
    fd_set read_fds, write_fds;
    struct timeval tv;
    int nfds;
    
    for (int i = 0; i < 3; i++) {
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        nfds = ares_fds(channel, &read_fds, &write_fds);
        
        if (nfds == 0) {
            break;
        }
        
        tv.tv_sec = 0;
        tv.tv_usec = 1000;
        select(nfds, &read_fds, &write_fds, NULL, &tv);
        ares_process(channel, &read_fds, &write_fds);
    }

    // Cleanup
    ares_cancel(channel);
    ares_destroy(channel);
    free(name);
    unlink("./dummy_file");

    return 0;
}
// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_gethostbyname_file at ares_gethostbyname.c:335:5 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_getaddrinfo at ares_getaddrinfo.c:695:6 in ares.h
// ares_process_fd at ares_process.c:267:6 in ares.h
// ares_gethostbyaddr at ares_gethostbyaddr.c:108:6 in ares.h
// ares_search at ares_search.c:431:6 in ares.h
// ares_gethostbyname at ares_gethostbyname.c:99:6 in ares.h
// ares_process_fd at ares_process.c:267:6 in ares.h
// ares_process_fd at ares_process.c:267:6 in ares.h
// ares_free_hostent at ares_free_hostent.c:33:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_freeaddrinfo at ares_freeaddrinfo.c:57:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* DNS constants that might not be defined in the included headers */
#ifndef C_IN
#define C_IN 1
#endif

#ifndef T_A
#define T_A 1
#endif

#ifndef T_AAAA
#define T_AAAA 28
#endif

/* Static callback functions to avoid dangling callbacks */
static void dummy_addrinfo_callback(void *arg, int status, int timeouts,
                                   struct ares_addrinfo *result) {
    (void)arg;
    (void)status;
    (void)timeouts;
    if (result) {
        ares_freeaddrinfo(result);
    }
}

static void dummy_host_callback(void *arg, int status, int timeouts,
                               const struct hostent *hostent) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
    /* Do NOT free hostent here - library manages this memory */
}

static void dummy_search_callback(void *arg, int status, int timeouts,
                                 const unsigned char *abuf, int alen) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

/* Helper to create a dummy hosts file */
static void create_dummy_hosts(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            size_t write_size = size > 1024 ? 1024 : size;
            write(fd, data, write_size);
        } else {
            const char *default_hosts = "127.0.0.1 localhost\n";
            write(fd, default_hosts, strlen(default_hosts));
        }
        close(fd);
    }
}

/* Helper to create a minimal channel */
static ares_channel_t* create_minimal_channel(void) {
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    
    /* Set minimal options to avoid network access */
    options.timeout = 100;
    optmask |= ARES_OPT_TIMEOUTMS;
    
    options.tries = 1;
    optmask |= ARES_OPT_TRIES;
    
    /* Set dummy hosts file path */
    options.hosts_path = "./dummy_file";
    optmask |= ARES_OPT_HOSTS_FILE;
    
    /* Disable lookups from DNS */
    options.lookups = "";
    optmask |= ARES_OPT_LOOKUPS;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        return NULL;
    }
    
    return channel;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    /* Create dummy hosts file */
    create_dummy_hosts(Data, Size);
    
    /* Create minimal channel */
    ares_channel_t *channel = create_minimal_channel();
    if (!channel) {
        unlink("./dummy_file");
        return 0;
    }
    
    /* Split input for diverse testing */
    size_t half = Size / 2;
    size_t quarter = Size / 4;
    
    /* Extract strings from fuzzer data with null termination */
    char name_buf[256] = {0};
    char service_buf[256] = {0};
    size_t name_len = half > 255 ? 255 : half;
    size_t service_len = quarter > 255 ? 255 : quarter;
    
    if (Size > 0) {
        memcpy(name_buf, Data, name_len);
        name_buf[name_len] = '\0';
        if (Size > half) {
            memcpy(service_buf, Data + half, service_len);
            service_buf[service_len] = '\0';
        }
    }
    
    /* Test ares_gethostbyname_file */
    struct hostent *host = NULL;
    int family = (Size % 2) ? AF_INET : AF_INET6;
    int ret = ares_gethostbyname_file(channel, name_buf, family, &host);
    (void)ret; /* Ignore return value */
    if (host) {
        ares_free_hostent(host);
    }
    
    /* Test ares_getaddrinfo */
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = (Size % 3) ? AF_UNSPEC : AF_INET;
    hints.ai_flags = (Size % 4) ? 0 : ARES_AI_NUMERICHOST;
    hints.ai_socktype = SOCK_STREAM;
    
    ares_getaddrinfo(channel, name_buf, service_buf, &hints,
                     dummy_addrinfo_callback, NULL);
    
    /* Process pending requests briefly */
    ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    
    /* Test ares_gethostbyaddr */
    struct in_addr addr4;
    struct in6_addr addr6;
    const void *addr = NULL;
    int addrlen = 0;
    
    if (family == AF_INET) {
        addr4.s_addr = htonl(0x7f000001); /* 127.0.0.1 */
        addr = &addr4;
        addrlen = sizeof(addr4);
    } else {
        memset(&addr6, 0, sizeof(addr6));
        addr6.s6_addr[15] = 1; /* ::1 */
        addr = &addr6;
        addrlen = sizeof(addr6);
    }
    
    ares_gethostbyaddr(channel, addr, addrlen, family,
                       dummy_host_callback, NULL);
    
    /* Test ares_search (deprecated but included) */
    int dnsclass = C_IN;
    int type = (Size % 5) ? T_A : T_AAAA;
    ares_search(channel, name_buf, dnsclass, type,
                dummy_search_callback, NULL);
    
    /* Test ares_gethostbyname (deprecated but included) */
    ares_gethostbyname(channel, name_buf, family,
                       dummy_host_callback, NULL);
    
    /* Process pending requests again */
    ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    
    /* Wait a bit for any pending callbacks */
    usleep(1000);
    
    /* Process one more time to clean up */
    ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    
    /* Test ares_free_hostent with NULL (should be safe) */
    ares_free_hostent(NULL);
    
    /* Cleanup */
    ares_destroy(channel);
    
    /* Remove dummy file */
    unlink("./dummy_file");
    
    return 0;
}
#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "ares.h"
#include "ares_dns_record.h"

namespace harness_010 {
/* Comprehensive fuzzing harness for c-ares library targeting event-driven DNS operations
 * and socket callback configuration variations identified as critical coverage gaps:
 * - Event module APIs (currently 0% covered)
 * - Event-driven DNS operations with socket callback configurations
 * - Socket callback configuration variations (different from harness_008 and harness_009)
 * 
 * This harness implements event-driven DNS resolution with custom socket callback functions
 * to test integration between event systems and socket management APIs. Focuses on 
 * event-driven operations rather than just socket callback mechanisms.
 * 
 * Semantic diversity from existing harnesses (000-009):
 * - harness_000: Modern APIs (getaddrinfo, DNS record)
 * - harness_001: Legacy DNS processing pipeline  
 * - harness_002: Event processing and channel management
 * - harness_003: Server configuration and option setting
 * - harness_004: Legacy DNS parsing and query creation
 * - harness_005: Legacy DNS mapping and Android/system config
 * - harness_006: Metrics and search operations
 * - harness_007: DNS record binary data manipulation
 * - harness_008: Socket callback mechanisms (ares_set_socket_functions_ex) and socket management
 * - harness_009: Legacy socket configuration (ares_set_socket_functions) and error handling
 * - harness_010: Event-driven DNS operations with socket callback variations (NEW)
 */


/* Custom socket callback functions for event-driven testing */
/* Different implementation from harness_008 and harness_009 to provide semantic diversity */

/* Event-aware socket creation function */
static ares_socket_t event_asocket(int domain, int type, int protocol, void *user_data) {
    (void)user_data;
    
    /* Create socket with event-driven considerations */
    ares_socket_t sock = socket(domain, type, protocol);
    if (sock == ARES_SOCKET_BAD) {
        /* Simulate different error conditions for event handling */
        errno = (domain == AF_INET6) ? EAFNOSUPPORT : EPROTONOSUPPORT;
        return ARES_SOCKET_BAD;
    }
    
    /* Set non-blocking mode for event-driven I/O */
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags != -1) {
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    }
    
    /* Set close-on-exec for event-driven environments */
    fcntl(sock, F_SETFD, FD_CLOEXEC);
    
    return sock;
}

/* Event-aware socket close function */
static int event_aclose(ares_socket_t sock, void *user_data) {
    (void)user_data;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    /* In event-driven systems, we might need to clean up event registrations */
    return close(sock);
}

/* Event-aware socket option setting */
static int event_asetsockopt(ares_socket_t sock, ares_socket_opt_t opt, const void *val,
                             ares_socklen_t val_size, void *user_data) {
    (void)user_data;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    /* Map c-ares socket options to system socket options with event considerations */
    int sys_opt = 0;
    int level = 0;
    
    switch (opt) {
        case ARES_SOCKET_OPT_SENDBUF_SIZE:
            sys_opt = SO_SNDBUF;
            level = SOL_SOCKET;
            /* Event-driven systems often need larger buffers */
            if (val != NULL && val_size >= sizeof(int)) {
                int buffer_size = *(const int*)val;
                if (buffer_size < 8192) {
                    /* Minimum buffer size for event-driven I/O */
                    int min_buffer = 8192;
                    return setsockopt(sock, level, sys_opt, &min_buffer, sizeof(min_buffer));
                }
            }
            break;
        case ARES_SOCKET_OPT_RECVBUF_SIZE:
            sys_opt = SO_RCVBUF;
            level = SOL_SOCKET;
            break;
        case ARES_SOCKET_OPT_BIND_DEVICE:
            /* SO_BINDTODEVICE for event-driven multi-interface handling */
            errno = ENOSYS; /* Simulate system without this feature */
            return -1;
        case ARES_SOCKET_OPT_TCP_FASTOPEN:
            /* TCP Fast Open for event-driven latency reduction */
            errno = ENOSYS; /* Simulate system without this feature */
            return -1;
    }
    
    if (val != NULL && val_size > 0) {
        return setsockopt(sock, level, sys_opt, val, val_size);
    }
    
    return 0;
}

/* Event-aware socket connect */
static int event_aconnect(ares_socket_t sock, const struct sockaddr *address,
                          ares_socklen_t address_len, unsigned int flags,
                          void *user_data) {
    (void)user_data;
    (void)flags;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    /* In event-driven systems, connect is typically non-blocking */
    int result = connect(sock, address, address_len);
    if (result == -1 && errno == EINPROGRESS) {
        /* Expected for non-blocking sockets in event-driven systems */
        return -1;
    }
    
    return result;
}

/* Event-aware socket receive */
static ares_ssize_t event_arecvfrom(ares_socket_t sock, void *buffer, size_t length,
                                    int flags, struct sockaddr *address,
                                    ares_socklen_t *address_len, void *user_data) {
    (void)user_data;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    /* Event-driven systems might have partial reads */
    ssize_t result = recvfrom(sock, buffer, length, flags, address, address_len);
    
    /* Simulate EAGAIN/EWOULDBLOCK for event-driven non-blocking I/O */
    if (result == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        /* No data available yet - event system should retry */
        return -1;
    }
    
    return result;
}

/* Event-aware socket send */
static ares_ssize_t event_asendto(ares_socket_t sock, const void *buffer, size_t length,
                                  int flags, const struct sockaddr *address,
                                  ares_socklen_t address_len, void *user_data) {
    (void)user_data;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    /* Event-driven systems might have partial writes */
    ssize_t result;
    if (address == NULL) {
        result = send(sock, buffer, length, flags);
    } else {
        result = sendto(sock, buffer, length, flags, address, address_len);
    }
    
    /* Simulate EAGAIN/EWOULDBLOCK for event-driven non-blocking I/O */
    if (result == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        /* Buffer full - event system should retry */
        return -1;
    }
    
    return result;
}

/* Event-aware get socket name */
static int event_agetsockname(ares_socket_t sock, struct sockaddr *address,
                              ares_socklen_t *address_len, void *user_data) {
    (void)user_data;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    return getsockname(sock, address, address_len);
}

/* Event-aware bind */
static int event_abind(ares_socket_t sock, unsigned int flags,
                       const struct sockaddr *address, socklen_t address_len,
                       void *user_data) {
    (void)user_data;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    /* For event-driven systems, we might want to set specific options */
    if (flags & ARES_SOCKET_BIND_TCP) {
        /* TCP-specific bind options for event-driven systems */
        int reuse = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    }
    
    return bind(sock, address, address_len);
}


/* Simple DNS callback for event-driven testing */
static void dns_callback(void *arg, int status, int timeouts,
                         const struct hostent *hostent) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
    /* In a real event-driven system, this would trigger event processing */
}

/* Simple getaddrinfo callback for event-driven testing */
static void getaddrinfo_callback(void *arg, int status, int timeouts,
                                 struct ares_addrinfo *addrinfo) {
    (void)arg;
    (void)status;
    (void)timeouts;
    if (addrinfo != NULL) {
        ares_freeaddrinfo(addrinfo);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    /* Minimum input size for meaningful testing */
    const size_t MIN_SIZE = sizeof(int) * 8 + 32; /* Enough for config and small strings */
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    /* Initialize c-ares library */
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    /* Create channel for event-driven testing */
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    
    /* Consume configuration from fuzzer input */
    options.flags = fdp.ConsumeIntegral<int>();
    optmask |= ARES_OPT_FLAGS;
    
    options.timeout = fdp.ConsumeIntegralInRange<int>(100, 5000); /* ms */
    optmask |= ARES_OPT_TIMEOUTMS;
    
    options.tries = fdp.ConsumeIntegralInRange<int>(1, 10);
    optmask |= ARES_OPT_TRIES;
    
    options.ndots = fdp.ConsumeIntegralInRange<int>(0, 15);
    optmask |= ARES_OPT_NDOTS;
    
    /* Create channel with options */
    int status = ares_init_options(&channel, &options, optmask);
    
    if (status != ARES_SUCCESS || channel == NULL) {
        ares_library_cleanup();
        return 0;
    }
    
    /* Set up custom socket functions for event-driven testing */
    struct ares_socket_functions_ex socket_funcs;
    memset(&socket_funcs, 0, sizeof(socket_funcs));
    
    socket_funcs.version = 1;
    socket_funcs.flags = ARES_SOCKFUNC_FLAG_NONBLOCKING;
    socket_funcs.asocket = event_asocket;
    socket_funcs.aclose = event_aclose;
    socket_funcs.asetsockopt = event_asetsockopt;
    socket_funcs.aconnect = event_aconnect;
    socket_funcs.arecvfrom = event_arecvfrom;
    socket_funcs.asendto = event_asendto;
    socket_funcs.agetsockname = event_agetsockname;
    socket_funcs.abind = event_abind;
    
    /* Set socket functions with event-driven configuration */
    ares_set_socket_functions_ex(channel, &socket_funcs, NULL);
    
    /* Consume hostname for DNS queries */
    std::string hostname = fdp.ConsumeRandomLengthString(256);
    
    /* Perform different types of event-driven DNS operations based on fuzzed input */
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (operation) {
        case 0: {
            /* Traditional gethostbyname with event-driven socket callbacks */
            ares_gethostbyname(channel, hostname.c_str(), AF_INET, dns_callback, NULL);
            break;
        }
        case 1: {
            /* Modern getaddrinfo with event-driven socket callbacks */
            struct ares_addrinfo_hints hints;
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = fdp.ConsumeBool() ? AF_INET : AF_UNSPEC;
            hints.ai_socktype = fdp.ConsumeBool() ? SOCK_STREAM : SOCK_DGRAM;
            hints.ai_flags = fdp.ConsumeIntegral<int>();
            
            ares_getaddrinfo(channel, hostname.c_str(), NULL, &hints,
                            getaddrinfo_callback, NULL);
            break;
        }
        case 2: {
            /* IPv6 resolution with event-driven socket callbacks */
            ares_gethostbyname(channel, hostname.c_str(), AF_INET6, dns_callback, NULL);
            break;
        }
        case 3: {
            /* Reverse DNS lookup with event-driven socket callbacks */
            if (fdp.remaining_bytes() >= 4) {
                std::vector<uint8_t> ip_bytes = fdp.ConsumeBytes<uint8_t>(4);
                struct in_addr addr;
                memcpy(&addr, ip_bytes.data(), 4);
                
                char addr_str[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &addr, addr_str, sizeof(addr_str));
                
                ares_gethostbyaddr(channel, &addr, sizeof(addr), AF_INET,
                                  dns_callback, NULL);
            }
            break;
        }
    }
    
    /* Process pending events/sockets */
    int max_fd = 0;
    fd_set read_fds, write_fds;
    struct timeval tv;
    
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    /* Get socket state for event processing */
    ares_socket_t sockets[ARES_GETSOCK_MAXNUM];
    int bitmask = ares_getsock(channel, sockets, ARES_GETSOCK_MAXNUM);
    
    for (int i = 0; i < ARES_GETSOCK_MAXNUM; i++) {
        if (ARES_GETSOCK_READABLE(bitmask, i)) {
            FD_SET(sockets[i], &read_fds);
            if (sockets[i] > max_fd) {
                max_fd = sockets[i];
            }
        }
        if (ARES_GETSOCK_WRITABLE(bitmask, i)) {
            FD_SET(sockets[i], &write_fds);
            if (sockets[i] > max_fd) {
                max_fd = sockets[i];
            }
        }
    }
    
    /* Simulate event loop processing */
    if (max_fd > 0) {
        tv.tv_sec = 0;
        tv.tv_usec = 10000; /* 10ms timeout for event-driven testing */
        
        int nfds = select(max_fd + 1, &read_fds, &write_fds, NULL, &tv);
        
        if (nfds > 0) {
            /* Process sockets with events */
            for (int i = 0; i < ARES_GETSOCK_MAXNUM; i++) {
                int action = 0;
                if (FD_ISSET(sockets[i], &read_fds)) {
                    action |= 1; /* Readable flag */
                }
                if (FD_ISSET(sockets[i], &write_fds)) {
                    action |= 2; /* Writable flag */
                }
                
                if (action != 0) {
                    ares_process_fd(channel, 
                                   (action & 1) ? sockets[i] : ARES_SOCKET_BAD,
                                   (action & 2) ? sockets[i] : ARES_SOCKET_BAD);
                }
            }
        }
    }
    
    /* Process timeout for event-driven operations */
    struct timeval *tv_ptr = ares_timeout(channel, NULL, &tv);
    if (tv_ptr != NULL) {
        /* In a real event-driven system, we'd set a timer event */
        /* For fuzzing, we just simulate time passing */
    }
    
    /* Clean up */
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_010

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
#include "ares.h"
#include "ares_dns_record.h"

namespace harness_008 {
/* Comprehensive fuzzing harness for c-ares library targeting socket callback functions
 * and socket management APIs identified as critical coverage gaps:
 * - ares_set_socket_functions_ex (socket callback mechanism)
 * - ares_getsock (socket state querying)
 * 
 * This harness implements custom socket callback functions to test the socket
 * function override mechanism, which allows integration with custom event loops
 * and socket management systems.
 * 
 * Semantic diversity from existing harnesses (000-007):
 * - harness_000: Modern APIs (getaddrinfo, DNS record)
 * - harness_001: Legacy DNS processing pipeline  
 * - harness_002: Event processing and channel management
 * - harness_003: Server configuration and option setting
 * - harness_004: Legacy DNS parsing and query creation
 * - harness_005: Legacy DNS mapping and Android/system config
 * - harness_006: Metrics and search operations
 * - harness_007: DNS record binary data manipulation
 * - harness_008: Socket callback mechanisms and socket management (NEW)
 */



/* Custom socket callback functions for ares_set_socket_functions_ex */

/* Custom socket creation function */
static ares_socket_t custom_asocket(int domain, int type, int protocol, void *user_data) {
    (void)user_data;
    
    /* Try to create socket, but don't fail if it doesn't work */
    ares_socket_t sock = socket(domain, type, protocol);
    if (sock == ARES_SOCKET_BAD) {
        /* Set errno appropriately */
        errno = EAFNOSUPPORT;
        return ARES_SOCKET_BAD;
    }
    
    /* Set non-blocking mode */
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags != -1) {
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    }
    
    return sock;
}

/* Custom socket close function */
static int custom_aclose(ares_socket_t sock, void *user_data) {
    (void)user_data;
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    return close(sock);
}

/* Custom socket option setting function */
static int custom_asetsockopt(ares_socket_t sock, ares_socket_opt_t opt, const void *val,
                              ares_socklen_t val_size, void *user_data) {
    (void)user_data;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    /* Map c-ares socket options to system socket options */
    int sys_opt = 0;
    int level = 0;
    
    switch (opt) {
        case ARES_SOCKET_OPT_SENDBUF_SIZE:
            sys_opt = SO_SNDBUF;
            level = SOL_SOCKET;
            break;
        case ARES_SOCKET_OPT_RECVBUF_SIZE:
            sys_opt = SO_RCVBUF;
            level = SOL_SOCKET;
            break;
        case ARES_SOCKET_OPT_BIND_DEVICE:
            /* SO_BINDTODEVICE may not be available on all systems */
            errno = ENOSYS;
            return -1;
        case ARES_SOCKET_OPT_TCP_FASTOPEN:
            /* TCP_FASTOPEN may not be available on all systems */
            errno = ENOSYS;
            return -1;
        default:
            errno = EINVAL;
            return -1;
    }
    
    return setsockopt(sock, level, sys_opt, val, val_size);
}

/* Custom socket connect function */
static int custom_aconnect(ares_socket_t sock, const struct sockaddr *address,
                           ares_socklen_t address_len, unsigned int flags,
                           void *user_data) {
    (void)flags;
    (void)user_data;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    /* Try to connect (will likely fail with EINPROGRESS due to non-blocking socket) */
    int result = connect(sock, address, address_len);
    if (result == -1 && errno == EINPROGRESS) {
        /* This is expected for non-blocking sockets */
        return -1;
    }
    
    return result;
}

/* Custom socket receive function */
static ares_ssize_t custom_arecvfrom(ares_socket_t sock, void *buffer, size_t length,
                                     int flags, struct sockaddr *address,
                                     ares_socklen_t *address_len, void *user_data) {
    (void)flags;
    (void)user_data;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    return recvfrom(sock, buffer, length, 0, address, address_len);
}

/* Custom socket send function */
static ares_ssize_t custom_asendto(ares_socket_t sock, const void *buffer, size_t length,
                                   int flags, const struct sockaddr *address,
                                   ares_socklen_t address_len, void *user_data) {
    (void)flags;
    (void)user_data;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    if (address == NULL) {
        return send(sock, buffer, length, 0);
    } else {
        return sendto(sock, buffer, length, 0, address, address_len);
    }
}

/* Custom socket get name function */
static int custom_agetsockname(ares_socket_t sock, struct sockaddr *address,
                               ares_socklen_t *address_len, void *user_data) {
    (void)user_data;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    return getsockname(sock, address, address_len);
}

/* Custom socket bind function */
static int custom_abind(ares_socket_t sock, unsigned int flags,
                        const struct sockaddr *address, socklen_t address_len,
                        void *user_data) {
    (void)flags;
    (void)user_data;
    
    if (sock == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    return bind(sock, address, address_len);
}

/* Custom interface name to index function */
static unsigned int custom_aif_nametoindex(const char *ifname, void *user_data) {
    (void)user_data;
    
    /* Simple implementation - return 1 for "lo", 0 otherwise */
    if (strcmp(ifname, "lo") == 0) {
        return 1;
    }
    return 0;
}

/* Custom interface index to name function */
static const char *custom_aif_indextoname(unsigned int ifindex, char *ifname_buf,
                                          size_t ifname_buf_len, void *user_data) {
    (void)user_data;
    
    if (ifindex == 1 && ifname_buf_len >= 3) {
        strncpy(ifname_buf, "lo", ifname_buf_len);
        ifname_buf[ifname_buf_len - 1] = '\0';
        return ifname_buf;
    }
    return NULL;
}

/* Simple callback for DNS operations */
static void dummy_dns_callback(void *arg, ares_status_t status,
                               size_t timeouts, const ares_dns_record_t *dnsrec) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    /* Minimum size check */
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    /* Initialize c-ares library */
    ares_library_init(ARES_LIB_INIT_ALL);

    /* Create initial channel without custom socket functions */
    ares_channel_t *channel = NULL;
    int init_result = ares_init(&channel);
    if (init_result != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    /* Consume fixed-size data for operation selection and parameters */
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t socket_family = fdp.ConsumeIntegral<uint8_t>() % 2;  /* 0=AF_INET, 1=AF_INET6 */
    uint16_t port = fdp.ConsumeIntegral<uint16_t>();
    uint8_t use_custom_sockets = fdp.ConsumeBool();
    uint8_t test_getsock = fdp.ConsumeBool();

    /* Consume variable-size data */
    std::string domain_name = fdp.ConsumeRandomLengthString(128);
    std::vector<uint8_t> addr_data;
    
    if (socket_family == 0) {
        /* IPv4 address */
        addr_data = fdp.ConsumeBytes<uint8_t>(4);
    } else {
        /* IPv6 address */
        addr_data = fdp.ConsumeBytes<uint8_t>(16);
    }

    /* Set up custom socket functions if requested */
    struct ares_socket_functions_ex custom_funcs;
    void *user_data = NULL;
    
    if (use_custom_sockets) {
        memset(&custom_funcs, 0, sizeof(custom_funcs));
        custom_funcs.version = 1;
        custom_funcs.flags = 0;
        custom_funcs.asocket = custom_asocket;
        custom_funcs.aclose = custom_aclose;
        custom_funcs.asetsockopt = custom_asetsockopt;
        custom_funcs.aconnect = custom_aconnect;
        custom_funcs.arecvfrom = custom_arecvfrom;
        custom_funcs.asendto = custom_asendto;
        custom_funcs.agetsockname = custom_agetsockname;
        custom_funcs.abind = custom_abind;
        custom_funcs.aif_nametoindex = custom_aif_nametoindex;
        custom_funcs.aif_indextoname = custom_aif_indextoname;
        
        /* Apply custom socket functions */
        ares_status_t set_result = ares_set_socket_functions_ex(channel, &custom_funcs, user_data);
        if (set_result != ARES_SUCCESS) {
            /* Clean up and exit if setting socket functions fails */
            ares_destroy(channel);
            ares_library_cleanup();
            return 0;
        }
    }

    /* Main operation switch */
    switch (operation) {
        case 0: {
            /* Test ares_getsock - socket state querying */
            if (test_getsock) {
                ares_socket_t socks[10];
                int num_socks = fdp.ConsumeIntegralInRange<int>(1, 10);
                
                /* Call ares_getsock to get socket information */
                int bitmap = ares_getsock(channel, socks, num_socks);
                
                /* The bitmap contains readable/writable socket information */
                (void)bitmap; /* Use the result to avoid unused variable warning */
            }
            break;
        }
        
        case 1: {
            /* Test basic DNS query with custom socket functions */
            ares_dns_record_t *dnsrec = NULL;
            ares_status_t create_result = ares_dns_record_create(&dnsrec, 0, 0, 
                                                                 ARES_OPCODE_QUERY, 
                                                                 ARES_RCODE_NOERROR);
            if (create_result == ARES_SUCCESS && dnsrec != NULL) {
                /* Add a query to the DNS record */
                ares_status_t query_result = ares_dns_record_query_add(
                    dnsrec, domain_name.c_str(), ARES_REC_TYPE_A, ARES_CLASS_IN);
                
                if (query_result == ARES_SUCCESS) {
                    /* Send the query using the channel (with potential custom sockets) */
                    unsigned short qid = 0;
                    ares_status_t send_result = ares_send_dnsrec(channel, dnsrec,
                                                                 dummy_dns_callback, NULL, &qid);
                    (void)send_result; /* Use result to avoid unused variable warning */
                }
                
                /* Clean up DNS record */
                ares_dns_record_destroy(dnsrec);
            }
            break;
        }
        
        case 2: {
            /* Test event processing with custom sockets */
            ares_fd_events_t events[2];
            events[0].fd = ARES_SOCKET_BAD;
            events[0].events = ARES_FD_EVENT_NONE;
            events[1].fd = ARES_SOCKET_BAD;
            events[1].events = ARES_FD_EVENT_NONE;
            
            /* Process any pending events */
            ares_status_t process_result = ares_process_fds(channel, events, 0, 
                                                           ARES_PROCESS_FLAG_NONE);
            (void)process_result; /* Use result to avoid unused variable warning */
            break;
        }
        
        case 3: {
            /* Test socket option setting through custom functions */
            /* Create a simple DNS record first to ensure some socket activity */
            ares_dns_record_t *dnsrec = NULL;
            ares_status_t create_result = ares_dns_record_create(&dnsrec, 0, 0,
                                                                 ARES_OPCODE_QUERY,
                                                                 ARES_RCODE_NOERROR);
            if (create_result == ARES_SUCCESS && dnsrec != NULL) {
                ares_dns_record_query_add(dnsrec, domain_name.c_str(), 
                                         ARES_REC_TYPE_A, ARES_CLASS_IN);
                
                /* Send query to potentially create sockets */
                unsigned short qid2 = 0;
                ares_send_dnsrec(channel, dnsrec, dummy_dns_callback, NULL, &qid2);
                /* Process any pending events to advance socket state */
                ares_process_fds(channel, NULL, 0, ARES_PROCESS_FLAG_NONE);
                
                ares_dns_record_destroy(dnsrec);
            }
            break;
        }
    }

    /* Test timeout processing */
    struct timeval maxtv;
    struct timeval tv;
    maxtv.tv_sec = 0;
    maxtv.tv_usec = 1000; /* 1ms timeout */
    
    const struct timeval *timeout_result = ares_timeout(channel, &maxtv, &tv);
    (void)timeout_result; /* Use result to avoid unused variable warning */

    /* Clean up */
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}

} // namespace harness_008

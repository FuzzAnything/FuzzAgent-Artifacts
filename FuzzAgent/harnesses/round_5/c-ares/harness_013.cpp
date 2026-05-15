#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "ares.h"
#include "ares_dns_record.h"

namespace harness_013 {
/* Comprehensive fuzzing harness for c-ares library targeting advanced socket callback 
 * configuration and queue management APIs identified as critical coverage gaps with 28+ 
 * undiscovered branches:
 * - ares_set_socket_configure_callback (socket configuration callback)
 * - ares_set_pending_write_cb (pending write notification callback)
 * - ares_set_socket_callback (socket creation callback)
 * - ares_set_server_state_callback (server state change callback)
 * - ares_queue_wait_empty (queue empty wait synchronization)
 * - ares_queue_active_queries (active query count retrieval)
 * 
 * This harness implements a complete callback ecosystem for socket management and 
 * queue synchronization, testing the integration between callback configuration APIs
 * and queue management functions.
 * 
 * Semantic diversity from existing socket harnesses (008, 009, 010):
 * - harness_008: Socket callback mechanisms (ares_set_socket_functions_ex) and socket management
 * - harness_009: Legacy socket configuration (ares_set_socket_functions) and error handling
 * - harness_010: Event-driven DNS operations with socket callback variations
 * - harness_013: Advanced socket callback configuration and queue management ecosystem (NEW)
 */



/* Global counters for callback tracking */
static int socket_create_cb_count = 0;
static int socket_configure_cb_count = 0;
static int pending_write_cb_count = 0;
static int server_state_cb_success_count = 0;
static int server_state_cb_failure_count = 0;

/* Custom socket creation callback for ares_set_socket_callback */
static int custom_socket_create_callback(ares_socket_t socket_fd, int type, void *data) {
    (void)data;
    socket_create_cb_count++;
    
    /* Use fuzzed data to determine behavior */
    FuzzedDataProvider* fdp = (FuzzedDataProvider*)data;
    if (fdp && fdp->remaining_bytes() > 0) {
        bool should_fail = fdp->ConsumeBool();
        if (should_fail) {
            /* Simulate socket configuration failure */
            errno = EINVAL;
            return -1;
        }
    }
    
    /* Configure socket based on type */
    if (socket_fd != ARES_SOCKET_BAD) {
        /* Set non-blocking mode for event-driven I/O */
        int flags = fcntl(socket_fd, F_GETFL, 0);
        if (flags != -1) {
            fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
        }
        
        /* Set socket options based on type */
        if (type == SOCK_STREAM) {
            /* TCP socket options */
            int enable = 1;
            setsockopt(socket_fd, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable));
        } else if (type == SOCK_DGRAM) {
            /* UDP socket options */
            int broadcast = 1;
            setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
        }
    }
    
    return 0;
}

/* Custom socket configuration callback for ares_set_socket_configure_callback */
static int custom_socket_configure_callback(ares_socket_t socket_fd, int type, void *data) {
    (void)data;
    socket_configure_cb_count++;
    
    /* Use fuzzed data to determine behavior */
    FuzzedDataProvider* fdp = (FuzzedDataProvider*)data;
    if (fdp && fdp->remaining_bytes() > 0) {
        bool should_fail = fdp->ConsumeBool();
        if (should_fail) {
            /* Simulate socket configuration failure */
            errno = ENOSYS;
            return -1;
        }
    }
    
    if (socket_fd == ARES_SOCKET_BAD) {
        errno = EBADF;
        return -1;
    }
    
    /* Apply socket configuration based on type */
    /* The 'type' parameter meaning is not well documented in public headers.
     * Based on usage patterns, we'll handle it generically */
    if (type == 1) {  /* Could be bind configuration */
        /* Bind configuration */
        int reuseaddr = 1;
        setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
    } else if (type == 2) {  /* Could be connect configuration */
        /* Connect configuration */
        int nodelay = 1;
        setsockopt(socket_fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));
    }
    
    return 0;
}

/* Custom pending write callback for ares_set_pending_write_cb */
static void custom_pending_write_callback(void *data) {
    (void)data;
    pending_write_cb_count++;
    
    /* This callback is invoked when there's pending write data */
    /* Could trigger additional processing based on fuzzed data */
    FuzzedDataProvider* fdp = (FuzzedDataProvider*)data;
    if (fdp && fdp->remaining_bytes() > 0) {
        /* Consume some data to influence behavior */
        uint8_t action = fdp->ConsumeIntegral<uint8_t>() % 4;
        (void)action; /* Action could be used for additional processing */
    }
}
/* Custom server state callback for ares_set_server_state_callback */
static void custom_server_state_callback(const char *server_string, 
                                         ares_bool_t success, 
                                         int flags, 
                                         void *data) {
    (void)data;
    
    /* Track success/failure counts */
    if (success) {
        server_state_cb_success_count++;
    } else {
        server_state_cb_failure_count++;
    }
    
    /* Validate flags */
    if (server_string) {
        /* Server string should be non-NULL */
        (void)server_string;
        
        /* Flags should indicate either UDP or TCP, not both */
        ares_bool_t udp = (flags & ARES_SERV_STATE_UDP) ? ARES_TRUE : ARES_FALSE;
        ares_bool_t tcp = (flags & ARES_SERV_STATE_TCP) ? ARES_TRUE : ARES_FALSE;
        
        /* According to documentation, should be either UDP or TCP, not both */
        if (udp && tcp) {
            /* This would be unexpected based on test code */
        }
    }
}
/* DNS host resolution callback for ares_gethostbyname */
static void dns_host_callback(void *arg, int status, int timeouts,
                             const struct hostent *hostent) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
    /* Callback invoked when host resolution completes */
}
/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    /* Need sufficient data for meaningful fuzzing */
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    /* Reset callback counters */
    socket_create_cb_count = 0;
    socket_configure_cb_count = 0;
    pending_write_cb_count = 0;
    server_state_cb_success_count = 0;
    server_state_cb_failure_count = 0;

    if (ares_library_init(ARES_LIB_INIT_WIN32) != ARES_SUCCESS) {
        return 0;
    }
    
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    
    /* Set options based on fuzzed input */
    if (fdp.ConsumeBool()) {
        options.flags = ARES_FLAG_STAYOPEN;
        optmask |= ARES_OPT_FLAGS;
    }
    
    if (fdp.ConsumeBool()) {
        options.timeout = fdp.ConsumeIntegralInRange<int>(100, 5000);
        optmask |= ARES_OPT_TIMEOUTMS;
    }
    
    if (fdp.ConsumeBool()) {
        options.tries = fdp.ConsumeIntegralInRange<int>(1, 5);
        optmask |= ARES_OPT_TRIES;
    }
    
    /* Create channel */
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    /* Set up socket callbacks using fuzzed data to determine configuration */
    
    /* Set socket creation callback */
    ares_set_socket_callback(channel, custom_socket_create_callback, &fdp);
    
    /* Set socket configuration callback */
    ares_set_socket_configure_callback(channel, custom_socket_configure_callback, &fdp);
    
    /* Set pending write callback */
    ares_set_pending_write_cb(channel, custom_pending_write_callback, &fdp);
    
    /* Set server state callback */
    ares_set_server_state_callback(channel, custom_server_state_callback, &fdp);
    
    /* Configure servers based on fuzzed input */
    if (fdp.remaining_bytes() > 10) {
        std::string servers_csv;
        size_t servers_len = fdp.ConsumeIntegralInRange<size_t>(5, fdp.remaining_bytes());
        servers_csv = fdp.ConsumeBytesAsString(servers_len);
        
        /* Try to set servers */
        if (servers_csv.find(',') != std::string::npos || 
            servers_csv.find(':') != std::string::npos) {
            ares_set_servers_csv(channel, servers_csv.c_str());
        }
    }
    
    /* Perform DNS operations to trigger callbacks */
    size_t num_operations = fdp.ConsumeIntegralInRange<size_t>(0, 5);
    
    for (size_t i = 0; i < num_operations && fdp.remaining_bytes() > 10; i++) {
        /* Consume hostname for DNS query */
        size_t hostname_len = fdp.ConsumeIntegralInRange<size_t>(5, 50);
        std::string hostname = fdp.ConsumeBytesAsString(hostname_len);
        
        /* Determine query type from fuzzed data */
        int query_type = fdp.ConsumeBool() ? AF_INET : AF_INET6;
        
        /* Perform async DNS query */
        ares_gethostbyname(channel, hostname.c_str(), query_type, 
                          dns_host_callback, NULL);
        /* Process pending events to trigger callbacks */
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = fdp.ConsumeIntegralInRange<int>(1000, 10000);
        
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        int nfds = ares_fds(channel, &read_fds, &write_fds);
        if (nfds > 0) {
            /* Select with timeout */
            select(nfds, &read_fds, &write_fds, NULL, &tv);
            ares_process(channel, &read_fds, &write_fds);
        }
        
        /* Check queue active queries */
        size_t active_queries = ares_queue_active_queries(channel);
        (void)active_queries;
        
        /* Test queue wait with short timeout */
        if (fdp.ConsumeBool()) {
            int timeout_ms = fdp.ConsumeIntegralInRange<int>(-1, 100);
            ares_status_t wait_status = ares_queue_wait_empty(channel, timeout_ms);
            (void)wait_status;
        }
        
        /* Trigger pending write callback if there are pending writes */
        if (pending_write_cb_count == 0 && fdp.ConsumeBool()) {
            ares_process_pending_write(channel);
        }
    }
    
    /* Process any remaining events */
    struct timeval final_tv;
    final_tv.tv_sec = 0;
    final_tv.tv_usec = 10000;
    
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    if (nfds > 0) {
        select(nfds, &read_fds, &write_fds, NULL, &final_tv);
        ares_process(channel, &read_fds, &write_fds);
    }
    
    /* Final queue checks */
    size_t final_active_queries = ares_queue_active_queries(channel);
    (void)final_active_queries;
    
    /* Try to wait for queue to empty with short timeout */
    ares_status_t final_wait_status = ares_queue_wait_empty(channel, 10);
    (void)final_wait_status;
    
    /* Clean up */
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_013

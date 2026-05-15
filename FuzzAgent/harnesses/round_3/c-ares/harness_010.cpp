#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include "ares.h"
#include "ares_nameser.h"

namespace harness_010 {

// Custom socket functions for testing ares_set_socket_functions
static ares_socket_t custom_asocket(int domain, int type, int protocol, void *user_data) {
    // Return ARES_SOCKET_BAD to simulate socket creation failure
    // or a mock socket value based on fuzzer input
    (void)domain;
    (void)type;
    (void)protocol;
    (void)user_data;
    return ARES_SOCKET_BAD;  // Always fail to test error paths
}

static int custom_aclose(ares_socket_t sock, void *user_data) {
    (void)sock;
    (void)user_data;
    return 0;  // Success
}

static int custom_aconnect(ares_socket_t sock, const struct sockaddr *addr, 
                           ares_socklen_t addrlen, void *user_data) {
    (void)sock;
    (void)addr;
    (void)addrlen;
    (void)user_data;
    return -1;  // Simulate connection failure
}

static ares_ssize_t custom_arecvfrom(ares_socket_t sock, void *buf, size_t len, int flags,
                                     struct sockaddr *src_addr, ares_socklen_t *addrlen, 
                                     void *user_data) {
    (void)sock;
    (void)buf;
    (void)len;
    (void)flags;
    (void)src_addr;
    (void)addrlen;
    (void)user_data;
    return -1;  // Simulate recv failure
}

static ares_ssize_t custom_asendv(ares_socket_t sock, const struct iovec *vector, 
                                  int count, void *user_data) {
    (void)sock;
    (void)vector;
    (void)count;
    (void)user_data;
    return -1;  // Simulate send failure
}

// Socket callback for ares_set_socket_callback
static int socket_create_callback(ares_socket_t socket_fd, int type, void *data) {
    (void)socket_fd;
    (void)type;
    (void)data;
    return 0;  // Success
}

// Socket configuration callback for ares_set_socket_configure_callback
static int socket_config_callback(ares_socket_t socket_fd, int type, void *data) {
    (void)socket_fd;
    (void)type;
    (void)data;
    return 0;  // Success
}

// Pending write callback for ares_set_pending_write_cb
static void pending_write_callback(void *data) {
    (void)data;
    // Do nothing, just satisfy the callback
}

// Simple query callback for DNS queries
static void query_callback(void *arg, int status, int timeouts, 
                          const unsigned char *abuf, int alen) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
    // Simple callback - does nothing, just satisfies API requirements
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for initialization and configuration
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // Step 2: Initialize ares channel
    ares_channel_t* channel = NULL;
    int init_result = ares_init(&channel);
    if (init_result != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Step 3: Set up socket functions structure
    struct ares_socket_functions sock_funcs;
    sock_funcs.asocket = custom_asocket;
    sock_funcs.aclose = custom_aclose;
    sock_funcs.aconnect = custom_aconnect;
    sock_funcs.arecvfrom = custom_arecvfrom;
    sock_funcs.asendv = custom_asendv;
    
    // Call ares_set_socket_functions
    ares_set_socket_functions(channel, &sock_funcs, NULL);
    
    // Step 4: Set socket callback
    ares_set_socket_callback(channel, socket_create_callback, NULL);
    
    // Step 5: Set socket configuration callback  
    ares_set_socket_configure_callback(channel, socket_config_callback, NULL);
    
    // Step 6: Set pending write callback
    ares_set_pending_write_cb(channel, pending_write_callback, NULL);
    
    // Step 7: Perform DNS query to test the configuration
    // Consume hostname from fuzzer input
    std::string hostname = fdp.ConsumeRandomLengthString(253); // Max DNS name length
    
    // Consume query type from fuzzer input
    int query_types[] = {
        T_A,      // A record = 1
        T_AAAA,   // AAAA record = 28  
        T_MX,     // MX record = 15
        T_TXT,    // TXT record = 16
        T_SRV,    // SRV record = 33
        T_PTR,    // PTR record = 12
        T_NS,     // NS record = 2
        T_CNAME,  // CNAME record = 5
        T_SOA,    // SOA record = 6
        T_ANY     // ANY record = 255
    };
    
    size_t type_idx = 0;
    if (fdp.remaining_bytes() >= 1) {
        type_idx = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(query_types)/sizeof(query_types[0]) - 1);
    }
    
    // Perform the query
    ares_query(channel, hostname.c_str(), C_IN, query_types[type_idx], query_callback, NULL);
    
    // Step 8: Process events (non-blocking)
    // Get file descriptors
    ares_socket_t sockets[ARES_GETSOCK_MAXNUM];
    int bitmask = ares_getsock(channel, sockets, ARES_GETSOCK_MAXNUM);
    
    // Process file descriptors if any are ready
    if (bitmask != 0) {
        for (int i = 0; i < ARES_GETSOCK_MAXNUM; i++) {
            if (ARES_GETSOCK_READABLE(bitmask, i) || ARES_GETSOCK_WRITABLE(bitmask, i)) {
                if (sockets[i] != ARES_SOCKET_BAD) {
                    // Process the socket
                    ares_process_fd(channel, 
                                   ARES_GETSOCK_READABLE(bitmask, i) ? sockets[i] : ARES_SOCKET_BAD,
                                   ARES_GETSOCK_WRITABLE(bitmask, i) ? sockets[i] : ARES_SOCKET_BAD);
                }
            }
        }
    }
    
    // Step 9: Process pending writes if configured
    ares_process_pending_write(channel);
    
    // Step 10: Cancel any pending queries
    ares_cancel(channel);
    
    // Step 11: Cleanup
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_010

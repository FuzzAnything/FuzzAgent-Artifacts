#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include "ares.h"

namespace harness_009 {
/* Fuzzing harness for c-ares library targeting channel configuration and hostname file resolution operations
 * This harness tests comprehensive channel configuration APIs and file-based hostname resolution
 * Target APIs: ares_gethostbyname_file (24 undiscovered branches), ares_set_sortlist (19),
 *              ares_set_socket_functions (16), ares_queue_wait_empty (14), ares_library_init_mem (7),
 *              ares_set_local_ip4 (2), ares_set_local_dev (2), ares_set_local_ip6 (2),
 *              ares_set_socket_configure_callback (4), ares_set_pending_write_cb (4),
 *              ares_set_socket_callback (2), ares_version (2)
 * Required helper APIs: ares_library_init (initialization), ares_init_options (channel creation),
 *                       ares_destroy (cleanup), ares_library_cleanup (library cleanup)
 * Following invocation sequence from coverage guidance:
 * 1. ares_library_init() to initialize the c-ares library.
 * 2. ares_library_init_mem() for memory initialization.
 * 3. ares_init_options() to create a channel with configuration options.
 * 4. ares_set_sortlist() to configure sortlist for address sorting.
 * 5. ares_set_local_ip4(), ares_set_local_ip6(), ares_set_local_dev() to set local interface bindings.
 * 6. ares_set_socket_functions() to set custom socket functions.
 * 7. ares_set_socket_configure_callback(), ares_set_pending_write_cb(), ares_set_socket_callback() for socket event configuration.
 * 8. ares_gethostbyname_file() to resolve a hostname from /etc/hosts or local hosts file.
 * 9. ares_queue_wait_empty() to wait for pending queries (if any).
 * 10. ares_destroy() to clean up the channel.
 * 11. ares_library_cleanup() to clean up library resources.
 * 12. ares_version() to get library version string.
 * Differentiates from previous harnesses by focusing on channel configuration and file-based resolution.
 */



// Custom socket functions for ares_set_socket_functions
static ares_socket_t dummy_socket(int domain, int type, int protocol, void* user_data) {
    (void)domain; (void)type; (void)protocol; (void)user_data;
    return ARES_SOCKET_BAD;
}

static int dummy_close(ares_socket_t socket_fd, void* user_data) {
    (void)socket_fd; (void)user_data;
    return 0;
}

static int dummy_connect(ares_socket_t socket_fd, const struct sockaddr* addr,
                         ares_socklen_t addrlen, void* user_data) {
    (void)socket_fd; (void)addr; (void)addrlen; (void)user_data;
    return 0;
}

static ares_ssize_t dummy_recvfrom(ares_socket_t socket_fd, void* buffer,
                                   size_t buf_size, int flags,
                                   struct sockaddr* addr,
                                   ares_socklen_t* addrlen, void* user_data) {
    (void)socket_fd; (void)buffer; (void)buf_size; (void)flags;
    (void)addr; (void)addrlen; (void)user_data;
    return -1;
}

static ares_ssize_t dummy_sendv(ares_socket_t socket_fd,
                                const struct iovec* iov, int iovcnt,
                                void* user_data) {
    (void)socket_fd; (void)iov; (void)iovcnt; (void)user_data;
    return -1;
}

// Callback functions for socket configuration
static int dummy_socket_callback(ares_socket_t socket_fd, int type, void* user_data) {
    (void)socket_fd; (void)type; (void)user_data;
    return 0;
}

static int dummy_sock_config_callback(ares_socket_t socket_fd, int type, void* user_data) {
    (void)socket_fd; (void)type; (void)user_data;
    return 0;
}

static void dummy_pending_write_cb(void* user_data) {
    (void)user_data;
}
// Memory management functions for ares_library_init_mem
static void* dummy_malloc(size_t size) {
    return malloc(size);
}

static void dummy_free(void* ptr) {
    free(ptr);
}

static void* dummy_realloc(void* ptr, size_t size) {
    return realloc(ptr, size);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    // We need enough data for: initialization, configuration strings, hostnames, etc.
    if (size < 100) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Initialize c-ares library
    int init_result = ares_library_init(ARES_LIB_INIT_ALL);
    if (init_result != ARES_SUCCESS) {
        return 0;
    }

    // Step 2: Memory initialization with custom memory functions
    int mem_init_result = ares_library_init_mem(ARES_LIB_INIT_ALL, 
                                               dummy_malloc, 
                                               dummy_free, 
                                               dummy_realloc);
    // Note: ares_library_init_mem may return an error, but we continue anyway
    (void)mem_init_result;
    
    ares_channel_t* channel = NULL;
    int options_mask = fdp.ConsumeIntegral<int>();
    
    // Initialize channel with options
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    options.flags = fdp.ConsumeIntegral<int>();
    options.timeout = fdp.ConsumeIntegral<int>();
    options.tries = fdp.ConsumeIntegral<int>();
    options.ndots = fdp.ConsumeIntegral<int>();
    options.udp_port = fdp.ConsumeIntegral<unsigned short>();
    options.tcp_port = fdp.ConsumeIntegral<unsigned short>();
    
    // Consume some string data for sortlist
    std::string sortlist_str = fdp.ConsumeRandomLengthString(100);
    
    // Consume IP address data for local bindings
    unsigned int local_ip4 = fdp.ConsumeIntegral<unsigned int>();
    
    // For IPv6, we need 16 bytes
    std::vector<unsigned char> local_ip6_bytes;
    if (fdp.remaining_bytes() >= 16) {
        local_ip6_bytes = fdp.ConsumeBytes<unsigned char>(16);
    } else {
        // If not enough bytes, fill with zeros
        local_ip6_bytes.resize(16, 0);
        size_t bytes_to_consume = std::min(fdp.remaining_bytes(), (size_t)16);
        auto some_bytes = fdp.ConsumeBytes<unsigned char>(bytes_to_consume);
        std::copy(some_bytes.begin(), some_bytes.end(), local_ip6_bytes.begin());
    }
    
    // Consume device name string
    std::string local_dev_str = fdp.ConsumeRandomLengthString(50);
    
    // Consume hostname for file resolution
    std::string hostname = fdp.ConsumeRandomLengthString(100);
    
    int status = ares_init_options(&channel, &options, options_mask);
    if (status != ARES_SUCCESS || channel == NULL) {
        ares_library_cleanup();
        return 0;
    }

    // Step 4: Configure sortlist
    if (!sortlist_str.empty()) {
        ares_set_sortlist(channel, sortlist_str.c_str());
    }

    // Step 5: Set local interface bindings
    ares_set_local_ip4(channel, local_ip4);
    
    ares_set_local_ip6(channel, local_ip6_bytes.data());
    
    if (!local_dev_str.empty()) {
        ares_set_local_dev(channel, local_dev_str.c_str());
    }

    // Step 6: Set custom socket functions
    struct ares_socket_functions socket_funcs;
    socket_funcs.asocket = dummy_socket;
    socket_funcs.aclose = dummy_close;
    socket_funcs.aconnect = dummy_connect;
    socket_funcs.arecvfrom = dummy_recvfrom;
    socket_funcs.asendv = dummy_sendv;
    
    ares_set_socket_functions(channel, &socket_funcs, NULL);

    // Step 7: Set socket callbacks
    ares_set_socket_callback(channel, dummy_socket_callback, NULL);
    ares_set_socket_configure_callback(channel, dummy_sock_config_callback, NULL);
    ares_set_pending_write_cb(channel, dummy_pending_write_cb, NULL);

    // Step 8: File-based hostname resolution
    // Try to resolve the hostname from /etc/hosts
    if (!hostname.empty()) {
        struct hostent* host_result = NULL;
        // Try both IPv4 and IPv6 families
        int family = fdp.ConsumeBool() ? AF_INET : AF_INET6;
        
        int resolve_result = ares_gethostbyname_file(channel, hostname.c_str(), 
                                                    family, &host_result);
        
        // Note: host_result may be NULL if resolution fails, which is OK for fuzzing
        (void)resolve_result;
    }
    // Step 9: Wait for pending queries (if any)
    // Note: ares_queue_wait_empty may block, but with our dummy socket functions,
    // there shouldn't be any real pending queries
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(-1, 1000); // -1 for infinite, up to 1 second
    ares_status_t wait_status = ares_queue_wait_empty(channel, timeout_ms);
    (void)wait_status;
    ares_destroy(channel);

    // Step 11: Clean up library resources
    ares_library_cleanup();

    // Step 12: Get library version (for coverage)
    const char* version = ares_version(NULL);
    (void)version;

    return 0;
}

} // namespace harness_009

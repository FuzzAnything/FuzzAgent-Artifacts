#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ares.h"
#include "ares_dns.h"

namespace harness_007 {
/* Fuzzing harness for c-ares library targeting modern address resolution (getaddrinfo) operations
 * This harness tests the modern getaddrinfo interface for address resolution with fuzzer-generated input
 * Target APIs: ares_getaddrinfo (183 undiscovered branches), ares_freeaddrinfo (cleanup)
 * Required helper APIs: ares_library_init (library initialization), ares_init or ares_init_options (channel setup),
 *                       ares_destroy (channel cleanup), ares_library_cleanup (library cleanup)
 * Following invocation sequence from coverage guidance:
 * 1. ares_library_init to initialize the library
 * 2. ares_init or ares_init_options to create a channel
 * 3. ares_getaddrinfo with various hostname, service, and hint combinations
 * 4. ares_freeaddrinfo to free returned address information
 * 5. ares_destroy to clean up channel
 * 6. ares_library_cleanup to finalize library cleanup
 * 
 * Semantic differentiation from previous harnesses:
 * - harness_000: DNS packet parsing
 * - harness_001: query processing
 * - harness_002: channel initialization and basic DNS resolution
 * - harness_003: server configuration
 * - harness_004: DNS record manipulation
 * - harness_005: legacy reply parsing
 * - harness_006: cancellation and mapping operations
 * - This harness (007): Modern address resolution using getaddrinfo interface
 */



// Callback for ares_getaddrinfo - called when address resolution completes
static void getaddrinfo_callback(void* arg, int status, int timeouts,
                                 struct ares_addrinfo* result) {
    // Store the result in the argument pointer
    if (arg != NULL) {
        struct ares_addrinfo** result_ptr = (struct ares_addrinfo**)arg;
        *result_ptr = result;
    }
    
    // Note: We don't free the result here - it will be freed in the main function
    // after we process it or if there was an error
    (void)status;
    (void)timeouts;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    // We need enough data for initialization, hostname, service, hints, and callback data
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Initialize the library
    int library_status = ares_library_init(ARES_LIB_INIT_ALL);
    if (library_status != ARES_SUCCESS) {
        // If library initialization fails, we can't proceed
        return 0;
    }

    // Step 2: Create channel with ares_init_options
    ares_channel_t* channel = NULL;
    ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    
    // Consume fuzzer data for channel initialization parameters
    int timeout = fdp.ConsumeIntegralInRange<int>(1, 10000);
    int tries = fdp.ConsumeIntegralInRange<int>(1, 10);
    int ndots = fdp.ConsumeIntegralInRange<int>(0, 15);
    bool use_tcp = fdp.ConsumeBool();
    bool use_edns = fdp.ConsumeBool();
    bool use_norecurse = fdp.ConsumeBool();
    
    options.timeout = timeout;
    optmask |= ARES_OPT_TIMEOUTMS;
    
    options.tries = tries;
    optmask |= ARES_OPT_TRIES;
    
    options.ndots = ndots;
    optmask |= ARES_OPT_NDOTS;
    
    // Set flags based on fuzzed data
    if (use_tcp) {
        options.flags |= ARES_FLAG_USEVC;
    }
    if (use_edns) {
        options.flags |= ARES_FLAG_EDNS;
    }
    if (use_norecurse) {
        options.flags |= ARES_FLAG_NORECURSE;
    }
    if (options.flags != 0) {
        optmask |= ARES_OPT_FLAGS;
    }
    
    if (use_edns) {
        options.ednspsz = fdp.ConsumeIntegralInRange<int>(0, 4096);
        optmask |= ARES_OPT_EDNSPSZ;
    }
    
    // Try creating channel with ares_init_options
    int channel_status = ares_init_options(&channel, &options, optmask);
    if (channel_status != ARES_SUCCESS) {
        // If ares_init_options fails, try the simpler ares_init
        channel_status = ares_init(&channel);
        if (channel_status != ARES_SUCCESS) {
            // Clean up library if channel creation fails
            ares_library_cleanup();
            return 0;
        }
    }

    // Step 3: Prepare parameters for ares_getaddrinfo
    
    // Consume hostname (node parameter)
    std::string node;
    if (fdp.ConsumeBool()) {
        // Sometimes use an IPv4 address
        uint32_t ipv4 = fdp.ConsumeIntegral<uint32_t>();
        struct in_addr addr;
        addr.s_addr = ipv4;
        node = inet_ntoa(addr);
    } else if (fdp.ConsumeBool()) {
        // Sometimes use an IPv6 address (simplified representation)
        std::vector<uint8_t> ipv6_bytes = fdp.ConsumeBytes<uint8_t>(16);
        if (ipv6_bytes.size() == 16) {
            char ipv6_str[INET6_ADDRSTRLEN];
            struct in6_addr addr;
            memcpy(&addr, ipv6_bytes.data(), 16);
            if (inet_ntop(AF_INET6, &addr, ipv6_str, INET6_ADDRSTRLEN) != NULL) {
                node = ipv6_str;
            } else {
                // Fall back to a hostname
                node = fdp.ConsumeRandomLengthString(256);
            }
        } else {
            node = fdp.ConsumeRandomLengthString(256);
        }
    } else {
        // Most often use a random hostname
        node = fdp.ConsumeRandomLengthString(256);
    }
    
    // Consume service (port/service name)
    std::string service;
    if (fdp.ConsumeBool()) {
        // Use a numeric port
        uint16_t port = fdp.ConsumeIntegral<uint16_t>();
        service = std::to_string(port);
    } else {
        // Use a service name
        service = fdp.ConsumeRandomLengthString(64);
    }
    
    // Prepare hints structure
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    
    // Consume hints flags
    if (fdp.ConsumeBool()) {
        hints.ai_flags |= ARES_AI_NUMERICHOST;
    }
    if (fdp.ConsumeBool()) {
        hints.ai_flags |= ARES_AI_NUMERICSERV;
    }
    if (fdp.ConsumeBool()) {
        hints.ai_flags |= ARES_AI_CANONNAME;
    }
    if (fdp.ConsumeBool()) {
        hints.ai_flags |= ARES_AI_PASSIVE;
    }
    if (fdp.ConsumeBool()) {
        hints.ai_flags |= ARES_AI_V4MAPPED;
    }
    if (fdp.ConsumeBool()) {
        hints.ai_flags |= ARES_AI_ALL;
    }
    if (fdp.ConsumeBool()) {
        hints.ai_flags |= ARES_AI_ADDRCONFIG;
    }
    
    // Consume address family
    int family_choice = fdp.ConsumeIntegralInRange<int>(0, 3);
    switch (family_choice) {
        case 0:
            hints.ai_family = AF_UNSPEC;  // Both IPv4 and IPv6
            break;
        case 1:
            hints.ai_family = AF_INET;    // IPv4 only
            break;
        case 2:
            hints.ai_family = AF_INET6;   // IPv6 only
            break;
        default:
            hints.ai_family = AF_UNSPEC;
            break;
    }
    
    // Consume socket type
    int socktype_choice = fdp.ConsumeIntegralInRange<int>(0, 3);
    switch (socktype_choice) {
        case 0:
            hints.ai_socktype = 0;        // Any socket type
            break;
        case 1:
            hints.ai_socktype = SOCK_STREAM;  // Stream socket
            break;
        case 2:
            hints.ai_socktype = SOCK_DGRAM;   // Datagram socket
            break;
        default:
            hints.ai_socktype = 0;
            break;
    }
    
    // Consume protocol
    int protocol_choice = fdp.ConsumeIntegralInRange<int>(0, 3);
    switch (protocol_choice) {
        case 0:
            hints.ai_protocol = 0;        // Any protocol
            break;
        case 1:
            hints.ai_protocol = IPPROTO_TCP;  // TCP
            break;
        case 2:
            hints.ai_protocol = IPPROTO_UDP;  // UDP
            break;
        default:
            hints.ai_protocol = 0;
            break;
    }

    // Prepare callback argument - pointer to store the result
    struct ares_addrinfo* result = NULL;
    
    // Call ares_getaddrinfo - this is asynchronous
    ares_getaddrinfo(channel, node.c_str(), service.c_str(), 
                     (hints.ai_flags != 0 || hints.ai_family != 0 || 
                      hints.ai_socktype != 0 || hints.ai_protocol != 0) ? &hints : NULL,
                     getaddrinfo_callback, &result);
    
    // Process pending events to get the callback invoked
    // We need to process the channel to get the callback to fire
    int nfds;
    fd_set read_fds, write_fds;
    struct timeval tv;
    
    // Set a short timeout to avoid hanging
    tv.tv_sec = 0;
    tv.tv_usec = 10000;  // 10ms
    
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    nfds = ares_fds(channel, &read_fds, &write_fds);
    if (nfds > 0) {
        // Note: In a real fuzzing environment, we would process the socket events
        // For fuzzing purposes, we'll just timeout quickly
        select(nfds, &read_fds, &write_fds, NULL, &tv);
        ares_process(channel, &read_fds, &write_fds);
    }
    
    // Also try ares_process_fd for good measure
    ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    
    // Step 4: Free the address info if we got a result
    if (result != NULL) {
        ares_freeaddrinfo(result);
        result = NULL;
    }
    
    // Step 5: Clean up channel
    if (channel != NULL) {
        ares_destroy(channel);
        channel = NULL;
    }
    
    // Step 6: Final library cleanup
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_007

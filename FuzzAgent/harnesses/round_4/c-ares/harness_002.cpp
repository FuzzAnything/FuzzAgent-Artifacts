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
#include "ares.h"
#include "ares_dns.h"

namespace harness_002 {
/* Fuzzing harness for c-ares library targeting channel initialization and basic DNS resolution
 * This harness tests channel lifecycle APIs with fuzzer-generated input
 * Target APIs: ares_dup (385 undiscovered branches), ares_init (277), ares_reinit (224), 
 *              ares_getnameinfo (250), ares_gethostbyaddr (250)
 * Helper APIs: ares_library_init, ares_init_options, ares_destroy, ares_library_cleanup
 * Following invocation sequence from coverage guidance:
 * 1. ares_library_init for global initialization
 * 2. ares_init or ares_init_options to create primary channel
 * 3. ares_dup to duplicate channel
 * 4. ares_reinit to reinitialize channel
 * 5. ares_gethostbyaddr or ares_getnameinfo on both channels for DNS resolution
 * 6. ares_destroy cleanup on both channels
 * 7. ares_library_cleanup for global cleanup
 */


// Callback for ares_gethostbyaddr
static void gethostbyaddr_callback(void* arg, int status, int timeouts,
                                   const struct hostent* hostent) {
    // Callback doesn't need to do anything for fuzzing - just consume the result
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
}

// Callback for ares_getnameinfo  
static void getnameinfo_callback(void* arg, int status, int timeouts,
                                 const char* node, const char* service) {
    // Callback doesn't need to do anything for fuzzing - just consume the result
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)node;
    (void)service;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    // We need enough data for initialization parameters, IP addresses, and hostnames
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Step 2: Create primary channel with ares_init_options
    ares_channel_t* primary_channel = NULL;
    ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    
    // Consume fuzzer data for initialization parameters
    int timeout = fdp.ConsumeIntegralInRange<int>(1, 10000);
    int tries = fdp.ConsumeIntegralInRange<int>(1, 10);
    int ndots = fdp.ConsumeIntegralInRange<int>(0, 15);
    bool use_tcp = fdp.ConsumeBool();
    bool use_edns = fdp.ConsumeBool();
    
    options.timeout = timeout;
    optmask |= ARES_OPT_TIMEOUTMS;
    
    options.tries = tries;
    optmask |= ARES_OPT_TRIES;
    
    options.ndots = ndots;
    optmask |= ARES_OPT_NDOTS;
    
    if (use_tcp) {
        options.flags |= ARES_FLAG_USEVC;
        optmask |= ARES_OPT_FLAGS;
    }
    
    if (use_edns) {
        options.flags |= ARES_FLAG_EDNS;
        optmask |= ARES_OPT_FLAGS;
        options.ednspsz = fdp.ConsumeIntegralInRange<int>(0, 4096);
        optmask |= ARES_OPT_EDNSPSZ;
    }
    
    // Try creating primary channel with ares_init_options
    int status = ares_init_options(&primary_channel, &options, optmask);
    if (status != ARES_SUCCESS) {
        // If ares_init_options fails, try the deprecated ares_init
        status = ares_init(&primary_channel);
        if (status != ARES_SUCCESS) {
            ares_library_cleanup();
            return 0;
        }
    }

    // Step 3: Duplicate channel using ares_dup
    ares_channel_t* duplicated_channel = NULL;
    int dup_status = ares_dup(&duplicated_channel, primary_channel);
    if (dup_status != ARES_SUCCESS) {
        // If duplication fails, clean up and exit
        ares_destroy(primary_channel);
        ares_library_cleanup();
        return 0;
    }

    // Step 4: Reinitialize one of the channels using ares_reinit
    // Choose which channel to reinitialize based on fuzzer input
    bool reinit_primary = fdp.ConsumeBool();
    ares_channel_t* channel_to_reinit = reinit_primary ? primary_channel : duplicated_channel;
    
    ares_status_t reinit_status = ares_reinit(channel_to_reinit);
    // Note: ares_reinit returns ares_status_t, not int
    // We continue regardless of reinit success to test other APIs

    // Step 5: Perform DNS resolution on both channels
    // Generate IP addresses and hostnames from fuzzer input
    
    // For ares_gethostbyaddr: need IP address
    // Generate IPv4 or IPv6 address based on fuzzer input
    bool use_ipv4 = fdp.ConsumeBool();
    std::vector<uint8_t> ip_address;
    
    if (use_ipv4 && fdp.remaining_bytes() >= 4) {
        // IPv4 address (4 bytes)
        ip_address = fdp.ConsumeBytes<uint8_t>(4);
    } else if (fdp.remaining_bytes() >= 16) {
        // IPv6 address (16 bytes)
        ip_address = fdp.ConsumeBytes<uint8_t>(16);
    } else {
        // Not enough data for IP address, skip this part
        ip_address = std::vector<uint8_t>();
    }
    
    // For ares_getnameinfo: need socket address
    // Create sockaddr_in or sockaddr_in6 structure
    struct sockaddr_in sa4;
    struct sockaddr_in6 sa6;
    struct sockaddr* sa = NULL;
    ares_socklen_t salen = 0;
    
    if (use_ipv4 && ip_address.size() == 4) {
        memset(&sa4, 0, sizeof(sa4));
        sa4.sin_family = AF_INET;
        memcpy(&sa4.sin_addr, ip_address.data(), 4);
        sa4.sin_port = fdp.ConsumeIntegral<uint16_t>();
        sa = (struct sockaddr*)&sa4;
        salen = sizeof(sa4);
    } else if (ip_address.size() == 16) {
        memset(&sa6, 0, sizeof(sa6));
        sa6.sin6_family = AF_INET6;
        memcpy(&sa6.sin6_addr, ip_address.data(), 16);
        sa6.sin6_port = fdp.ConsumeIntegral<uint16_t>();
        sa = (struct sockaddr*)&sa6;
        salen = sizeof(sa6);
    }
    
    // Generate hostname for reverse lookups
    std::string hostname;
    if (fdp.remaining_bytes() > 5) {
        size_t hostname_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
        hostname = fdp.ConsumeRandomLengthString(hostname_len);
        if (hostname.empty()) {
            hostname = "localhost";
        }
    } else {
        hostname = "localhost";
    }
    
    // Consume flags for getnameinfo - use c-ares specific constants
    int nameinfo_flags = 0;
    if (fdp.ConsumeBool()) nameinfo_flags |= ARES_NI_NOFQDN;
    if (fdp.ConsumeBool()) nameinfo_flags |= ARES_NI_NUMERICHOST;
    if (fdp.ConsumeBool()) nameinfo_flags |= ARES_NI_NAMEREQD;
    if (fdp.ConsumeBool()) nameinfo_flags |= ARES_NI_NUMERICSERV;
    if (fdp.ConsumeBool()) nameinfo_flags |= ARES_NI_DGRAM;
    // Perform DNS resolution on primary channel
    if (!ip_address.empty()) {
        // Test ares_gethostbyaddr on primary channel
        ares_gethostbyaddr(primary_channel, ip_address.data(), ip_address.size(),
                          use_ipv4 ? AF_INET : AF_INET6, gethostbyaddr_callback, NULL);
        
        // Test ares_getnameinfo if we have a valid socket address
        if (sa != NULL && salen > 0) {
            ares_getnameinfo(primary_channel, sa, salen, nameinfo_flags,
                            getnameinfo_callback, NULL);
        }
    }
    
    // Perform DNS resolution on duplicated channel
    if (!ip_address.empty()) {
        // Test ares_gethostbyaddr on duplicated channel
        ares_gethostbyaddr(duplicated_channel, ip_address.data(), ip_address.size(),
                          use_ipv4 ? AF_INET : AF_INET6, gethostbyaddr_callback, NULL);
        
        // Test ares_getnameinfo if we have a valid socket address
        if (sa != NULL && salen > 0) {
            ares_getnameinfo(duplicated_channel, sa, salen, nameinfo_flags,
                            getnameinfo_callback, NULL);
        }
    }
    
    // Process any pending events (timeouts, etc.)
    // This helps exercise the internal state machines
    fd_set read_fds, write_fds;
    struct timeval tv;
    
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    // Get sockets from both channels
    ares_socket_t primary_socks[ARES_GETSOCK_MAXNUM];
    ares_socket_t dup_socks[ARES_GETSOCK_MAXNUM];
    
    int primary_numsocks = ares_getsock(primary_channel, primary_socks, ARES_GETSOCK_MAXNUM);
    int dup_numsocks = ares_getsock(duplicated_channel, dup_socks, ARES_GETSOCK_MAXNUM);
    
    // Set up file descriptor sets based on random fuzzer input
    for (int i = 0; i < primary_numsocks; i++) {
        if (fdp.ConsumeBool()) {
            FD_SET(primary_socks[i], &read_fds);
        }
        if (fdp.ConsumeBool()) {
            FD_SET(primary_socks[i], &write_fds);
        }
    }
    
    for (int i = 0; i < dup_numsocks; i++) {
        if (fdp.ConsumeBool()) {
            FD_SET(dup_socks[i], &read_fds);
        }
        if (fdp.ConsumeBool()) {
            FD_SET(dup_socks[i], &write_fds);
        }
    }
    // Process with timeout (use minimal timeout for fuzzing)
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    // Process primary channel
    ares_process(primary_channel, &read_fds, &write_fds);
    
    // Process duplicated channel
    ares_process(duplicated_channel, &read_fds, &write_fds);

    // Step 6: Cleanup - destroy both channels
    ares_destroy(primary_channel);
    ares_destroy(duplicated_channel);

    // Step 7: Global library cleanup
    ares_library_cleanup();

    return 0;
}

} // namespace harness_002

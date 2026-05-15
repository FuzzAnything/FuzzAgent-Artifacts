#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"

namespace harness_001 {
/*
 * Fuzzing harness for c-ares DNS library - Hostname resolution and network APIs
 * Targets: ares_getaddrinfo, ares_gethostbyname, ares_gethostbyaddr, ares_getnameinfo,
 *          ares_set_servers_csv, and related network configuration APIs
 */





// Callback functions for async operations (do nothing in fuzzing context)
static void host_callback(void *arg, int status, int timeouts, const struct hostent *hostent) {
    // Do nothing - just to satisfy API requirements
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
}

static void addrinfo_callback(void *arg, int status, int timeouts, struct ares_addrinfo *res) {
    // Do nothing - just to satisfy API requirements
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)res;
}

static void nameinfo_callback(void *arg, int status, int timeouts, const char *node, const char *service) {
    // Do nothing - just to satisfy API requirements
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)node;
    (void)service;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);

    // Create a channel
    ares_channel_t *channel = NULL;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    options.timeout = 1000;
    
    int optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS;
    ares_init_options(&channel, &options, optmask);

    if (channel == NULL) {
        ares_library_cleanup();
        return 0;
    }

    // Test ares_set_servers_csv with random CSV data
    if (fdp.remaining_bytes() > 10) {
        size_t csv_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
        std::string csv_data = fdp.ConsumeBytesAsString(csv_len);
        
        if (!csv_data.empty()) {
            // Null terminate the CSV data
            char *csv_copy = strdup(csv_data.c_str());
            if (csv_copy) {
                ares_set_servers_csv(channel, csv_copy);
                free(csv_copy);
            }
        }
    }

    // Test hostname resolution APIs
    if (fdp.remaining_bytes() > 10) {
        size_t hostname_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        std::string hostname = fdp.ConsumeBytesAsString(hostname_len);
        
        if (!hostname.empty()) {
            // Test ares_gethostbyname (deprecated but still present)
            char *hostname_copy = strdup(hostname.c_str());
            if (hostname_copy) {
                // Async call with proper callback
                ares_gethostbyname(channel, hostname_copy, AF_INET, host_callback, NULL);
                free(hostname_copy);
            }
            
            // Test ares_getaddrinfo (modern API)
            struct ares_addrinfo_hints hints;
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = fdp.ConsumeBool() ? AF_INET : AF_INET6;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;
            hints.ai_flags = 0;
            
            char *service = NULL;
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
                size_t service_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                std::string service_str = fdp.ConsumeBytesAsString(service_len);
                service = strdup(service_str.c_str());
            }
            
            char *hostname_copy2 = strdup(hostname.c_str());
            if (hostname_copy2) {
                ares_getaddrinfo(channel, hostname_copy2, service, &hints, addrinfo_callback, NULL);
                free(hostname_copy2);
            }
            
            if (service) {
                free(service);
            }
        }
    }

    // Test address-based lookups
    if (fdp.remaining_bytes() > 20) {
        // Generate random IP address data
        int addr_family = fdp.ConsumeBool() ? AF_INET : AF_INET6;
        
        if (addr_family == AF_INET) {
            struct in_addr addr;
            uint32_t ip = fdp.ConsumeIntegral<uint32_t>();
            addr.s_addr = ip;
            
            // Test ares_gethostbyaddr with IPv4
            ares_gethostbyaddr(channel, &addr, sizeof(addr), addr_family, host_callback, NULL);
            
            // Test ares_getnameinfo with IPv4
            struct sockaddr_in sa;
            memset(&sa, 0, sizeof(sa));
            sa.sin_family = AF_INET;
            sa.sin_addr = addr;
            sa.sin_port = htons(80);
            
            ares_getnameinfo(channel, (struct sockaddr*)&sa, sizeof(sa), 
                            NI_NUMERICHOST, nameinfo_callback, NULL);
        } else {
            struct in6_addr addr6;
            std::vector<uint8_t> ip6_bytes = fdp.ConsumeBytes<uint8_t>(16);
            if (ip6_bytes.size() == 16) {
                memcpy(&addr6, ip6_bytes.data(), 16);
                
                // Test ares_gethostbyaddr with IPv6
                ares_gethostbyaddr(channel, &addr6, sizeof(addr6), addr_family, host_callback, NULL);
                
                // Test ares_getnameinfo with IPv6
                struct sockaddr_in6 sa6;
                memset(&sa6, 0, sizeof(sa6));
                sa6.sin6_family = AF_INET6;
                memcpy(&sa6.sin6_addr, &addr6, sizeof(addr6));
                sa6.sin6_port = htons(80);
                
                ares_getnameinfo(channel, (struct sockaddr*)&sa6, sizeof(sa6),
                                NI_NUMERICHOST, nameinfo_callback, NULL);
            }
        }
    }

    // Test ares_gethostbyname_file (reads from /etc/hosts)
    if (fdp.remaining_bytes() > 10) {
        size_t file_hostname_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::string file_hostname = fdp.ConsumeBytesAsString(file_hostname_len);
        
        if (!file_hostname.empty()) {
            struct hostent *host = NULL;
            char *file_hostname_copy = strdup(file_hostname.c_str());
            if (file_hostname_copy) {
                // This reads from /etc/hosts - should handle gracefully
                ares_gethostbyname_file(channel, file_hostname_copy, AF_INET, &host);
                if (host) {
                    ares_free_hostent(host);
                }
                free(file_hostname_copy);
            }
        }
    }

    // Test ares_inet_ntop and ares_inet_pton with random data
    if (fdp.remaining_bytes() > 20) {
        int family = fdp.ConsumeBool() ? AF_INET : AF_INET6;
        
        if (family == AF_INET) {
            // Test IPv4 conversion
            uint32_t ip = fdp.ConsumeIntegral<uint32_t>();
            struct in_addr addr;
            addr.s_addr = ip;
            
            char buf[INET_ADDRSTRLEN];
            const char *result = ares_inet_ntop(AF_INET, &addr, buf, sizeof(buf));
            (void)result; // Ignore result
            
            // Try parsing a random string as IPv4
            size_t ip_str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
            std::string ip_str = fdp.ConsumeBytesAsString(ip_str_len);
            if (!ip_str.empty()) {
                struct in_addr parsed_addr;
                ares_inet_pton(AF_INET, ip_str.c_str(), &parsed_addr);
            }
        } else {
            // Test IPv6 conversion
            struct in6_addr addr6;
            std::vector<uint8_t> ip6_bytes = fdp.ConsumeBytes<uint8_t>(16);
            if (ip6_bytes.size() == 16) {
                memcpy(&addr6, ip6_bytes.data(), 16);
                
                char buf[INET6_ADDRSTRLEN];
                const char *result = ares_inet_ntop(AF_INET6, &addr6, buf, sizeof(buf));
                (void)result; // Ignore result
                
                // Try parsing a random string as IPv6
                size_t ip6_str_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                std::string ip6_str = fdp.ConsumeBytesAsString(ip6_str_len);
                if (!ip6_str.empty()) {
                    struct in6_addr parsed_addr6;
                    ares_inet_pton(AF_INET6, ip6_str.c_str(), &parsed_addr6);
                }
            }
        }
    }

    // Test channel manipulation functions
    if (fdp.remaining_bytes() > 4) {
        // Test ares_set_local_ip4 and ares_set_local_ip6
        uint32_t local_ip = fdp.ConsumeIntegral<uint32_t>();
        ares_set_local_ip4(channel, local_ip);
        
        if (fdp.remaining_bytes() >= 16) {
            unsigned char local_ip6[16];
            std::vector<uint8_t> ip6_bytes = fdp.ConsumeBytes<uint8_t>(16);
            memcpy(local_ip6, ip6_bytes.data(), 16);
            ares_set_local_ip6(channel, local_ip6);
        }
        
        // Test ares_set_local_dev with random interface name
        if (fdp.remaining_bytes() > 5) {
            size_t dev_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
            std::string dev_name = fdp.ConsumeBytesAsString(dev_len);
            if (!dev_name.empty()) {
                ares_set_local_dev(channel, dev_name.c_str());
            }
        }
    }
    // Process any pending operations (non-blocking) using deprecated but simpler APIs
    if (fdp.ConsumeBool()) {
        fd_set read_fds;
        fd_set write_fds;
        
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        // Get file descriptors
        int nfds = ares_fds(channel, &read_fds, &write_fds);
        
        if (nfds > 0) {
            // Process with zero timeout (non-blocking)
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 0;
            
            // Use deprecated ares_process which is simpler for fuzzing
            ares_process(channel, &read_fds, &write_fds);
        }
    }

    // Cleanup
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_001

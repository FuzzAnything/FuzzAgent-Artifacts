#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <ares.h>
}
extern "C" {
#include <ares_dns.h>
}
extern "C" {
#include <ares_nameser.h>
}
//<ID> 540
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_query_enqueue_cb","ares_set_local_ip6","ares_destroy","ares_destroy_options","ares_library_cleanup","ares_getaddrinfo","ares_queue_active_queries","ares_fds","ares_process","ares_inet_ntop","ares_inet_ntop","ares_destroy","ares_destroy_options","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_query_enqueue_cb","ares_set_local_ip6","ares_getaddrinfo","ares_queue_active_queries","ares_fds","ares_process","ares_inet_ntop","ares_inet_ntop","ares_destroy","ares_destroy_options","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

// Callback for getaddrinfo
static void  id_000020_getaddrinfo_callback(void* arg, int status, int timeouts, struct ares_addrinfo* result) {
    (void)arg;
    (void)status;
    (void)timeouts;
    if (result) {
        ares_freeaddrinfo(result);
    }
}

// Callback for query enqueue
static void  id_000020_query_enqueue_callback(void* user_data) {
    (void)user_data;
    // Empty callback for demonstration
}

// Main fuzzer function
extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    ares_channel_t* channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize options with some default values
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    options.timeout = 5000;
    options.tries = 4;
    optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    // Initialize channel with options
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Set query enqueue callback
    ares_set_query_enqueue_cb(channel,  id_000020_query_enqueue_callback, nullptr);
    
    // Set local IPv6 address (using loopback ::1)
    static const unsigned char local_ip6[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
    ares_set_local_ip6(channel, local_ip6);
    
    // Create FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_destroy_options(&options);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read hostname from input data (first part until null byte or size limit)
    char hostname[256];
    size_t hostname_len = 0;
    if (size > 0) {
        // Find null terminator or use max 255 bytes
        size_t max_len = (size < sizeof(hostname) - 1) ? size : sizeof(hostname) - 1;
        for (hostname_len = 0; hostname_len < max_len; hostname_len++) {
            int c = fgetc(in_file);
            if (c == EOF || c == '\0') {
                break;
            }
            hostname[hostname_len] = (char)c;
        }
        hostname[hostname_len] = '\0';
    } else {
        hostname[0] = '\0';
    }
    
    // If we have data after null terminator, read service name
    char service[64];
    size_t service_len = 0;
    if (ftell(in_file) < (long)size) {
        // Read service from remaining data
        size_t max_len = (size - ftell(in_file) < sizeof(service) - 1) ? 
                         size - ftell(in_file) : sizeof(service) - 1;
        for (service_len = 0; service_len < max_len; service_len++) {
            int c = fgetc(in_file);
            if (c == EOF || c == '\0') {
                break;
            }
            service[service_len] = (char)c;
        }
        service[service_len] = '\0';
    } else {
        strcpy(service, "80");  // Default to port 80
    }
    
    assert_file_closed(&in_file);;
    
    // Call getaddrinfo with the parsed hostname and service
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    ares_getaddrinfo(channel, hostname, service, &hints,  id_000020_getaddrinfo_callback, nullptr);
    
    // Process DNS queries
    fd_set read_fds, write_fds;
    struct timeval tv;
    
    // Set timeout
    tv.tv_sec = 0;
    tv.tv_usec = 10000;  // 10ms timeout for fuzzing
    
    int max_fd = 0;
    int nfds = 0;
    
    // Process until no active queries
    while (ares_queue_active_queries(channel) > 0) {
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        // Get file descriptors for active queries
        max_fd = ares_fds(channel, &read_fds, &write_fds);
        
        if (max_fd == 0) {
            break;
        }
        
        // Wait for activity (with timeout)
        nfds = select(max_fd + 1, &read_fds, &write_fds, nullptr, &tv);
        if (nfds < 0) {
            break;
        }
        
        // Process active file descriptors
        ares_process(channel, &read_fds, &write_fds);
    }
    
    // Test ares_inet_ntop with IPv4 address from remaining data
    if (size >= 4) {
        struct in_addr addr;
        memcpy(&addr, data, sizeof(addr));
        char ip_str[INET_ADDRSTRLEN];
        const char* result = ares_inet_ntop(AF_INET, &addr, ip_str, sizeof(ip_str));
        (void)result;  // Ignore result for fuzzing
    }
    
    // Test ares_inet_ntop with IPv6 address from remaining data
    if (size >= 16) {
        struct in6_addr addr6;
        memcpy(&addr6, data, sizeof(addr6));
        char ip6_str[INET6_ADDRSTRLEN];
        const char* result = ares_inet_ntop(fuzz_int32_t_2, &addr6, ip6_str, sizeof(ip6_str));
        (void)result;  // Ignore result for fuzzing
    }
    
    // Clean up
    ares_destroy(channel);
    ares_destroy_options(&options);
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
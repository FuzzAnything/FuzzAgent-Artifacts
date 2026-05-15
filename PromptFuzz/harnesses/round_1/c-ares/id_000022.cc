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
//<ID> 542
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"ares_init_by_options":[[298,9,298,30,0,0,4,0]]},"library_calls":["ares_library_init_mem","ares_init_options","ares_init","ares_library_cleanup","ares_set_local_ip6","ares_set_query_enqueue_cb","ares_getaddrinfo","ares_queue_active_queries","ares_fds","ares_inet_ntop","ares_inet_ntop","ares_destroy_options","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init_mem","ares_init_options","ares_init","ares_set_local_ip6","ares_set_query_enqueue_cb","ares_getaddrinfo","ares_queue_active_queries","ares_fds","ares_inet_ntop","ares_inet_ntop","ares_destroy_options","ares_destroy","ares_library_cleanup"],"visited":0}
/**/



// Define the ares_query_enqueue_cb type since it's in custom types but not in API list
typedef void (*ares_query_enqueue_cb)(void *);

// Global counter for query enqueue callback
static int  id_000022_query_enqueue_count = 0;

// Callback for query enqueue
static void  id_000022_query_enqueue_callback(void *user_data) {
     id_000022_query_enqueue_count++;
}

// Callback for getaddrinfo
static void  id_000022_getaddrinfo_callback(void *arg, int status, int timeouts, 
                                 struct ares_addrinfo *result) {
    // Store status for verification
    int *status_ptr = static_cast<int*>(arg);
    if (status_ptr) {
        *status_ptr = status;
    }
    
    if (result) {
        ares_freeaddrinfo(result);
    }
}

// Helper function to extract string from fuzzer input
static const char*  id_000022_get_string_from_data(const uint8_t *data, size_t size, 
                                        size_t *offset, size_t max_len) {
    if (*offset >= size) return nullptr;
    
    size_t remaining = size - *offset;
    size_t str_len = (remaining > max_len) ? max_len : remaining;
    
    // Find null terminator or use available bytes
    const char *str = reinterpret_cast<const char*>(data + *offset);
    
    // Update offset for next extraction
    *offset += str_len;
    
    return str;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Reset global counter
     id_000022_query_enqueue_count = 0;
    
    // Initialize c-ares library
    if (ares_library_init_mem(ARES_LIB_INIT_ALL, malloc, free, realloc) != ARES_SUCCESS) {
        return 0;
    }
    
    // Initialize channel with options
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    
    // Set some options from fuzzer input
    if (size >= sizeof(int)) {
        options.timeout = *reinterpret_cast<const int*>(data) % 5000;
        optmask |= ARES_OPT_TIMEOUTMS;
        
        options.tries = (*reinterpret_cast<const uint8_t*>(data) % 5) + 1;
        optmask |= ARES_OPT_TRIES;
    }
    
    // Create channel with options
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        // Try without options
        if (ares_init(&channel) != ARES_SUCCESS) {
            ares_library_cleanup();
            return 0;
        }
    }
    
    // Set local IPv6 address from fuzzer data
    if (size >= 16) {
        unsigned char local_ip6[16];
        memcpy(local_ip6, data, 16);
        ares_set_local_ip6(channel, local_ip6);
    }
    
    // Set query enqueue callback
    int user_data = 42; // Arbitrary user data
    ares_set_query_enqueue_cb(channel,  id_000022_query_enqueue_callback, &user_data);
    
    // Parse node and service from fuzzer input
    size_t offset = 0;
    const char *node = nullptr;
    const char *service = nullptr;
    
    if (size > 0) {
        // Extract node (hostname) - use first part of data
        size_t node_len = (size > 100) ? 100 : size;
        char *node_buf = static_cast<char*>(malloc(node_len + 1));
        if (node_buf) {
            memcpy(node_buf, data, node_len);
            node_buf[node_len] = '\0';
            
            // Clean up any non-printable characters for hostname safety
            for (size_t i = 0; i < node_len; i++) {
                if (node_buf[i] < 32 || node_buf[i] > 126) {
                    node_buf[i] = 'a' + (node_buf[i] % 26);
                }
            }
            
            node = node_buf;
            offset = node_len;
        }
        
        // Extract service (port/service name) - use next part if available
        if (size > offset && (size - offset) > 0) {
            size_t service_len = ((size - offset) > 50) ? 50 : (size - offset);
            char *service_buf = static_cast<char*>(malloc(service_len + 1));
            if (service_buf) {
                memcpy(service_buf, data + offset, service_len);
                service_buf[service_len] = '\0';
                
                // Clean up for service name
                for (size_t i = 0; i < service_len; i++) {
                    if (service_buf[i] < 32 || service_buf[i] > 126) {
                        service_buf[i] = '0' + (service_buf[i] % 10);
                    }
                }
                
                service = service_buf;
            }
        }
    }
    
    // Use default values if extraction failed
    if (!node) {
        node = "localhost";
    }
    if (!service) {
        service = "http";
    }
    
    // Set up hints for getaddrinfo
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = ARES_AI_CANONNAME;
    
    // Variable to store callback status
    int callback_status = -1;
    
    // Perform getaddrinfo lookup
    ares_getaddrinfo(channel, node, service, &hints,  id_000022_getaddrinfo_callback, &callback_status);
    
    // Check active queries count
    size_t active_queries = ares_queue_active_queries(channel);
    
    // Get file descriptors for the channel
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    
    // Process any pending events (non-blocking)
    if (nfds > 0) {
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        
        // Note: Normally we'd use select() here, but for fuzzing we just check
        // that ares_fds() gave us some FDs to work with
    }
    
    // Test ares_inet_ntop with IPv6 address from fuzzer data
    if (size >= 32) {
        char ip6_str[INET6_ADDRSTRLEN];
        const struct in6_addr *ip6_addr = reinterpret_cast<const struct in6_addr*>(data + 16);
        
        // Use ares_inet_ntop to convert IPv6 address to string
        const char *result = ares_inet_ntop(fuzz_int32_t_2, ip6_addr, ip6_str, sizeof(ip6_str));
        
        // Test with IPv4 address if enough data
        if (size >= 48) {
            char ip4_str[INET_ADDRSTRLEN];
            const struct in_addr *ip4_addr = reinterpret_cast<const struct in_addr*>(data + 32);
            result = ares_inet_ntop(fuzz_int32_t_3, ip4_addr, ip4_str, sizeof(ip4_str));
        }
    }
    
    // Clean up extracted strings
    if (node && node != "localhost") {
        free(const_cast<char*>(node));
    }
    if (service && service != "http") {
        free(const_cast<char*>(service));
    }
    
    // Destroy options structure
    ares_destroy_options(&options);
    
    // Clean up channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();
    
    return 0;
}
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
//<ID> 541
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["ares_init","ares_set_local_ip6","ares_set_query_enqueue_cb","ares_getaddrinfo","ares_fds","ares_queue_active_queries","ares_inet_ntop","ares_process","ares_destroy_options","ares_destroy"],"critical_calls":["ares_init","ares_set_local_ip6","ares_set_query_enqueue_cb","ares_getaddrinfo","ares_fds","ares_queue_active_queries","ares_inet_ntop","ares_process","ares_destroy_options","ares_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for ares_getaddrinfo
static void  id_000021_getaddrinfo_callback(void* arg, int status, int timeouts,
                                 struct ares_addrinfo* result) {
    (void)status;
    (void)timeouts;
    if (result) {
        ares_freeaddrinfo(result);
    }
}

// Callback for ares_set_query_enqueue_cb
static void  id_000021_query_enqueue_callback(void* user_data) {
    (void)user_data;
    // This callback can be empty for fuzzing purposes
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // Use data to create a FILE* for reading
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file for writing
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create channel
    ares_channel_t* channel = nullptr;
    
    // Initialize with default options
    if (ares_init(&channel) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create options structure and initialize
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    
    // Set some options
    options.flags = ARES_FLAG_STAYOPEN;
    options.timeout = 2000;
    options.tries = 3;
    
    // Set local IPv6 address - use first 16 bytes of input if available
    unsigned char local_ip6[16];
    if (size >= 16) {
        memcpy(local_ip6, data, 16);
        ares_set_local_ip6(channel, local_ip6);
    }

    // Set query enqueue callback
    ares_set_query_enqueue_cb(channel,  id_000021_query_enqueue_callback, nullptr);

    // Prepare node and service strings from input data
    char node[256] = {0};
    char service[256] = {0};
    
    size_t read_pos = 0;
    
    // Read node from input (null-terminated string)
    if (read_pos < size) {
        size_t node_len = (size - read_pos) < 255 ? (size - read_pos) : 255;
        memcpy(node, data + read_pos, node_len);
        node[node_len] = '\0';
        read_pos += node_len;
    }
    
    // Read service from remaining input
    if (read_pos < size) {
        size_t service_len = (size - read_pos) < 255 ? (size - read_pos) : 255;
        memcpy(service, data + read_pos, service_len);
        service[service_len] = '\0';
        read_pos += service_len;
    }

    // Prepare hints for getaddrinfo
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = ARES_AI_CANONNAME;

    // Start asynchronous getaddrinfo request
    ares_getaddrinfo(channel, node, service, &hints,  id_000021_getaddrinfo_callback, nullptr);

    // Get file descriptors for the channel
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    
    // Check active queries
    size_t active_queries = ares_queue_active_queries(channel);

    // Convert IPv6 address to string using ares_inet_ntop
    if (size >= 16) {
        char ip6_str[INET6_ADDRSTRLEN];
        ares_inet_ntop(fuzz_int32_t_1, local_ip6, ip6_str, sizeof(ip6_str));
        
        // Write the converted string to output file
        fwrite(ip6_str, 1, strlen(ip6_str), out_file);
    }

    // Process any pending I/O on the channel
    ares_process(channel, &read_fds, &write_fds);

    // Use file descriptors from our files
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Add our file descriptors to the sets (just for demonstration)
    if (in_fd >= 0) {
        FD_SET(in_fd, &read_fds);
    }
    if (out_fd >= 0) {
        FD_SET(out_fd, &write_fds);
    }

    // Read some data from input file
    uint8_t buffer[1024];
    if (in_fd >= 0) {
        // Move file pointer to beginning
        fseek(in_file, 0, SEEK_SET);
        fread(buffer, 1, size < 1024 ? size : 1024, in_file);
    }

    // Destroy options (even though we didn't use them with init_options)
    ares_destroy_options(&options);

    // Clean up channel
    ares_destroy(channel);

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
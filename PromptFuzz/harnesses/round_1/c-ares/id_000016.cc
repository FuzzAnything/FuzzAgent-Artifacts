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
//<ID> 398
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_local_ip4","ares_dup","ares_destroy","ares_create_query","ares_send","ares_queue_active_queries","ares_gethostbyaddr","ares_dns_opt_get_name","ares_fds","ares_process","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_local_ip4","ares_dup","ares_destroy","ares_create_query","ares_send","ares_queue_active_queries","ares_gethostbyaddr","ares_dns_opt_get_name","ares_fds","ares_process","ares_destroy","ares_library_cleanup"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	 // Minimal size for operations

    // Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);

    // Step 1: Create a channel
    ares_channel_t *channel = nullptr;
    ares_channel_t *dup_channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Step 2: Set local IPv4 address using input bytes (first 4 bytes)
    unsigned int local_ip = *(reinterpret_cast<const unsigned int*>(data));
    ares_set_local_ip4(channel, local_ip);

    // Step 3: Use ares_dup to duplicate channel
    if (ares_dup(&dup_channel, channel) == ARES_SUCCESS) {
        // Use duplicated channel
        ares_destroy(dup_channel);
    }

    // Step 4: Create a DNS query and send it via ares_send
    unsigned char *query_buf = nullptr;
    int query_len = 0;
    const char *name = "fuzz.example.com"; // Fixed name for query
    
    // Create a simple query - use data to influence query parameters
    unsigned short query_id = static_cast<unsigned short>(data[4] | (data[5] << 8));
    
    // Use ares_create_query to build a DNS query
    int result = ares_create_query(name, C_IN, 
                                  static_cast<int>(data[6] % 16), // Vary query type
                                  query_id, 
                                  1, // Recursion desired
                                  &query_buf, 
                                  &query_len, 
                                  fuzz_int32_t_2); // Standard UDP size
    
    if (result == ARES_SUCCESS && query_len > 0) {
        // Define callback for ares_send
        auto send_callback = [](void* arg, int status, int timeouts, 
                               const unsigned char* abuf, int alen) {
            // Empty callback - just consume the response
        };
        
        // Send the query
        ares_send(channel, query_buf, fuzz_int32_t_3, send_callback, nullptr);
        free(query_buf);
    }

    // Step 5: Check active queries count
    size_t active_queries = ares_queue_active_queries(channel);

    // Step 6: Perform reverse DNS lookup (gethostbyaddr) using input data
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    
    // Use bytes 4-7 for the IPv4 address (little-endian)
    uint32_t ip_addr = *(reinterpret_cast<const uint32_t*>(data + 4));
    addr.sin_addr.s_addr = ip_addr;
    
    // Define host callback
    auto host_callback = [](void* arg, int status, int timeouts, 
                           const struct hostent* hostent) {
        // Empty callback
    };
    
    // Perform reverse lookup
    ares_gethostbyaddr(channel, &addr.sin_addr, sizeof(addr.sin_addr), 
                      fuzz_int32_t_4, host_callback, nullptr);

    // Step 7: Process DNS option name lookup
    // Use bytes 8-11 for key and opt (if available)
    if (size >= 12) {
        ares_dns_rr_key_t key = static_cast<ares_dns_rr_key_t>(
            data[8] | (data[9] << 8));
        unsigned short opt = static_cast<unsigned short>(
            data[10] | (data[11] << 8));
        
        // Get DNS option name (if valid)
        const char* opt_name = ares_dns_opt_get_name(key, opt);
        // opt_name may be nullptr for invalid inputs
    }

    // Step 8: Process any pending events
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    if (nfds > 0) {
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        
        ares_process(channel, &read_fds, &write_fds);
    }

    // Step 9: Cleanup
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}
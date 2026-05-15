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
//<ID> 181
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["ares_library_init","ares_library_cleanup","ares_init","ares_library_cleanup","ares_save_options","ares_dup","ares_set_pending_write_cb","ares_process_pending_write","ares_inet_pton","ares_inet_pton","ares_dns_parse","ares_dns_record_rr_get_const","ares_dns_rr_get_opt_byid","ares_dns_record_destroy","ares_getaddrinfo","ares_freeaddrinfo","ares_freeaddrinfo","ares_process","ares_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_save_options","ares_dup","ares_set_pending_write_cb","ares_process_pending_write","ares_inet_pton","ares_inet_pton","ares_dns_parse","ares_dns_record_rr_get_const","ares_dns_rr_get_opt_byid","ares_dns_record_destroy","ares_getaddrinfo","ares_freeaddrinfo","ares_freeaddrinfo","ares_process","ares_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    ares_channel_t *channel = nullptr;
    ares_channel_t *dup_channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Create a FILE* from input data for potential parsing
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize first channel
    if (ares_init(&channel) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Use ares_save_options
    if (ares_save_options(channel, &options, &optmask) == ARES_SUCCESS) {
        // Options saved successfully
    }
    
    // 2. Use ares_dup to duplicate the channel
    if (ares_dup(&dup_channel, channel) == ARES_SUCCESS) {
        // 4. Use ares_set_pending_write_cb
        ares_set_pending_write_cb(dup_channel, [](void *user_data) {
            // Callback for pending writes
        }, nullptr);
        
        // 3. Use ares_process_pending_write
        ares_process_pending_write(dup_channel);
    }
    
    // 6. Use ares_inet_pton
    // Extract a potential IP address string from input
    char ip_str[INET6_ADDRSTRLEN] = {0};
    size_t ip_len = size < INET6_ADDRSTRLEN - 1 ? size : INET6_ADDRSTRLEN - 1;
    memcpy(ip_str, data, ip_len);
    ip_str[ip_len] = '\0';
    
    struct in_addr addr4;
    struct in6_addr addr6;
    
    // Try IPv4
    if (ares_inet_pton(fuzz_int32_t_1, ip_str, &addr4) == 1) {
        // Successfully parsed as IPv4
    }
    // Try IPv6
    if (ares_inet_pton(fuzz_int32_t_2, ip_str, &addr6) == 1) {
        // Successfully parsed as IPv6
    }
    
    // 5. Use ares_dns_rr_get_opt_byid
    // Try to parse input as DNS response
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_3, &dnsrec);
    
    if (parse_status == ARES_SUCCESS && dnsrec) {
        // Get the first resource record from answer section
        const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_4);
        if (rr) {
            // Try to get OPT record data
            const unsigned char *opt_val = nullptr;
            size_t opt_len = 0;
            
            // Check for EDNS options (common OPT record types)
            ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS, fuzz_uint16_t_5 /* EDNS Client Subnet */, 
                                    &opt_val, &opt_len);
        }
        
        ares_dns_record_destroy(dnsrec);
    }
    
    // 7. Use ares_getaddrinfo
    // Extract a potential hostname from input
    char hostname[256] = {0};
    size_t hostname_len = size < 255 ? size : 255;
    memcpy(hostname, data, hostname_len);
    hostname[hostname_len] = '\0';
    
    // Ensure null termination for safety
    for (size_t i = 0; i < hostname_len; i++) {
        if (hostname[i] == '\0') break;
        if (hostname[i] < 32 || hostname[i] > 126) {
            hostname[i] = 'a'; // Make it valid ASCII
        }
    }
    
    if (dup_channel) {
        struct ares_addrinfo_hints hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        
        ares_getaddrinfo(dup_channel, hostname, fuzz_str_6, &hints, 
                        [](void *arg, int status, int timeouts, struct ares_addrinfo *result) {
                            // Callback for getaddrinfo
                            if (result) {
                                ares_freeaddrinfo(result);
                            }
                        }, nullptr);
        
        // Process any pending operations
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        
        ares_process(dup_channel, &read_fds, &write_fds);
    }
    
    // Cleanup
    assert_file_closed(&in_file);;
    
    if (dup_channel) {
        ares_destroy(dup_channel);
    }
    
    if (channel) {
        ares_destroy(channel);
    }
    
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
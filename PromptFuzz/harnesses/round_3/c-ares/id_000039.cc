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
//<ID> 781
//<Prompt> []
/*<Combination>: [
*/
//<score> 64, nr_unique_branch: 3
//<Quality> {"density":16,"unique_branches":{"ares_array_set_size":[[176,22,176,31,0,0,4,0]],"ares_buf_fetch":[[505,22,505,33,0,0,4,0]],"ares_dns_rr_get_addr6":[[784,7,784,19,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_parse_naptr_reply","ares_free_data","ares_parse_srv_reply","ares_free_data","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_bin","ares_dns_rr_get_bin","ares_dns_rr_get_addr6","ares_search_dnsrec","ares_dns_record_destroy","ares_gethostbyname_file","ares_free_hostent","ares_fds","ares_process","ares_reinit","ares_getaddrinfo","ares_fds","ares_process","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_parse_naptr_reply","ares_free_data","ares_parse_srv_reply","ares_free_data","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_bin","ares_dns_rr_get_bin","ares_dns_rr_get_addr6","ares_search_dnsrec","ares_dns_record_destroy","ares_gethostbyname_file","ares_free_hostent","ares_fds","ares_process","ares_reinit","ares_getaddrinfo","ares_fds","ares_process","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<45) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create channel
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    options.timeout = 1000;
    options.tries = 1;
    
    if (ares_init_options(&channel, &options, ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Parse NAPTR reply from input data
    struct ares_naptr_reply *naptr_reply = nullptr;
    if (size > 0) {
        ares_parse_naptr_reply(data, static_cast<int>(size), &naptr_reply);
        if (naptr_reply) {
            ares_free_data(naptr_reply);
        }
    }

    // Parse SRV reply from input data
    struct ares_srv_reply *srv_reply = nullptr;
    if (size > 0) {
        ares_parse_srv_reply(data, static_cast<int>(size), &srv_reply);
        if (srv_reply) {
            ares_free_data(srv_reply);
        }
    }

    // Create DNS record
    ares_dns_record_t *dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_1, fuzz_uint16_t_2, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) == ARES_SUCCESS && dnsrec) {
        // Add resource record
        ares_dns_rr_t *rr = nullptr;
        ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                              fuzz_str_3, ARES_REC_TYPE_A, ARES_CLASS_IN, fuzz_uint32_t_4);
        
        if (rr && size > 0) {
            // Set binary data in RR
            size_t bin_len = size > 100 ? 100 : size;
            ares_dns_rr_set_bin(rr, ARES_RR_A_ADDR, data, bin_len);
            
            // Get binary data back
            size_t get_len = 0;
            const unsigned char *bin_data = ares_dns_rr_get_bin(rr, ARES_RR_A_ADDR, &get_len);
            
            // Get IPv6 address (if set)
            const struct ares_in6_addr *addr6 = ares_dns_rr_get_addr6(rr, ARES_RR_AAAA_ADDR);
        }
        
        // Perform search with DNS record
        ares_search_dnsrec(channel, dnsrec, 
                          [](void *arg, ares_status_t status, size_t timeouts, 
                             const ares_dns_record_t *dnsrec) {
                              // Dummy callback
                          }, nullptr);
        
        // Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // Create temporary file with input data for gethostbyname_file
    FILE *temp_file = fmemopen((void *)data, size, "rb");
    if (temp_file) {
        // Create a temporary hosts file
        FILE *hosts_file = fopen("hosts_file", "wb");
        if (hosts_file) {
            if (size > 0) {
                fwrite(data, 1, size, hosts_file);
            }
            assert_file_closed(&hosts_file);;
            
            // Try to get host by name from file
            struct hostent *host = nullptr;
            ares_gethostbyname_file(channel, fuzz_str_5, fuzz_int32_t_6, &host);
            if (host) {
                ares_free_hostent(host);
            }
            
            // Remove temporary file
            unlink("hosts_file");
        }
        assert_file_closed(&temp_file);;
    }

    // Setup file descriptors for processing
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    // Get file descriptors for channel
    ares_fds(channel, &read_fds, &write_fds);
    
    // Process any pending requests
    ares_process(channel, &read_fds, &write_fds);

    // Reinitialize channel
    ares_reinit(channel);

    // Setup getaddrinfo request
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    ares_getaddrinfo(channel, "example.com", fuzz_str_7, &hints,
                    [](void *arg, int status, int timeouts, 
                       struct ares_addrinfo *result) {
                        // Dummy callback
                    }, nullptr);

    // Process again
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    ares_fds(channel, &read_fds, &write_fds);
    ares_process(channel, &read_fds, &write_fds);

    // Cancel all pending requests
    ares_cancel(channel);

    // Cleanup
    ares_destroy(channel);
    ares_library_cleanup();

    assert_file_closed(&temp_file);
	return 0;
}
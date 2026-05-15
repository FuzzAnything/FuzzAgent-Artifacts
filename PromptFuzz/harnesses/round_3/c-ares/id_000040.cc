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
//<ID> 783
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"ares_realloc_zero":[[101,7,101,27,0,0,4,1]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_parse_naptr_reply","ares_free_data","ares_parse_srv_reply","ares_free_data","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_bin","ares_dns_rr_get_bin","ares_dns_rr_get_addr6","ares_dns_record_query_add","ares_gethostbyname_file","ares_free_hostent","ares_reinit","ares_search_dnsrec","ares_getaddrinfo","ares_fds","ares_process","ares_cancel","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_parse_naptr_reply","ares_free_data","ares_parse_srv_reply","ares_free_data","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_bin","ares_dns_rr_get_bin","ares_dns_rr_get_addr6","ares_dns_record_query_add","ares_gethostbyname_file","ares_free_hostent","ares_reinit","ares_search_dnsrec","ares_getaddrinfo","ares_fds","ares_process","ares_cancel","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<69) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    options.timeout = 2000;
    options.tries = 3;
    
    int optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Parse NAPTR reply from input data
    struct ares_naptr_reply *naptr_reply = nullptr;
    if (size > 0) {
        ares_parse_naptr_reply(data, (int)(size > INT_MAX ? INT_MAX : size), &naptr_reply);
        if (naptr_reply) {
            ares_free_data(naptr_reply);
        }
    }

    // Parse SRV reply from input data
    struct ares_srv_reply *srv_reply = nullptr;
    if (size > 0) {
        ares_parse_srv_reply(data, (int)(size > INT_MAX ? INT_MAX : size), &srv_reply);
        if (srv_reply) {
            ares_free_data(srv_reply);
        }
    }

    // Create a DNS record
    ares_dns_record_t *dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_1, fuzz_uint16_t_2, ARES_OPCODE_QUERY, 
                               ARES_RCODE_NOERROR) == ARES_SUCCESS && dnsrec) {
        
        // Add a resource record to the DNS record
        ares_dns_rr_t *rr = nullptr;
        if (size > 0) {
            // Use first 4 bytes as TTL if available
            unsigned int ttl = 300;
            if (size >= 4) {
                ttl = *(unsigned int*)data;
            }
            
            ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                   fuzz_str_3, ARES_REC_TYPE_A, 
                                   ARES_CLASS_IN, fuzz_uint32_t_4);
            
            if (rr && size > 0) {
                // Set binary data in the RR
                size_t bin_len = size > 100 ? 100 : size;
                ares_dns_rr_set_bin(rr, ARES_RR_A_ADDR, data, bin_len);
                
                // Get the binary data back
                size_t retrieved_len = 0;
                const unsigned char *retrieved_data = 
                    ares_dns_rr_get_bin(rr, ARES_RR_A_ADDR, &retrieved_len);
                
                // Try to get IPv6 address (will return NULL for A record)
                const struct ares_in6_addr *addr6 = 
                    ares_dns_rr_get_addr6(rr, ARES_RR_A_ADDR);
                (void)addr6; // Use to avoid unused variable warning
            }
        }
        
        // Add a query to the DNS record
        ares_dns_record_query_add(dnsrec, fuzz_str_5, 
                                  ARES_REC_TYPE_AAAA, ARES_CLASS_IN);
    }

    // Create a temporary hosts file for gethostbyname_file
    FILE *hosts_file = fopen("input_file", "wb");
    if (hosts_file) {
        if (size > 0) {
            fwrite(data, 1, size > 1024 ? 1024 : size, hosts_file);
        }
        assert_file_closed(&hosts_file);;
        
        // Try to get host by name from file
        struct hostent *host = nullptr;
        ares_gethostbyname_file(channel, fuzz_str_6, fuzz_int32_t_7, &host);
        if (host) {
            ares_free_hostent(host);
        }
        
        // Clean up the temporary file
        unlink("input_file");
    }

    // Reinitialize the channel
    ares_reinit(channel);

    // Perform DNS search with the created DNS record
    if (dnsrec) {
        ares_search_dnsrec(channel, dnsrec, 
            [](void *arg, ares_status_t status, size_t timeouts, 
               const ares_dns_record_t *dnsrec) {
                // Callback for search
                (void)arg;
                (void)status;
                (void)timeouts;
                (void)dnsrec;
            }, nullptr);
    }

    // Perform getaddrinfo lookup
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    ares_getaddrinfo(channel, "example.com", fuzz_str_8, &hints,
        [](void *arg, int status, int timeouts, struct ares_addrinfo *result) {
            // Callback for getaddrinfo
            (void)arg;
            (void)status;
            (void)timeouts;
            (void)result;
        }, nullptr);

    // Process any pending operations
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

    // Cancel any pending operations
    ares_cancel(channel);

    // Clean up
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    assert_file_closed(&in_file);;
    ares_destroy(channel);
    ares_library_cleanup();
    
    assert_file_closed(&hosts_file);
	assert_file_closed(&in_file);
	return 0;
}
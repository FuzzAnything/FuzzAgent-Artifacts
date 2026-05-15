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
//<ID> 940
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 3
//<Quality> {"density":12,"unique_branches":{"ares_dns_rr_get_addr":[[767,7,767,19,0,0,4,0]],"ares_array_insert_at":[[224,7,224,50,0,0,4,0],[226,9,226,31,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_save_options","ares_parse_soa_reply","ares_free_data","ares_dns_parse","ares_dns_record_get_flags","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_name","ares_dns_rr_get_addr","ares_inet_ntop","ares_dns_record_rr_del","ares_dns_record_rr_add","ares_dns_record_destroy","ares_gethostbyname_file","ares_free_hostent","ares_query_dnsrec","ares_timeout","ares_process_fd","ares_queue_wait_empty","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_save_options","ares_parse_soa_reply","ares_free_data","ares_dns_parse","ares_dns_record_get_flags","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_name","ares_dns_rr_get_addr","ares_inet_ntop","ares_dns_record_rr_del","ares_dns_record_rr_add","ares_dns_record_destroy","ares_gethostbyname_file","ares_free_hostent","ares_query_dnsrec","ares_timeout","ares_process_fd","ares_queue_wait_empty","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<92) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Save channel options
    struct ares_options options;
    int optmask = 0;
    ares_save_options(channel, &options, &optmask);

    // Parse input as DNS response for SOA record
    struct ares_soa_reply *soa_reply = nullptr;
    if (size > 0) {
        ares_parse_soa_reply(data, static_cast<int>(size), &soa_reply);
        if (soa_reply) {
            ares_free_data(soa_reply);
        }
    }

    // Parse input as DNS record
    ares_dns_record_t *dnsrec = nullptr;
    if (size > 0) {
        ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
        
        if (dnsrec) {
            // Get flags
            unsigned short flags = ares_dns_record_get_flags(dnsrec);
            
            // Count RRs in answer section
            size_t answer_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
            
            // Process each RR in answer section
            for (size_t i = 0; i < answer_count; i++) {
                const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_2);
                if (rr) {
                    const char *name = ares_dns_rr_get_name(rr);
                    // Convert IP address to string if needed
                    char ip_str[INET6_ADDRSTRLEN];
                    const struct in_addr *addr = ares_dns_rr_get_addr(rr, ARES_RR_A_ADDR);
                    if (addr) {
                        ares_inet_ntop(fuzz_int32_t_3, addr, ip_str, sizeof(ip_str));
                    }
                }
            }
            
            // Delete first RR if exists
            if (answer_count > 0) {
                ares_dns_record_rr_del(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_4);
            }
            
            // Add a new RR
            ares_dns_rr_t *new_rr = nullptr;
            ares_dns_record_rr_add(&new_rr, dnsrec, ARES_SECTION_ANSWER, 
                                  fuzz_str_5, ARES_REC_TYPE_A, 
                                  ARES_CLASS_IN, fuzz_uint32_t_6);
            
            ares_dns_record_destroy(dnsrec);
        }
    }

    // Create temporary file with input data for gethostbyname_file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Write to a temporary hosts file
        FILE *hosts_file = fopen("input_file", "wb");
        if (hosts_file) {
            fwrite(data, 1, size, hosts_file);
            assert_file_closed(&hosts_file);;
            
            // Lookup hostname in file
            struct hostent *host = nullptr;
            ares_gethostbyname_file(channel, fuzz_str_7, fuzz_int32_t_8, &host);
            if (host) {
                ares_free_hostent(host);
            }
            
            // Remove temporary file
            unlink("input_file");
        }
        assert_file_closed(&in_file);;
    }

    // Perform DNS query
    unsigned short qid = 0;
    ares_query_dnsrec(channel, fuzz_str_9, ARES_CLASS_IN, 
                     ARES_REC_TYPE_A, nullptr, nullptr, &qid);

    // Process events with timeout
    struct timeval tv;
    struct timeval *timeout = ares_timeout(channel, nullptr, &tv);
    
    // Process file descriptors (using dummy values since we don't have real sockets)
    ares_process_fd(channel, ARES_SOCKET_BAD, fuzz_int32_t_10);
    
    // Wait for queue to empty
    ares_queue_wait_empty(channel, fuzz_int32_t_11);

    // Cleanup
    ares_destroy(channel);
    ares_library_cleanup();

    assert_file_closed(&in_file);
	return 0;
}
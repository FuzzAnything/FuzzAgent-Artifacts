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
//<ID> 193
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_parse_ns_reply","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_get_keys","ares_dns_record_get_flags","ares_dns_section_tostr","ares_dns_record_rr_get_const","ares_dns_rr_get_ttl","ares_dns_record_rr_get","ares_dns_rr_set_ttl","ares_query_dnsrec","ares_process","ares_dns_record_destroy","ares_free_hostent","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_parse_ns_reply","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_get_keys","ares_dns_record_get_flags","ares_dns_section_tostr","ares_dns_record_rr_get_const","ares_dns_rr_get_ttl","ares_dns_record_rr_get","ares_dns_rr_set_ttl","ares_query_dnsrec","ares_process","ares_dns_record_destroy","ares_free_hostent","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<53) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }

    // Parse the input data as a DNS response
    struct hostent *host = nullptr;
    int parse_result = ares_parse_ns_reply(data, size, &host);
    
    // Create a new DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, fuzz_uint16_t_2, fuzz_uint16_t_3, 
                                                  ARES_OPCODE_QUERY, 
                                                  ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Add a resource record to the DNS record
        ares_dns_rr_t *rr_out = nullptr;
        status = ares_dns_record_rr_add(&rr_out, dnsrec, ARES_SECTION_ANSWER,
                                       fuzz_str_4, ARES_REC_TYPE_A,
                                       ARES_CLASS_IN, fuzz_uint32_t_5);
        
        // Get keys for A record type
        size_t key_cnt = 0;
        const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(ARES_REC_TYPE_A, &key_cnt);
        
        // Get flags from DNS record
        unsigned short flags = ares_dns_record_get_flags(dnsrec);
        
        // Get section string
        const char *section_str = ares_dns_section_tostr(ARES_SECTION_ANSWER);
        
        // Get RR from DNS record (const version)
        const ares_dns_rr_t *const_rr = ares_dns_record_rr_get_const(dnsrec, 
                                                                    ARES_SECTION_ANSWER, fuzz_uint64_t_6);
        if (const_rr) {
            // Get TTL from RR
            unsigned int ttl = ares_dns_rr_get_ttl(const_rr);
            
            // Get RR (non-const version) and set new TTL
            ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_7);
            if (rr) {
                status = ares_dns_rr_set_ttl(rr, ttl + 100);
            }
        }
        
        // Send a DNS query using the DNS record
        unsigned short qid = 0;
        status = ares_query_dnsrec(channel, fuzz_str_8, ARES_CLASS_IN,
                                  ARES_REC_TYPE_A, nullptr, nullptr, &qid);
        
        // Process the channel
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        ares_process(channel, &read_fds, &write_fds);
        
        // Write some data to output file
        if (section_str) {
            fwrite(section_str, 1, strlen(section_str), out_file);
        }
        fwrite(&flags, sizeof(flags), 1, out_file);
        
        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Free hostent if parsing succeeded
    if (parse_result == ARES_SUCCESS && host) {
        ares_free_hostent(host);
    }
    
    // Cancel any pending operations
    ares_cancel(channel);
    
    // Close output file
    assert_file_closed(&out_file);;
    
    // Clean up channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();
    
    assert_file_closed(&out_file);
	return 0;
}
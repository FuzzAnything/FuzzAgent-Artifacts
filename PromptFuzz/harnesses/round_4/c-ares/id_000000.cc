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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"ares_dns_rcode_isvalid":[[54,5,54,28,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_dns_parse","ares_dns_record_get_flags","ares_parse_ns_reply","ares_free_hostent","ares_dns_section_tostr","ares_dns_record_rr_get_const","ares_dns_rr_get_ttl","ares_dns_record_rr_get","ares_dns_rr_set_ttl","ares_query_dnsrec","ares_process","ares_dns_record_destroy","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_dns_parse","ares_dns_record_get_flags","ares_parse_ns_reply","ares_free_hostent","ares_dns_section_tostr","ares_dns_record_rr_get_const","ares_dns_rr_get_ttl","ares_dns_record_rr_get","ares_dns_rr_set_ttl","ares_query_dnsrec","ares_process","ares_dns_record_destroy","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel for DNS operations
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Parse the input data as a DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec != nullptr) {
        // 1. Get flags from the parsed DNS record
        unsigned short flags = ares_dns_record_get_flags(dnsrec);
        
        // 2. Parse the data as an NS reply
        struct hostent *host = nullptr;
        int parse_result = ares_parse_ns_reply(data, size, &host);
        
        if (parse_result == ARES_SUCCESS && host != nullptr) {
            // Process the parsed hostent data if needed
            ares_free_hostent(host);
        }
        
        // 3. Get sections and process resource records
        // We'll process the answer section
        const char *section_str = ares_dns_section_tostr(ARES_SECTION_ANSWER);
        
        // Get the first resource record in answer section
        size_t rr_count = 0;
        // We need to iterate through sections - for simplicity, let's process answer section
        const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_2);
        
        if (rr != nullptr) {
            // 4. Get the current TTL and set a new one
            unsigned int current_ttl = ares_dns_rr_get_ttl(rr);
            // We need a non-const RR to set TTL, so get a mutable version
            ares_dns_rr_t *mutable_rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_3);
            if (mutable_rr != nullptr) {
                // Set TTL to a modified value (current + 1000)
                ares_dns_rr_set_ttl(mutable_rr, current_ttl + 1000);
            }
        }
        
        // 5. Use ares_query_dnsrec to send a DNS query
        // We'll query for a fixed domain to demonstrate the API usage
        unsigned short qid = 0;
        status = ares_query_dnsrec(channel, fuzz_str_4, 
                                   ARES_CLASS_IN, ARES_REC_TYPE_A, 
                                   [](void *arg, ares_status_t status, 
                                      size_t timeouts, const ares_dns_record_t *dnsrec) {
                                       // Callback for query completion
                                       // Process the response if needed
                                   }, 
                                   nullptr, &qid);
        
        // Process any pending operations on the channel
        // We need to create a minimal fd set for processing
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        // Use ares_process to handle any pending operations
        ares_process(channel, &read_fds, &write_fds);
        
        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Cancel any pending operations
    ares_cancel(channel);
    
    // Clean up channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();
    
    return 0;
}
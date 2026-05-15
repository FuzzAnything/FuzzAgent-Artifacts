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
//<ID> 341
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 8
//<Quality> {"density":14,"unique_branches":{"ares_dns_rr_set_bin":[[1246,40,1246,71,0,0,4,0],[1249,29,1249,40,0,0,4,0],[1259,7,1259,18,0,0,4,0],[1264,7,1264,29,0,0,4,1]],"ares_dns_rr_set_bin_own":[[1203,7,1203,59,0,0,4,1],[1207,7,1207,59,0,0,4,0],[1209,9,1209,21,0,0,4,1],[1213,9,1213,22,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_dns_parse","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_query_cnt","ares_dns_record_query_set_type","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_set_ttl","ares_dns_rr_get_opt_cnt","ares_dns_rr_get_opt","ares_dns_rr_del_opt_byid","ares_dns_rr_key_tostr","ares_dns_rr_get_type","ares_dns_rr_set_bin","ares_set_servers_ports_csv","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_dns_parse","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_query_cnt","ares_dns_record_query_set_type","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_set_ttl","ares_dns_rr_get_opt_cnt","ares_dns_rr_get_opt","ares_dns_rr_del_opt_byid","ares_dns_rr_key_tostr","ares_dns_rr_get_type","ares_dns_rr_set_bin","ares_set_servers_ports_csv","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<47) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Create a channel for DNS operations
    ares_channel_t* channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Use input data as a DNS response buffer
    if (size > 0) {
        // Parse the DNS response data into a DNS record structure
        ares_dns_record_t* dnsrec = nullptr;
        ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
        
        if (parse_status == ARES_SUCCESS && dnsrec != nullptr) {
            // Get DNS record opcode and convert to string using required API
            ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
            const char* opcode_str = ares_dns_opcode_tostr(opcode);
            
            // Get number of queries in the DNS record
            size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
            
            // If there are queries, modify the first query's type
            if (query_cnt > 0) {
                // Set the first query type to A record (type 1)
                ares_dns_record_query_set_type(dnsrec, fuzz_uint64_t_2, ARES_REC_TYPE_A);
            }
            
            // Process resource records in the answer section
            size_t answer_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
            if (answer_cnt > 0) {
                // Get the first resource record from answer section
                ares_dns_rr_t* rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_3);
                if (rr != nullptr) {
                    // Set TTL for this resource record using required API
                    // Use a fixed TTL value derived from input size for consistency
                    unsigned int ttl = static_cast<unsigned int>(size % 3600);
                    if (ttl < 60) ttl = 300; // Minimum reasonable TTL
                    ares_dns_rr_set_ttl(rr, ttl);
                    
                    // Try to retrieve an OPT record option if present
                    const unsigned char* opt_val = nullptr;
                    size_t opt_len = 0;
                    
                    // First check if there are any OPT options
                    size_t opt_cnt = ares_dns_rr_get_opt_cnt(rr, ARES_RR_OPT_OPTIONS);
                    if (opt_cnt > 0) {
                        // Get the first OPT option using required API
                        unsigned short opt_id = ares_dns_rr_get_opt(rr, ARES_RR_OPT_OPTIONS, fuzz_uint64_t_4, 
                                                                   &opt_val, &opt_len);
                        
                        // If we have a valid option ID, try to delete it using required API
                        if (opt_id != 0) {
                            ares_dns_rr_del_opt_byid(rr, ARES_RR_OPT_OPTIONS, opt_id);
                        }
                    }
                    
                    // Convert a DNS RR key to string using required API
                    // We'll use A record key as an example
                    const char* key_str = ares_dns_rr_key_tostr(ARES_RR_A_ADDR);
                    
                    // For demonstration, if this is a TXT record, set a string value
                    if (ares_dns_rr_get_type(rr) == ARES_REC_TYPE_TXT) {
                        // Note: ares_dns_rr_set_str is not in the provided API list
                        // We'll use ares_dns_rr_set_bin instead with a string
                        const char* txt_data = "fuzz";
                        ares_dns_rr_set_bin(rr, ARES_RR_TXT_DATA, 
                                           fuzz_str_5, 
                                           static_cast<size_t>(fuzz_str_sz_5));
                    }
                }
            }
            
            // Set DNS servers using CSV format with required API
            // Use a simple local server for testing
            const char* servers_csv = "127.0.0.1";
            ares_set_servers_ports_csv(channel, fuzz_str_6);
            
            // Clean up DNS record
            ares_dns_record_destroy(dnsrec);
        }
    }
    
    // Clean up resources
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
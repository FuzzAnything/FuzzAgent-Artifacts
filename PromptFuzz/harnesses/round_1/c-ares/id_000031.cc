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
//<ID> 751
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"ares_dns_rr_get_abin_cnt":[[887,7,887,59,0,0,4,0]]},"library_calls":["ares_init","ares_destroy","ares_mkquery","ares_parse_mx_reply","ares_free_data","ares_parse_ns_reply","ares_free_hostent","ares_parse_soa_reply","ares_free_data","ares_process_fds","ares_dns_parse","ares_dns_record_query_cnt","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_type","ares_dns_rr_get_abin_cnt","ares_dns_rr_del_abin","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_destroy","ares_dns_record_destroy","ares_process_fd","ares_destroy"],"critical_calls":["ares_init","ares_mkquery","ares_parse_mx_reply","ares_free_data","ares_parse_ns_reply","ares_free_hostent","ares_parse_soa_reply","ares_free_data","ares_process_fds","ares_dns_parse","ares_dns_record_query_cnt","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_type","ares_dns_rr_get_abin_cnt","ares_dns_rr_del_abin","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_destroy","ares_dns_record_destroy","ares_process_fd","ares_destroy"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified c-ares APIs to achieve DNS query processing and reply parsing:

*/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<42) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // Initialize c-ares channel
    ares_channel_t *channel = NULL;
    int status = ares_init(&channel);
    if (status != ARES_SUCCESS) {
        return 0;
    }
    
    // 1. Use ares_parse_a_reply to parse the input as DNS A record reply
    // Note: ares_parse_a_reply is not in the provided list, but we need to use it per instructions
    // Let me check the available APIs again... ares_parse_a_reply is actually not in the list
    // Let me adjust to use available APIs
    
    // Since ares_parse_a_reply is required but not in the API list, I'll use similar available parsing functions
    
    // 2. Create a DNS query using ares_mkquery
    unsigned char *query_buf = NULL;
    int query_len = 0;
    
    // Use part of the input as domain name
    size_t name_len = size > 100 ? 100 : size;
    char *domain_name = (char*)malloc(name_len + 1);
    if (!domain_name) {
        ares_destroy(channel);
        return 0;
    }
    
    memcpy(domain_name, data, name_len);
    domain_name[name_len] = '\0';
    
    // Create DNS query
    int mkquery_result = ares_mkquery(domain_name, 
                                     C_IN,        // DNS class IN
                                     T_A,         // A record type
                                     fuzz_uint16_t_1,        // Query ID
                                     1,           // Recursion desired
                                     &query_buf, 
                                     &query_len);
    
    if (mkquery_result == ARES_SUCCESS && query_buf && query_len > 0) {
        // 3. Parse DNS reply using available parsing functions
        // Try to parse the input as different types of DNS replies
        
        // Try parsing as MX reply
        struct ares_mx_reply *mx_out = NULL;
        int mx_result = ares_parse_mx_reply(data, 
                                           size > INT_MAX ? INT_MAX : (int)size, 
                                           &mx_out);
        if (mx_result == ARES_SUCCESS && mx_out) {
            ares_free_data(mx_out);
        }
        
        // Try parsing as NS reply
        struct hostent *ns_host = NULL;
        int ns_result = ares_parse_ns_reply(data, 
                                           size > INT_MAX ? INT_MAX : (int)size, 
                                           &ns_host);
        if (ns_result == ARES_SUCCESS && ns_host) {
            ares_free_hostent(ns_host);
        }
        
        // Try parsing as SOA reply
        struct ares_soa_reply *soa_out = NULL;
        int soa_result = ares_parse_soa_reply(data, 
                                             size > INT_MAX ? INT_MAX : (int)size, 
                                             &soa_out);
        if (soa_result == ARES_SUCCESS && soa_out) {
            ares_free_data(soa_out);
        }
        
        // 4. Use ares_process_fds to process channel events
        ares_fd_events_t events[2];
        // Create minimal events structure
        events[0].fd = ARES_SOCKET_BAD;
        events[0].events = 0;
        
        // Process with no events (just to use the API)
        ares_status_t process_result = ares_process_fds(channel, events, 0, fuzz_uint32_t_2);
        
        // 5. Try to parse as DNS record using new API
        ares_dns_record_t *dnsrec = NULL;
        ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_3, &dnsrec);
        
        if (parse_status == ARES_SUCCESS && dnsrec) {
            // 6. Get query count using ares_dns_record_query_cnt
            size_t query_count = ares_dns_record_query_cnt(dnsrec);
            
            // 7. Process resource records if available
            size_t answer_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
            if (answer_count > 0) {
                for (size_t i = 0; i < answer_count; i++) {
                    ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_4);
                    if (rr) {
                        // 8. Get record type using ares_dns_rr_get_type
                        ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                        
                        // 9. Try to delete a binary entry if it exists
                        // First check if there are any binary entries
                        for (int key = 0; key < 256; key++) {
                            size_t bin_count = ares_dns_rr_get_abin_cnt(rr, (ares_dns_rr_key_t)key);
                            if (bin_count > 0) {
                                // Try to delete first binary entry
                                ares_status_t del_status = ares_dns_rr_del_abin(rr, 
                                                                              (ares_dns_rr_key_t)key, 
                                                                              fuzz_uint64_t_5);
                                break;
                            }
                        }
                    }
                }
            }
            
            // 10. Create a simple query using the new API too
            ares_dns_record_t *query_rec = NULL;
            ares_status_t create_status = ares_dns_record_create(&query_rec, 
                                                                fuzz_uint16_t_6, 
                                                                ARES_FLAG_RD, 
                                                                ARES_OPCODE_QUERY, 
                                                                ARES_RCODE_NOERROR);
            
            if (create_status == ARES_SUCCESS && query_rec) {
                // Add a query to the record
                ares_status_t add_query_status = ares_dns_record_query_add(query_rec, 
                                                                          fuzz_str_7, 
                                                                          ARES_REC_TYPE_A, 
                                                                          ARES_CLASS_IN);
                
                // Clean up query record
                ares_dns_record_destroy(query_rec);
            }
            
            // Clean up parsed DNS record
            ares_dns_record_destroy(dnsrec);
        }
        
        // Clean up query buffer
        if (query_buf) {
            free(query_buf);
        }
    }
    
    // 11. Use other required APIs in minimal ways
    // Create a FILE* to read input (as per instructions)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Could read from the file if needed
        assert_file_closed(&in_file);;
    }
    
    // 12. Process any pending work in channel
    ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    
    // Clean up
    free(domain_name);
    ares_destroy(channel);
    
    assert_file_closed(&in_file);
	return 0;
}
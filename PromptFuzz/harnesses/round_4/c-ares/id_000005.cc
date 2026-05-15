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
//<ID> 112
//<Prompt> []
/*<Combination>: [
*/
//<score> 64, nr_unique_branch: 18
//<Quality> {"density":16,"unique_branches":{"ares_dns_rr_set_bin_own":[[1202,7,1202,58,0,0,4,0],[1203,7,1203,59,0,0,4,0]],"ares_parse_a_reply":[[70,7,70,29,0,0,4,0],[87,7,87,23,0,0,4,0],[87,27,87,40,0,0,4,1],[102,7,102,30,0,0,4,0]],"ares_dns_class_fromstr":[[683,15,683,35,0,0,4,1],[684,9,684,42,0,0,4,1]],"ares_dns_rr_set_bin":[[1246,6,1246,36,0,0,4,1],[1246,40,1246,71,0,0,4,1],[1249,29,1249,40,0,0,4,1],[1252,7,1252,19,0,0,4,1],[1259,7,1259,18,0,0,4,1],[1264,7,1264,29,0,0,4,0]],"ares_dns_class_tostr":[[243,5,243,27,0,0,4,0]],"ares_dns_record_create_query":[[1595,7,1595,29,0,0,4,0],[1633,7,1633,29,0,0,4,0]],"ares_search":[[459,7,459,29,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_dns_class_fromstr","ares_dns_class_tostr","ares_parse_a_reply","ares_dns_record_create","ares_dns_record_get_opcode","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_set_bin","ares_dns_record_destroy","ares_inet_ntop","ares_search","ares_queue_active_queries","ares_create_query","ares_send","ares_free_string","ares_process_fd","ares_free_hostent","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_dns_class_fromstr","ares_dns_class_tostr","ares_parse_a_reply","ares_dns_record_create","ares_dns_record_get_opcode","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_set_bin","ares_dns_record_destroy","ares_inet_ntop","ares_search","ares_queue_active_queries","ares_create_query","ares_send","ares_free_string","ares_process_fd","ares_free_hostent","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


// Include necessary headers for c-ares

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<91) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_6, fuzz_char_array_size_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
	
		const unsigned char * fuzzer_var_ares_dns_rr_set_bin_10_2 = fuzz_char_array_6;
		size_t fuzzer_size_ares_dns_rr_set_bin_10_2 = static_cast<size_t>(fuzz_char_array_size_6);//fuzzer shim end}




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

    // Use ares_dns_class_fromstr to parse a DNS class from input
    ares_dns_class_t dns_class;
    const char *class_str = "IN";  // Default class
    if (size > 0) {
        // Create a temporary string from input for parsing
        char *temp_str = (char *)malloc(size + 1);
        if (temp_str) {
            memcpy(temp_str, data, size);
            temp_str[size] = '\0';
            ares_dns_class_fromstr(&dns_class, temp_str);
            free(temp_str);
        }
    }

    // Use ares_dns_class_tostr to convert class back to string
    const char *class_str_result = ares_dns_class_tostr(dns_class);

    // Parse input as DNS A record reply
    struct hostent *host = nullptr;
    struct ares_addrttl addrttls[10];
    int naddrttls = 0;
    
    if (size > 0) {
        ares_parse_a_reply(data, size, &host, addrttls, &naddrttls);
    }

    // Create a DNS record for testing
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, fuzz_uint16_t_1, fuzz_uint16_t_2, 
                                                  ARES_OPCODE_QUERY, 
                                                  ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Get opcode from DNS record
        ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
        
        // Add a query to the DNS record
        ares_dns_record_query_add(dnsrec, fuzz_str_3, 
                                  ARES_REC_TYPE_A, dns_class);
        
        // Add an answer RR to the DNS record
        ares_dns_rr_t *rr = nullptr;
        status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                                       fuzz_str_4, ARES_REC_TYPE_A,
                                       dns_class, fuzz_uint32_t_5);
        
        if (status == ARES_SUCCESS && rr) {
            // Use ares_dns_rr_set_bin to set binary data in RR
            if (size > 0) {
                // Use first 4 bytes of input for IPv4 address
                unsigned char ip_data[4] = {0};
                size_t data_len = size > 4 ? 4 : size;
                memcpy(ip_data, data, data_len);
                
                ares_dns_rr_set_bin(rr, ARES_RR_A_ADDR, fuzzer_var_ares_dns_rr_set_bin_10_2, fuzzer_size_ares_dns_rr_set_bin_10_2);
            }
        }
        
        // Destroy the DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // Use ares_inet_ntop to convert IP address to string
    char ip_str[INET_ADDRSTRLEN];
    if (naddrttls > 0) {
        ares_inet_ntop(fuzz_int32_t_7, &addrttls[0].ipaddr, ip_str, sizeof(ip_str));
    }

    // Use ares_search to perform DNS search
    ares_search(channel, fuzz_str_8, dns_class, T_A, 
                [](void *arg, int status, int timeouts, 
                   const unsigned char *abuf, int alen) {
                    // Callback for search results
                }, nullptr);

    // Check active queries in channel
    size_t active_queries = ares_queue_active_queries(channel);

    // Use ares_send to send a DNS query
    if (size > 12) {  // Minimum DNS query size
        // Create a simple DNS query
        unsigned char *query_buf = nullptr;
        int query_len = 0;
        
        if (ares_create_query(fuzz_str_9, C_IN, T_A, fuzz_uint16_t_10, fuzz_int32_t_11, 
                             &query_buf, &query_len, fuzz_int32_t_12) == ARES_SUCCESS) {
            ares_send(channel, query_buf, fuzz_int32_t_13,
                     [](void *arg, int status, int timeouts,
                        const unsigned char *abuf, int alen) {
                         // Callback for send results
                     }, nullptr);
            
            // Free the query buffer
            ares_free_string(query_buf);
        }
    }

    // Process any pending events
    ares_process_fd(channel, fuzz_int32_t_14, fuzz_int32_t_15);

    // Clean up
    if (host) {
        ares_free_hostent(host);
    }
    
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}
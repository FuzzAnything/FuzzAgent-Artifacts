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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 19
//<Quality> {"density":12,"unique_branches":{"ares_buf_parse_dns_binstr_int":[[1254,9,1254,27,0,0,4,1]],"ares_expand_string_ex":[[50,7,50,22,0,0,4,1],[50,26,50,38,0,0,4,1],[50,42,50,51,0,0,4,1],[50,55,50,69,0,0,4,1],[54,7,54,21,0,0,4,1],[54,25,54,47,0,0,4,1],[61,7,61,8,0,0,4,0],[67,7,67,18,0,0,4,1],[72,7,72,29,0,0,4,1],[79,7,79,29,0,0,4,0],[79,7,79,29,0,0,4,1],[87,7,87,30,0,0,4,1],[87,34,87,57,0,0,4,0],[87,34,87,57,0,0,4,1]],"ares_expand_string":[[99,7,99,22,0,0,4,1],[99,26,99,38,0,0,4,1],[99,42,99,51,0,0,4,1],[99,55,99,69,0,0,4,1]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_create_query","ares_dns_parse","ares_dns_record_query_cnt","ares_dns_record_query_set_name","ares_dns_record_rr_get","ares_dns_rr_set_u16","ares_dns_rr_get_abin","ares_dns_rr_del_abin","ares_dns_addr_to_ptr","ares_free_string","ares_expand_name","ares_free_string","ares_expand_string","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_create_query","ares_dns_parse","ares_dns_record_query_cnt","ares_dns_record_query_set_name","ares_dns_record_rr_get","ares_dns_rr_set_u16","ares_dns_rr_get_abin","ares_dns_rr_del_abin","ares_dns_addr_to_ptr","ares_free_string","ares_expand_name","ares_free_string","ares_expand_string","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel for DNS operations
    ares_channel_t *channel = nullptr;
    if (ares_init_options(&channel, nullptr, 0) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Create a DNS query using input data as domain name
    if (size > 0 && size < 256) {  // Reasonable domain name length
        char domain[256];
        memcpy(domain, data, size);
        domain[size] = '\0';
        
        unsigned char *query_buf = nullptr;
        int query_len = 0;
        
        // Use ares_create_query to create a DNS query
        if (ares_create_query(domain, C_IN, T_A, 12345, 1, 
                              &query_buf, &query_len, 512) == ARES_SUCCESS) {
            // Create a DNS record from the query buffer
            ares_dns_record_t *dnsrec = nullptr;
            if (ares_dns_parse(query_buf, query_len, 0, &dnsrec) == ARES_SUCCESS) {
                // Modify the DNS record using various APIs
                
                // 1. Change query name if there are queries
                size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
                if (query_cnt > 0) {
                    // Use some of input data to create modified name
                    char modified_name[256];
                    size_t name_len = size > 100 ? 100 : size;
                    snprintf(modified_name, sizeof(modified_name), 
                            "modified-%.*s", (int)name_len, data);
                    
                    ares_dns_record_query_set_name(dnsrec, 0, modified_name);
                }
                
                // 2. Get first resource record and modify it
                ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, 0);
                if (rr != nullptr) {
                    // Set a 16-bit value (like port or priority)
                    ares_dns_rr_set_u16(rr, ARES_RR_A_ADDR, 12345);
                    
                    // Delete binary data if it exists
                    size_t bin_len = 0;
                    const unsigned char *bin_data = ares_dns_rr_get_abin(
                        rr, ARES_RR_A_ADDR, 0, &bin_len);
                    if (bin_data != nullptr && bin_len > 0) {
                        ares_dns_rr_del_abin(rr, ARES_RR_A_ADDR, 0);
                    }
                }
                
                // Create a test address for ares_dns_addr_to_ptr
                struct ares_addr addr;
                if (size >= sizeof(struct in_addr)) {
                    addr.family = AF_INET;
                    memcpy(&addr.addr.addr4, data, sizeof(struct in_addr));
                    
                    // Convert address to PTR string
                    char *ptr_str = ares_dns_addr_to_ptr(&addr);
                    if (ptr_str != nullptr) {
                        ares_free_string(ptr_str);
                    }
                }
                
                // Try to expand names/strings from the query buffer
                if (query_len > 12) {  // Minimum DNS header size
                    char *expanded_name = nullptr;
                    long enclen = 0;
                    
                    // Try to expand a name from the query
                    ares_expand_name(query_buf + 12, query_buf, query_len, 
                                    &expanded_name, &enclen);
                    if (expanded_name != nullptr) {
                        ares_free_string(expanded_name);
                    }
                    
                    // Try to expand a string from the query
                    unsigned char *expanded_str = nullptr;
                    if (query_len > 20) {
                        ares_expand_string(query_buf + 20, query_buf, query_len,
                                          &expanded_str, &enclen);
                        if (expanded_str != nullptr) {
                            free(expanded_str);
                        }
                    }
                }
                
                ares_dns_record_destroy(dnsrec);
            }
            
            if (query_buf != nullptr) {
                free(query_buf);
            }
        }
    }

    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
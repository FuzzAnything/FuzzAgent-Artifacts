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
//<ID> 786
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 9
//<Quality> {"density":10,"unique_branches":{"ares_free_data":[[94,7,94,40,0,0,4,0]],"ares_get_servers_ports":[[1174,7,1174,22,0,0,4,1],[1180,56,1180,68,0,0,4,0],[1180,56,1180,68,0,0,4,1],[1186,9,1186,19,0,0,4,1],[1190,9,1190,18,0,0,4,1],[1202,9,1202,37,0,0,4,0],[1211,7,1211,29,0,0,4,1]],"ares_malloc_data":[[154,5,154,38,0,0,4,0]]},"library_calls":["ares_library_init_mem","ares_init_options","ares_library_cleanup","ares_set_server_state_callback","ares_set_local_ip6","ares_get_servers_ports","ares_free_data","ares_dns_parse","ares_dns_record_get_flags","ares_dns_record_query_cnt","ares_dns_record_query_get","ares_expand_name","ares_dns_record_rr_get_const","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_key_to_rec_type","ares_strerror","ares_dns_record_destroy","ares_freeaddrinfo","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init_mem","ares_init_options","ares_set_server_state_callback","ares_set_local_ip6","ares_get_servers_ports","ares_free_data","ares_dns_parse","ares_dns_record_get_flags","ares_dns_record_query_cnt","ares_dns_record_query_get","ares_expand_name","ares_dns_record_rr_get_const","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_key_to_rec_type","ares_strerror","ares_dns_record_destroy","ares_freeaddrinfo","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Server state callback function
static void  id_000034_server_state_callback(const char* name, ares_bool_t is_up, 
                                  int status, void* user_data) {
    // Empty callback - just needed to demonstrate API usage
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
	//fuzzer shim end}




    // Initialize library with custom memory functions
    ares_library_init_mem(ARES_LIB_INIT_ALL, 
                         malloc, 
                         free, 
                         realloc);
    
    ares_channel_t* channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize channel
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Set server state callback
    ares_set_server_state_callback(channel,  id_000034_server_state_callback, nullptr);
    
    // Set local IPv6 address from input data (first 16 bytes if available)
    if (size >= 16) {
        const unsigned char* ip6 = data;
        ares_set_local_ip6(channel, ip6);
    }
    
    // Get servers and ports
    struct ares_addr_port_node* servers = nullptr;
    if (ares_get_servers_ports(channel, &servers) == ARES_SUCCESS) {
        // Free the server list (using ares_free_data)
        if (servers) {
            ares_free_data(servers);
        }
    }
    
    // Parse DNS record from input data
    ares_dns_record_t* dnsrec = nullptr;
    if (size > 0) {
        ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_2, &dnsrec);
        
        if (parse_status == ARES_SUCCESS && dnsrec) {
            // Get DNS record flags
            unsigned short flags = ares_dns_record_get_flags(dnsrec);
            
            // Try to expand a name from the DNS record
            // Look for a query in the DNS record to expand
            size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
            if (query_cnt > 0) {
                const char* name = nullptr;
                ares_dns_rec_type_t qtype;
                ares_dns_class_t qclass;
                
                if (ares_dns_record_query_get(dnsrec, fuzz_uint64_t_3, &name, &qtype, &qclass) 
                    == ARES_SUCCESS && name) {
                    
                    // Try to expand the name (simulate compression expansion)
                    // We need encoded DNS data for ares_expand_name
                    // Use the original input data as buffer
                    char* expanded = nullptr;
                    long enclen = 0;
                    
                    // Note: ares_expand_name expects a compressed name at 'encoded'
                    // Since we don't know where compressed names are in the data,
                    // we'll try from the beginning with bounds checking
                    if (size > 2) {
                        // Use first 2 bytes as potential offset (safe bounds)
                        unsigned short offset = (data[0] << 8) | data[1];
                        offset = offset % (size - 2);  // Ensure within bounds
                        
                        const unsigned char* encoded = data + offset;
                        int remaining = size - offset;
                        
                        if (remaining > 0) {
                            ares_expand_name(encoded, data, size, 
                                           &expanded, &enclen);
                            if (expanded) {
                                free(expanded);
                            }
                        }
                    }
                }
            }
            
            // Get RR keys and convert to record type for first RR in answer section
            size_t rr_count = 0;
            const ares_dns_rr_t* rr = ares_dns_record_rr_get_const(dnsrec, 
                                                                  ARES_SECTION_ANSWER, 
                                                                  0);
            if (rr) {
                // Get RR type
                ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                
                // Get RR keys for this type
                size_t key_count = 0;
                const ares_dns_rr_key_t* keys = ares_dns_rr_get_keys(rr_type, 
                                                                    &key_count);
                
                if (keys && key_count > 0) {
                    // Convert first key to record type
                    ares_dns_rec_type_t converted_type = 
                        ares_dns_rr_key_to_rec_type(keys[0]);
                    
                    // Get string representation of error code
                    const char* error_str = ares_strerror(ARES_SUCCESS);
                }
            }
            
            // Clean up DNS record
            ares_dns_record_destroy(dnsrec);
        }
    }
    
    // Create a dummy ares_addrinfo structure and free it
    // This demonstrates ares_freeaddrinfo usage
    struct ares_addrinfo* ai = (struct ares_addrinfo*)malloc(sizeof(struct ares_addrinfo));
    if (ai) {
        memset(ai, 0, sizeof(struct ares_addrinfo));
        ares_freeaddrinfo(ai);
    }
    
    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
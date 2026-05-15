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
//<ID> 403
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 10
//<Quality> {"density":4,"unique_branches":{"ares_free_data":[[90,7,90,35,0,0,4,0]],"ares_malloc_data":[[153,5,153,33,0,0,4,0]],"ares_get_servers":[[1119,7,1119,22,0,0,4,1],[1125,56,1125,68,0,0,4,0],[1125,56,1125,68,0,0,4,1],[1131,9,1131,19,0,0,4,1],[1135,9,1135,18,0,0,4,1],[1144,9,1144,37,0,0,4,0],[1153,7,1153,29,0,0,4,1]],"ares_dns_opt_get_name":[[904,5,904,12,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_parse_uri_reply","ares_get_servers","ares_dns_rr_get_keys","ares_dns_opt_get_name","ares_dns_opt_get_datatype","ares_free_data","ares_free_data","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_parse_uri_reply","ares_get_servers","ares_dns_rr_get_keys","ares_dns_opt_get_name","ares_dns_opt_get_datatype","ares_free_data","ares_free_data","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Include c-ares headers

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for very small inputs
    if (size < 4) {
	return 0;
	}
	
    
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
    
    // Event: Parse a URI DNS reply from input data and analyze DNS options
    struct ares_uri_reply *uri_reply = nullptr;
    int parse_result = ares_parse_uri_reply(data, size, &uri_reply);
    
    if (parse_result == ARES_SUCCESS && uri_reply != nullptr) {
        // Demonstrate ares_get_servers - get configured DNS servers
        struct ares_addr_node *servers = nullptr;
        int get_servers_result = ares_get_servers(channel, &servers);
        
        // Process the URI reply linked list
        struct ares_uri_reply *current = uri_reply;
        while (current != nullptr) {
            // For demonstration, we'll examine DNS RR keys for URI record type
            size_t key_count = 0;
            const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(ARES_REC_TYPE_URI, &key_count);
            
            // For each key, try to get option names and datatypes for some option numbers
            for (size_t i = 0; i < key_count && i < 5; i++) { // Limit to 5 keys for fuzzing
                ares_dns_rr_key_t key = keys[i];
                
                // Try a few option numbers (0-2)
                for (unsigned short opt_num = 0; opt_num < 3; opt_num++) {
                    // Get option name
                    const char *opt_name = ares_dns_opt_get_name(key, opt_num);
                    
                    // Get option datatype
                    ares_dns_opt_datatype_t datatype = ares_dns_opt_get_datatype(key, opt_num);
                    
                    // Use the results to avoid compiler warnings about unused variables
                    (void)opt_name;
                    (void)datatype;
                }
            }
            
            current = current->next;
        }
        
        // Free the servers list if it was allocated
        if (servers != nullptr) {
            ares_free_data(servers);
        }
        
        // Free the URI reply structure
        ares_free_data(uri_reply);
    }
    
    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
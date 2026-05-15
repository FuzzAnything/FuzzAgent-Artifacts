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
//<ID> 133
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_dns_parse","ares_dns_record_get_id","ares_set_servers_ports_csv","ares_dns_record_query_cnt","ares_dns_record_query_get","ares_dns_record_query_set_type","ares_dns_rr_get_keys","ares_dns_record_destroy","ares_set_servers_ports_csv","ares_fds","ares_process","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_dns_parse","ares_dns_record_get_id","ares_set_servers_ports_csv","ares_dns_record_query_cnt","ares_dns_record_query_get","ares_dns_record_query_set_type","ares_dns_rr_get_keys","ares_dns_record_destroy","ares_set_servers_ports_csv","ares_fds","ares_process","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel for DNS operations
    ares_channel_t *channel = nullptr;
    if (ares_init_options(&channel, nullptr, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Parse the input data as a DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_2, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec != nullptr) {
        // 1. Get the DNS record ID
        unsigned short record_id = ares_dns_record_get_id(dnsrec);
        
        // 2. Set servers using CSV format (use a dummy CSV from input data)
        // We'll use the first part of input as CSV string (up to 100 bytes or size)
        size_t csv_len = size > 100 ? 100 : size;
        char *csv_servers = (char*)malloc(csv_len + 1);
        if (csv_servers) {
            memcpy(csv_servers, data, csv_len);
            csv_servers[csv_len] = '\0';
            ares_set_servers_ports_csv(channel, csv_servers);
            free(csv_servers);
        }
        
        // 3. Get query count and modify query types
        size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
        for (size_t i = 0; i < query_cnt; i++) {
            // Get current query info
            const char *name = nullptr;
            ares_dns_rec_type_t qtype;
            ares_dns_class_t qclass;
            
            if (ares_dns_record_query_get(dnsrec, fuzz_uint64_t_3, &name, &qtype, &qclass) == ARES_SUCCESS) {
                // Set query type to a different type (cycle through types)
                ares_dns_rec_type_t new_type = static_cast<ares_dns_rec_type_t>(
                    (qtype + 1) % 16); // Simple type rotation
                ares_dns_record_query_set_type(dnsrec, fuzz_uint64_t_4, new_type);
            }
        }
        
        // 4. Get keys for different DNS record types
        size_t key_cnt = 0;
        // Try with a few different record types
        for (int type_val = 1; type_val <= 5; type_val++) {
            ares_dns_rec_type_t type = static_cast<ares_dns_rec_type_t>(type_val);
            const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(type, &key_cnt);
            // keys can be used for further processing if needed
        }
        
        // 5. Set query names (we need to create a new query or modify existing ones)
        // Since we don't have ares_dns_record_query_set_name in the provided APIs,
        // we'll skip this step and note that it's not available
        
        // 6. Convert DNS sections to strings (we need ares_dns_section_tostr)
        // Since this API is not in the provided list, we'll skip this step
        
        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Additional fuzzing: Try to use the input as a CSV for servers
    if (size > 0) {
        // Create a null-terminated string from input for CSV
        char *csv_input = (char*)malloc(size + 1);
        if (csv_input) {
            memcpy(csv_input, data, size);
            csv_input[size] = '\0';
            ares_set_servers_ports_csv(channel, csv_input);
            free(csv_input);
        }
    }
    
    // Process any pending operations
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    if (nfds > 0) {
        // We have file descriptors ready, process them
        ares_process(channel, &read_fds, &write_fds);
    }
    
    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
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
//<ID> 132
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 12
//<Quality> {"density":10,"unique_branches":{"ares_dns_record_query_set_type":[[312,7,312,21,0,0,4,1],[312,25,312,58,0,0,4,1],[313,7,313,51,0,0,4,1]],"ares_dns_section_tostr":[[739,11,739,18,0,0,4,0],[739,11,739,18,0,0,4,1],[740,5,740,29,0,0,4,0],[740,5,740,29,0,0,4,1],[742,5,742,32,0,0,4,0],[742,5,742,32,0,0,4,1],[744,5,744,33,0,0,4,0],[744,5,744,33,0,0,4,1]],"ares_dns_rr_get_keys":[[597,11,597,15,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_dns_parse","ares_dns_record_get_id","ares_set_servers_ports_csv","ares_dns_record_query_cnt","ares_dns_record_query_set_type","ares_dns_rr_get_keys","ares_dns_record_query_set_name","ares_dns_section_tostr","ares_dns_record_destroy","ares_dns_record_create","ares_dns_record_query_cnt","ares_dns_rr_get_keys","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_dns_parse","ares_dns_record_get_id","ares_set_servers_ports_csv","ares_dns_record_query_cnt","ares_dns_record_query_set_type","ares_dns_rr_get_keys","ares_dns_record_query_set_name","ares_dns_section_tostr","ares_dns_record_destroy","ares_dns_record_create","ares_dns_record_query_cnt","ares_dns_rr_get_keys","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<26) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel for DNS operations
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Parse input data as a DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_2, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec != nullptr) {
        // 1. Get DNS record ID
        unsigned short dns_id = ares_dns_record_get_id(dnsrec);
        
        // 2. Set servers and ports using CSV format
        // Create a CSV string from the input data (truncate to reasonable length)
        size_t csv_len = size < 256 ? size : 256;
        char *csv_servers = (char *)malloc(csv_len + 1);
        if (csv_servers) {
            memcpy(csv_servers, data, csv_len);
            csv_servers[csv_len] = '\0';
            ares_set_servers_ports_csv(channel, csv_servers);
            free(csv_servers);
        }
        
        // 3. Set query type for the first query (if exists)
        size_t query_count = ares_dns_record_query_cnt(dnsrec);
        if (query_count > 0) {
            // Use modulo to get a valid DNS record type
            ares_dns_rec_type_t qtype = static_cast<ares_dns_rec_type_t>(
                (data[0] % 16) + 1); // Valid types are usually 1-16
            ares_dns_record_query_set_type(dnsrec, fuzz_uint64_t_3, qtype);
            
            // 4. Get keys for this DNS record type
            size_t key_count = 0;
            const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(qtype, &key_count);
            
            // 5. Set query name for the first query
            // Create a name from input data (truncate to reasonable length)
            size_t name_len = size < 64 ? size : 64;
            char *query_name = (char *)malloc(name_len + 1);
            if (query_name) {
                // Convert binary data to a valid domain name format
                for (size_t i = 0; i < name_len; i++) {
                    char c = data[i % size];
                    // Only use alphanumeric and hyphen for domain names
                    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
                        (c >= '0' && c <= '9') || c == '-' || c == '.') {
                        query_name[i] = c;
                    } else {
                        query_name[i] = 'a' + (c % 26);
                    }
                }
                query_name[name_len] = '\0';
                ares_dns_record_query_set_name(dnsrec, fuzz_uint64_t_4, query_name);
                free(query_name);
            }
        }
        
        // 6. Convert DNS section to string
        // Use modulo to get a valid section
        ares_dns_section_t section = static_cast<ares_dns_section_t>(data[1 % size] % 4);
        const char *section_str = ares_dns_section_tostr(section);
        
        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Create a new DNS record to test more APIs
    ares_dns_record_t *new_dnsrec = nullptr;
    if (ares_dns_record_create(&new_dnsrec, 
                               static_cast<unsigned short>(size & 0xFFFF),  // ID
                               fuzz_uint16_t_5,                                           // Flags
                               ARES_OPCODE_QUERY,                           // Opcode
                               ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        
        // Add a query to the new record
        if (ares_dns_record_query_cnt(new_dnsrec) == 0) {
            // We would need ares_dns_record_query_add which isn't in the list,
            // but we can still use the set functions on existing queries
        }
        
        // Get keys for A record type
        size_t a_key_count = 0;
        const ares_dns_rr_key_t *a_keys = ares_dns_rr_get_keys(ARES_REC_TYPE_A, &a_key_count);
        
        ares_dns_record_destroy(new_dnsrec);
    }
    
    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
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
//<ID> 864
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"ares_dns_opt_get_type_opt":[[753,11,753,16,0,0,4,0],[754,5,754,28,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_dns_parse","ares_dns_record_get_flags","ares_queue_active_queries","ares_dns_write","ares_dns_record_rr_get","ares_dns_rr_get_class","ares_dns_record_rr_get","ares_dns_rr_get_type","ares_dns_opt_get_datatype","ares_save_options","ares_destroy_options","ares_set_servers_ports_csv","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_dns_parse","ares_dns_record_get_flags","ares_queue_active_queries","ares_dns_write","ares_dns_record_rr_get","ares_dns_rr_get_class","ares_dns_record_rr_get","ares_dns_rr_get_type","ares_dns_opt_get_datatype","ares_save_options","ares_destroy_options","ares_set_servers_ports_csv","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize channel with default options
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // 1. Use ares_search-like functionality via ares_query_dnsrec
    // Parse input as DNS response using ares_dns_parse
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t parse_status = ares_dns_parse(data, size, 0, &dnsrec);
    
    if (parse_status == ARES_SUCCESS && dnsrec != nullptr) {
        // 2. Get flags from DNS record
        unsigned short flags = ares_dns_record_get_flags(dnsrec);
        (void)flags; // Use to avoid unused variable warning
        
        // 3. Check active queries
        size_t active_queries = ares_queue_active_queries(channel);
        (void)active_queries;
        
        // 4. Write DNS record back to buffer
        unsigned char *output_buf = nullptr;
        size_t output_len = 0;
        if (ares_dns_write(dnsrec, &output_buf, &output_len) == ARES_SUCCESS) {
            free(output_buf);
        }
        
        // 5. Get class from first resource record if exists
        ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, 0);
        if (rr != nullptr) {
            ares_dns_class_t rr_class = ares_dns_rr_get_class(rr);
            (void)rr_class;
            
            // 6. Get option datatype for OPT record
            // First check if we have an OPT record in additional section
            size_t rr_count = 0;
            for (size_t i = 0; ; i++) {
                rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ADDITIONAL, i);
                if (rr == nullptr) break;
                
                if (ares_dns_rr_get_type(rr) == ARES_REC_TYPE_OPT) {
                    // Get datatype for EDNS option
                    ares_dns_opt_datatype_t datatype = 
                        ares_dns_opt_get_datatype(ARES_RR_OPT_OPTIONS, 0);
                    (void)datatype;
                    break;
                }
            }
        }
        
        // 7. Save channel options
        memset(&options, 0, sizeof(options));
        if (ares_save_options(channel, &options, &optmask) == ARES_SUCCESS) {
            ares_destroy_options(&options);
        }
        
        // 8. Set servers using CSV format (simulating ares_set_servers_csv)
        // Create a mock CSV from input data
        if (size > 0) {
            std::vector<char> server_csv(size + 1);
            memcpy(server_csv.data(), data, size);
            server_csv[size] = '\0';
            
            // Use ares_set_servers_ports_csv as alternative
            ares_set_servers_ports_csv(channel, server_csv.data());
        }
        
        ares_dns_record_destroy(dnsrec);
    }
    
    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
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
//<ID> 884
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"ares_uri_parse_hostport":[[1196,9,1196,31,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_set_servers_csv","ares_mkquery","ares_dns_parse","ares_dns_record_get_id","ares_dns_opt_get_name","ares_dns_record_destroy","ares_gethostbyaddr","ares_fds","ares_fds","ares_timeout","ares_process","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_servers_csv","ares_mkquery","ares_dns_parse","ares_dns_record_get_id","ares_dns_opt_get_name","ares_dns_record_destroy","ares_gethostbyaddr","ares_fds","ares_fds","ares_timeout","ares_process","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<10) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Use input data as DNS server CSV string
    // Ensure we have a null-terminated string for the server CSV
    char *server_csv = nullptr;
    if (size > 0) {
        server_csv = static_cast<char*>(malloc(size + 1));
        if (!server_csv) {
            ares_destroy(channel);
            ares_library_cleanup();
            return 0;
        }
        memcpy(server_csv, data, size);
        server_csv[size] = '\0';
        
        // Try to set servers from CSV
        ares_set_servers_csv(channel, server_csv);
        
        free(server_csv);
    }

    // Parse input as DNS response and create query
    if (size >= sizeof(unsigned short)) {
        // Extract ID from input data
        unsigned short query_id;
        memcpy(&query_id, data, sizeof(query_id));
        
        // Create a DNS query
        unsigned char *query_buf = nullptr;
        int query_len = 0;
        
        // Use part of input as query name (ensure null termination)
        const size_t name_len = size > sizeof(query_id) ? 
            (size - sizeof(query_id)) : 0;
        char *query_name = nullptr;
        
        if (name_len > 0) {
            query_name = static_cast<char*>(malloc(name_len + 1));
            if (query_name) {
                memcpy(query_name, data + sizeof(query_id), name_len);
                query_name[name_len] = '\0';
                
                // Create DNS query
                ares_mkquery(query_name, 
                           ns_c_in,          // DNS class IN
                           ns_t_a,           // Query type A
                           query_id,         // Use extracted ID
                           1,                // Recursion desired
                           &query_buf, 
                           &query_len);
                
                free(query_name);
            }
        }
        
        if (query_buf) {
            // Parse the query buffer back as DNS record
            ares_dns_record_t *dnsrec = nullptr;
            ares_status_t status = ares_dns_parse(query_buf, query_len, fuzz_uint32_t_1, &dnsrec);
            
            if (status == ARES_SUCCESS && dnsrec) {
                // Use ares_dns_record_get_id
                unsigned short parsed_id = ares_dns_record_get_id(dnsrec);
                
                // Use ares_dns_rr_key_tostr (using ARES_RR_A as example)
                // Note: We'll need to define the key value since it's not in provided APIs
                // Using a reasonable default
                const char *key_str = nullptr;
                
                // Use ares_dns_opt_get_name (using example values)
                // Using ARES_RR_OPT and opt code 8 (EDNS0 client subnet)
                const char *opt_name = ares_dns_opt_get_name(
                    static_cast<ares_dns_rr_key_t>(41),  // ARES_RR_OPT
                    fuzz_uint16_t_2                                     // EDNS0 client subnet option
                );
                
                ares_dns_record_destroy(dnsrec);
            }
            
            free(query_buf);
        }
    }

    // Use ares_gethostbyaddr for reverse DNS lookup
    if (size >= sizeof(struct in_addr)) {
        struct in_addr addr;
        memcpy(&addr, data, sizeof(addr));
        
        // Define a callback function
        auto host_callback = [](void *arg, int status, int timeouts, 
                               const struct hostent *hostent) {
            // Callback implementation
            // We don't need to do anything in fuzzing context
        };
        
        // Perform reverse lookup
        ares_gethostbyaddr(channel, &addr, sizeof(addr), fuzz_int32_t_3, 
                          host_callback, nullptr);
        
        // Process pending requests
        struct timeval tv = {0, 0};
        fd_set read_fds, write_fds;
        
        while (true) {
            int nfds = ares_fds(channel, &read_fds, &write_fds);
            if (nfds == 0) {
                break;
            }
            
            FD_ZERO(&read_fds);
            FD_ZERO(&write_fds);
            nfds = ares_fds(channel, &read_fds, &write_fds);
            
            struct timeval *timeout = ares_timeout(channel, nullptr, &tv);
            
            // We can't actually wait for sockets in fuzzer, so just process
            ares_process(channel, &read_fds, &write_fds);
        }
    }

    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
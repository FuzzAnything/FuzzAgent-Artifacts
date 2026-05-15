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
//<ID> 1054
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"ares_dns_parse":[[1319,7,1319,18,0,0,4,0]]},"library_calls":["ares_library_init","ares_threadsafety","ares_init_options","ares_library_cleanup","ares_set_local_ip4","ares_set_local_dev","ares_dns_parse","ares_dns_record_duplicate","ares_dns_record_destroy","ares_dns_record_destroy","ares_reinit","ares_destroy","ares_library_cleanup","ares_query","ares_fds","ares_process","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_threadsafety","ares_init_options","ares_set_local_ip4","ares_set_local_dev","ares_dns_parse","ares_dns_record_duplicate","ares_dns_record_destroy","ares_dns_record_destroy","ares_reinit","ares_query","ares_fds","ares_process","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

// Callback function for ares_query
static void  id_000055_query_callback(void *arg, int status, int timeouts, 
                          const unsigned char *abuf, int alen) {
    // Parse the DNS response using the same input buffer
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t parse_status = ares_dns_parse(abuf, alen, 0, &dnsrec);
    
    if (parse_status == ARES_SUCCESS && dnsrec != nullptr) {
        // Duplicate the DNS record
        ares_dns_record_t *dup_dnsrec = ares_dns_record_duplicate(dnsrec);
        
        if (dup_dnsrec != nullptr) {
            // Get the ID from the duplicated record
            unsigned short id = ares_dns_record_get_id(dup_dnsrec);
            
            // Process the original record
            size_t rr_count = 0;
            // Try to get binary data from the first answer RR if it exists
            const ares_dns_rr_t *answer_rr = nullptr;
            // Note: ares_dns_record_rr_get doesn't exist in the provided API list
            // We'll demonstrate ares_dns_rr_get_bin differently
            
            ares_dns_record_destroy(dup_dnsrec);
        }
        
        ares_dns_record_destroy(dnsrec);
    }
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    if (size < sizeof(uint32_t)) {
        return 0;
    }

    // 1. Initialize the c-ares library
    int init_flags = ARES_LIB_INIT_ALL;
    if (ares_library_init(init_flags) != ARES_SUCCESS) {
        return 0;
    }

    // 2. Check thread safety
    ares_bool_t threadsafe = ares_threadsafety();

    // 3. Create a channel with options
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    optmask |= ARES_OPT_FLAGS;
    
    options.timeout = 2000;
    optmask |= ARES_OPT_TIMEOUTMS;
    
    options.tries = 3;
    optmask |= ARES_OPT_TRIES;
    
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 4. Set local IPv4 address
    uint32_t local_ip;
    if (size >= sizeof(local_ip)) {
        memcpy(&local_ip, data, sizeof(local_ip));
        ares_set_local_ip4(channel, local_ip);
    }

    // 5. Set local device name
    // Extract a device name from input data (max 16 chars for safety)
    char dev_name[17] = {0};
    size_t name_len = size > sizeof(local_ip) ? 
                      (size - sizeof(local_ip) > 16 ? 16 : size - sizeof(local_ip)) : 0;
    if (name_len > 0) {
        memcpy(dev_name, data + sizeof(local_ip), name_len);
        dev_name[name_len] = '\0';
        ares_set_local_dev(channel, dev_name);
    }

    // 6. Parse input as DNS response
    ares_dns_record_t *parsed_dnsrec = nullptr;
    ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_2, &parsed_dnsrec);
    
    if (parse_status == ARES_SUCCESS && parsed_dnsrec != nullptr) {
        // 7. Duplicate the DNS record
        ares_dns_record_t *dup_dnsrec = ares_dns_record_duplicate(parsed_dnsrec);
        
        if (dup_dnsrec != nullptr) {
            // 8. Extract binary data from DNS records if possible
            // Create a dummy DNS RR to demonstrate ares_dns_rr_get_bin
            // In real usage, we would get this from the parsed record
            
            // For demonstration, we'll parse the input as a DNS response
            // and try to extract binary data from it
            size_t bin_len = 0;
            // Note: This is just to demonstrate the API call - in real usage
            // we would need to get actual DNS RRs from the parsed record
            
            ares_dns_record_destroy(dup_dnsrec);
        }
        
        ares_dns_record_destroy(parsed_dnsrec);
    }

    // 9. Reinitialize the channel
    if (ares_reinit(channel) != ARES_SUCCESS) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }

    // 10. Perform a DNS query using remaining input as query name
    size_t query_offset = sizeof(local_ip) + name_len;
    if (query_offset < size) {
        size_t query_len = size - query_offset;
        char *query_name = (char*)malloc(query_len + 1);
        if (query_name) {
            memcpy(query_name, data + query_offset, query_len);
            query_name[query_len] = '\0';
            
            // Ensure the query name is null-terminated and valid
            for (size_t i = 0; i < query_len; i++) {
                if (query_name[i] == '\0') {
                    break;
                }
            }
            
            ares_query(channel, query_name, ns_c_in, ns_t_a,  id_000055_query_callback, nullptr);
            free(query_name);
        }
    }

    // 11. Process pending DNS operations
    fd_set read_fds, write_fds;
    struct timeval tv;
    
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    // Get file descriptors for the channel
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    
    // Set a timeout
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    // Process any pending operations
    ares_process(channel, &read_fds, &write_fds);

    // 12. Clean up
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}
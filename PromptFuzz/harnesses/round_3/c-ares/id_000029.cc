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
//<ID> 537
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 11
//<Quality> {"density":8,"unique_branches":{"ares_gethostbyname":[[105,7,105,22,0,0,4,1],[109,7,109,16,0,0,4,1],[118,7,118,16,0,0,4,1]],"ares_freeaddrinfo":[[59,7,59,17,0,0,4,0]],"ares_gethostbyname_callback":[[69,7,69,29,0,0,4,1],[76,7,76,29,0,0,4,1],[81,7,81,29,0,0,4,1]],"invoke_server_state_cb":[[89,7,89,39,0,0,4,1],[94,7,94,18,0,0,4,1],[99,7,99,29,0,0,4,1],[106,7,106,28,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_server_state_callback","ares_parse_ptr_reply","ares_free_hostent","ares_parse_caa_reply","ares_free_data","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_keys","ares_dns_rr_get_type","ares_dns_rr_get_abin_cnt","ares_dns_record_destroy","ares_gethostbyname","ares_getaddrinfo","ares_fds","ares_process","ares_process_pending_write","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_server_state_callback","ares_parse_ptr_reply","ares_free_hostent","ares_parse_caa_reply","ares_free_data","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_keys","ares_dns_rr_get_type","ares_dns_rr_get_abin_cnt","ares_dns_record_destroy","ares_gethostbyname","ares_getaddrinfo","ares_fds","ares_process","ares_process_pending_write","ares_destroy","ares_library_cleanup"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified c-ares APIs to parse DNS response data and perform various DNS operations:

*/


// Callback functions
static void  id_000029_server_state_callback(const char *server, ares_bool_t up, int rtt, void *user_data) {
    // Dummy callback for server state changes
}

static void  id_000029_host_callback(void *arg, int status, int timeouts, const struct hostent *host) {
    // Dummy callback for host lookups
}

static void  id_000029_addrinfo_callback(void *arg, int status, int timeouts, struct ares_addrinfo *ai) {
    // Dummy callback for addrinfo lookups
}

// Global channel for callbacks
static ares_channel_t * id_000029_g_channel = nullptr;

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
	//fuzzer shim end}




    // 1. Initialize library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // 2. Create channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
     id_000029_g_channel = channel;
    
    // 3. Set server state callback
    ares_set_server_state_callback(channel,  id_000029_server_state_callback, nullptr);
    
    // Parse input as PTR reply
    struct hostent *host_ptr = nullptr;
    if (size >= sizeof(struct in_addr)) {
        // Create a dummy address for PTR lookup
        struct in_addr addr;
        memcpy(&addr, data, sizeof(struct in_addr));
        
        ares_parse_ptr_reply(data, size, &addr, sizeof(addr), fuzz_int32_t_1, &host_ptr);
        
        if (host_ptr) {
            ares_free_hostent(host_ptr);
        }
    }
    
    // Parse input as CAA reply
    struct ares_caa_reply *caa_out = nullptr;
    if (size > 0) {
        ares_parse_caa_reply(data, size, &caa_out);
        
        if (caa_out) {
            ares_free_data(caa_out);
        }
    }
    
    // 4. Create DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, 
                                                 fuzz_uint16_t_2,      // id
                                                 fuzz_uint16_t_3,         // flags
                                                 ARES_OPCODE_QUERY, 
                                                 ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // 5. Add query to DNS record
        status = ares_dns_record_query_add(dnsrec, fuzz_str_4, 
                                          ARES_REC_TYPE_A, 
                                          ARES_CLASS_IN);
        
        // 6. Set ID (using the function from provided list)
        // Note: This function wasn't in the exported list, so we'll skip it
        // and use create with the right ID instead
        
        // 7. Get count of resource records
        size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
        
        // 8. Get resource record if available
        if (rr_count > 0) {
            ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_5);
            if (rr) {
                // 9. Get keys for record type
                size_t key_cnt = 0;
                const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(ares_dns_rr_get_type(rr), &key_cnt);
                
                // 10. Get binary count for a key if available
                if (key_cnt > 0) {
                    size_t abin_cnt = ares_dns_rr_get_abin_cnt(rr, keys[0]);
                }
            }
        }
        
        // 11. Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // 12. Perform gethostbyname (async)
    if (size > 0) {
        // Use first non-null byte as part of domain name
        char domain[256] = "test";
        if (size > 1 && data[0] != 0) {
            snprintf(domain, sizeof(domain), "test%u.com", (unsigned int)data[0]);
        }
        ares_gethostbyname(channel, fuzz_str_6, fuzz_int32_t_7,  id_000029_host_callback, nullptr);
    }
    
    // 13. Perform getaddrinfo (async)
    if (size > 0) {
        struct ares_addrinfo_hints hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        
        char node[256] = "example";
        if (size > 2 && data[1] != 0) {
            snprintf(node, sizeof(node), "node%u", (unsigned int)data[1]);
        }
        
        ares_getaddrinfo(channel, node, fuzz_str_8, &hints,  id_000029_addrinfo_callback, nullptr);
    }
    
    // 14. Process fds
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    
    if (nfds > 0) {
        // Set timeout to 0 for immediate return
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        
        // Use select to wait for events
        select(nfds, &read_fds, &write_fds, nullptr, &tv);
        
        // 15. Process events
        ares_process(channel, &read_fds, &write_fds);
    }
    
    // 16. Process pending write
    ares_process_pending_write(channel);
    
    // 17. Clean up
    ares_destroy(channel);
    ares_library_cleanup();
     id_000029_g_channel = nullptr;
    
    return 0;
}
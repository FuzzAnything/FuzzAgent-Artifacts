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
//<ID> 141
//<Prompt> []
/*<Combination>: [
*/
//<score> 27.5, nr_unique_branch: 5
//<Quality> {"density":11,"unique_branches":{"ares_parse_txt_reply_int":[[80,26,80,28,0,0,4,1],[95,11,95,13,0,0,4,1]],"ares_free_data":[[84,7,84,35,0,0,4,0]],"ares_malloc_data":[[151,5,151,33,0,0,4,0]],"ares_parse_txt_reply":[[129,7,129,15,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_parse_txt_reply","ares_parse_ptr_reply","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_duplicate","ares_dns_record_set_id","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_get_abin_cnt","ares_dns_record_destroy","ares_dns_record_destroy","ares_getaddrinfo","ares_fds","ares_process","ares_free_data","ares_free_hostent","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_parse_txt_reply","ares_parse_ptr_reply","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_duplicate","ares_dns_record_set_id","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_get_abin_cnt","ares_dns_record_destroy","ares_dns_record_destroy","ares_getaddrinfo","ares_fds","ares_process","ares_free_data","ares_free_hostent","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" {
}

// Callback for ares_getaddrinfo
static void  id_000009_getaddrinfo_callback(void* arg, int status, int timeouts, struct ares_addrinfo* ai) {
    (void)arg;
    (void)timeouts;
    
    if (ai != nullptr) {
        ares_freeaddrinfo(ai);
    }
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<44) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Create a channel
    ares_channel_t* channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // 1. Parse TXT reply from input data
    struct ares_txt_reply* txt_reply = nullptr;
    if (size > 0) {
        ares_parse_txt_reply(data, static_cast<int>(size), &txt_reply);
    }
    
    // 2. Parse PTR reply from input data
    struct hostent* host_ptr = nullptr;
    if (size > 0) {
        // Use a dummy IPv4 address for PTR lookup
        struct in_addr dummy_addr;
        dummy_addr.s_addr = htonl(0x7f000001); // 127.0.0.1
        ares_parse_ptr_reply(data, static_cast<int>(size), 
                            &dummy_addr, sizeof(dummy_addr), 
                            fuzz_int32_t_1, &host_ptr);
    }
    
    // 3. Create a DNS record and duplicate it
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, 
                                                fuzz_uint16_t_2,  // ID
                                                fuzz_uint16_t_3,      // Flags
                                                ARES_OPCODE_QUERY,
                                                ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dnsrec != nullptr) {
        // Add a query section
        ares_dns_record_query_add(dnsrec, fuzz_str_4, 
                                 ARES_REC_TYPE_A, ARES_CLASS_IN);
        
        // 4. Duplicate the DNS record
        ares_dns_record_t* duplicated = ares_dns_record_duplicate(dnsrec);
        
        if (duplicated != nullptr) {
            // 5. Set a new ID on the duplicated record
            ares_dns_record_set_id(duplicated, fuzz_uint16_t_5);
            
            // Get RR from answer section (if any exists)
            size_t rr_count = ares_dns_record_rr_cnt(duplicated, ARES_SECTION_ANSWER);
            if (rr_count > 0) {
                // Get first RR from answer section
                ares_dns_rr_t* rr = ares_dns_record_rr_get(duplicated, 
                                                          ARES_SECTION_ANSWER, fuzz_uint64_t_6);
                if (rr != nullptr) {
                    // 6. Get abin count for various keys
                    // First get available keys for this RR type
                    ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                    size_t key_count = 0;
                    const ares_dns_rr_key_t* keys = ares_dns_rr_get_keys(rr_type, &key_count);
                    
                    // Try to get abin count for each key
                    for (size_t i = 0; i < key_count; i++) {
                        size_t abin_cnt = ares_dns_rr_get_abin_cnt(rr, keys[i]);
                        (void)abin_cnt; // Use result to avoid unused variable warning
                    }
                }
            }
            
            ares_dns_record_destroy(duplicated);
        }
        
        ares_dns_record_destroy(dnsrec);
    }
    
    // 7. Perform getaddrinfo lookup (asynchronous)
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    ares_getaddrinfo(channel, 
                    fuzz_str_7,  // Node to lookup
                    nullptr,      // Service
                    &hints, 
                     id_000009_getaddrinfo_callback, 
                    nullptr);
    
    // Process the channel to handle the async query
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    
    if (nfds > 0) {
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        
        ares_process(channel, &read_fds, &write_fds);
    }
    
    // Clean up TXT reply if allocated
    if (txt_reply != nullptr) {
        ares_free_data(txt_reply);
    }
    
    // Clean up PTR reply if allocated
    if (host_ptr != nullptr) {
        ares_free_hostent(host_ptr);
    }
    
    // Clean up channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();
    
    return 0;
}
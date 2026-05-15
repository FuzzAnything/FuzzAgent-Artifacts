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
//<ID> 325
//<Prompt> []
/*<Combination>: [
*/
//<score> 35, nr_unique_branch: 9
//<Quality> {"density":14,"unique_branches":{"ares_open_connection":[[399,7,399,29,0,0,4,0]],"default_abind":[[418,7,418,35,0,0,4,1]],"ares_socket_configure":[[271,28,271,46,0,0,4,0],[288,7,288,14,0,0,4,0],[288,18,288,51,0,0,4,0],[290,9,290,15,0,0,4,1],[293,9,294,67,0,0,4,0],[293,9,294,67,0,0,4,1]],"ares_search":[[455,18,455,51,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_local_ip4","ares_dns_record_create","ares_dns_record_set_id","ares_dns_record_query_add","ares_strerror","ares_parse_a_reply","ares_free_data","ares_strerror","ares_parse_a_reply","ares_free_data","ares_search","ares_fds","ares_process","ares_queue_wait_empty","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_bin","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_local_ip4","ares_dns_record_create","ares_dns_record_set_id","ares_dns_record_query_add","ares_free_data","ares_parse_a_reply","ares_strerror","ares_free_data","ares_parse_a_reply","ares_strerror","ares_search","ares_fds","ares_process","ares_queue_wait_empty","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_bin","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    ares_channel_t* channel = nullptr;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    
    // Initialize channel with options
    if (ares_init_options(&channel, &options, 0) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set local IP4 using first 4 bytes of input data
    unsigned int local_ip = 0;
    if (size >= 4) {
        memcpy(&local_ip, data, 4);
    }
    ares_set_local_ip4(channel, local_ip);

    // Create DNS record
    ares_dns_record_t* dnsrec = nullptr;
    unsigned short id = 0;
    if (size >= 6) {
        memcpy(&id, data + 4, 2);
    }
    
    if (ares_dns_record_create(&dnsrec, id, 
                              ARES_FLAG_RD,  // Query flag
                              ARES_OPCODE_QUERY, 
                              ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        
        // Set DNS record ID
        ares_dns_record_set_id(dnsrec, id);
        
        // Add a query section
        const char* query_name = "fuzz.example.com";
        if (ares_dns_record_query_add(dnsrec, query_name, 
                                     ARES_REC_TYPE_A, 
                                     ARES_CLASS_IN) == ARES_SUCCESS) {
            
            // Prepare callback for search
            struct CallbackData {
                const uint8_t* fuzz_data;
                size_t fuzz_size;
                bool callback_called = false;
            };
            
            CallbackData cb_data;
            cb_data.fuzz_data = data;
            cb_data.fuzz_size = size;
            
            // Define callback function
            auto callback = [](void* arg, int status, int timeouts,
                              const unsigned char* abuf, int alen) {
                CallbackData* cb_data = static_cast<CallbackData*>(arg);
                cb_data->callback_called = true;
                
                // Try to parse the response (even though we won't get one in fuzzing)
                if (status == ARES_SUCCESS && abuf && alen > 0) {
                    // Use ares_strerror to convert status code to string
                    const char* error_str = ares_strerror(status);
                    (void)error_str; // Use variable to avoid warning
                    
                    // Try to parse as A record reply
                    struct hostent* host = nullptr;
                    struct ares_addrttl addrttls[10];
                    int naddrttls = 10;
                    ares_parse_a_reply(abuf, alen, &host, addrttls, &naddrttls);
                    
                    if (host) {
                        ares_free_data(host);
                    }
                }
            };
            
            // Perform DNS search
            ares_search(channel, query_name, 
                       ARES_CLASS_IN, 
                       ARES_REC_TYPE_A, 
                       callback, &cb_data);
            
            // Process any pending operations
            fd_set read_fds, write_fds;
            FD_ZERO(&read_fds);
            FD_ZERO(&write_fds);
            
            int max_fd = ares_fds(channel, &read_fds, &write_fds);
            if (max_fd > 0) {
                struct timeval tv;
                tv.tv_sec = 0;
                tv.tv_usec = 0;
                
                ares_process(channel, &read_fds, &write_fds);
            }
            
            // Wait for queue to empty (with short timeout)
            ares_queue_wait_empty(channel, 10); // 10ms timeout
            
            // If we have enough data, try to get binary data from DNS record
            if (size >= 20) {
                // Get the first answer record if exists
                size_t answer_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
                if (answer_count > 0) {
                    ares_dns_rr_t* rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, 0);
                    if (rr) {
                        size_t len = 0;
                        const unsigned char* bin_data = ares_dns_rr_get_bin(
                            rr, ARES_RR_A_ADDR, &len);
                        (void)bin_data; // Use variable to avoid warning
                        (void)len;      // Use variable to avoid warning
                    }
                }
            }
        }
        
        // Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // Clean up options (though we didn't allocate any special options)
    // Note: ares_destroy_options is not in the provided API list, but we'll
    // simulate it by zeroing out the options struct
    memset(&options, 0, sizeof(options));
    
    // Destroy channel
    ares_destroy(channel);
    
    // Cleanup library
    ares_library_cleanup();
    
    return 0;
}
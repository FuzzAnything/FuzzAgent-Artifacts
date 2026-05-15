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
//<ID> 898
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 3
//<Quality> {"density":14,"unique_branches":{"ares_hosts_entry_to_hostent":[[943,7,943,29,0,0,4,0],[954,7,954,29,0,0,4,0]],"ares_gethostbyname_file_int":[[317,7,317,29,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_save_options","ares_set_server_state_callback","ares_set_socket_callback","ares_parse_uri_reply","ares_free_string","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_addr","ares_dns_rr_set_opt","ares_gethostbyname_file","ares_free_hostent","ares_dns_record_destroy","ares_query","ares_search","ares_fds","ares_process_fd","ares_process_fd","ares_process_fd","ares_queue_wait_empty","ares_destroy_options","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_save_options","ares_set_server_state_callback","ares_set_socket_callback","ares_parse_uri_reply","ares_free_string","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_addr","ares_dns_rr_set_opt","ares_gethostbyname_file","ares_free_hostent","ares_dns_record_destroy","ares_query","ares_search","ares_fds","ares_process_fd","ares_process_fd","ares_queue_wait_empty","ares_destroy_options","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


// c-ares headers

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<30) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    ares_dns_record_t *dnsrec = nullptr;
    ares_dns_rr_t *rr = nullptr;
    struct ares_uri_reply *uri_reply = nullptr;
    struct hostent *hostent_ptr = nullptr;
    
    // Initialize channel
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Save current options
    if (ares_save_options(channel, &options, &optmask) == ARES_SUCCESS) {
        // We'll use these later
    }
    
    // Set server state callback
    ares_set_server_state_callback(channel, [](const char *server, ares_bool_t is_up, int rtt, void *user_data) {
        // Callback implementation
    }, nullptr);
    
    // Set socket callback
    ares_set_socket_callback(channel, [](ares_socket_t socket_fd, int type, void *user_data) -> int {
        return 0;
    }, nullptr);
    
    // Parse URI reply from input data if we have enough
    if (size >= 12) { // Minimum DNS header size
        ares_parse_uri_reply(data, (int)size, &uri_reply);
        
        // Free URI reply if parsed
        if (uri_reply) {
            // Free the linked list
            struct ares_uri_reply *current = uri_reply;
            while (current) {
                struct ares_uri_reply *next = current->next;
                ares_free_string(current->uri);
                free(current);
                current = next;
            }
        }
    }
    
    // Create DNS record
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_1, ARES_FLAG_RD, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        // Add a resource record
        if (ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                   fuzz_str_2, ARES_REC_TYPE_A, 
                                   ARES_CLASS_IN, fuzz_uint32_t_3) == ARES_SUCCESS) {
            // Set address for the RR if we have enough data
            if (size >= sizeof(struct in_addr)) {
                struct in_addr addr;
                memcpy(&addr, data, sizeof(struct in_addr));
                ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr);
            }
            
            // Set an OPT option if we have enough data
            if (size >= 4) {
                ares_dns_rr_set_opt(rr, ARES_RR_OPT_OPTIONS, fuzz_uint16_t_4, data, size > 100 ? 100 : size);
            }
        }
        
        // Get hostent from file for comparison
        if (size > 0) {
            // Create a temporary string from data for hostname
            char *hostname = (char*)malloc(size + 1);
            if (hostname) {
                memcpy(hostname, data, size);
                hostname[size] = '\0';
                
                // Try to get hostent from file
                ares_gethostbyname_file(channel, hostname, fuzz_int32_t_5, &hostent_ptr);
                
                // Free hostent if obtained
                if (hostent_ptr) {
                    ares_free_hostent(hostent_ptr);
                }
                
                free(hostname);
            }
        }
        
        // Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Prepare callback for queries
    ares_callback query_callback = [](void *arg, int status, int timeouts, 
                                       const unsigned char *abuf, int alen) {
        // Do nothing in fuzzing context
    };
    
    // Perform query if we have data
    if (size > 0) {
        // Extract query name from data
        char query_name[256] = {0};
        size_t name_len = size < 255 ? size : 255;
        memcpy(query_name, data, name_len);
        query_name[name_len] = '\0';
        
        ares_query(channel, query_name, C_IN, T_A, query_callback, nullptr);
    }
    
    // Perform search if we have data
    if (size > 0) {
        char search_name[256] = {0};
        size_t search_len = size < 255 ? size : 255;
        memcpy(search_name, data, search_len);
        search_name[search_len] = '\0';
        
        ares_search(channel, search_name, C_IN, T_A, query_callback, nullptr);
    }
    
    // Process any pending events
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    
    // Process file descriptors if any
    if (nfds > 0) {
        // We need actual file descriptors, so create temporary ones
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            int fd = fuzz_fileno(in_file);
            
            // Check if our fd is in the sets (it won't be, but we call process_fd anyway)
            ares_process_fd(channel, fd, ARES_SOCKET_BAD);
            
            assert_file_closed(&in_file);;
        }
        
        // Also try with output file descriptor
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            int fd = fuzz_fileno(out_file);
            ares_process_fd(channel, ARES_SOCKET_BAD, fd);
            assert_file_closed(&out_file);;
        }
    } else {
        // Process without valid fds
        ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    }
    
    // Wait for queue to empty with short timeout
    ares_queue_wait_empty(channel, fuzz_int32_t_6); // 1ms timeout
    
    // Clean up
    ares_destroy_options(&options);
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
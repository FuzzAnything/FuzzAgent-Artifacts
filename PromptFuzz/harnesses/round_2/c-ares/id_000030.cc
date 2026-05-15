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
//<ID> 770
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 3
//<Quality> {"density":22,"unique_branches":{"ares_dns_rr_add_abin":[[940,5,940,38,0,0,4,1],[941,39,941,50,0,0,4,1],[945,7,945,59,0,0,4,0]]},"library_calls":["ares_library_init","ares_library_cleanup","ares_init_options","ares_library_cleanup","ares_set_local_ip6","ares_set_pending_write_cb","ares_dns_parse","ares_dns_record_query_cnt","ares_dns_record_query_get","ares_search_dnsrec","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_key_datatype","ares_dns_record_duplicate","ares_dns_rr_add_abin","ares_dns_record_destroy","ares_dns_opt_get_name","ares_dns_record_destroy","ares_parse_mx_reply","ares_free_data","ares_getsock","ares_process_fd","ares_process_pending_write","ares_queue_wait_empty","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_local_ip6","ares_set_pending_write_cb","ares_dns_parse","ares_dns_record_query_cnt","ares_dns_record_query_get","ares_search_dnsrec","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_key_datatype","ares_dns_record_duplicate","ares_dns_rr_add_abin","ares_dns_record_destroy","ares_dns_opt_get_name","ares_dns_record_destroy","ares_parse_mx_reply","ares_free_data","ares_getsock","ares_process_fd","ares_process_pending_write","ares_queue_wait_empty","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Callback for ares_search_dnsrec
static void  id_000030_search_callback(void* arg, ares_status_t status,
                           size_t timeout, const ares_dns_record_t* dnsrec) {
    // Nothing to do in fuzzing context
}

// Callback for pending write
static void  id_000030_pending_write_cb(void* user_data) {
    // Nothing to do in fuzzing context
}

// Callback for server state changes
static void  id_000030_server_state_callback(void* user_data,
                                 const struct sockaddr* addr,
                                 ares_socklen_t addrlen,
                                 int failure_count,
                                 ares_bool_t connected) {
    // Nothing to do in fuzzing context
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    ares_channel_t* channel = nullptr;
    ares_dns_record_t* dnsrec = nullptr;
    struct ares_mx_reply* mx_out = nullptr;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    
    // Create a file from input data for potential use
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize channel with default options
    if (ares_init_options(&channel, &options, 0) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set local IPv6 address (use first 16 bytes if available)
    if (size >= 16) {
        ares_set_local_ip6(channel, data);
    }
    
    // Set pending write callback
    ares_set_pending_write_cb(channel,  id_000030_pending_write_cb, nullptr);
    
    // Try to parse input as DNS record
    if (ares_dns_parse(data, size, 0, &dnsrec) == ARES_SUCCESS) {
        // Get query count
        size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
        
        // Try to get first query if exists
        if (query_cnt > 0) {
            const char* name = nullptr;
            ares_dns_rec_type_t qtype;
            ares_dns_class_t qclass;
            
            if (ares_dns_record_query_get(dnsrec, 0, &name, &qtype, &qclass) == ARES_SUCCESS) {
                // Perform search with the DNS record
                ares_search_dnsrec(channel, dnsrec,  id_000030_search_callback, nullptr);
            }
        }
        
        // Add binary data to a resource record if we have at least one RR
        size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
        if (rr_cnt > 0) {
            const ares_dns_rr_t* rr = ares_dns_record_rr_get_const(dnsrec, 
                                                                  ARES_SECTION_ANSWER, 0);
            if (rr) {
                // Get available keys for this RR type
                ares_dns_rec_type_t type = ares_dns_rr_get_type(rr);
                size_t key_cnt = 0;
                const ares_dns_rr_key_t* keys = ares_dns_rr_get_keys(type, &key_cnt);
                
                // Find a key that accepts binary data
                for (size_t i = 0; i < key_cnt; i++) {
                    if (ares_dns_rr_key_datatype(keys[i]) == ARES_DATATYPE_BIN) {
                        // Create a mutable copy to add binary data
                        ares_dns_record_t* mutable_dnsrec = ares_dns_record_duplicate(dnsrec);
                        if (mutable_dnsrec) {
                            ares_dns_rr_t* mutable_rr = nullptr;
                            // Get the RR to modify
                            // Note: In real usage, we'd need to extract and modify the RR
                            // For fuzzing, we just demonstrate the API call
                            ares_dns_rr_add_abin(const_cast<ares_dns_rr_t*>(rr), 
                                                keys[i], data, 
                                                size > 1024 ? 1024 : size);
                            ares_dns_record_destroy(mutable_dnsrec);
                        }
                        break;
                    }
                }
            }
        }
        
        // Get option name for demonstration
        const char* opt_name = ares_dns_opt_get_name(ARES_RR_A_ADDR, 0);
        (void)opt_name; // Use to avoid unused variable warning
        
        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Try to parse as MX reply
    if (size > 0) {
        ares_parse_mx_reply(data, 
                           size > INT_MAX ? INT_MAX : (int)size, 
                           &mx_out);
        
        // Free MX reply data if allocated
        if (mx_out) {
            ares_free_data(mx_out);
        }
    }
    
    // Get sockets and process them
    ares_socket_t socks[ARES_GETSOCK_MAXNUM];
    int numsocks = ares_getsock(channel, socks, ARES_GETSOCK_MAXNUM);
    
    // Process file descriptor from our input file
    int fd = fuzz_fileno(in_file);
    if (fd >= 0) {
        ares_process_fd(channel, fd, ARES_SOCKET_BAD);
    }
    
    // Process pending writes
    ares_process_pending_write(channel);
    
    // Wait for queue to empty (with minimal timeout for fuzzing)
    ares_queue_wait_empty(channel, 1);
    
    // Cancel any pending operations
    ares_cancel(channel);
    
    // Clean up
    assert_file_closed(&in_file);;
    ares_destroy(channel);
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
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
//<ID> 210
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.3333335, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"ares_array_destroy":[[107,7,107,28,0,0,4,1]]},"library_calls":["ares_library_init","ares_threadsafety","ares_init_options","ares_library_cleanup","ares_set_local_dev","ares_dup","ares_reinit","ares_query","ares_dns_parse","ares_dns_record_get_opcode","ares_dns_record_destroy","ares_query","ares_getsock","ares_fds","ares_process","ares_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_threadsafety","ares_init_options","ares_set_local_dev","ares_dup","ares_reinit","ares_query","ares_dns_parse","ares_dns_record_get_opcode","ares_dns_record_destroy","ares_query","ares_getsock","ares_fds","ares_process","ares_destroy","ares_destroy","ares_library_cleanup"],"visited":2}
/**/


// Include c-ares headers

// Dummy callback functions
static void  id_000012_dummy_callback(void* arg, int status, int timeouts, 
                          const unsigned char* abuf, int alen) {
    // Do nothing - just for fuzzing
}

static void  id_000012_dummy_search_callback(void* arg, int status, int timeouts,
                                 const unsigned char* abuf, int alen) {
    // Do nothing - just for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<40) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Check thread safety
    ares_bool_t thread_safe = ares_threadsafety();

    // Create source channel
    ares_channel_t* src_channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize with default options
    if (ares_init_options(&src_channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set local device name from input data (limited to 255 chars for safety)
    if (size > 0) {
        // Create null-terminated string from input data
        size_t dev_name_len = size > 255 ? 255 : size;
        char* dev_name = (char*)malloc(dev_name_len + 1);
        if (dev_name) {
            memcpy(dev_name, data, dev_name_len);
            dev_name[dev_name_len] = '\0';
            ares_set_local_dev(src_channel, dev_name);
            free(dev_name);
        }
    }

    // Duplicate the channel
    ares_channel_t* dest_channel = nullptr;
    if (ares_dup(&dest_channel, src_channel) == ARES_SUCCESS) {
        // Reinitialize the duplicated channel
        ares_status_t reinit_status = ares_reinit(dest_channel);
        (void)reinit_status; // Use variable to avoid unused warning

        // Try to parse input as SOA reply
        if (size > 0) {
            struct ares_soa_reply* soa_out = nullptr;
            // Note: ares_parse_soa_reply is not in the provided API list,
            // so we'll use a different approach
            // For fuzzing purposes, we'll create a dummy parse attempt
            // using available APIs
            unsigned char* buf_copy = (unsigned char*)malloc(size);
            if (buf_copy) {
                memcpy(buf_copy, data, size);
                // In real usage, we would call ares_parse_soa_reply here
                // For now, just free the buffer
                free(buf_copy);
            }
        }

        // Create a query name from input data
        char query_name[256] = "example.com";
        if (size > 0) {
            // Use first byte to determine query name (modulo 3 for variety)
            switch (data[0] % 3) {
                case 0: strcpy(query_name, "example.com"); break;
                case 1: strcpy(query_name, "test.local"); break;
                case 2: strcpy(query_name, "fuzz.domain"); break;
            }
        }

        // Perform a search (note: ares_search is not in the provided API list)
        // We'll use ares_query instead as it's available
        ares_query(dest_channel, fuzz_str_2, 
                   ns_c_in,  // DNS class IN
                   ns_t_a,   // Record type A
                    id_000012_dummy_callback, nullptr);

        // Try to get DNS record from input for opcode check
        ares_dns_record_t* dnsrec = nullptr;
        if (size > 0) {
            unsigned char* buf_copy = (unsigned char*)malloc(size);
            if (buf_copy) {
                memcpy(buf_copy, data, size);
                
                // Try to parse as DNS record using available API
                ares_status_t parse_status = ares_dns_parse(
                    buf_copy, size, fuzz_uint32_t_3, &dnsrec);
                
                if (parse_status == ARES_SUCCESS && dnsrec != nullptr) {
                    // Get opcode from DNS record
                    ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
                    (void)opcode; // Use variable to avoid unused warning
                    
                    // Clean up DNS record
                    ares_dns_record_destroy(dnsrec);
                }
                
                free(buf_copy);
            }
        }

        // Perform another query with different parameters
        ares_query(dest_channel, fuzz_str_4,
                   ns_c_in,      // DNS class IN
                   ns_t_aaaa,    // Record type AAAA
                    id_000012_dummy_callback, nullptr);

        // Process any pending operations
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        // Get socket descriptors
        ares_socket_t socks[ARES_GETSOCK_MAXNUM];
        int bitmask = ares_getsock(dest_channel, socks, fuzz_int32_t_5);
        
        if (bitmask != 0) {
            // Set up file descriptors based on bitmask
            for (int i = 0; i < ARES_GETSOCK_MAXNUM; i++) {
                if (ARES_GETSOCK_READABLE(bitmask, i)) {
                    FD_SET(socks[i], &read_fds);
                }
                if (ARES_GETSOCK_WRITABLE(bitmask, i)) {
                    FD_SET(socks[i], &write_fds);
                }
            }
            
            // Process with timeout of 0 (non-blocking)
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 0;
            
            int nfds = ares_fds(dest_channel, &read_fds, &write_fds);
            if (nfds > 0) {
                ares_process(dest_channel, &read_fds, &write_fds);
            }
        }

        // Clean up duplicated channel
        ares_destroy(dest_channel);
    }

    // Clean up source channel
    ares_destroy(src_channel);
    
    // Cleanup library
    ares_library_cleanup();

    return 0;
}
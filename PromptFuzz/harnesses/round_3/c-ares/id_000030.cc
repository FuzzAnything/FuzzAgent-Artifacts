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
//<ID> 540
//<Prompt> []
/*<Combination>: [
*/
//<score> 108, nr_unique_branch: 14
//<Quality> {"density":18,"unique_branches":{"ares_dns_rr_get_bin":[[860,9,860,21,0,0,4,1]],"ares_dns_rr_set_bin":[[1264,7,1264,29,0,0,4,1]],"ares_version":[[31,7,31,14,0,0,4,1]],"ares_dns_multistring_combined":[[200,7,200,19,0,0,4,1],[200,23,200,34,0,0,4,1],[207,7,207,31,0,0,4,1],[219,15,219,45,0,0,4,0],[219,15,219,45,0,0,4,1],[221,9,221,21,0,0,4,1],[222,9,222,68,0,0,4,1],[230,7,230,30,0,0,4,1]],"ares_dns_rr_set_bin_own":[[1209,9,1209,21,0,0,4,1],[1213,9,1213,22,0,0,4,0],[1215,11,1215,24,0,0,4,1]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_socket_callback","ares_parse_naptr_reply","ares_free_data","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_cnt","ares_dns_record_query_set_name","ares_dns_record_rr_add","ares_dns_rr_set_bin","ares_dns_rr_get_bin","ares_dns_record_get_rcode","ares_search_dnsrec","ares_fds","ares_process","ares_cancel","ares_dns_record_destroy","ares_version","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_socket_callback","ares_parse_naptr_reply","ares_free_data","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_cnt","ares_dns_record_query_set_name","ares_dns_record_rr_add","ares_dns_rr_set_bin","ares_dns_rr_get_bin","ares_dns_record_get_rcode","ares_search_dnsrec","ares_fds","ares_process","ares_cancel","ares_dns_record_destroy","ares_version","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Dummy callback for ares_search_dnsrec
static void  id_000030_dummy_callback(void* arg, ares_status_t status, size_t timeouts, 
                           const ares_dns_record_t* dnsrec) {
    // Do nothing - just a placeholder for the callback
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
}

// Dummy socket callback that returns invalid socket
static int  id_000030_dummy_sock_create_callback(ares_socket_t socket_fd, int type, void* user_data) {
    (void)socket_fd;
    (void)type;
    (void)user_data;
    return -1; // Return invalid socket
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<67) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    ares_channel_t* channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Set up minimal options
    memset(&options, 0, sizeof(options));
    options.timeout = 1000; // 1 second timeout
    options.tries = 1;
    optmask = ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    // Initialize channel with options
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Set socket callback to prevent actual network I/O
    ares_set_socket_callback(channel,  id_000030_dummy_sock_create_callback, nullptr);
    
    // Parse NAPTR reply from input data if we have enough
    if (size >= 12) { // Minimum DNS header size
        struct ares_naptr_reply* naptr_reply = nullptr;
        int parse_result = ares_parse_naptr_reply(data, (int)size, &naptr_reply);
        
        if (parse_result == ARES_SUCCESS && naptr_reply != nullptr) {
            // Free the parsed NAPTR data
            ares_free_data(naptr_reply);
        }
    }
    
    // Create a DNS record
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t create_status = ares_dns_record_create(&dnsrec, 
                                                         fuzz_uint16_t_2,    // ID
                                                         fuzz_uint16_t_3,        // Flags
                                                         ARES_OPCODE_QUERY, // Opcode
                                                         ARES_RCODE_NOERROR); // Rcode
    
    if (create_status == ARES_SUCCESS && dnsrec != nullptr) {
        // Add a query to the DNS record
        ares_dns_record_query_add(dnsrec, fuzz_str_4, 
                                 ARES_REC_TYPE_A, ARES_CLASS_IN);
        
        // Set query name (for the first query at index 0)
        if (ares_dns_record_query_cnt(dnsrec) > 0) {
            ares_dns_record_query_set_name(dnsrec, fuzz_uint64_t_6, fuzz_str_5);
        }
        
        // Add a resource record (RR) to answer section
        ares_dns_rr_t* rr = nullptr;
        ares_status_t rr_status = ares_dns_record_rr_add(&rr, dnsrec, 
                                                         ARES_SECTION_ANSWER,
                                                         fuzz_str_7,
                                                         ARES_REC_TYPE_TXT,
                                                         ARES_CLASS_IN,
                                                         fuzz_uint32_t_8); // TTL
        
        if (rr_status == ARES_SUCCESS && rr != nullptr) {
            // Set binary data for the TXT record if we have input data
            if (size > 0) {
                // Use up to 255 bytes for TXT data (max for one TXT string)
                size_t txt_len = size > 255 ? 255 : size;
                ares_dns_rr_set_bin(rr, ARES_RR_TXT_DATA, data, txt_len);
            }
            
            // Retrieve the binary data back
            size_t retrieved_len = 0;
            const unsigned char* retrieved_data = ares_dns_rr_get_bin(rr, 
                                                                     ARES_RR_TXT_DATA, 
                                                                     &retrieved_len);
            (void)retrieved_data; // Suppress unused variable warning
        }
        
        // Get RCODE from the DNS record
        ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
        (void)rcode; // Suppress unused variable warning
        
        // Perform search with the DNS record (will fail due to dummy socket callback)
        ares_search_dnsrec(channel, dnsrec,  id_000030_dummy_callback, nullptr);
        
        // Process file descriptors (none will be ready due to dummy callback)
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        int nfds = ares_fds(channel, &read_fds, &write_fds);
        if (nfds > 0) {
            ares_process(channel, &read_fds, &write_fds);
        }
        
        // Cancel any pending operations
        ares_cancel(channel);
        
        // Destroy the DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Get c-ares version
    const char* version_str = ares_version(nullptr);
    (void)version_str; // Suppress unused variable warning
    
    // Use FILE* operations if we have enough data
    if (size > 0) {
        FILE* in_file = fmemopen((void*)data, size, "rb");
        if (in_file != nullptr) {
            // Read a small amount to demonstrate file usage
            char buf[16];
            size_t read = fread(buf, 1, sizeof(buf), in_file);
            (void)read; // Suppress unused variable warning
            
            // Get file descriptor
            int fd = fuzz_fileno(in_file);
            (void)fd; // Suppress unused variable warning
            
            assert_file_closed(&in_file);;
        }
        
        // Create output file
        FILE* out_file = fopen("output_file", "wb");
        if (out_file != nullptr) {
            // Write some data
            const char* test_data = "test";
            fwrite(test_data, 1, 4, out_file);
            
            // Get file descriptor
            int out_fd = fuzz_fileno(out_file);
            (void)out_fd; // Suppress unused variable warning
            
            assert_file_closed(&out_file);;
            
            // Clean up output file
            remove("output_file");
        }
    }
    
    // Destroy channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();
    
    return 0;
}
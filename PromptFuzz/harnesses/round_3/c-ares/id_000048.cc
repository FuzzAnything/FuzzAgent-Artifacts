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
//<ID> 871
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.5, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_set_ttl","ares_dns_opt_get_name","ares_queue_active_queries","ares_create_query","ares_send","ares_getaddrinfo","ares_queue_active_queries","ares_dns_record_destroy","ares_send","ares_fds","ares_process","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_set_ttl","ares_dns_opt_get_name","ares_queue_active_queries","ares_create_query","ares_send","ares_getaddrinfo","ares_queue_active_queries","ares_dns_record_destroy","ares_fds","ares_process","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


// Include necessary c-ares headers

// Callback function for ares_send
static void  id_000048_send_callback(void* arg, int status, int timeouts,
                         const unsigned char* abuf, int alen) {
    // For fuzzing, we don't need to do anything with the response
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

// Callback function for ares_getaddrinfo
static void  id_000048_addrinfo_callback(void* arg, int status, int timeouts,
                             struct ares_addrinfo* result) {
    // For fuzzing, we don't need to process the result
    (void)arg;
    (void)status;
    (void)timeouts;
    
    if (result) {
        ares_freeaddrinfo(result);
    }
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<71) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t* channel = nullptr;
    if (ares_init_options(&channel, nullptr, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Create a DNS record from input data
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_2, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Get the first resource record from answer section if available
        size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
        if (rr_cnt > 0) {
            ares_dns_rr_t* rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_3);
            if (rr) {
                // Set TTL for the resource record (using a fixed value for fuzzing)
                ares_dns_rr_set_ttl(rr, fuzz_uint32_t_4);
                
                // Get option name for a specific key (example: using ARES_RR_A_ADDR)
                const char* opt_name = ares_dns_opt_get_name(ARES_RR_A_ADDR, fuzz_uint16_t_5);
                (void)opt_name; // Use the value to avoid compiler warnings
            }
        }
        
        // Get the active query count before sending
        size_t active_queries_before = ares_queue_active_queries(channel);
        (void)active_queries_before; // Use the value
        
        // Create a DNS query
        unsigned char* query_buf = nullptr;
        int query_len = 0;
        
        // Use a fixed domain name for query creation
        const char* test_domain = "example.com";
        int rc = ares_create_query(fuzz_str_6, C_IN, T_A, fuzz_uint16_t_7, fuzz_int32_t_8, 
                                   &query_buf, &query_len, fuzz_int32_t_9);
        
        if (rc == ARES_SUCCESS && query_buf && query_len > 0) {
            // Send the query
            ares_send(channel, query_buf, fuzz_int32_t_10,  id_000048_send_callback, nullptr);
            
            // Free the query buffer
            free(query_buf);
        }
        
        // Prepare hints for getaddrinfo
        struct ares_addrinfo_hints hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        
        // Perform getaddrinfo lookup
        ares_getaddrinfo(channel, fuzz_str_11, fuzz_str_12, &hints, 
                         id_000048_addrinfo_callback, nullptr);
        
        // Get active query count after sending
        size_t active_queries_after = ares_queue_active_queries(channel);
        (void)active_queries_after; // Use the value
        
        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    } else {
        // If parsing fails, try to use the data as a raw DNS query
        if (size > 0) {
            // Send the raw data as a DNS query
            ares_send(channel, data, static_cast<int>(size),  id_000048_send_callback, nullptr);
        }
    }
    
    // Process pending events (non-blocking)
    fd_set read_fds, write_fds;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    if (nfds > 0) {
        ares_process(channel, &read_fds, &write_fds);
    }
    
    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
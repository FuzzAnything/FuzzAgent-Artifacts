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
//<ID> 954
//<Prompt> []
/*<Combination>: [
*/
//<score> 35, nr_unique_branch: 6
//<Quality> {"density":7,"unique_branches":{"ares_dns_write_header":[[94,29,94,70,0,0,4,1]],"ares_dns_write_rr_soa":[[360,7,360,29,0,0,4,0]],"ares_dns_rec_type_tostr":[[216,5,216,27,0,0,4,0],[222,5,222,29,0,0,4,0]],"ares_dns_get_opt_rr_const":[[1553,15,1553,71,0,0,4,0],[1557,9,1557,54,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_dns_record_create","ares_dns_record_get_id","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_opt_cnt","ares_dns_opt_get_name","ares_send","ares_queue_active_queries","ares_fds","ares_process","ares_getaddrinfo","ares_strerror","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_dns_record_create","ares_dns_record_get_id","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_opt_cnt","ares_dns_opt_get_name","ares_send","ares_queue_active_queries","ares_fds","ares_process","ares_getaddrinfo","ares_strerror","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

// Callback functions
static void  id_000057_getaddrinfo_callback(void *arg, int status, int timeouts, 
                                 struct ares_addrinfo *result) {
    (void)arg;
    (void)status;
    (void)timeouts;
    // Minimal processing - just ensure we handle the result
    if (result) {
        ares_freeaddrinfo(result);
    }
}

static void  id_000057_send_callback(void *arg, int status, int timeouts, 
                          const unsigned char *abuf, int alen) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
    // Do nothing - just satisfy callback requirement
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize channel with default options
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Create a file from input data for potential use
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for select()
    int fd = fuzz_fileno(in_file);
    
    // Create a DNS record for parsing
    ares_dns_record_t *dnsrec = nullptr;
    
    // Try to parse input data as DNS response
    // Note: We need to create a proper DNS query first to get a valid response format
    // For fuzzing, we'll attempt to parse anyway
    
    // Create a simple DNS query record
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_2, fuzz_uint16_t_3, ARES_OPCODE_QUERY, 
                              ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        
        // Get record ID
        unsigned short id = ares_dns_record_get_id(dnsrec);
        (void)id;
        
        // Get RR count for each section
        for (int sect = ARES_SECTION_ANSWER; sect <= ARES_SECTION_ADDITIONAL; sect++) {
            size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, (ares_dns_section_t)sect);
            
            // Try to get each RR if they exist
            for (size_t idx = 0; idx < rr_cnt; idx++) {
                ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, (ares_dns_section_t)sect, fuzz_uint64_t_4);
                if (rr) {
                    // Get OPT count if this is an OPT record
                    size_t opt_cnt = ares_dns_rr_get_opt_cnt(rr, ARES_RR_OPT_OPTIONS);
                    
                    // Try to get OPT names
                    for (size_t opt_idx = 0; opt_idx < opt_cnt; opt_idx++) {
                        const char *opt_name = ares_dns_opt_get_name(ARES_RR_OPT_OPTIONS, 
                                                                    fuzz_uint16_t_5);
                        (void)opt_name;
                    }
                }
            }
        }
    }
    
    // Send a DNS query (using part of input data as query buffer if available)
    if (size > 12) {  // Minimum DNS header size
        // Create a simple query (A record for example.com)
        unsigned char query[512];
        int qlen = 0;
        // Use input data to influence query construction
        size_t query_size = size > sizeof(query) ? sizeof(query) : size;
        memcpy(query, data, query_size);
        qlen = (int)query_size;
        
        ares_send(channel, query, fuzz_int32_t_6,  id_000057_send_callback, nullptr);
    }
    
    // Get active query count
    size_t active_queries = ares_queue_active_queries(channel);
    (void)active_queries;
    
    // Get file descriptors for select
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    
    // Add our input file descriptor to read set
    if (fd >= 0) {
        FD_SET(fd, &read_fds);
        if (fd >= nfds) {
            nfds = fd + 1;
        }
    }
    
    // Set timeout
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;  // 1ms timeout
    
    // Wait for events
    int select_result = select(nfds, &read_fds, &write_fds, nullptr, &tv);
    
    if (select_result > 0) {
        // Process events
        ares_process(channel, &read_fds, &write_fds);
    }
    
    // Perform getaddrinfo query (using input data as hostname if possible)
    if (size > 0) {
        // Create null-terminated hostname from input data
        char *hostname = (char *)malloc(size + 1);
        if (hostname) {
            memcpy(hostname, data, size);
            hostname[size] = '\0';
            
            // Limit hostname length to reasonable size
            if (size > 255) {
                hostname[255] = '\0';
            }
            
            struct ares_addrinfo_hints hints;
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            
            ares_getaddrinfo(channel, hostname, nullptr, &hints, 
                             id_000057_getaddrinfo_callback, nullptr);
            
            free(hostname);
        }
    }
    
    // Get error string for a sample error code
    const char *error_str = ares_strerror(ARES_ENODATA);
    (void)error_str;
    
    // Cleanup
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    assert_file_closed(&in_file);;
    ares_destroy(channel);
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
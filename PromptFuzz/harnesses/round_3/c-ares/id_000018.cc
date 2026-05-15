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
//<ID> 347
//<Prompt> []
/*<Combination>: [
*/
//<score> 46.75, nr_unique_branch: 21
//<Quality> {"density":17,"unique_branches":{"ares_send_nolock":[[151,5,151,39,0,0,4,0]],"ares_fetch_connection":[[1197,7,1197,23,0,0,4,0]],"ares_open_connection":[[350,35,350,41,0,0,4,0],[364,27,364,33,0,0,4,0],[378,7,378,39,0,0,4,0],[398,5,398,39,0,0,4,0],[404,7,404,39,0,0,4,0],[405,7,405,73,0,0,4,1]],"default_asocket":[[235,7,235,26,0,0,4,0],[245,9,246,10,0,0,4,1]],"ares_socket_enable_tfo":[[217,7,219,71,0,0,4,1]],"default_asetsockopt":[[277,11,277,14,0,0,4,1],[278,5,278,38,0,0,4,1],[285,5,285,38,0,0,4,1],[292,5,292,37,0,0,4,1],[306,5,306,38,0,0,4,0],[307,11,307,42,0,0,4,1]],"ares_conn_connect":[[332,5,332,39,0,0,4,0]],"ares_conn_set_sockaddr":[[92,5,92,37,0,0,4,0]],"server_increment_failures":[[135,26,135,47,0,0,4,0]],"ares_socket_connect":[[367,7,367,13,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_parse_naptr_reply","ares_free_data","ares_destroy","ares_library_cleanup","ares_gethostbyname_file","ares_free_hostent","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_bin","ares_dns_rr_get_bin","ares_search_dnsrec","ares_dns_record_destroy","ares_getaddrinfo","ares_fds","ares_process","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_parse_naptr_reply","ares_free_data","ares_gethostbyname_file","ares_free_hostent","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_bin","ares_dns_rr_get_bin","ares_search_dnsrec","ares_dns_record_destroy","ares_getaddrinfo","ares_fds","ares_process","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":3}
/**/


// Callback function for ares_search_dnsrec
static void  id_000018_dnsrec_callback(void* arg, ares_status_t status,
                           size_t timeouts, const ares_dns_record_t* dnsrec) {
    // Simple callback that does nothing for fuzzing
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
}

// Callback function for ares_getaddrinfo
static void  id_000018_addrinfo_callback(void* arg, int status,
                              int timeouts, struct ares_addrinfo* ai) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)ai;
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<37) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	
    
    // Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // Create channel with options
    ares_channel_t* channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    memset(&options, 0, sizeof(options));
    
    options.flags = ARES_FLAG_USEVC;
    options.timeout = 2000;
    options.tries = 2;
    optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Parse NAPTR reply from input data
    struct ares_naptr_reply* naptr_reply = nullptr;
    int parse_result = ares_parse_naptr_reply(data, (int)size, &naptr_reply);
    
    if (parse_result == ARES_SUCCESS && naptr_reply) {
        // Free the NAPTR reply data
        ares_free_data(naptr_reply);
    }
    
    // Create a temporary file with the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Try to parse the input as a hostname for file-based lookup
    // Use first 100 bytes or size, whichever is smaller
    size_t hostname_len = size < 100 ? size : 100;
    char* hostname = (char*)malloc(hostname_len + 1);
    if (hostname) {
        fseek(in_file, 0, SEEK_SET);
        fread(hostname, 1, hostname_len, in_file);
        hostname[hostname_len] = '\0';
        
        // Try file-based host lookup
        struct hostent* host = nullptr;
        if (ares_gethostbyname_file(channel, hostname, fuzz_int32_t_2, &host) == ARES_SUCCESS) {
            ares_free_hostent(host);
        }
        free(hostname);
    }
    
    // Create DNS record
    ares_dns_record_t* dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_3, fuzz_uint16_t_4, ARES_OPCODE_QUERY, 
                               ARES_RCODE_NOERROR) == ARES_SUCCESS && dnsrec) {
        
        // Add a resource record
        ares_dns_rr_t* rr = nullptr;
        if (ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                   fuzz_str_5, ARES_REC_TYPE_A, 
                                   ARES_CLASS_IN, fuzz_uint32_t_6) == ARES_SUCCESS && rr) {
            
            // Set binary data in the RR (use first 4 bytes of input for IPv4 address)
            if (size >= 4) {
                ares_dns_rr_set_bin(rr, ARES_RR_A_ADDR, data, 4);
                
                // Retrieve the binary data back
                size_t retrieved_len = 0;
                const unsigned char* retrieved_data = 
                    ares_dns_rr_get_bin(rr, ARES_RR_A_ADDR, &retrieved_len);
                (void)retrieved_data;  // Mark as used
                (void)retrieved_len;   // Mark as used
            }
        }
        
        // Search the DNS record
        ares_search_dnsrec(channel, dnsrec,  id_000018_dnsrec_callback, nullptr);
        
        // Destroy the DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Perform getaddrinfo lookup
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    ares_getaddrinfo(channel, "www.example.com", fuzz_str_7, &hints, 
                      id_000018_addrinfo_callback, nullptr);
    
    // Use ares_fds and ares_process to handle events
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    if (nfds > 0) {
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000;  // 1ms timeout
        
        // Add our file descriptor to the read set
        int fd = fuzz_fileno(in_file);
        if (fd >= 0) {
            FD_SET(fd, &read_fds);
            if (fd >= nfds) nfds = fd + 1;
        }
        
        // Process the events
        ares_process(channel, &read_fds, &write_fds);
    }
    
    // Cancel any pending operations
    ares_cancel(channel);
    
    // Clean up
    assert_file_closed(&in_file);;
    ares_destroy(channel);
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
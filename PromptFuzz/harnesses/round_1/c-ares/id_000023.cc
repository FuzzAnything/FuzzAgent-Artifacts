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
//<ID> 545
//<Prompt> []
/*<Combination>: [
*/
//<score> 49.5, nr_unique_branch: 17
//<Quality> {"density":9,"unique_branches":{"ares_array_len":[[61,7,61,18,0,0,4,0]],"process_timeouts":[[710,9,710,45,0,0,4,1],[720,9,720,30,0,0,4,1]],"ares_conn_read":[[62,9,62,37,0,0,4,0],[63,9,64,52,0,0,4,0]],"ares_process":[[364,9,364,17,0,0,4,1],[370,9,370,18,0,0,4,1]],"ares_timedout":[[168,7,168,15,0,0,4,0],[171,7,171,15,0,0,4,1],[176,10,176,68,0,0,4,0]],"read_answers":[[593,9,593,31,0,0,4,0],[630,10,630,37,0,0,4,1],[661,7,661,22,0,0,4,1]],"ares_socket_recvfrom":[[194,7,194,13,0,0,4,0]],"read_conn_packets":[[510,39,510,70,0,0,4,1]],"process_read":[[687,7,687,29,0,0,4,1]],"ares_sockaddr_addr_eq":[[106,7,106,34,0,0,4,1]]},"library_calls":["ares_library_init","ares_version","ares_library_cleanup","ares_library_cleanup","ares_init_options","ares_library_cleanup","ares_set_socket_functions","ares_query_dnsrec","ares_fds","ares_process","ares_process_pending_write","ares_timeout","ares_process","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_version","ares_init_options","ares_set_socket_functions","ares_query_dnsrec","ares_fds","ares_process","ares_process_pending_write","ares_timeout","ares_process","ares_destroy","ares_library_cleanup"],"visited":1}
/*Looking at the APIs provided, I'll create a fuzz driver that tests DNS query and response handling using c-ares library. The event I'll implement is: "Perform a DNS query and process the response using custom socket callbacks".

Here's the step-by-step implementation:

*/


// Include c-ares headers
extern "C" {
}

// Custom socket function implementations
static ares_socket_t  id_000023_custom_socket(int domain, int type, int protocol, void *user_data) {
    FILE *in_file = (FILE *)user_data;
    return fuzz_fileno(in_file);
}

static int  id_000023_custom_close(ares_socket_t socket_fd, void *user_data) {
    // Don't actually close the fd since it's from fmemopen
    return 0;
}

static int  id_000023_custom_connect(ares_socket_t socket_fd, const struct sockaddr *addr,
                         ares_socklen_t addrlen, void *user_data) {
    // Simulate successful connection
    return 0;
}

static ares_ssize_t  id_000023_custom_recvfrom(ares_socket_t socket_fd, void *buf, size_t len,
                                   int flags, struct sockaddr *from,
                                   ares_socklen_t *fromlen, void *user_data) {
    FILE *in_file = (FILE *)user_data;
    
    // Read from our input data file
    size_t bytes_read = fread(buf, 1, len, in_file);
    
    if (bytes_read == 0) {
        return -1; // EOF or error
    }
    
    // Simulate receiving from a source address if needed
    if (from && fromlen) {
        memset(from, 0, sizeof(*from));
        *fromlen = sizeof(*from);
    }
    
    return bytes_read;
}

static ares_ssize_t  id_000023_custom_sendv(ares_socket_t socket_fd, const struct iovec *vec,
                                int count, void *user_data) {
    // For fuzzing, we don't actually send data
    // Just return total length that would have been sent
    ares_ssize_t total = 0;
    for (int i = 0; i < count; i++) {
        total += vec[i].iov_len;
    }
    return total;
}

// DNS response callback
static void  id_000023_dns_rec_callback(void *arg, ares_status_t status, 
                            size_t timeouts, const ares_dns_record_t *dnsrec) {
    // For fuzzing, we don't need to do anything with the response
    // Just mark that we received a callback
    int *callback_received = (int *)arg;
    *callback_received = 1;
    
    if (dnsrec) {
        // Extract and print some info for debugging
        unsigned short id = ares_dns_record_get_id(dnsrec);
        unsigned short flags = ares_dns_record_get_flags(dnsrec);
        
        // Get query count
        size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
        
        // If we have queries, get the first one
        if (query_cnt > 0) {
            const char *name = nullptr;
            ares_dns_rec_type_t qtype = ARES_REC_TYPE_A;
            ares_dns_class_t qclass = ARES_CLASS_IN;
            
            ares_status_t s = ares_dns_record_query_get(dnsrec, 0, &name, &qtype, &qclass);
            if (s == ARES_SUCCESS && name) {
                // Query name available
            }
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Get c-ares version (required API usage)
    int version;
    const char *version_str = ares_version(&version);
    
    // Create input file from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        ares_library_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize channel
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    
    // Set some basic options
    options.flags = ARES_FLAG_STAYOPEN;
    optmask |= ARES_OPT_FLAGS;
    
    options.timeout = 1000; // 1 second timeout
    optmask |= ARES_OPT_TIMEOUTMS;
    
    options.tries = 1; // Only try once for fuzzing
    optmask |= ARES_OPT_TRIES;
    
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        ares_library_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set custom socket functions (required API usage)
    struct ares_socket_functions funcs;
    funcs.asocket =  id_000023_custom_socket;
    funcs.aclose =  id_000023_custom_close;
    funcs.aconnect =  id_000023_custom_connect;
    funcs.arecvfrom =  id_000023_custom_recvfrom;
    funcs.asendv =  id_000023_custom_sendv;
    
    ares_set_socket_functions(channel, &funcs, in_file);
    
    // Create a simple query name from the input data
    // Use first few bytes as query name if possible
    char query_name[256] = "example.com";
    
    if (size > 0) {
        // Use up to 63 bytes for domain name (max label length)
        size_t name_len = (size > 63) ? 63 : size;
        
        // Create a simple domain name from the data
        // Convert non-printable characters to 'x'
        for (size_t i = 0; i < name_len; i++) {
            if (data[i] >= 32 && data[i] <= 126 && data[i] != '.') {
                query_name[i] = data[i];
            } else {
                query_name[i] = 'x';
            }
        }
        query_name[name_len] = '\0';
        
        // Ensure it ends with .com for valid domain
        if (name_len < 250) {
            strcat(query_name, ".com");
        }
    }
    
    // Perform DNS query using ares_query_dnsrec (required API usage)
    int callback_received = 0;
    unsigned short query_id = 0;
    
    ares_status_t status = ares_query_dnsrec(channel, fuzz_str_2, 
                                            ARES_CLASS_IN, ARES_REC_TYPE_A,
                                             id_000023_dns_rec_callback, &callback_received,
                                            &query_id);
    
    // Also use ares_search for compatibility (simulating with ares_query_dnsrec)
    // Note: ares_search isn't in the provided list, so we'll skip it
    
    // Process DNS responses using ares_fds and ares_process (required API usage)
    if (status == ARES_SUCCESS) {
        fd_set read_fds, write_fds;
        struct timeval tv;
        int max_fd = 0;
        
        // Set timeout
        tv.tv_sec = 0;
        tv.tv_usec = 100000; // 100ms
        
        // Get file descriptors
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        int nfds = ares_fds(channel, &read_fds, &write_fds);
        
        if (nfds > 0) {
            // Find max fd
            for (int i = 0; i < nfds; i++) {
                if (FD_ISSET(i, &read_fds) || FD_ISSET(i, &write_fds)) {
                    max_fd = i + 1;
                }
            }
            
            // Use select to wait for activity
            select(max_fd, &read_fds, &write_fds, NULL, &tv);
            
            // Process the events
            ares_process(channel, &read_fds, &write_fds);
            
            // Also try to process pending writes
            ares_process_pending_write(channel);
        }
        
        // Try one more time with timeout
        struct timeval *timeout = ares_timeout(channel, NULL, &tv);
        if (timeout) {
            select(0, NULL, NULL, NULL, timeout);
            ares_process(channel, NULL, NULL);
        }
    }
    
    // Write some debug info to output file
    fprintf(out_file, "Version: %s\n", version_str);
    fprintf(out_file, "Query: %s\n", query_name);
    fprintf(out_file, "Status: %d\n", status);
    fprintf(out_file, "Callback received: %d\n", callback_received);
    fprintf(out_file, "Query ID: %u\n", query_id);
    
    // Cleanup
    ares_destroy(channel);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    ares_library_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
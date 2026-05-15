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
//<ID> 136
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 19
//<Quality> {"density":10,"unique_branches":{"fake_addrinfo":[[245,7,245,24,0,0,4,1],[245,28,245,46,0,0,4,1],[245,50,245,69,0,0,4,0],[251,32,251,41,0,0,4,1],[254,18,254,27,0,0,4,0],[254,18,254,27,0,0,4,1],[262,9,262,21,0,0,4,1],[262,25,262,31,0,0,4,0],[279,41,279,60,0,0,4,0],[282,7,282,49,0,0,4,0],[283,9,283,15,0,0,4,1],[95,26,95,51,1,0,4,1],[95,55,95,80,1,0,4,0]],"ares_getaddrinfo_int":[[616,7,616,24,0,0,4,0],[616,28,616,46,0,0,4,0],[616,50,616,69,0,0,4,1]],"next_dns_lookup":[[723,5,723,17,0,0,4,1],[734,5,734,19,0,0,4,0]]," id_000013_host_callback":[[542,7,542,25,0,0,4,1]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_parse_srv_reply","ares_parse_ns_reply","ares_parse_caa_reply","ares_free_data","ares_free_hostent","ares_free_data","ares_dns_record_create","ares_dns_record_query_add","ares_send_dnsrec","ares_dns_record_destroy","ares_gethostbyname","ares_getaddrinfo","ares_gethostbyaddr","ares_fds","ares_timeout","ares_process_fd","ares_destroy","ares_destroy_options","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_parse_srv_reply","ares_parse_ns_reply","ares_parse_caa_reply","ares_free_data","ares_free_hostent","ares_free_data","ares_dns_record_create","ares_dns_record_query_add","ares_send_dnsrec","ares_dns_record_destroy","ares_gethostbyname","ares_getaddrinfo","ares_gethostbyaddr","ares_fds","ares_timeout","ares_process_fd","ares_destroy","ares_destroy_options","ares_library_cleanup"],"visited":1}
/**/


extern "C" {
}

// Callback functions
static void  id_000013_dnsrec_callback(void* arg, ares_status_t status, size_t timeouts, 
                           const ares_dns_record_t* dnsrec) {
    // Cleanup if needed
    if (dnsrec) {
        // Note: We don't own the dnsrec, so we don't destroy it here
    }
}

static void  id_000013_host_callback(void* arg, int status, int timeouts, 
                         const struct hostent* host) {
    if (host) {
        ares_free_hostent(const_cast<struct hostent*>(host));
    }
}

static void  id_000013_addrinfo_callback(void* arg, int status, int timeouts,
                             struct ares_addrinfo* ai) {
    if (ai) {
        ares_freeaddrinfo(ai);
    }
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
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
    
    // Create a channel
    ares_channel_t* channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Set some options
    options.timeout = 1000;  // 1 second timeout
    optmask |= ARES_OPT_TIMEOUT;
    
    options.tries = 1;
    optmask |= ARES_OPT_TRIES;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // 1. Parse the input data as DNS replies
    struct ares_srv_reply* srv_out = nullptr;
    struct hostent* ns_host = nullptr;
    struct ares_caa_reply* caa_out = nullptr;
    
    // Try to parse as SRV reply
    ares_parse_srv_reply(data, (int)size, &srv_out);
    
    // Try to parse as NS reply  
    ares_parse_ns_reply(data, (int)size, &ns_host);
    
    // Try to parse as CAA reply
    ares_parse_caa_reply(data, (int)size, &caa_out);
    
    // Cleanup parsed replies
    if (srv_out) {
        ares_free_data(srv_out);
    }
    if (ns_host) {
        ares_free_hostent(ns_host);
    }
    if (caa_out) {
        ares_free_data(caa_out);
    }
    
    // 2. Create a DNS query and send it
    ares_dns_record_t* dnsrec = nullptr;
    unsigned short qid = 0;
    
    // Create a DNS query record
    if (ares_dns_record_create(&dnsrec, 12345, 0x0100, 
                              ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        // Use part of input as query name if possible
        char query_name[256] = {0};
        size_t name_len = size > 255 ? 255 : size;
        
        // Ensure the name is null-terminated and valid for DNS
        for (size_t i = 0; i < name_len; i++) {
            if (data[i] >= 'a' && data[i] <= 'z') {
                query_name[i] = data[i];
            } else if (data[i] >= 'A' && data[i] <= 'Z') {
                query_name[i] = data[i];
            } else if (data[i] >= '0' && data[i] <= '9') {
                query_name[i] = data[i];
            } else if (data[i] == '.' || data[i] == '-') {
                query_name[i] = data[i];
            } else {
                query_name[i] = 'a';
            }
        }
        query_name[name_len] = '\0';
        
        // Add query if we have a valid name
        if (strlen(query_name) > 0) {
            ares_dns_record_query_add(dnsrec, query_name, 
                                     ARES_REC_TYPE_A, ARES_CLASS_IN);
            
            // Send the query
            ares_send_dnsrec(channel, dnsrec,  id_000013_dnsrec_callback, 
                           nullptr, &qid);
        }
        
        ares_dns_record_destroy(dnsrec);
    }
    
    // 3. Test various DNS queries using input data
    char hostname[256] = {0};
    if (size >= 10) {
        // Create a hostname from input data
        size_t hostname_len = size > 255 ? 255 : size;
        for (size_t i = 0; i < hostname_len; i++) {
            char c = data[i];
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
                (c >= '0' && c <= '9') || c == '.' || c == '-') {
                hostname[i] = c;
            } else {
                hostname[i] = 'a';
            }
        }
        hostname[hostname_len] = '\0';
        
        if (strlen(hostname) > 0) {
            // Test gethostbyname
            ares_gethostbyname(channel, hostname, AF_INET, 
                              id_000013_host_callback, nullptr);
            
            // Test getaddrinfo
            struct ares_addrinfo_hints hints;
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            
            ares_getaddrinfo(channel, hostname, "http", &hints,
                            id_000013_addrinfo_callback, nullptr);
            
            // Test gethostbyaddr with a dummy address
            struct in_addr addr;
            if (size >= sizeof(struct in_addr)) {
                memcpy(&addr, data, sizeof(struct in_addr));
                ares_gethostbyaddr(channel, &addr, sizeof(addr), 
                                 AF_INET,  id_000013_host_callback, nullptr);
            }
        }
    }
    
    // 4. Process file descriptors for a short time
    fd_set read_fds, write_fds;
    struct timeval tv;
    
    // Initialize fd sets
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    // Get file descriptors from channel
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    
    // Get timeout
    struct timeval* timeout = ares_timeout(channel, nullptr, &tv);
    
    // If we have file descriptors, do a short select
    if (nfds > 0 && timeout) {
        // Use a very short timeout for fuzzing
        struct timeval short_timeout = {0, 1000};  // 1ms
        select(nfds, &read_fds, &write_fds, nullptr, &short_timeout);
        
        // Process the file descriptors
        for (int i = 0; i < nfds; i++) {
            if (FD_ISSET(i, &read_fds) || FD_ISSET(i, &write_fds)) {
                ares_process_fd(channel, 
                    FD_ISSET(i, &read_fds) ? i : ARES_SOCKET_BAD,
                    FD_ISSET(i, &write_fds) ? i : ARES_SOCKET_BAD);
            }
        }
    }
    
    // 5. Cleanup
    ares_destroy(channel);
    ares_destroy_options(&options);
    ares_library_cleanup();
    
    return 0;
}
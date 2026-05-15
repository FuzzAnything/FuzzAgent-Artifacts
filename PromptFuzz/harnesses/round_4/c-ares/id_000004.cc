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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"ares_parse_a_reply":[[87,7,87,23,0,0,4,1]]},"library_calls":["ares_library_initialized","ares_library_init","ares_init","ares_dns_parse","ares_dns_record_get_opcode","ares_parse_a_reply","ares_dns_class_fromstr","ares_dns_class_tostr","ares_create_query","ares_send","ares_queue_active_queries","ares_inet_ntop","ares_search","ares_free_hostent","ares_dns_record_destroy","ares_fds","ares_process","ares_cancel","ares_destroy"],"critical_calls":["ares_library_initialized","ares_library_init","ares_init","ares_dns_parse","ares_dns_record_get_opcode","ares_parse_a_reply","ares_dns_class_fromstr","ares_dns_class_tostr","ares_create_query","ares_send","ares_queue_active_queries","ares_inet_ntop","ares_search","ares_free_hostent","ares_dns_record_destroy","ares_fds","ares_process","ares_cancel","ares_destroy"],"visited":0}
/**/


// Fuzz driver that processes DNS response data through c-ares APIs
extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<49) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
	//fuzzer shim end}




    // Initialize c-ares library if not already initialized
    if (ares_library_initialized() == 0) {
        ares_library_init(ARES_LIB_INIT_ALL);
    }

    // Create a channel for DNS operations
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        return 0;
    }

    // Parse the input data as a DNS record
    ares_dns_record_t *dnsrec = nullptr;
    if (size > 0) {
        ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
    }

    // If we successfully parsed a DNS record, extract information from it
    if (dnsrec) {
        // Get the opcode from the DNS record
        ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
        
        // Try to parse as an A record reply
        struct hostent *host = nullptr;
        struct ares_addrttl *addrttls = nullptr;
        int naddrttls = 0;
        
        if (size > 0) {
            ares_parse_a_reply(data, static_cast<int>(size), &host, addrttls, &naddrttls);
        }
        
        // Convert DNS class from string and back
        ares_dns_class_t dns_class;
        const char *class_str = "IN";
        ares_dns_class_fromstr(&dns_class, fuzz_str_2);
        const char *class_str_back = ares_dns_class_tostr(dns_class);
        
        // Create a simple query and send it
        if (size > 0) {
            unsigned char *qbuf = nullptr;
            int qlen = 0;
            
            // Create a query using the input data as inspiration
            if (size >= 4) {
                // Use first 4 bytes as query ID
                unsigned short id = (data[0] << 8) | data[1];
                
                // Create a simple query for "example.com"
                ares_create_query(fuzz_str_3, 
                                 C_IN, 
                                 T_A, 
                                 id, 
                                 fuzz_int32_t_4, 
                                 &qbuf, 
                                 &qlen, 
                                 fuzz_int32_t_5);
                
                if (qbuf && qlen > 0) {
                    // Send the query
                    ares_send(channel, qbuf, fuzz_int32_t_6, 
                             [](void *arg, int status, int timeouts, 
                                const unsigned char *abuf, int alen) {
                                 // Callback for query response
                             }, 
                             nullptr);
                    
                    // Check active queries
                    size_t active_queries = ares_queue_active_queries(channel);
                    
                    // Clean up query buffer
                    if (qbuf) {
                        free(qbuf);
                    }
                }
            }
        }
        
        // Use ares_inet_ntop to convert an IP address
        char ip_str[INET_ADDRSTRLEN];
        struct in_addr ip_addr;
        ip_addr.s_addr = htonl(0x7F000001); // 127.0.0.1
        ares_inet_ntop(AF_INET, &ip_addr, ip_str, sizeof(ip_str));
        
        // Perform a DNS search
        ares_search(channel, fuzz_str_7, C_IN, T_A, 
                   [](void *arg, int status, int timeouts, 
                      const unsigned char *abuf, int alen) {
                       // Callback for search results
                   }, 
                   nullptr);
        
        // Clean up parsed hostent
        if (host) {
            ares_free_hostent(host);
        }
        
        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Process any pending operations (non-blocking)
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    // Get file descriptors that need monitoring
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    
    if (nfds > 0) {
        // Process with empty fd sets (non-blocking)
        ares_process(channel, &read_fds, &write_fds);
    }
    
    // Cancel any pending operations
    ares_cancel(channel);
    
    // Clean up channel
    ares_destroy(channel);
    
    return 0;
}
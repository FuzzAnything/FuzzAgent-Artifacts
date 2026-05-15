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
//<ID> 351
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 5
//<Quality> {"density":7,"unique_branches":{"ares_dns_record_query_add":[[261,7,261,21,0,0,4,0]],"ares_dns_record_create_query":[[1595,7,1595,29,0,0,4,0],[1633,7,1633,29,0,0,4,0]],"ares_create_query_int":[[37,32,37,34,0,0,4,1],[50,7,50,29,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_socket_callback","ares_destroy","ares_library_cleanup","ares_mkquery","ares_dns_record_query_add","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_name","ares_dns_record_rr_del","ares_dns_record_destroy","ares_parse_uri_reply","ares_free_string","ares_timeout","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_socket_callback","ares_mkquery","ares_dns_record_query_add","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_name","ares_dns_record_rr_del","ares_dns_record_destroy","ares_parse_uri_reply","ares_free_string","ares_timeout","ares_destroy","ares_library_cleanup"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified c-ares APIs to process DNS queries and parse URI replies:

*/


// Include c-ares headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set socket configure callback (using ares_set_socket_callback as alternative)
    ares_set_socket_callback(channel, [](ares_socket_t socket_fd, int type, void *data) -> int {
        // Simple callback that does basic socket configuration
        int optval = 1;
        setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        return 0;
    }, nullptr);

    // Create a DNS query for URI record
    unsigned char *query_buf = nullptr;
    int query_len = 0;
    
    // Use part of the input as domain name (ensure it's null-terminated)
    size_t name_len = size > 100 ? 100 : size - 4;
    char *domain_name = (char *)malloc(name_len + 1);
    if (!domain_name) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }
    
    memcpy(domain_name, data, name_len);
    domain_name[name_len] = '\0';
    
    // Extract parameters from input
    uint16_t id = *((uint16_t *)data);
    int dnsclass = data[2] % 3; // Random class: 0=IN, 1=CS, 2=CH
    int type = 256; // URI type (assuming 256 as URI type)
    int rd = data[3] & 1; // Recursion desired flag
    
    // Create query using ares_mkquery (similar to ares_create_query)
    int result = ares_mkquery(domain_name, dnsclass, type, id, rd, &query_buf, &query_len);
    
    if (result == ARES_SUCCESS && query_buf && query_len > 0) {
        // Create a DNS record structure
        ares_dns_record_t *dnsrec = nullptr;
        
        // Add a query to the DNS record
        ares_status_t status = ares_dns_record_query_add(dnsrec, domain_name, 
                                                        (ares_dns_rec_type_t)type,
                                                        (ares_dns_class_t)dnsclass);
        
        if (status == ARES_SUCCESS && dnsrec) {
            // Get resource record count
            size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
            
            if (rr_count > 0) {
                // Get first RR from answer section
                ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_1);
                if (rr) {
                    // Get RR name
                    const char *rr_name = ares_dns_rr_get_name(rr);
                    
                    // Delete the RR if we have at least one
                    ares_dns_record_rr_del(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_2);
                }
            }
            
            // Clean up DNS record
            ares_dns_record_destroy(dnsrec);
        }
        
        // Parse the input as URI reply
        struct ares_uri_reply *uri_reply = nullptr;
        
        // Use the input data as DNS reply buffer
        if (size > 12) { // Minimum DNS header size
            ares_parse_uri_reply(data, size, &uri_reply);
            
            // Free URI reply chain if parsed successfully
            if (uri_reply) {
                struct ares_uri_reply *current = uri_reply;
                while (current) {
                    struct ares_uri_reply *next = current->next;
                    if (current->uri) {
                        ares_free_string(current->uri);
                    }
                    free(current);
                    current = next;
                }
            }
        }
        
        // Clean up query buffer
        if (query_buf) {
            free(query_buf);
        }
    }
    
    // Free allocated resources
    free(domain_name);
    
    // Process any pending events
    struct timeval tv = {0, 0};
    ares_timeout(channel, nullptr, &tv);
    
    // Clean up channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();
    
    return 0;
}
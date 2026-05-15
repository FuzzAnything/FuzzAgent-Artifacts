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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["ares_library_init_mem","ares_dns_parse","ares_dns_write","ares_init","ares_set_local_ip6","ares_get_servers","ares_inet_pton","ares_dns_record_destroy","ares_free_data","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init_mem","ares_dns_parse","ares_dns_write","ares_init","ares_set_local_ip6","ares_get_servers","ares_inet_pton","ares_dns_record_destroy","ares_free_data","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize c-ares with custom memory functions
    ares_library_init_mem(ARES_LIB_INIT_ALL, 
                         malloc, 
                         free, 
                         realloc);
    
    ares_channel_t *channel = nullptr;
    ares_dns_record_t *dnsrec = nullptr;
    struct ares_addr_node *servers = nullptr;
    unsigned char *dns_buffer = nullptr;
    size_t dns_buf_len = 0;
    
    // Create a DNS record from input data
    if (size >= 12) {  // Minimum DNS header size
        ares_status_t status = ares_dns_parse(data, size, 0, &dnsrec);
        if (status == ARES_SUCCESS && dnsrec) {
            // Write DNS record back to buffer
            ares_dns_write(dnsrec, &dns_buffer, &dns_buf_len);
        }
    }
    
    // Initialize ares channel
    if (ares_init(&channel) == ARES_SUCCESS && channel) {
        // Set local IPv6 address from input data
        if (size >= 16) {  // IPv6 address is 16 bytes
            ares_set_local_ip6(channel, data);
        }
        
        // Get servers from channel
        ares_get_servers(channel, &servers);
        
        // Convert IPv6 string representation to binary
        if (size >= 50) {  // Need enough data for IPv6 string
            char ip6_str[50];
            struct in6_addr ip6_bin;
            
            // Copy data as IPv6 string (null-terminate it)
            size_t str_len = (size < 49) ? size : 49;
            memcpy(ip6_str, data, str_len);
            ip6_str[str_len] = '\0';
            
            // Try to convert string to IPv6 binary
            ares_inet_pton(AF_INET6, ip6_str, &ip6_bin);
        }
    }
    
    // Cleanup
    if (dns_buffer) {
        free(dns_buffer);
    }
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    if (servers) {
        ares_free_data(servers);
    }
    if (channel) {
        ares_destroy(channel);
    }
    
    ares_library_cleanup();
    return 0;
}
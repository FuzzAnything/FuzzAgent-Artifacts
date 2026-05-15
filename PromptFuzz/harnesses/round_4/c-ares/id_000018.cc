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
//<ID> 260
//<Prompt> []
/*<Combination>: [
*/
//<score> 50, nr_unique_branch: 11
//<Quality> {"density":10,"unique_branches":{"ares_parse_dns_name_escape":[[132,35,132,62,3,0,4,0]],"ares_inet_net_pton_ipv4":[[106,13,106,20,0,0,4,0],[114,11,114,18,0,0,4,0],[99,34,99,59,5,0,4,1],[100,5,100,30,5,0,4,1],[95,26,95,51,19,0,4,1]],"ares_hosts_update":[[709,7,709,49,0,0,4,0]],"ares_hosts_expired":[[616,7,616,17,0,0,4,1],[621,7,621,18,0,0,4,1],[627,7,627,46,0,0,4,1],[631,7,631,23,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_save_options","ares_init_options","ares_set_pending_write_cb","ares_expand_name","ares_inet_pton","ares_inet_pton","ares_getnameinfo","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_get_opt_byid","ares_dns_record_destroy","ares_getaddrinfo","ares_process","ares_process","ares_free_string","ares_destroy","ares_destroy","ares_destroy_options","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_save_options","ares_init_options","ares_set_pending_write_cb","ares_expand_name","ares_inet_pton","ares_inet_pton","ares_getnameinfo","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_get_opt_byid","ares_dns_record_destroy","ares_getaddrinfo","ares_process","ares_process","ares_free_string","ares_destroy","ares_destroy","ares_destroy_options","ares_library_cleanup"],"visited":0}
/**/


// Include c-ares headers

// Callback function declarations
static void  id_000018_nameinfo_callback(void* arg, int status, int timeouts, 
                             const char* node, const char* service) {
    // Do nothing in fuzzing context
    (void)arg; (void)status; (void)timeouts; (void)node; (void)service;
}

static void  id_000018_addrinfo_callback(void* arg, int status, int timeouts,
                              struct ares_addrinfo* result) {
    // Do nothing in fuzzing context
    (void)arg; (void)status; (void)timeouts;
    if (result) {
        ares_freeaddrinfo(result);
    }
}

static void  id_000018_pending_write_callback(void* arg) {
    // Do nothing in fuzzing context
    (void)arg;
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<57) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    ares_channel_t *channel = nullptr;
    ares_channel_t *dup_channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    char *expanded_name = nullptr;
    long enclen = 0;
    
    // 1. Initialize channel
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // 2. Save options from channel
    if (ares_save_options(channel, &options, &optmask) == ARES_SUCCESS) {
        // Options saved successfully
    }
    
    // 3. Duplicate channel (using ares_dup if available, otherwise simulate)
    // Note: ares_dup is not in the provided API list, so we'll use ares_init_options
    // to create a similar channel
    if (ares_init_options(&dup_channel, &options, fuzz_int32_t_1) == ARES_SUCCESS) {
        // 4. Set pending write callback on duplicated channel
        ares_set_pending_write_cb(dup_channel,  id_000018_pending_write_callback, nullptr);
    }
    
    // 5. Try to expand a name from the input data
    // Use input data as DNS message buffer
    if (size > 0) {
        // Create a temporary buffer with the input data
        unsigned char *buf = (unsigned char *)malloc(size);
        if (buf) {
            memcpy(buf, data, size);
            
            // Try to expand name from the beginning of buffer
            // Use the buffer as both encoded pointer and abuf
            ares_expand_name(buf, buf, size, &expanded_name, &enclen);
            
            free(buf);
        }
    }
    
    // 6. Try to use ares_inet_pton with data interpreted as IP address string
    if (size > 0 && size < 256) {
        char ip_str[256];
        struct in_addr addr4;
        struct in6_addr addr6;
        
        // Copy data as null-terminated string for IP address
        size_t copy_size = (size < 255) ? size : 255;
        memcpy(ip_str, data, copy_size);
        ip_str[copy_size] = '\0';
        
        // Try IPv4 conversion
        ares_inet_pton(fuzz_int32_t_2, ip_str, &addr4);
        
        // Try IPv6 conversion
        ares_inet_pton(fuzz_int32_t_3, ip_str, &addr6);
    }
    
    // 7. Create a sockaddr structure for getnameinfo
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    
    // Use first 4 bytes of input for IP address if available
    if (size >= 4) {
        memcpy(&sa.sin_addr, data, 4);
    } else {
        sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    }
    sa.sin_port = htons(80);
    
    // 8. Perform getnameinfo query
    ares_getnameinfo(channel, (struct sockaddr*)&sa, sizeof(sa),
                    ARES_NI_NOFQDN | ARES_NI_NUMERICSERV,
                     id_000018_nameinfo_callback, nullptr);
    
    // 9. Try to create a DNS RR structure and check for options
    // Create a minimal DNS record
    ares_dns_record_t *dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_4, fuzz_uint16_t_5, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        // Add a query section
        ares_dns_record_query_add(dnsrec, fuzz_str_6, ARES_REC_TYPE_A, ARES_CLASS_IN);
        
        // Get the RR from answer section if available
        ares_dns_rr_t *rr = nullptr;
        if (ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                  fuzz_str_7, ARES_REC_TYPE_A, ARES_CLASS_IN, fuzz_uint32_t_8) == ARES_SUCCESS) {
            // Try to get an option by ID (using a common OPT record ID)
            const unsigned char *opt_val = nullptr;
            size_t opt_len = 0;
            ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS, fuzz_uint16_t_9, &opt_val, &opt_len);
        }
        
        // Destroy the DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // 10. Perform getaddrinfo query
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    // Create node string from input data if possible
    char node_str[256];
    if (size > 0 && size < 256) {
        size_t copy_size = (size < 255) ? size : 255;
        memcpy(node_str, data, copy_size);
        node_str[copy_size] = '\0';
        
        // Try to ensure it's a valid-ish string for DNS
        for (size_t i = 0; i < copy_size; i++) {
            if (node_str[i] < 32 || node_str[i] > 126) {
                node_str[i] = 'a' + (node_str[i] % 26);
            }
        }
    } else {
        strcpy(node_str, "localhost");
    }
    
    ares_getaddrinfo(channel, node_str, fuzz_str_10, &hints, 
                     id_000018_addrinfo_callback, nullptr);
    
    // 11. Process channels with empty fd_sets
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    // Process original channel
    ares_process(channel, &read_fds, &write_fds);
    
    // Process duplicated channel if created
    if (dup_channel) {
        ares_process(dup_channel, &read_fds, &write_fds);
    }
    
    // 12. Clean up
    if (expanded_name) {
        ares_free_string(expanded_name);
    }
    
    if (dup_channel) {
        ares_destroy(dup_channel);
    }
    
    if (channel) {
        ares_destroy(channel);
    }
    
    // Destroy options if any were allocated
    ares_destroy_options(&options);
    
    ares_library_cleanup();
    
    return 0;
}
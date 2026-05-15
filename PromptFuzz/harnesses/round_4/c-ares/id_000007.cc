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
//<ID> 179
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 7
//<Quality> {"density":8,"unique_branches":{"ares_getaddrinfo_int":[[608,7,608,13,0,0,4,0],[626,7,626,14,0,0,4,1]],"ares_dns_rr_get_opt_byid":[[1085,9,1085,27,0,0,4,0],[1090,7,1090,15,0,0,4,1],[1090,19,1090,33,0,0,4,1],[1094,7,1094,10,0,0,4,0],[1097,7,1097,14,0,0,4,0]]},"library_calls":["ares_init","ares_save_options","ares_dup","ares_set_pending_write_cb","ares_process_pending_write","ares_dns_parse","ares_dns_record_rr_get_const","ares_dns_rr_get_opt_byid","ares_dns_record_destroy","ares_getaddrinfo","ares_process","ares_inet_pton","ares_inet_pton","ares_destroy","ares_destroy"],"critical_calls":["ares_init","ares_save_options","ares_dup","ares_set_pending_write_cb","ares_process_pending_write","ares_dns_parse","ares_dns_record_rr_get_const","ares_dns_rr_get_opt_byid","ares_dns_record_destroy","ares_getaddrinfo","ares_process","ares_inet_pton","ares_inet_pton","ares_destroy","ares_destroy"],"visited":1}
/**/


extern "C" {
}

// Callback for getaddrinfo
static void  id_000007_getaddrinfo_callback(void* arg, int status, int timeouts, struct ares_addrinfo* result) {
    if (result) {
        ares_freeaddrinfo(result);
    }
}

// Pending write callback
static void  id_000007_pending_write_callback(void* user_data) {
    // Do nothing - just satisfy the API requirement
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<14) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    ares_channel_t *channel = nullptr;
    ares_channel_t *duped_channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize c-ares library
    if (ares_init(&channel) != ARES_SUCCESS) {
        return 0;
    }
    
    // 1. Use ares_save_options
    if (ares_save_options(channel, &options, &optmask) == ARES_SUCCESS) {
        // Options saved successfully
    }
    
    // 2. Use ares_dup
    if (ares_dup(&duped_channel, channel) == ARES_SUCCESS) {
        // Channel duplicated successfully
        
        // 7. Use ares_set_pending_write_cb
        ares_set_pending_write_cb(duped_channel,  id_000007_pending_write_callback, nullptr);
        
        // 4. Use ares_process_pending_write
        ares_process_pending_write(duped_channel);
    }
    
    // Parse input as DNS response
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
    
    if (parse_status == ARES_SUCCESS && dnsrec) {
        // Try to get resource records and check for options
        for (int sect = ARES_SECTION_ANSWER; sect <= ARES_SECTION_ADDITIONAL; sect++) {
            size_t i = 0;
            const ares_dns_rr_t* rr = nullptr;
            
            while ((rr = ares_dns_record_rr_get_const(dnsrec, (ares_dns_section_t)sect, i)) != nullptr) {
                // 5. Use ares_dns_rr_get_opt_byid
                const unsigned char* opt_val = nullptr;
                size_t opt_val_len = 0;
                unsigned short opt_id = 0;  // Try with opt ID 0
                
                ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS, fuzz_uint16_t_2, &opt_val, &opt_val_len);
                
                i++;
            }
        }
        
        ares_dns_record_destroy(dnsrec);
    }
    
    // 3. Use ares_getaddrinfo
    // Extract a potential hostname from the input data
    char hostname[256] = {0};
    size_t hostname_len = size < 255 ? size : 255;
    memcpy(hostname, data, hostname_len);
    hostname[hostname_len] = '\0';
    
    // Ensure we have a somewhat valid-looking string
    for (size_t i = 0; i < hostname_len; i++) {
        if (hostname[i] < 32 || hostname[i] > 126) {
            hostname[i] = 'a';
        }
    }
    
    ares_getaddrinfo(channel, hostname, nullptr, nullptr,  id_000007_getaddrinfo_callback, nullptr);
    
    // Process to trigger the callback
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    ares_process(channel, &read_fds, &write_fds);
    
    // 6. Use ares_inet_pton
    // Try to parse an IP address from the input data
    char ip_str[64] = {0};
    size_t ip_len = size < 63 ? size : 63;
    memcpy(ip_str, data, ip_len);
    ip_str[ip_len] = '\0';
    
    // Try IPv4
    struct in_addr addr4;
    int ipv4_result = ares_inet_pton(fuzz_int32_t_3, ip_str, &addr4);
    
    // Try IPv6 if IPv4 failed
    if (ipv4_result != 1) {
        struct in6_addr addr6;
        ares_inet_pton(fuzz_int32_t_4, ip_str, &addr6);
    }
    
    // Clean up
    if (duped_channel) {
        ares_destroy(duped_channel);
    }
    ares_destroy(channel);
    
    return 0;
}
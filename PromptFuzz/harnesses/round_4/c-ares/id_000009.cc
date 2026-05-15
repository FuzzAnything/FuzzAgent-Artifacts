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
//<ID> 182
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 8
//<Quality> {"density":8,"unique_branches":{"lookup_service":[[227,13,227,16,0,0,4,1]],"numeric_service_to_port":[[589,7,589,17,0,0,4,0],[589,21,589,33,0,0,4,0],[589,37,589,49,0,0,4,0]],"ares_dns_rr_get_opt_byid":[[1080,15,1080,22,0,0,4,1],[1090,7,1090,15,0,0,4,0]],"ares_getaddrinfo_int":[[634,11,634,16,0,0,4,0],[635,13,635,53,0,0,4,1]]},"library_calls":["ares_init","ares_dup","ares_save_options","ares_set_pending_write_cb","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_opt_byid","ares_inet_pton","ares_inet_pton","ares_getaddrinfo","ares_freeaddrinfo","ares_freeaddrinfo","ares_process_pending_write","ares_dns_record_destroy","ares_destroy","ares_destroy"],"critical_calls":["ares_init","ares_dup","ares_save_options","ares_set_pending_write_cb","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_opt_byid","ares_inet_pton","ares_inet_pton","ares_getaddrinfo","ares_freeaddrinfo","ares_freeaddrinfo","ares_process_pending_write","ares_dns_record_destroy","ares_destroy","ares_destroy"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<27) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Initialize first channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        return 0;
    }

    // 1. Use ares_dup to duplicate channel
    ares_channel_t *dup_channel = nullptr;
    if (ares_dup(&dup_channel, channel) == ARES_SUCCESS) {
        // 2. Use ares_save_options on original channel
        struct ares_options options;
        int optmask = 0;
        ares_save_options(channel, &options, &optmask);
        
        // 3. Use ares_set_pending_write_cb on duplicated channel
        ares_set_pending_write_cb(dup_channel, [](void* user_data) {
            // Empty callback
        }, nullptr);
        
        // 4. Use ares_dns_rr_get_opt_byid (requires parsed DNS record)
        // First parse input as DNS record
        ares_dns_record_t *dnsrec = nullptr;
        ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
        
        if (status == ARES_SUCCESS && dnsrec) {
            // Get first resource record from answer section if available
            size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
            if (rr_count > 0) {
                const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, 
                                                                      ARES_SECTION_ANSWER, fuzz_uint64_t_2);
                if (rr) {
                    // Try to get an OPT record if available
                    const unsigned char *opt_val = nullptr;
                    size_t opt_len = 0;
                    ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS, 
                                            fuzz_uint16_t_3, &opt_val, &opt_len);
                }
            }
        }
        
        // 5. Use ares_inet_pton with some input data
        if (size >= 16) {
            // Try to extract an IP address string from the data
            char ip_str[INET6_ADDRSTRLEN] = {0};
            size_t ip_len = size < sizeof(ip_str) ? size : sizeof(ip_str) - 1;
            memcpy(ip_str, data, ip_len);
            ip_str[ip_len] = '\0';
            
            // Try IPv4 first
            struct in_addr addr4;
            if (ares_inet_pton(fuzz_int32_t_4, ip_str, &addr4) != 1) {
                // Try IPv6
                struct in6_addr addr6;
                ares_inet_pton(fuzz_int32_t_5, ip_str, &addr6);
            }
        }
        
        // 6. Use ares_getaddrinfo on duplicated channel
        // Extract a potential hostname from the data
        char hostname[256] = {0};
        size_t hostname_len = size < sizeof(hostname) ? size : sizeof(hostname) - 1;
        // Use first part of data as hostname, but ensure it's null-terminated
        memcpy(hostname, data, hostname_len);
        hostname[hostname_len] = '\0';
        
        struct ares_addrinfo_hints hints = {0};
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        
        ares_getaddrinfo(dup_channel, hostname, fuzz_str_6, &hints,
                        [](void* arg, int status, int timeouts, 
                           struct ares_addrinfo* result) {
                            // Callback - free result if provided
                            if (result) {
                                ares_freeaddrinfo(result);
                            }
                        }, nullptr);
        
        // 7. Use ares_process_pending_write on duplicated channel
        ares_process_pending_write(dup_channel);
        
        // Clean up DNS record if created
        if (dnsrec) {
            ares_dns_record_destroy(dnsrec);
        }
        
        // Destroy duplicated channel
        ares_destroy(dup_channel);
    }
    
    // Destroy original channel
    ares_destroy(channel);
    
    return 0;
}
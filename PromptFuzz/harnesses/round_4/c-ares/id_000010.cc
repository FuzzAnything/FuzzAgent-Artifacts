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
//<ID> 183
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_dns_parse","ares_dup","ares_save_options","ares_set_pending_write_cb","ares_dns_record_rr_get_const","ares_dns_rr_get_keys","ares_dns_rr_key_datatype","ares_dns_rr_get_opt_byid","ares_inet_pton","ares_inet_pton","ares_process_pending_write","ares_getaddrinfo","ares_freeaddrinfo","ares_freeaddrinfo","ares_process_fd","ares_dns_record_destroy","ares_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_dns_parse","ares_dup","ares_save_options","ares_set_pending_write_cb","ares_dns_record_rr_get_const","ares_dns_rr_get_keys","ares_dns_rr_key_datatype","ares_dns_rr_get_opt_byid","ares_inet_pton","ares_inet_pton","ares_process_pending_write","ares_getaddrinfo","ares_freeaddrinfo","ares_freeaddrinfo","ares_process_fd","ares_dns_record_destroy","ares_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<55) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create channels
    ares_channel_t *channel = nullptr;
    ares_channel_t *dest_channel = nullptr;
    
    // Initialize main channel
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Use the input data as if it were c-ares output data
    // We'll attempt to parse it as a DNS response
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
    
    // 1. Duplicate the channel
    int dup_result = ares_dup(&dest_channel, channel);
    
    // 2. Save options from original channel
    struct ares_options options;
    int optmask = 0;
    int save_result = 0;
    if (channel) {
        save_result = ares_save_options(channel, &options, &optmask);
    }
    
    // 3. Set pending write callback
    if (dest_channel) {
        ares_set_pending_write_cb(dest_channel, 
            [](void *user_data) {
                // Empty callback for fuzzing
            }, 
            nullptr);
    }
    
    // 4. Use ares_dns_rr_get_opt_byid if we have a parsed DNS record
    if (parse_status == ARES_SUCCESS && dnsrec) {
        // Get first resource record from answer section if available
        const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_2);
        if (rr) {
            const unsigned char *opt_val = nullptr;
            size_t opt_val_len = 0;
            
            // Try to get OPT option by ID (using key ARES_RR_OPT_OPTIONS if available)
            // We need to find a key that has OPT datatype
            size_t key_cnt = 0;
            const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(ARES_REC_TYPE_OPT, &key_cnt);
            
            for (size_t i = 0; i < key_cnt; i++) {
                ares_dns_rr_key_t key = keys[i];
                if (ares_dns_rr_key_datatype(key) == ARES_DATATYPE_OPT) {
                    // Try to get option with ID 1 (example)
                    ares_dns_rr_get_opt_byid(rr, key, fuzz_uint16_t_3, &opt_val, &opt_val_len);
                    break;
                }
            }
        }
    }
    
    // 5. Use ares_inet_pton with some sample data
    struct in_addr addr4;
    struct in6_addr addr6;
    
    // Try IPv4 conversion
    ares_inet_pton(fuzz_int32_t_5, fuzz_str_4, &addr4);
    
    // Try IPv6 conversion
    ares_inet_pton(fuzz_int32_t_7, fuzz_str_6, &addr6);
    
    // 6. Process pending writes
    if (dest_channel) {
        ares_process_pending_write(dest_channel);
    }
    
    // 7. Use ares_getaddrinfo with a sample query
    if (dest_channel) {
        struct ares_addrinfo_hints hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        
        // Use a small part of input data as hostname (if available)
        char hostname[256] = {0};
        if (size > 0) {
            size_t hostname_len = size < 255 ? size : 255;
            memcpy(hostname, data, hostname_len);
            hostname[hostname_len] = '\0';
            
            // Replace non-printable characters
            for (size_t i = 0; i < hostname_len; i++) {
                if (hostname[i] < 32 || hostname[i] > 126) {
                    hostname[i] = 'a' + (hostname[i] % 26);
                }
            }
        } else {
            strcpy(hostname, "localhost");
        }
        
        ares_getaddrinfo(dest_channel, hostname, fuzz_str_8, &hints,
            [](void *arg, int status, int timeouts, struct ares_addrinfo *result) {
                if (result) {
                    ares_freeaddrinfo(result);
                }
            }, 
            nullptr);
        
        // Process the query
        ares_process_fd(dest_channel, fuzz_int32_t_9, fuzz_int32_t_10);
    }
    
    // Cleanup
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    if (dest_channel) {
        ares_destroy(dest_channel);
    }
    
    if (channel) {
        ares_destroy(channel);
    }
    
    ares_library_cleanup();
    
    return 0;
}
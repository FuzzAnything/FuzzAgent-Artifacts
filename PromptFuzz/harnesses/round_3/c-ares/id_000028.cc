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
//<ID> 512
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{" id_000028_nameinfo_callback":[[255,9,255,47,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_dup","ares_get_servers_csv","ares_free_string","ares_dns_parse","ares_dns_record_get_id","ares_dns_record_set_id","ares_dns_record_get_flags","ares_parse_a_reply","ares_free_hostent","ares_parse_ns_reply","ares_free_hostent","ares_dns_record_destroy","ares_process_fd","ares_getnameinfo","ares_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_dup","ares_get_servers_csv","ares_free_string","ares_dns_parse","ares_dns_record_get_id","ares_dns_record_set_id","ares_dns_record_get_flags","ares_parse_a_reply","ares_free_hostent","ares_parse_ns_reply","ares_free_hostent","ares_dns_record_destroy","ares_process_fd","ares_getnameinfo","ares_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified c-ares APIs to process DNS response data:

*/


// Custom callback for ares_getnameinfo
static void  id_000028_nameinfo_callback(void *arg, int status, int timeouts,
                               const char *node, const char *service) {
    // Empty callback - just for fuzzing
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)node;
    (void)service;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<14) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create channels
    ares_channel_t *channel = nullptr;
    ares_channel_t *dup_channel = nullptr;
    
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Test ares_dup
    if (ares_dup(&dup_channel, channel) == ARES_SUCCESS && dup_channel != nullptr) {
        // Get servers as CSV
        char *servers_csv = ares_get_servers_csv(channel);
        if (servers_csv != nullptr) {
            ares_free_string(servers_csv);
        }
    }

    // Try to parse the input as a DNS record
    ares_dns_record_t *dnsrec = nullptr;
    if (size > 0) {
        ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
        
        if (parse_status == ARES_SUCCESS && dnsrec != nullptr) {
            // Get and set DNS record ID
            unsigned short original_id = ares_dns_record_get_id(dnsrec);
            (void)original_id; // Unused but retrieved
            
            // Set a new ID (using first 2 bytes of data if available)
            unsigned short new_id = 0x1234;
            if (size >= 2) {
                new_id = (data[0] << 8) | data[1];
            }
            ares_dns_record_set_id(dnsrec, fuzz_uint16_t_2);
            
            // Get flags
            unsigned short flags = ares_dns_record_get_flags(dnsrec);
            (void)flags; // Unused but retrieved
            
            // Try to parse as A record reply
            struct hostent *host_a = nullptr;
            struct ares_addrttl *addrttls = nullptr;
            int naddrttls = 0;
            
            if (ares_parse_a_reply(data, (int)(size > INT_MAX ? INT_MAX : size),
                                   &host_a, addrttls, &naddrttls) == ARES_SUCCESS) {
                if (host_a != nullptr) {
                    ares_free_hostent(host_a);
                }
            }
            
            // Try to parse as NS record reply
            struct hostent *host_ns = nullptr;
            if (ares_parse_ns_reply(data, (int)(size > INT_MAX ? INT_MAX : size),
                                    &host_ns) == ARES_SUCCESS) {
                if (host_ns != nullptr) {
                    ares_free_hostent(host_ns);
                }
            }
            
            // Clean up DNS record
            ares_dns_record_destroy(dnsrec);
        }
    }

    // Test ares_process_fd with file descriptor from input data
    if (size > 0) {
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file != nullptr) {
            int fd = fuzz_fileno(in_file);
            if (fd >= 0) {
                // Process the file descriptor (use ARES_SOCKET_BAD for write fd)
                ares_process_fd(channel, fd, fuzz_int32_t_3);
            }
            assert_file_closed(&in_file);;
        }
    }

    // Test ares_getnameinfo with a dummy sockaddr
    if (size >= sizeof(struct sockaddr_in)) {
        struct sockaddr_in sa;
        memset(&sa, 0, sizeof(sa));
        sa.sin_family = AF_INET;
        
        // Use some bytes from input for the address if available
        if (size >= sizeof(sa.sin_addr) + sizeof(sa.sin_family)) {
            memcpy(&sa.sin_addr, data, sizeof(sa.sin_addr) > size ? size : sizeof(sa.sin_addr));
        } else {
            sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        }
        sa.sin_port = htons(80);
        
        ares_getnameinfo(channel, (struct sockaddr*)&sa, sizeof(sa),
                         fuzz_int32_t_4,  id_000028_nameinfo_callback, nullptr);
    }

    // Clean up
    if (dup_channel != nullptr) {
        ares_destroy(dup_channel);
    }
    if (channel != nullptr) {
        ares_destroy(channel);
    }
    
    ares_library_cleanup();
    
    return 0;
}
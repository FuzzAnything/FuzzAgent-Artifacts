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
//<ID> 266
//<Prompt> []
/*<Combination>: [
*/
//<score> 75, nr_unique_branch: 17
//<Quality> {"density":15,"unique_branches":{"ares_process_fd":[[281,7,281,34,0,0,4,0],[282,9,282,28,0,0,4,0]],"ares_dns_rec_type_tostr":[[202,5,202,29,0,0,4,0]],"ares_parse_ns_reply":[[60,7,60,29,0,0,4,0],[146,9,146,32,0,0,4,0]],"ares_process_fds_nolock":[[210,9,210,50,0,0,4,1],[216,9,216,30,0,0,4,1],[224,9,224,49,0,0,4,0]],"ares_getnameinfo_int":[[131,9,131,36,0,0,4,0],[140,11,140,35,0,0,4,1],[144,11,144,36,0,0,4,1],[154,11,154,40,0,0,4,1]],"process_write":[[392,7,392,19,0,0,4,0]],"ares_search_int":[[354,7,354,29,0,0,4,0]],"ares_dns_rcode_isvalid":[[53,5,53,28,0,0,4,0]],"ares_set_pending_write_cb":[[420,7,420,22,0,0,4,1],[420,26,420,66,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_get_servers_csv","ares_free_string","ares_set_pending_write_cb","ares_dup","ares_process_fd","ares_process_fd","ares_parse_ns_reply","ares_free_hostent","ares_dns_parse","ares_search_dnsrec","ares_dns_record_destroy","ares_getnameinfo","ares_search","ares_free_hostent","ares_dns_record_destroy","ares_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_get_servers_csv","ares_free_string","ares_set_pending_write_cb","ares_dup","ares_process_fd","ares_process_fd","ares_parse_ns_reply","ares_free_hostent","ares_dns_parse","ares_search_dnsrec","ares_dns_record_destroy","ares_getnameinfo","ares_search","ares_free_hostent","ares_dns_record_destroy","ares_destroy","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


// c-ares headers

// Dummy callback functions
static void  id_000014_dummy_nameinfo_callback(void *arg, int status, int timeouts, 
                                    const char *node, const char *service) {
    // Do nothing
}

static void  id_000014_dummy_pending_write_callback(void *arg) {
    // Do nothing
}

static void  id_000014_dummy_callback(void *arg, int status, int timeouts, 
                          const unsigned char *abuf, int alen) {
    // Do nothing
}

static void  id_000014_dummy_callback_dnsrec(void *arg, ares_status_t status, size_t timeouts,
                                 const ares_dns_record_t *dnsrec) {
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    ares_channel_t *channel = nullptr;
    ares_channel_t *duplicated_channel = nullptr;
    struct hostent *host = nullptr;
    ares_dns_record_t *dnsrec = nullptr;
    char *servers_csv = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    
    // Create input file from fuzzer data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        goto cleanup;
    }
    
    // Create output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        goto cleanup;
    }
    
    // Initialize ares channel
    if (ares_init(&channel) != ARES_SUCCESS) {
        goto cleanup;
    }
    
    // 1. Get servers CSV from channel
    servers_csv = ares_get_servers_csv(channel);
    if (servers_csv) {
        // Could potentially use this CSV string for something
        ares_free_string(servers_csv);
    }
    
    // 2. Set pending write callback
    ares_set_pending_write_cb(channel,  id_000014_dummy_pending_write_callback, nullptr);
    
    // 3. Duplicate the channel
    if (ares_dup(&duplicated_channel, channel) == ARES_SUCCESS && duplicated_channel) {
        // Use the duplicated channel for subsequent operations
        
        // 4. Process file descriptors
        int in_fd = fuzz_fileno(in_file);
        int out_fd = fuzz_fileno(out_file);
        
        // Process input file descriptor for reading
        ares_process_fd(duplicated_channel, in_fd, fuzz_int32_t_1);
        // Process output file descriptor for writing
        ares_process_fd(duplicated_channel, ARES_SOCKET_BAD, out_fd);
        
        // 5. Parse NS reply from input data
        if (size > 0) {
            // We need to ensure we don't read beyond size
            int parse_size = (size < INT_MAX) ? (int)size : INT_MAX;
            // This might fail if data isn't valid DNS response, but that's OK for fuzzing
            ares_parse_ns_reply(data, parse_size, &host);
            
            if (host) {
                ares_free_hostent(host);
                host = nullptr;
            }
        }
        
        // 6. Create a DNS record from input data for search_dnsrec
        if (size > 0) {
            // Try to parse the input as a DNS record
            ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_2, &dnsrec);
            
            if (parse_status == ARES_SUCCESS && dnsrec) {
                // 7. Search using the DNS record
                ares_search_dnsrec(duplicated_channel, dnsrec, 
                                   id_000014_dummy_callback_dnsrec, nullptr);
                
                ares_dns_record_destroy(dnsrec);
                dnsrec = nullptr;
            }
        }
        
        // 8. Prepare socket address for getnameinfo
        struct sockaddr_in sa;
        memset(&sa, 0, sizeof(sa));
        sa.sin_family = AF_INET;
        
        // Use some data from input for the address if available
        if (size >= 4) {
            memcpy(&sa.sin_addr.s_addr, data, 4);
        } else {
            // Default to loopback
            sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        }
        sa.sin_port = htons(80);
        
        // 9. Call getnameinfo
        ares_getnameinfo(duplicated_channel, (struct sockaddr *)&sa, sizeof(sa),
                        ARES_NI_NOFQDN | ARES_NI_NUMERICHOST,
                         id_000014_dummy_nameinfo_callback, nullptr);
        
        // 10. Perform a search operation
        // Extract a potential domain name from the input data
        char domain_name[256] = {0};
        if (size > 0) {
            // Try to find a null-terminated string in the input
            size_t copy_len = (size < 255) ? size : 255;
            memcpy(domain_name, data, copy_len);
            domain_name[copy_len] = '\0';
            
            // Ensure it looks somewhat like a domain name
            // If not, use a default
            if (strlen(domain_name) == 0 || strchr(domain_name, '.') == nullptr) {
                strcpy(domain_name, "example.com");
            }
        } else {
            strcpy(domain_name, "example.com");
        }
        
        ares_search(duplicated_channel, domain_name, C_IN, T_A, 
                    id_000014_dummy_callback, nullptr);
    }
    
cleanup:
    // Clean up resources
    if (host) {
        ares_free_hostent(host);
    }
    
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    if (duplicated_channel) {
        ares_destroy(duplicated_channel);
    }
    
    if (channel) {
        ares_destroy(channel);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
        // Remove the output file we created
        unlink("output_file");
    }
    
    // Cleanup library
    ares_library_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
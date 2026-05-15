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
//<ID> 558
//<Prompt> []
/*<Combination>: [
*/
//<score> 56, nr_unique_branch: 14
//<Quality> {"density":8,"unique_branches":{"ares_gethostbyname_file_int":[[312,7,312,29,0,0,4,0],[332,32,332,55,0,0,4,1]],"ares_dns_rr_set_bin_own":[[1202,7,1202,58,0,0,4,0],[1203,7,1203,59,0,0,4,0]],"ares_dns_record_rr_del":[[455,7,455,21,0,0,4,1],[455,25,455,56,0,0,4,1],[459,11,459,15,0,0,4,1],[460,5,460,29,0,0,4,0],[463,5,463,32,0,0,4,1],[466,5,466,33,0,0,4,1]],"ares_save_options":[[222,7,222,46,0,0,4,0]],"ares_dns_rr_set_bin":[[1264,7,1264,29,0,0,4,0]],"ares_is_localhost":[[377,7,377,15,0,0,4,0],[381,7,382,35,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_server_state_callback","ares_set_pending_write_cb","ares_gethostbyname_file","ares_save_options","ares_parse_aaaa_reply","ares_free_hostent","ares_dns_parse","ares_dns_record_rr_add","ares_dns_rr_set_bin","ares_dns_record_rr_del","ares_dns_record_destroy","ares_cancel","ares_free_hostent","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_server_state_callback","ares_set_pending_write_cb","ares_gethostbyname_file","ares_save_options","ares_parse_aaaa_reply","ares_free_hostent","ares_dns_parse","ares_dns_record_rr_add","ares_dns_rr_set_bin","ares_dns_record_rr_del","ares_dns_record_destroy","ares_cancel","ares_free_hostent","ares_destroy","ares_library_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified c-ares APIs to simulate DNS response processing and host lookup:

*/



// Custom callback functions
static void  id_000036_test_server_state_callback(const char *name, ares_bool_t up, 
                                       int timeout, void *user_data) {}

static void  id_000036_test_pending_write_callback(void *user_data) {}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Skip empty inputs
    if (size < 4) {
        return 0;
    }

    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set up callbacks
    ares_set_server_state_callback(channel,  id_000036_test_server_state_callback, nullptr);
    ares_set_pending_write_cb(channel,  id_000036_test_pending_write_callback, nullptr);

    // 1. Use ares_gethostbyname_file - read hostname from input data
    struct hostent *host_ent = nullptr;
    // Extract a null-terminated hostname from the input
    size_t hostname_len = std::min(size, (size_t)256);
    std::vector<char> hostname_buf(hostname_len + 1);
    memcpy(hostname_buf.data(), data, hostname_len);
    hostname_buf[hostname_len] = '\0';
    
    // Try to resolve from file
    FILE *hosts_file = fmemopen((void *)data, std::min(size, (size_t)1024), "rb");
    if (hosts_file) {
        // Note: ares_gethostbyname_file would use system's hosts file
        // We're just calling it with our input as potential hostname
        ares_gethostbyname_file(channel, hostname_buf.data(), AF_INET, &host_ent);
        assert_file_closed(&hosts_file);;
    }

    // 2. Use ares_save_options to save channel configuration
    struct ares_options saved_options;
    int saved_optmask = 0;
    ares_save_options(channel, &saved_options, &saved_optmask);

    // 3. Use ares_parse_aaaa_reply - parse input as DNS AAAA response
    struct hostent *aaaa_host = nullptr;
    struct ares_addr6ttl addr6ttls[10];
    int naddr6ttls = 0;
    
    // Ensure we have enough data for parsing
    if (size >= sizeof(HEADER)) {
        // Create a copy of input data for parsing
        std::vector<unsigned char> abuf(data, data + size);
        
        // Parse as AAAA reply
        ares_parse_aaaa_reply(abuf.data(), (int)size, 
                             &aaaa_host, addr6ttls, &naddr6ttls);
        
        // Clean up if hostent was allocated
        if (aaaa_host) {
            ares_free_hostent(aaaa_host);
        }
    }

    // 4. Use ares_dns_record_rr_del and ares_dns_rr_set_bin
    // Create a DNS record
    ares_dns_record_t *dnsrec = nullptr;
    
    // Try to parse input as DNS record
    if (size >= 12) {  // Minimum DNS header size
        // Create a buffer for DNS parsing
        std::vector<unsigned char> dns_buf(data, data + std::min(size, (size_t)512));
        
        // Parse DNS record
        ares_status_t status = ares_dns_parse(dns_buf.data(), dns_buf.size(), 
                                             0, &dnsrec);
        
        if (status == ARES_SUCCESS && dnsrec) {
            // Add a resource record to the answer section
            ares_dns_rr_t *rr = nullptr;
            status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                                           "example.com", ARES_REC_TYPE_A,
                                           ARES_CLASS_IN, 300);
            
            if (status == ARES_SUCCESS && rr) {
                // Set binary data in the RR using input data
                size_t bin_len = std::min(size, (size_t)16);
                std::vector<unsigned char> bin_data(data, data + bin_len);
                
                // Try to set binary data (for appropriate RR types)
                ares_dns_rr_set_bin(rr, ARES_RR_A_ADDR, 
                                   bin_data.data(), bin_len);
                
                // Delete the RR we just added (idx 0)
                ares_dns_record_rr_del(dnsrec, ARES_SECTION_ANSWER, 0);
            }
            
            // Clean up DNS record
            ares_dns_record_destroy(dnsrec);
        }
    }

    // 5. Cancel any pending operations
    ares_cancel(channel);

    // Clean up resources
    if (host_ent) {
        ares_free_hostent(host_ent);
    }
    
    // Free any data allocated by ares_save_options
    if (saved_optmask & ARES_OPT_SERVERS) {
        free(saved_options.servers);
    }
    if (saved_optmask & ARES_OPT_DOMAINS) {
        for (int i = 0; i < saved_options.ndomains; i++) {
            free(saved_options.domains[i]);
        }
        free(saved_options.domains);
    }
    if (saved_optmask & ARES_OPT_LOOKUPS) {
        free(saved_options.lookups);
    }
    if (saved_optmask & ARES_OPT_SORTLIST) {
        free(saved_options.sortlist);
    }
    if (saved_optmask & ARES_OPT_RESOLVCONF) {
        free(saved_options.resolvconf_path);
    }
    if (saved_optmask & ARES_OPT_HOSTS_FILE) {
        free(saved_options.hosts_path);
    }

    // Destroy channel
    ares_destroy(channel);
    
    // Cleanup library
    ares_library_cleanup();

    assert_file_closed(&hosts_file);
	return 0;
}
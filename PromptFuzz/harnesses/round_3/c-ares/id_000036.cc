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
//<ID> 704
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 25
//<Quality> {"density":8,"unique_branches":{"ares_servers_update":[[786,11,786,41,0,0,4,0]],"inet_ntop6":[[194,11,194,24,0,0,4,1]],"ares_sconfig_append":[[474,9,474,31,0,0,4,1]],"ares_save_options":[[109,7,109,40,0,0,4,0],[115,7,115,44,0,0,4,0],[119,7,119,40,0,0,4,0]],"ares_expand_name":[[91,7,91,22,0,0,4,1],[91,26,91,38,0,0,4,1],[91,42,91,51,0,0,4,1],[91,55,91,69,0,0,4,1]],"ares_save_opt_servers":[[71,9,71,39,0,0,4,0]],"ares_sconfig_linklocal":[[403,9,405,15,0,0,4,1]],"ares_server_create":[[671,7,671,37,0,0,4,0]],"ares_expand_name_validated":[[45,7,45,22,0,0,4,1],[45,26,45,38,0,0,4,1],[45,42,45,51,0,0,4,1],[45,55,45,69,0,0,4,1],[49,7,49,21,0,0,4,1],[49,25,49,47,0,0,4,1],[56,7,56,8,0,0,4,0],[62,7,62,18,0,0,4,1],[67,7,67,29,0,0,4,1],[73,7,73,29,0,0,4,0],[73,7,73,29,0,0,4,1]],"parse_nameserver":[[351,9,351,31,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_socket_configure_callback","ares_set_servers_csv","ares_set_servers_ports_csv","ares_create_query","ares_expand_name","ares_free_string","ares_process_pending_write","ares_dns_rr_get_keys","ares_dns_rr_key_tostr","ares_save_options","ares_destroy_options","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_socket_configure_callback","ares_set_servers_csv","ares_set_servers_ports_csv","ares_create_query","ares_expand_name","ares_free_string","ares_process_pending_write","ares_dns_rr_get_keys","ares_dns_rr_key_tostr","ares_save_options","ares_destroy_options","ares_destroy","ares_library_cleanup"],"visited":1}
/**/



extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<14) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Initialize channel
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    options.timeout = 2000;
    options.tries = 2;
    optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;

    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 1. Set socket configure callback
    auto sock_config_callback = [](ares_socket_t socket_fd, int type, void *user_data) -> int {
        // Simple socket configuration
        int optval = 1;
        setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        return 0;
    };
    
    ares_set_socket_configure_callback(channel, sock_config_callback, nullptr);

    // 2. Use input data for servers CSV if we have enough bytes
    if (size > 0) {
        // Create null-terminated string from input for servers CSV
        std::string servers_csv((const char*)data, std::min(size, (size_t)1024));
        
        // Try to set servers using CSV format
        ares_set_servers_csv(channel, servers_csv.c_str());
        
        // Also try the ports CSV version
        ares_set_servers_ports_csv(channel, servers_csv.c_str());
    }

    // 3. Create query using input data
    unsigned char *query_buf = nullptr;
    int query_len = 0;
    
    // Extract domain name from input data if possible
    std::string domain_name;
    if (size >= 4) {
        // Use part of input as domain name (limit to 253 chars max for DNS)
        size_t name_len = std::min(size, (size_t)253);
        domain_name = std::string((const char*)data, name_len);
        
        // Replace non-printable characters with 'a' to make valid-ish domain
        for (char &c : domain_name) {
            if (c < 33 || c > 126) c = 'a';
        }
        
        // Ensure it has at least one dot
        if (domain_name.find('.') == std::string::npos) {
            domain_name += ".example.com";
        }
        
        // Generate random ID
        unsigned short query_id = 0;
        if (size >= 2) {
            query_id = (data[0] << 8) | data[1];
        }
        
        // Create DNS query
        ares_create_query(domain_name.c_str(), 
                         ns_c_in,  // DNS class IN
                         ns_t_a,   // Query type A
                         fuzz_uint16_t_2,
                         fuzz_int32_t_3,        // Recursion desired
                         &query_buf,
                         &query_len,
                         fuzz_int32_t_4);    // Max UDP size
    }

    // 4. Try to expand name from input data
    if (size > 0) {
        char *expanded_name = nullptr;
        long enclen = 0;
        
        // Use input as encoded name buffer
        const unsigned char *encoded = data;
        int alen = static_cast<int>(std::min(size, (size_t)INT_MAX));
        
        // Try to expand name (this may fail, which is OK)
        ares_expand_name(encoded, encoded, alen, &expanded_name, &enclen);
        
        if (expanded_name) {
            ares_free_string(expanded_name);
        }
    }

    // 5. Process pending writes
    ares_process_pending_write(channel);

    // 6. Get DNS RR keys and convert to string
    size_t key_count = 0;
    const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(ARES_REC_TYPE_A, &key_count);
    
    if (keys && key_count > 0) {
        // Convert first key to string
        const char *key_str = ares_dns_rr_key_tostr(keys[0]);
        (void)key_str; // Use to avoid unused variable warning
    }

    // 7. Save channel options
    struct ares_options saved_options;
    int saved_optmask = 0;
    
    memset(&saved_options, 0, sizeof(saved_options));
    ares_save_options(channel, &saved_options, &saved_optmask);
    
    // Clean up saved options
    ares_destroy_options(&saved_options);

    // Clean up query buffer if allocated
    if (query_buf) {
        free(query_buf);
    }

    // Clean up channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();

    return 0;
}
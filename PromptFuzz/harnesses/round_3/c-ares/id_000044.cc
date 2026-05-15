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
//<ID> 836
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"ares_search":[[455,18,455,51,0,0,4,1]]},"library_calls":["ares_library_init","ares_library_cleanup","ares_library_cleanup","ares_init_options","ares_init","ares_library_cleanup","ares_get_servers","ares_free_data","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_record_destroy","ares_expand_string","ares_free_string","ares_gethostbyname_file","ares_free_hostent","ares_search","ares_destroy","ares_destroy_options","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_init","ares_get_servers","ares_free_data","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_record_destroy","ares_expand_string","ares_free_string","ares_gethostbyname_file","ares_free_hostent","ares_search","ares_destroy","ares_destroy_options","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

// Callback function for ares_search
static void  id_000044_search_callback(void* arg, int status, int timeouts, 
                           const unsigned char* abuf, int alen) {
    // Just acknowledge callback, no further processing needed for fuzzing
    (void)arg; (void)status; (void)timeouts; (void)abuf; (void)alen;
}

// Callback for ares_gethostbyname_file
static void  id_000044_host_callback(void* arg, int status, int timeouts, 
                         struct hostent* hostent) {
    (void)arg; (void)status; (void)timeouts;
    if (hostent) {
        ares_free_hostent(hostent);
    }
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<51) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        ares_library_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    ares_channel_t* channel = nullptr;
    struct ares_options options;
    int optmask = 0;

    // Initialize channel with options
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    options.timeout = 2000;
    options.tries = 3;
    optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;

    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        // Fall back to simple init
        if (ares_init(&channel) != ARES_SUCCESS) {
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            ares_library_cleanup();
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }

    // Get servers (even if empty)
    struct ares_addr_node* servers = nullptr;
    ares_get_servers(channel, &servers);
    if (servers) {
        ares_free_data(servers);
    }

    // Parse DNS data if we have enough input
    ares_dns_record_t* dnsrec = nullptr;
    if (size >= 12) {  // Minimum DNS header size
        ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_2, &dnsrec);
        
        if (parse_status == ARES_SUCCESS && dnsrec) {
            // Get resource record count in answer section
            size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
            
            // Try to get first RR if exists
            if (rr_count > 0) {
                const ares_dns_rr_t* rr = ares_dns_record_rr_get_const(
                    dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_3);
                (void)rr;  // Use rr to avoid unused variable warning
            }
            
            // Clean up DNS record
            ares_dns_record_destroy(dnsrec);
        }
    }

    // Try to expand a string from the input
    if (size >= 2) {
        unsigned char* expanded = nullptr;
        long enclen = 0;
        
        // Use a portion of input as encoded string
        size_t use_size = size > 100 ? 100 : size;
        int expand_result = ares_expand_string(
            data, 
            data, 
            static_cast<int>(use_size), 
            &expanded, 
            &enclen);
        
        if (expand_result == ARES_SUCCESS && expanded) {
            ares_free_string(expanded);
        }
    }

    // Try host file lookup with random name
    struct hostent* host = nullptr;
    char test_name[] = "test.example.com";
    ares_gethostbyname_file(channel, fuzz_str_4, fuzz_int32_t_5, &host);
    if (host) {
        ares_free_hostent(host);
    }

    // Perform a search (won't actually send due to no network setup)
    ares_search(channel, fuzz_str_6, C_IN, T_A,  id_000044_search_callback, nullptr);

    // Clean up
    ares_destroy(channel);
    ares_destroy_options(&options);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    unlink("output_file");
    
    // Final cleanup
    ares_library_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}
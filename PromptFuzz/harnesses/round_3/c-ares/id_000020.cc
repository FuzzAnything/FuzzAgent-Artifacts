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
//<ID> 360
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["ares_library_init","ares_library_cleanup","ares_init_options","ares_library_cleanup","ares_set_local_ip6","ares_set_local_dev","ares_dns_record_create","ares_destroy","ares_library_cleanup","ares_dns_record_query_add","ares_dns_record_query_cnt","ares_dns_record_rr_get","ares_dns_rr_get_u16","ares_dns_rr_set_u16","ares_dns_rr_get_u32","ares_dns_rr_del_opt_byid","ares_timeout","ares_queue_active_queries","ares_gethostbyname_file","ares_free_hostent","ares_dns_record_destroy","ares_destroy","ares_free_data","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_local_ip6","ares_set_local_dev","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_cnt","ares_dns_record_rr_get","ares_dns_rr_get_u16","ares_dns_rr_set_u16","ares_dns_rr_get_u32","ares_dns_rr_del_opt_byid","ares_timeout","ares_queue_active_queries","ares_gethostbyname_file","ares_free_hostent","ares_dns_record_destroy","ares_destroy","ares_free_data","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<27) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize ares channel with options
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Set some basic options
    options.flags = ARES_FLAG_STAYOPEN;
    optmask |= ARES_OPT_FLAGS;
    options.timeout = 2000;
    optmask |= ARES_OPT_TIMEOUTMS;
    options.tries = 3;
    optmask |= ARES_OPT_TRIES;

    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Set local IPv6 address (using dummy address)
    unsigned char local_ip6[16] = {0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    ares_set_local_ip6(channel, local_ip6);

    // Set local device name
    ares_set_local_dev(channel, fuzz_str_2);

    // Create DNS record
    ares_dns_record_t *dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_3, fuzz_uint16_t_4, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) 
        != ARES_SUCCESS) {
        ares_destroy(channel);
        assert_file_closed(&in_file);;
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Add query to DNS record
    if (size > 0) {
        // Use first byte as query name length (clamped to reasonable size)
        size_t name_len = data[0] % 64;
        if (name_len > 0 && name_len < size) {
            char query_name[65] = {0};
            memcpy(query_name, data + 1, name_len);
            query_name[name_len] = '\0';
            
            ares_dns_record_query_add(dnsrec, query_name, 
                                     ARES_REC_TYPE_A, ARES_CLASS_IN);
        }
    }

    // Get resource record from DNS record (try to get first query if exists)
    if (ares_dns_record_query_cnt(dnsrec) > 0) {
        ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_5);
        if (rr) {
            // Try to get and set U16 value
            unsigned short orig_val = ares_dns_rr_get_u16(rr, ARES_RR_A_ADDR);
            ares_dns_rr_set_u16(rr, ARES_RR_A_ADDR, orig_val + 1);
            
            // Try to get U32 value
            unsigned int u32_val = ares_dns_rr_get_u32(rr, ARES_RR_A_ADDR);
            
            // Try to delete option by ID (though this RR may not have options)
            ares_dns_rr_del_opt_byid(rr, ARES_RR_A_ADDR, fuzz_uint16_t_6);
        }
    }

    // Get timeout for channel
    struct timeval maxtv, tv;
    struct timeval *timeout = ares_timeout(channel, &maxtv, &tv);

    // Check active queries
    size_t active_queries = ares_queue_active_queries(channel);

    // Use gethostbyname_file with input data as hosts file
    FILE *hosts_file = fopen("input_file", "wb");
    if (hosts_file) {
        fwrite(data, 1, size, hosts_file);
        assert_file_closed(&hosts_file);;
        
        struct hostent *host = nullptr;
        if (size > 0) {
            // Try to extract a hostname from input data
            size_t hostname_len = size > 1 ? data[1] % 64 : 10;
            char hostname[65];
            if (hostname_len > 0 && hostname_len + 2 < size) {
                memcpy(hostname, data + 2, hostname_len);
                hostname[hostname_len] = '\0';
                
                ares_gethostbyname_file(channel, hostname, fuzz_int32_t_7, &host);
                
                if (host) {
                    ares_free_hostent(host);
                }
            }
        }
        
        remove("input_file");
    }

    // Create output file for any potential output
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some dummy output
        fwrite("test", 1, 4, out_file);
        assert_file_closed(&out_file);;
        remove("output_file");
    }

    // Cleanup
    ares_dns_record_destroy(dnsrec);
    ares_destroy(channel);
    assert_file_closed(&in_file);;
    
    // Free any data that might have been allocated
    ares_free_data(nullptr);
    
    ares_library_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&hosts_file);
	assert_file_closed(&in_file);
	return 0;
}
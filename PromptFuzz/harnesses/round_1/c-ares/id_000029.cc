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
//<ID> 721
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 5
//<Quality> {"density":8,"unique_branches":{"ares_strerror":[[34,5,34,22,0,0,4,1],[36,5,36,22,0,0,4,0],[38,5,38,23,0,0,4,0],[40,5,40,24,0,0,4,0],[42,5,42,24,0,0,4,0]]},"library_calls":["ares_library_initialized","ares_library_init","ares_init_options","ares_library_cleanup","ares_set_query_enqueue_cb","ares_destroy","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_process_fd","ares_parse_naptr_reply","ares_free_data","ares_parse_ptr_reply","ares_free_hostent","ares_dns_record_create","ares_dns_record_set_id","ares_dns_record_query_add","ares_dns_record_query_set_name","ares_dns_record_query_get","ares_dns_rcode_tostr","ares_inet_ntop","ares_dns_record_rr_get","ares_dns_rr_del_opt_byid","ares_strerror","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_initialized","ares_library_init","ares_init_options","ares_set_query_enqueue_cb","ares_process_fd","ares_parse_naptr_reply","ares_free_data","ares_parse_ptr_reply","ares_free_hostent","ares_dns_record_create","ares_dns_record_set_id","ares_dns_record_query_add","ares_dns_record_query_set_name","ares_dns_record_query_get","ares_dns_rcode_tostr","ares_inet_ntop","ares_dns_record_rr_get","ares_dns_rr_del_opt_byid","ares_strerror","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Include necessary c-ares headers
extern "C" {
}

// Custom callback function for query enqueue
static void  id_000029_query_enqueue_callback(void *user_data) {
    // Empty callback for testing
}

// Fuzzer entry point
extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<63) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 4) {
        return 0;
    }

    // Initialize library if not already initialized
    if (!ares_library_initialized()) {
        int init_result = ares_library_init(ARES_LIB_INIT_ALL);
        if (init_result != ARES_SUCCESS) {
            return 0;
        }
    }

    // Initialize channel
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    options.timeout = 2000;
    options.tries = 3;
    
    int optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set query enqueue callback
    ares_set_query_enqueue_cb(channel,  id_000029_query_enqueue_callback, nullptr);

    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int read_fd = fuzz_fileno(in_file);
    int write_fd = fuzz_fileno(out_file);

    // Process file descriptors with c-ares
    ares_process_fd(channel, read_fd, write_fd);

    // Parse NAPTR reply (if we have enough data)
    struct ares_naptr_reply *naptr_reply = nullptr;
    size_t naptr_size = size / 2;
    if (naptr_size > 0) {
        // Use first half of data for NAPTR parsing
        ares_parse_naptr_reply(data, static_cast<int>(naptr_size), &naptr_reply);
        
        // Clean up NAPTR reply if parsed
        if (naptr_reply) {
            ares_free_data(naptr_reply);
        }
    }

    // Parse PTR reply (if we have enough data)
    struct hostent *host = nullptr;
    size_t ptr_size = size - naptr_size;
    if (ptr_size > 0) {
        // Use second half of data for PTR parsing
        // Create dummy address for PTR lookup
        struct in_addr dummy_addr;
        dummy_addr.s_addr = 0x01020304;  // 1.2.3.4 in network byte order
        
        ares_parse_ptr_reply(data + naptr_size, 
                            static_cast<int>(ptr_size),
                            &dummy_addr,
                            sizeof(dummy_addr),
                            fuzz_int32_t_1,
                            &host);
        
        // Clean up hostent if parsed
        if (host) {
            ares_free_hostent(host);
        }
    }

    // Create DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, 
                                                fuzz_uint16_t_2,  // ID
                                                fuzz_uint16_t_3,      // Flags
                                                ARES_OPCODE_QUERY,
                                                ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Set DNS record ID
        ares_dns_record_set_id(dnsrec, fuzz_uint16_t_4);
        
        // Add query to DNS record
        char query_name[] = "example.com";
        status = ares_dns_record_query_add(dnsrec, 
                                          fuzz_str_5,
                                          ARES_REC_TYPE_A,
                                          ARES_CLASS_IN);
        
        // If query was added, try to set its name
        if (status == ARES_SUCCESS && size > 0) {
            // Use first byte of data to determine if we should try to set name
            if (data[0] < 128) {  // Random condition
                ares_dns_record_query_set_name(dnsrec, fuzz_uint64_t_7, fuzz_str_6);
            }
            
            // Get query information
            const char *retrieved_name = nullptr;
            ares_dns_rec_type_t retrieved_type;
            ares_dns_class_t retrieved_class;
            
            ares_dns_record_query_get(dnsrec, fuzz_uint64_t_8, 
                                     &retrieved_name, 
                                     &retrieved_type, 
                                     &retrieved_class);
            
            // Test DNS RCODE to string conversion
            const char *rcode_str = ares_dns_rcode_tostr(ARES_RCODE_NOERROR);
            
            // Test inet_ntop (convert IP address to string)
            char ip_str[INET6_ADDRSTRLEN];
            struct in_addr test_addr;
            test_addr.s_addr = 0x01020304;  // 1.2.3.4
            
            const char *ntop_result = ares_inet_ntop(AF_INET, 
                                                    &test_addr, 
                                                    ip_str, 
                                                    sizeof(ip_str));
            
            // Get a resource record from the DNS record (though none exist yet)
            ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, 
                                                      ARES_SECTION_ANSWER, 
                                                      fuzz_uint64_t_9);
            
            // If we somehow got an RR, try to delete an option by ID
            if (rr) {
                // Use a random key and opt from the data
                if (size >= 2) {
                    ares_dns_rr_key_t key = static_cast<ares_dns_rr_key_t>(data[0] % 256);
                    unsigned short opt = static_cast<unsigned short>(data[1]);
                    ares_dns_rr_del_opt_byid(rr, key, opt);
                }
            }
            
            // Test ares_strerror with some error codes
            for (int i = 0; i < 5; i++) {
                const char *error_str = ares_strerror(fuzz_int32_t_10);
            }
        }
        
        // Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // Clean up resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    ares_destroy(channel);
    ares_library_cleanup();

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(write_fd);
	assert_fd_closed(read_fd);
	return 0;
}
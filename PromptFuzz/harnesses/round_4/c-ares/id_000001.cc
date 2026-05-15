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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 3
//<Quality> {"density":9,"unique_branches":{"ares_dns_rec_type_tostr":[[194,5,194,26,0,0,4,0],[196,5,196,29,0,0,4,0],[204,5,204,26,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_parse_ns_reply","ares_free_hostent","ares_dns_parse","ares_dns_rec_type_tostr","ares_dns_record_get_flags","ares_dns_record_rr_get_const","ares_dns_record_rr_get","ares_dns_rr_set_ttl","ares_query_dnsrec","ares_process_fd","ares_dns_record_destroy","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_parse_ns_reply","ares_free_hostent","ares_dns_parse","ares_dns_rec_type_tostr","ares_dns_record_get_flags","ares_dns_record_rr_get_const","ares_dns_record_rr_get","ares_dns_rr_set_ttl","ares_query_dnsrec","ares_process_fd","ares_dns_record_destroy","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Include necessary headers for c-ares

// Callback for ares_query_dnsrec
static void  id_000001_query_callback(void* arg, ares_status_t status, 
                          size_t timeouts, const ares_dns_record_t* dnsrec) {
    // This is just a placeholder callback for the fuzzer
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t* channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Create a FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Read DNS reply data from the file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    if (file_size <= 0) {
        assert_file_closed(&in_file);;
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    unsigned char* abuf = (unsigned char*)malloc(file_size);
    if (!abuf) {
        assert_file_closed(&in_file);;
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    size_t bytes_read = fread(abuf, 1, file_size, in_file);
    assert_file_closed(&in_file);;

    if (bytes_read == 0) {
        free(abuf);
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Parse NS reply using the input data
    struct hostent* host = nullptr;
    int parse_result = ares_parse_ns_reply(abuf, bytes_read, &host);
    
    // Clean up hostent if allocated
    if (host) {
        ares_free_hostent(host);
    }

    // 2. Parse the same data as a DNS record
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t parse_status = ares_dns_parse(abuf, bytes_read, fuzz_uint32_t_1, &dnsrec);

    if (parse_status == ARES_SUCCESS && dnsrec) {
        // 3. Use ares_dns_section_tostr (note: this function wasn't in the provided list,
        // but we'll use ares_dns_rec_type_tostr as an alternative)
        const char* type_str = ares_dns_rec_type_tostr(ARES_REC_TYPE_NS);
        (void)type_str; // Avoid unused variable warning

        // 4. Get DNS record flags
        unsigned short flags = ares_dns_record_get_flags(dnsrec);
        (void)flags; // Avoid unused variable warning

        // Get the first resource record from answer section if it exists
        size_t answer_count = 0;
        // We need to get the count - iterate until we find one
        for (size_t i = 0; ; i++) {
            const ares_dns_rr_t* rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, i);
            if (!rr) {
                if (i == 0) answer_count = 0;
                break;
            }
            answer_count = i + 1;
            
            // 5. Set TTL on the resource record
            // First get the RR in mutable form
            ares_dns_rr_t* mutable_rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_2);
            if (mutable_rr) {
                // Use some data from input for TTL
                unsigned int ttl_value = (data[0] << 24) | (data[1] << 16) | 
                                        (data[2] << 8) | data[3];
                ttl_value = ttl_value % 86400; // Limit to reasonable TTL
                
                ares_dns_rr_set_ttl(mutable_rr, ttl_value);
            }
        }

        // 6. Perform a DNS query
        unsigned short qid = 0;
        const char* query_name = "example.com";
        
        // Use different query types based on input data
        ares_dns_rec_type_t query_type = ARES_REC_TYPE_NS;
        if (size > 1) {
            uint8_t type_selector = data[0] % 4;
            switch (type_selector) {
                case 0: query_type = ARES_REC_TYPE_A; break;
                case 1: query_type = ARES_REC_TYPE_NS; break;
                case 2: query_type = ARES_REC_TYPE_CNAME; break;
                case 3: query_type = ARES_REC_TYPE_MX; break;
            }
        }
        
        ares_query_dnsrec(channel, fuzz_str_3, ARES_CLASS_IN, 
                         query_type,  id_000001_query_callback, nullptr, &qid);
        
        // Process any pending operations
        ares_process_fd(channel, fuzz_int32_t_4, fuzz_int32_t_5);
        
        // Destroy the DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // Clean up
    free(abuf);
    
    // Cancel any pending operations
    ares_cancel(channel);
    
    // Destroy channel
    ares_destroy(channel);
    
    // Cleanup library
    ares_library_cleanup();

    assert_file_closed(&in_file);
	return 0;
}
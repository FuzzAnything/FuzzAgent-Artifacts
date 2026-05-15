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
//<ID> 537
//<Prompt> []
/*<Combination>: [
*/
//<score> 4.3333335, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["ares_library_initialized","ares_library_init","ares_init_options","ares_set_query_enqueue_cb","ares_destroy","ares_parse_caa_reply","ares_free_data","ares_parse_naptr_reply","ares_free_data","ares_parse_ptr_reply","ares_free_hostent","ares_inet_ntop","ares_dns_rcode_tostr","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_set_name","ares_dns_record_query_get","ares_dns_record_rr_get","ares_dns_rr_get_opt_byid","ares_dns_record_destroy","ares_send","ares_process_fd","ares_destroy"],"critical_calls":["ares_library_initialized","ares_library_init","ares_init_options","ares_set_query_enqueue_cb","ares_parse_caa_reply","ares_free_data","ares_parse_naptr_reply","ares_free_data","ares_parse_ptr_reply","ares_free_hostent","ares_inet_ntop","ares_dns_rcode_tostr","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_set_name","ares_dns_record_query_get","ares_dns_record_rr_get","ares_dns_rr_get_opt_byid","ares_dns_record_destroy","ares_send","ares_process_fd","ares_destroy"],"visited":2}
/**/


extern "C" {
}

// Dummy callbacks for demonstration
static void  id_000018_dummy_query_enqueue_cb(void* user_data) {
    // Do nothing
}

static void  id_000018_dummy_ares_callback(void* arg, int status, int timeouts, 
                                const unsigned char* abuf, int alen) {
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_9, fdp);
	//fuzzer shim end}




    // Initialize library if needed
    if (!ares_library_initialized()) {
        ares_library_init(ARES_LIB_INIT_ALL);
    }
    
    ares_channel_t* channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize channel
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        return 0;
    }
    
    // Set query enqueue callback
    ares_set_query_enqueue_cb(channel,  id_000018_dummy_query_enqueue_cb, nullptr);
    
    // Create FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Parse CAA reply using input data
    struct ares_caa_reply* caa_out = nullptr;
    int caa_ret = ares_parse_caa_reply(data, size, &caa_out);
    if (caa_out) {
        ares_free_data(caa_out);
    }
    
    // Parse NAPTR reply using input data
    struct ares_naptr_reply* naptr_out = nullptr;
    int naptr_ret = ares_parse_naptr_reply(data, size, &naptr_out);
    if (naptr_out) {
        ares_free_data(naptr_out);
    }
    
    // Parse PTR reply using input data
    // Use dummy address for PTR lookup
    struct hostent* host_out = nullptr;
    in_addr_t dummy_addr = 0x01020304; // 1.2.3.4
    int ptr_ret = ares_parse_ptr_reply(data, size, &dummy_addr, 
                                      sizeof(dummy_addr), fuzz_int32_t_2, &host_out);
    if (host_out) {
        ares_free_hostent(host_out);
    }
    
    // Convert address to string
    char addr_str[INET_ADDRSTRLEN];
    const char* ntop_result = ares_inet_ntop(AF_INET, &dummy_addr, 
                                             addr_str, sizeof(addr_str));
    
    // Get rcode string
    const char* rcode_str = ares_dns_rcode_tostr(ARES_RCODE_NOERROR);
    
    // Create DNS record
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t rec_status = ares_dns_record_create(&dnsrec, fuzz_uint16_t_3, fuzz_uint16_t_4, 
                                                      ARES_OPCODE_QUERY, 
                                                      ARES_RCODE_NOERROR);
    
    if (rec_status == ARES_SUCCESS && dnsrec) {
        // Add a query first
        size_t query_idx = 0;
        ares_dns_record_query_add(dnsrec, fuzz_str_5, ARES_REC_TYPE_A, 
                                  ARES_CLASS_IN);
        
        // Try to set query name if we have valid data
        if (size > 0) {
            // Extract a string from the input data
            char* name_buf = (char*)malloc(size + 1);
            if (name_buf) {
                memcpy(name_buf, data, size > 255 ? 255 : size);
                name_buf[size > 255 ? 255 : size] = '\0';
                ares_dns_record_query_set_name(dnsrec, fuzz_uint64_t_6, name_buf);
                free(name_buf);
            }
        }
        
        // Get query information
        const char* name = nullptr;
        ares_dns_rec_type_t qtype;
        ares_dns_class_t qclass;
        ares_dns_record_query_get(dnsrec, fuzz_uint64_t_7, &name, &qtype, &qclass);
        
        // Try to get OPT RR (won't exist but demonstrates API usage)
        ares_dns_rr_t* rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_8);
        if (rr) {
            const unsigned char* val = nullptr;
            size_t val_len = 0;
            ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS, fuzz_uint16_t_9, &val, &val_len);
        }
        
        // Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Send query using input data as query buffer
    if (size > 0) {
        ares_send(channel, data, size > 65535 ? 65535 : size, 
                  id_000018_dummy_ares_callback, nullptr);
        
        // Process pending operations
        ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    ares_destroy(channel);
    
    assert_file_closed(&in_file);
	return 0;
}
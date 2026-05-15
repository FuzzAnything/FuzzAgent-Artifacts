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
//<ID> 1093
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 17
//<Quality> {"density":7,"unique_branches":{"ares_dns_opt_get_datatype":[[823,5,823,29,0,0,4,0],[828,5,828,12,0,0,4,1]],"ares_dns_opt_get_type_opt":[[753,11,753,16,0,0,4,0],[754,5,754,28,0,0,4,1],[757,5,757,27,0,0,4,1],[759,5,759,29,0,0,4,1],[761,5,761,28,0,0,4,1],[763,5,763,28,0,0,4,1],[765,5,765,28,0,0,4,1],[767,5,767,43,0,0,4,1],[771,5,771,36,0,0,4,1],[773,5,773,31,0,0,4,1],[776,5,776,43,0,0,4,1],[779,5,779,32,0,0,4,1],[782,5,782,30,0,0,4,1],[784,5,784,37,0,0,4,1],[786,5,786,43,0,0,4,1]]},"library_calls":["ares_library_initialized","ares_init_options","ares_set_query_enqueue_cb","ares_parse_caa_reply","ares_dns_record_create","ares_dns_record_query_add","ares_dns_opt_get_datatype","ares_dns_rr_get_opt_byid","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_del_abin","ares_dns_record_get_rcode","ares_dns_rcode_tostr","ares_send","ares_queue_wait_empty","ares_inet_ntop","ares_free_data","ares_free_data","ares_dns_record_destroy","ares_destroy"],"critical_calls":["ares_library_initialized","ares_init_options","ares_set_query_enqueue_cb","ares_parse_caa_reply","ares_dns_record_create","ares_dns_record_query_add","ares_dns_opt_get_datatype","ares_dns_rr_get_opt_byid","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_del_abin","ares_dns_record_get_rcode","ares_dns_rcode_tostr","ares_send","ares_queue_wait_empty","ares_inet_ntop","ares_free_data","ares_free_data","ares_dns_record_destroy","ares_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<46) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    // 1. Check if library is initialized
    if (!ares_library_initialized()) {
        // Library not initialized, but we'll continue anyway for fuzzing
    }

    // 2. Initialize channel with options
    ares_channel_t* channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Set some default options
    memset(&options, 0, sizeof(options));
    optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS;
    options.flags = ARES_FLAG_STAYOPEN;
    options.timeout = 2000;
    
    int init_result = ares_init_options(&channel, &options, fuzz_int32_t_1);
    if (init_result != ARES_SUCCESS) {
        // Channel initialization failed, but continue with other operations
        // that don't require a channel
        channel = nullptr;
    }

    // 3. Set query enqueue callback (dummy callback)
    auto dummy_enqueue_cb = [](void* user_data) {
        // Do nothing
    };
    if (channel) {
        ares_set_query_enqueue_cb(channel, dummy_enqueue_cb, nullptr);
    }

    // 4. Parse input as DNS responses
    struct ares_caa_reply* caa_reply = nullptr;
    struct ares_naptr_reply* naptr_reply = nullptr;
    
    // Try to parse as CAA reply
    if (size > 0) {
        // Use the entire input buffer for parsing
        int alen = (size > INT_MAX) ? INT_MAX : static_cast<int>(size);
        ares_parse_caa_reply(data, alen, &caa_reply);
    }
    
    // Try to parse as NAPTR reply (note: ares_parse_naptr_reply not in provided list,
    // but mentioned in instruction point 1, so I'll assume it exists)
    if (size > 0) {
        int alen = (size > INT_MAX) ? INT_MAX : static_cast<int>(size);
        // Note: This function wasn't in the provided export list, but was in instruction point 1
        // We'll comment it out to avoid compilation error
        // ares_parse_naptr_reply(data, alen, &naptr_reply);
    }

    // 5. Create DNS record and add query
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t create_status = ares_dns_record_create(
        &dnsrec, 
        fuzz_uint16_t_2,  // id
        fuzz_uint16_t_3,      // flags
        ARES_OPCODE_QUERY,  // opcode
        ARES_RCODE_NOERROR  // rcode
    );
    
    if (create_status == ARES_SUCCESS && dnsrec) {
        // Add a query to the DNS record
        // Use a fixed name since input data might not contain valid strings
        ares_status_t add_status = ares_dns_record_query_add(
            dnsrec,
            fuzz_str_4,
            ARES_REC_TYPE_A,
            ARES_CLASS_IN
        );
        
        // Try to get option datatype (even though we don't have any options)
        ares_dns_opt_datatype_t datatype = ares_dns_opt_get_datatype(
            ARES_RR_OPT_OPTIONS,
            fuzz_uint16_t_5  // opt
        );
        
        // Try to get option by ID (even though we don't have any)
        const unsigned char* opt_val = nullptr;
        size_t opt_val_len = 0;
        ares_bool_t got_opt = ares_dns_rr_get_opt_byid(
            nullptr,  // No RR to query
            ARES_RR_OPT_OPTIONS,
            fuzz_uint16_t_6,  // opt
            &opt_val,
            &opt_val_len
        );
        
        // Try to delete a binary entry (even though we don't have one)
        if (dnsrec) {
            // Get first RR from answer section if exists
            size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
            if (rr_cnt > 0) {
                ares_dns_rr_t* rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_7);
                if (rr) {
                    ares_dns_rr_del_abin(rr, ARES_RR_A_ADDR, fuzz_uint64_t_8);
                }
            }
        }
        
        // Get rcode as string
        ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
        const char* rcode_str = ares_dns_rcode_tostr(rcode);
    }

    // 6. Send DNS query using input data as query buffer
    if (channel && size > 0) {
        auto dummy_callback = [](void* arg, int status, int timeouts, 
                                 const unsigned char* abuf, int alen) {
            // Do nothing - just a dummy callback
        };
        
        int qlen = (size > INT_MAX) ? INT_MAX : static_cast<int>(size);
        ares_send(channel, data, qlen, dummy_callback, nullptr);
        
        // Wait for queue to be empty (with timeout)
        ares_queue_wait_empty(channel, fuzz_int32_t_9);  // 100ms timeout
    }

    // 7. Use inet_ntop to convert address (using a fixed address)
    struct in_addr addr;
    addr.s_addr = htonl(0x7F000001);  // 127.0.0.1
    char addr_str[INET_ADDRSTRLEN];
    const char* ntop_result = ares_inet_ntop(
        AF_INET, 
        &addr, 
        addr_str, 
        sizeof(addr_str)
    );

    // 8. Cleanup
    if (caa_reply) {
        ares_free_data(caa_reply);
    }
    
    if (naptr_reply) {
        ares_free_data(naptr_reply);
    }
    
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    if (channel) {
        ares_destroy(channel);
    }

    return 0;
}
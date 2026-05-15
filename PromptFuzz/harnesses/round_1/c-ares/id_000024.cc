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
//<ID> 560
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["ares_library_init","ares_library_cleanup","ares_init","ares_library_cleanup","ares_dup","ares_destroy","ares_library_cleanup","ares_set_local_ip4","ares_dns_record_create","ares_dns_record_get_flags","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_get_class","ares_dns_opt_get_datatype","ares_gethostbyaddr","ares_fds","ares_process","ares_timeout","ares_dns_record_destroy","ares_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_dup","ares_set_local_ip4","ares_dns_record_create","ares_dns_record_get_flags","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_get_class","ares_dns_opt_get_datatype","ares_gethostbyaddr","ares_fds","ares_process","ares_timeout","ares_dns_record_destroy","ares_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<30) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




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

    // Initialize channels
    ares_channel_t *channel1 = nullptr;
    ares_channel_t *channel2 = nullptr;
    
    if (ares_init(&channel1) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Use ares_dup API - duplicate the channel
    if (ares_dup(&channel2, channel1) != ARES_SUCCESS) {
        ares_destroy(channel1);
        assert_file_closed(&in_file);;
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Extract IP address from input data for local IP setting
    unsigned int local_ip = 0;
    if (size >= sizeof(local_ip)) {
        memcpy(&local_ip, data, sizeof(local_ip));
        local_ip = ntohl(local_ip); // Convert to host byte order
    } else if (size > 0) {
        // Use partial data if available
        memcpy(&local_ip, data, size);
    }

    // Use ares_set_local_ip4 API
    ares_set_local_ip4(channel1, fuzz_uint32_t_1);

    // Create a dummy DNS record to use other APIs
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, fuzz_uint16_t_2, 
                                                   ARES_FLAG_RD | ARES_FLAG_AA,
                                                   ARES_OPCODE_QUERY,
                                                   ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Use ares_dns_record_get_flags API
        unsigned short flags = ares_dns_record_get_flags(dnsrec);
        
        // Add a query to the DNS record
        if (size > 10) {
            // Use part of input data as query name (sanitized)
            char query_name[256] = {0};
            size_t query_len = size > 255 ? 255 : size;
            for (size_t i = 0; i < query_len; i++) {
                if (data[i] >= 32 && data[i] <= 126) { // Printable ASCII
                    query_name[i] = data[i];
                } else {
                    query_name[i] = 'a' + (data[i] % 26);
                }
            }
            query_name[query_len] = '\0';
            
            // Add query to DNS record
            ares_dns_record_query_add(dnsrec, query_name, 
                                       ARES_REC_TYPE_A, ARES_CLASS_IN);
        }

        // Get RR class (we'll create a dummy RR first)
        ares_dns_rr_t *rr = nullptr;
        if (size > 20) {
            status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                                           fuzz_str_3, ARES_REC_TYPE_A,
                                           ARES_CLASS_IN, fuzz_uint32_t_4);
            if (status == ARES_SUCCESS && rr) {
                // Use ares_dns_rr_get_class API
                ares_dns_class_t rr_class = ares_dns_rr_get_class(rr);
                
                // Use ares_dns_opt_get_datatype API
                ares_dns_opt_datatype_t datatype = 
                    ares_dns_opt_get_datatype(ARES_RR_A_ADDR, fuzz_uint16_t_5);
            }
        }
    }

    // Prepare address for gethostbyaddr
    struct in_addr addr;
    if (size >= sizeof(struct in_addr)) {
        memcpy(&addr, data, sizeof(struct in_addr));
        
        // Use ares_gethostbyaddr API with callback
        ares_gethostbyaddr(channel2, &addr, sizeof(addr), fuzz_int32_t_6,
                           [](void *arg, int status, int timeouts,
                              const struct hostent *hostent) {
                               // Callback implementation
                               if (status == ARES_SUCCESS && hostent) {
                                   // Process hostent if needed
                               }
                           }, nullptr);
        
        // Process pending requests
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 10000; // 10ms timeout
        
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        int nfds = ares_fds(channel2, &read_fds, &write_fds);
        if (nfds > 0) {
            ares_process(channel2, &read_fds, &write_fds);
        }
        
        ares_timeout(channel2, nullptr, &tv);
    }

    // Cleanup
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    ares_destroy(channel2);
    ares_destroy(channel1);
    assert_file_closed(&in_file);;
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
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
//<ID> 1056
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"ares_process_fd":[[282,9,282,28,0,0,4,1]]},"library_calls":["ares_library_initialized","ares_init_options","ares_set_query_enqueue_cb","ares_destroy","ares_parse_caa_reply","ares_free_data","ares_parse_naptr_reply","ares_free_data","ares_parse_ptr_reply","ares_free_hostent","ares_dns_record_create","ares_dns_record_query_add","ares_dns_rr_get_opt_byid","ares_dns_rcode_tostr","ares_mkquery","ares_send","ares_dns_record_destroy","ares_inet_ntop","ares_process_fd","ares_process_fd","ares_destroy"],"critical_calls":["ares_library_initialized","ares_init_options","ares_set_query_enqueue_cb","ares_parse_caa_reply","ares_free_data","ares_parse_naptr_reply","ares_free_data","ares_parse_ptr_reply","ares_free_hostent","ares_dns_record_create","ares_dns_record_query_add","ares_dns_rr_get_opt_byid","ares_dns_rcode_tostr","ares_mkquery","ares_send","ares_dns_record_destroy","ares_inet_ntop","ares_process_fd","ares_process_fd","ares_destroy"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    // 1. Check if library is initialized and init if not
    if (!ares_library_initialized()) {
        // Note: ares_library_init() is not in the provided list, but we need initialization
        // We'll use ares_init_options which initializes implicitly
    }

    // 2. Initialize channel with options
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    // Set some minimal options
    options.timeout = 2000; // 2 seconds
    options.tries = 2;
    optmask = ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    int status = ares_init_options(&channel, &options, fuzz_int32_t_1);
    if (status != ARES_SUCCESS) {
        return 0;
    }

    // 3. Set query enqueue callback (minimal implementation)
    ares_set_query_enqueue_cb(channel, nullptr, nullptr);

    // 4. Create output file for any writes
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        ares_destroy(channel);
        assert_file_closed(&out_file);
	return 0;
    }
    int out_fd = fuzz_fileno(out_file);

    // 5. Parse replies using the input data
    // Parse as CAA reply
    struct ares_caa_reply *caa_out = nullptr;
    ares_parse_caa_reply(data, (int)size, &caa_out);
    if (caa_out) {
        ares_free_data(caa_out);
    }

    // Parse as NAPTR reply
    struct ares_naptr_reply *naptr_out = nullptr;
    ares_parse_naptr_reply(data, (int)size, &naptr_out);
    if (naptr_out) {
        struct ares_naptr_reply *next = naptr_out;
        while (next) {
            struct ares_naptr_reply *temp = next;
            next = next->next;
            ares_free_data(temp);
        }
    }

    // Parse as PTR reply
    struct hostent *host_out = nullptr;
    // Use dummy address for PTR parsing
    struct in_addr dummy_addr;
    dummy_addr.s_addr = 0x01010101; // 1.1.1.1
    ares_parse_ptr_reply(data, (int)size, &dummy_addr, sizeof(dummy_addr), fuzz_int32_t_2, &host_out);
    if (host_out) {
        ares_free_hostent(host_out);
    }

    // 6. Create DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t dns_status = ares_dns_record_create(&dnsrec, fuzz_uint16_t_3, 
                                                     ARES_FLAG_QR | ARES_FLAG_RD,
                                                     ARES_OPCODE_QUERY, 
                                                     ARES_RCODE_NOERROR);
    if (dns_status == ARES_SUCCESS && dnsrec) {
        // Add a query
        ares_dns_record_query_add(dnsrec, fuzz_str_4, 
                                 ARES_REC_TYPE_A, ARES_CLASS_IN);
        
        // Try to get an option (will fail since no RRs added, but API called)
        const unsigned char *opt_val;
        size_t opt_len;
        // Use a dummy RR key (will return false)
        ares_dns_rr_get_opt_byid(nullptr, ARES_RR_OPT_OPTIONS, fuzz_uint16_t_5, &opt_val, &opt_len);
        
        // Convert rcode to string
        const char *rcode_str = ares_dns_rcode_tostr(ARES_RCODE_NOERROR);
        
        // Send the DNS query
        unsigned char *qbuf = nullptr;
        int qlen = 0;
        if (ares_mkquery("example.com", C_IN, T_A, fuzz_uint16_t_6, 1, &qbuf, &qlen) == ARES_SUCCESS) {
            ares_send(channel, qbuf, fuzz_int32_t_7, [](void*, int, int, const unsigned char*, int){}, nullptr);
            free(qbuf);
        }
        
        ares_dns_record_destroy(dnsrec);
    }

    // 7. Use inet_ntop to convert an address
    char addr_str[INET6_ADDRSTRLEN];
    struct in6_addr addr6;
    memset(&addr6, 0, sizeof(addr6));
    ares_inet_ntop(fuzz_int32_t_8, &addr6, addr_str, sizeof(addr_str));

    // 8. Process the channel with file descriptors
    // Create input file from fuzzing data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        int in_fd = fuzz_fileno(in_file);
        ares_process_fd(channel, in_fd, in_fd);
        assert_file_closed(&in_file);;
    }

    // 9. Process output file descriptor
    ares_process_fd(channel, ARES_SOCKET_BAD, out_fd);

    // 10. Cleanup
    assert_file_closed(&out_file);;
    ares_destroy(channel);
    
    // Clean up library if this is the final use
    // Note: In fuzzing, we might want to keep library initialized between runs
    // ares_library_cleanup();

    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	return 0;
}
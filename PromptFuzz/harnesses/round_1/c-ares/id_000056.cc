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
//<ID> 1055
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["ares_library_initialized","ares_library_init","ares_init_options","ares_library_cleanup","ares_set_query_enqueue_cb","ares_parse_caa_reply","ares_parse_naptr_reply","ares_parse_ptr_reply","ares_inet_ntop","ares_dns_record_create","ares_dns_record_query_add","ares_dns_rcode_tostr","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_opt_byid","ares_dns_record_destroy","ares_send","ares_process_fd","ares_free_data","ares_free_data","ares_free_hostent","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_initialized","ares_library_init","ares_init_options","ares_set_query_enqueue_cb","ares_parse_caa_reply","ares_parse_naptr_reply","ares_parse_ptr_reply","ares_inet_ntop","ares_dns_record_create","ares_dns_record_query_add","ares_dns_rcode_tostr","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_opt_byid","ares_dns_record_destroy","ares_send","ares_process_fd","ares_free_data","ares_free_data","ares_free_hostent","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Dummy callback functions for ares
static void  id_000056_dummy_callback(void *arg, int status, int timeouts, 
                          const unsigned char *abuf, int alen) {}

static void  id_000056_dummy_query_enqueue_cb(void *arg) {}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	

    // Open input file from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize library
    if (!ares_library_initialized()) {
        // Note: ares_library_init is actually void, but we include for API coverage
        ares_library_init(ARES_LIB_INIT_ALL);
    }

    // Create channel
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    options.timeout = 1000;
    options.tries = 2;
    
    int optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    int init_result = ares_init_options(&channel, &options, optmask);
    
    if (init_result != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Set query enqueue callback
    ares_set_query_enqueue_cb(channel,  id_000056_dummy_query_enqueue_cb, nullptr);

    // Parse CAA reply from input data
    struct ares_caa_reply *caa_out = nullptr;
    ares_parse_caa_reply(data, (int)(size > 1024 ? 1024 : size), &caa_out);
    
    // Parse NAPTR reply from input data
    struct ares_naptr_reply *naptr_out = nullptr;
    ares_parse_naptr_reply(data, (int)(size > 1024 ? 1024 : size), &naptr_out);
    
    // Parse PTR reply from input data (using dummy address)
    struct hostent *host = nullptr;
    struct in_addr dummy_addr;
    dummy_addr.s_addr = htonl(0x08080808); // 8.8.8.8
    ares_parse_ptr_reply(data, (int)(size > 1024 ? 1024 : size), 
                        &dummy_addr, sizeof(dummy_addr), fuzz_int32_t_1, &host);

    // Test ares_inet_ntop
    char ip_str[INET6_ADDRSTRLEN];
    struct in_addr test_addr;
    test_addr.s_addr = htonl(0x01020304);
    const char *ntop_result = ares_inet_ntop(AF_INET, &test_addr, 
                                           ip_str, sizeof(ip_str));

    // Test DNS record creation and manipulation
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t create_status = ares_dns_record_create(&dnsrec, 
        fuzz_uint16_t_2,  // id
        ARES_FLAG_QR | ARES_FLAG_RD,  // flags
        ARES_OPCODE_QUERY,  // opcode
        ARES_RCODE_NOERROR  // rcode
    );

    if (create_status == ARES_SUCCESS && dnsrec) {
        // Add a query to the DNS record
        ares_dns_record_query_add(dnsrec, fuzz_str_3, 
                                 ARES_REC_TYPE_A, ARES_CLASS_IN);
        
        // Test getting RCODE string
        const char *rcode_str = ares_dns_rcode_tostr(ARES_RCODE_NOERROR);
        
        // Test ares_dns_rr_get_opt_byid (won't find anything in new record)
        const unsigned char *opt_val = nullptr;
        size_t val_len = 0;
        if (ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER) > 0) {
            const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(
                dnsrec, ARES_SECTION_ANSWER, 0);
            ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS, 
                                   fuzz_uint16_t_4, &opt_val, &val_len);
        }
        
        // Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // Send query using the input data as query buffer
    // Note: In real usage, this would send a DNS query over network
    // For fuzzing, we just call the API with our data
    ares_send(channel, data, (int)(size > 512 ? 512 : size), 
              id_000056_dummy_callback, nullptr);

    // Process file descriptor (using input file descriptor)
    int fd = fuzz_fileno(in_file);
    if (fd >= 0) {
        ares_process_fd(channel, fd, ARES_SOCKET_BAD);
    }

    // Cleanup
    if (caa_out) {
        ares_free_data(caa_out);
    }
    if (naptr_out) {
        ares_free_data(naptr_out);
    }
    if (host) {
        ares_free_hostent(host);
    }
    
    assert_file_closed(&in_file);;
    ares_destroy(channel);
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
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
//<ID> 1058
//<Prompt> []
/*<Combination>: [
*/
//<score> 68, nr_unique_branch: 12
//<Quality> {"density":17,"unique_branches":{"ares_dns_rcode_tostr":[[912,11,912,16,0,0,4,0],[925,5,925,29,0,0,4,0],[927,5,927,28,0,0,4,0],[929,5,929,28,0,0,4,0],[931,5,931,28,0,0,4,0],[933,5,933,28,0,0,4,0],[935,5,935,29,0,0,4,0]],"ares_dns_rcode_isvalid":[[51,5,51,28,0,0,4,0],[53,5,53,28,0,0,4,0],[54,5,54,28,0,0,4,0],[55,5,55,29,0,0,4,0]],"ares_dns_record_create":[[63,43,63,73,0,0,4,0]]},"library_calls":["ares_library_initialized","ares_library_init","ares_init_options","ares_library_cleanup","ares_set_query_enqueue_cb","ares_dns_record_create","ares_dns_record_query_add","ares_parse_caa_reply","ares_parse_naptr_reply","ares_parse_ptr_reply","ares_inet_ntop","ares_dns_rcode_tostr","ares_free_data","ares_free_data","ares_free_hostent","ares_dns_record_rr_get_const","ares_dns_record_rr_get_const","ares_dns_rr_get_opt_byid","ares_send","ares_process_fd","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_initialized","ares_library_init","ares_init_options","ares_set_query_enqueue_cb","ares_dns_record_create","ares_dns_record_query_add","ares_parse_caa_reply","ares_parse_naptr_reply","ares_parse_ptr_reply","ares_inet_ntop","ares_dns_rcode_tostr","ares_free_data","ares_free_data","ares_free_hostent","ares_dns_record_rr_get_const","ares_dns_record_rr_get_const","ares_dns_rr_get_opt_byid","ares_send","ares_process_fd","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<265) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeCharArray(char, fuzz_char_array_1, fuzz_char_array_size_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
	
		const char * fuzzer_var_ares_dns_record_query_add_6_1 = fuzz_char_array_1;//fuzzer shim end}




    // 1. Initialize c-ares library
    if (!ares_library_initialized()) {
        if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS)
            return 0;
    }

    // 2. Create ares channel with options
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    options.timeout = 2000;
    options.tries = 2;
    int optmask = ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 3. Set query enqueue callback (dummy)
    ares_set_query_enqueue_cb(channel, [](void*){}, nullptr);

    // 4. Create DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    if (size > 0) {
        rcode = static_cast<ares_dns_rcode_t>(data[0] % 16); // Valid rcode values
    }
    
    ares_status_t status = ares_dns_record_create(&dnsrec, 
        (size > 1 ? data[1] : 1234),  // ID
        (size > 2 ? data[2] : 0x0100), // Flags (RD bit set)
        ARES_OPCODE_QUERY,
        rcode);

    // 5. Add queries to DNS record
    if (status == ARES_SUCCESS && dnsrec && size > 10) {
        // Use part of input as query name
        char name[256] = {0};
        size_t name_len = (size < 256) ? size : 256;
        for (size_t i = 0; i < name_len && i < size; i++) {
            if (data[i] >= 32 && data[i] < 127) {
                name[i] = data[i];
            } else {
                name[i] = 'a' + (data[i] % 26);
            }
        }
        name[name_len-1] = '\0';
        
        ares_dns_record_query_add(dnsrec, fuzzer_var_ares_dns_record_query_add_6_1, 
            ARES_REC_TYPE_A, 
            ARES_CLASS_IN);
    }

    // 6. Parse input as DNS replies
    if (size > 0) {
        // Parse as CAA reply
        struct ares_caa_reply *caa_out = nullptr;
        ares_parse_caa_reply(data, static_cast<int>(size), &caa_out);
        
        // Parse as NAPTR reply
        struct ares_naptr_reply *naptr_out = nullptr;
        ares_parse_naptr_reply(data, static_cast<int>(size), &naptr_out);
        
        // Parse as PTR reply
        struct hostent *host_out = nullptr;
        struct in_addr addr;
        addr.s_addr = (size > 3) ? *reinterpret_cast<const uint32_t*>(data) : INADDR_LOOPBACK;
        ares_parse_ptr_reply(data, static_cast<int>(size), 
                           &addr, sizeof(addr), fuzz_int32_t_2, &host_out);
        
        // Convert address to string
        char addr_str[INET_ADDRSTRLEN] = {0};
        ares_inet_ntop(AF_INET, &addr, addr_str, sizeof(addr_str));
        
        // Get rcode string
        const char *rcode_str = ares_dns_rcode_tostr(rcode);
        
        // Free parsed data
        if (caa_out) ares_free_data(caa_out);
        if (naptr_out) ares_free_data(naptr_out);
        if (host_out) ares_free_hostent(host_out);
    }

    // 7. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some test data
        const char *test = "Test output\n";
        fwrite(test, 1, strlen(test), out_file);
        assert_file_closed(&out_file);;
    }

    // 8. Try to get OPT from DNS RR (if available)
    if (dnsrec && ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, 0)) {
        const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, 0);
        if (rr) {
            const unsigned char *val = nullptr;
            size_t val_len = 0;
            ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS, fuzz_uint16_t_3, &val, &val_len);
        }
    }

    // 9. Send DNS query using input data as buffer
    if (size > 0 && channel) {
        ares_send(channel, data, static_cast<int>(size), 
                  [](void*, int, int, const unsigned char*, int){}, nullptr);
        
        // Create input file for processing
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            int fd = fuzz_fileno(in_file);
            if (fd >= 0) {
                ares_process_fd(channel, fd, ARES_SOCKET_BAD);
            }
            assert_file_closed(&in_file);;
        }
    }

    // 10. Cleanup
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    if (channel) {
        ares_destroy(channel);
    }
    
    ares_library_cleanup();
    
    assert_file_closed(&out_file);
	return 0;
}
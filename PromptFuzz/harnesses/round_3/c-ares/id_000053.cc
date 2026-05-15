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
//<ID> 927
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 3
//<Quality> {"density":11,"unique_branches":{"ares_dns_rr_get_bin":[[860,9,860,21,0,0,4,0]],"ares_round_up_pow2":[[75,7,75,22,0,0,4,1]],"ares_dns_rr_set_bin_own":[[1209,9,1209,21,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_server_state_callback","ares_parse_naptr_reply","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_set_addr6","ares_dns_rr_set_bin","ares_dns_rr_get_bin","ares_dns_rr_get_addr6","ares_dns_record_destroy","ares_free_data","ares_destroy","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_gethostbyname_file","ares_free_hostent","ares_dns_rec_type_fromstr","ares_reinit","ares_dns_rcode_tostr","ares_getaddrinfo","ares_freeaddrinfo","ares_freeaddrinfo","ares_process","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_server_state_callback","ares_parse_naptr_reply","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_set_addr6","ares_dns_rr_set_bin","ares_dns_rr_get_bin","ares_dns_rr_get_addr6","ares_dns_record_destroy","ares_free_data","ares_gethostbyname_file","ares_free_hostent","ares_dns_rec_type_fromstr","ares_reinit","ares_dns_rcode_tostr","ares_getaddrinfo","ares_freeaddrinfo","ares_freeaddrinfo","ares_process","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<81) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    ares_channel_t *channel = nullptr;
    struct ares_options options = {0};
    int optmask = 0;

    // Create channel with options
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set server state callback
    ares_set_server_state_callback(channel, nullptr, nullptr);

    // Parse NAPTR reply from input data
    struct ares_naptr_reply *naptr_reply = nullptr;
    if (size > 0) {
        ares_parse_naptr_reply(data, static_cast<int>(size), &naptr_reply);
    }

    // Create DNS record
    ares_dns_record_t *dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_2, fuzz_uint16_t_3, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        // Add query to DNS record
        ares_dns_record_query_add(dnsrec, fuzz_str_4, ARES_REC_TYPE_A, ARES_CLASS_IN);

        // Add resource record
        ares_dns_rr_t *rr = nullptr;
        if (ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                   fuzz_str_5, ARES_REC_TYPE_AAAA, ARES_CLASS_IN, fuzz_uint32_t_6) == ARES_SUCCESS) {
            // Set IPv6 address in RR
            struct ares_in6_addr addr6 = {{{0}}};
            if (size >= 16) {
                memcpy(addr6._S6_un._S6_u8, data, 16);
                ares_dns_rr_set_addr6(rr, ARES_RR_AAAA_ADDR, &addr6);
            }

            // Set binary data in RR
            if (size > 0) {
                size_t bin_len = size > 100 ? 100 : size;
                ares_dns_rr_set_bin(rr, ARES_RR_TXT_DATA, data, bin_len);
            }

            // Get binary data back
            size_t get_len = 0;
            const unsigned char *bin_data = ares_dns_rr_get_bin(rr, ARES_RR_TXT_DATA, &get_len);

            // Get IPv6 address back
            const struct ares_in6_addr *get_addr6 = ares_dns_rr_get_addr6(rr, ARES_RR_AAAA_ADDR);
        }

        // Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // Free NAPTR reply data
    if (naptr_reply) {
        ares_free_data(naptr_reply);
    }

    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
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
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Use gethostbyname_file with the input file
    struct hostent *host = nullptr;
    if (size > 0) {
        // Write some host data to the file for parsing
        fwrite("127.0.0.1 localhost\n", 1, 20, out_file);
        fflush(out_file);
        
        // Try to parse host from file (note: ares_gethostbyname_file reads from system files,
        // not from our arbitrary file, but we call it for API coverage)
        ares_gethostbyname_file(channel, fuzz_str_7, fuzz_int32_t_8, &host);
        if (host) {
            ares_free_hostent(host);
        }
    }

    // Try to convert string to DNS record type
    ares_dns_rec_type_t qtype;
    ares_dns_rec_type_fromstr(&qtype, fuzz_str_9);

    // Reinitialize channel
    ares_reinit(channel);

    // Set up file descriptors for ares_process
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    if (in_fd >= 0) {
        FD_SET(in_fd, &read_fds);
    }
    if (out_fd >= 0) {
        FD_SET(out_fd, &write_fds);
    }

    // Get RCODE string
    const char *rcode_str = ares_dns_rcode_tostr(ARES_RCODE_NOERROR);

    // Start async getaddrinfo operation
    struct ares_addrinfo_hints hints = {0};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    ares_getaddrinfo(channel, fuzz_str_10, fuzz_str_11, &hints, 
                     [](void *arg, int status, int timeouts, struct ares_addrinfo *result) {
                         if (result) {
                             ares_freeaddrinfo(result);
                         }
                     }, nullptr);

    // Process pending operations
    ares_process(channel, &read_fds, &write_fds);

    // Cancel all pending operations
    ares_cancel(channel);

    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    ares_destroy(channel);
    ares_library_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
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
//<ID> 614
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 3
//<Quality> {"density":6,"unique_branches":{"ares_open_connection":[[409,7,409,30,0,0,4,0],[412,9,412,16,0,0,4,1]],"ares_gethostbyaddr":[[111,7,111,22,0,0,4,1]]},"library_calls":["ares_library_init","ares_library_cleanup","ares_library_cleanup","ares_init_options","ares_library_cleanup","ares_set_socket_callback","ares_set_socket_configure_callback","ares_dns_parse","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_duplicate","ares_dns_record_destroy","ares_dns_record_destroy","ares_gethostbyaddr","ares_process_fd","ares_destroy","ares_library_initialized","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_socket_callback","ares_set_socket_configure_callback","ares_dns_parse","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_duplicate","ares_dns_record_destroy","ares_dns_record_destroy","ares_gethostbyaddr","ares_process_fd","ares_destroy","ares_library_initialized","ares_library_cleanup"],"visited":0}
/**/


// Define callback types
static void  id_000035_host_callback(void* arg, int status, int timeouts, const struct hostent* host) {
    // Callback for ares_gethostbyaddr
    if (host) {
        ares_free_hostent(const_cast<struct hostent*>(host));
    }
}

static int  id_000035_sock_create_callback(ares_socket_t sockfd, int type, void* user_data) {
    // Socket creation callback
    return 0;
}

static int  id_000035_sock_config_callback(ares_socket_t sockfd, int type, void* user_data) {
    // Socket configuration callback
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create input file from fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        ares_library_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize channel
    ares_channel_t* channel = nullptr;
    if (ares_init_options(&channel, nullptr, fuzz_int32_t_1) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        ares_library_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set socket callbacks
    ares_set_socket_callback(channel,  id_000035_sock_create_callback, nullptr);
    ares_set_socket_configure_callback(channel,  id_000035_sock_config_callback, nullptr);

    // Parse DNS record from input data
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_2, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Get opcode and convert to string
        ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
        const char* opcode_str = ares_dns_opcode_tostr(opcode);
        
        // Duplicate DNS record
        ares_dns_record_t* dup_rec = ares_dns_record_duplicate(dnsrec);
        
        // Write opcode string to output file
        if (opcode_str) {
            fwrite(opcode_str, strlen(opcode_str), 1, out_file);
        }
        
        // Clean up duplicated record
        if (dup_rec) {
            ares_dns_record_destroy(dup_rec);
        }
        
        // Clean up original record
        ares_dns_record_destroy(dnsrec);
    }

    // Try to perform reverse DNS lookup using input data as address
    if (size >= 4) {
        // Interpret first 4 bytes as IPv4 address
        struct in_addr addr;
        memcpy(&addr, data, 4);
        
        // Perform reverse DNS lookup
        ares_gethostbyaddr(channel, &addr, sizeof(addr), fuzz_int32_t_3,  id_000035_host_callback, nullptr);
        
        // Process the lookup (non-blocking, will call callback)
        ares_process_fd(channel, ARES_SOCKET_BAD, fuzz_int32_t_4);
    }

    // Clean up
    ares_destroy(channel);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Check if library was initialized (for completeness)
    if (ares_library_initialized()) {
        ares_library_cleanup();
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
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
//<ID> 250
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 36
//<Quality> {"density":8,"unique_branches":{"ares_dns_rr_set_ttl":[[539,7,539,21,0,0,4,1]],"ares_parse_ptr_reply_dnsrec":[[54,7,54,29,0,0,4,1],[59,7,59,19,0,0,4,0],[59,7,59,19,0,0,4,1],[66,7,66,22,0,0,4,1],[74,7,74,35,0,0,4,1],[79,7,79,19,0,0,4,0],[79,23,79,34,0,0,4,0],[81,9,81,40,0,0,4,1],[92,7,92,33,0,0,4,1],[100,15,100,26,0,0,4,0],[100,15,100,26,0,0,4,1],[104,9,104,19,0,0,4,1],[110,9,110,51,0,0,4,0],[110,9,110,51,0,0,4,1],[115,9,115,56,0,0,4,0],[115,9,115,56,0,0,4,1],[117,11,117,26,0,0,4,1],[124,9,124,54,0,0,4,0],[124,9,124,54,0,0,4,1],[145,9,145,25,0,0,4,1],[152,9,152,45,0,0,4,1],[159,7,159,20,0,0,4,0],[159,7,159,20,0,0,4,1],[168,7,168,30,0,0,4,1],[174,7,174,29,0,0,4,0],[174,7,174,29,0,0,4,1],[177,9,177,32,0,0,4,1]],"ares_set_socket_functions_ex":[[101,15,101,67,0,0,4,1],[102,9,102,44,0,0,4,1],[106,7,106,60,0,0,4,0]],"ares_parse_ptr_reply":[[194,7,194,19,0,0,4,1],[201,7,201,29,0,0,4,0],[201,7,201,29,0,0,4,1],[209,7,209,30,0,0,4,0],[209,7,209,30,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_socket_functions_ex","ares_getsock","ares_save_options","ares_destroy","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_parse_ptr_reply","ares_free_hostent","ares_expand_name","ares_free_string","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_ttl","ares_dns_rr_get_class","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_socket_functions_ex","ares_getsock","ares_save_options","ares_parse_ptr_reply","ares_free_hostent","ares_expand_name","ares_free_string","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_ttl","ares_dns_rr_get_class","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Custom socket functions structure
struct ares_socket_functions_ex  id_000020_custom_funcs = {
    .version = 0,
    .flags = 0,
    .asocket = nullptr,
    .aclose = nullptr,
    .asetsockopt = nullptr,
    .aconnect = nullptr,
    .arecvfrom = nullptr,
    .asendto = nullptr,
    .agetsockname = nullptr,
    .abind = nullptr,
    .aif_nametoindex = nullptr,
    .aif_indextoname = nullptr
};

// Callback for ares_parse_ptr_reply
static void  id_000020_host_callback(void* arg, int status, int timeouts, struct hostent* host) {
    (void)arg;
    (void)status;
    (void)timeouts;
    if (host) {
        ares_free_hostent(host);
    }
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Initialize library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create channel
    ares_channel_t* channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 1. Use ares_set_socket_functions_ex
    ares_set_socket_functions_ex(channel, & id_000020_custom_funcs, nullptr);

    // 2. Use ares_getsock
    ares_socket_t socks[ARES_GETSOCK_MAXNUM];
    int num_socks = ares_getsock(channel, socks, ARES_GETSOCK_MAXNUM);

    // 3. Use ares_save_options
    struct ares_options options;
    int optmask = 0;
    ares_save_options(channel, &options, &optmask);

    // Create a FILE* from input data for parsing
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Read data for parsing
    unsigned char* buffer = (unsigned char*)malloc(size);
    if (!buffer) {
        assert_file_closed(&in_file);;
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t bytes_read = fread(buffer, 1, size, in_file);
    assert_file_closed(&in_file);;

    if (bytes_read < 12) { // Minimum DNS header size
        free(buffer);
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Use ares_parse_ptr_reply
    // Extract IPv4 address from first 4 bytes of data
    struct in_addr addr;
    if (bytes_read >= 4) {
        memcpy(&addr, data, 4);
    } else {
        addr.s_addr = INADDR_ANY;
    }
    
    struct hostent* host = nullptr;
    int parse_result = ares_parse_ptr_reply(buffer, bytes_read, 
                                           &addr, sizeof(addr), 
                                           AF_INET, &host);
    if (host) {
        ares_free_hostent(host);
    }

    // 5. Use ares_expand_name
    // Try to expand a name from the buffer
    if (bytes_read > 12) {
        char* expanded_name = nullptr;
        long enclen = 0;
        
        // Start from offset 12 (after DNS header) for name expansion
        const unsigned char* name_ptr = buffer + 12;
        int remaining = bytes_read - 12;
        
        if (remaining > 0) {
            int expand_result = ares_expand_name(name_ptr, buffer, bytes_read, 
                                                &expanded_name, &enclen);
            if (expand_result == ARES_SUCCESS && expanded_name) {
                ares_free_string(expanded_name);
            }
        }
    }

    // 6. Create DNS record and use ares_dns_rr_set_ttl and ares_dns_rr_get_class
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, 12345, 
                                                  ARES_FLAG_QR | ARES_FLAG_RD, 
                                                  ARES_OPCODE_QUERY, 
                                                  ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Add a resource record
        ares_dns_rr_t* rr = nullptr;
        status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                                       "example.com", ARES_REC_TYPE_A,
                                       ARES_CLASS_IN, 300);
        
        if (status == ARES_SUCCESS && rr) {
            // 7. Use ares_dns_rr_set_ttl
            ares_dns_rr_set_ttl(rr, 3600);
            
            // 8. Use ares_dns_rr_get_class
            ares_dns_class_t rr_class = ares_dns_rr_get_class(rr);
            (void)rr_class; // Use variable to avoid unused warning
        }
        
        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // Clean up
    free(buffer);
    ares_destroy(channel);
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
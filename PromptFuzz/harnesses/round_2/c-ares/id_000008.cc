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
//<ID> 162
//<Prompt> []
/*<Combination>: [
*/
//<score> 58.666668, nr_unique_branch: 43
//<Quality> {"density":11,"unique_branches":{"ares_default_loopback_addrs":[[123,8,123,27,0,0,4,1],[123,31,123,49,0,0,4,1],[133,8,133,27,0,0,4,1],[133,31,133,48,0,0,4,0]],"hostent_nalias":[[53,13,53,36,0,0,4,0],[53,40,53,66,0,0,4,0],[53,40,53,66,0,0,4,1]],"hostent_naddr":[[74,13,74,38,0,0,4,0],[74,42,74,70,0,0,4,0],[74,42,74,70,0,0,4,1]],"ares_gethostbyname_file_int":[[295,7,295,22,0,0,4,1],[295,26,295,38,0,0,4,1],[295,42,295,54,0,0,4,1],[307,7,307,33,0,0,4,1],[312,7,312,29,0,0,4,1],[317,7,317,29,0,0,4,1],[332,7,332,28,0,0,4,0],[332,32,332,55,0,0,4,0]],"ares_addrinfo2hostent":[[126,7,126,20,0,0,4,1],[140,7,140,30,0,0,4,1]],"ares_create_query_int":[[37,32,37,34,0,0,4,0],[39,7,39,19,0,0,4,1],[39,23,39,35,0,0,4,1],[39,39,39,54,0,0,4,1],[50,7,50,29,0,0,4,1],[55,7,55,29,0,0,4,1]],"ares_set_local_ip6":[[559,7,559,22,0,0,4,1]],"ares_gethostbyname_file":[[343,7,343,22,0,0,4,1]],"ares_hostent_localhost":[[265,7,265,17,0,0,4,1],[271,7,271,29,0,0,4,1],[276,7,276,29,0,0,4,1]],"ares_parse_txt_reply_int":[[80,26,80,28,0,0,4,1],[95,11,95,13,0,0,4,1]],"ares_parse_txt_reply":[[129,7,129,15,0,0,4,1]],"ares_addrinfo_localhost":[[219,5,219,17,0,0,4,0],[221,5,221,19,0,0,4,1]],"ares_malloc_data":[[151,5,151,33,0,0,4,0]],"ares_send":[[254,7,254,22,0,0,4,1],[259,7,259,22,0,0,4,1],[259,26,259,43,0,0,4,1],[265,7,265,29,0,0,4,1],[271,7,271,19,0,0,4,1]],"ares_free_data":[[84,7,84,35,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_local_ip6","ares_set_local_ip6","ares_create_query","ares_send","ares_free_string","ares_parse_txt_reply","ares_free_data","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_cnt","ares_dns_opcode_tostr","ares_dns_record_destroy","ares_gethostbyname_file","ares_free_hostent","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_local_ip6","ares_create_query","ares_send","ares_free_string","ares_parse_txt_reply","ares_free_data","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_cnt","ares_dns_opcode_tostr","ares_dns_record_destroy","ares_gethostbyname_file","ares_free_hostent","ares_destroy","ares_library_cleanup"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set local IPv6 address if we have at least 16 bytes
    if (size >= 16) {
        ares_set_local_ip6(channel, data);
    } else {
        // Use default IPv6 address
        static const unsigned char default_ip6[16] = {
            0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
        };
        ares_set_local_ip6(channel, default_ip6);
    }

    // Create a DNS query
    unsigned char *query_buf = nullptr;
    int query_len = 0;
    const char *domain = "example.com";
    
    if (ares_create_query(domain, C_IN, T_TXT, 0x1234, 1, 
                         &query_buf, &query_len, 512) == ARES_SUCCESS) {
        // Send the query with a dummy callback
        ares_send(channel, query_buf, query_len, 
                 [](void*, int, int, const unsigned char*, int) {}, 
                 nullptr);
        
        // Clean up query buffer
        if (query_buf) {
            ares_free_string(query_buf);
        }
    }

    // Parse TXT reply from input data
    struct ares_txt_reply *txt_reply = nullptr;
    if (size > 0) {
        // Use first min(size, 4096) bytes as DNS response
        size_t parse_len = size > 4096 ? 4096 : size;
        ares_parse_txt_reply(data, parse_len, &txt_reply);
        
        // Clean up TXT reply if parsed
        if (txt_reply) {
            ares_free_data(txt_reply);
        }
    }

    // Create a DNS record and check query count
    ares_dns_record_t *dnsrec = nullptr;
    ares_dns_record_create(&dnsrec, 0x5678, 
                          ARES_FLAG_QR | ARES_FLAG_RD, 
                          ARES_OPCODE_QUERY, 
                          ARES_RCODE_NOERROR);
    
    if (dnsrec) {
        // Add a query to the DNS record
        ares_dns_record_query_add(dnsrec, "test.com", 
                                 ARES_REC_TYPE_A, 
                                 ARES_CLASS_IN);
        
        // Get query count
        size_t query_count = ares_dns_record_query_cnt(dnsrec);
        
        // Convert opcode to string (unused but called)
        const char *opcode_str = ares_dns_opcode_tostr(ARES_OPCODE_QUERY);
        (void)opcode_str; // Avoid unused variable warning
        
        ares_dns_record_destroy(dnsrec);
    }

    // Create a temporary hosts file and use gethostbyname_file
    FILE *hosts_file = fopen("input_file", "wb");
    if (hosts_file) {
        // Write input data to file (truncate to reasonable size)
        size_t write_size = size > 1024 ? 1024 : size;
        if (write_size > 0) {
            fwrite(data, 1, write_size, hosts_file);
        }
        assert_file_closed(&hosts_file);;
        
        // Try to lookup a hostname from the file
        struct hostent *host = nullptr;
        ares_gethostbyname_file(channel, "localhost", AF_INET, &host);
        
        // Clean up hostent if returned
        if (host) {
            ares_free_hostent(host);
        }
        
        // Remove temporary file
        remove("input_file");
    }

    // Create an output file (if needed for other operations)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        assert_file_closed(&out_file);;
        remove("output_file");
    }

    // Clean up channel
    ares_destroy(channel);
    
    // Cleanup library
    ares_library_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&hosts_file);
	return 0;
}
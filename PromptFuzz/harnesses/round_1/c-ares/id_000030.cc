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
//<ID> 734
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_socket_callback","ares_destroy","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_parse_aaaa_reply","ares_free_hostent","ares_parse_uri_reply","ares_dns_record_create","ares_dns_record_query_set_name","ares_dns_record_rr_add","ares_dns_rr_get_type","ares_dns_rr_set_bin","ares_dns_record_rr_add","ares_dns_rr_set_opt","ares_dns_record_get_flags","ares_queue_wait_empty","ares_dns_record_destroy","ares_getaddrinfo","ares_queue_wait_empty","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_socket_callback","ares_parse_aaaa_reply","ares_free_hostent","ares_parse_uri_reply","ares_dns_record_create","ares_dns_record_query_set_name","ares_dns_record_rr_add","ares_dns_rr_get_type","ares_dns_rr_set_bin","ares_dns_record_rr_add","ares_dns_rr_set_opt","ares_dns_record_get_flags","ares_queue_wait_empty","ares_dns_record_destroy","ares_getaddrinfo","ares_queue_wait_empty","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

// Socket callback function
static ares_socket_t  id_000030_socket_callback(ares_socket_t socket_fd, int type, void *data) {
    // Simple socket creation callback
    (void)type;
    (void)data;
    return socket_fd;
}

// Getaddrinfo callback function
static void  id_000030_getaddrinfo_callback(void *arg, int status, int timeouts, struct ares_addrinfo *result) {
    (void)arg;
    (void)status;
    (void)timeouts;
    if (result) {
        ares_freeaddrinfo(result);
    }
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<96) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	

    // 1. Initialize c-ares library
    if (ares_library_init(fuzz_int32_t_1) != ARES_SUCCESS) {
        return 0;
    }

    // 2. Initialize channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 3. Set socket callback
    ares_set_socket_callback(channel,  id_000030_socket_callback, nullptr);

    // Create a FILE* from input data for parsing functions
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Read some data for parsing
    unsigned char *parse_buf = (unsigned char *)malloc(size);
    if (!parse_buf) {
        assert_file_closed(&in_file);;
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    size_t bytes_read = fread(parse_buf, 1, size, in_file);
    assert_file_closed(&in_file);;

    if (bytes_read > 0) {
        // 4. Parse AAAA reply
        struct hostent *host = nullptr;
        struct ares_addr6ttl addrttls[1];
        int naddrttls = 0;
        ares_parse_aaaa_reply(parse_buf, bytes_read, &host, addrttls, &naddrttls);
        
        if (host) {
            // 15. Free hostent
            ares_free_hostent(host);
        }

        // 5. Parse URI reply
        struct ares_uri_reply *uri_out = nullptr;
        ares_parse_uri_reply(parse_buf, bytes_read, &uri_out);
        
        if (uri_out) {
            // Free URI reply chain
            struct ares_uri_reply *curr = uri_out;
            while (curr) {
                struct ares_uri_reply *next = curr->next;
                free(curr->uri);
                free(curr);
                curr = next;
            }
        }
    }

    free(parse_buf);

    // 6. Create DNS record
    ares_dns_record_t *dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_2, fuzz_uint16_t_3, 
                               ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        
        // 9. Set query name
        ares_dns_record_query_set_name(dnsrec, fuzz_uint64_t_5, fuzz_str_4);
        
        // 10. Add resource record (answer section)
        ares_dns_rr_t *rr = nullptr;
        if (ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                                   fuzz_str_6, ARES_REC_TYPE_A, 
                                   ARES_CLASS_IN, fuzz_uint32_t_7) == ARES_SUCCESS) {
            
            // 11. Get RR type
            ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
            (void)rr_type;
            
            // 12. Set binary data in RR
            const char *txt = "fuzz test";
            ares_dns_rr_set_bin(rr, ARES_RR_TXT_DATA, 
                               fuzz_str_8, static_cast<size_t>(fuzz_str_sz_8));
            
            // 2. Set OPT (using separate OPT RR)
            ares_dns_rr_t *opt_rr = nullptr;
            if (ares_dns_record_rr_add(&opt_rr, dnsrec, ARES_SECTION_ADDITIONAL,
                                       fuzz_str_9, ARES_REC_TYPE_OPT, 
                                       ARES_CLASS_IN, fuzz_uint32_t_10) == ARES_SUCCESS) {
                // Set OPT option using input data (limited to 100 bytes)
                size_t opt_len = size > 100 ? 100 : size;
                ares_dns_rr_set_opt(opt_rr, ARES_RR_OPT_OPTIONS, 
                                   fuzz_uint16_t_11, data, opt_len);
            }
        }
        
        // 8. Get DNS record flags
        unsigned short flags = ares_dns_record_get_flags(dnsrec);
        (void)flags;
        
        // 13. Wait for queue to be empty
        ares_queue_wait_empty(channel, fuzz_int32_t_12);
        
        // 14. Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // 17. Perform getaddrinfo lookup
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    ares_getaddrinfo(channel, fuzz_str_13, fuzz_str_14, &hints, 
                     id_000030_getaddrinfo_callback, nullptr);
    
    // Wait for pending operations
    ares_queue_wait_empty(channel, fuzz_int32_t_15);

    // 16. Destroy channel
    ares_destroy(channel);
    
    // 18. Cleanup library
    ares_library_cleanup();

    assert_file_closed(&in_file);
	return 0;
}
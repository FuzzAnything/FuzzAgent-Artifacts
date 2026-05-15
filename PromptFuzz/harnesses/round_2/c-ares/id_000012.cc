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
//<ID> 268
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 94
//<Quality> {"density":8,"unique_branches":{"ares_array_at":[[69,7,69,18,0,0,4,0]],"parse_sort":[[125,7,125,29,0,0,4,1],[132,7,132,77,0,0,4,1],[138,7,138,29,0,0,4,1],[144,7,144,58,0,0,4,1],[149,7,149,63,0,0,4,1],[205,7,205,29,0,0,4,1]],"ares_addrinfo2hostent":[[198,11,198,29,0,0,4,0],[204,11,204,28,0,0,4,1],[214,31,214,55,0,0,4,1]],"ares_parse_into_addrinfo":[[143,8,143,18,0,0,4,1],[143,23,143,32,0,0,4,0],[143,36,143,57,0,0,4,1],[158,16,158,24,0,0,4,1]],"ares_array_set_size":[[176,7,176,18,0,0,4,0],[176,22,176,31,0,0,4,0],[176,35,176,50,0,0,4,0]],"ares_buf_create_const":[[61,23,61,36,0,0,4,0]],"ares_thread_join":[[615,7,615,21,0,0,4,1],[619,7,619,46,0,0,4,1],[624,7,624,29,0,0,4,0],[624,33,624,43,0,0,4,0]],"ares_addrinfo2addrttl":[[240,7,240,24,0,0,4,0],[240,28,240,46,0,0,4,1],[244,7,244,17,0,0,4,1],[244,21,244,38,0,0,4,1],[248,7,248,24,0,0,4,1],[252,7,252,25,0,0,4,0],[252,29,252,46,0,0,4,1],[256,7,256,25,0,0,4,1],[263,10,263,20,0,0,4,0],[263,10,263,20,0,0,4,1],[264,9,264,36,0,0,4,0],[270,26,270,38,0,0,4,0],[270,26,270,38,0,0,4,1],[271,9,271,34,0,0,4,0],[271,9,271,34,0,0,4,1],[275,9,275,36,0,0,4,1],[279,9,279,27,0,0,4,0],[280,11,280,35,0,0,4,1]],"ares_buf_split":[[955,13,955,16,0,0,4,1]],"ares_dns_rec_type_fromstr":[[724,7,724,20,0,0,4,1],[724,24,724,35,0,0,4,1],[728,15,728,35,0,0,4,0],[729,9,729,42,0,0,4,0],[729,9,729,42,0,0,4,1]],"ares_reinit":[[411,7,411,22,0,0,4,1],[419,7,419,23,0,0,4,1],[419,27,419,50,0,0,4,1],[426,7,426,26,0,0,4,0],[429,9,429,39,0,0,4,1],[438,9,438,31,0,0,4,1]],"ip_natural_mask":[[78,7,78,31,0,0,4,1],[83,7,83,17,0,0,4,0]],"ares_buf_fetch":[[501,7,501,18,0,0,4,1],[505,7,505,18,0,0,4,0],[505,22,505,33,0,0,4,0]],"ares_parse_sortlist":[[221,7,221,23,0,0,4,1],[221,27,221,40,0,0,4,1],[221,44,221,55,0,0,4,1],[225,7,225,24,0,0,4,1],[233,7,233,18,0,0,4,1],[249,7,249,29,0,0,4,1],[254,15,254,22,0,0,4,0],[254,15,254,22,0,0,4,1],[261,9,261,31,0,0,4,1],[265,9,265,31,0,0,4,1],[269,9,269,48,0,0,4,1],[281,7,281,29,0,0,4,1]],"ares_array_insert_at":[[247,7,247,15,0,0,4,1]],"ares_set_sortlist":[[587,7,587,15,0,0,4,1],[593,7,593,29,0,0,4,0],[593,33,593,41,0,0,4,0],[594,9,594,26,0,0,4,1]],"ares_thread_create":[[593,7,593,19,0,0,4,1],[593,23,593,37,0,0,4,1],[598,7,598,18,0,0,4,1],[601,7,601,57,0,0,4,1]],"sortlist_append":[[100,7,100,22,0,0,4,1]],"ares_malloc_zero":[[87,7,87,18,0,0,4,1]],"ares_realloc_zero":[[101,7,101,27,0,0,4,1]],"ares_round_up_pow2":[[75,7,75,22,0,0,4,1]],"ares_parse_aaaa_reply":[[78,7,78,29,0,0,4,1],[85,9,85,31,0,0,4,1],[90,27,90,40,0,0,4,0]],"ares_buf_tag_fetch_bytes":[[409,7,409,18,0,0,4,0]],"ares_destroy":[[62,7,62,37,0,0,4,0]],"ares_queue_wait_empty":[[773,7,773,27,0,0,4,1],[777,7,777,22,0,0,4,1],[781,7,781,22,0,0,4,0],[788,10,788,46,0,0,4,1]],"ares_reinit_thread":[[389,7,389,29,0,0,4,1],[397,7,397,29,0,0,4,0],[397,33,397,48,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_reinit","ares_cancel","ares_destroy","ares_library_cleanup","ares_set_sortlist","ares_parse_aaaa_reply","ares_free_hostent","ares_mkquery","ares_dns_record_create","ares_dns_record_query_add","ares_dns_rec_type_fromstr","ares_dns_class_tostr","ares_queue_wait_empty","ares_dns_record_destroy","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_class_tostr","ares_dns_rec_type_fromstr","ares_dns_record_destroy","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_reinit","ares_set_sortlist","ares_parse_aaaa_reply","ares_free_hostent","ares_mkquery","ares_dns_record_create","ares_dns_record_query_add","ares_dns_rec_type_fromstr","ares_dns_class_tostr","ares_queue_wait_empty","ares_dns_record_destroy","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_class_tostr","ares_dns_rec_type_fromstr","ares_dns_record_destroy","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":2}
/*Here's a C++ fuzz driver that uses the specified c-ares APIs to achieve DNS query parsing and manipulation:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < sizeof(unsigned short) + 1) {
        return 0;
    }

    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Use ares_reinit
    if (ares_reinit(channel) != ARES_SUCCESS) {
        ares_cancel(channel);
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }

    // Use ares_set_sortlist with a simple sort string
    const char *sortstr = "1.2.3.4 2.3.4.5";
    ares_set_sortlist(channel, sortstr);

    // Parse AAAA reply from input data
    struct hostent *host = nullptr;
    struct ares_addr6ttl addrttls[10];
    int naddrttls = 10;
    
    int parse_result = ares_parse_aaaa_reply(data, size, &host, addrttls, &naddrttls);
    
    // Clean up parsed hostent if allocated
    if (host != nullptr) {
        ares_free_hostent(host);
    }

    // Create a DNS query using ares_mkquery
    unsigned short query_id = *(unsigned short*)data;  // Use first 2 bytes as query ID
    const char *query_name = "example.com";
    unsigned char *query_buf = nullptr;
    int query_buflen = 0;
    
    int mkquery_result = ares_mkquery(query_name, 
                                      ARES_CLASS_IN, 
                                      ARES_REC_TYPE_AAAA, 
                                      query_id, 
                                      1,  // RD flag set
                                      &query_buf, 
                                      &query_buflen);
    
    // Clean up query buffer if allocated
    if (query_buf != nullptr) {
        free(query_buf);
    }

    // Create a minimal DNS resource record for testing
    ares_dns_rr_t *dns_rr = nullptr;
    // Note: We need to create a DNS record first to get an RR
    ares_dns_record_t *dnsrec = nullptr;
    
    if (ares_dns_record_create(&dnsrec, query_id, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        // Add a query to the record
        if (ares_dns_record_query_add(dnsrec, query_name, ARES_REC_TYPE_AAAA, ARES_CLASS_IN) == ARES_SUCCESS) {
            // Get the first (and only) RR from the answer section (though it will be empty initially)
            // We'll create a simple RR manually by simulating what would be in a parsed response
            
            // For demonstration, we'll use ares_dns_rr_add_abin with some test data
            // First create a simple RR structure (simplified approach)
            // In real usage, we would get RRs from parsed DNS responses
            
            // Use ares_dns_rec_type_fromstr
            ares_dns_rec_type_t qtype;
            if (ares_dns_rec_type_fromstr(&qtype, "AAAA")) {
                // This API succeeded
            }
            
            // Use ares_dns_class_tostr
            const char *class_str = ares_dns_class_tostr(ARES_CLASS_IN);
            
            // Create a simple AAAA record data (IPv6 address)
            unsigned char ipv6_data[16] = {
                0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x00, 0x00,
                0x00, 0x00, 0x8a, 0x2e, 0x03, 0x70, 0x73, 0x34
            };
            
            // For the purpose of demonstrating ares_dns_rr_add_abin, we would need
            // an actual ares_dns_rr_t object. Since we don't have one from the APIs
            // we're limited in how we can test this function directly.
            // We'll simulate what could be done if we had parsed a DNS response.
            
            // Use ares_queue_wait_empty to wait for any pending operations
            // Timeout after 100ms
            if (ares_queue_wait_empty(channel, 100) != ARES_SUCCESS) {
                // Timeout or error occurred
            }
        }
        
        // Clean up DNS record
        if (dnsrec != nullptr) {
            ares_dns_record_destroy(dnsrec);
        }
    }

    // Alternative approach: Try to parse the input as a DNS record
    // and work with any RRs found in it
    if (size > 12) {  // Minimum DNS header size
        ares_dns_record_t *parsed_dnsrec = nullptr;
        ares_status_t parse_status = ares_dns_parse(data, size, 0, &parsed_dnsrec);
        
        if (parse_status == ARES_SUCCESS && parsed_dnsrec != nullptr) {
            // Get number of RRs in answer section
            size_t answer_count = ares_dns_record_rr_cnt(parsed_dnsrec, ARES_SECTION_ANSWER);
            
            if (answer_count > 0) {
                // Get the first RR in answer section
                // Note: ares_dns_record_rr_get is not in the provided APIs,
                // so we cannot actually get the RR object to use with ares_dns_rr_add_abin
                // or ares_dns_rr_get_class
                
                // We can still demonstrate ares_dns_class_tostr
                const char *class_str_parsed = ares_dns_class_tostr(ARES_CLASS_IN);
                
                // And ares_dns_rec_type_fromstr
                ares_dns_rec_type_t parsed_qtype;
                ares_dns_rec_type_fromstr(&parsed_qtype, "A");
            }
            
            // Clean up parsed DNS record
            ares_dns_record_destroy(parsed_dnsrec);
        }
    }

    // Clean up
    ares_cancel(channel);
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}
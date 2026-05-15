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
//<ID> 669
//<Prompt> []
/*<Combination>: [
*/
//<score> 138, nr_unique_branch: 6
//<Quality> {"density":23,"unique_branches":{"ares_create_query_int":[[55,7,55,29,0,0,4,0]],"ares_query_nolock":[[72,42,72,58,0,0,4,0],[75,9,75,25,0,0,4,1]],"ares_dns_rr_set_u16":[[1163,7,1163,57,0,0,4,0]],"ares_gethostbyname":[[109,7,109,16,0,0,4,0]],"ares_dns_rr_set_str_own":[[1282,7,1282,18,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_socket_callback","ares_dns_record_create","ares_destroy","ares_library_cleanup","ares_dns_record_destroy","ares_destroy","ares_library_cleanup","ares_dns_class_fromstr","ares_dns_record_query_add","ares_dns_record_destroy","ares_destroy","ares_library_cleanup","ares_dns_record_query_cnt","ares_dns_record_get_flags","ares_dns_record_rr_add","ares_dns_rr_set_ttl","ares_dns_rr_set_u16","ares_dns_rr_set_str","ares_dns_record_rr_del","ares_mkquery","ares_query_dnsrec","ares_process_pending_write","ares_gethostbyname","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_socket_callback","ares_dns_record_create","ares_dns_class_fromstr","ares_dns_record_query_add","ares_dns_record_query_cnt","ares_dns_record_get_flags","ares_dns_record_rr_add","ares_dns_rr_set_ttl","ares_dns_rr_set_u16","ares_dns_rr_set_str","ares_dns_record_rr_del","ares_mkquery","ares_query_dnsrec","ares_process_pending_write","ares_gethostbyname","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Include necessary c-ares headers

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




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

    // Set socket callback (though we won't actually use sockets in fuzzing)
    ares_set_socket_callback(channel, nullptr, nullptr);

    // Create a DNS record
    ares_dns_record_t *dnsrec = nullptr;
    unsigned short id = 12345;
    unsigned short flags = 0;
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    if (ares_dns_record_create(&dnsrec, id, flags, opcode, rcode) != ARES_SUCCESS) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }

    // Use input data for DNS query name (ensure null termination)
    size_t name_len = size > 255 ? 255 : size;
    char *name = (char*)malloc(name_len + 1);
    if (!name) {
        ares_dns_record_destroy(dnsrec);
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }
    memcpy(name, data, name_len);
    name[name_len] = '\0';

    // Convert DNS class from string
    ares_dns_class_t dnsclass;
    if (!ares_dns_class_fromstr(&dnsclass, "IN")) {
        dnsclass = ARES_CLASS_IN;
    }

    // Add query to DNS record
    if (ares_dns_record_query_add(dnsrec, name, ARES_REC_TYPE_A, dnsclass) != ARES_SUCCESS) {
        free(name);
        ares_dns_record_destroy(dnsrec);
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }

    // Get query count
    size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
    
    // Get flags
    unsigned short record_flags = ares_dns_record_get_flags(dnsrec);

    // Add a resource record (answer)
    ares_dns_rr_t *rr = nullptr;
    if (ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                               "example.com", ARES_REC_TYPE_A, 
                               ARES_CLASS_IN, 300) == ARES_SUCCESS) {
        // Set TTL for the resource record
        ares_dns_rr_set_ttl(rr, 600);
        
        // Set a 16-bit value (like port for SRV, but we'll use it differently)
        ares_dns_rr_set_u16(rr, ARES_RR_A_ADDR, htons(8080));
        
        // Set string value (for demonstration, using part of input data)
        size_t str_len = size > 100 ? 100 : size;
        char *str_val = (char*)malloc(str_len + 1);
        if (str_val) {
            memcpy(str_val, data, str_len);
            str_val[str_len] = '\0';
            ares_dns_rr_set_str(rr, ARES_RR_CNAME_CNAME, str_val);
            free(str_val);
        }
        
        // Delete the resource record we just added
        ares_dns_record_rr_del(dnsrec, ARES_SECTION_ANSWER, 0);
    }

    // Create a DNS query using mkquery (traditional method)
    unsigned char *query_buf = nullptr;
    int query_len = 0;
    ares_mkquery(name, dnsclass, T_A, id, 1, &query_buf, &query_len);
    
    // Clean up query buffer if allocated
    if (query_buf) {
        free(query_buf);
    }

    // Use query_dnsrec API
    unsigned short qid = 0;
    ares_query_dnsrec(channel, name, dnsclass, ARES_REC_TYPE_A, 
                      nullptr, nullptr, &qid);

    // Process pending writes
    ares_process_pending_write(channel);

    // Use gethostbyname API
    ares_gethostbyname(channel, name, AF_INET, nullptr, nullptr);

    // Clean up
    free(name);
    ares_dns_record_destroy(dnsrec);
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}
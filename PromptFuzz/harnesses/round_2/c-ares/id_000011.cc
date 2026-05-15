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
//<ID> 239
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 86
//<Quality> {"density":12,"unique_branches":{"ares_parse_ns_reply":[[53,7,53,19,0,0,4,1],[60,7,60,29,0,0,4,0],[60,7,60,29,0,0,4,1],[65,7,65,19,0,0,4,0],[65,7,65,19,0,0,4,1],[72,7,72,22,0,0,4,1],[80,7,80,35,0,0,4,1],[90,7,90,29,0,0,4,1],[94,7,94,30,0,0,4,1],[101,7,101,33,0,0,4,1],[107,15,107,26,0,0,4,0],[107,15,107,26,0,0,4,1],[111,9,111,19,0,0,4,1],[117,9,117,51,0,0,4,0],[117,9,117,51,0,0,4,1],[118,9,118,53,0,0,4,0],[118,9,118,53,0,0,4,1],[123,9,123,25,0,0,4,1],[129,9,129,44,0,0,4,1],[136,7,136,19,0,0,4,0],[136,7,136,19,0,0,4,1],[143,7,143,29,0,0,4,0],[143,7,143,29,0,0,4,1],[146,9,146,32,0,0,4,0],[146,9,146,32,0,0,4,1]],"ares_malloc_data":[[152,5,152,33,0,0,4,0]],"ares_free_data":[[111,7,111,35,0,0,4,0]],"ares_dns_record_create":[[64,7,64,38,0,0,4,0]],"ares_strerror":[[33,11,33,17,0,0,4,1],[34,5,34,22,0,0,4,0],[34,5,34,22,0,0,4,1],[36,5,36,22,0,0,4,0],[36,5,36,22,0,0,4,1],[38,5,38,23,0,0,4,0],[38,5,38,23,0,0,4,1],[40,5,40,24,0,0,4,1],[42,5,42,24,0,0,4,1],[44,5,44,22,0,0,4,1],[46,5,46,23,0,0,4,1],[48,5,48,24,0,0,4,1],[50,5,50,23,0,0,4,0],[50,5,50,23,0,0,4,1],[52,5,52,25,0,0,4,1],[54,5,54,23,0,0,4,0],[54,5,54,23,0,0,4,1],[56,5,56,27,0,0,4,1],[58,5,58,23,0,0,4,1],[60,5,60,18,0,0,4,1],[62,5,62,20,0,0,4,1],[64,5,64,21,0,0,4,1],[66,5,66,27,0,0,4,1],[68,5,68,22,0,0,4,0],[68,5,68,22,0,0,4,1],[70,5,70,24,0,0,4,1],[72,5,72,22,0,0,4,1],[74,5,74,24,0,0,4,1],[76,5,76,30,0,0,4,1],[78,5,78,28,0,0,4,1],[80,5,80,36,0,0,4,1],[82,5,82,25,0,0,4,1],[84,5,84,23,0,0,4,1],[86,5,86,24,0,0,4,1]],"ares_parse_caa_reply":[[43,7,43,19,0,0,4,1],[50,7,50,29,0,0,4,0],[50,7,50,29,0,0,4,1],[54,7,54,63,0,0,4,0],[54,7,54,63,0,0,4,1],[59,15,59,70,0,0,4,0],[59,15,59,70,0,0,4,1],[65,9,65,19,0,0,4,1],[72,9,72,51,0,0,4,0],[72,9,72,51,0,0,4,1],[73,9,73,54,0,0,4,0],[73,9,73,54,0,0,4,1],[78,9,78,54,0,0,4,0],[78,9,78,54,0,0,4,1],[84,9,84,25,0,0,4,1],[90,9,90,17,0,0,4,0],[90,9,90,17,0,0,4,1],[100,9,100,35,0,0,4,1],[109,9,109,20,0,0,4,1],[116,9,116,32,0,0,4,1],[127,7,127,29,0,0,4,0],[127,7,127,29,0,0,4,1],[128,9,128,17,0,0,4,1]],"ares_dns_flags_arevalid":[[75,7,75,24,0,0,4,0]]},"library_calls":["ares_library_initialized","ares_library_init","ares_init","ares_parse_caa_reply","ares_strerror","ares_parse_ns_reply","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_set_bin","ares_dns_rr_add_abin","ares_dns_rr_del_abin","ares_dns_section_tostr","ares_dns_rr_key_tostr","ares_expand_string","ares_free_data","ares_dns_record_destroy","ares_free_data","ares_free_hostent","ares_destroy"],"critical_calls":["ares_library_initialized","ares_library_init","ares_init","ares_parse_caa_reply","ares_strerror","ares_parse_ns_reply","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_set_bin","ares_dns_rr_add_abin","ares_dns_rr_del_abin","ares_dns_section_tostr","ares_dns_rr_key_tostr","ares_expand_string","ares_free_data","ares_dns_record_destroy","ares_free_data","ares_free_hostent","ares_destroy"],"visited":1}
/**/


extern "C" {
}

// Custom callback functions to handle completion
static void  id_000011_parse_caa_callback(void *arg, int status, int timeouts, unsigned char *abuf, int alen) {
    (void)arg; (void)status; (void)timeouts; (void)abuf; (void)alen;
    // Callback for demonstration
}

static void  id_000011_parse_ns_callback(void *arg, int status, int timeouts, struct hostent *hostent) {
    (void)arg; (void)status; (void)timeouts; (void)hostent;
    // Callback for demonstration
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // Initialize c-ares library if not already initialized
    if (!ares_library_initialized()) {
        ares_library_init(ARES_LIB_INIT_ALL);
    }
    
    // Create a FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create a FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    ares_channel_t *channel = nullptr;
    ares_status_t status;
    
    // 1. Initialize channel
    if (ares_init(&channel) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Parse input as DNS response for CAA records
    struct ares_caa_reply *caa_reply = nullptr;
    int parse_result = ares_parse_caa_reply(data, (int)(size > INT_MAX ? INT_MAX : size), &caa_reply);
    
    // Use ares_strerror to get error string
    const char *error_str = ares_strerror(parse_result);
    if (error_str) {
        fwrite(error_str, 1, strlen(error_str), out_file);
    }
    
    // 3. Parse input as DNS response for NS records
    struct hostent *ns_hostent = nullptr;
    int ns_result = ares_parse_ns_reply(data, (int)(size > INT_MAX ? INT_MAX : size), &ns_hostent);
    
    // 4. Create a DNS record and manipulate it
    ares_dns_record_t *dnsrec = nullptr;
    status = ares_dns_record_create(&dnsrec, 12345, 0x0100, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Add a query section
        status = ares_dns_record_query_add(dnsrec, "example.com", ARES_REC_TYPE_A, ARES_CLASS_IN);
        
        // Create and add an answer RR
        ares_dns_rr_t *rr = nullptr;
        status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                       "example.com", ARES_REC_TYPE_A, 
                                       ARES_CLASS_IN, 300);
        
        if (status == ARES_SUCCESS && rr) {
            // 5. Use ares_dns_rr_set_bin to set binary data (using part of input)
            size_t bin_data_len = size > 128 ? 128 : size;
            status = ares_dns_rr_set_bin(rr, ARES_RR_A_ADDR, data, bin_data_len);
            
            // 6. Use ares_dns_rr_add_abin to add additional binary data
            size_t add_len = (size > 64 && size >= 128) ? 64 : (size > 64 ? size - 64 : 0);
            if (add_len > 0) {
                status = ares_dns_rr_add_abin(rr, ARES_RR_A_ADDR, data + 64, add_len);
            }
            
            // 7. Use ares_dns_rr_del_abin to delete binary data (delete first entry if exists)
            ares_dns_rr_del_abin(rr, ARES_RR_A_ADDR, 0);
            
            // 8. Use string conversion functions
            const char *section_str = ares_dns_section_tostr(ARES_SECTION_ANSWER);
            const char *key_str = ares_dns_rr_key_tostr(ARES_RR_A_ADDR);
            
            if (section_str) fwrite(section_str, 1, strlen(section_str), out_file);
            if (key_str) fwrite(key_str, 1, strlen(key_str), out_file);
        }
        
        // 9. Use ares_expand_string on part of the input
        if (size > 20) {
            unsigned char *expanded_str = nullptr;
            long enclen = 0;
            
            // Create a simple DNS compressed string for testing
            unsigned char encoded_str[20];
            memcpy(encoded_str, data, size > 20 ? 20 : size);
            encoded_str[0] = 3; // Length byte for "www"
            encoded_str[1] = 'w'; encoded_str[2] = 'w'; encoded_str[3] = 'w';
            encoded_str[4] = 0; // Null terminator
            
            int expand_result = ares_expand_string(encoded_str, encoded_str, 20, 
                                                  &expanded_str, &enclen);
            
            if (expand_result == ARES_SUCCESS && expanded_str) {
                ares_free_data(expanded_str);
            }
        }
        
        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Clean up parsed replies
    if (caa_reply) {
        ares_free_data(caa_reply);
    }
    if (ns_hostent) {
        ares_free_hostent(ns_hostent);
    }
    
    // Clean up channel
    ares_destroy(channel);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}
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
//<ID> 923
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 6
//<Quality> {"density":7,"unique_branches":{"ares_save_options":[[176,7,176,43,0,0,4,0],[178,9,178,23,0,0,4,0],[180,11,180,29,0,0,4,1],[183,19,183,37,0,0,4,0],[183,19,183,37,0,0,4,1]],"parse_sort":[[174,11,174,19,0,0,4,0]]},"library_calls":["ares_library_initialized","ares_library_init","ares_library_cleanup","ares_init_options","ares_library_cleanup","ares_set_sortlist","ares_parse_txt_reply","ares_free_data","ares_create_query","ares_mkquery","ares_dns_rec_type_fromstr","ares_dns_pton","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_name","ares_dns_rr_get_opt_byid","ares_dns_opt_get_name","ares_dns_record_destroy","ares_free_string","ares_free_string","ares_save_options","ares_destroy_options","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_initialized","ares_library_init","ares_init_options","ares_set_sortlist","ares_parse_txt_reply","ares_free_data","ares_create_query","ares_mkquery","ares_dns_rec_type_fromstr","ares_dns_pton","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_name","ares_dns_rr_get_opt_byid","ares_dns_opt_get_name","ares_dns_record_destroy","ares_free_string","ares_free_string","ares_save_options","ares_destroy_options","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<58) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_8, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_9, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_13, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_14, fdp);
	//fuzzer shim end}




    // 1. Initialize c-ares library
    if (ares_library_initialized() != 1) {
        if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
            return 0;
        }
    }
    
    // 2. Create a FILE* from input data for reading binary data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Initialize ares channel
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Set some default options
    memset(&options, 0, sizeof(options));
    optmask = ARES_OPT_FLAGS;
    options.flags = ARES_FLAG_STAYOPEN;
    
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Use ares_set_sortlist (first API from list)
    // Extract sortlist string from input data
    size_t sortlist_len = size > 100 ? 100 : size;
    char *sortlist_str = (char *)malloc(sortlist_len + 1);
    if (sortlist_str) {
        memcpy(sortlist_str, data, sortlist_len);
        sortlist_str[sortlist_len] = '\0';
        ares_set_sortlist(channel, sortlist_str);
        free(sortlist_str);
    }
    
    // 5. Parse TXT reply (using ares_parse_txt_reply)
    struct ares_txt_reply *txt_out = nullptr;
    // Use part of input data as DNS reply buffer
    if (size >= 12) {
        const unsigned char *abuf = data;
        int alen = size > 1024 ? 1024 : size;
        ares_parse_txt_reply(abuf, alen, &txt_out);
        
        // Free TXT reply if parsed successfully
        if (txt_out) {
            ares_free_data(txt_out);
        }
    }
    
    // 6. Create DNS query using ares_create_query and ares_mkquery
    unsigned char *query_buf1 = nullptr;
    unsigned char *query_buf2 = nullptr;
    int buflen1 = 0, buflen2 = 0;
    
    // Extract query name from input
    size_t name_len = size > 50 ? 50 : size;
    char *query_name = (char *)malloc(name_len + 1);
    if (query_name && name_len > 0) {
        memcpy(query_name, data, name_len);
        query_name[name_len] = '\0';
        
        // Use ares_create_query (second API from list)
        ares_create_query(query_name, fuzz_int32_t_2, fuzz_int32_t_3, fuzz_uint16_t_4, 1, &query_buf1, &buflen1, fuzz_int32_t_5);
        
        // Use ares_mkquery (fifth API from list)
        ares_mkquery(query_name, fuzz_int32_t_6, fuzz_int32_t_7, fuzz_uint16_t_8, 1, &query_buf2, &buflen2);
        
        free(query_name);
    }
    
    // 7. Convert string to DNS record type using ares_dns_rec_type_fromstr
    ares_dns_rec_type_t qtype;
    if (size > 10) {
        char type_str[11];
        memcpy(type_str, data, 10);
        type_str[10] = '\0';
        ares_dns_rec_type_fromstr(&qtype, type_str);
    }
    
    // 8. Convert IP address string using ares_dns_pton
    struct ares_addr addr;
    size_t out_len;
    if (size > 15) {
        char ip_str[16];
        memcpy(ip_str, data, 15);
        ip_str[15] = '\0';
        ares_dns_pton(ip_str, &addr, &out_len);
    }
    
    // 9. Create a dummy DNS resource record and test APIs that require it
    // Create a simple DNS record first
    ares_dns_record_t *dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_9, fuzz_uint16_t_10, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        // Add a query section
        ares_dns_record_query_add(dnsrec, fuzz_str_11, ARES_REC_TYPE_A, ARES_CLASS_IN);
        
        // Get RR from the record
        if (ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER) > 0) {
            ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_12);
            if (rr) {
                // Use ares_dns_rr_get_name (seventh API from list)
                const char *rr_name = ares_dns_rr_get_name(rr);
                
                // Use ares_dns_rr_get_opt_byid (eighth API from list)
                const unsigned char *opt_val;
                size_t val_len;
                ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS, fuzz_uint16_t_13, &opt_val, &val_len);
                
                // Use ares_dns_opt_get_name (first API from list)
                const char *opt_name = ares_dns_opt_get_name(ARES_RR_OPT_OPTIONS, fuzz_uint16_t_14);
            }
        }
        
        ares_dns_record_destroy(dnsrec);
    }
    
    // 10. Clean up
    if (query_buf1) {
        ares_free_string(query_buf1);
    }
    if (query_buf2) {
        ares_free_string(query_buf2);
    }
    
    // Save and destroy channel options
    struct ares_options saved_options;
    int saved_optmask;
    ares_save_options(channel, &saved_options, &saved_optmask);
    ares_destroy_options(&saved_options);
    
    ares_destroy(channel);
    assert_file_closed(&in_file);;
    
    // Cleanup library
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
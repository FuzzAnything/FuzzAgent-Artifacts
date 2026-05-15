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
//<ID> 920
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 19
//<Quality> {"density":6,"unique_branches":{"ares_dns_opt_get_name":[[899,5,899,29,0,0,4,0],[904,5,904,12,0,0,4,1]],"ares_dns_opt_get_name_opt":[[837,11,837,16,0,0,4,1],[838,5,838,28,0,0,4,1],[840,5,840,27,0,0,4,1],[842,5,842,29,0,0,4,0],[844,5,844,28,0,0,4,1],[846,5,846,28,0,0,4,1],[848,5,848,28,0,0,4,1],[850,5,850,43,0,0,4,1],[852,5,852,36,0,0,4,1],[854,5,854,31,0,0,4,1],[856,5,856,43,0,0,4,1],[858,5,858,32,0,0,4,1],[860,5,860,30,0,0,4,1],[862,5,862,37,0,0,4,1],[864,5,864,43,0,0,4,1]],"ares_dns_rr_get_opt_byid":[[1075,7,1075,19,0,0,4,1],[1075,23,1075,36,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_parse_txt_reply","ares_set_sortlist","ares_create_query","ares_mkquery","ares_dns_pton","ares_dns_rec_type_fromstr","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_get_name","ares_dns_rr_get_opt_byid","ares_dns_opt_get_name","ares_dns_record_destroy","ares_free_data","ares_free_string","ares_free_string","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_parse_txt_reply","ares_set_sortlist","ares_create_query","ares_mkquery","ares_dns_pton","ares_dns_rec_type_fromstr","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_get_name","ares_dns_rr_get_opt_byid","ares_dns_opt_get_name","ares_dns_record_destroy","ares_free_data","ares_free_string","ares_free_string","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

// Forward declaration for missing function from the list
extern "C" {
ares_bool_t ares_dns_rec_type_fromstr(ares_dns_rec_type_t *qtype, const char *str);
const char *ares_dns_rr_get_name(const ares_dns_rr_t *rr);
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<27) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_9, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_10, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    if (ares_init_options(&channel, nullptr, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Use input data for various operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Try to parse input as TXT reply
    struct ares_txt_reply *txt_out = nullptr;
    if (size > 0) {
        // We'll use a portion of the input as DNS reply data
        size_t parse_size = size > 1024 ? 1024 : size;
        unsigned char *abuf = (unsigned char *)malloc(parse_size);
        if (abuf) {
            fread(abuf, 1, parse_size, in_file);
            rewind(in_file);
            
            ares_parse_txt_reply(abuf, parse_size, &txt_out);
            
            free(abuf);
        }
    }

    // 2. Use input for sortlist (first 256 bytes as string)
    char sortstr[257];
    if (size > 0) {
        size_t str_len = size > 256 ? 256 : size;
        fread(sortstr, 1, str_len, in_file);
        sortstr[str_len] = '\0';
        rewind(in_file);
        
        ares_set_sortlist(channel, sortstr);
    }

    // 3. Create DNS queries using input data
    unsigned char *buf1 = nullptr, *buf2 = nullptr;
    int buflen1 = 0, buflen2 = 0;
    
    // Try to use input as domain name (first 63 bytes max for domain)
    char domain[64];
    if (size > 0) {
        size_t domain_len = size > 63 ? 63 : size;
        fread(domain, 1, domain_len, in_file);
        domain[domain_len] = '\0';
        rewind(in_file);
        
        // Create query using ares_create_query
        ares_create_query(domain, C_IN, T_TXT, fuzz_uint16_t_2, 1, &buf1, &buflen1, fuzz_int32_t_3);
        
        // Create query using ares_mkquery (older API)
        ares_mkquery(domain, C_IN, T_TXT, fuzz_uint16_t_4, 1, &buf2, &buflen2);
    }

    // 4. Try to convert IP address from input
    struct ares_addr addr;
    size_t out_len = 0;
    if (size >= 4) {
        char ip_str[16];
        // Format first 4 bytes as IP address
        snprintf(ip_str, sizeof(ip_str), "%u.%u.%u.%u", 
                 data[0], data[1], data[2], data[3]);
        ares_dns_pton(ip_str, &addr, &out_len);
    }

    // 5. Test DNS record type conversion from string
    ares_dns_rec_type_t qtype;
    if (size > 0) {
        char type_str[16];
        size_t type_len = size > 15 ? 15 : size;
        memcpy(type_str, data, type_len);
        type_str[type_len] = '\0';
        ares_dns_rec_type_fromstr(&qtype, type_str);
    }

    // 6. Create a DNS record and test RR functions
    ares_dns_record_t *dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_5, fuzz_uint16_t_6, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        // Add a query
        if (size > 0) {
            char qname[64];
            size_t qname_len = size > 63 ? 63 : size;
            memcpy(qname, data, qname_len);
            qname[qname_len] = '\0';
            ares_dns_record_query_add(dnsrec, qname, ARES_REC_TYPE_TXT, ARES_CLASS_IN);
        }
        
        // Add an OPT RR for testing options
        ares_dns_rr_t *opt_rr = nullptr;
        if (ares_dns_record_rr_add(&opt_rr, dnsrec, ARES_SECTION_ADDITIONAL, 
                                   fuzz_str_7, ARES_REC_TYPE_OPT, ARES_CLASS_IN, fuzz_uint32_t_8) == ARES_SUCCESS) {
            // Get RR name
            const char *rr_name = ares_dns_rr_get_name(opt_rr);
            
            // Test option functions
            const unsigned char *opt_val = nullptr;
            size_t opt_val_len = 0;
            
            // Test ares_dns_rr_get_opt_byid
            ares_dns_rr_get_opt_byid(opt_rr, ARES_RR_OPT_OPTIONS, fuzz_uint16_t_9, &opt_val, &opt_val_len);
            
            // Test ares_dns_opt_get_name
            const char *opt_name = ares_dns_opt_get_name(ARES_RR_OPT_OPTIONS, fuzz_uint16_t_10);
        }
        
        ares_dns_record_destroy(dnsrec);
    }

    // Clean up
    if (txt_out) {
        ares_free_data(txt_out);
    }
    
    if (buf1) {
        ares_free_string(buf1);
    }
    
    if (buf2) {
        ares_free_string(buf2);
    }
    
    assert_file_closed(&in_file);;
    ares_destroy(channel);
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
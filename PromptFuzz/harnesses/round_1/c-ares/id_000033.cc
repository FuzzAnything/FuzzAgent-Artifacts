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
//<ID> 758
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_local_dev","ares_dns_class_fromstr","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_ttl","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_key_datatype","ares_dns_record_rr_get","ares_dns_rr_set_u8","ares_dns_rr_set_ttl","ares_dns_record_rr_del","ares_dns_record_destroy","ares_parse_ns_reply","ares_free_hostent","ares_parse_mx_reply","ares_free_data","ares_parse_txt_reply","ares_free_data","ares_get_servers","ares_free_data","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_local_dev","ares_dns_class_fromstr","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_ttl","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_key_datatype","ares_dns_record_rr_get","ares_dns_rr_set_u8","ares_dns_rr_set_ttl","ares_dns_record_rr_del","ares_dns_record_destroy","ares_parse_ns_reply","ares_free_hostent","ares_parse_mx_reply","ares_free_data","ares_parse_txt_reply","ares_free_data","ares_get_servers","ares_free_data","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create channel with default options
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set local device name using a portion of input data
    if (size > 0) {
        // Take first 32 bytes or less for device name
        size_t dev_name_len = size < 32 ? size : 32;
        char *dev_name = (char*)malloc(dev_name_len + 1);
        if (dev_name) {
            memcpy(dev_name, data, dev_name_len);
            dev_name[dev_name_len] = '\0';
            ares_set_local_dev(channel, dev_name);
            free(dev_name);
        }
    }

    // Parse DNS class from string using portion of input
    ares_dns_class_t dns_class;
    if (size > 10) {
        // Create a string from input data for DNS class
        char class_str[16];
        size_t str_len = size < 15 ? size : 15;
        memcpy(class_str, data, str_len);
        class_str[str_len] = '\0';
        
        // Try to parse DNS class
        ares_dns_class_fromstr(&dns_class, class_str);
    }

    // Parse the input as a DNS record using ares_dns_parse
    ares_dns_record_t *dnsrec = nullptr;
    if (size > 0) {
        ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_2, &dnsrec);
        
        if (status == ARES_SUCCESS && dnsrec != nullptr) {
            // Get rcode from DNS record
            ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
            
            // Get number of RRs in answer section
            size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
            
            if (rr_cnt > 0) {
                // Get first RR from answer section
                const ares_dns_rr_t *rr_const = ares_dns_record_rr_get_const(
                    dnsrec, ARES_SECTION_ANSWER, 0);
                
                if (rr_const != nullptr) {
                    // Get TTL of RR
                    unsigned int ttl = ares_dns_rr_get_ttl(rr_const);
                    
                    // Get RR type
                    ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr_const);
                    
                    // Get keys for this RR type
                    size_t key_cnt = 0;
                    const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(rr_type, &key_cnt);
                    
                    // Check key datatypes
                    for (size_t i = 0; i < key_cnt && i < 3; i++) {
                        ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(keys[i]);
                        
                        // If key is U8 type, try to set a value
                        if (datatype == ARES_DATATYPE_U8) {
                            // Get mutable RR
                            ares_dns_rr_t *rr_mut = ares_dns_record_rr_get(
                                dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_3);
                            if (rr_mut != nullptr) {
                                // Set a U8 value using some input data
                                unsigned char u8_val = size > 0 ? data[0] : 0;
                                ares_dns_rr_set_u8(rr_mut, keys[i], u8_val);
                                
                                // Modify TTL
                                unsigned int new_ttl = (ttl + 1) % 3600;
                                ares_dns_rr_set_ttl(rr_mut, new_ttl);
                            }
                        }
                    }
                    
                    // Try to delete the first RR if there's more than one
                    if (rr_cnt > 1) {
                        ares_dns_record_rr_del(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_4);
                    }
                }
            }
            
            // Destroy the DNS record
            ares_dns_record_destroy(dnsrec);
        }
    }

    // Parse input using legacy parsing functions
    struct hostent *host = nullptr;
    struct ares_mx_reply *mx_out = nullptr;
    struct ares_txt_reply *txt_out = nullptr;
    
    // Try to parse as NS reply
    if (size > 0) {
        ares_parse_ns_reply(data, (int)size, &host);
        if (host != nullptr) {
            ares_free_hostent(host);
        }
    }
    
    // Try to parse as MX reply
    if (size > 0) {
        ares_parse_mx_reply(data, (int)size, &mx_out);
        if (mx_out != nullptr) {
            ares_free_data(mx_out);
        }
    }
    
    // Try to parse as TXT reply
    if (size > 0) {
        ares_parse_txt_reply(data, (int)size, &txt_out);
        if (txt_out != nullptr) {
            ares_free_data(txt_out);
        }
    }

    // Get servers from channel
    struct ares_addr_node *servers = nullptr;
    ares_get_servers(channel, &servers);
    if (servers != nullptr) {
        ares_free_data(servers);
    }

    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
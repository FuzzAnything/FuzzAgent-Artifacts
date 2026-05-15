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
//<ID> 755
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 3
//<Quality> {"density":19,"unique_branches":{"ares_dns_record_rr_del":[[460,5,460,29,0,0,4,1],[463,5,463,32,0,0,4,0],[466,5,466,33,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_local_dev","ares_dns_class_fromstr","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_type","ares_dns_rr_get_ttl","ares_dns_rr_get_keys","ares_dns_record_rr_get","ares_dns_rr_set_ttl","ares_dns_rr_key_datatype","ares_dns_rr_set_u8","ares_dns_record_rr_del","ares_dns_record_destroy","ares_parse_ns_reply","ares_free_hostent","ares_parse_mx_reply","ares_free_data","ares_parse_txt_reply","ares_free_data","ares_get_servers","ares_free_data","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_local_dev","ares_dns_class_fromstr","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_type","ares_dns_rr_get_ttl","ares_dns_rr_get_keys","ares_dns_record_rr_get","ares_dns_rr_set_ttl","ares_dns_rr_key_datatype","ares_dns_rr_set_u8","ares_dns_record_rr_del","ares_dns_record_destroy","ares_parse_ns_reply","ares_free_hostent","ares_parse_mx_reply","ares_free_data","ares_parse_txt_reply","ares_free_data","ares_get_servers","ares_free_data","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // Initialize channel
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    options.timeout = 5000;
    options.tries = 4;
    optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    int init_status = ares_init_options(&channel, &options, fuzz_int32_t_1);
    if (init_status != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Set local device (using fixed string for fuzzing)
    ares_set_local_dev(channel, fuzz_str_2);
    
    // Parse DNS class from string
    ares_dns_class_t dns_class;
    ares_dns_class_fromstr(&dns_class, fuzz_str_3);
    
    // Parse input as DNS response using ares_dns_parse
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_4, &dnsrec);
    
    if (parse_status == ARES_SUCCESS && dnsrec != nullptr) {
        // Get DNS response code
        ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
        
        // Process each DNS section
        ares_dns_section_t sections[] = {
            ARES_SECTION_ANSWER,
            ARES_SECTION_AUTHORITY,
            ARES_SECTION_ADDITIONAL
        };
        
        for (size_t s = 0; s < 3; s++) {
            ares_dns_section_t sect = sections[s];
            size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, sect);
            
            for (size_t i = 0; i < rr_cnt && i < 5; i++) {
                // Get const record for reading
                const ares_dns_rr_t *rr_const = ares_dns_record_rr_get_const(dnsrec, sect, i);
                if (rr_const) {
                    // Get record type and TTL
                    ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr_const);
                    unsigned int ttl = ares_dns_rr_get_ttl(rr_const);
                    
                    // Get available keys for this record type
                    size_t key_cnt = 0;
                    const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(rr_type, &key_cnt);
                    
                    // Get mutable record for modifications
                    ares_dns_rr_t *rr_mutable = ares_dns_record_rr_get(dnsrec, sect, fuzz_uint64_t_5);
                    if (rr_mutable) {
                        // Modify TTL if possible
                        ares_dns_rr_set_ttl(rr_mutable, ttl + 1);
                        
                        // Try to modify U8 fields
                        for (size_t k = 0; k < key_cnt && k < 3; k++) {
                            ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(keys[k]);
                            if (datatype == ARES_DATATYPE_U8) {
                                // Use data byte for value, modulo to ensure valid range
                                unsigned char val = (size > 0) ? (data[0] % 256) : 1;
                                ares_dns_rr_set_u8(rr_mutable, keys[k], val);
                            }
                        }
                        
                        // Delete every 3rd record (if not the only one)
                        if (rr_cnt > 1 && i % 3 == 0) {
                            ares_dns_record_rr_del(dnsrec, sect, fuzz_uint64_t_6);
                            // Adjust indices after deletion
                            i--;
                            rr_cnt--;
                        }
                    }
                }
            }
        }
        
        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Try to parse as various reply types using the input data
    // NS reply
    struct hostent *host = nullptr;
    int ns_status = ares_parse_ns_reply(data, (size > INT_MAX) ? INT_MAX : (int)size, &host);
    if (ns_status == ARES_SUCCESS && host != nullptr) {
        ares_free_hostent(host);
    }
    
    // MX reply
    struct ares_mx_reply *mx_out = nullptr;
    int mx_status = ares_parse_mx_reply(data, (size > INT_MAX) ? INT_MAX : (int)size, &mx_out);
    if (mx_status == ARES_SUCCESS && mx_out != nullptr) {
        ares_free_data(mx_out);
    }
    
    // TXT reply
    struct ares_txt_reply *txt_out = nullptr;
    int txt_status = ares_parse_txt_reply(data, (size > INT_MAX) ? INT_MAX : (int)size, &txt_out);
    if (txt_status == ARES_SUCCESS && txt_out != nullptr) {
        ares_free_data(txt_out);
    }
    
    // Get servers (even though we may not use them)
    struct ares_addr_node *servers = nullptr;
    int get_servers_status = ares_get_servers(channel, &servers);
    if (get_servers_status == ARES_SUCCESS && servers != nullptr) {
        ares_free_data(servers);
    }
    
    // Cleanup
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
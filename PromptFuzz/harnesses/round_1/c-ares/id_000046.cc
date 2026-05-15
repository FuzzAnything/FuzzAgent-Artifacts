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
//<ID> 914
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 4
//<Quality> {"density":16,"unique_branches":{"ares_dns_rr_get_keys":[[643,5,643,29,0,0,4,0]],"ares_dns_rr_key_tostr":[[414,5,414,32,0,0,4,0],[417,5,417,30,0,0,4,0],[420,5,420,30,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_socket_callback","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_rr_cnt","ares_dns_record_rr_cnt","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_record_rr_get","ares_dns_rr_get_class","ares_dns_rr_get_type","ares_dns_rr_get_ttl","ares_dns_rr_get_keys","ares_dns_rr_key_tostr","ares_dns_rr_key_datatype","ares_dns_rr_get_abin","ares_dns_rr_get_u32","ares_getnameinfo","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_socket_callback","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_rr_cnt","ares_dns_record_rr_cnt","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_record_rr_get","ares_dns_rr_get_class","ares_dns_rr_get_type","ares_dns_rr_get_ttl","ares_dns_rr_get_keys","ares_dns_rr_key_tostr","ares_dns_rr_key_datatype","ares_dns_rr_get_abin","ares_getnameinfo","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Include c-ares headers

// Dummy socket callback function
static int  id_000046_dummy_socket_create(ares_socket_t socket_fd, int type, void *user_data) {
    return 0;
}

// Dummy nameinfo callback function
static void  id_000046_dummy_nameinfo_callback(void *arg, int status, int timeouts,
                                   const char *node, const char *service) {
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Initialize channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Set socket callback
    ares_set_socket_callback(channel,  id_000046_dummy_socket_create, nullptr);
    
    // Parse input as DNS response using ares_dns_parse
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
    
    // Try parsing as TXT reply (extended version if available, otherwise use standard)
    struct ares_txt_ext *txt_out = nullptr;
    // Use ares_parse_txt_reply if available, but we'll parse using ares_dns_parse above
    
    if (parse_status == ARES_SUCCESS && dnsrec != nullptr) {
        // Get DNS response code
        ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
        
        // Get number of sections
        size_t answer_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
        size_t authority_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_AUTHORITY);
        size_t additional_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ADDITIONAL);
        
        // Process answer section
        for (size_t i = 0; i < answer_count; i++) {
            // Get RR - try const version first
            const ares_dns_rr_t *rr_const = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, i);
            ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_2);
            
            if (rr_const != nullptr) {
                // Get RR class and type
                ares_dns_class_t rr_class = ares_dns_rr_get_class(rr_const);
                ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr_const);
                
                // Get TTL
                unsigned int ttl = ares_dns_rr_get_ttl(rr_const);
                
                // Get keys for this RR type
                size_t key_count = 0;
                const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(rr_type, &key_count);
                
                // Process each key
                for (size_t k = 0; k < key_count; k++) {
                    ares_dns_rr_key_t key = keys[k];
                    
                    // Get key string representation
                    const char *key_str = ares_dns_rr_key_tostr(key);
                    
                    // Get key datatype
                    ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
                    
                    // Try to extract data based on datatype
                    switch (datatype) {
                        case ARES_DATATYPE_INADDR:
                        case ARES_DATATYPE_INADDR6:
                        case ARES_DATATYPE_NAME:
                        case ARES_DATATYPE_STR:
                        case ARES_DATATYPE_BIN:
                        case ARES_DATATYPE_BINP:
                            {
                                // Try to get binary data
                                size_t data_len = 0;
                                const unsigned char *bin_data = ares_dns_rr_get_abin(rr_const, key, fuzz_uint64_t_3, &data_len);
                                if (bin_data != nullptr && data_len > 0) {
                                    // Do something with the data
                                }
                            }
                            break;
                        case ARES_DATATYPE_U8:
                        case ARES_DATATYPE_U16:
                        case ARES_DATATYPE_U32:
                            if (datatype == ARES_DATATYPE_U32) {
                                unsigned int u32_val = ares_dns_rr_get_u32(rr_const, key);
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        
        // Create a dummy socket address for getnameinfo
        struct sockaddr_in sa;
        memset(&sa, 0, sizeof(sa));
        sa.sin_family = AF_INET;
        sa.sin_addr.s_addr = htonl(0x7f000001); // 127.0.0.1
        sa.sin_port = htons(80);
        
        // Perform reverse DNS lookup (getnameinfo)
        ares_getnameinfo(channel, (struct sockaddr*)&sa, sizeof(sa), 
                        ARES_NI_LOOKUPHOST | ARES_NI_LOOKUPSERVICE,
                         id_000046_dummy_nameinfo_callback, nullptr);
        
        // Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Clean up channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();
    
    return 0;
}
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
//<ID> 170
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_local_dev","ares_dns_class_fromstr","ares_dns_class_fromstr","ares_parse_ns_reply","ares_dns_parse","ares_dns_record_rr_get_const","ares_dns_record_rr_del","ares_dns_rr_get_keys","ares_dns_record_rr_get","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_key_datatype","ares_dns_rr_set_ttl","ares_dns_record_destroy","ares_get_servers","ares_free_data","ares_parse_mx_reply","ares_free_data","ares_parse_txt_reply","ares_free_data","ares_free_hostent","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_local_dev","ares_dns_class_fromstr","ares_parse_ns_reply","ares_dns_parse","ares_dns_record_rr_get_const","ares_dns_record_rr_del","ares_dns_rr_get_keys","ares_dns_record_rr_get","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_key_datatype","ares_dns_rr_set_ttl","ares_dns_record_destroy","ares_get_servers","ares_free_data","ares_parse_mx_reply","ares_free_data","ares_parse_txt_reply","ares_free_data","ares_free_hostent","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<38) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Initialize channel
    ares_channel_t *channel = nullptr;
    if (ares_init_options(&channel, nullptr, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // 1. Use ares_set_local_dev
    ares_set_local_dev(channel, fuzz_str_2);
    
    // 2. Use ares_dns_class_fromstr to parse class from input
    ares_dns_class_t qclass;
    const char *class_str = "IN";  // Default to Internet class
    if (size > 2) {
        // Use first few bytes to create a class string
        char class_buf[4] = {0};
        size_t len = size < 3 ? size : 3;
        for (size_t i = 0; i < len; i++) {
            class_buf[i] = 'A' + (data[i] % 26);  // Convert to letters
        }
        class_buf[len] = '\0';
        ares_dns_class_fromstr(&qclass, class_buf);
    } else {
        ares_dns_class_fromstr(&qclass, fuzz_str_3);
    }
    
    // 3. Parse input as DNS response using ares_parse_ns_reply
    struct hostent *host = nullptr;
    ares_parse_ns_reply(data, (int)size, &host);
    
    // 4. Create a DNS record and manipulate it
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_4, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Get the number of answer records
        size_t answer_count = 0;
        // Try to get answer records (we'll check by attempting to access them)
        for (size_t i = 0; i < 10; i++) {  // Reasonable upper bound
            const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, i);
            if (!rr) break;
            answer_count++;
        }
        
        // 5. Use ares_dns_record_rr_del if we have at least one answer record
        if (answer_count > 0) {
            // Delete the first answer record
            ares_dns_record_rr_del(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_5);
        }
        
        // 6. Create a new RR and set u32 value if possible
        // First check what RR types are available for u32 values
        // For example, SOA records have serial number as u32
        const ares_dns_rr_key_t *keys = nullptr;
        size_t key_count = 0;
        keys = ares_dns_rr_get_keys(ARES_REC_TYPE_SOA, &key_count);
        
        // Try to set u32 value on a new RR
        ares_dns_rr_t *new_rr = nullptr;
        // We need to add a RR to the record first - let's try to add a new answer RR
        // Create a simple RR (this is simplified - actual creation requires more setup)
        
        // Instead, let's try to modify an existing RR if available
        for (size_t i = 0; i < 5; i++) {
            ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_6);
            if (rr) {
                // Check if this RR type supports u32 values
                ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                keys = ares_dns_rr_get_keys(rr_type, &key_count);
                
                // Look for keys that might accept u32 values
                for (size_t j = 0; j < key_count; j++) {
                    ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(keys[j]);
                    if (datatype == ARES_DATATYPE_U32) {
                        // Try to set a u32 value (using TTL as example)
                        // Note: ares_dns_rr_set_u32 might not exist in all versions
                        // Using ares_dns_rr_set_ttl instead which sets u32 TTL
                        ares_dns_rr_set_ttl(rr, fuzz_uint32_t_7);  // Set TTL to 1 hour
                        break;
                    }
                }
                break;
            }
        }
        
        ares_dns_record_destroy(dnsrec);
    }
    
    // 7. Get servers from channel
    struct ares_addr_node *servers = nullptr;
    int ret = ares_get_servers(channel, &servers);
    if (ret == ARES_SUCCESS && servers) {
        // Free the servers list
        ares_free_data(servers);
    }
    
    // Parse as other reply types if data looks appropriate
    if (size > 12) {  // Minimum DNS header size
        struct ares_mx_reply *mx_out = nullptr;
        ares_parse_mx_reply(data, (int)size, &mx_out);
        if (mx_out) {
            ares_free_data(mx_out);
        }
        
        struct ares_txt_reply *txt_out = nullptr;
        ares_parse_txt_reply(data, (int)size, &txt_out);
        if (txt_out) {
            ares_free_data(txt_out);
        }
    }
    
    // Clean up hostent if allocated
    if (host) {
        ares_free_hostent(host);
    }
    
    // Clean up channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();
    
    return 0;
}
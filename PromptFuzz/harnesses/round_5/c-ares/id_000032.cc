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
//<ID> 436
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_parse_txt_reply","ares_strerror","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_name","ares_dns_rr_get_ttl","ares_dns_record_rr_get","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_get_abin_cnt","ares_dns_rr_del_abin","ares_dns_record_destroy","ares_set_servers_ports_csv","ares_free_data","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_parse_txt_reply","ares_strerror","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_name","ares_dns_rr_get_ttl","ares_dns_record_rr_get","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_get_abin_cnt","ares_dns_rr_del_abin","ares_dns_record_destroy","ares_set_servers_ports_csv","ares_free_data","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Include necessary c-ares headers

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimal data

    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize with default options
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 1. Parse TXT reply using input data
    struct ares_txt_reply *txt_out = nullptr;
    // Use first half of data for TXT parsing
    size_t txt_size = size > 100 ? 100 : size;  // Limit size for parsing
    int parse_result = ares_parse_txt_reply(data, txt_size, &txt_out);
    
    // Use ares_strerror to get error message if parsing failed
    const char *error_str = ares_strerror(parse_result);

    // 2. Create a DNS resource record for testing other APIs
    ares_dns_record_t *dnsrec = nullptr;
    // Use second half of data for DNS parsing (if enough data)
    if (size > txt_size) {
        size_t dns_size = size - txt_size;
        // Try to parse as DNS record
        ares_dns_parse(data + txt_size, dns_size, 0, &dnsrec);
        
        if (dnsrec) {
            // Get the first resource record from answer section
            size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
            if (rr_count > 0) {
                const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, 0);
                
                if (rr) {
                    // 3. Get RR name
                    const char *rr_name = ares_dns_rr_get_name(rr);
                    
                    // 4. Get RR TTL
                    unsigned int ttl = ares_dns_rr_get_ttl(rr);
                    
                    // For mutable operations, create a mutable copy
                    ares_dns_rr_t *mutable_rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, 0);
                    if (mutable_rr) {
                        // 5. Try to delete binary data if present
                        // Check for any binary data fields
                        ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                        const ares_dns_rr_key_t *keys = nullptr;
                        size_t key_count = 0;
                        keys = ares_dns_rr_get_keys(rr_type, &key_count);
                        
                        // Find a binary data field
                        for (size_t i = 0; i < key_count; i++) {
                            size_t bin_cnt = ares_dns_rr_get_abin_cnt(mutable_rr, keys[i]);
                            if (bin_cnt > 0) {
                                // Try to delete the first binary data entry
                                ares_dns_rr_del_abin(mutable_rr, keys[i], 0);
                                break;
                            }
                        }
                    }
                }
            }
            
            // Clean up DNS record
            ares_dns_record_destroy(dnsrec);
        }
    }

    // 6. Set servers using CSV string from input data
    // Use a portion of input as CSV string (null-terminated)
    size_t csv_len = size > 200 ? 200 : size;
    char *csv_buf = (char *)malloc(csv_len + 1);
    if (csv_buf) {
        memcpy(csv_buf, data, csv_len > size ? size : csv_len);
        csv_buf[csv_len] = '\0';  // Ensure null termination
        
        // Try to set servers (may fail with invalid CSV, that's OK for fuzzing)
        ares_set_servers_ports_csv(channel, csv_buf);
        
        free(csv_buf);
    }

    // Clean up TXT reply if allocated
    if (txt_out) {
        ares_free_data(txt_out);
    }

    // Clean up channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();

    return 0;
}
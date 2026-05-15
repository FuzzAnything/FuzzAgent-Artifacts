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
//<ID> 344
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_servers_ports_csv","ares_dns_parse","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_query_cnt","ares_dns_record_query_set_type","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_set_ttl","ares_dns_rr_get_type","ares_dns_rr_key_tostr","ares_dns_rr_set_bin","ares_dns_rr_get_opt_cnt","ares_dns_rr_get_opt","ares_dns_rr_del_opt_byid","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_servers_ports_csv","ares_dns_parse","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_query_cnt","ares_dns_record_query_set_type","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_set_ttl","ares_dns_rr_get_type","ares_dns_rr_key_tostr","ares_dns_rr_set_bin","ares_dns_rr_get_opt_cnt","ares_dns_rr_get_opt","ares_dns_rr_del_opt_byid","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<47) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
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

    // 1. Use ares_set_servers_ports_csv with the input data
    // Convert input to null-terminated string for CSV parsing
    if (size > 0 && size < 1024) {  // Limit size for safety
        char *servers_csv = static_cast<char*>(malloc(size + 1));
        if (servers_csv) {
            memcpy(servers_csv, data, size);
            servers_csv[size] = '\0';
            // Use only if it looks like CSV (contains comma or space)
            if (strchr(servers_csv, ',') || strchr(servers_csv, ' ')) {
                ares_set_servers_ports_csv(channel, servers_csv);
            }
            free(servers_csv);
        }
    }

    // 2. Parse input as DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
    
    if (parse_status == ARES_SUCCESS && dnsrec) {
        // 3. Get opcode and convert to string using ares_dns_opcode_tostr
        ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
        const char *opcode_str = ares_dns_opcode_tostr(opcode);
        // opcode_str is used for informational purposes
        
        // 4. Process queries if any exist
        size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
        if (query_cnt > 0) {
            // Get first query and set its type
            // We'll cycle through some common types based on input size
            ares_dns_rec_type_t new_type = static_cast<ares_dns_rec_type_t>(
                (size % 5) + 1  // A, NS, CNAME, SOA, PTR
            );
            ares_dns_record_query_set_type(dnsrec, fuzz_uint64_t_2, new_type);
        }

        // 5. Process resource records
        size_t answer_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
        if (answer_cnt > 0) {
            // Get first answer RR
            ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_3);
            if (rr) {
                // 6. Set TTL based on input size
                unsigned int ttl = static_cast<unsigned int>(size % 3600);
                ares_dns_rr_set_ttl(rr, ttl);
                
                // 7. Get RR key and convert to string using ares_dns_rr_key_tostr
                ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                ares_dns_rr_key_t sample_key = ARES_RR_A_ADDR;  // Example key
                const char *key_str = ares_dns_rr_key_tostr(sample_key);
                // key_str is used for informational purposes
                
                // 8. Try to set a string value if appropriate for this RR type
                // For example, for CNAME or NS records
                if (rr_type == ARES_REC_TYPE_CNAME || rr_type == ARES_REC_TYPE_NS) {
                    const char *test_str = "test.example.com";
                    // Note: ares_dns_rr_set_str is not in the provided list,
                    // but we'll use ares_dns_rr_set_bin as alternative
                    ares_dns_rr_set_bin(rr, ARES_RR_CNAME_CNAME, 
                                       reinterpret_cast<const unsigned char*>(fuzz_str_4),
                                       static_cast<size_t>(fuzz_str_sz_4));
                }
                
                // 9. Try to get OPT record data if it exists
                size_t opt_cnt = ares_dns_rr_get_opt_cnt(rr, ARES_RR_OPT_OPTIONS);
                if (opt_cnt > 0) {
                    const unsigned char *opt_val = nullptr;
                    size_t opt_len = 0;
                    unsigned short opt_data = ares_dns_rr_get_opt(rr, ARES_RR_OPT_OPTIONS, 
                                                                 fuzz_uint64_t_5, &opt_val, &opt_len);
                    // Try to delete the option by its ID if we got valid data
                    if (opt_len > 0) {
                        ares_dns_rr_del_opt_byid(rr, ARES_RR_OPT_OPTIONS, opt_data);
                    }
                }
            }
        }
        
        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // Clean up channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();
    
    return 0;
}
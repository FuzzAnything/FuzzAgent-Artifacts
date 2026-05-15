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
//<ID> 1237
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"ares_dns_rr_set_u16":[[1168,7,1168,18,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_local_ip4","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_query_cnt","ares_dns_record_query_set_name","ares_dns_record_query_set_name","ares_dns_rr_get_keys","ares_dns_opt_get_datatype","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_str","ares_dns_record_rr_get","ares_dns_rr_set_u16","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_local_ip4","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_query_cnt","ares_dns_record_query_set_name","ares_dns_rr_get_keys","ares_dns_opt_get_datatype","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_str","ares_dns_record_rr_get","ares_dns_rr_set_u16","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* f_data, size_t f_size) {
	if(f_size<55) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel for DNS operations
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set local IPv4 address using data (convert first 4 bytes to IP)
    unsigned int local_ip = 0;
    if (size >= 4) {
        memcpy(&local_ip, data, 4);
        local_ip = ntohl(local_ip);  // Convert to host byte order
        ares_set_local_ip4(channel, fuzz_uint32_t_1);
    }

    // Parse DNS response from input data
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_2, &dnsrec);
    
    if (parse_status == ARES_SUCCESS && dnsrec != nullptr) {
        // Get DNS response code
        ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
        (void)rcode;  // Use result to avoid unused variable warning

        // Get query count
        size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
        
        if (query_cnt > 0) {
            // Modify first query name using available data
            const char *new_name = "fuzzed.example.com";
            if (size > 0) {
                // Use data to create a modified name (simplified example)
                char temp_name[256];
                snprintf(temp_name, sizeof(temp_name), "fuzzed-%02x.example.com", 
                         data[0] & 0xFF);
                ares_dns_record_query_set_name(dnsrec, fuzz_uint64_t_3, temp_name);
            } else {
                ares_dns_record_query_set_name(dnsrec, fuzz_uint64_t_5, fuzz_str_4);
            }
        }

        // Get RR keys and check their datatypes
        size_t key_cnt = 0;
        const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(ARES_REC_TYPE_A, &key_cnt);
        if (keys != nullptr && key_cnt > 0) {
            // Check datatype of first key with opt=0
            ares_dns_opt_datatype_t datatype = ares_dns_opt_get_datatype(keys[0], fuzz_uint16_t_6);
            (void)datatype;  // Use result
        }

        // Process resource records
        for (ares_dns_section_t sect = ARES_SECTION_ANSWER; 
             sect <= ARES_SECTION_ADDITIONAL; 
             sect = static_cast<ares_dns_section_t>(sect + 1)) {
            
            size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, sect);
            for (size_t i = 0; i < rr_cnt && i < 3; i++) {
                const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, sect, i);
                if (rr != nullptr) {
                    // Get string value from RR
                    const char *str_val = ares_dns_rr_get_str(rr, ARES_RR_A_ADDR);
                    (void)str_val;  // Use result
                    
                    // Set a uint16_t value in RR (for demonstration)
                    // Note: We need a non-const rr for this, so get mutable version
                    ares_dns_rr_t *mutable_rr = ares_dns_record_rr_get(dnsrec, sect, fuzz_uint64_t_7);
                    if (mutable_rr != nullptr) {
                        // Try to set a uint16_t value if the RR supports it
                        // Using PORT as an example key that takes u16
                        ares_status_t set_status = ares_dns_rr_set_u16(
                            mutable_rr, 
                            ARES_RR_SRV_PORT, 
                            static_cast<unsigned short>(size & 0xFFFF)
                        );
                        (void)set_status;  // Use result
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
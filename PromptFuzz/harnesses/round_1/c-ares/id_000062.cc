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
//<ID> 1162
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 3
//<Quality> {"density":14,"unique_branches":{"ares_dns_rr_del_abin":[[922,7,922,59,0,0,4,1],[927,7,927,19,0,0,4,0]],"ares_dns_rr_set_u32":[[1181,7,1181,57,0,0,4,0]]},"library_calls":["ares_library_init","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_class","ares_dns_rr_get_addr","ares_dns_rr_get_str","ares_free_string","ares_dns_record_set_id","ares_dns_record_rr_add","ares_dns_rr_add_abin","ares_dns_record_rr_add","ares_dns_rr_set_addr6","ares_dns_rr_set_u32","ares_dns_rr_del_abin","ares_dns_record_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_class","ares_dns_rr_get_addr","ares_dns_rr_get_str","ares_free_string","ares_dns_record_set_id","ares_dns_record_rr_add","ares_dns_rr_add_abin","ares_dns_record_rr_add","ares_dns_rr_set_addr6","ares_dns_rr_set_u32","ares_dns_rr_del_abin","ares_dns_record_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size) {
	if(f_size<66) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_5, fuzz_char_array_size_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
	
		const unsigned char * fuzzer_var_ares_dns_rr_add_abin_10_2 = fuzz_char_array_5;
		size_t fuzzer_size_ares_dns_rr_add_abin_10_2 = static_cast<size_t>(fuzz_char_array_size_5);//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Parse DNS record from input data
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec != nullptr) {
        // Get count of answer section records
        size_t answer_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
        
        // Process each answer record if available
        for (size_t i = 0; i < answer_cnt; ++i) {
            const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, i);
            if (rr) {
                // Get record class
                ares_dns_class_t rr_class = ares_dns_rr_get_class(rr);
                
                // Try to get IPv4 address (for A records)
                ares_dns_rr_key_t key = ARES_RR_A_ADDR;
                const struct in_addr *addr = ares_dns_rr_get_addr(rr, key);
                
                // Try to get string value (for CNAME, PTR, etc.)
                key = ARES_RR_CNAME_CNAME;
                const char *str = ares_dns_rr_get_str(rr, key);
                if (str) {
                    // Duplicate string to demonstrate ares_free_string
                    char *str_copy = strdup(str);
                    if (str_copy) {
                        ares_free_string(str_copy);
                    }
                }
            }
        }
        
        // Set new ID for the DNS record (using first 2 bytes of input as ID)
        unsigned short new_id = 0;
        if (size >= 2) {
            new_id = (data[0] << 8) | data[1];
        }
        ares_dns_record_set_id(dnsrec, fuzz_uint16_t_2);
        
        // Add a new TXT record to authority section
        ares_dns_rr_t *new_rr = nullptr;
        status = ares_dns_record_rr_add(&new_rr, dnsrec, ARES_SECTION_AUTHORITY,
                                       fuzz_str_3, ARES_REC_TYPE_TXT,
                                       ARES_CLASS_IN, fuzz_uint32_t_4);
        
        if (status == ARES_SUCCESS && new_rr) {
            // Set binary data (TXT record data)
            unsigned char bin_data[] = {'t', 'e', 's', 't'};
            status = ares_dns_rr_add_abin(new_rr, ARES_RR_TXT_DATA, 
                                         fuzzer_var_ares_dns_rr_add_abin_10_2, fuzzer_size_ares_dns_rr_add_abin_10_2);
            
            // Set IPv6 address for AAAA record (though this is TXT record, 
            // we'll create another record for AAAA)
            ares_dns_rr_t *aaaa_rr = nullptr;
            status = ares_dns_record_rr_add(&aaaa_rr, dnsrec, ARES_SECTION_ADDITIONAL,
                                           fuzz_str_6, ARES_REC_TYPE_AAAA,
                                           ARES_CLASS_IN, fuzz_uint32_t_7);
            
            if (status == ARES_SUCCESS && aaaa_rr) {
                struct ares_in6_addr addr6;
                memset(&addr6, 0, sizeof(addr6));
                if (size >= sizeof(addr6._S6_un._S6_u8)) {
                    memcpy(&addr6._S6_un._S6_u8, data, 
                           sizeof(addr6._S6_un._S6_u8));
                }
                status = ares_dns_rr_set_addr6(aaaa_rr, ARES_RR_AAAA_ADDR, &addr6);
                
                // Set 32-bit value (for example, for MX preference)
                status = ares_dns_rr_set_u32(aaaa_rr, ARES_RR_MX_PREFERENCE, fuzz_uint32_t_8);
                
                // Delete binary data by index (if we had added any)
                status = ares_dns_rr_del_abin(aaaa_rr, ARES_RR_TXT_DATA, fuzz_uint64_t_9);
            }
        }
        
        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Library cleanup
    ares_library_cleanup();
    
    return 0;
}
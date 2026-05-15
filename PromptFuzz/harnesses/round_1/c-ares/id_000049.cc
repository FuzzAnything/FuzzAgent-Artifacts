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
//<ID> 937
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["ares_library_init","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_name","ares_dns_rr_get_class","ares_dns_record_duplicate","ares_dns_record_rr_get_const","ares_dns_rr_get_addr","ares_dns_addr_to_ptr","ares_free_string","ares_dns_rr_get_addr6","ares_dns_addr_to_ptr","ares_free_string","ares_dns_record_rr_add","ares_dns_rr_add_abin","ares_dns_rr_get_str","ares_dns_record_destroy","ares_dns_record_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_name","ares_dns_rr_get_class","ares_dns_record_duplicate","ares_dns_record_rr_get_const","ares_dns_rr_get_addr","ares_dns_addr_to_ptr","ares_free_string","ares_dns_rr_get_addr6","ares_dns_addr_to_ptr","ares_free_string","ares_dns_record_rr_add","ares_dns_rr_add_abin","ares_dns_rr_get_str","ares_dns_record_destroy","ares_dns_record_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_4, fuzz_char_array_size_4, fdp);
	
		const unsigned char * fuzzer_var_ares_dns_rr_add_abin_15_2 = fuzz_char_array_4;
		size_t fuzzer_size_ares_dns_rr_add_abin_15_2 = static_cast<size_t>(fuzz_char_array_size_4);//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Create a DNS record by parsing input data
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec != nullptr) {
        // Get resource record count in answer section
        size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
        
        if (rr_count > 0) {
            // Get first resource record from answer section
            const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, 0);
            if (rr != nullptr) {
                // 1. Get string value from RR (e.g., domain name)
                const char *name = ares_dns_rr_get_name(rr);
                if (name) {
                    // Use the name string
                    (void)name;
                }
                
                // 2. Get class of the resource record
                ares_dns_class_t rr_class = ares_dns_rr_get_class(rr);
                (void)rr_class;
                
                // Duplicate the DNS record
                ares_dns_record_t *duplicated = ares_dns_record_duplicate(dnsrec);
                
                if (duplicated != nullptr) {
                    // Get the duplicated RR from answer section
                    const ares_dns_rr_t *dup_rr = ares_dns_record_rr_get_const(duplicated, ARES_SECTION_ANSWER, 0);
                    if (dup_rr != nullptr) {
                        // 3. Try to get IPv4 address
                        const struct in_addr *addr4 = ares_dns_rr_get_addr(dup_rr, ARES_RR_A_ADDR);
                        if (addr4 != nullptr) {
                            // Convert to ares_addr structure
                            ares_addr addr_struct;
                            addr_struct.family = AF_INET;
                            addr_struct.addr.addr4 = *addr4;
                            
                            // 5. Convert address to PTR string
                            char *ptr_str = ares_dns_addr_to_ptr(&addr_struct);
                            if (ptr_str != nullptr) {
                                ares_free_string(ptr_str);
                            }
                        }
                        
                        // 4. Try to get IPv6 address
                        const struct ares_in6_addr *addr6 = ares_dns_rr_get_addr6(dup_rr, ARES_RR_AAAA_ADDR);
                        if (addr6 != nullptr) {
                            // Convert to ares_addr structure
                            ares_addr addr6_struct;
                            addr6_struct.family = AF_INET6;
                            addr6_struct.addr.addr6 = *addr6;
                            
                            // Convert address to PTR string
                            char *ptr_str6 = ares_dns_addr_to_ptr(&addr6_struct);
                            if (ptr_str6 != nullptr) {
                                ares_free_string(ptr_str6);
                            }
                        }
                        
                        // 6. Add binary data to the RR (needs non-const RR)
                        // First get mutable RR from duplicated record
                        // Note: We need to get the RR at index 0 again, but mutable
                        // Since we don't have direct API for mutable get, we'll work with
                        // a new RR that we add to the duplicated record
                        
                        // Create a new RR of type TXT to add binary data
                        ares_dns_rr_t *new_rr = nullptr;
                        status = ares_dns_record_rr_add(&new_rr, duplicated, 
                                                       ARES_SECTION_ANSWER,
                                                       fuzz_str_2,
                                                       ARES_REC_TYPE_TXT,
                                                       ARES_CLASS_IN,
                                                       fuzz_uint32_t_3);
                        
                        if (status == ARES_SUCCESS && new_rr != nullptr) {
                            // Add binary data to the new RR
                            const unsigned char bin_data[] = {0x01, 0x02, 0x03, 0x04};
                            status = ares_dns_rr_add_abin(new_rr, ARES_RR_TXT_DATA, 
                                                         fuzzer_var_ares_dns_rr_add_abin_15_2, fuzzer_size_ares_dns_rr_add_abin_15_2);
                            
                            // Try to get string value from the new RR
                            const char *new_name = ares_dns_rr_get_str(new_rr, ARES_RR_TXT_DATA);
                            (void)new_name;
                        }
                    }
                    
                    // Clean up duplicated record
                    ares_dns_record_destroy(duplicated);
                }
            }
        }
        
        // Clean up original record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Clean up c-ares library
    ares_library_cleanup();
    
    return 0;
}
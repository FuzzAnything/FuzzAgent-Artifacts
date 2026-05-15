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
//<ID> 1046
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 2
//<Quality> {"density":15,"unique_branches":{"ares_dns_rr_get_addr":[[762,7,762,60,0,0,4,0]],"ares_dns_rr_get_addr6":[[779,7,779,61,0,0,4,0]]},"library_calls":["ares_library_initialized","ares_library_init","ares_dns_parse","ares_library_cleanup","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_class","ares_dns_rr_get_type","ares_dns_rr_get_addr","ares_inet_ntop","ares_inet_pton","ares_dns_rr_get_str","ares_free_string","ares_dns_rr_get_addr6","ares_dns_rr_get_name","ares_dns_record_rr_add","ares_dns_rr_set_addr6","ares_dns_rr_add_abin","ares_dns_rr_set_u32","ares_dns_record_destroy","ares_library_cleanup"],"critical_calls":["ares_library_initialized","ares_library_init","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_class","ares_dns_rr_get_type","ares_dns_rr_get_addr6","ares_dns_rr_get_name","ares_dns_record_rr_add","ares_dns_rr_set_addr6","ares_dns_rr_add_abin","ares_dns_rr_set_u32","ares_dns_record_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

// Constants needed from c-ares headers (not provided in prompt)
#define ARES_DNS_SECTION_ANSWER 1
#define ARES_DNS_REC_TYPE_A 1
#define ARES_DNS_REC_TYPE_AAAA 28
#define ARES_DNS_RR_A_ADDR 0
#define ARES_DNS_RR_AAAA_ADDR 0
#define ARES_DNS_CLASS_IN 1
#define ARES_SUCCESS 0

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
	//fuzzer shim end}




    if (size < 12) {
	return 0;
	}
	 // Minimum DNS header size
    
    // Initialize library
    if (ares_library_initialized() == 0) {
        ares_library_init(fuzz_int32_t_1);
    }
    
    // Parse DNS response from input data
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_2, &dnsrec);
    if (status != ARES_SUCCESS || !dnsrec) {
        ares_library_cleanup();
        return 0;
    }
    
    // Get count of resource records in answer section
    size_t answer_count = ares_dns_record_rr_cnt(dnsrec, (ares_dns_section_t)ARES_DNS_SECTION_ANSWER);
    
    // Process each answer record
    for (size_t i = 0; i < answer_count; i++) {
        const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(
            dnsrec, (ares_dns_section_t)ARES_DNS_SECTION_ANSWER, i);
        if (!rr) continue;
        
        // Get record class
        ares_dns_class_t rr_class = ares_dns_rr_get_class(rr);
        
        // Get record type
        ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
        
        if (rr_type == ARES_DNS_REC_TYPE_A) {
            // IPv4 address record
            const struct in_addr *addr = ares_dns_rr_get_addr(
                rr, (ares_dns_rr_key_t)ARES_DNS_RR_A_ADDR);
            if (addr) {
                // Convert to string and back using ares_inet_pton
                char addr_str[INET_ADDRSTRLEN];
                const char *str_result = ares_inet_ntop(fuzz_int32_t_3, addr, addr_str, sizeof(addr_str));
                if (str_result) {
                    struct in_addr parsed_addr;
                    int pton_result = ares_inet_pton(fuzz_int32_t_4, addr_str, &parsed_addr);
                    (void)pton_result; // Use result to avoid unused warning
                }
                
                // Get name as string
                const char *name = ares_dns_rr_get_str(rr, (ares_dns_rr_key_t)0);
                if (name) {
                    // Duplicate string to test ares_free_string
                    char *name_copy = strdup(name);
                    if (name_copy) {
                        ares_free_string(name_copy);
                    }
                }
            }
        } else if (rr_type == ARES_DNS_REC_TYPE_AAAA) {
            // IPv6 address record
            const struct ares_in6_addr *addr6 = ares_dns_rr_get_addr6(
                rr, (ares_dns_rr_key_t)ARES_DNS_RR_AAAA_ADDR);
            if (addr6) {
                // Add a new record similar to this one
                ares_dns_rr_t *new_rr = nullptr;
                const char *name = ares_dns_rr_get_name(rr);
                if (name) {
                    status = ares_dns_record_rr_add(
                        &new_rr, dnsrec, (ares_dns_section_t)ARES_DNS_SECTION_ANSWER,
                        name, (ares_dns_rec_type_t)ARES_DNS_REC_TYPE_AAAA,
                        (ares_dns_class_t)ARES_DNS_CLASS_IN, fuzz_uint32_t_5);
                    
                    if (status == ARES_SUCCESS && new_rr) {
                        // Set modified IPv6 address (flip last byte)
                        struct ares_in6_addr modified_addr6 = *addr6;
                        modified_addr6._S6_un._S6_u8[15] ^= 0xFF;
                        
                        status = ares_dns_rr_set_addr6(
                            new_rr, (ares_dns_rr_key_t)ARES_DNS_RR_AAAA_ADDR, &modified_addr6);
                        
                        // Add original address as binary data
                        if (status == ARES_SUCCESS) {
                            status = ares_dns_rr_add_abin(
                                new_rr, (ares_dns_rr_key_t)ARES_DNS_RR_AAAA_ADDR,
                                (const unsigned char *)addr6, sizeof(*addr6));
                        }
                        
                        // Set a TTL value
                        if (status == ARES_SUCCESS) {
                            status = ares_dns_rr_set_u32(new_rr, (ares_dns_rr_key_t)0, fuzz_uint32_t_6);
                        }
                    }
                }
            }
        }
    }
    
    // Clean up
    ares_dns_record_destroy(dnsrec);
    ares_library_cleanup();
    
    return 0;
}
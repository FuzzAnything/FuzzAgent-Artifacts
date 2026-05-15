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
//<ID> 450
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 16
//<Quality> {"density":10,"unique_branches":{"ares_dns_rr_key_tostr":[[273,5,273,24,0,0,4,1],[276,5,276,28,0,0,4,0],[279,5,279,29,0,0,4,0],[282,5,282,27,0,0,4,0],[303,5,303,27,0,0,4,0],[306,5,306,27,0,0,4,0],[315,5,315,31,0,0,4,0],[321,5,321,26,0,0,4,0],[351,5,351,30,0,0,4,0],[363,5,363,29,0,0,4,0],[441,5,441,29,0,0,4,0]],"ares_strerror":[[36,5,36,22,0,0,4,1],[38,5,38,23,0,0,4,0],[50,5,50,23,0,0,4,0],[54,5,54,23,0,0,4,0],[68,5,68,22,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_local_ip4","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_class","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_key_tostr","ares_dns_record_create","ares_dns_record_query_add","ares_dns_opt_get_datatype","ares_dns_record_destroy","ares_dns_record_destroy","ares_strerror","ares_reinit","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_local_ip4","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_class","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_key_tostr","ares_dns_record_create","ares_dns_record_query_add","ares_dns_opt_get_datatype","ares_dns_record_destroy","ares_dns_record_destroy","ares_reinit","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<40) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_8, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    int optmask = 0;
    
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set local IPv4 address (using some arbitrary IP)
    unsigned int local_ip = 0x7F000001; // 127.0.0.1
    ares_set_local_ip4(channel, fuzz_uint32_t_2);

    // Parse input data as DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_3, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec != nullptr) {
        // Get the first resource record from answer section (if exists)
        size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
        if (rr_cnt > 0) {
            const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_4);
            
            // Get class of the RR
            ares_dns_class_t rr_class = ares_dns_rr_get_class(rr);
            
            // Get keys for this RR type
            size_t key_cnt = 0;
            ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
            const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(rr_type, &key_cnt);
            
            if (keys != nullptr && key_cnt > 0) {
                // Use ares_dns_rr_key_tostr on first key
                const char *key_str = ares_dns_rr_key_tostr(keys[0]);
                
                // Try to set a string value (create a mutable copy first)
                ares_dns_rr_t *mutable_rr = nullptr;
                // Note: In real usage, we'd need to create a mutable RR from the const one
                // For fuzzing purposes, we'll demonstrate the API usage
                // We'll create a new RR instead
                
                // Create a new DNS record for manipulation
                ares_dns_record_t *new_dnsrec = nullptr;
                if (ares_dns_record_create(&new_dnsrec, fuzz_uint16_t_5, fuzz_uint16_t_6, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR) == ARES_SUCCESS) {
                    // Add a query
                    ares_dns_record_query_add(new_dnsrec, fuzz_str_7, ARES_REC_TYPE_A, ARES_CLASS_IN);
                    
                    // Get the first RR from the new record (if any in answer section)
                    // We'll add one first
                    // For simplicity, we'll just demonstrate the APIs
                    
                    // Get datatype for an option (using first key and opt=0)
                    ares_dns_opt_datatype_t datatype = ares_dns_opt_get_datatype(keys[0], fuzz_uint16_t_8);
                    
                    // Try to delete an option by ID (if this RR has options)
                    // We'll create a temporary mutable RR to demonstrate
                    // In real scenario, we'd get a mutable RR from the record
                    
                    // Clean up new record
                    ares_dns_record_destroy(new_dnsrec);
                }
            }
        }
        
        // Clean up parsed DNS record
        ares_dns_record_destroy(dnsrec);
    } else {
        // Use ares_strerror to get error string
        const char *error_str = ares_strerror((int)status);
    }

    // Reinitialize the channel
    status = ares_reinit(channel);
    
    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
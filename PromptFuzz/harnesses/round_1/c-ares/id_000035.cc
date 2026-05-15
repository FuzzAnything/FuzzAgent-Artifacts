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
//<ID> 791
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_dns_parse","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_rr_get_const","ares_dns_rr_get_type","ares_dns_rr_set_ttl","ares_dns_rr_get_opt_cnt","ares_dns_rr_get_opt","ares_dns_rr_key_tostr","ares_dns_rr_set_bin","ares_set_servers_ports_csv","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_dns_parse","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_rr_get_const","ares_dns_rr_get_type","ares_dns_rr_set_ttl","ares_dns_rr_get_opt_cnt","ares_dns_rr_get_opt","ares_dns_rr_key_tostr","ares_dns_rr_set_bin","ares_set_servers_ports_csv","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<26) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t* channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Parse DNS record from input data
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec != nullptr) {
        // Get and convert opcode
        ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
        const char* opcode_str = ares_dns_opcode_tostr(opcode);
        
        // Try to get the first RR from answer section (if exists)
        const ares_dns_rr_t* rr = ares_dns_record_rr_get_const(
            dnsrec, ARES_SECTION_ANSWER, 0);
        
        if (rr != nullptr) {
            // Get RR type
            ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
            
            // Try to set TTL (modify the RR - need non-const version)
            ares_dns_rr_t* mod_rr = const_cast<ares_dns_rr_t*>(rr);
            ares_dns_rr_set_ttl(mod_rr, fuzz_uint32_t_2); // Set TTL to 300
            
            // Check for OPT records (if RR is OPT type)
            if (rr_type == ARES_REC_TYPE_OPT) {
                size_t opt_cnt = ares_dns_rr_get_opt_cnt(
                    rr, ARES_RR_OPT_OPTIONS);
                
                if (opt_cnt > 0) {
                    const unsigned char* opt_val = nullptr;
                    size_t opt_val_len = 0;
                    
                    // Get first OPT record
                    unsigned short opt = ares_dns_rr_get_opt(
                        rr, ARES_RR_OPT_OPTIONS, fuzz_uint64_t_3, 
                        &opt_val, &opt_val_len);
                    
                    // Try to delete by ID (if supported)
                    // Note: ares_dns_rr_del_opt_byid is listed but not in exported APIs
                    // We'll use available alternative if needed
                    
                    // Get key string for debugging
                    const char* key_str = ares_dns_rr_key_tostr(ARES_RR_OPT_OPTIONS);
                    
                    // Set binary data (use first 16 bytes of input or less)
                    size_t bin_len = size > 16 ? 16 : size;
                    ares_dns_rr_set_bin(mod_rr, ARES_RR_OPT_OPTIONS, 
                                       data, bin_len);
                }
            }
        }
    }
    
    // Set servers using CSV (use dummy server)
    const char* servers_csv = "8.8.8.8";
    ares_set_servers_ports_csv(channel, fuzz_str_4);
    
    // Clean up
    if (dnsrec != nullptr) {
        ares_dns_record_destroy(dnsrec);
    }
    
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
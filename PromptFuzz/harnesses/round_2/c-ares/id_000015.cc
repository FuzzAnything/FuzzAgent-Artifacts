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
//<ID> 308
//<Prompt> []
/*<Combination>: [
*/
//<score> 56, nr_unique_branch: 18
//<Quality> {"density":16,"unique_branches":{"ares_dns_multistring_combined":[[200,7,200,19,0,0,4,1],[200,23,200,34,0,0,4,1],[207,7,207,31,0,0,4,1],[219,15,219,45,0,0,4,0],[219,15,219,45,0,0,4,1],[221,9,221,21,0,0,4,1],[222,9,222,68,0,0,4,1],[230,7,230,30,0,0,4,1]],"ares_buf_append":[[225,7,225,20,0,0,4,0]],"ares_dns_rr_get_keys":[[649,5,649,27,0,0,4,0],[652,5,652,27,0,0,4,0]],"ares_dns_rr_get_bin":[[849,8,849,59,0,0,4,0],[850,8,850,60,0,0,4,0],[850,8,850,60,0,0,4,1],[856,7,856,59,0,0,4,0],[860,9,860,21,0,0,4,1]],"ares_dns_write_rr_soa":[[367,7,367,29,0,0,4,0]],"ares_expand_string_ex":[[54,25,54,47,0,0,4,0]]},"library_calls":["ares_dns_parse","ares_dns_record_destroy","ares_dns_record_get_id","ares_dns_record_get_flags","ares_dns_record_duplicate","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_name","ares_dns_rr_get_class","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_get_bin","ares_expand_string","ares_free_string","ares_dns_record_destroy","ares_dns_record_destroy"],"critical_calls":["ares_dns_parse","ares_dns_record_get_id","ares_dns_record_get_flags","ares_dns_record_duplicate","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_name","ares_dns_rr_get_class","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_get_bin","ares_expand_string","ares_free_string","ares_dns_record_destroy","ares_dns_record_destroy"],"visited":1}
/**/


// Include c-ares headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;  // Need minimal data
    }

    // 1. Parse DNS record from input data
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, 0, &dnsrec);
    
    if (status != ARES_SUCCESS || !dnsrec) {
        // Cleanup if parsing failed
        if (dnsrec) {
            ares_dns_record_destroy(dnsrec);
        }
        return 0;
    }

    // 2. Get DNS record ID (API #1)
    unsigned short id = ares_dns_record_get_id(dnsrec);
    
    // 3. Get DNS record flags (API #7)
    unsigned short flags = ares_dns_record_get_flags(dnsrec);
    
    // 4. Duplicate DNS record (API #3)
    ares_dns_record_t *dup_rec = ares_dns_record_duplicate(dnsrec);
    
    if (dup_rec) {
        // 5. Process each RR in the answer section
        size_t rr_count = ares_dns_record_rr_cnt(dup_rec, ARES_SECTION_ANSWER);
        
        for (size_t i = 0; i < rr_count; i++) {
            const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dup_rec, ARES_SECTION_ANSWER, i);
            if (!rr) continue;
            
            // 6. Get RR name (API #6)
            const char *name = ares_dns_rr_get_name(rr);
            
            // 7. Get RR class (API #5)
            ares_dns_class_t rr_class = ares_dns_rr_get_class(rr);
            
            // 8. Try to get binary data from RR (API #4)
            size_t bin_len = 0;
            
            // Try different keys that might contain binary data
            ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
            
            // Get available keys for this RR type
            size_t key_count = 0;
            const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(rr_type, &key_count);
            
            for (size_t k = 0; k < key_count; k++) {
                const unsigned char *bin_data = ares_dns_rr_get_bin(rr, keys[k], &bin_len);
                
                if (bin_data && bin_len > 0) {
                    // 9. Use ares_expand_string (API #2)
                    unsigned char *expanded = nullptr;
                    long enclen = 0;
                    
                    // Note: ares_expand_string expects compressed DNS data
                    // We need valid DNS message data for this
                    if (size > 12) {  // DNS header is 12 bytes
                        int result = ares_expand_string(bin_data, 
                                                       data, 
                                                       (int)size, 
                                                       &expanded, 
                                                       &enclen);
                        
                        if (result == ARES_SUCCESS && expanded) {
                            ares_free_string(expanded);
                        }
                    }
                    break;  // Found binary data, no need to check other keys
                }
            }
        }
        
        // Clean up duplicated record
        ares_dns_record_destroy(dup_rec);
    }
    
    // 10. Clean up original record
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
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
//<ID> 176
//<Prompt> []
/*<Combination>: [
*/
//<score> 17.5, nr_unique_branch: 26
//<Quality> {"density":5,"unique_branches":{"ares_malloc_data":[[155,5,155,35,0,0,4,0]],"ares_free_data":[[98,7,98,37,0,0,4,0]],"ares_set_local_ip4":[[548,7,548,22,0,0,4,1]],"ares_dns_flags_arevalid":[[75,7,75,24,0,0,4,0]],"ares_dns_record_create":[[64,7,64,38,0,0,4,0]],"ares_parse_naptr_reply":[[42,7,42,19,0,0,4,1],[49,7,49,29,0,0,4,0],[49,7,49,29,0,0,4,1],[53,7,53,63,0,0,4,0],[53,7,53,63,0,0,4,1],[58,15,58,70,0,0,4,0],[58,15,58,70,0,0,4,1],[62,9,62,19,0,0,4,1],[68,9,68,51,0,0,4,0],[68,9,68,51,0,0,4,1],[69,9,69,56,0,0,4,0],[69,9,69,56,0,0,4,1],[75,9,75,27,0,0,4,1],[81,9,81,19,0,0,4,1],[94,9,94,34,0,0,4,1],[101,9,101,36,0,0,4,1],[108,9,108,35,0,0,4,1],[114,9,114,40,0,0,4,1],[122,7,122,29,0,0,4,0],[122,7,122,29,0,0,4,1],[123,9,123,19,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_parse_naptr_reply","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_get_keys","ares_dns_rr_key_datatype","ares_dns_rr_set_str","ares_dns_rcode_tostr","ares_set_local_ip4","ares_process_fds","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_free_data","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_parse_naptr_reply","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_get_keys","ares_dns_rr_key_datatype","ares_dns_rr_set_str","ares_dns_rcode_tostr","ares_set_local_ip4","ares_process_fds","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_free_data","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<51) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
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
    
    // 1. Parse NAPTR reply from input data
    struct ares_naptr_reply *naptr_out = nullptr;
    if (size > 0) {
        // Use the input data as DNS reply buffer
        ares_parse_naptr_reply(data, (int)size, &naptr_out);
    }
    
    // 2. Create a DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, 
                                                  fuzz_uint16_t_1,  // ID
                                                  fuzz_uint16_t_2, // Flags (QR=1, RD=1)
                                                  ARES_OPCODE_QUERY,
                                                  ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dnsrec != nullptr) {
        // 3. Set string values in DNS record
        // First, add a resource record to set string on
        ares_dns_rr_t *rr = nullptr;
        status = ares_dns_record_rr_add(&rr, dnsrec, 
                                        ARES_SECTION_ANSWER,
                                        fuzz_str_3,
                                        ARES_REC_TYPE_NAPTR,
                                        ARES_CLASS_IN,
                                        fuzz_uint32_t_4);  // TTL
        
        if (status == ARES_SUCCESS && rr != nullptr) {
            // Get available keys for NAPTR record type
            size_t key_cnt = 0;
            const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(ARES_REC_TYPE_NAPTR, &key_cnt);
            
            // Try to set string values on available string keys
            for (size_t i = 0; i < key_cnt; i++) {
                ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(keys[i]);
                if (datatype == ARES_DATATYPE_STR) {
                    // Try setting a test string
                    ares_dns_rr_set_str(rr, keys[i], fuzz_str_5);
                }
            }
        }
        
        // 4. Convert RCODE to string
        const char *rcode_str = ares_dns_rcode_tostr(ARES_RCODE_NOERROR);
        // Use rcode_str to avoid unused variable warning
        if (rcode_str != nullptr) {
            // Do nothing, just using the API
        }
    }
    
    // 5. Set local IPv4 address for channel
    unsigned int local_ip = 0x7F000001; // 127.0.0.1
    ares_set_local_ip4(channel, fuzz_uint32_t_6);
    
    // 6. Process file descriptors using input data
    if (size > 0) {
        // Create a file from input data
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file != nullptr) {
            // Get file descriptor
            int fd = fuzz_fileno(in_file);
            if (fd >= 0) {
                // Create fd events structure
                ares_fd_events_t events[1];
                events[0].fd = fd;
                events[0].events = 0x01; // Read event
                
                // Process the file descriptor
                status = ares_process_fds(channel, events, 1, fuzz_uint32_t_7);
                
                // Clean up file
                assert_file_closed(&in_file);;
            }
        }
    }
    
    // 7. (Missing API from list but needed for completeness)
    // Try to get class of a resource record if we have one
    if (dnsrec != nullptr) {
        size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
        if (rr_count > 0) {
            const ares_dns_rr_t *rr_const = ares_dns_record_rr_get_const(dnsrec, 
                                                                        ARES_SECTION_ANSWER, 
                                                                        fuzz_uint64_t_8);
            if (rr_const != nullptr) {
                // Note: ares_dns_rr_get_class API is listed but not in provided exports
                // We'll skip this as it's not in the exported API list
            }
        }
    }
    
    // Clean up resources
    
    // Free NAPTR reply if allocated
    if (naptr_out != nullptr) {
        ares_free_data(naptr_out);
    }
    
    // Destroy DNS record if created
    if (dnsrec != nullptr) {
        ares_dns_record_destroy(dnsrec);
    }
    
    // Destroy channel
    if (channel != nullptr) {
        ares_destroy(channel);
    }
    
    // Cleanup library
    ares_library_cleanup();
    
    return 0;
}
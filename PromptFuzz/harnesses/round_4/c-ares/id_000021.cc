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
//<ID> 324
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 16
//<Quality> {"density":11,"unique_branches":{"ares_init_by_options":[[291,7,291,31,0,0,4,0]],"ares_gethostbyname_file":[[343,7,343,22,0,0,4,1]],"init_by_defaults":[[133,7,133,43,0,0,4,1]],"ares_dns_record_rr_del":[[455,7,455,21,0,0,4,1],[455,25,455,56,0,0,4,1],[459,11,459,15,0,0,4,1],[460,5,460,29,0,0,4,0],[463,5,463,32,0,0,4,1],[466,5,466,33,0,0,4,1]],"ares_gethostbyname_file_int":[[295,7,295,22,0,0,4,1],[295,26,295,38,0,0,4,1],[295,42,295,54,0,0,4,1],[307,7,307,33,0,0,4,1],[312,7,312,29,0,0,4,0],[332,7,332,28,0,0,4,0],[332,32,332,55,0,0,4,1]]},"library_calls":["ares_library_init","ares_threadsafety","ares_init_options","ares_library_cleanup","ares_dns_parse","ares_dns_record_get_flags","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_class","ares_dns_class_tostr","ares_dns_rr_get_ttl","ares_dns_record_rr_del","ares_dns_record_destroy","ares_strerror","ares_gethostbyname_file","ares_free_data","ares_get_servers_csv","ares_free_string","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_threadsafety","ares_init_options","ares_dns_parse","ares_dns_record_get_flags","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_class","ares_dns_class_tostr","ares_dns_rr_get_ttl","ares_dns_record_rr_del","ares_dns_record_destroy","ares_gethostbyname_file","ares_free_data","ares_get_servers_csv","ares_free_string","ares_destroy","ares_library_cleanup"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Initialize the c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Check thread safety
    ares_threadsafety();
    
    // 2. Create a channel with default options
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize options to default values
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    options.timeout = 5000;
    options.tries = 4;
    optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // 3. Parse DNS record from input data
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_2, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec != nullptr) {
        // 4. Get DNS record flags
        unsigned short flags = ares_dns_record_get_flags(dnsrec);
        (void)flags; // Use flags to avoid unused variable warning
        
        // 5. Get DNS record opcode
        ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
        
        // 6. Convert opcode to string
        const char *opcode_str = ares_dns_opcode_tostr(opcode);
        if (opcode_str) {
            // Use opcode_str to avoid unused warning
            volatile const char *temp = opcode_str;
            (void)temp;
        }
        
        // 7. Get resource record count in answer section
        size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
        
        // 8. Process resource records if available
        if (rr_count > 0) {
            // Get first resource record in answer section
            const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_3);
            if (rr != nullptr) {
                // 9. Get RR class
                ares_dns_class_t rr_class = ares_dns_rr_get_class(rr);
                
                // 10. Convert class to string
                const char *class_str = ares_dns_class_tostr(rr_class);
                if (class_str) {
                    volatile const char *temp = class_str;
                    (void)temp;
                }
                
                // 11. Get RR TTL
                unsigned int ttl = ares_dns_rr_get_ttl(rr);
                (void)ttl; // Use ttl to avoid unused variable warning
                
                // 12. Delete first resource record
                // Note: We need to cast away const for deletion
                ares_dns_record_rr_del(const_cast<ares_dns_record_t*>(dnsrec), ARES_SECTION_ANSWER, fuzz_uint64_t_4);
            }
        }
        
        // 13. Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    } else {
        // Handle parse error
        int error_code = static_cast<int>(status);
        const char *error_str = ares_strerror(error_code);
        if (error_str) {
            volatile const char *temp = error_str;
            (void)temp;
        }
    }
    
    // 14. Create a temporary file with input data for gethostbyname_file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Create a temporary host file
        FILE *host_file = fopen("input_file", "wb");
        if (host_file) {
            // Write input data to host file (simulating /etc/hosts format)
            if (size > 0) {
                // Write a minimal hosts entry
                const char *host_entry = "127.0.0.1 test.local\n";
                fwrite(host_entry, 1, strlen(host_entry), host_file);
                fwrite(data, 1, (size < 1024 ? size : 1024), host_file);
            }
            assert_file_closed(&host_file);;
            
            // 15. Perform host lookup from file
            struct hostent *host = nullptr;
            int result = ares_gethostbyname_file(channel, "test.local", fuzz_int32_t_5, &host);
            
            if (result == ARES_SUCCESS && host != nullptr) {
                // 16. Free the hostent data
                ares_free_data(host);
            }
            
            // Remove the temporary file
            remove("input_file");
        }
        assert_file_closed(&in_file);;
    }
    
    // 17. Get servers in CSV format
    char *servers_csv = ares_get_servers_csv(channel);
    if (servers_csv) {
        // 18. Free the CSV string
        ares_free_string(servers_csv);
    }
    
    // 19. Destroy the channel
    ares_destroy(channel);
    
    // 20. Clean up library
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
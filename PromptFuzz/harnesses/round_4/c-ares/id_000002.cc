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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 8
//<Quality> {"density":8,"unique_branches":{" id_000002_nameinfo_callback":[[211,9,211,47,0,0,4,0],[212,11,212,37,0,0,4,0],[225,9,225,40,0,0,4,1],[255,9,255,47,0,0,4,0],[256,11,256,37,0,0,4,0]],"ares_getnameinfo_int":[[113,7,113,39,0,0,4,1],[118,7,118,38,0,0,4,0],[118,42,118,71,0,0,4,1]]},"library_calls":["ares_library_init_mem","ares_init","ares_library_cleanup","ares_dns_parse","ares_dns_record_duplicate","ares_dns_record_query_cnt","ares_dns_record_query_set_name","ares_dns_record_destroy","ares_expand_name","ares_free_string","ares_dns_record_destroy","ares_getnameinfo","ares_process_fd","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init_mem","ares_init","ares_dns_parse","ares_dns_record_duplicate","ares_dns_record_query_cnt","ares_dns_record_query_set_name","ares_dns_record_destroy","ares_expand_name","ares_free_string","ares_dns_record_destroy","ares_getnameinfo","ares_process_fd","ares_destroy","ares_library_cleanup"],"visited":2}
/**/


extern "C" {
}

// Custom memory functions for ares_library_init_mem
static void*  id_000002_custom_malloc(size_t size) { return malloc(size); }
static void  id_000002_custom_free(void* ptr) { free(ptr); }
static void*  id_000002_custom_realloc(void* ptr, size_t size) { return realloc(ptr, size); }

// Callback for ares_getnameinfo
static void  id_000002_nameinfo_callback(void* arg, int status, int timeouts,
                              const char* node, const char* service) {
    // Do nothing in fuzzing context
    (void)arg; (void)status; (void)timeouts; (void)node; (void)service;
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	

    // 1. Initialize c-ares with custom memory functions
    if (ares_library_init_mem(ARES_LIB_INIT_ALL,
                               id_000002_custom_malloc,
                               id_000002_custom_free,
                               id_000002_custom_realloc) != ARES_SUCCESS) {
        return 0;
    }

    // 2. Create a channel
    ares_channel_t* channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 3. Parse DNS response from input data
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec != nullptr) {
        // 4. Duplicate the DNS record
        ares_dns_record_t* dup_dnsrec = ares_dns_record_duplicate(dnsrec);
        
        if (dup_dnsrec != nullptr) {
            // 5. Modify a query name in the duplicated record
            size_t query_cnt = ares_dns_record_query_cnt(dup_dnsrec);
            if (query_cnt > 0) {
                // Use first 4 bytes of data to create a small name string
                char new_name[5] = {0};
                snprintf(new_name, sizeof(new_name), "%c%c%c%c",
                         data[0] % 26 + 'a',
                         data[1] % 26 + 'a',
                         data[2] % 26 + 'a',
                         data[3] % 26 + 'a');
                
                ares_dns_record_query_set_name(dup_dnsrec, 0, new_name);
            }
            ares_dns_record_destroy(dup_dnsrec);
        }

        // 6. Try to expand a name from the DNS response
        if (size > 12) {  // Minimum size for DNS header
            char* expanded_name = nullptr;
            long enclen = 0;
            
            // Use ares_expand_name on the original data
            // Start from offset 12 (after DNS header) if possible
            size_t offset = (size > 20) ? 12 : 0;
            int result = ares_expand_name(data + offset, data, size,
                                         &expanded_name, &enclen);
            
            if (result == ARES_SUCCESS && expanded_name != nullptr) {
                ares_free_string(expanded_name);
            }
        }

        ares_dns_record_destroy(dnsrec);
    }

    // 7. Create sockaddr for ares_getnameinfo
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    
    // Use first 4 bytes of input for IP address
    if (size >= 4) {
        memcpy(&sa.sin_addr, data, 4);
    } else {
        sa.sin_addr.s_addr = htonl(0x7f000001);  // 127.0.0.1
    }
    
    // 8. Call ares_getnameinfo
    ares_getnameinfo(channel, (struct sockaddr*)&sa, sizeof(sa),
                     ARES_NI_LOOKUPHOST | ARES_NI_LOOKUPSERVICE,
                      id_000002_nameinfo_callback, nullptr);

    // 9. Process any pending operations
    ares_process_fd(channel, fuzz_int32_t_2, fuzz_int32_t_3);

    // 10. Cleanup
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}
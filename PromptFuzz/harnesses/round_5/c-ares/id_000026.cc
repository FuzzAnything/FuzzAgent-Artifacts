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
//<ID> 369
//<Prompt> []
/*<Combination>: [
*/
//<score> 70, nr_unique_branch: 13
//<Quality> {"density":10,"unique_branches":{"ares_dns_rr_get_u8":[[801,7,801,17,0,0,4,0]],"default_abind":[[418,7,418,35,0,0,4,1]],"ares_socket_configure":[[271,28,271,46,0,0,4,0],[288,7,288,14,0,0,4,0],[288,18,288,51,0,0,4,0],[290,9,290,15,0,0,4,1],[293,9,294,67,0,0,4,0],[293,9,294,67,0,0,4,1]],"ares_open_connection":[[399,7,399,29,0,0,4,0]],"ares_dns_rec_type_tostr":[[220,5,220,28,0,0,4,0]],"ares_search_dnsrec":[[479,7,479,22,0,0,4,1],[479,26,479,40,0,0,4,1],[479,44,479,60,0,0,4,1]]},"library_calls":["ares_library_init_mem","ares_init","ares_library_cleanup","ares_set_local_ip4","ares_dns_parse","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_duplicate","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_u8","ares_search_dnsrec","ares_dns_record_destroy","ares_dns_record_destroy","ares_timeout","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init_mem","ares_init","ares_set_local_ip4","ares_dns_parse","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_duplicate","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_u8","ares_search_dnsrec","ares_dns_record_destroy","ares_dns_record_destroy","ares_timeout","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Custom memory functions
static void*  id_000026_custom_malloc(size_t size) {
    return malloc(size);
}

static void  id_000026_custom_free(void* ptr) {
    free(ptr);
}

static void*  id_000026_custom_realloc(void* ptr, size_t size) {
    return realloc(ptr, size);
}

// Callback for ares_search_dnsrec
static void  id_000026_search_callback(void* arg, ares_status_t status, 
                           size_t timeouts, const ares_dns_record_t* dnsrec) {
    // Process callback if needed
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize c-ares with custom memory functions
    int init_result = ares_library_init_mem(ARES_LIB_INIT_ALL,
                                            id_000026_custom_malloc,
                                            id_000026_custom_free,
                                            id_000026_custom_realloc);
    if (init_result != ARES_SUCCESS) {
        return 0;
    }

    // Create channel
    ares_channel_t* channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set local IPv4 address (using first 4 bytes of input if available)
    if (size >= 4) {
        unsigned int local_ip = 0;
        memcpy(&local_ip, data, 4);
        ares_set_local_ip4(channel, local_ip);
    }

    // Parse input as DNS record
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t parse_status = ares_dns_parse(data, size, 0, &dnsrec);
    
    if (parse_status == ARES_SUCCESS && dnsrec != nullptr) {
        // Get opcode from DNS record and convert to string
        ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
        const char* opcode_str = ares_dns_opcode_tostr(opcode);
        (void)opcode_str; // Use to avoid unused variable warning
        
        // Duplicate the DNS record
        ares_dns_record_t* duplicated = ares_dns_record_duplicate(dnsrec);
        
        if (duplicated != nullptr) {
            // Get first resource record from answer section if available
            size_t rr_count = ares_dns_record_rr_cnt(duplicated, ARES_SECTION_ANSWER);
            if (rr_count > 0) {
                const ares_dns_rr_t* rr = ares_dns_record_rr_get_const(duplicated, 
                                                                      ARES_SECTION_ANSWER, 0);
                if (rr != nullptr) {
                    // Try to get a u8 value from the RR
                    unsigned char u8_val = ares_dns_rr_get_u8(rr, ARES_RR_OPT_VERSION);
                    (void)u8_val; // Use to avoid unused variable warning
                }
            }
            
            // Perform search with duplicated record
            ares_search_dnsrec(channel, duplicated,  id_000026_search_callback, nullptr);
            
            // Clean up duplicated record
            ares_dns_record_destroy(duplicated);
        }
        
        // Clean up original record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Get timeout information
    struct timeval maxtv, tv;
    maxtv.tv_sec = 5;
    maxtv.tv_usec = 0;
    struct timeval* timeout = ares_timeout(channel, &maxtv, &tv);
    (void)timeout; // Use to avoid unused variable warning
    
    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
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
//<ID> 566
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 15
//<Quality> {"density":7,"unique_branches":{"ares_parse_uri_reply":[[54,7,54,63,0,0,4,1],[59,15,59,70,0,0,4,0],[59,15,59,70,0,0,4,1],[63,9,63,19,0,0,4,1],[69,9,69,51,0,0,4,0],[69,9,69,51,0,0,4,1],[70,9,70,54,0,0,4,0],[103,7,103,29,0,0,4,1]],"ares_dns_rr_get_opt_byid":[[1063,7,1063,10,0,0,4,0],[1066,7,1066,14,0,0,4,0],[1070,7,1070,57,0,0,4,1],[1075,7,1075,19,0,0,4,0],[1075,7,1075,19,0,0,4,1],[1075,23,1075,36,0,0,4,0]],"ares_dns_class_tostr":[[238,11,238,17,0,0,4,0]]},"library_calls":["ares_library_init_mem","ares_init_options","ares_library_cleanup","ares_set_query_enqueue_cb","ares_parse_uri_reply","ares_search","ares_dns_parse","ares_dns_record_rr_get_const","ares_dns_rr_get_opt_byid","ares_dns_rr_get_class","ares_dns_class_tostr","ares_dns_record_destroy","ares_queue_wait_empty","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init_mem","ares_init_options","ares_set_query_enqueue_cb","ares_parse_uri_reply","ares_search","ares_dns_parse","ares_dns_record_rr_get_const","ares_dns_rr_get_opt_byid","ares_dns_rr_get_class","ares_dns_class_tostr","ares_dns_record_destroy","ares_queue_wait_empty","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


// Include necessary c-ares headers

// Custom memory allocator functions
void*  id_000034_custom_malloc(size_t size) {
    return malloc(size);
}

void  id_000034_custom_free(void* ptr) {
    free(ptr);
}

void*  id_000034_custom_realloc(void* ptr, size_t size) {
    return realloc(ptr, size);
}

// Query enqueue callback
void  id_000034_query_enqueue_callback(void* user_data) {
    // This callback is called when a query is enqueued
    // We can use it to track query status if needed
    (void)user_data;  // Unused in this example
}

// DNS callback function
void  id_000034_dns_callback(void* arg, int status, int timeouts, 
                  const unsigned char* abuf, int alen) {
    (void)arg; (void)status; (void)timeouts; (void)abuf; (void)alen;
    // Process DNS response if needed
}

// Fuzzer entry point
extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    if (size < 2) {
        return 0;  // Not enough data
    }

    // Initialize c-ares library with custom memory functions
    int init_result = ares_library_init_mem(ARES_LIB_INIT_ALL, 
                                             id_000034_custom_malloc, 
                                             id_000034_custom_free, 
                                             id_000034_custom_realloc);
    if (init_result != ARES_SUCCESS) {
        return 0;
    }

    // Initialize ares channel
    ares_channel_t* channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    memset(&options, 0, sizeof(options));
    
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set query enqueue callback
    ares_set_query_enqueue_cb(channel,  id_000034_query_enqueue_callback, nullptr);

    // Parse URI reply from input data
    struct ares_uri_reply* uri_reply = nullptr;
    int parse_result = ares_parse_uri_reply(data, static_cast<int>(size), &uri_reply);
    
    // If we successfully parsed a URI reply, use the domain from it
    const char* domain_to_search = "example.com";  // Default domain
    if (parse_result == ARES_SUCCESS && uri_reply != nullptr) {
        // Use the URI from the first reply as domain (if available)
        if (uri_reply->uri != nullptr) {
            // Simple heuristic: extract domain from URI
            // In real usage, you'd want proper URI parsing
            domain_to_search = "test.domain";  // Simplified for fuzzing
        }
        
        // Free URI reply chain
        struct ares_uri_reply* current = uri_reply;
        while (current != nullptr) {
            struct ares_uri_reply* next = current->next;
            if (current->uri) {
                free(current->uri);
            }
            free(current);
            current = next;
        }
    }

    // Perform DNS search
    ares_search(channel, fuzz_str_2, C_IN, T_A,  id_000034_dns_callback, nullptr);

    // Create a dummy DNS RR to test ares_dns_rr_get_opt_byid
    // We need to parse the input as a DNS record first
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, fuzz_uint32_t_3, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec != nullptr) {
        // Get the first resource record from answer section
        const ares_dns_rr_t* rr = ares_dns_record_rr_get_const(dnsrec, 
                                                               ARES_SECTION_ANSWER, 
                                                               fuzz_uint64_t_4);
        if (rr != nullptr) {
            // Try to get OPT record data if available
            const unsigned char* opt_val = nullptr;
            size_t opt_len = 0;
            
            // Check if this RR has any OPT data
            ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS, 
                                    fuzz_uint16_t_5, &opt_val, &opt_len);
            
            // Get DNS class and convert to string
            ares_dns_class_t dns_class = ares_dns_rr_get_class(rr);
            const char* class_str = ares_dns_class_tostr(dns_class);
            (void)class_str;  // Use to avoid unused variable warning
        }
        
        // Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // Wait for queue to be empty (with timeout)
    ares_queue_wait_empty(channel, fuzz_int32_t_6);  // 100ms timeout

    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}
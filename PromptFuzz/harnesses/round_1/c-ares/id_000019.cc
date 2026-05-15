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
//<ID> 538
//<Prompt> []
/*<Combination>: [
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["ares_library_initialized","ares_library_init","ares_init_options","ares_library_cleanup","ares_set_query_enqueue_cb","ares_parse_caa_reply","ares_parse_naptr_reply","ares_parse_ptr_reply","ares_inet_ntop","ares_dns_rcode_tostr","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_set_name","ares_dns_record_query_get","ares_dns_record_rr_get","ares_dns_rr_get_opt_byid","ares_send","ares_process_fd","ares_free_data","ares_free_data","ares_free_hostent","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_initialized","ares_library_init","ares_init_options","ares_set_query_enqueue_cb","ares_parse_caa_reply","ares_parse_naptr_reply","ares_parse_ptr_reply","ares_inet_ntop","ares_dns_rcode_tostr","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_set_name","ares_dns_record_query_get","ares_dns_record_rr_get","ares_dns_rr_get_opt_byid","ares_send","ares_process_fd","ares_free_data","ares_free_data","ares_free_hostent","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" {
}

// Event: Parse DNS response data containing CAA, NAPTR, and PTR records,
// create a DNS query record, set up a channel, and send a DNS query

// Callback function for query enqueue (required by ares_set_query_enqueue_cb)
static void  id_000019_query_enqueue_callback(void* user_data) {
    // This is a dummy callback since we just need to call the API
    (void)user_data;
}

// Callback function for ares_send (required)
static void  id_000019_send_callback(void* arg, int status, int timeouts, 
                         const unsigned char* abuf, int alen) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
    // Do nothing - this is just to satisfy API requirements
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<38) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_8, fdp);
	//fuzzer shim end}




    // 1. Initialize c-ares library
    if (!ares_library_initialized()) {
        if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
            return 0;
        }
    }

    // 2. Create a channel
    ares_channel_t* channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize options
    memset(&options, 0, sizeof(options));
    
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 3. Set query enqueue callback
    ares_set_query_enqueue_cb(channel,  id_000019_query_enqueue_callback, nullptr);

    // 4. Parse input as CAA reply
    struct ares_caa_reply* caa_reply = nullptr;
    if (size > 0) {
        ares_parse_caa_reply(data, static_cast<int>(size), &caa_reply);
    }

    // 5. Parse input as NAPTR reply
    struct ares_naptr_reply* naptr_reply = nullptr;
    if (size > 0) {
        ares_parse_naptr_reply(data, static_cast<int>(size), &naptr_reply);
    }

    // 6. Parse input as PTR reply (need dummy address)
    struct hostent* host = nullptr;
    if (size > 0) {
        struct in_addr dummy_addr;
        dummy_addr.s_addr = htonl(0x08080808); // 8.8.8.8
        ares_parse_ptr_reply(data, static_cast<int>(size), 
                            &dummy_addr, sizeof(dummy_addr), 
                            fuzz_int32_t_2, &host);
    }

    // 7. Test ares_inet_ntop
    char addr_str[INET6_ADDRSTRLEN];
    if (size >= 4) {
        struct in_addr in_addr;
        memcpy(&in_addr.s_addr, data, 4);
        ares_inet_ntop(AF_INET, &in_addr, addr_str, sizeof(addr_str));
    }

    // 8. Test ares_dns_rcode_tostr
    const char* rcode_str = ares_dns_rcode_tostr(ARES_RCODE_NOERROR);

    // 9. Create a DNS record
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, 
                                                 fuzz_uint16_t_3,  // id
                                                 fuzz_uint16_t_4, // flags
                                                 ARES_OPCODE_QUERY,
                                                 ARES_RCODE_NOERROR);

    if (status == ARES_SUCCESS && dnsrec) {
        // 10. Set query name using data from input
        if (size > 0 && dnsrec) {
            // Create a null-terminated string from input data
            size_t name_len = size > 100 ? 100 : size;
            char* domain_name = static_cast<char*>(malloc(name_len + 1));
            if (domain_name) {
                memcpy(domain_name, data, name_len);
                domain_name[name_len] = '\0';
                
                // Add a query to the record
                ares_dns_record_query_add(dnsrec, domain_name, 
                                         ARES_REC_TYPE_A, ARES_CLASS_IN);
                
                // Set the query name
                ares_dns_record_query_set_name(dnsrec, fuzz_uint64_t_5, domain_name);
                
                free(domain_name);
            }
        }

        // 11. Get query information back
        const char* retrieved_name = nullptr;
        ares_dns_rec_type_t qtype;
        ares_dns_class_t qclass;
        
        ares_dns_record_query_get(dnsrec, fuzz_uint64_t_6, &retrieved_name, &qtype, &qclass);

        // 12. Try to get OPT record (will likely fail since we didn't add one)
        const unsigned char* opt_val = nullptr;
        size_t opt_len = 0;
        ares_dns_rr_t* rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ADDITIONAL, fuzz_uint64_t_7);
        if (rr) {
            ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS, fuzz_uint16_t_8, &opt_val, &opt_len);
        }
    }

    // 13. Send a DNS query using the input data
    if (size > 0 && channel) {
        ares_send(channel, data, static_cast<int>(size), 
                  id_000019_send_callback, nullptr);
        
        // Process pending operations
        ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    }

    // 14. Cleanup

    // Free CAA reply
    if (caa_reply) {
        ares_free_data(caa_reply);
    }

    // Free NAPTR reply (note: must free linked list)
    while (naptr_reply) {
        struct ares_naptr_reply* next = naptr_reply->next;
        ares_free_data(naptr_reply);
        naptr_reply = next;
    }

    // Free PTR reply hostent
    if (host) {
        ares_free_hostent(host);
    }

    // Destroy DNS record
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }

    // Destroy channel
    if (channel) {
        ares_destroy(channel);
    }

    // Cleanup library
    ares_library_cleanup();

    return 0;
}
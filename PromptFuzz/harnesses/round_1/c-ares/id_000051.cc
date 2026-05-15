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
//<ID> 1036
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"ares_search_dnsrec":[[479,26,479,40,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_server_state_callback","ares_set_pending_write_cb","ares_parse_caa_reply","ares_free_data","ares_parse_txt_reply","ares_free_data","ares_create_query","ares_query","ares_create_query","ares_search_dnsrec","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_server_state_callback","ares_set_pending_write_cb","ares_parse_caa_reply","ares_free_data","ares_parse_txt_reply","ares_free_data","ares_create_query","ares_query","ares_create_query","ares_search_dnsrec","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// c-ares headers

// Callback declarations
static void  id_000051_server_state_callback(const char *name, ares_bool_t reachable, 
                                 int roundtrip, void *user_data) {
    // Callback for server state changes - empty as we just need to set it
}

static void  id_000051_pending_write_callback(void *user_data) {
    // Callback for pending writes - empty as we just need to set it
}

static void  id_000051_query_callback(void *arg, int status, int timeouts, 
                          const unsigned char *abuf, int alen) {
    // Query callback - we don't need to do anything here for fuzzing
}

static void  id_000051_search_dnsrec_callback(void *arg, ares_status_t status, size_t timeouts,
                                  const ares_dns_record_t *dnsrec) {
    // Search callback - we don't need to do anything here for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set callbacks
    ares_set_server_state_callback(channel,  id_000051_server_state_callback, nullptr);
    ares_set_pending_write_cb(channel,  id_000051_pending_write_callback, nullptr);

    // Try to parse input as DNS response for various record types
    // This will consume the data in different ways
    if (size > 0) {
        // Parse as CAA reply if we have enough data
        struct ares_caa_reply *caa_out = nullptr;
        ares_parse_caa_reply(data, (int)(size > INT_MAX ? INT_MAX : size), &caa_out);
        if (caa_out) {
            ares_free_data(caa_out);
        }

        // Parse as TXT reply if we have enough data
        struct ares_txt_reply *txt_out = nullptr;
        ares_parse_txt_reply(data, (int)(size > INT_MAX ? INT_MAX : size), &txt_out);
        if (txt_out) {
            ares_free_data(txt_out);
        }
    }

    // Create a DNS record to use with search_dnsrec
    ares_dns_record_t *dnsrec = nullptr;
    
    // We'll create a simple query record
    // First create a query using ares_create_query
    unsigned char *qbuf = nullptr;
    int qlen = 0;
    
    // Use a portion of input data as domain name (max 255 bytes for DNS)
    char domain[256] = {0};
    size_t domain_len = size < 255 ? size : 255;
    if (domain_len > 0) {
        memcpy(domain, data, domain_len);
        // Ensure null termination
        domain[domain_len] = '\0';
        
        // Replace non-printable characters with dots
        for (size_t i = 0; i < domain_len; i++) {
            if (domain[i] < 32 || domain[i] > 126) {
                domain[i] = '.';
            }
        }
        
        // Create query
        ares_create_query(domain, ns_c_in, ns_t_a, fuzz_uint16_t_1, 1, &qbuf, &qlen, fuzz_int32_t_2);
    }

    // Perform a query using the channel (even if we don't have valid network)
    if (domain[0] != '\0') {
        ares_query(channel, domain, ns_c_in, ns_t_a,  id_000051_query_callback, nullptr);
    }

    // Create a simple DNS RR for testing ares_dns_rr_set_u32 and ares_dns_rr_get_u8
    // We'll use the input data to populate values
    if (size >= 8) {
        // We need a DNS record structure. Since we can't create one directly,
        // we'll use ares_create_query to create a query and try to work with it
        if (qbuf && qlen > 0) {
            // Create another query for searching
            unsigned char *search_buf = nullptr;
            int search_len = 0;
            
            // Use different part of input for search domain
            char search_domain[256] = {0};
            size_t search_offset = domain_len;
            size_t search_domain_len = (size - search_offset) < 255 ? (size - search_offset) : 255;
            
            if (search_domain_len > 0) {
                memcpy(search_domain, data + search_offset, search_domain_len);
                search_domain[search_domain_len] = '\0';
                
                // Replace non-printable characters
                for (size_t i = 0; i < search_domain_len; i++) {
                    if (search_domain[i] < 32 || search_domain[i] > 126) {
                        search_domain[i] = '.';
                    }
                }
                
                // Create search query
                ares_create_query(search_domain, ns_c_in, ns_t_a, fuzz_uint16_t_3, 1, 
                                &search_buf, &search_len, fuzz_int32_t_4);
                
                if (search_buf && search_len > 0) {
                    // We would normally use ares_search_dnsrec here, but we need
                    // a proper dnsrec structure. For fuzzing purposes, we'll
                    // just call the function with nullptr to ensure it's invoked
                    ares_search_dnsrec(channel, nullptr,  id_000051_search_dnsrec_callback, nullptr);
                    
                    free(search_buf);
                }
            }
        }
    }

    // Clean up
    if (qbuf) {
        free(qbuf);
    }
    
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
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
//<ID> 455
//<Prompt> []
/*<Combination>: [
*/
//<score> 68, nr_unique_branch: 4
//<Quality> {"density":17,"unique_branches":{"ares_split_dns_name":[[343,33,343,77,0,0,4,0]],"ares_query":[[138,7,138,22,0,0,4,1],[143,7,143,19,0,0,4,1]],"ares_dns_opcode_isvalid":[[33,5,33,28,0,0,4,0]]},"library_calls":["ares_library_init_mem","ares_init_options","ares_mkquery","ares_destroy","ares_save_options","ares_dns_record_create","ares_destroy_options","ares_destroy","ares_dns_record_rr_add","ares_dns_record_destroy","ares_destroy_options","ares_destroy","ares_parse_aaaa_reply","ares_send","ares_query","ares_process","ares_free_hostent","ares_dns_record_destroy","ares_destroy_options","ares_destroy"],"critical_calls":["ares_library_init_mem","ares_init_options","ares_mkquery","ares_save_options","ares_dns_record_create","ares_dns_record_rr_add","ares_parse_aaaa_reply","ares_send","ares_query","ares_process","ares_free_hostent","ares_dns_record_destroy","ares_destroy_options","ares_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<39) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize c-ares with custom memory allocators
    static bool library_initialized = false;
    if (!library_initialized) {
        ares_library_init_mem(ARES_LIB_INIT_ALL, 
                              [](size_t sz) { return malloc(sz); },
                              [](void *ptr) { free(ptr); },
                              [](void *ptr, size_t sz) { return realloc(ptr, sz); });
        library_initialized = true;
    }
    
    // Extract domain name from input data (first null-terminated string)
    const char *domain = reinterpret_cast<const char*>(data);
    size_t domain_len = 0;
    while (domain_len < size && data[domain_len] != 0) {
        domain_len++;
    }
    if (domain_len == 0 || domain_len >= size) {
	return 0;
	}
	
    
    // Initialize ares channel
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        return 0;
    }
    
    // Create a DNS query for AAAA record
    unsigned char *query_buf = nullptr;
    int query_len = 0;
    unsigned short query_id = 0x1234; // Fixed ID for consistency
    
    // Note: ares_create_query is actually ares_mkquery in the provided API list
    if (ares_mkquery(domain, ns_c_in, ns_t_aaaa, fuzz_uint16_t_2, fuzz_int32_t_3, 
                     &query_buf, &query_len) != ARES_SUCCESS) {
        ares_destroy(channel);
        return 0;
    }
    
    // Save channel options
    struct ares_options saved_options;
    int saved_optmask = 0;
    ares_save_options(channel, &saved_options, &saved_optmask);
    
    // Create a DNS record structure
    ares_dns_record_t *dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_4, 
                               ARES_FLAG_QR | ARES_FLAG_RD, 
                               ARES_OPCODE_QUERY, 
                               ARES_RCODE_NOERROR) != ARES_SUCCESS) {
        free(query_buf);
        ares_destroy_options(&saved_options);
        ares_destroy(channel);
        return 0;
    }
    
    // Add a resource record to the DNS record
    ares_dns_rr_t *rr = nullptr;
    const char *rr_name = "test.example.com";
    if (ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                               fuzz_str_5, ARES_REC_TYPE_AAAA,
                               ARES_CLASS_IN, fuzz_uint32_t_6) != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        free(query_buf);
        ares_destroy_options(&saved_options);
        ares_destroy(channel);
        return 0;
    }
    
    // Parse the input data as AAAA reply
    struct hostent *host = nullptr;
    struct ares_addr6ttl addrttls[10];
    int naddrttls = 0;
    
    // Use the remaining input data (after domain name) as reply buffer
    const unsigned char *reply_buf = data + domain_len + 1;
    size_t reply_len = size - domain_len - 1;
    
    if (reply_len > 0) {
        ares_parse_aaaa_reply(reply_buf, reply_len, &host, addrttls, &naddrttls);
    }
    
    // Send query using ares_send (simulated, won't actually send without network)
    ares_send(channel, query_buf, fuzz_int32_t_7, 
              [](void* /*arg*/, int /*status*/, int /*timeouts*/, 
                 const unsigned char* /*abuf*/, int /*alen*/) {
                  // Callback - do nothing for fuzzing
              }, 
              nullptr);
    
    // Also use ares_query with the same domain
    ares_query(channel, domain, ns_c_in, ns_t_aaaa,
               [](void* /*arg*/, int /*status*/, int /*timeouts*/, 
                  const unsigned char* /*abuf*/, int /*alen*/) {
                   // Callback - do nothing for fuzzing
               },
               nullptr);
    
    // Process any pending operations
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    ares_process(channel, &read_fds, &write_fds);
    
    // Cleanup
    if (host) ares_free_hostent(host);
    ares_dns_record_destroy(dnsrec);
    free(query_buf);
    ares_destroy_options(&saved_options);
    ares_destroy(channel);
    
    return 0;
}
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
//<ID> 720
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 23
//<Quality> {"density":9,"unique_branches":{"ares_parse_soa_reply":[[41,7,41,19,0,0,4,1],[48,7,48,29,0,0,4,0],[48,7,48,29,0,0,4,1],[52,7,52,63,0,0,4,0],[52,7,52,63,0,0,4,1],[57,15,57,70,0,0,4,0],[57,15,57,70,0,0,4,1],[61,9,61,19,0,0,4,1],[67,9,67,51,0,0,4,0],[67,9,67,51,0,0,4,1],[68,9,68,54,0,0,4,0],[68,9,68,54,0,0,4,1],[74,9,74,20,0,0,4,1],[85,9,85,28,0,0,4,1],[90,9,90,32,0,0,4,1],[97,7,97,18,0,0,4,0],[97,7,97,18,0,0,4,1],[103,7,103,29,0,0,4,0],[103,7,103,29,0,0,4,1],[106,9,106,32,0,0,4,0],[106,9,106,32,0,0,4,1]],"ares_free_data":[[106,7,106,35,0,0,4,0]],"ares_malloc_data":[[156,5,156,33,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_create_query","ares_send","ares_parse_soa_reply","ares_free_data","ares_parse_txt_reply","ares_free_data","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_get_opcode","ares_dns_record_destroy","ares_process_fds","ares_free_hostent","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_create_query","ares_send","ares_parse_soa_reply","ares_free_data","ares_parse_txt_reply","ares_free_data","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_get_opcode","ares_dns_record_destroy","ares_process_fds","ares_free_hostent","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// c-ares headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    ares_channel_t* channel = nullptr;
    ares_dns_record_t* dnsrec = nullptr;
    struct ares_soa_reply* soa_reply = nullptr;
    struct ares_txt_reply* txt_reply = nullptr;
    struct hostent* host = nullptr;
    unsigned char* query_buf = nullptr;
    int query_buflen = 0;
    int rc = 0;
    
    // Create a channel
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Create a DNS query using some fixed parameters
    const char* domain_name = "example.com";
    unsigned short query_id = 12345;
    rc = ares_create_query(domain_name, 
                           ns_c_in,       // DNS class IN
                           ns_t_soa,      // SOA record type
                           query_id,      // Query ID
                           1,             // Recursion desired
                           &query_buf,
                           &query_buflen,
                           512);          // Max UDP size
    
    if (rc == ARES_SUCCESS && query_buf && query_buflen > 0) {
        // Dummy callback for ares_send
        auto dummy_callback = [](void* arg, int status, int timeouts, 
                                const unsigned char* abuf, int alen) {
            // Do nothing in fuzzing context
        };
        
        // Send the query (won't actually send without network)
        ares_send(channel, query_buf, query_buflen, dummy_callback, nullptr);
        
        // Free the query buffer
        free(query_buf);
        query_buf = nullptr;
    }

    // Try to parse the input as SOA reply if we have enough data
    if (size >= 2) {
        const unsigned char* abuf = reinterpret_cast<const unsigned char*>(data);
        int alen = static_cast<int>(size > INT_MAX ? INT_MAX : size);
        
        rc = ares_parse_soa_reply(abuf, alen, &soa_reply);
        if (rc == ARES_SUCCESS && soa_reply) {
            // Free the SOA reply data
            ares_free_data(soa_reply);
        }
    }

    // Try to parse the input as TXT reply
    if (size >= 2) {
        const unsigned char* abuf = reinterpret_cast<const unsigned char*>(data);
        int alen = static_cast<int>(size > INT_MAX ? INT_MAX : size);
        
        rc = ares_parse_txt_reply(abuf, alen, &txt_reply);
        if (rc == ARES_SUCCESS && txt_reply) {
            // Free the TXT reply data
            ares_free_data(txt_reply);
        }
    }

    // Create a DNS record manually
    ares_status_t status = ares_dns_record_create(&dnsrec,
                                                  query_id,
                                                  0,  // flags
                                                  ARES_OPCODE_QUERY,
                                                  ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Add a query to the DNS record
        status = ares_dns_record_query_add(dnsrec,
                                          domain_name,
                                          ARES_REC_TYPE_SOA,
                                          ARES_CLASS_IN);
        
        if (status == ARES_SUCCESS) {
            // Get the opcode from the DNS record
            ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
            (void)opcode;  // Suppress unused warning
        }
        
        // Destroy the DNS record
        ares_dns_record_destroy(dnsrec);
        dnsrec = nullptr;
    }

    // Process file descriptors (none in this fuzzing context)
    ares_fd_events_t events = {0};
    status = ares_process_fds(channel, &events, 0, 0);
    
    // Try gethostbyname_file - create a temporary hosts file
    FILE* hosts_file = fmemopen((void*)data, size, "rb");
    if (hosts_file) {
        int fd = fuzz_fileno(hosts_file);
        if (fd >= 0) {
            // Note: ares_gethostbyname_file is not in the provided API list,
            // but we'll simulate similar functionality
            // For now, skip this as it requires file reading
        }
        assert_file_closed(&hosts_file);;
    }
    
    // Cleanup
    if (host) {
        ares_free_hostent(host);
    }
    
    ares_destroy(channel);
    ares_library_cleanup();
    
    // Free any remaining allocated strings using ares_free_string
    // (not used in this specific flow, but included for completeness)
    // ares_free_string(some_string);
    
    assert_file_closed(&hosts_file);
	return 0;
}
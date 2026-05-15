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
//<ID> 446
//<Prompt> []
/*<Combination>: [
*/
//<score> 13.5, nr_unique_branch: 10
//<Quality> {"density":9,"unique_branches":{"ares_check_cleanup_conns":[[106,11,106,48,0,0,4,1],[111,11,111,49,0,0,4,0],[119,11,119,44,0,0,4,1],[124,11,124,46,0,0,4,0],[124,50,124,78,0,0,4,1],[129,11,129,22,0,0,4,1]],"ares_cancel":[[42,7,42,47,0,0,4,0],[55,9,55,37,0,0,4,1],[61,12,61,24,0,0,4,0],[61,12,61,24,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_parse_soa_reply","ares_free_data","ares_dns_rec_type_fromstr","ares_dns_rec_type_fromstr","ares_set_socket_functions","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_record_get_rcode","ares_dns_rcode_tostr","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_record_get_rcode","ares_dns_rcode_tostr","ares_query_dnsrec","ares_cancel","ares_process","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_parse_soa_reply","ares_free_data","ares_dns_rec_type_fromstr","ares_dns_rec_type_fromstr","ares_set_socket_functions","ares_dns_rcode_tostr","ares_dns_record_get_rcode","ares_dns_record_rr_get","ares_dns_record_rr_cnt","ares_dns_rcode_tostr","ares_dns_record_get_rcode","ares_dns_record_rr_get","ares_dns_record_rr_cnt","ares_query_dnsrec","ares_cancel","ares_process","ares_destroy","ares_library_cleanup"],"visited":1}
/*I'll create a fuzzer that simulates DNS query processing and response parsing using the specified c-ares APIs. This fuzzer will:

1. Parse SOA record responses from the input data
2. Create DNS queries for SOA records
3. Process those queries with custom socket functions
4. Handle the results appropriately

*/


// C-ares headers

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<10) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < sizeof(ares_dns_rec_type_t) + 1) {
        return 0;
    }

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

    // Create a FILE* to read from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Extract a domain name from the input data
    char domain_name[256] = {0};
    size_t name_len = fread(domain_name, 1, sizeof(domain_name) - 1, in_file);
    if (name_len > 0) {
        domain_name[name_len] = '\0';
        
        // Replace any non-printable characters with dots for domain safety
        for (size_t i = 0; i < name_len; i++) {
            if (domain_name[i] < 32 || domain_name[i] > 126) {
                domain_name[i] = '.';
            }
        }
    } else {
        strcpy(domain_name, "example.com");
    }

    // Parse the input data as SOA reply
    struct ares_soa_reply *soa_reply = nullptr;
    int parse_result = ares_parse_soa_reply(data, size, &soa_reply);
    
    if (parse_result == ARES_SUCCESS && soa_reply) {
        // Free the parsed SOA reply data
        ares_free_data(soa_reply);
    }

    // Convert string to DNS record type
    ares_dns_rec_type_t record_type;
    ares_bool_t type_converted = ares_dns_rec_type_fromstr(&record_type, fuzz_str_1);
    
    if (!type_converted) {
        // Try converting from the input data instead
        char type_str[32] = {0};
        size_t type_str_len = fread(type_str, 1, sizeof(type_str) - 1, in_file);
        if (type_str_len > 0) {
            ares_dns_rec_type_fromstr(&record_type, fuzz_str_2);  // Fallback to A record
        }
    }

    // Set up custom socket functions structure
    struct ares_socket_functions socket_funcs = {
        // asocket
        [](int domain, int type, int protocol, void *user_data) -> ares_socket_t {
            // Create a simple socket, but we won't actually use it for network ops
            return socket(domain, type, protocol);
        },
        // aclose
        [](ares_socket_t socket_fd, void *user_data) -> int {
            return close(socket_fd);
        },
        // aconnect
        [](ares_socket_t socket_fd, const struct sockaddr *addr, ares_socklen_t addrlen, void *user_data) -> int {
            // Simulate connection (won't actually connect in fuzzing)
            return 0;
        },
        // arecvfrom
        [](ares_socket_t socket_fd, void *buf, size_t buf_len, int flags, struct sockaddr *from, ares_socklen_t *fromlen, void *user_data) -> ares_ssize_t {
            // Return 0 to simulate no data received
            return 0;
        },
        // asendv
        [](ares_socket_t socket_fd, const struct iovec *vec, int len, void *user_data) -> ares_ssize_t {
            // Simulate sending data
            size_t total = 0;
            for (int i = 0; i < len; i++) {
                total += vec[i].iov_len;
            }
            return total;
        }
    };

    // Set the custom socket functions
    ares_set_socket_functions(channel, &socket_funcs, nullptr);

    // Create a simple callback function for DNS queries
    auto query_callback = [](void *arg, ares_status_t status, size_t timeouts, const ares_dns_record_t *dnsrec) {
        if (status == ARES_SUCCESS && dnsrec) {
            // Try to get a resource record from the DNS record
            if (ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER) > 0) {
                // Get the first RR from the answer section
                ares_dns_rr_t *rr = ares_dns_record_rr_get(const_cast<ares_dns_record_t*>(dnsrec), 
                                                          ARES_SECTION_ANSWER, 0);
                if (rr) {
                    // We successfully got the RR (though we don't use it further)
                }
            }
            
            // Get and convert the response code to string
            ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
            const char *rcode_str = ares_dns_rcode_tostr(rcode);
            // rcode_str is not used, but we've called the function
        }
    };

    // Start a DNS query for SOA record
    unsigned short query_id = 0;
    ares_status_t query_status = ares_query_dnsrec(channel, domain_name, 
                                                   ARES_CLASS_IN, record_type,
                                                   query_callback, nullptr, &query_id);

    // Immediately cancel the query since we're fuzzing offline
    ares_cancel(channel);

    // Try to create an output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some debugging info to output file
        fprintf(out_file, "Fuzzed domain: %s\n", domain_name);
        fprintf(out_file, "Query status: %d\n", query_status);
        assert_file_closed(&out_file);;
    }

    // Clean up resources
    assert_file_closed(&in_file);;
    
    // Process any pending events (with empty fd sets since we're not really reading/writing)
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    // Set a timeout to avoid hanging
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;  // 1ms timeout
    
    ares_process(channel, &read_fds, &write_fds);
    
    // Clean up channel and library
    ares_destroy(channel);
    ares_library_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
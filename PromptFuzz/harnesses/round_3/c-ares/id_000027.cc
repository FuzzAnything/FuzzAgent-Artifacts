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
//<ID> 509
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 5
//<Quality> {"density":10,"unique_branches":{"ares_dns_rr_get_abin":[[905,7,905,59,0,0,4,0],[910,7,910,19,0,0,4,0]],"ares_dns_rr_set_str":[[1300,7,1300,18,0,0,4,0],[1302,9,1302,21,0,0,4,1],[1308,7,1308,29,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_socket_callback","ares_destroy","ares_library_cleanup","ares_set_servers_csv","ares_destroy","ares_library_cleanup","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_rr_get_const","ares_dns_rr_get_ttl","ares_dns_rr_get_abin","ares_dns_rr_get_abin","ares_dns_record_rr_add","ares_dns_rr_set_str","ares_queue_active_queries","ares_process_fds","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_socket_callback","ares_set_servers_csv","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_rr_get_const","ares_dns_rr_get_ttl","ares_dns_rr_get_abin","ares_dns_rr_get_abin","ares_dns_record_rr_add","ares_dns_rr_set_str","ares_queue_active_queries","ares_process_fds","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

// Custom socket callback for ares_set_socket_callback
static ares_socket_t  id_000027_socket_callback_create(ares_socket_t sockfd, int type, void *data) {
    // In fuzzing context, we might not create real sockets
    return ARES_SOCKET_BAD;
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    ares_channel_t *channel = nullptr;
    FILE *in_file = nullptr;
    
    // Create a channel
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Use ares_set_socket_callback
    ares_set_socket_callback(channel,  id_000027_socket_callback_create, nullptr);

    // Use ares_set_servers_csv with a simple DNS server string
    // Use a small portion of input data for server string
    size_t csv_len = size > 100 ? 100 : size;
    char *server_csv = (char*)malloc(csv_len + 1);
    if (!server_csv) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }
    memcpy(server_csv, data, csv_len);
    server_csv[csv_len] = '\0';
    ares_set_servers_csv(channel, server_csv);
    free(server_csv);

    // Create a file from input data for potential parsing
    in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Try to parse DNS response from input data
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
    
    if (parse_status == ARES_SUCCESS && dnsrec) {
        // Use ares_dns_record_get_rcode
        ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
        
        // Get opcode from DNS record and convert to string
        ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
        const char *opcode_str = ares_dns_opcode_tostr(opcode);
        (void)opcode_str; // Use to avoid unused variable warning
        
        // Get number of answer RRs and process them
        size_t answer_count = 0;
        const ares_dns_rr_t *rr = nullptr;
        
        // Count answer RRs (iterate until we get NULL)
        while ((rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, answer_count)) != nullptr) {
            // Use ares_dns_rr_get_ttl
            unsigned int ttl = ares_dns_rr_get_ttl(rr);
            (void)ttl;
            
            // Use ares_dns_rr_get_abin for different keys
            // Try to get A record data if available
            size_t bin_len = 0;
            const unsigned char *bin_data = ares_dns_rr_get_abin(rr, ARES_RR_A_ADDR, fuzz_uint64_t_2, &bin_len);
            (void)bin_data;
            (void)bin_len;
            
            // Try to get TXT record data if available
            bin_data = ares_dns_rr_get_abin(rr, ARES_RR_TXT_DATA, fuzz_uint64_t_3, &bin_len);
            (void)bin_data;
            (void)bin_len;
            
            answer_count++;
        }
        
        // Create a new RR to test ares_dns_rr_set_str
        if (answer_count > 0) {
            ares_dns_rr_t *new_rr = nullptr;
            // Try to add a new RR to the authority section
            if (ares_dns_record_rr_add(&new_rr, dnsrec, ARES_SECTION_AUTHORITY, 
                                      fuzz_str_4, ARES_REC_TYPE_NS, 
                                      ARES_CLASS_IN, fuzz_uint32_t_5) == ARES_SUCCESS && new_rr) {
                // Use ares_dns_rr_set_str to set NS server
                ares_dns_rr_set_str(new_rr, ARES_RR_NS_NSDNAME, fuzz_str_6);
            }
        }
    }

    // Use ares_queue_active_queries
    size_t active_queries = ares_queue_active_queries(channel);
    (void)active_queries;

    // Create fake file descriptor events for ares_process_fds
    // We'll create minimal events since we don't have real sockets
    ares_fd_events_t events[1];
    events[0].fd = ARES_SOCKET_BAD;
    events[0].events = 0;
    
    // Use ares_process_fds
    ares_status_t process_status = ares_process_fds(channel, events, 0, fuzz_uint32_t_7);
    (void)process_status;

    // Clean up
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    ares_destroy(channel);
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
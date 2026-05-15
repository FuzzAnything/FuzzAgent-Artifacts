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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 114, nr_unique_branch: 20
//<Quality> {"density":19,"unique_branches":{"ares_dns_addr_to_ptr":[[1474,7,1474,30,0,0,4,1],[1479,7,1479,18,0,0,4,1],[1483,7,1483,30,0,0,4,0],[1491,21,1491,26,0,0,4,0],[1491,21,1491,26,0,0,4,1],[1492,9,1492,32,0,0,4,0],[1511,9,1511,31,0,0,4,1],[1516,9,1516,31,0,0,4,1],[1521,7,1521,30,0,0,4,0],[1526,7,1526,29,0,0,4,1]],"ares_count_digits":[[128,7,128,18,0,0,4,0]],"ares_cancel":[[36,7,36,22,0,0,4,1],[42,7,42,47,0,0,4,1]],"ares_fds":[[37,7,37,22,0,0,4,1],[37,26,37,42,0,0,4,1],[37,46,37,63,0,0,4,1],[46,57,46,70,0,0,4,0],[46,57,46,70,0,0,4,1],[51,61,51,73,0,0,4,1]],"ares_dns_rec_type_tostr":[[206,5,206,27,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_server_state_callback","ares_dns_record_create","ares_destroy","ares_library_cleanup","ares_dns_record_destroy","ares_destroy","ares_library_cleanup","ares_dns_record_query_add","ares_dns_record_destroy","ares_destroy","ares_library_cleanup","ares_dns_record_query_cnt","ares_dns_record_query_set_type","ares_dns_record_get_opcode","ares_dns_addr_to_ptr","ares_free_string","ares_inet_ntop","ares_search_dnsrec","ares_timeout","ares_fds","ares_process","ares_dns_record_destroy","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_server_state_callback","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_cnt","ares_dns_record_query_set_type","ares_dns_record_get_opcode","ares_dns_addr_to_ptr","ares_free_string","ares_inet_ntop","ares_search_dnsrec","ares_timeout","ares_fds","ares_process","ares_dns_record_destroy","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Include c-ares headers

// Server state callback function
static void  id_000001_server_state_callback(const char* name, ares_bool_t is_up, int timeouts, void* user_data) {
    // Simple callback that does nothing for fuzzing
    (void)name;
    (void)is_up;
    (void)timeouts;
    (void)user_data;
}

// DNS record callback function
static void  id_000001_dnsrec_callback(void* arg, ares_status_t status, size_t timeouts, const ares_dns_record_t* dnsrec) {
    // Simple callback that does nothing for fuzzing
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 16) {
        return 0; // Need enough data for basic operations
    }

    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create channel
    ares_channel_t* channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 1. Use ares_set_server_state_callback
    ares_set_server_state_callback(channel,  id_000001_server_state_callback, nullptr);

    // 2. Create DNS record for query
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, 
                                                  (unsigned short)(data[0] | (data[1] << 8)), // ID from data
                                                  ARES_FLAG_RD, // Recursion desired
                                                  ARES_OPCODE_QUERY, 
                                                  ARES_RCODE_NOERROR);
    
    if (status != ARES_SUCCESS || dnsrec == nullptr) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }

    // 3. Extract query name from input data
    // Use first non-zero bytes as domain name (limited to 253 chars for DNS)
    size_t name_len = size > 253 ? 253 : size;
    char* domain_name = (char*)malloc(name_len + 1);
    if (!domain_name) {
        ares_dns_record_destroy(dnsrec);
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }
    
    // Copy data to domain name, ensuring null termination
    for (size_t i = 0; i < name_len; i++) {
        if (data[i] == 0 || data[i] == '.') {
            domain_name[i] = '.';
        } else if (data[i] < 32 || data[i] > 126) {
            domain_name[i] = 'a' + (data[i] % 26);
        } else {
            domain_name[i] = data[i];
        }
    }
    domain_name[name_len] = '\0';

    // 4. Add query to DNS record
    status = ares_dns_record_query_add(dnsrec, 
                                       domain_name,
                                       ARES_REC_TYPE_A, // Default type
                                       ARES_CLASS_IN);
    
    free(domain_name);
    
    if (status != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }

    // 5. Modify query type using data
    size_t type_index = data[2] % 256; // Ensure within bounds
    if (type_index >= ares_dns_record_query_cnt(dnsrec)) {
        type_index = 0;
    }
    
    ares_dns_rec_type_t qtype = (ares_dns_rec_type_t)((data[3] % 16) + 1); // Valid DNS types 1-16
    ares_dns_record_query_set_type(dnsrec, type_index, qtype);

    // 6. Get opcode from DNS record
    ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
    (void)opcode; // Use opcode to avoid unused variable warning

    // 7. Create a test address for ares_dns_addr_to_ptr
    struct ares_addr test_addr;
    test_addr.family = AF_INET;
    // Use bytes from input for IP address
    if (size >= 20) {
        uint32_t ip = *(uint32_t*)(data + 16);
        test_addr.addr.addr4.s_addr = ip;
    } else {
        test_addr.addr.addr4.s_addr = htonl(0x7f000001); // 127.0.0.1
    }

    // 8. Convert address to PTR string
    char* ptr_str = ares_dns_addr_to_ptr(&test_addr);
    if (ptr_str) {
        ares_free_string(ptr_str);
    }

    // 9. Prepare source address for ares_inet_ntop
    char src_addr[INET6_ADDRSTRLEN];
    if (test_addr.family == AF_INET) {
        // Use ares_inet_ntop to convert IPv4 address
        const char* result = ares_inet_ntop(AF_INET, 
                                          &test_addr.addr.addr4, 
                                          src_addr, 
                                          sizeof(src_addr));
        (void)result; // Use result to avoid unused variable warning
    }

    // 10. Use ares_search_dnsrec to perform DNS query
    status = ares_search_dnsrec(channel, dnsrec,  id_000001_dnsrec_callback, nullptr);
    
    // 11. Get timeout information
    struct timeval tv;
    struct timeval* timeout = ares_timeout(channel, nullptr, &tv);
    (void)timeout; // Use timeout to avoid unused variable warning

    // 12. Process any pending DNS queries
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    if (nfds > 0) {
        // Set timeout to 0 for non-blocking
        struct timeval zero_timeout;
        zero_timeout.tv_sec = 0;
        zero_timeout.tv_usec = 0;
        
        select(nfds, &read_fds, &write_fds, nullptr, &zero_timeout);
        ares_process(channel, &read_fds, &write_fds);
    }

    // Clean up
    ares_dns_record_destroy(dnsrec);
    ares_cancel(channel); // Cancel any pending queries
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}
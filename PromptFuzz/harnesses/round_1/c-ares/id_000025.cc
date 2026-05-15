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
//<ID> 657
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 8
//<Quality> {"density":11,"unique_branches":{"ares_save_opt_servers":[[63,7,63,18,0,0,4,1],[67,57,67,70,0,0,4,0],[67,57,67,70,0,0,4,1],[71,9,71,39,0,0,4,0],[71,9,71,39,0,0,4,1]],"ares_save_options":[[143,7,143,42,0,0,4,0],[145,9,145,33,0,0,4,1],[573,23,573,53,1,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_servers_csv","ares_set_server_state_callback","ares_save_options","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_addr","ares_dns_rr_set_opt","ares_set_socket_callback","ares_search","ares_fds","ares_process_fd","ares_queue_wait_empty","ares_dns_record_destroy","ares_destroy_options","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_servers_csv","ares_set_server_state_callback","ares_save_options","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_addr","ares_dns_rr_set_opt","ares_set_socket_callback","ares_search","ares_fds","ares_process_fd","ares_queue_wait_empty","ares_dns_record_destroy","ares_destroy_options","ares_destroy","ares_library_cleanup"],"visited":2}
/**/


// Include necessary c-ares headers

// Dummy callback functions
static void  id_000025_dummy_callback(void* arg, int status, int timeouts, 
                          const unsigned char* abuf, int alen) {
    // Do nothing
}

static void  id_000025_dummy_server_state_callback(const char* server, 
                                       ares_bool_t enabled, 
                                       int reason, void* user_data) {
    // Do nothing
}

static int  id_000025_dummy_socket_callback(ares_socket_t socket_fd, int type, 
                                void* user_data) {
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    if (size < 20) {
        // Need minimum data for DNS record and address
        return 0;
    }

    // Create a FILE* for reading input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Create channel
    ares_channel_t* channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Parse CSV servers from input data
    // Read first part of data as CSV string (up to 256 bytes or until null byte)
    char servers_csv[256] = {0};
    size_t csv_len = size > 255 ? 255 : size;
    
    // Find null terminator in input data
    for (size_t i = 0; i < size && i < 255; i++) {
        if (data[i] == 0) {
            csv_len = i;
            break;
        }
    }
    
    memcpy(servers_csv, data, csv_len);
    servers_csv[csv_len] = '\0';
    
    // Set servers using CSV format
    ares_set_servers_csv(channel, servers_csv);
    
    // Step 3: Set server state callback
    ares_set_server_state_callback(channel,  id_000025_dummy_server_state_callback, nullptr);
    
    // Step 4: Save channel options
    struct ares_options options;
    int optmask = 0;
    ares_save_options(channel, &options, &optmask);
    
    // Step 5: Create DNS record and resource record
    ares_dns_record_t* dnsrec = nullptr;
    ares_dns_rr_t* rr = nullptr;
    
    // Create DNS record with ID from input data
    unsigned short id = *(unsigned short*)data;
    ares_status_t status = ares_dns_record_create(&dnsrec, id, 
                                                 ARES_FLAG_QR | ARES_FLAG_RD,
                                                 ARES_OPCODE_QUERY,
                                                 ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Add a resource record to answer section
        status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                                       fuzz_str_1, ARES_REC_TYPE_A,
                                       ARES_CLASS_IN, fuzz_uint32_t_2);
        
        // Step 6: Set address in resource record if we have enough data
        if (status == ARES_SUCCESS && rr && size >= 8) {
            struct in_addr addr;
            // Use bytes 2-5 for IP address
            memcpy(&addr.s_addr, data + 2, 4);
            ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr);
            
            // Step 8: Set option in resource record
            // Use bytes 6-7 for option ID
            if (size >= 10) {
                unsigned short opt_id = *(unsigned short*)(data + 6);
                size_t opt_val_len = size - 8 > 100 ? 100 : size - 8;
                ares_dns_rr_set_opt(rr, ARES_RR_OPT_OPTIONS, opt_id,
                                   data + 8, opt_val_len);
            }
        }
    }
    
    // Step 7: Set socket callback
    ares_set_socket_callback(channel,  id_000025_dummy_socket_callback, nullptr);
    
    // Step 8: Perform DNS search
    // Use part of input data as domain name
    char domain[256] = {0};
    size_t domain_len = size > 100 ? 100 : size;
    
    // Ensure domain name is null-terminated
    for (size_t i = 0; i < domain_len; i++) {
        // Only use printable characters for domain
        if (data[i] >= 32 && data[i] <= 126) {
            domain[i] = data[i];
        } else {
            domain[i] = 'a';
        }
    }
    domain[domain_len] = '\0';
    
    // Ensure we have at least something that looks like a domain
    if (domain_len == 0) {
        strcpy(domain, "example.com");
    }
    
    // Perform DNS search
    ares_search(channel, domain, C_IN, T_A,  id_000025_dummy_callback, nullptr);
    
    // Step 9: Process pending operations
    // Get file descriptors
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    
    if (nfds > 0) {
        // Create timeout
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 1000; // 1ms timeout
        
        // Process file descriptors
        for (int i = 0; i < nfds; i++) {
            ares_socket_t sock = i;
            if (FD_ISSET(sock, &read_fds) || FD_ISSET(sock, &write_fds)) {
                ares_process_fd(channel, 
                               FD_ISSET(sock, &read_fds) ? sock : ARES_SOCKET_BAD,
                               FD_ISSET(sock, &write_fds) ? sock : ARES_SOCKET_BAD);
            }
        }
    }
    
    // Step 10: Wait for queue to empty
    ares_queue_wait_empty(channel, 100); // 100ms timeout
    
    // Cleanup
    
    // Destroy DNS record if created
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    // Destroy saved options
    ares_destroy_options(&options);
    
    // Destroy channel
    ares_destroy(channel);
    
    // Cleanup library
    ares_library_cleanup();
    
    // Close input file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}
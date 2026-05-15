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
//<ID> 757
//<Prompt> []
/*<Combination>: [
*/
//<score> 76.5, nr_unique_branch: 20
//<Quality> {"density":17,"unique_branches":{"ares_dns_opcode_tostr":[[256,5,256,27,0,0,4,1],[264,5,264,28,0,0,4,0]],"ares_dns_class_tostr":[[239,5,239,23,0,0,4,1],[241,5,241,26,0,0,4,0],[243,5,243,27,0,0,4,0],[245,5,245,24,0,0,4,0],[247,5,247,25,0,0,4,0]],"process_write":[[392,7,392,19,0,0,4,0]],"ares_process_fd":[[281,7,281,34,0,0,4,0],[282,9,282,28,0,0,4,0]],"ares_nameoffset_find":[[129,28,129,55,0,0,4,0]],"ares_process_fds_nolock":[[210,9,210,50,0,0,4,1],[216,9,216,30,0,0,4,1],[224,9,224,49,0,0,4,0]],"ares_dns_rec_type_tostr":[[212,5,212,27,0,0,4,0],[222,5,222,29,0,0,4,0],[224,5,224,27,0,0,4,0],[228,5,228,27,0,0,4,0]],"ares_qcache_calc_key":[[73,7,73,27,0,0,4,0],[75,9,75,31,0,0,4,1]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_set_local_ip6","ares_set_pending_write_cb","ares_parse_mx_reply","ares_dns_parse","ares_dns_record_query_cnt","ares_dns_record_query_get","ares_search_dnsrec","ares_process_fd","ares_getsock","ares_process_fd","ares_queue_wait_empty","ares_dns_record_destroy","ares_free_data","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_local_ip6","ares_set_pending_write_cb","ares_parse_mx_reply","ares_dns_parse","ares_dns_record_query_cnt","ares_dns_record_query_get","ares_search_dnsrec","ares_process_fd","ares_getsock","ares_process_fd","ares_queue_wait_empty","ares_dns_record_destroy","ares_free_data","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":1}
/*Here's a fuzz driver that uses the specified c-ares APIs to process DNS MX record replies:

*/


extern "C" {
}

// Callback for ares_search_dnsrec
static void  id_000029_search_callback_dnsrec(void* arg, ares_status_t status,
                                   size_t timeout, const ares_dns_record_t* dnsrec) {
    // Empty callback for fuzzing
    (void)arg;
    (void)status;
    (void)timeout;
    (void)dnsrec;
}

// Callback for ares_set_pending_write_cb
static void  id_000029_pending_write_callback(void* user_data) {
    // Empty callback for fuzzing
    (void)user_data;
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < sizeof(unsigned short)) {
        return 0;
    }

    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    ares_channel_t* channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize channel with default options
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Create FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int read_fd = fuzz_fileno(in_file);
    int write_fd = fuzz_fileno(out_file);

    // 1. Set local IPv6 address (using dummy address ::1)
    unsigned char local_ip6[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
    ares_set_local_ip6(channel, local_ip6);

    // 2. Set pending write callback
    ares_set_pending_write_cb(channel,  id_000029_pending_write_callback, nullptr);

    // 3. Parse input as MX reply
    struct ares_mx_reply* mx_out = nullptr;
    // Use first few bytes as flags/ID, rest as DNS reply
    if (size > 12) {
        // Skip DNS header (12 bytes) for parsing
        ares_parse_mx_reply(data + 12, size - 12, &mx_out);
    }

    // 4. Try to parse input as DNS record
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, 0, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // 5. Get query information from DNS record
        size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
        if (query_cnt > 0) {
            const char* name = nullptr;
            ares_dns_rec_type_t qtype;
            ares_dns_class_t qclass;
            
            for (size_t i = 0; i < query_cnt && i < 1; i++) { // Limit to 1 query for fuzzing
                ares_dns_record_query_get(dnsrec, i, &name, &qtype, &qclass);
                // We could use name, qtype, qclass here if needed
            }
        }

        // 6. Search using the DNS record
        ares_search_dnsrec(channel, dnsrec,  id_000029_search_callback_dnsrec, nullptr);
    }

    // 7. Process file descriptors
    ares_process_fd(channel, read_fd, write_fd);

    // 8. Get sockets from channel
    ares_socket_t socks[ARES_GETSOCK_MAXNUM];
    int numsocks = ares_getsock(channel, socks, ARES_GETSOCK_MAXNUM);
    
    if (numsocks > 0) {
        // Process sockets
        for (int i = 0; i < numsocks; i++) {
            ares_process_fd(channel, socks[i], ARES_SOCKET_BAD);
        }
    }

    // 9. Wait for queue to empty (with short timeout for fuzzing)
    ares_queue_wait_empty(channel, 100); // 100ms timeout

    // Cleanup
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    if (mx_out) {
        ares_free_data(mx_out);
    }

    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Cancel any pending operations
    ares_cancel(channel);
    
    // Clean up channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(write_fd);
	assert_fd_closed(read_fd);
	return 0;
}
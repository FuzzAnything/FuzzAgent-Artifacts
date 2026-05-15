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
//<ID> 408
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 12
//<Quality> {"density":10,"unique_branches":{"ares_free_data":[[94,7,94,40,0,0,4,0]],"ares_malloc_data":[[154,5,154,38,0,0,4,0]],"ares_parse_a_reply":[[87,7,87,23,0,0,4,0],[87,27,87,40,0,0,4,1]],"ares_get_servers_ports":[[1174,7,1174,22,0,0,4,1],[1180,56,1180,68,0,0,4,0],[1180,56,1180,68,0,0,4,1],[1186,9,1186,19,0,0,4,1],[1190,9,1190,18,0,0,4,0],[1190,9,1190,18,0,0,4,1],[1202,9,1202,37,0,0,4,0],[1211,7,1211,29,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_query_enqueue_cb","ares_parse_txt_reply_ext","ares_free_data","ares_parse_a_reply","ares_free_hostent","ares_destroy","ares_library_cleanup","ares_process_fd","ares_process","ares_get_servers_ports","ares_free_data","ares_get_servers_csv","ares_free_string","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_query_enqueue_cb","ares_parse_txt_reply_ext","ares_free_data","ares_parse_a_reply","ares_free_hostent","ares_process_fd","ares_process","ares_get_servers_ports","ares_free_data","ares_get_servers_csv","ares_free_string","ares_destroy","ares_library_cleanup"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified c-ares APIs to simulate DNS response processing:

*/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
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

    // Set query enqueue callback (empty callback for fuzzing)
    ares_set_query_enqueue_cb(channel, [](void *user_data) {
        // Empty callback
    }, nullptr);

    // Parse the input as TXT reply
    struct ares_txt_ext *txt_out = nullptr;
    if (size > 0) {
        ares_parse_txt_reply_ext(data, static_cast<int>(size), &txt_out);
        if (txt_out) {
            ares_free_data(txt_out);
        }
    }

    // Parse the input as A reply
    struct hostent *host = nullptr;
    struct ares_addrttl addrttls[10];
    int naddrttls = 0;
    if (size > 0) {
        ares_parse_a_reply(data, static_cast<int>(size), &host, addrttls, &naddrttls);
        if (host) {
            ares_free_hostent(host);
        }
    }

    // Create a file from input data for processing
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    int fd = fuzz_fileno(in_file);
    if (fd >= 0) {
        // Process the file descriptor
        ares_process_fd(channel, fd, fuzz_int32_t_1);
        
        // Also process using fd_set
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        if (fd < FD_SETSIZE) {
            FD_SET(fd, &read_fds);
            ares_process(channel, &read_fds, &write_fds);
        }
    }

    assert_file_closed(&in_file);;

    // Get servers and ports
    struct ares_addr_port_node *servers = nullptr;
    if (ares_get_servers_ports(channel, &servers) == ARES_SUCCESS) {
        if (servers) {
            ares_free_data(servers);
        }
    }

    // Get servers as CSV
    char *csv = ares_get_servers_csv(channel);
    if (csv) {
        ares_free_string(csv);
    }

    // Cleanup
    ares_destroy(channel);
    ares_library_cleanup();

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
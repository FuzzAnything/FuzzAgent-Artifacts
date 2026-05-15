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
//<ID> 322
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 11
//<Quality> {"density":6,"unique_branches":{"ares_array_insert_at":[[247,7,247,15,0,0,4,1]],"ares_array_set_size":[[176,7,176,18,0,0,4,0],[176,22,176,31,0,0,4,0],[176,35,176,50,0,0,4,0]],"ares_round_up_pow2":[[75,7,75,22,0,0,4,1]],"ares_realloc_zero":[[101,7,101,27,0,0,4,1]],"ares_buf_fetch":[[501,7,501,18,0,0,4,1],[505,7,505,18,0,0,4,0],[505,22,505,33,0,0,4,0]],"ares_malloc_zero":[[87,7,87,18,0,0,4,1]],"ares_array_at":[[69,7,69,18,0,0,4,0]]},"library_calls":["ares_library_init_mem","ares_init","ares_library_cleanup","ares_set_local_dev","ares_set_servers","ares_reinit","ares_parse_caa_reply","ares_cancel","ares_timeout","ares_free_data","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init_mem","ares_init","ares_set_local_dev","ares_set_servers","ares_reinit","ares_parse_caa_reply","ares_cancel","ares_timeout","ares_free_data","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init_mem(ARES_LIB_INIT_ALL, malloc, free, realloc) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 1. Set local device name
    // Use a portion of data as device name (limited to 100 chars for safety)
    char dev_name[101] = {0};
    size_t dev_name_len = size > 100 ? 100 : size;
    if (dev_name_len > 0) {
        memcpy(dev_name, data, dev_name_len);
        dev_name[dev_name_len] = '\0';
        ares_set_local_dev(channel, dev_name);
    }

    // 2. Set servers using data
    // Create a server list from data - use first portion for IPv4 address
    struct ares_addr_node *servers = nullptr;
    struct ares_addr_node *server_node = nullptr;
    
    if (size >= sizeof(struct in_addr)) {
        server_node = (struct ares_addr_node*)malloc(sizeof(struct ares_addr_node));
        if (server_node) {
            server_node->next = nullptr;
            server_node->family = AF_INET;
            memcpy(&server_node->addr.addr4, data, sizeof(struct in_addr));
            
            // Try to set the server
            ares_set_servers(channel, server_node);
            
            // Free the server node
            free(server_node);
        }
    }

    // 3. Reinitialize the channel
    ares_status_t reinit_status = ares_reinit(channel);

    // 4. Parse CAA reply from input data
    struct ares_caa_reply *caa_reply = nullptr;
    int parse_result = 0;
    if (size > 0) {
        parse_result = ares_parse_caa_reply(data, size, &caa_reply);
    }

    // 5. Cancel the channel (clean up pending operations)
    ares_cancel(channel);

    // 6. Get timeout information
    struct timeval maxtv = {5, 0};  // 5 second max timeout
    struct timeval tv = {0, 0};
    struct timeval *timeout = ares_timeout(channel, &maxtv, &tv);

    // Create a FILE* to read remaining data if needed
    FILE *in_file = nullptr;
    if (size > 0) {
        in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            // Read something from the file to ensure it's processed
            char buffer[256];
            size_t read_bytes = fread(buffer, 1, sizeof(buffer), in_file);
            assert_file_closed(&in_file);;
        }
    }

    // Clean up parsed CAA reply
    if (caa_reply) {
        ares_free_data(caa_reply);
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (size > 0) {
            fwrite(data, 1, size, out_file);
        }
        assert_file_closed(&out_file);;
    }

    // Clean up channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();

    assert_file_closed(&out_file);
	return 0;
}
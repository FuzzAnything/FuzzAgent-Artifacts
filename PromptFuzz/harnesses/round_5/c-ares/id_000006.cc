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
//<ID> 90
//<Prompt> []
/*<Combination>: [
*/
//<score> 73.5, nr_unique_branch: 31
//<Quality> {"density":7,"unique_branches":{"ares_process_fds_nolock":[[210,9,210,50,0,0,4,1],[216,9,216,30,0,0,4,1],[224,9,224,49,0,0,4,0]],"process_write":[[392,7,392,19,0,0,4,0]],"default_asetsockopt":[[292,5,292,37,0,0,4,0],[295,11,295,70,0,0,4,0],[295,11,295,70,0,0,4,1],[306,5,306,38,0,0,4,1]],"ares_buf_tag_fetch_bytes":[[409,7,409,18,0,0,4,0]],"ares_process_fd":[[281,7,281,34,0,0,4,0],[282,9,282,28,0,0,4,0]]," id_000006_nameinfo_callback":[[255,9,255,47,0,0,4,1]],"ares_buf_consume_nonwhitespace":[[697,15,697,32,0,0,4,1]],"ares_buf_tag_fetch":[[387,22,387,49,0,0,4,0]],"ares_buf_create_const":[[61,7,61,19,0,0,4,0]],"ares_getnameinfo_int":[[113,7,113,39,0,0,4,0],[113,43,113,72,0,0,4,0],[118,7,118,38,0,0,4,1]],"ares_socket_configure":[[261,7,261,43,0,0,4,0]],"invoke_server_state_cb":[[89,7,89,39,0,0,4,1],[94,7,94,18,0,0,4,1],[99,7,99,29,0,0,4,1],[106,7,106,28,0,0,4,1]],"ares_llist_insert_at":[[128,7,128,19,0,0,4,0]],"ares_buf_tag_fetch_string":[[449,22,449,30,0,0,4,0]],"ares_inet_net_pton_ipv4":[[93,25,93,52,6,0,4,1]],"ares_array_destroy":[[107,7,107,28,0,0,4,1]],"ares_buf_begins_with":[[1084,38,1084,51,0,0,4,0]],"ares_inet_pton":[[441,14,441,28,0,0,4,1]],"ares_llist_attach_at":[[76,23,76,35,0,0,4,0]],"ares_strnlen":[[37,7,37,18,0,0,4,1]]},"library_calls":["ares_init","ares_set_server_state_callback","ares_set_local_dev","ares_reinit","ares_getnameinfo","ares_process_fd","ares_destroy"],"critical_calls":["ares_init","ares_set_server_state_callback","ares_set_local_dev","ares_reinit","ares_getnameinfo","ares_process_fd","ares_destroy"],"visited":1}
/**/


extern "C" {
}

// Callback for ares_getnameinfo
static void  id_000006_nameinfo_callback(void* arg, int status, int timeouts, 
                             const char* node, const char* service) {
    // Do nothing in fuzzing context
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)node;
    (void)service;
}

// Callback for ares_set_server_state_callback
static void  id_000006_server_state_callback(const char* name, ares_bool_t is_up, 
                                 int timeout, void* user_data) {
    // Do nothing in fuzzing context
    (void)name;
    (void)is_up;
    (void)timeout;
    (void)user_data;
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < sizeof(struct sockaddr_in)) {
        return 0;
    }

    ares_channel_t* channel = nullptr;
    int res;
    
    // 1. Initialize channel
    res = ares_init(&channel);
    if (res != ARES_SUCCESS) {
        return 0;
    }
    
    // 2. Set server state callback
    ares_set_server_state_callback(channel,  id_000006_server_state_callback, nullptr);
    
    // 3. Set local device name (use first part of input data as device name)
    // Find null terminator or use first 63 bytes max
    size_t dev_name_len = 0;
    while (dev_name_len < size && dev_name_len < 63 && data[dev_name_len] != 0) {
        dev_name_len++;
    }
    
    if (dev_name_len > 0) {
        char* dev_name = (char*)malloc(dev_name_len + 1);
        if (dev_name) {
            memcpy(dev_name, data, dev_name_len);
            dev_name[dev_name_len] = 0;
            ares_set_local_dev(channel, dev_name);
            free(dev_name);
        }
    }
    
    // 4. Reinitialize the channel
    ares_status_t status = ares_reinit(channel);
    (void)status; // Result doesn't matter for fuzzing
    
    // 5. Create sockaddr from input data
    struct sockaddr_in sa;
    memcpy(&sa, data, sizeof(struct sockaddr_in));
    
    // Ensure valid address family for getnameinfo
    sa.sin_family = AF_INET;
    
    // 6. Call getnameinfo
    ares_getnameinfo(channel, (struct sockaddr*)&sa, sizeof(sa), 
                    0,  id_000006_nameinfo_callback, nullptr);
    
    // 7. Process file descriptors
    // Create a file from input data for reading
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        int read_fd = fuzz_fileno(in_file);
        
        // Create output file
        FILE* out_file = fopen("output_file", "wb");
        if (out_file) {
            int write_fd = fuzz_fileno(out_file);
            
            // Process the file descriptors
            ares_process_fd(channel, read_fd, write_fd);
            
            assert_file_closed(&out_file);;
            remove("output_file");
        }
        
        assert_file_closed(&in_file);;
    }
    
    // 8. Cleanup
    ares_destroy(channel);
    
    assert_file_closed(&in_file);
	return 0;
}
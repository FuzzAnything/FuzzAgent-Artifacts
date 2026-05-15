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
//<ID> 331
//<Prompt> []
/*<Combination>: [
*/
//<score> 120, nr_unique_branch: 32
//<Quality> {"density":10,"unique_branches":{"ares_timeval_remaining":[[42,7,42,27,0,0,4,1],[43,8,43,29,0,0,4,1],[48,7,48,29,0,0,4,0]],"ares_timeout_int":[[101,7,101,19,0,0,4,1],[113,7,113,20,0,0,4,1],[120,7,120,30,0,0,4,0]],"ares_socket_configure":[[261,7,261,43,0,0,4,0]],"ares_cancel":[[42,7,42,47,0,0,4,0],[55,9,55,37,0,0,4,1],[61,12,61,24,0,0,4,0],[61,12,61,24,0,0,4,1]],"ares_cookie_apply":[[305,7,305,17,0,0,4,0]],"ares_qcache_calc_key":[[67,7,67,27,0,0,4,1],[73,7,73,27,0,0,4,0],[75,9,75,31,0,0,4,1]],"ares_strnlen":[[37,7,37,18,0,0,4,1]],"ares_dns_get_opt_rr":[[1540,15,1540,71,0,0,4,1]],"ares_check_cleanup_conns":[[106,11,106,48,0,0,4,1],[111,11,111,49,0,0,4,0],[119,11,119,44,0,0,4,1],[124,11,124,46,0,0,4,0],[124,50,124,78,0,0,4,1],[129,11,129,22,0,0,4,1]],"ares_send":[[259,7,259,22,0,0,4,0],[259,26,259,43,0,0,4,0],[265,7,265,29,0,0,4,0]],"default_asetsockopt":[[277,11,277,14,0,0,4,1],[278,5,278,38,0,0,4,1],[285,5,285,38,0,0,4,1],[292,5,292,37,0,0,4,0],[295,11,295,70,0,0,4,1],[306,5,306,38,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_local_dev","ares_set_servers","ares_set_servers_ports","ares_send","ares_parse_caa_reply","ares_free_data","ares_fds","ares_timeout","ares_reinit","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_local_dev","ares_set_servers","ares_set_servers_ports","ares_send","ares_parse_caa_reply","ares_free_data","ares_fds","ares_timeout","ares_reinit","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<9) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Initialize the library with standard memory functions
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    ares_channel_t *channel = nullptr;
    
    // Create a channel
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set local device (using a dummy name)
    ares_set_local_dev(channel, fuzz_str_1);

    // Prepare server list for testing
    struct ares_addr_node server;
    server.next = nullptr;
    server.family = AF_INET;
    server.addr.addr4.s_addr = htonl(0x7F000001); // 127.0.0.1
    
    struct ares_addr_port_node server_port;
    server_port.next = nullptr;
    server_port.family = AF_INET;
    server_port.addr.addr4.s_addr = htonl(0x7F000001);
    server_port.udp_port = 53;
    server_port.tcp_port = 53;

    // Set servers using different methods
    ares_set_servers(channel, &server);
    ares_set_servers_ports(channel, &server_port);

    // Prepare query data from input
    if (size > 0) {
        // Use first byte as query ID
        unsigned short id = data[0];
        
        // Create a simple query buffer (truncate if too large)
        unsigned char *qbuf = nullptr;
        int qlen = 0;
        
        // Use input data as query buffer (skip ID byte if present)
        const size_t query_size = (size > 1) ? size - 1 : size;
        const unsigned char *query_data = (size > 1) ? data + 1 : data;
        
        if (query_size > 0) {
            qbuf = (unsigned char *)malloc(query_size);
            if (qbuf) {
                memcpy(qbuf, query_data, query_size);
                qlen = query_size;
            }
        }
        
        // Create a dummy callback
        ares_callback callback = [](void *arg, int status, int timeouts, 
                                   const unsigned char *abuf, int alen) {
            // Do nothing in fuzzing
        };
        
        // Send the query
        if (qbuf && qlen > 0) {
            ares_send(channel, qbuf, fuzz_int32_t_2, callback, nullptr);
        }
        
        // Free query buffer
        free(qbuf);
        
        // Parse the input as CAA reply (using entire input)
        struct ares_caa_reply *caa_reply = nullptr;
        if (size > 0) {
            ares_parse_caa_reply(data, size, &caa_reply);
            if (caa_reply) {
                ares_free_data(caa_reply);
            }
        }
    }

    // Process file descriptors
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    
    // Get timeout
    struct timeval tv, maxtv;
    struct timeval *timeout = ares_timeout(channel, &maxtv, &tv);
    
    // Reinitialize the channel
    ares_reinit(channel);
    
    // Cancel all pending operations
    ares_cancel(channel);
    
    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
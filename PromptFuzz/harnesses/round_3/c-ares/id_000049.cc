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
//<ID> 885
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 25
//<Quality> {"density":6,"unique_branches":{"ares_addrinfo2addrttl":[[240,7,240,24,0,0,4,0],[240,28,240,46,0,0,4,1],[244,7,244,17,0,0,4,1],[244,21,244,38,0,0,4,1],[248,7,248,24,0,0,4,1],[252,7,252,25,0,0,4,0],[252,29,252,46,0,0,4,1],[256,7,256,25,0,0,4,1],[263,10,263,20,0,0,4,0],[263,10,263,20,0,0,4,1],[264,9,264,36,0,0,4,0],[264,9,264,36,0,0,4,1],[270,26,270,38,0,0,4,0],[270,26,270,38,0,0,4,1],[271,9,271,34,0,0,4,0],[271,9,271,34,0,0,4,1],[275,9,275,36,0,0,4,1],[279,9,279,27,0,0,4,0],[280,11,280,35,0,0,4,1]],"ares_check_cleanup_conns":[[106,11,106,48,0,0,4,0]],"ares_timedout":[[168,7,168,15,0,0,4,1],[171,7,171,15,0,0,4,0]],"process_timeouts":[[704,10,704,77,0,0,4,0],[710,9,710,45,0,0,4,0]],"ares_parse_aaaa_reply":[[90,27,90,40,0,0,4,0]]},"library_calls":["ares_library_init","ares_version","ares_init","ares_library_cleanup","ares_set_socket_functions","ares_set_servers_ports","ares_mkquery","ares_parse_aaaa_reply","ares_free_hostent","ares_getaddrinfo","ares_process_fd","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_version","ares_init","ares_set_socket_functions","ares_set_servers_ports","ares_mkquery","ares_parse_aaaa_reply","ares_free_hostent","ares_getaddrinfo","ares_process_fd","ares_destroy","ares_library_cleanup"],"visited":0}
/**/



// Custom socket functions to redirect network I/O to fuzzed data
struct FuzzSocketData {
    const uint8_t* data;
    size_t size;
    size_t pos;
    bool used;
};

ares_socket_t  id_000049_fuzz_asocket(int domain, int type, int protocol, void* user_data) {
    // Return a dummy socket descriptor
    return 0x1234;
}

int  id_000049_fuzz_aclose(ares_socket_t socket, void* user_data) {
    // Nothing to close for fuzzing
    return 0;
}

int  id_000049_fuzz_aconnect(ares_socket_t socket, const struct sockaddr* addr, 
                  ares_socklen_t addrlen, void* user_data) {
    // Pretend connection succeeded
    return 0;
}

ares_ssize_t  id_000049_fuzz_arecvfrom(ares_socket_t socket, void* buffer, size_t buffer_size,
                           int flags, struct sockaddr* addr,
                           ares_socklen_t* addrlen, void* user_data) {
    FuzzSocketData* fuzz_data = static_cast<FuzzSocketData*>(user_data);
    
    if (fuzz_data->used || fuzz_data->pos >= fuzz_data->size) {
        return -1; // No more data or already used
    }
    
    // Copy fuzzed data into buffer
    size_t to_copy = std::min(buffer_size, fuzz_data->size - fuzz_data->pos);
    memcpy(buffer, fuzz_data->data + fuzz_data->pos, to_copy);
    fuzz_data->pos += to_copy;
    fuzz_data->used = true;
    
    // Set a dummy address
    if (addr && addrlen && *addrlen >= sizeof(struct sockaddr_in)) {
        struct sockaddr_in* sin = reinterpret_cast<struct sockaddr_in*>(addr);
        sin->sin_family = AF_INET;
        sin->sin_port = htons(53);
        sin->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        *addrlen = sizeof(struct sockaddr_in);
    }
    
    return to_copy;
}

ares_ssize_t  id_000049_fuzz_asendv(ares_socket_t socket, const struct iovec* vector,
                        int count, void* user_data) {
    // Pretend we sent everything
    ares_ssize_t total = 0;
    for (int i = 0; i < count; i++) {
        total += vector[i].iov_len;
    }
    return total;
}

// Callbacks
void  id_000049_dnsrec_callback(void* arg, ares_status_t status, size_t timeouts,
                     const ares_dns_record_t* dnsrec) {
    // Nothing to do in fuzzing context
}

void  id_000049_addrinfo_callback(void* arg, int status, int timeouts,
                       struct ares_addrinfo* ai) {
    // Nothing to do in fuzzing context
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<31) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Get version
    int version;
    const char* ver_str = ares_version(&version);
    
    // Create channel
    ares_channel_t* channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Set up custom socket functions
    FuzzSocketData fuzz_data = {data, size, 0, false};
    struct ares_socket_functions funcs = {
         id_000049_fuzz_asocket,
         id_000049_fuzz_aclose,
         id_000049_fuzz_aconnect,
         id_000049_fuzz_arecvfrom,
         id_000049_fuzz_asendv
    };
    ares_set_socket_functions(channel, &funcs, &fuzz_data);
    
    // Set dummy servers
    struct ares_addr_port_node server;
    server.next = nullptr;
    server.family = AF_INET;
    server.addr.addr4.s_addr = htonl(INADDR_LOOPBACK);
    server.udp_port = 53;
    server.tcp_port = 53;
    ares_set_servers_ports(channel, &server);
    
    // Create a DNS query
    unsigned char* query_buf = nullptr;
    int query_len = 0;
    if (ares_mkquery(fuzz_str_1, C_IN, T_AAAA, fuzz_uint16_t_2, fuzz_int32_t_3, 
                     &query_buf, &query_len) == ARES_SUCCESS) {
        free(query_buf);
    }
    
    // Create a DNS record for searching
    ares_dns_record_t* dnsrec = nullptr;
    // Note: ares_dns_record_create is not in provided API list, but we need it
    // to use ares_dns_record_query_add. We'll skip this part if we can't create.
    // For fuzzing purposes, we'll focus on parsing the input data instead.
    
    // Parse the input data as AAAA reply
    struct hostent* host = nullptr;
    struct ares_addr6ttl addrttls[10];
    int naddrttls = 10;
    if (size > 0) {
        ares_parse_aaaa_reply(data, static_cast<int>(size), &host, addrttls, &naddrttls);
        if (host) {
            ares_free_hostent(host);
        }
    }
    
    // Try to use ares_getaddrinfo (will use our custom socket functions)
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    // Start async query
    ares_getaddrinfo(channel, "example.com", fuzz_str_4, &hints, 
                      id_000049_addrinfo_callback, nullptr);
    
    // Process pending operations
    ares_process_fd(channel, ARES_SOCKET_BAD, fuzz_int32_t_5);
    
    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
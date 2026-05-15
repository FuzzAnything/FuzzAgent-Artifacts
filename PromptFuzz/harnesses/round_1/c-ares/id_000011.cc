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
//<ID> 177
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"ares_library_cleanup":[[141,7,141,24,0,0,4,0]]},"library_calls":["ares_init","ares_strerror","ares_set_socket_configure_callback","ares_set_local_ip4","ares_set_socket_functions","ares_dns_parse","ares_dns_record_query_cnt","ares_dns_addr_to_ptr","ares_inet_ntop","ares_free_string","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_init","ares_strerror","ares_set_socket_configure_callback","ares_set_local_ip4","ares_set_socket_functions","ares_dns_parse","ares_dns_record_query_cnt","ares_dns_addr_to_ptr","ares_inet_ntop","ares_free_string","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

// Socket configure callback function
static int  id_000011_socket_configure_callback(ares_socket_t sock, int type, void *user_data) {
    // Simple callback that does nothing
    return 0;
}

// Socket functions structure with dummy implementations
static ares_socket_t  id_000011_dummy_asocket(int domain, int type, int protocol, void *user_data) {
    return socket(domain, type, protocol);
}

static int  id_000011_dummy_aclose(ares_socket_t sock, void *user_data) {
    return close(sock);
}

static int  id_000011_dummy_aconnect(ares_socket_t sock, const struct sockaddr *addr, 
                         ares_socklen_t addrlen, void *user_data) {
    return connect(sock, addr, addrlen);
}

static ares_ssize_t  id_000011_dummy_arecvfrom(ares_socket_t sock, void *buf, size_t len,
                                    int flags, struct sockaddr *from,
                                    ares_socklen_t *fromlen, void *user_data) {
    return recvfrom(sock, buf, len, flags, from, fromlen);
}

static ares_ssize_t  id_000011_dummy_asendv(ares_socket_t sock, const struct iovec *vec,
                                int len, void *user_data) {
    // Simple implementation - just send first vector
    if (len > 0) {
        return send(sock, vec[0].iov_base, vec[0].iov_len, 0);
    }
    return -1;
}

static struct ares_socket_functions  id_000011_socket_funcs = {
     id_000011_dummy_asocket,
     id_000011_dummy_aclose,
     id_000011_dummy_aconnect,
     id_000011_dummy_arecvfrom,
     id_000011_dummy_asendv
};

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    if (size < 12) {
	return 0;
	}
	  // Minimum DNS header size

    // 1. Initialize c-ares library and create channel
    ares_channel_t *channel = nullptr;
    int init_result = ares_init(&channel);
    
    // Use ares_strerror to get error string (API #4)
    const char *error_str = ares_strerror(init_result);
    
    // 2. Set socket configure callback (API #2)
    ares_set_socket_configure_callback(channel,  id_000011_socket_configure_callback, nullptr);
    
    // 3. Set local IPv4 address (API #3)
    // Use a random address from fuzz data if available
    unsigned int local_ip = 0;
    if (size >= 4) {
        memcpy(&local_ip, data, 4);
    }
    ares_set_local_ip4(channel, fuzz_uint32_t_1);
    
    // 4. Set socket functions (API #5)
    ares_set_socket_functions(channel, & id_000011_socket_funcs, nullptr);
    
    // 5. Parse DNS data (API #8)
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t parse_result = ares_dns_parse(data, size, fuzz_uint32_t_2, &dnsrec);
    
    if (parse_result == ARES_SUCCESS && dnsrec != nullptr) {
        // 6. Get query count (API #12)
        size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
        
        // 7. Create a dummy ares_addr and convert to PTR (API #6)
        struct ares_addr addr;
        if (size >= 8) {
            // Use first 4 bytes for IPv4 address
            addr.family = AF_INET;
            memcpy(&addr.addr.addr4, data + 4, 4);
            
            char *ptr_str = ares_dns_addr_to_ptr(&addr);
            if (ptr_str) {
                // 8. Use ares_inet_ntop to convert address back to string
                char ip_str[INET6_ADDRSTRLEN];
                const void *src = (addr.family == AF_INET) ? 
                    (const void*)&addr.addr.addr4 : 
                    (const void*)&addr.addr.addr6;
                
                ares_inet_ntop(addr.family, src, ip_str, sizeof(ip_str));
                
                // 9. Free the string from ares_dns_addr_to_ptr (API #10)
                ares_free_string(ptr_str);
            }
        }
        
        // 10. Destroy DNS record (API #9)
        ares_dns_record_destroy(dnsrec);
    }
    
    // 11. Destroy channel (API #11)
    ares_destroy(channel);
    
    // 12. Clean up library (API #13)
    ares_library_cleanup();
    
    return 0;
}
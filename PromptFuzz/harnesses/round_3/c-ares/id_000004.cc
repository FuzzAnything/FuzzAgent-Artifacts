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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["ares_init","ares_set_socket_functions","ares_set_socket_configure_callback","ares_threadsafety","ares_dns_record_create","ares_destroy","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_set_ttl","ares_dns_rr_add_abin","ares_dns_rr_get_opt","ares_dns_rr_del_abin","ares_dns_rr_key_tostr","ares_query","ares_fds","ares_process","ares_destroy","ares_dns_record_destroy"],"critical_calls":["ares_init","ares_set_socket_functions","ares_set_socket_configure_callback","ares_threadsafety","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_set_ttl","ares_dns_rr_add_abin","ares_dns_rr_get_opt","ares_dns_rr_del_abin","ares_dns_rr_key_tostr","ares_query","ares_fds","ares_process","ares_destroy","ares_dns_record_destroy"],"visited":0}
/**/


// Custom socket functions structure
static ares_socket_t  id_000004_dummy_socket(int domain, int type, int protocol, void *user_data) {
    return ARES_SOCKET_BAD;
}

static int  id_000004_dummy_close(ares_socket_t socket_fd, void *user_data) {
    return 0;
}

static int  id_000004_dummy_connect(ares_socket_t socket_fd, const struct sockaddr *addr, 
                        ares_socklen_t addrlen, void *user_data) {
    return -1;
}

static ares_ssize_t  id_000004_dummy_recvfrom(ares_socket_t socket_fd, void *buf, size_t len,
                                  int flags, struct sockaddr *from, 
                                  ares_socklen_t *fromlen, void *user_data) {
    return -1;
}

static ares_ssize_t  id_000004_dummy_sendv(ares_socket_t socket_fd, const struct iovec *vec,
                               int len, void *user_data) {
    return -1;
}

// Socket configure callback
static int  id_000004_dummy_sock_config(ares_socket_t socket_fd, int flags, void *user_data) {
    return 0;
}

// Query callback
static void  id_000004_query_callback(void *arg, int status, int timeouts, 
                          const unsigned char *abuf, int alen) {
    // Do nothing in fuzzing context
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // Initialize c-ares
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        return 0;
    }
    
    // 1. Set socket functions
    struct ares_socket_functions funcs = {
         id_000004_dummy_socket,
         id_000004_dummy_close,
         id_000004_dummy_connect,
         id_000004_dummy_recvfrom,
         id_000004_dummy_sendv
    };
    ares_set_socket_functions(channel, &funcs, nullptr);
    
    // 2. Set socket configure callback
    ares_set_socket_configure_callback(channel,  id_000004_dummy_sock_config, nullptr);
    
    // 3. Check thread safety
    ares_threadsafety();
    
    // Create a DNS record to work with
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, fuzz_uint16_t_1, fuzz_uint16_t_2, 
                                                  ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    if (status != ARES_SUCCESS) {
        ares_destroy(channel);
        return 0;
    }
    
    // Add a query to the record
    char query_name[256];
    size_t name_len = size > 255 ? 255 : size;
    memcpy(query_name, data, name_len);
    query_name[name_len] = '\0';
    
    ares_dns_record_query_add(dnsrec, query_name, ARES_REC_TYPE_A, ARES_CLASS_IN);
    
    // Add a resource record
    ares_dns_rr_t *rr = nullptr;
    status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                   query_name, ARES_REC_TYPE_A, ARES_CLASS_IN, fuzz_uint32_t_3);
    if (status == ARES_SUCCESS && rr != nullptr) {
        // 4. Set TTL using input data
        unsigned int ttl = 0;
        if (size >= 8) {
            memcpy(&ttl, data, sizeof(ttl));
        }
        ares_dns_rr_set_ttl(rr, fuzz_uint32_t_4);
        
        // 5. Add binary data from input
        if (size > 8) {
            size_t bin_len = size - 8;
            if (bin_len > 100) bin_len = 100; // Limit size
            ares_dns_rr_add_abin(rr, ARES_RR_A_ADDR, data + 8, bin_len);
        }
        
        // 6. Get option (if any binary data was added)
        const unsigned char *opt_val = nullptr;
        size_t opt_len = 0;
        if (ares_dns_rr_get_opt(rr, ARES_RR_A_ADDR, fuzz_uint64_t_5, &opt_val, &opt_len)) {
            // Option exists
        }
        
        // 7. Delete binary data if we added any
        if (size > 8) {
            ares_dns_rr_del_abin(rr, ARES_RR_A_ADDR, fuzz_uint64_t_6);
        }
        
        // 8. Convert key to string
        const char *key_str = ares_dns_rr_key_tostr(ARES_RR_A_ADDR);
        if (key_str) {
            // 9. Free the string (if needed - but tostr returns const, so we don't own it)
        }
    }
    
    // 10. Perform a query using the input data
    if (size > 0) {
        ares_query(channel, query_name, C_IN, T_A,  id_000004_query_callback, nullptr);
    }
    
    // Process pending operations (non-blocking)
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    ares_fds(channel, &read_fds, &write_fds);
    ares_process(channel, &read_fds, &write_fds);
    
    // Cleanup
    ares_destroy(channel);
    ares_dns_record_destroy(dnsrec);
    
    return 0;
}
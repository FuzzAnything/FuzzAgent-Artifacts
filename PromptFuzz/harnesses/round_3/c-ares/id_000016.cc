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
//<ID> 312
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 3
//<Quality> {"density":13,"unique_branches":{"ares_dns_rr_add_abin":[[940,5,940,38,0,0,4,1],[941,39,941,50,0,0,4,1],[945,7,945,59,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_save_options","ares_destroy","ares_library_cleanup","ares_gethostbyname_file","ares_free_hostent","ares_parse_soa_reply","ares_free_data","ares_inet_ntop","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_name","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_del","ares_dns_record_destroy","ares_dns_record_rr_add","ares_dns_rr_add_abin","ares_dns_record_destroy","ares_query_dnsrec","ares_timeout","ares_getsock","ares_process_fd","ares_process_fd","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_save_options","ares_gethostbyname_file","ares_free_hostent","ares_parse_soa_reply","ares_free_data","ares_inet_ntop","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_name","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_del","ares_dns_record_destroy","ares_dns_record_rr_add","ares_dns_rr_add_abin","ares_dns_record_destroy","ares_query_dnsrec","ares_timeout","ares_getsock","ares_process_fd","ares_process_fd","ares_destroy","ares_library_cleanup"],"visited":1}
/**/



// Declare missing types that might be needed
typedef void (*ares_callback_dnsrec)(void *arg, ares_status_t status,
                                     size_t timeouts, const ares_dns_record_t *dnsrec);
typedef int (*ares_sock_create_callback)(ares_socket_t socket_fd, int type, void *data);
typedef int (*ares_sock_config_callback)(ares_socket_t socket_fd, int type, void *data);
typedef void (*ares_pending_write_callback)(void *data, ares_socket_t socket_fd);

// Required API that wasn't in the initial list but is needed
extern "C" ares_status_t ares_dns_record_rr_del(ares_dns_record_t *dnsrec, 
                                                ares_dns_section_t sect, size_t idx);
extern "C" ares_status_t ares_query_dnsrec(ares_channel_t *channel, const char *name,
                                          ares_dns_class_t dnsclass, ares_dns_rec_type_t type,
                                          ares_callback_dnsrec callback, void *arg,
                                          unsigned short *qid);

// The fuzz driver
extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<67) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Initialize ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 1. Use ares_save_options
    struct ares_options options;
    int optmask = 0;
    if (ares_save_options(channel, &options, &optmask) == ARES_SUCCESS) {
        // Options saved successfully
    }

    // 2. Use ares_gethostbyname_file with input data as file content
    // Create a temporary file with the input data
    FILE *hosts_file = fmemopen((void *)data, size, "rb");
    if (!hosts_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&hosts_file);
	return 0;
    }
    
    // Set hosts file path (simulate by using the file descriptor)
    // Note: This is a simplified approach as c-ares doesn't directly accept FILE*
    int fd = fuzz_fileno(hosts_file);
    if (fd >= 0) {
        // We'll use the file content as if it were a hosts file
        // For demonstration, we'll just call the function with a test name
        struct hostent *host = nullptr;
        ares_gethostbyname_file(channel, fuzz_str_1, fuzz_int32_t_2, &host);
        if (host) {
            ares_free_hostent(host);
        }
    }
    assert_file_closed(&hosts_file);;

    // 3. Parse the input as SOA reply
    struct ares_soa_reply *soa_out = nullptr;
    if (size >= 20) {  // Minimum size for SOA
        ares_parse_soa_reply(data, static_cast<int>(size), &soa_out);
        if (soa_out) {
            // Clean up SOA reply
            ares_free_data(soa_out);
        }
    }

    // 4. Use ares_inet_ntop to convert IP addresses
    char ip_str[INET6_ADDRSTRLEN];
    if (size >= 4) {
        struct in_addr ipv4_addr;
        memcpy(&ipv4_addr, data, sizeof(ipv4_addr) < size ? sizeof(ipv4_addr) : size);
        const char *result = ares_inet_ntop(AF_INET, &ipv4_addr, ip_str, sizeof(ip_str));
        (void)result; // Suppress unused warning
    }

    // 5. Create and manipulate a DNS record
    ares_dns_record_t *dnsrec = nullptr;
    if (ares_dns_parse(data, size, fuzz_uint32_t_3, &dnsrec) == ARES_SUCCESS && dnsrec) {
        
        // 6. Use ares_dns_rr_get_name
        size_t answer_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
        if (answer_count > 0) {
            const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_4);
            if (rr) {
                const char *name = ares_dns_rr_get_name(rr);
                (void)name; // Suppress unused warning
            }
        }

        // 7. Use ares_dns_record_rr_del (if we have at least one RR)
        if (answer_count > 0) {
            // Create a mutable copy for deletion
            ares_dns_record_t *mutable_dnsrec = nullptr;
            // We need to parse again to get a mutable version
            // For simplicity, we'll parse a subset of the data
            size_t parse_size = size > 100 ? 100 : size;
            if (ares_dns_parse(data, parse_size, fuzz_uint32_t_5, &mutable_dnsrec) == ARES_SUCCESS && 
                mutable_dnsrec) {
                size_t mutable_count = ares_dns_record_rr_cnt(mutable_dnsrec, ARES_SECTION_ANSWER);
                if (mutable_count > 0) {
                    ares_dns_record_rr_del(mutable_dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_6);
                }
                ares_dns_record_destroy(mutable_dnsrec);
            }
        }

        // 8. Use ares_dns_rr_add_abin
        // Create a new RR and add binary data
        ares_dns_rr_t *new_rr = nullptr;
        if (ares_dns_record_rr_add(&new_rr, dnsrec, ARES_SECTION_ANSWER, 
                                  fuzz_str_7, ARES_REC_TYPE_A, 
                                  ARES_CLASS_IN, fuzz_uint32_t_8) == ARES_SUCCESS && new_rr) {
            // Add some binary data from the input
            size_t data_to_add = size > 16 ? 16 : size;
            ares_dns_rr_add_abin(new_rr, ARES_RR_A_ADDR, data, data_to_add);
        }

        ares_dns_record_destroy(dnsrec);
    }

    // 9. Use ares_query_dnsrec (asynchronous query)
    // We'll create a simple callback
    auto callback = [](void *arg, ares_status_t status, 
                      size_t timeouts, const ares_dns_record_t *dnsrec) {
        (void)arg; (void)status; (void)timeouts; (void)dnsrec;
    };
    
    unsigned short qid = 0;
    if (size > 0) {
        // Use first byte as domain name length hint
        size_t name_len = data[0] % 64;
        if (name_len > 0 && name_len + 1 < size) {
            char domain[256];
            size_t copy_len = name_len < sizeof(domain) - 1 ? name_len : sizeof(domain) - 1;
            memcpy(domain, data + 1, copy_len);
            domain[copy_len] = '\0';
            
            ares_query_dnsrec(channel, domain, ARES_CLASS_IN, 
                            ARES_REC_TYPE_A, callback, nullptr, &qid);
        }
    }

    // Process any pending events
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    ares_timeout(channel, nullptr, &tv);
    
    ares_socket_t socks[ARES_GETSOCK_MAXNUM];
    int bitmask = ares_getsock(channel, socks, ARES_GETSOCK_MAXNUM);
    if (bitmask) {
        for (int i = 0; i < ARES_GETSOCK_MAXNUM; i++) {
            if (ARES_GETSOCK_READABLE(bitmask, i)) {
                ares_process_fd(channel, socks[i], fuzz_int32_t_9);
            }
            if (ARES_GETSOCK_WRITABLE(bitmask, i)) {
                ares_process_fd(channel, ARES_SOCKET_BAD, socks[i]);
            }
        }
    }

    // Cleanup
    ares_destroy(channel);
    ares_library_cleanup();

    assert_file_closed(&hosts_file);
	assert_fd_closed(fd);
	return 0;
}
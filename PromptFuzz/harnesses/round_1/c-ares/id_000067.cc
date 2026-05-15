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
//<ID> 1243
//<Prompt> []
/*<Combination>: [
*/
//<score> 77, nr_unique_branch: 11
//<Quality> {"density":11,"unique_branches":{"ares_fetch_connection":[[1197,7,1197,23,0,0,4,0]],"ares_open_connection":[[350,35,350,41,0,0,4,0],[364,27,364,33,0,0,4,0],[378,7,378,39,0,0,4,0],[398,5,398,39,0,0,4,0],[404,7,404,39,0,0,4,0],[405,7,405,73,0,0,4,0]],"ares_send_nolock":[[151,5,151,39,0,0,4,0]],"ares_socket_enable_tfo":[[217,7,219,71,0,0,4,0]],"ares_conn_set_sockaddr":[[92,5,92,37,0,0,4,0]],"server_increment_failures":[[135,26,135,47,0,0,4,0]]},"library_calls":["ares_library_init","ares_library_cleanup","ares_library_cleanup","ares_init_options","ares_library_cleanup","ares_parse_soa_reply","ares_parse_srv_reply","ares_set_servers_ports","ares_set_socket_functions","ares_set_local_ip6","ares_set_query_enqueue_cb","ares_dns_record_create","ares_dns_record_query_add","ares_dns_rec_type_tostr","ares_send","ares_gethostbyaddr","ares_fds","ares_process","ares_cancel","ares_free_data","ares_free_data","ares_free_string","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_parse_soa_reply","ares_parse_srv_reply","ares_set_servers_ports","ares_set_socket_functions","ares_set_local_ip6","ares_set_query_enqueue_cb","ares_dns_record_create","ares_dns_record_query_add","ares_dns_rec_type_tostr","ares_send","ares_gethostbyaddr","ares_fds","ares_process","ares_cancel","ares_free_data","ares_free_data","ares_free_string","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Mock socket functions for testing
static ares_socket_t  id_000067_mock_socket(int domain, int type, int protocol, void *user_data) {
    return socket(domain, type, protocol);
}

static int  id_000067_mock_close(ares_socket_t sock, void *user_data) {
    return close(sock);
}

static int  id_000067_mock_connect(ares_socket_t sock, const struct sockaddr *addr, ares_socklen_t addrlen, void *user_data) {
    return connect(sock, addr, addrlen);
}

static ares_ssize_t  id_000067_mock_recvfrom(ares_socket_t sock, void *buf, size_t len, int flags,
                                  struct sockaddr *src_addr, ares_socklen_t *addrlen, void *user_data) {
    return recvfrom(sock, buf, len, flags, src_addr, addrlen);
}

static ares_ssize_t  id_000067_mock_sendv(ares_socket_t sock, const struct iovec *iov, int iovcnt, void *user_data) {
    // Simple implementation - send first vector only for testing
    return send(sock, iov[0].iov_base, iov[0].iov_len, 0);
}

// Query enqueue callback
static void  id_000067_query_enqueue_cb(void *user_data) {
    // Do nothing for fuzzing
}

// Socket configuration callback
static void  id_000067_sock_config_cb(ares_socket_t socket_fd, int type, void *user_data) {
    // Do nothing for fuzzing
}

// Callback for ares_send
static void  id_000067_send_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    // Do nothing for fuzzing
}

// Callback for ares_gethostbyaddr
static void  id_000067_host_callback(void *arg, int status, int timeouts, const struct hostent *hostent) {
    // Do nothing for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<6) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // 1. Initialize c-ares library
    int init_result = ares_library_init(ARES_LIB_INIT_ALL);
    if (init_result != ARES_SUCCESS) {
        // Library initialization failed, skip fuzzing
        return 0;
    }

    // Create a FILE* from input data for potential use
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        ares_library_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 2. Initialize channel with options
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_USEVC; // Use TCP only
    options.timeout = 2000; // 2 second timeout
    options.tries = 2;
    
    int optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        ares_library_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 3. Parse SOA reply from input data if we have enough data
    struct ares_soa_reply *soa_reply = nullptr;
    if (size > 0) {
        // Use the input data as a DNS reply buffer
        ares_parse_soa_reply(data, (int)(size > INT_MAX ? INT_MAX : size), &soa_reply);
    }

    // 4. Parse SRV reply from input data if we have enough data
    struct ares_srv_reply *srv_reply = nullptr;
    if (size > 0) {
        // Use the input data as a DNS reply buffer
        ares_parse_srv_reply(data, (int)(size > INT_MAX ? INT_MAX : size), &srv_reply);
    }

    // 5. Set server ports (using a dummy server)
    struct ares_addr_port_node server;
    memset(&server, 0, sizeof(server));
    server.next = nullptr;
    server.family = AF_INET;
    server.addr.addr4.s_addr = htonl(0x08080808); // 8.8.8.8
    server.udp_port = 53;
    server.tcp_port = 53;
    
    ares_set_servers_ports(channel, &server);

    // 6. Set socket functions
    struct ares_socket_functions funcs;
    funcs.asocket =  id_000067_mock_socket;
    funcs.aclose =  id_000067_mock_close;
    funcs.aconnect =  id_000067_mock_connect;
    funcs.arecvfrom =  id_000067_mock_recvfrom;
    funcs.asendv =  id_000067_mock_sendv;
    
    ares_set_socket_functions(channel, &funcs, nullptr);

    // 7. Set local IPv6 address (dummy address)
    unsigned char local_ip6[16] = {0x20, 0x01, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    ares_set_local_ip6(channel, local_ip6);

    // 8. Set query enqueue callback
    ares_set_query_enqueue_cb(channel,  id_000067_query_enqueue_cb, nullptr);

    // 9. Create DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, fuzz_uint16_t_1, 
                                                  ARES_FLAG_QR | ARES_FLAG_AA,
                                                  ARES_OPCODE_QUERY,
                                                  ARES_RCODE_NOERROR);
    if (status == ARES_SUCCESS && dnsrec) {
        // Add a query to the DNS record if we have enough data
        if (size > 10) {
            // Use first 10 bytes as a dummy domain name (null-terminated)
            char domain[11];
            memcpy(domain, data, size > 10 ? 10 : size);
            domain[10] = '\0';
            
            ares_dns_record_query_add(dnsrec, domain, ARES_REC_TYPE_A, ARES_CLASS_IN);
        }
    }

    // 10. Get DNS record type as string
    const char *type_str = ares_dns_rec_type_tostr(ARES_REC_TYPE_A);
    // Write type string to output file
    if (type_str) {
        fwrite(type_str, strlen(type_str), 1, out_file);
    }

    // 11. Send DNS query
    if (size > 0) {
        // Use input data as query buffer
        unsigned short qid = 0;
        ares_send(channel, data, (int)(size > 1024 ? 1024 : size), 
                  id_000067_send_callback, nullptr);
    }

    // 12. Perform reverse DNS lookup (using dummy address)
    struct in_addr addr;
    addr.s_addr = htonl(0x08080808); // 8.8.8.8
    ares_gethostbyaddr(channel, &addr, sizeof(addr), fuzz_int32_t_2,  id_000067_host_callback, nullptr);

    // 13. Process pending requests
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    if (nfds > 0) {
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        
        select(nfds, &read_fds, &write_fds, nullptr, &tv);
        ares_process(channel, &read_fds, &write_fds);
    }

    // 14. Cancel any pending requests
    ares_cancel(channel);

    // 15. Free allocated data
    if (soa_reply) {
        ares_free_data(soa_reply);
    }
    
    if (srv_reply) {
        ares_free_data(srv_reply);
    }

    // 16. Free any strings (example)
    char *test_str = strdup("test");
    if (test_str) {
        ares_free_string(test_str);
    }

    // 17. Destroy DNS record
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }

    // 18. Destroy channel
    ares_destroy(channel);

    // Clean up files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // 19. Clean up library
    ares_library_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
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
//<ID> 1241
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 10
//<Quality> {"density":11,"unique_branches":{"ares_dns_get_opt_rr":[[1543,9,1543,54,0,0,4,1]],"ares_addrpnode_to_sconfig_llist":[[906,9,906,32,0,0,4,0],[906,36,906,60,0,0,4,1],[916,9,916,32,0,0,4,1],[919,16,919,48,0,0,4,0]],"ares_send_query":[[1395,7,1395,32,0,0,4,0]],"ares_check_cleanup_conns":[[119,11,119,44,0,0,4,1],[129,11,129,22,0,0,4,0]],"ares_socket_configure":[[277,14,278,53,0,0,4,0],[288,18,288,51,0,0,4,1]]},"library_calls":["ares_library_init","ares_library_cleanup","ares_init_options","ares_library_cleanup","ares_set_socket_functions","ares_set_local_ip6","ares_set_query_enqueue_cb","ares_set_servers_ports","ares_parse_soa_reply","ares_free_data","ares_parse_srv_reply","ares_free_data","ares_dns_record_create","ares_dns_record_destroy","ares_dns_rec_type_tostr","ares_send","ares_gethostbyaddr","ares_fds","ares_process","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_socket_functions","ares_set_local_ip6","ares_set_query_enqueue_cb","ares_set_servers_ports","ares_parse_soa_reply","ares_free_data","ares_parse_srv_reply","ares_free_data","ares_dns_record_create","ares_dns_record_destroy","ares_dns_rec_type_tostr","ares_send","ares_gethostbyaddr","ares_fds","ares_process","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Dummy socket functions for the fuzzer
extern "C" {
static ares_socket_t dummy_asocket(int domain, int type, int protocol, void *user_data) {
    return socket(domain, type, protocol);
}

static int dummy_aclose(ares_socket_t sockfd, void *user_data) {
    return close(sockfd);
}

static int dummy_aconnect(ares_socket_t sockfd, const struct sockaddr *addr,
                         ares_socklen_t addrlen, void *user_data) {
    return connect(sockfd, addr, addrlen);
}

static ares_ssize_t dummy_arecvfrom(ares_socket_t sockfd, void *buf, size_t len,
                                   int flags, struct sockaddr *from,
                                   ares_socklen_t *fromlen, void *user_data) {
    return recvfrom(sockfd, buf, len, flags, from, fromlen);
}

static ares_ssize_t dummy_asendv(ares_socket_t sockfd, const struct iovec *iov,
                                int iovcnt, void *user_data) {
    ssize_t total = 0;
    for (int i = 0; i < iovcnt; i++) {
        ssize_t sent = send(sockfd, iov[i].iov_base, iov[i].iov_len, 0);
        if (sent < 0) return -1;
        total += sent;
    }
    return total;
}
}

// Callback functions
static void  id_000066_dummy_callback(void *arg, int status, int timeouts,
                          const unsigned char *abuf, int alen) {
    // Do nothing for fuzzing
}

static void  id_000066_dummy_host_callback(void *arg, int status, int timeouts,
                               const struct hostent *host) {
    // Do nothing for fuzzing
}

static void  id_000066_dummy_query_enqueue_cb(void *arg) {
    // Do nothing for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* f_data, size_t f_size) {
	if(f_size<6) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create FILE* for input data if needed
    FILE *in_file = nullptr;
    if (size > 0) {
        in_file = fmemopen((void *)data, size, "rb");
        if (!in_file) {
            ares_library_cleanup();
            assert_file_closed(&in_file);
	return 0;
        }
    }

    // Initialize channel with options
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    options.flags = ARES_FLAG_STAYOPEN;
    options.timeout = 2000;
    options.tries = 3;
    
    int optmask = ARES_OPT_FLAGS | ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        if (in_file) assert_file_closed(&in_file);;
        ares_library_cleanup();
        return 0;
    }

    // Set up dummy socket functions
    struct ares_socket_functions socket_funcs;
    socket_funcs.asocket = dummy_asocket;
    socket_funcs.aclose = dummy_aclose;
    socket_funcs.aconnect = dummy_aconnect;
    socket_funcs.arecvfrom = dummy_arecvfrom;
    socket_funcs.asendv = dummy_asendv;
    ares_set_socket_functions(channel, &socket_funcs, nullptr);

    // Set local IPv6 address (dummy ::1)
    unsigned char local_ip6[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
    ares_set_local_ip6(channel, local_ip6);

    // Set query enqueue callback
    ares_set_query_enqueue_cb(channel,  id_000066_dummy_query_enqueue_cb, nullptr);

    // Set dummy servers (localhost)
    struct ares_addr_port_node servers[2];
    memset(servers, 0, sizeof(servers));
    
    // IPv4 server
    servers[0].family = AF_INET;
    servers[0].addr.addr4.s_addr = htonl(INADDR_LOOPBACK);
    servers[0].udp_port = 53;
    servers[0].tcp_port = 53;
    
    // IPv6 server
    servers[1].family = AF_INET6;
    memset(&servers[1].addr.addr6, 0, 16);
    servers[1].addr.addr6._S6_un._S6_u8[15] = 1;  // ::1
    servers[1].udp_port = 53;
    servers[1].tcp_port = 53;
    servers[0].next = &servers[1];
    
    ares_set_servers_ports(channel, servers);

    // Parse input data as SOA reply if we have enough data
    if (size >= 2) {
        struct ares_soa_reply *soa_out = nullptr;
        int soa_result = ares_parse_soa_reply(data, size, &soa_out);
        if (soa_out) {
            ares_free_data(soa_out);
        }
    }

    // Parse input data as SRV reply if we have enough data
    if (size >= 2) {
        struct ares_srv_reply *srv_out = nullptr;
        int srv_result = ares_parse_srv_reply(data, size, &srv_out);
        if (srv_out) {
            // Free the SRV reply chain
            struct ares_srv_reply *current = srv_out;
            while (current) {
                struct ares_srv_reply *next = current->next;
                ares_free_data(current);
                current = next;
            }
        }
    }

    // Create and destroy DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t create_status = ares_dns_record_create(&dnsrec, fuzz_uint16_t_1, 
                                                        ARES_FLAG_QR | ARES_FLAG_RD,
                                                        ARES_OPCODE_QUERY,
                                                        ARES_RCODE_NOERROR);
    if (create_status == ARES_SUCCESS && dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }

    // Test DNS record type to string conversion
    const char *type_str = ares_dns_rec_type_tostr(ARES_REC_TYPE_A);
    if (type_str) {
        // We have the string, but don't need to free it
    }

    // Send DNS query using input data as query buffer
    if (size > 0) {
        ares_send(channel, data, size > INT_MAX ? INT_MAX : (int)size,
                  id_000066_dummy_callback, nullptr);
    }

    // Perform reverse DNS lookup for localhost
    struct in_addr addr;
    addr.s_addr = htonl(INADDR_LOOPBACK);
    ares_gethostbyaddr(channel, &addr, sizeof(addr), fuzz_int32_t_2,
                       id_000066_dummy_host_callback, nullptr);

    // Process pending events
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    if (nfds > 0) {
        ares_process(channel, &read_fds, &write_fds);
    }

    // Cancel all pending queries
    ares_cancel(channel);

    // Clean up allocated strings (if any were allocated)
    // In this fuzzer, we didn't allocate strings with ares functions that need freeing

    // Destroy channel
    ares_destroy(channel);

    // Clean up input file
    if (in_file) {
        assert_file_closed(&in_file);;
    }

    // Clean up library
    ares_library_cleanup();

    return 0;
}
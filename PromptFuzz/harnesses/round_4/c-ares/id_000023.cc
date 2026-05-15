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
//<ID> 339
//<Prompt> []
/*<Combination>: [
*/
//<score> 441, nr_unique_branch: 195
//<Quality> {"density":9,"unique_branches":{"parse_sort":[[125,7,125,29,0,0,4,0],[125,7,125,29,0,0,4,1],[132,7,132,77,0,0,4,0],[132,7,132,77,0,0,4,1],[138,7,138,29,0,0,4,0],[138,7,138,29,0,0,4,1],[144,7,144,58,0,0,4,0],[144,7,144,58,0,0,4,1],[149,7,149,63,0,0,4,0],[149,7,149,63,0,0,4,1],[160,9,161,10,0,0,4,0],[160,9,161,10,0,0,4,1],[167,9,167,31,0,0,4,1],[171,9,171,32,0,0,4,0],[171,9,171,32,0,0,4,1],[174,11,174,19,0,0,4,1],[174,23,174,33,0,0,4,0],[174,23,174,33,0,0,4,1],[177,11,177,38,0,0,4,0],[177,42,177,51,0,0,4,0],[177,42,177,51,0,0,4,1],[188,11,188,62,0,0,4,0],[188,11,188,62,0,0,4,1],[205,7,205,29,0,0,4,0],[205,7,205,29,0,0,4,1]],"server_sort_cb":[[99,7,99,48,0,0,4,0],[102,7,102,48,0,0,4,0],[105,7,105,24,0,0,4,0]],"ares_conn_set_sockaddr":[[97,5,97,17,0,0,4,1],[108,5,108,18,0,0,4,0],[110,11,110,49,0,0,4,1]],"ares_search_dnsrec":[[479,7,479,22,0,0,4,1],[479,26,479,40,0,0,4,1],[479,44,479,60,0,0,4,1]],"process_read":[[676,7,676,19,0,0,4,1],[687,7,687,29,0,0,4,0]],"ares_socket_deref_error":[[79,5,79,22,0,0,4,0]],"ares_fds":[[67,11,67,27,0,0,4,1]],"ares_send_query":[[1301,7,1301,28,0,0,4,0],[1306,7,1306,19,0,0,4,1],[1331,5,1331,22,0,0,4,1],[1341,5,1341,27,0,0,4,0],[1345,11,1345,34,0,0,4,1],[1391,7,1391,26,0,0,4,1]],"ares_socket_write":[[145,7,145,14,0,0,4,0]],"channel_socket_list":[[309,61,309,73,0,0,4,0],[315,11,315,38,0,0,4,1],[320,11,320,33,0,0,4,1]],"ares_conn_write":[[205,7,205,35,0,0,4,0],[218,7,218,35,0,0,4,1],[225,14,225,45,0,0,4,1]],"ares_addrpnode_to_sconfig_llist":[[906,9,906,32,0,0,4,0],[906,36,906,60,0,0,4,1],[916,9,916,32,0,0,4,1],[919,16,919,48,0,0,4,0]],"ares_parse_soa_reply":[[41,7,41,19,0,0,4,1],[48,7,48,29,0,0,4,0],[48,7,48,29,0,0,4,1],[52,7,52,63,0,0,4,0],[52,7,52,63,0,0,4,1],[57,15,57,70,0,0,4,0],[57,15,57,70,0,0,4,1],[61,9,61,19,0,0,4,1],[67,9,67,51,0,0,4,0],[67,9,67,51,0,0,4,1],[68,9,68,54,0,0,4,0],[97,7,97,18,0,0,4,0],[103,7,103,29,0,0,4,0],[106,9,106,32,0,0,4,0],[106,9,106,32,0,0,4,1]],"handle_conn_error":[[958,7,958,23,0,0,4,0],[960,15,960,49,0,0,4,1]],"ares_process":[[355,7,355,18,0,0,4,0],[357,9,357,23,0,0,4,1],[362,15,362,30,0,0,4,0],[364,9,364,17,0,0,4,0],[370,9,370,18,0,0,4,0]],"ares_close_sockets":[[77,10,77,69,0,0,4,0]],"timeadd":[[187,7,187,27,0,0,4,0]],"ares_dns_write_rr_opt":[[653,15,653,67,0,0,4,0],[662,9,662,31,0,0,4,1],[668,9,668,31,0,0,4,1],[673,9,673,12,0,0,4,0],[673,16,673,23,0,0,4,0],[675,11,675,33,0,0,4,1]],"ares_slist_node_find":[[346,11,346,17,0,0,4,0],[357,14,357,26,0,0,4,1],[360,9,360,16,0,0,4,1],[373,10,373,31,0,0,4,0],[373,35,373,75,0,0,4,1]],"ares_dns_rr_set_opt":[[1411,7,1411,18,0,0,4,0],[1413,9,1413,21,0,0,4,1],[1421,7,1421,29,0,0,4,1]],"ares_query":[[138,7,138,22,0,0,4,1],[143,7,143,19,0,0,4,1]],"ares_check_cleanup_conns":[[106,11,106,48,0,0,4,0]],"ares_timedout":[[168,7,168,15,0,0,4,1],[171,7,171,15,0,0,4,0]],"ares_buf_ensure_space":[[170,7,170,36,0,0,4,0]],"ares_socket_recvfrom":[[194,7,194,13,0,0,4,1],[199,7,199,14,0,0,4,1]],"read_conn_packets":[[461,9,461,44,0,0,4,0],[462,9,462,62,0,0,4,1],[471,9,471,20,0,0,4,1],[480,9,480,37,0,0,4,0],[482,11,482,46,0,0,4,0],[510,7,510,35,0,0,4,0],[510,39,510,70,0,0,4,0]],"ares_conn_flush":[[280,9,280,37,0,0,4,0],[281,11,281,42,0,0,4,0],[303,7,303,29,0,0,4,1]],"ares_sockaddr_to_ares_addr":[[306,7,306,37,0,0,4,1],[322,7,322,38,0,0,4,0],[331,9,331,13,0,0,4,1]],"ares_search_int":[[371,7,371,29,0,0,4,1]],"ares_addr_equal":[[266,7,266,37,0,0,4,1],[271,5,271,17,0,0,4,0],[271,5,271,17,0,0,4,1],[272,11,273,49,0,0,4,0],[277,5,277,18,0,0,4,0],[277,5,277,18,0,0,4,1],[281,11,283,63,0,0,4,0],[287,5,287,12,0,0,4,1]],"ares_query_timeout_cmp_cb":[[77,7,77,40,0,0,4,0],[77,7,77,40,0,0,4,1],[80,7,80,40,0,0,4,0],[80,7,80,40,0,0,4,1],[84,7,84,42,0,0,4,0],[84,7,84,42,0,0,4,1],[87,7,87,42,0,0,4,0]],"ares_llist_node_detach":[[275,7,275,17,0,0,4,0],[283,7,283,25,0,0,4,1]],"ares_buf_create_const":[[61,23,61,36,0,0,4,0]],"ares_calc_query_timeout":[[1155,7,1155,17,0,0,4,0],[1172,7,1172,17,0,0,4,0]],"ares_requeue_query":[[980,7,980,26,0,0,4,0]],"ares_dns_get_opt_rr":[[1540,15,1540,71,0,0,4,0],[1543,9,1543,54,0,0,4,0]],"ares_fetch_connection":[[1203,7,1203,19,0,0,4,1],[1209,7,1209,39,0,0,4,1],[1215,7,1215,40,0,0,4,0],[1215,7,1215,40,0,0,4,1],[1220,7,1220,35,0,0,4,1]],"ares_parse_sortlist":[[221,7,221,23,0,0,4,1],[221,27,221,40,0,0,4,1],[221,44,221,55,0,0,4,1],[225,7,225,24,0,0,4,1],[233,7,233,18,0,0,4,0],[233,7,233,18,0,0,4,1],[249,7,249,29,0,0,4,1],[254,15,254,22,0,0,4,0],[254,15,254,22,0,0,4,1],[261,9,261,31,0,0,4,0],[261,9,261,31,0,0,4,1],[261,35,261,59,0,0,4,0],[261,35,261,59,0,0,4,1],[265,9,265,31,0,0,4,0],[265,9,265,31,0,0,4,1],[269,9,269,48,0,0,4,1],[281,7,281,29,0,0,4,0],[281,7,281,29,0,0,4,1]],"ip_natural_mask":[[78,7,78,31,0,0,4,0],[78,7,78,31,0,0,4,1],[83,7,83,17,0,0,4,0],[83,7,83,17,0,0,4,1],[87,7,87,17,0,0,4,0],[87,7,87,17,0,0,4,1]],"ares_socket_configure":[[271,7,271,24,0,0,4,1],[276,14,276,32,0,0,4,0],[277,14,278,53,0,0,4,1]],"ares_set_sortlist":[[587,7,587,15,0,0,4,1],[593,7,593,29,0,0,4,0],[593,7,593,29,0,0,4,1],[593,33,593,41,0,0,4,0],[593,33,593,41,0,0,4,1],[594,9,594,26,0,0,4,1]],"process_timeouts":[[704,10,704,77,0,0,4,0],[710,9,710,45,0,0,4,0]],"ares_conn_from_fd":[[506,7,506,19,0,0,4,1]],"ares_destroy":[[74,10,74,22,0,0,4,0]],"ares_requeue_queries":[[39,10,39,71,0,0,4,0]],"sortlist_append":[[100,7,100,22,0,0,4,1]],"ares_conn_read":[[51,7,51,42,0,0,4,0],[62,9,62,37,0,0,4,1],[73,7,73,35,0,0,4,1]],"ares_cookie_apply":[[305,7,305,17,0,0,4,1],[310,7,310,39,0,0,4,1],[316,7,316,45,0,0,4,1],[324,7,324,47,0,0,4,1],[337,7,337,43,0,0,4,0],[337,7,337,43,0,0,4,1],[344,8,344,46,0,0,4,0],[346,7,346,59,0,0,4,1],[352,7,352,45,0,0,4,1],[361,7,361,25,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_set_sortlist","ares_set_servers_ports","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_set_opt","ares_query","ares_search_dnsrec","ares_parse_soa_reply","ares_fds","ares_process","ares_free_data","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_sortlist","ares_set_servers_ports","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_set_opt","ares_query","ares_search_dnsrec","ares_parse_soa_reply","ares_fds","ares_process","ares_free_data","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// c-ares headers
extern "C" {
}

// Custom callback functions
static void  id_000023_query_callback(void* arg, int status, int timeouts, 
                          const unsigned char* abuf, int alen) {
    // Do nothing - just needed for ares_query
}

static void  id_000023_search_callback(void* arg, ares_status_t status, size_t timeouts,
                           const ares_dns_record_t* dnsrec) {
    // Do nothing - just needed for ares_search_dnsrec
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}




    // Ensure minimum size for operations
    if (size < 100) {
	return 0;
	}
	
    
    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Create channel
    ares_channel_t* channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Create FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Part 1: Set sortlist using first 50 bytes as string
    char sortlist_str[51];
    size_t read_size = fread(sortlist_str, 1, 50, in_file);
    sortlist_str[read_size] = '\0';
    rewind(in_file);
    
    // Call ares_set_sortlist
    ares_set_sortlist(channel, sortlist_str);
    
    // Part 2: Set servers using parsed address data
    struct ares_addr_port_node servers[2];
    servers[0].next = &servers[1];
    servers[0].family = AF_INET;
    
    // Read IP address from input
    uint32_t ip_addr;
    if (fread(&ip_addr, sizeof(ip_addr), 1, in_file) == 1) {
        servers[0].addr.addr4.s_addr = ip_addr;
    }
    servers[0].udp_port = 53;
    servers[0].tcp_port = 53;
    
    servers[1].next = nullptr;
    servers[1].family = AF_INET6;
    servers[1].udp_port = 53;
    servers[1].tcp_port = 53;
    
    // Call ares_set_servers_ports
    ares_set_servers_ports(channel, servers);
    
    // Part 3: Create DNS record and set OPT options
    ares_dns_record_t* dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_1, 
                               ARES_FLAG_RD | ARES_FLAG_AA,
                               ARES_OPCODE_QUERY,
                               ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        
        // Add a query
        ares_dns_record_query_add(dnsrec, fuzz_str_2, 
                                 ARES_REC_TYPE_A, ARES_CLASS_IN);
        
        // Create OPT RR
        ares_dns_rr_t* opt_rr = nullptr;
        if (ares_dns_record_rr_add(&opt_rr, dnsrec, ARES_SECTION_ADDITIONAL,
                                  fuzz_str_3, ARES_REC_TYPE_OPT, 
                                  ARES_CLASS_IN, fuzz_uint32_t_4) == ARES_SUCCESS) {
            
            // Read OPT value from input
            unsigned char opt_val[100];
            size_t opt_len = fread(opt_val, 1, 100, in_file);
            
            // Call ares_dns_rr_set_opt
            ares_dns_rr_set_opt(opt_rr, ARES_RR_OPT_OPTIONS,
                              fuzz_uint16_t_5, opt_val, opt_len);
        }
    }
    
    // Part 4: Perform DNS query
    // Call ares_query
    ares_query(channel, fuzz_str_6, ARES_CLASS_IN, 
              ARES_REC_TYPE_A,  id_000023_query_callback, nullptr);
    
    // Part 5: Perform search with DNS record
    if (dnsrec) {
        // Call ares_search_dnsrec
        ares_search_dnsrec(channel, dnsrec,  id_000023_search_callback, nullptr);
    }
    
    // Part 6: Parse SOA reply from remaining input data
    struct ares_soa_reply* soa_out = nullptr;
    
    // Get remaining data size and position
    long current_pos = ftell(in_file);
    if (current_pos < 0) current_pos = 0;
    size_t remaining = size - current_pos;
    
    if (remaining > 0) {
        // Allocate buffer for SOA parsing
        unsigned char* soa_buf = (unsigned char*)malloc(remaining);
        if (soa_buf) {
            fread(soa_buf, 1, remaining, in_file);
            
            // Call ares_parse_soa_reply
            ares_parse_soa_reply(soa_buf, remaining, &soa_out);
            
            free(soa_buf);
        }
    }
    
    // Process any pending operations
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    // Get file descriptors to process
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    
    if (nfds > 0) {
        // Process with timeout of 0 to avoid blocking
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        
        // Process any ready operations
        ares_process(channel, &read_fds, &write_fds);
    }
    
    // Clean up resources
    if (soa_out) {
        ares_free_data(soa_out);
    }
    
    if (dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }
    
    assert_file_closed(&in_file);;
    ares_destroy(channel);
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
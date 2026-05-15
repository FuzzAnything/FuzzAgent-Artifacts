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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 140, nr_unique_branch: 78
//<Quality> {"density":5,"unique_branches":{"ares_dns_rr_get_abin_cnt":[[887,7,887,59,0,0,4,1],[892,7,892,19,0,0,4,1]],"ares_parse_txt_reply_int":[[43,7,43,29,0,0,4,0],[43,7,43,29,0,0,4,1],[47,7,47,63,0,0,4,0],[47,7,47,63,0,0,4,1],[52,15,52,70,0,0,4,0],[52,15,52,70,0,0,4,1],[59,9,59,19,0,0,4,1],[66,10,66,52,0,0,4,0],[66,10,66,52,0,0,4,1],[67,10,67,55,0,0,4,0],[67,10,67,55,0,0,4,1],[68,9,68,54,0,0,4,0],[68,9,68,54,0,0,4,1],[74,17,74,24,0,0,4,0],[74,17,74,24,0,0,4,1],[80,26,80,28,0,0,4,0],[81,11,81,27,0,0,4,1],[87,11,87,19,0,0,4,0],[87,11,87,19,0,0,4,1],[95,11,95,13,0,0,4,0],[95,17,95,23,0,0,4,0],[95,17,95,23,0,0,4,1],[102,11,102,32,0,0,4,1],[114,7,114,29,0,0,4,0],[114,7,114,29,0,0,4,1],[115,9,115,17,0,0,4,1]],"ares_servers_remove_stale":[[734,9,734,51,0,0,4,0]],"ares_set_servers_ports":[[1252,7,1252,22,0,0,4,1],[1257,7,1257,29,0,0,4,1]],"ares_array_at_const":[[77,7,77,18,0,0,4,1],[77,22,77,37,0,0,4,1]],"ares_slist_calc_level":[[137,19,137,45,0,0,4,0],[137,49,137,66,0,0,4,0],[137,49,137,66,0,0,4,1]],"ares_server_find":[[591,56,591,68,0,0,4,0],[595,9,595,50,0,0,4,0]],"ares_slist_node_destroy":[[453,7,453,19,0,0,4,1],[460,7,460,18,0,0,4,0],[460,22,460,38,0,0,4,0]],"ares_set_local_dev":[[570,7,570,22,0,0,4,1]],"ares_dns_opcode_isvalid":[[33,5,33,28,0,0,4,0]],"ares_malloc_data":[[150,5,150,31,0,0,4,0]],"server_destroy_cb":[[116,7,116,19,0,0,4,1]],"ares_server_in_newconfig":[[703,47,703,59,0,0,4,1],[707,9,707,50,0,0,4,0]],"ares_addrpnode_to_sconfig_llist":[[898,7,898,16,0,0,4,1],[902,24,902,36,0,0,4,0],[902,24,902,36,0,0,4,1],[906,9,906,32,0,0,4,1],[911,9,911,24,0,0,4,1],[916,9,916,32,0,0,4,0],[927,9,927,51,0,0,4,1]],"ares_dns_multistring_cnt":[[169,7,169,19,0,0,4,1]],"ares_free_data":[[85,7,85,33,0,0,4,0]],"ares_servers_update":[[811,7,811,54,0,0,4,0],[820,7,820,21,0,0,4,0]],"ares_dns_rr_get_name":[[507,7,507,17,0,0,4,1]],"ares_dns_rr_get_abin":[[905,7,905,59,0,0,4,1],[910,7,910,19,0,0,4,1]],"ares_sconfig_get_port":[[575,7,575,16,0,0,4,1],[579,7,579,16,0,0,4,1]],"ares_parse_txt_reply_ext":[[139,7,139,15,0,0,4,1]],"ares_slist_coin_flip":[[94,7,94,27,0,0,4,1],[102,10,102,55,0,0,4,0]],"ares_slist_node_pop":[[265,11,265,17,0,0,4,1],[272,9,272,30,0,0,4,1]],"ares_set_local_ip4":[[548,7,548,22,0,0,4,1]],"ares_addr_match":[[82,35,83,73,0,0,4,1],[87,7,87,32,0,0,4,1]],"ares_dns_multistring_get":[[181,7,181,19,0,0,4,1],[181,23,181,34,0,0,4,1],[186,7,186,19,0,0,4,1]],"ares_slist_node_push":[[153,25,153,46,0,0,4,0],[154,9,154,55,0,0,4,1],[183,9,183,30,0,0,4,0],[187,11,187,17,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_parse_txt_reply_ext","ares_set_local_dev","ares_set_servers_ports","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_addr","ares_dns_rr_get_name","ares_dns_record_destroy","ares_free_data","ares_set_local_ip4","ares_process","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_parse_txt_reply_ext","ares_set_local_dev","ares_set_servers_ports","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_addr","ares_dns_rr_get_name","ares_dns_record_destroy","ares_free_data","ares_set_local_ip4","ares_process","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Initialize channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 1. Use the input data as DNS response to parse TXT reply
    struct ares_txt_ext *txt_out = nullptr;
    int parse_result = ares_parse_txt_reply_ext(data, size, &txt_out);
    
    // If parsing successful, use the parsed data
    if (parse_result == ARES_SUCCESS && txt_out != nullptr) {
        // 2. Set local device name using first TXT record if available
        if (txt_out->txt != nullptr && txt_out->length > 0) {
            // Use first TXT record as device name (truncate if too long)
            char dev_name[256] = {0};
            size_t copy_len = txt_out->length < 255 ? txt_out->length : 255;
            memcpy(dev_name, txt_out->txt, copy_len);
            dev_name[copy_len] = '\0';
            ares_set_local_dev(channel, dev_name);
        }
        
        // 3. Set servers using parsed data
        if (size >= sizeof(struct in_addr)) {
            struct ares_addr_port_node server;
            memset(&server, 0, sizeof(server));
            server.next = nullptr;
            server.family = AF_INET;
            
            // Use first 4 bytes of input as IP address
            memcpy(&server.addr.addr4, data, sizeof(struct in_addr));
            server.udp_port = 53;
            server.tcp_port = 53;
            
            ares_set_servers_ports(channel, &server);
        }
        
        // 4. Create a DNS resource record and set various fields
        ares_dns_record_t *dnsrec = nullptr;
        ares_dns_record_create(&dnsrec, 12345, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
        
        if (dnsrec != nullptr) {
            // Add an A record
            ares_dns_rr_t *rr = nullptr;
            ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                                  "example.com", ARES_REC_TYPE_A, 
                                  ARES_CLASS_IN, 300);
            
            if (rr != nullptr) {
                // Set address for A record using input data
                struct in_addr addr;
                if (size >= sizeof(struct in_addr)) {
                    memcpy(&addr, data, sizeof(struct in_addr));
                    ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr);
                }
                
                // Get and use the RR name
                const char *rr_name = ares_dns_rr_get_name(rr);
                if (rr_name != nullptr) {
                    // Could use the name for something
                }
                
                // Clean up DNS record
                ares_dns_record_destroy(dnsrec);
            }
        }
        
        // Free TXT reply data
        ares_free_data(txt_out);
    }

    // 5. Set local IPv4 address using input data
    if (size >= sizeof(uint32_t)) {
        uint32_t local_ip;
        memcpy(&local_ip, data, sizeof(uint32_t));
        ares_set_local_ip4(channel, local_ip);
    }

    // Process any pending operations (with empty fd sets since we don't have real sockets)
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    ares_process(channel, &read_fds, &write_fds);

    // Create a file from input data to simulate file operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file != nullptr) {
        // Read a small amount from the file
        char buf[16];
        fread(buf, 1, sizeof(buf) < size ? sizeof(buf) : size, in_file);
        assert_file_closed(&in_file);;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file != nullptr) {
        fwrite(data, 1, size < 1024 ? size : 1024, out_file);
        assert_file_closed(&out_file);;
    }

    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
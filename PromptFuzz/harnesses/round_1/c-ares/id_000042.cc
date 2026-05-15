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
//<ID> 871
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.5, nr_unique_branch: 2
//<Quality> {"density":7,"unique_branches":{"ares_set_socket_functions_ex":[[115,9,115,31,0,0,4,0]],"ares_sysconfig_apply":[[520,29,520,67,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_set_servers_csv","ares_set_servers_ports_csv","ares_set_socket_functions_ex","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_name","ares_dns_class_fromstr","ares_dns_record_rr_add","ares_dns_record_destroy","ares_reinit","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_servers_csv","ares_set_servers_ports_csv","ares_set_socket_functions_ex","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_name","ares_dns_class_fromstr","ares_dns_record_rr_add","ares_dns_record_destroy","ares_reinit","ares_destroy","ares_library_cleanup"],"visited":1}
/**/



extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<27) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Part 1: Use a portion of input as CSV server list
    if (size > 0) {
        size_t csv_len = size > 100 ? 100 : size;
        char *servers_csv = (char *)malloc(csv_len + 1);
        if (!servers_csv) {
            ares_destroy(channel);
            ares_library_cleanup();
            return 0;
        }
        memcpy(servers_csv, data, csv_len);
        servers_csv[csv_len] = '\0';
        
        // Call ares_set_servers_csv with input data
        int result = ares_set_servers_csv(channel, servers_csv);
        (void)result; // Ignore result for fuzzing
        
        free(servers_csv);
    }

    // Part 2: Create and set servers using ares_addr_port_node
    if (size >= sizeof(struct ares_addr_port_node)) {
        struct ares_addr_port_node *server_node = 
            (struct ares_addr_port_node *)malloc(sizeof(struct ares_addr_port_node));
        if (server_node) {
            memset(server_node, 0, sizeof(struct ares_addr_port_node));
            server_node->next = nullptr;
            server_node->family = AF_INET;
            
            // Use input bytes for address if available
            if (size >= sizeof(struct ares_addr_port_node) + 4) {
                memcpy(&server_node->addr.addr4, data + sizeof(struct ares_addr_port_node), 4);
            } else {
                server_node->addr.addr4.s_addr = htonl(0x7F000001); // 127.0.0.1
            }
            server_node->udp_port = 53;
            server_node->tcp_port = 53;
            
            // Call ares_set_servers_ports (note: this is actually ares_set_servers_ports_csv in API list)
            // We'll use the CSV version for simplicity
            char ports_csv[256];
            snprintf(ports_csv, sizeof(ports_csv), "127.0.0.1");
            ares_set_servers_ports_csv(channel, ports_csv);
            
            free(server_node);
        }
    }

    // Part 3: Set custom socket functions using input data
    struct ares_socket_functions_ex socket_funcs;
    memset(&socket_funcs, 0, sizeof(socket_funcs));
    socket_funcs.version = 1;
    
    // We'll use the input data to set some function pointers (dummy implementations)
    // For fuzzing, we just need to call the API
    ares_status_t status = ares_set_socket_functions_ex(channel, &socket_funcs, nullptr);
    (void)status;

    // Part 4: Parse DNS record from input data
    ares_dns_record_t *dnsrec = nullptr;
    if (size > 0) {
        // Try to parse input as DNS data
        status = ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
        
        if (status == ARES_SUCCESS && dnsrec) {
            // Call ares_dns_record_get_rcode
            ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
            (void)rcode;
            
            // Try to get a resource record name
            size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
            if (rr_count > 0) {
                const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, 0);
                if (rr) {
                    // Call ares_dns_rr_get_name
                    const char *name = ares_dns_rr_get_name(rr);
                    (void)name;
                    
                    // Try to parse DNS class from string using input data
                    if (size > 10) {
                        char class_str[11];
                        size_t copy_len = size > 10 ? 10 : size;
                        memcpy(class_str, data, copy_len);
                        class_str[copy_len] = '\0';
                        
                        ares_dns_class_t dns_class;
                        // Call ares_dns_class_fromstr
                        ares_bool_t class_result = ares_dns_class_fromstr(&dns_class, class_str);
                        (void)class_result;
                        
                        // Add a new resource record to the DNS record
                        ares_dns_rr_t *new_rr = nullptr;
                        
                        // Use the parsed class or default
                        ares_dns_class_t use_class = class_result ? dns_class : ARES_CLASS_IN;
                        
                        // Call ares_dns_record_rr_add
                        ares_status_t add_status = ares_dns_record_rr_add(
                            &new_rr, dnsrec, ARES_SECTION_ANSWER,
                            fuzz_str_2, ARES_REC_TYPE_A, use_class, fuzz_uint32_t_3);
                        (void)add_status;
                    }
                }
            }
            
            // Clean up DNS record
            ares_dns_record_destroy(dnsrec);
        }
    }

    // Part 5: Reinitialize the channel
    // Call ares_reinit
    status = ares_reinit(channel);
    (void)status;

    // Cleanup
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
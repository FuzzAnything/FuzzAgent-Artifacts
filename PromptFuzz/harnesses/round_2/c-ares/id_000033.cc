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
//<ID> 780
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 13
//<Quality> {"density":12,"unique_branches":{"lookup_service":[[227,13,227,16,0,0,4,1]],"ares_getaddrinfo_int":[[634,11,634,16,0,0,4,0],[635,13,635,53,0,0,4,0],[635,13,635,53,0,0,4,1]],"ares_send_nolock":[[122,7,122,44,0,0,4,0]],"numeric_service_to_port":[[589,7,589,17,0,0,4,0],[589,7,589,17,0,0,4,1],[589,21,589,33,0,0,4,0],[589,21,589,33,0,0,4,1],[589,37,589,49,0,0,4,0],[589,37,589,49,0,0,4,1]],"ares_gethostbyname_file_int":[[312,7,312,29,0,0,4,0],[332,32,332,55,0,0,4,1]]},"library_calls":["ares_dns_parse","ares_dns_record_destroy","ares_dns_record_set_id","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_get","ares_dns_rr_set_bin","ares_dns_record_destroy","ares_init","ares_dns_record_destroy","ares_set_servers_ports_csv","ares_freeaddrinfo","ares_freeaddrinfo","ares_getaddrinfo","ares_gethostbyname_file","ares_free_hostent","ares_dns_addr_to_ptr","ares_free_string","ares_dns_record_destroy","ares_destroy"],"critical_calls":["ares_dns_parse","ares_dns_record_set_id","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_get","ares_dns_rr_set_bin","ares_dns_record_destroy","ares_init","ares_set_servers_ports_csv","ares_freeaddrinfo","ares_freeaddrinfo","ares_getaddrinfo","ares_gethostbyname_file","ares_free_hostent","ares_dns_addr_to_ptr","ares_free_string","ares_dns_record_destroy","ares_destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Parse DNS response data into DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, 0, &dnsrec);
    if (status != ARES_SUCCESS || !dnsrec) {
        if (dnsrec) ares_dns_record_destroy(dnsrec);
        return 0;
    }

    // 2. Set DNS record ID using input data
    unsigned short id = 0;
    if (size >= 2) {
        id = (data[0] << 8) | data[1];
    }
    ares_dns_record_set_id(dnsrec, id);

    // 3. Get resource record from DNS record
    size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
    if (rr_count > 0) {
        const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(dnsrec, ARES_SECTION_ANSWER, 0);
        if (rr) {
            // 4. Set binary data in a new RR (simulated)
            // Create a minimal new RR for demonstration
            ares_dns_record_t *new_dnsrec = nullptr;
            status = ares_dns_record_create(&new_dnsrec, 12345, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
            if (status == ARES_SUCCESS && new_dnsrec) {
                // Add a new RR to the answer section
                ares_dns_record_query_add(new_dnsrec, "example.com", ARES_REC_TYPE_A, ARES_CLASS_IN);
                ares_dns_rr_t *new_rr = ares_dns_record_rr_get(new_dnsrec, ARES_SECTION_ANSWER, 0);
                if (new_rr && size > 0) {
                    // Use first min(size, 4) bytes as binary data for A record
                    size_t bin_len = size > 4 ? 4 : size;
                    ares_dns_rr_set_bin(new_rr, ARES_RR_A_ADDR, data, bin_len);
                }
                ares_dns_record_destroy(new_dnsrec);
            }
        }
    }

    // 5. Initialize channel and set servers
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        return 0;
    }

    // Create CSV server string from input data
    char *server_csv = nullptr;
    if (size > 0) {
        // Use input data as server address (limit to reasonable size)
        size_t csv_len = size > 100 ? 100 : size;
        server_csv = (char*)malloc(csv_len + 1);
        if (server_csv) {
            memcpy(server_csv, data, csv_len);
            server_csv[csv_len] = '\0';
            ares_set_servers_ports_csv(channel, server_csv);
            free(server_csv);
        }
    }

    // 6. Perform getaddrinfo lookup
    // Create node and service strings from input
    char *node = nullptr;
    char *service = nullptr;
    if (size >= 10) {
        // Use first half for node, second half for service
        size_t node_len = size/2;
        size_t service_len = size - node_len;
        
        node = (char*)malloc(node_len + 1);
        service = (char*)malloc(service_len + 1);
        
        if (node && service) {
            memcpy(node, data, node_len);
            node[node_len] = '\0';
            memcpy(service, data + node_len, service_len);
            service[service_len] = '\0';
            
            // Callback for getaddrinfo
            auto callback = [](void* arg, int status, int timeouts, struct ares_addrinfo* ai) {
                if (ai) {
                    ares_freeaddrinfo(ai);
                }
            };
            
            ares_getaddrinfo(channel, node, service, nullptr, callback, nullptr);
        }
    }

    // 7. Perform gethostbyname file lookup
    struct hostent *host = nullptr;
    if (size > 0) {
        // Create filename from input
        char *filename = (char*)malloc(size + 1);
        if (filename) {
            memcpy(filename, data, size);
            filename[size] = '\0';
            
            // Use input as hostname for lookup
            ares_gethostbyname_file(channel, filename, AF_INET, &host);
            if (host) {
                ares_free_hostent(host);
            }
            free(filename);
        }
    }

    // 8. Convert address to PTR
    struct ares_addr addr;
    if (size >= sizeof(struct in_addr)) {
        addr.family = AF_INET;
        memcpy(&addr.addr.addr4, data, sizeof(struct in_addr));
        char *ptr = ares_dns_addr_to_ptr(&addr);
        if (ptr) {
            ares_free_string(ptr);
        }
    }

    // Cleanup
    ares_dns_record_destroy(dnsrec);
    ares_destroy(channel);
    
    if (node) free(node);
    if (service) free(service);

    return 0;
}
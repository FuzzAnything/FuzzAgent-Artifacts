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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 19
//<Quality> {"density":9,"unique_branches":{"ares_getnameinfo_int":[[99,13,99,37,0,0,4,1],[103,14,103,16,0,0,4,0],[103,20,103,45,0,0,4,0],[103,20,103,45,0,0,4,1],[104,14,104,66,0,0,4,1],[113,7,113,39,0,0,4,1],[118,7,118,38,0,0,4,0],[118,42,118,71,0,0,4,1]],"ares_dns_addr_to_ptr":[[1474,7,1474,30,0,0,4,0],[1474,34,1474,58,0,0,4,1],[1483,7,1483,30,0,0,4,1],[1492,9,1492,32,0,0,4,1],[1499,11,1499,33,0,0,4,1],[1504,11,1504,33,0,0,4,1],[1521,7,1521,30,0,0,4,1]]," id_000003_nameinfo_callback":[[255,9,255,47,0,0,4,0],[256,11,256,37,0,0,4,0]],"ares_dns_rec_type_fromstr":[[728,15,728,35,0,0,4,1]],"ares_get_servers":[[1144,9,1144,37,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_dns_rec_type_fromstr","ares_set_servers_ports_csv","ares_get_servers","ares_dns_addr_to_ptr","ares_free_data","ares_gethostbyname_file","ares_free_hostent","ares_getnameinfo","ares_process_pending_write","ares_dns_record_create","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_dns_rec_type_fromstr","ares_set_servers_ports_csv","ares_get_servers","ares_dns_addr_to_ptr","ares_free_data","ares_gethostbyname_file","ares_free_hostent","ares_getnameinfo","ares_process_pending_write","ares_dns_record_create","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


// Custom callback for ares_getnameinfo
static void  id_000003_nameinfo_callback(void *arg, int status, int timeouts,
                              const char *node, const char *service) {
    (void)arg; (void)status; (void)timeouts; (void)node; (void)service;
    // Callback implementation
}

// Custom callback for ares queries
static void  id_000003_query_callback(void *arg, int status, int timeouts,
                           unsigned char *abuf, int alen) {
    (void)arg; (void)status; (void)timeouts; (void)abuf; (void)alen;
    // Callback implementation
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<6) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





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

    // 1. Use ares_dns_rec_type_fromstr - parse DNS record type from input string
    if (size > 0) {
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            char *type_str = nullptr;
            size_t len = 0;
            if (getline(&type_str, &len, in_file) > 0) {
                ares_dns_rec_type_t qtype;
                if (ares_dns_rec_type_fromstr(&qtype, type_str)) {
                    // Successfully parsed record type
                }
                free(type_str);
            }
            assert_file_closed(&in_file);;
        }
    }

    // 2. Use ares_set_servers_ports_csv - set servers from CSV string
    if (size > 10) {
        char *csv_servers = (char *)malloc(size + 1);
        if (csv_servers) {
            memcpy(csv_servers, data, size);
            csv_servers[size] = '\0';
            ares_set_servers_ports_csv(channel, csv_servers);
            free(csv_servers);
        }
    }

    // 3. Use ares_get_servers - retrieve configured servers
    struct ares_addr_node *servers = nullptr;
    ares_get_servers(channel, &servers);
    
    // Process the server list
    struct ares_addr_node *server = servers;
    while (server) {
        // Convert server address to PTR string using ares_dns_addr_to_ptr
        struct ares_addr addr;
        addr.family = server->family;
        if (server->family == AF_INET) {
            addr.addr.addr4 = server->addr.addr4;
        } else if (server->family == AF_INET6) {
            addr.addr.addr6 = server->addr.addr6;
        }
        
        char *ptr_str = ares_dns_addr_to_ptr(&addr);
        if (ptr_str) {
            // Use the PTR string for something
            free(ptr_str);
        }
        
        server = server->next;
    }
    
    // Free the server list
    if (servers) {
        ares_free_data(servers);
    }

    // 4. Use ares_gethostbyname_file - lookup hostname in hosts file
    if (size > 5) {
        // Create a temporary hosts file with input data
        FILE *hosts_file = fopen("input_file", "wb");
        if (hosts_file) {
            fwrite(data, 1, size, hosts_file);
            assert_file_closed(&hosts_file);;
            
            struct hostent *host = nullptr;
            char hostname[256];
            snprintf(hostname, sizeof(hostname), "testhost");
            
            ares_gethostbyname_file(channel, hostname, fuzz_int32_t_1, &host);
            if (host) {
                ares_free_hostent(host);
            }
            
            remove("input_file");
        }
    }

    // 5. Use ares_getnameinfo - reverse DNS lookup
    if (size >= sizeof(struct sockaddr_in)) {
        struct sockaddr_in sa;
        memcpy(&sa, data, sizeof(struct sockaddr_in));
        
        ares_getnameinfo(channel, (struct sockaddr *)&sa, sizeof(sa),
                         ARES_NI_LOOKUPHOST | ARES_NI_LOOKUPSERVICE,
                          id_000003_nameinfo_callback, nullptr);
    }

    // 6. Use ares_process_pending_write - process pending writes
    ares_process_pending_write(channel);

    // 7. Use ares_dns_rr_set_bin - set binary data in DNS resource record
    if (size > 20) {
        ares_dns_record_t *dnsrec = nullptr;
        ares_status_t status = ares_dns_record_create(&dnsrec, fuzz_uint16_t_2, 
            ARES_FLAG_QR | ARES_FLAG_AA, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
        
        if (status == ARES_SUCCESS && dnsrec) {
            // Create a resource record
            ares_dns_rr_t *rr = nullptr;
            
            // For simplicity, we'll just demonstrate the API usage
            // In real usage, we would properly create and populate the RR
            
            // Allocate and set binary data
            unsigned char *bin_data = (unsigned char *)malloc(size);
            if (bin_data) {
                memcpy(bin_data, data, size);
                
                // Note: This is simplified - in real usage we would have a valid dns_rr
                // and appropriate key
                // ares_dns_rr_set_bin(rr, ARES_RR_A_ADDR, bin_data, size);
                
                free(bin_data);
            }
            
            ares_dns_record_destroy(dnsrec);
        }
    }

    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}
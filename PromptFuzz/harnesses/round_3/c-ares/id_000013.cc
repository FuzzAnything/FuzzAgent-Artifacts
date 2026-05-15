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
//<ID> 248
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"ares_sysconfig_apply":[[520,29,520,67,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_dns_class_fromstr","ares_destroy","ares_library_cleanup","ares_set_servers_csv","ares_dns_record_create","ares_dns_record_query_add","ares_dns_write","ares_free_data","ares_free_data","ares_set_socket_callback","ares_set_server_state_callback","ares_reinit","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_dns_class_fromstr","ares_set_servers_csv","ares_dns_record_create","ares_dns_record_query_add","ares_dns_write","ares_free_data","ares_free_data","ares_set_socket_callback","ares_set_server_state_callback","ares_reinit","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
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

    // 1. Use ares_dns_class_fromstr with input data as string
    ares_dns_class_t dns_class;
    if (size > 0) {
        // Create null-terminated string from input data
        char *class_str = (char*)malloc(size + 1);
        if (!class_str) {
            ares_destroy(channel);
            ares_library_cleanup();
            return 0;
        }
        memcpy(class_str, data, size);
        class_str[size] = '\0';
        
        // Try to parse DNS class from string
        ares_dns_class_fromstr(&dns_class, class_str);
        free(class_str);
    }

    // 2. Use ares_set_servers_csv with input data as CSV string
    if (size > 0) {
        char *servers_csv = (char*)malloc(size + 1);
        if (!servers_csv) {
            ares_destroy(channel);
            ares_library_cleanup();
            return 0;
        }
        memcpy(servers_csv, data, size);
        servers_csv[size] = '\0';
        
        // Try to set servers from CSV
        ares_set_servers_csv(channel, servers_csv);
        free(servers_csv);
    }

    // 3. Create a DNS record and use ares_dns_write
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, 
                                                  fuzz_uint16_t_1,  // ID
                                                  0x0100,  // Flags
                                                  ARES_OPCODE_QUERY, 
                                                  ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Add a query to the DNS record
        status = ares_dns_record_query_add(dnsrec, fuzz_str_2, 
                                           ARES_REC_TYPE_A, 
                                           ARES_CLASS_IN);
        
        if (status == ARES_SUCCESS) {
            // Write DNS record to buffer
            unsigned char *buf = nullptr;
            size_t buf_len = 0;
            status = ares_dns_write(dnsrec, &buf, &buf_len);
            
            if (status == ARES_SUCCESS && buf) {
                // Process the buffer if needed
                ares_free_data(buf);
            }
        }
        
        // Clean up DNS record
        ares_free_data(dnsrec);
    }

    // 4. Set socket callback (dummy implementation)
    ares_set_socket_callback(channel, 
        [](ares_socket_t socket_fd, int type, void *user_data) -> int {
            return 0;
        }, 
        nullptr);

    // 5. Set server state callback (dummy implementation)
    ares_set_server_state_callback(channel,
        [](const char *name, ares_bool_t healthy, int conn_duration, void *user_data) {
            // Do nothing
        },
        nullptr);

    // 6. Reinitialize the channel
    ares_reinit(channel);

    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}
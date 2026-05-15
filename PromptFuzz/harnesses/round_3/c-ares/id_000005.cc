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
//<ID> 95
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 6
//<Quality> {"density":9,"unique_branches":{"ares_hosts_entry_to_hostent":[[943,7,943,29,0,0,4,0],[954,7,954,29,0,0,4,0]],"ares_gethostbyname_file_int":[[317,7,317,29,0,0,4,0]],"ares_set_local_dev":[[570,7,570,22,0,0,4,1]],"ares_hosts_entry_to_addrinfo":[[901,7,901,29,0,0,4,0],[915,7,915,29,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_local_ip6","ares_set_local_dev","ares_parse_srv_reply","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_get","ares_dns_rr_get_type","ares_dns_rr_set_u16","ares_dns_rr_set_u16","ares_dns_record_destroy","ares_free_data","ares_timeout","ares_gethostbyname_file","ares_free_hostent","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_local_ip6","ares_set_local_dev","ares_parse_srv_reply","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_get","ares_dns_rr_get_type","ares_dns_rr_set_u16","ares_dns_rr_set_u16","ares_dns_record_destroy","ares_free_data","ares_timeout","ares_gethostbyname_file","ares_free_hostent","ares_destroy","ares_library_cleanup"],"visited":2}
/*Here's a C++ fuzz driver that uses the specified c-ares APIs to achieve DNS SRV record resolution with custom local configuration:

*/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize channel with default options
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set local IPv6 address (using ::1 as example)
    unsigned char local_ip6[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
    ares_set_local_ip6(channel, local_ip6);

    // Set local device name (using input data for device name)
    if (size > 0) {
        // Create a null-terminated device name from input data
        char *dev_name = (char *)malloc(size + 1);
        if (dev_name) {
            memcpy(dev_name, data, size);
            dev_name[size] = '\0';
            ares_set_local_dev(channel, dev_name);
            free(dev_name);
        }
    }

    // Parse SRV reply from input data
    struct ares_srv_reply *srv_out = nullptr;
    if (size > 0) {
        // Use input data as DNS reply buffer
        ares_parse_srv_reply(data, (int)size, &srv_out);
        
        // If parsing succeeded, process the SRV records
        if (srv_out) {
            struct ares_srv_reply *current = srv_out;
            while (current) {
                // Create a DNS resource record for demonstration
                ares_dns_record_t *dnsrec = nullptr;
                ares_status_t status = ares_dns_record_create(&dnsrec, 
                    (unsigned short)rand(),  // random ID
                    fuzz_uint16_t_2,  // flags
                    ARES_OPCODE_QUERY,  // opcode
                    ARES_RCODE_NOERROR  // rcode
                );
                
                if (status == ARES_SUCCESS && dnsrec) {
                    // Add a query to the DNS record
                    status = ares_dns_record_query_add(dnsrec, 
                        current->host ? current->host : "example.com",
                        ARES_REC_TYPE_SRV,
                        ARES_CLASS_IN
                    );
                    
                    if (status == ARES_SUCCESS) {
                        // Get the first resource record from answer section
                        ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, 
                            ARES_SECTION_ANSWER, fuzz_uint64_t_3);
                        
                        if (rr) {
                            // Get the type of the resource record
                            ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                            
                            // Convert opcode to string for demonstration
                            // Note: ares_dns_opcode_tostr is not in the exported list,
                            // so we'll use a placeholder. In real usage, you'd need
                            // to include the appropriate header.
                            const char *opcode_str = "QUERY"; // Placeholder
                            
                            // We can't use ares_dns_rr_set_str as it's not exported,
                            // but we can use other available setter functions
                            // For demonstration, we'll use available setters
                            ares_status_t set_status = ares_dns_rr_set_u16(rr, 
                                ARES_RR_SRV_PORT,  // Using port field for SRV
                                current->port
                            );
                            
                            // Set priority using available setter
                            if (set_status == ARES_SUCCESS) {
                                ares_dns_rr_set_u16(rr, 
                                    ARES_RR_SRV_PRIORITY,
                                    current->priority
                                );
                            }
                        }
                    }
                    
                    // Clean up DNS record
                    ares_dns_record_destroy(dnsrec);
                }
                
                current = current->next;
            }
            
            // Free SRV reply data
            ares_free_data(srv_out);
        }
    }

    // Get timeout for the channel
    struct timeval maxtv;
    struct timeval tv;
    struct timeval *timeout = ares_timeout(channel, &maxtv, &tv);
    
    // Perform a host lookup (using ares_gethostbyname_file as ares_gethostbyname is not exported)
    // Note: ares_gethostbyname is not in the exported list, so we'll use ares_gethostbyname_file
    // which is available
    if (size > 0) {
        // Create a null-terminated hostname from input data
        char *hostname = (char *)malloc(size + 1);
        if (hostname) {
            memcpy(hostname, data, size);
            hostname[size] = '\0';
            
            struct hostent *host = nullptr;
            ares_gethostbyname_file(channel, hostname, fuzz_int32_t_4, &host);
            
            if (host) {
                ares_free_hostent(host);
            }
            
            free(hostname);
        }
    }

    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}
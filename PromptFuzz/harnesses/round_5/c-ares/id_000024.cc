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
//<ID> 358
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 15
//<Quality> {"density":9,"unique_branches":{"ares_send_nolock":[[122,7,122,44,0,0,4,0]],"ares_socket_configure":[[271,7,271,24,0,0,4,1],[276,14,276,32,0,0,4,0],[277,14,278,53,0,0,4,1]],"ares_cookie_apply":[[337,7,337,43,0,0,4,1]],"ares_conn_set_sockaddr":[[97,5,97,17,0,0,4,1],[108,5,108,18,0,0,4,0],[110,11,110,49,0,0,4,1]],"ares_sockaddr_to_ares_addr":[[306,7,306,37,0,0,4,1],[322,7,322,38,0,0,4,0],[331,9,331,13,0,0,4,1]],"ares_search_int":[[371,7,371,29,0,0,4,1]],"ares_addr_equal":[[271,5,271,17,0,0,4,1],[277,5,277,18,0,0,4,0],[281,11,283,63,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_set_servers_csv","ares_get_servers_csv","ares_free_string","ares_search","ares_fds","ares_process","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_servers_csv","ares_get_servers_csv","ares_free_string","ares_search","ares_fds","ares_process","ares_destroy","ares_library_cleanup"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified c-ares APIs together to achieve a DNS operation with NAPTR record parsing and manipulation:

*/


// Custom callback function for ares_search
static void  id_000024_search_callback(void *arg, int status, int timeouts,
                            const unsigned char *abuf, int alen) {
    // This callback will parse NAPTR records and manipulate DNS RRs
    if (status != ARES_SUCCESS || abuf == NULL || alen <= 0) {
        return;
    }
    
    struct ares_naptr_reply *naptr_out = NULL;
    
    // Parse NAPTR reply using the provided buffer
    int parse_result = ares_parse_naptr_reply(abuf, alen, &naptr_out);
    
    if (parse_result == ARES_SUCCESS && naptr_out != NULL) {
        // Create a DNS resource record to manipulate
        ares_dns_record_t *dnsrec = NULL;
        ares_status_t status = ares_dns_record_create(&dnsrec, 
                                                      (unsigned short)12345,
                                                      ARES_FLAG_RD,
                                                      ARES_OPCODE_QUERY,
                                                      ARES_RCODE_NOERROR);
        
        if (status == ARES_SUCCESS && dnsrec != NULL) {
            // Add a query to the DNS record
            status = ares_dns_record_query_add(dnsrec, "example.com",
                                               ARES_REC_TYPE_NAPTR,
                                               ARES_CLASS_IN);
            
            if (status == ARES_SUCCESS) {
                // Get the first RR from answer section (if exists)
                size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
                if (rr_count > 0) {
                    ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec,
                                                              ARES_SECTION_ANSWER,
                                                              0);
                    
                    if (rr != NULL) {
                        // First, set a U8 value in the RR
                        // For NAPTR record, we could set order field (assuming it's a U8)
                        // Use a dummy key - in reality, we need proper key for NAPTR records
                        ares_dns_rr_key_t order_key = ARES_RR_NAPTR_ORDER;
                        ares_status_t set_status = ares_dns_rr_set_u8(rr, order_key, 10);
                        
                        // Delete an option by ID (if available)
                        // For OPT records, we might delete an option
                        ares_dns_rr_key_t opt_key = ARES_RR_OPT_OPTIONS;
                        ares_status_t del_status = ares_dns_rr_del_opt_byid(rr, opt_key, 1);
                        
                        // Clean up the NAPTR reply
                        struct ares_naptr_reply *current = naptr_out;
                        struct ares_naptr_reply *next;
                        while (current != NULL) {
                            next = current->next;
                            ares_free_data(current->flags);
                            ares_free_data(current->service);
                            ares_free_data(current->regexp);
                            ares_free_data(current->replacement);
                            ares_free_data(current);
                            current = next;
                        }
                    }
                }
            }
            
            // Destroy the DNS record
            ares_free_data(dnsrec);
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }
    
    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Create channel
    ares_channel_t *channel = NULL;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Get servers as CSV from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read CSV servers from input (first line)
    char csv_buffer[1024];
    if (fgets(csv_buffer, sizeof(csv_buffer), in_file)) {
        // Remove newline if present
        size_t len = strlen(csv_buffer);
        if (len > 0 && csv_buffer[len - 1] == '\n') {
            csv_buffer[len - 1] = '\0';
        }
        
        // Set servers from CSV
        int set_result = ares_set_servers_csv(channel, csv_buffer);
        
        if (set_result == ARES_SUCCESS) {
            // Get servers back as CSV to verify
            char *servers_csv = ares_get_servers_csv(channel);
            if (servers_csv) {
                // Use the CSV string (could write to file or process)
                FILE *out_file = fopen("output_file", "wb");
                if (out_file) {
                    fwrite(servers_csv, strlen(servers_csv), 1, out_file);
                    assert_file_closed(&out_file);;
                }
                ares_free_string(servers_csv);
            }
            
            // Perform a search using remaining data as domain name
            // Use the rest of the input as domain name
            fseek(in_file, 0, SEEK_SET);
            char domain_buffer[256];
            // Skip the first line (CSV) and use second line as domain
            fgets(csv_buffer, sizeof(csv_buffer), in_file); // Skip CSV line
            if (fgets(domain_buffer, sizeof(domain_buffer), in_file)) {
                // Remove newline
                len = strlen(domain_buffer);
                if (len > 0 && domain_buffer[len - 1] == '\n') {
                    domain_buffer[len - 1] = '\0';
                }
                
                // Perform DNS search
                ares_search(channel, domain_buffer, 
                           ARES_CLASS_IN, ARES_REC_TYPE_NAPTR,
                            id_000024_search_callback, NULL);
                
                // Process events to trigger callbacks
                struct timeval tv;
                tv.tv_sec = 0;
                tv.tv_usec = 0;
                
                int nfds;
                fd_set read_fds, write_fds;
                
                // Process once to trigger the callback
                FD_ZERO(&read_fds);
                FD_ZERO(&write_fds);
                nfds = ares_fds(channel, &read_fds, &write_fds);
                
                if (nfds > 0) {
                    ares_process(channel, &read_fds, &write_fds);
                }
            }
        }
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    ares_destroy(channel);
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
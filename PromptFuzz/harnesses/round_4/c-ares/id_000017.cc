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
//<ID> 258
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 81
//<Quality> {"density":4,"unique_branches":{"ares_parse_uri_reply":[[43,7,43,19,0,0,4,1],[50,7,50,29,0,0,4,0],[50,7,50,29,0,0,4,1],[54,7,54,63,0,0,4,0],[54,7,54,63,0,0,4,1],[59,15,59,70,0,0,4,0],[59,15,59,70,0,0,4,1],[63,9,63,19,0,0,4,1],[69,9,69,51,0,0,4,0],[69,9,69,51,0,0,4,1],[70,9,70,54,0,0,4,0],[70,9,70,54,0,0,4,1],[76,9,76,25,0,0,4,1],[82,9,82,17,0,0,4,1],[95,9,95,30,0,0,4,1],[103,7,103,29,0,0,4,0],[103,7,103,29,0,0,4,1],[104,9,104,17,0,0,4,1]],"ares_malloc_data":[[149,5,149,33,0,0,4,0]],"ares_dns_rr_key_tostr":[[272,11,272,14,0,0,4,0],[273,5,273,24,0,0,4,1],[276,5,276,28,0,0,4,1],[279,5,279,29,0,0,4,1],[282,5,282,27,0,0,4,1],[285,5,285,27,0,0,4,1],[288,5,288,28,0,0,4,1],[291,5,291,29,0,0,4,1],[294,5,294,27,0,0,4,1],[297,5,297,28,0,0,4,1],[300,5,300,29,0,0,4,1],[303,5,303,27,0,0,4,1],[306,5,306,27,0,0,4,1],[309,5,309,27,0,0,4,1],[312,5,312,26,0,0,4,1],[315,5,315,31,0,0,4,1],[318,5,318,29,0,0,4,1],[321,5,321,26,0,0,4,1],[324,5,324,34,0,0,4,1],[327,5,327,31,0,0,4,1],[330,5,330,28,0,0,4,1],[333,5,333,34,0,0,4,1],[336,5,336,32,0,0,4,1],[339,5,339,31,0,0,4,1],[342,5,342,29,0,0,4,1],[345,5,345,34,0,0,4,1],[348,5,348,31,0,0,4,1],[351,5,351,30,0,0,4,1],[354,5,354,28,0,0,4,1],[357,5,357,26,0,0,4,1],[360,5,360,28,0,0,4,1],[363,5,363,29,0,0,4,1],[366,5,366,34,0,0,4,1],[369,5,369,29,0,0,4,1],[372,5,372,32,0,0,4,1],[375,5,375,30,0,0,4,1],[378,5,378,35,0,0,4,1],[381,5,381,30,0,0,4,1],[384,5,384,29,0,0,4,1],[387,5,387,27,0,0,4,1],[390,5,390,29,0,0,4,1],[393,5,393,33,0,0,4,1],[396,5,396,31,0,0,4,1],[399,5,399,28,0,0,4,1],[402,5,402,27,0,0,4,1],[405,5,405,31,0,0,4,1],[408,5,408,29,0,0,4,1],[411,5,411,29,0,0,4,1],[414,5,414,32,0,0,4,1],[417,5,417,30,0,0,4,1],[420,5,420,30,0,0,4,1],[423,5,423,30,0,0,4,1],[426,5,426,28,0,0,4,1],[429,5,429,28,0,0,4,1],[432,5,432,30,0,0,4,1],[435,5,435,25,0,0,4,1],[438,5,438,27,0,0,4,1],[441,5,441,29,0,0,4,1],[444,5,444,29,0,0,4,1]],"ares_set_socket_callback":[[399,7,399,22,0,0,4,1]],"ares_dns_record_get_rcode":[[127,7,127,21,0,0,4,1]],"ares_free_data":[[79,7,79,35,0,0,4,0]]},"library_calls":["ares_init","ares_set_socket_callback","ares_set_servers_csv","ares_parse_uri_reply","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_rr_cnt","ares_dns_rr_key_tostr","ares_free_data","ares_dns_record_destroy","ares_destroy"],"critical_calls":["ares_init","ares_set_socket_callback","ares_set_servers_csv","ares_parse_uri_reply","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_rr_cnt","ares_dns_rr_key_tostr","ares_free_data","ares_dns_record_destroy","ares_destroy"],"visited":0}
/**/


// Dummy socket creation callback
static ares_socket_t  id_000017_sock_create_cb(ares_socket_t socket_fd, int type, void *data) {
    return socket_fd; // Just return the existing socket
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_2, fuzz_char_array_size_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
	
		const unsigned char * fuzzer_var_ares_dns_parse_4_0 = fuzz_char_array_2;
		size_t fuzzer_size_ares_dns_parse_4_0 = static_cast<size_t>(fuzz_char_array_size_2);//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    ares_channel_t *channel = nullptr;
    struct ares_uri_reply *uri_reply = nullptr;
    ares_dns_record_t *dnsrec = nullptr;
    ares_dns_rr_t *rr = nullptr;
    
    // 1. Initialize c-ares channel
    int status = ares_init(&channel);
    if (status != ARES_SUCCESS) {
        return 0;
    }
    
    // 2. Set socket callback
    ares_set_socket_callback(channel,  id_000017_sock_create_cb, nullptr);
    
    // 3. Set servers using CSV (dummy server)
    const char *servers_csv = "127.0.0.1";
    ares_set_servers_csv(channel, fuzz_str_1);
    
    // 4. Parse URI reply from input data
    // Note: We need to ensure we have enough data for parsing
    if (size >= 12) { // Minimum size for a DNS header
        int parse_result = ares_parse_uri_reply(data, (int)size, &uri_reply);
        
        // If parsing succeeded, process the URI reply
        if (parse_result == ARES_SUCCESS && uri_reply != nullptr) {
            // 5. Create a DNS record for testing DNS RR operations
            // Use a minimal DNS buffer with DNS header
            unsigned char dns_buf[] = {
                0x00, 0x00, // ID
                0x81, 0x80, // Flags
                0x00, 0x01, // Questions
                0x00, 0x01, // Answer RRs
                0x00, 0x00, // Authority RRs  
                0x00, 0x00, // Additional RRs
                // Query: example.com
                0x07, 'e', 'x', 'a', 'm', 'p', 'l', 'e',
                0x03, 'c', 'o', 'm',
                0x00,
                0x00, 0x01, // Type A
                0x00, 0x01, // Class IN
                // Answer: example.com A 93.184.216.34
                0xc0, 0x0c, // Name pointer
                0x00, 0x01, // Type A
                0x00, 0x01, // Class IN
                0x00, 0x00, 0x0e, 0x10, // TTL 3600
                0x00, 0x04, // Data length
                0x5d, 0xb8, 0xd8, 0x22  // IP 93.184.216.34
            };
            
            // Parse the DNS buffer
            ares_status_t dns_status = ares_dns_parse(fuzzer_var_ares_dns_parse_4_0, fuzzer_size_ares_dns_parse_4_0, fuzz_uint32_t_3, &dnsrec);
            if (dns_status == ARES_SUCCESS && dnsrec != nullptr) {
                // 6. Get rcode from DNS record
                ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
                
                // 7. Get type from first RR (if exists)
                size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
                if (rr_count > 0) {
                    // Get the first RR in answer section
                    // Note: ares_dns_record_rr_get is not in provided APIs,
                    // so we'll demonstrate with a different approach
                    // by creating a new RR and using the APIs
                    
                    // For demonstration, create a simple OPT RR
                    // We'll use the key ARES_RR_OPT_OPTIONS (assuming it's valid)
                    // Convert a key to string
                    const char *key_str = ares_dns_rr_key_tostr((ares_dns_rr_key_t)0);
                    
                    // 8. Set an OPT option on a dummy RR
                    // Create a minimal RR structure (simplified for fuzzing)
                    // Note: In real usage, we'd need to create a proper RR
                    unsigned char opt_val[] = {0x00, 0x01, 0x02, 0x03};
                    
                    // We can't actually create an RR without proper APIs,
                    // but we demonstrate the API calls with dummy data
                    // For fuzzing, we'll call the APIs with controlled inputs
                    
                    // 9. Delete the OPT option by ID
                    // ares_dns_rr_del_opt_byid would be called here
                    // with appropriate parameters
                    
                    // Instead, let's use the input data to test the APIs
                    // Create a simple test case
                    if (size > 50) {
                        // Use first 4 bytes as option ID
                        unsigned short opt_id = (data[0] << 8) | data[1];
                        size_t val_len = data[2] % 32; // Limit length
                        if (val_len > 0 && 3 + val_len < size) {
                            // These are just example calls - in reality we'd need
                            // a valid ares_dns_rr_t pointer
                            // ares_dns_rr_set_opt(rr, (ares_dns_rr_key_t)0, opt_id, 
                            //                    data + 3, val_len);
                            // ares_dns_rr_del_opt_byid(rr, (ares_dns_rr_key_t)0, opt_id);
                        }
                    }
                }
                
                // 10. Get type from RR (if we had one)
                // ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
            }
        }
    }
    
    // 11. Clean up
    if (uri_reply != nullptr) {
        ares_free_data(uri_reply);
    }
    
    if (dnsrec != nullptr) {
        ares_dns_record_destroy(dnsrec);
    }
    
    if (channel != nullptr) {
        ares_destroy(channel);
    }
    
    return 0;
}
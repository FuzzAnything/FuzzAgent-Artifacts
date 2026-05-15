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
//<ID> 134
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"ares_dns_rr_get_keys":[[622,5,622,27,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_set_servers_ports_csv","ares_dns_parse","ares_dns_record_get_id","ares_dns_record_query_cnt","ares_dns_record_query_get","ares_dns_record_query_set_type","ares_dns_rr_get_keys","ares_dns_section_tostr","ares_dns_record_query_set_name","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_servers_ports_csv","ares_dns_parse","ares_dns_record_get_id","ares_dns_record_query_cnt","ares_dns_record_query_get","ares_dns_record_query_set_type","ares_dns_rr_get_keys","ares_dns_section_tostr","ares_dns_record_query_set_name","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Include c-ares headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    if (ares_init_options(&channel, nullptr, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Read some data to use as CSV string (limit to reasonable size)
    char csv_buffer[4096];
    size_t csv_read = fread(csv_buffer, 1, sizeof(csv_buffer) - 1, in_file);
    csv_buffer[csv_read] = '\0';

    // API 1: Set servers using CSV string
    if (csv_read > 0) {
        ares_set_servers_ports_csv(channel, csv_buffer);
    }

    // Parse DNS record from the remaining input data
    rewind(in_file);
    
    // Read buffer for DNS parsing
    unsigned char *dns_buf = nullptr;
    size_t dns_len = 0;
    
    // Try to read the entire file as DNS data
    fseek(in_file, 0, SEEK_END);
    dns_len = ftell(in_file);
    rewind(in_file);
    
    if (dns_len > 0) {
        dns_buf = (unsigned char *)malloc(dns_len);
        if (dns_buf) {
            fread(dns_buf, 1, dns_len, in_file);
            
            // API 2: Parse DNS record
            ares_dns_record_t *dnsrec = nullptr;
            ares_status_t parse_status = ares_dns_parse(dns_buf, dns_len, fuzz_uint32_t_2, &dnsrec);
            
            if (parse_status == ARES_SUCCESS && dnsrec) {
                // API 3: Get DNS record ID
                unsigned short record_id = ares_dns_record_get_id(dnsrec);
                (void)record_id; // Use variable to avoid unused warning
                
                // API 4: Get query count
                size_t query_cnt = ares_dns_record_query_cnt(dnsrec);
                
                if (query_cnt > 0) {
                    // API 5: Get query info
                    const char *name = nullptr;
                    ares_dns_rec_type_t qtype = ARES_REC_TYPE_A;
                    ares_dns_class_t qclass = ARES_CLASS_IN;
                    
                    if (ares_dns_record_query_get(dnsrec, fuzz_uint64_t_3, &name, &qtype, &qclass) == ARES_SUCCESS) {
                        // API 6: Set query type (cycle through types)
                        ares_dns_rec_type_t new_type = static_cast<ares_dns_rec_type_t>(
                            (qtype + 1) % (ARES_REC_TYPE_ANY + 1));
                        ares_dns_record_query_set_type(dnsrec, fuzz_uint64_t_4, new_type);
                        
                        // API 7: Get keys for record type
                        size_t key_cnt = 0;
                        const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(new_type, &key_cnt);
                        (void)keys; // Use variable
                        
                        // API 8: Convert section to string (using answer section)
                        const char *section_str = ares_dns_section_tostr(ARES_SECTION_ANSWER);
                        (void)section_str; // Use variable
                        
                        // API 9: Set query name (use a modified version)
                        char modified_name[256];
                        if (name) {
                            snprintf(modified_name, sizeof(modified_name), "fuzz.%s", name);
                            ares_dns_record_query_set_name(dnsrec, fuzz_uint64_t_5, modified_name);
                        }
                    }
                }
                
                // Clean up DNS record
                ares_dns_record_destroy(dnsrec);
            }
            
            free(dns_buf);
        }
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Get file descriptor for output
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd; // Use variable
        
        // Write some data to output file
        fwrite(data, 1, size > 1024 ? 1024 : size, out_file);
        assert_file_closed(&out_file);;
    }

    // Clean up
    assert_file_closed(&in_file);;
    ares_destroy(channel);
    ares_library_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
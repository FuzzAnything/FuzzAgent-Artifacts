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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 77, nr_unique_branch: 26
//<Quality> {"density":14,"unique_branches":{"ares_dns_rr_del_abin":[[922,7,922,59,0,0,4,0]],"ares_dns_get_opt_rr_const":[[1553,15,1553,71,0,0,4,0],[1557,9,1557,54,0,0,4,0],[1557,9,1557,54,0,0,4,1]],"ares_dns_write_header":[[94,29,94,70,0,0,4,1]],"ares_dns_rcode_isvalid":[[53,5,53,28,0,0,4,0]],"ares_dns_rr_get_keys":[[604,5,604,29,0,0,4,0],[607,5,607,27,0,0,4,0],[610,5,610,27,0,0,4,0],[616,5,616,26,0,0,4,0],[619,5,619,27,0,0,4,0],[625,5,625,28,0,0,4,0],[628,5,628,27,0,0,4,0],[631,5,631,29,0,0,4,0],[640,5,640,28,0,0,4,0],[643,5,643,29,0,0,4,0],[655,5,655,30,0,0,4,0]],"ares_process_fds_nolock":[[210,9,210,50,0,0,4,1],[216,9,216,30,0,0,4,1],[224,9,224,49,0,0,4,0]],"process_write":[[392,7,392,19,0,0,4,0]],"ares_process_fd":[[275,7,275,33,0,0,4,0],[281,7,281,34,0,0,4,0],[282,9,282,28,0,0,4,0]],"ares_dns_record_rr_cnt":[[352,7,352,21,0,0,4,0]],"ares_dns_record_query_cnt":[[246,7,246,21,0,0,4,0]]},"library_calls":["ares_library_init_mem","ares_init","ares_dns_parse","ares_dns_record_duplicate","ares_dns_record_query_cnt","ares_dns_record_query_set_name","ares_dns_record_query_set_name","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_key_datatype","ares_dns_rr_key_datatype","ares_dns_rr_add_abin","ares_dns_rr_del_abin","ares_expand_name","ares_getnameinfo","ares_process_fd","ares_process_fd","ares_free_string","ares_dns_record_destroy","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init_mem","ares_init","ares_dns_parse","ares_dns_record_duplicate","ares_dns_record_query_cnt","ares_dns_record_query_set_name","ares_dns_record_query_set_name","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_type","ares_dns_rr_get_keys","ares_dns_rr_key_datatype","ares_dns_rr_key_datatype","ares_dns_rr_add_abin","ares_dns_rr_del_abin","ares_expand_name","ares_getnameinfo","ares_process_fd","ares_process_fd","ares_free_string","ares_dns_record_destroy","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":1}
/*Looking at the provided APIs, I can see they involve DNS record manipulation and c-ares library operations. Let me create a fuzz driver that uses these APIs to achieve DNS record parsing and modification:

*/


extern "C" {
}

// Custom memory functions for ares_library_init_mem
static void * id_000003_custom_malloc(size_t size) {
    return malloc(size);
}

static void  id_000003_custom_free(void *ptr) {
    free(ptr);
}

static void * id_000003_custom_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

// Dummy callback for getnameinfo
static void  id_000003_dummy_nameinfo_callback(void *arg, int status, int timeouts,
                                    const char *node, const char *service) {
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<27) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_3, fuzz_char_array_size_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
	
		const unsigned char * fuzzer_var_ares_dns_rr_add_abin_13_2 = fuzz_char_array_3;
		size_t fuzzer_size_ares_dns_rr_add_abin_13_2 = static_cast<size_t>(fuzz_char_array_size_3);//fuzzer shim end}




    // Initialize c-ares with custom memory functions
    if (ares_library_init_mem(ARES_LIB_INIT_ALL, 
                               id_000003_custom_malloc, 
                               id_000003_custom_free, 
                               id_000003_custom_realloc) != ARES_SUCCESS) {
        return 0;
    }
    
    ares_channel_t *channel = nullptr;
    ares_dns_record_t *dnsrec = nullptr;
    ares_dns_record_t *duplicate = nullptr;
    char *expanded_name = nullptr;
    long enclen = 0;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    
    // 1. Create a channel
    if (ares_init(&channel) != ARES_SUCCESS) {
        goto cleanup;
    }
    
    // 2. Parse DNS record from input data
    if (size > 0) {
        unsigned int flags = 0;
        if (ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec) == ARES_SUCCESS && dnsrec != nullptr) {
            
            // 3. Duplicate the DNS record
            duplicate = ares_dns_record_duplicate(dnsrec);
            
            // 4. Modify query names in duplicate if any exist
            size_t query_count = ares_dns_record_query_cnt(duplicate);
            if (query_count > 0) {
                // Change the first query name to "example.com"
                ares_dns_record_query_set_name(duplicate, 0, "example.com");
                
                // Try to modify other queries with valid data if available
                for (size_t i = 1; i < query_count && i < 3; i++) {
                    if (size > i * 10) {
                        // Create a simple name from input data
                        char temp_name[256] = {0};
                        size_t len = (size - i * 10) < 255 ? (size - i * 10) : 255;
                        for (size_t j = 0; j < len; j++) {
                            char c = data[i * 10 + j];
                            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
                                (c >= '0' && c <= '9') || c == '.' || c == '-') {
                                temp_name[j] = c;
                            } else {
                                temp_name[j] = 'a' + (c % 26);
                            }
                        }
                        temp_name[len] = '\0';
                        ares_dns_record_query_set_name(duplicate, i, temp_name);
                    }
                }
            }
            
            // 5. Process resource records in answer section
            size_t rr_count = ares_dns_record_rr_cnt(duplicate, ARES_SECTION_ANSWER);
            if (rr_count > 0) {
                // Get the first resource record
                const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(duplicate, ARES_SECTION_ANSWER, fuzz_uint64_t_2);
                if (rr != nullptr) {
                    // Try to add and delete binary data
                    // First, find a binary key for this RR type
                    ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                    const ares_dns_rr_key_t *keys = nullptr;
                    size_t key_count = 0;
                    keys = ares_dns_rr_get_keys(rr_type, &key_count);
                    
                    for (size_t k = 0; k < key_count; k++) {
                        if (ares_dns_rr_key_datatype(keys[k]) == ARES_DATATYPE_BIN ||
                            ares_dns_rr_key_datatype(keys[k]) == ARES_DATATYPE_BINP) {
                            // Try to add binary data
                            if (size > 20) {
                                unsigned char test_data[] = {0x01, 0x02, 0x03, 0x04};
                                // Note: We need to cast away const to modify
                                ares_dns_rr_t *modifiable_rr = (ares_dns_rr_t *)rr;
                                ares_dns_rr_add_abin(modifiable_rr, keys[k], fuzzer_var_ares_dns_rr_add_abin_13_2, fuzzer_size_ares_dns_rr_add_abin_13_2);
                                
                                // Delete the binary data we just added
                                ares_dns_rr_del_abin(modifiable_rr, keys[k], fuzz_uint64_t_4);
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    
    // 6. Try to expand a name from the input data
    if (size > 12) {
        // Use ares_expand_name on a portion of the input
        ares_expand_name(data + 4, data, size, &expanded_name, &enclen);
    }
    
    // 7. Create a socket address and call getnameinfo
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    if (size > 4) {
        // Use first 4 bytes as IP address
        memcpy(&sa.sin_addr, data, 4);
    } else {
        sa.sin_addr.s_addr = htonl(0x7f000001); // 127.0.0.1
    }
    sa.sin_port = htons(53);
    
    ares_getnameinfo(channel, (struct sockaddr *)&sa, sizeof(sa), 
                     ARES_NI_NOFQDN,  id_000003_dummy_nameinfo_callback, nullptr);
    
    // 8. Create file descriptors and process them
    in_file = fmemopen((void *)data, size, "rb");
    out_file = fopen("output_file", "wb");
    
    if (in_file != nullptr && out_file != nullptr) {
        int in_fd = fuzz_fileno(in_file);
        int out_fd = fuzz_fileno(out_file);
        
        // Process the channel with file descriptors
        ares_process_fd(channel, in_fd, out_fd);
        
        // Also try with invalid descriptors
        ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    }
    
    // 9. Write some data to output file if it exists
    if (out_file != nullptr && size > 0) {
        size_t write_size = size > 100 ? 100 : size;
        fwrite(data, 1, write_size, out_file);
    }

cleanup:
    // Clean up resources
    if (expanded_name != nullptr) {
        ares_free_string(expanded_name);
    }
    
    if (duplicate != nullptr) {
        ares_dns_record_destroy(duplicate);
    }
    
    if (dnsrec != nullptr) {
        ares_dns_record_destroy(dnsrec);
    }
    
    if (channel != nullptr) {
        ares_destroy(channel);
    }
    
    if (in_file != nullptr) {
        assert_file_closed(&in_file);;
    }
    
    if (out_file != nullptr) {
        assert_file_closed(&out_file);;
        remove("output_file");
    }
    
    ares_library_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
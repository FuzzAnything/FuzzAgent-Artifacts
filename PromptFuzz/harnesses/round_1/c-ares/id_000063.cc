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
//<ID> 1217
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 3
//<Quality> {"density":8,"unique_branches":{"nameinfo_callback":[[243,12,243,36,0,0,4,1]],"ares_dns_rr_get_name":[[507,7,507,17,0,0,4,0]],"addr_callback":[[177,14,177,41,0,0,4,0]]},"library_calls":["ares_library_init_mem","ares_library_cleanup","ares_parse_uri_reply","ares_dns_rec_type_fromstr","ares_free_string","ares_expand_string","ares_dns_rr_get_name","ares_free_string","ares_init","ares_dup","ares_set_servers","ares_get_servers_csv","ares_dns_rec_type_fromstr","ares_free_string","ares_dns_record_create","ares_dns_record_query_add","ares_dns_write","ares_getnameinfo","ares_dns_record_destroy","ares_destroy","ares_destroy","ares_expand_string","ares_free_string","ares_library_cleanup"],"critical_calls":["ares_library_init_mem","ares_parse_uri_reply","ares_dns_rec_type_fromstr","ares_free_string","ares_expand_string","ares_dns_rr_get_name","ares_free_string","ares_init","ares_dup","ares_set_servers","ares_get_servers_csv","ares_dns_rec_type_fromstr","ares_free_string","ares_dns_record_create","ares_dns_record_query_add","ares_dns_write","ares_getnameinfo","ares_dns_record_destroy","ares_destroy","ares_destroy","ares_expand_string","ares_free_string","ares_library_cleanup"],"visited":0}
/**/


// Custom memory allocation tracking
static size_t  id_000063_total_allocated = 0;
static size_t  id_000063_allocation_count = 0;

void*  id_000063_counting_malloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr) {
         id_000063_total_allocated += size;
         id_000063_allocation_count++;
    }
    return ptr;
}

void  id_000063_counting_free(void* ptr) {
    free(ptr);
}

void*  id_000063_counting_realloc(void* oldptr, size_t size) {
    void* newptr = realloc(oldptr, size);
    if (newptr && newptr != oldptr) {
        // We can't accurately track realloc without knowing old size
        // Just increment count for tracking purposes
         id_000063_allocation_count++;
    }
    return newptr;
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Initialize c-ares library with custom memory allocators
    if (ares_library_init_mem(ARES_LIB_INIT_ALL, 
                              id_000063_counting_malloc, 
                              id_000063_counting_free, 
                              id_000063_counting_realloc) != ARES_SUCCESS) {
        return 0;
    }

    // Reset tracking counters
     id_000063_total_allocated = 0;
     id_000063_allocation_count = 0;

    // Create FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Part 1: Parse URI reply from input data
    struct ares_uri_reply* uri_out = nullptr;
    int parse_result = ares_parse_uri_reply(data, static_cast<int>(size), &uri_out);
    
    // Process URI reply if successful
    if (parse_result == ARES_SUCCESS && uri_out) {
        struct ares_uri_reply* current = uri_out;
        while (current) {
            // For each URI, try to convert to DNS record type string
            ares_dns_rec_type_t qtype;
            ares_bool_t fromstr_result = ares_dns_rec_type_fromstr(&qtype, current->uri);
            
            current = current->next;
        }
        
        // Free URI reply structure
        struct ares_uri_reply* next;
        current = uri_out;
        while (current) {
            next = current->next;
            ares_free_string(current->uri);
            free(current);
            current = next;
        }
    }

    // Part 2: Expand string from DNS compressed format
    if (size > 0) {
        unsigned char* expanded_str = nullptr;
        long enclen = 0;
        
        // Use the input data as both encoded string and buffer
        int expand_result = ares_expand_string(data, data, static_cast<int>(size), 
                                               &expanded_str, &enclen);
        
        if (expand_result == ARES_SUCCESS && expanded_str) {
            // Get the name from the expanded string (treating it as a DNS RR)
            const char* rr_name = ares_dns_rr_get_name(nullptr); // NULL for demonstration
            
            ares_free_string(expanded_str);
        }
    }

    // Part 3: Channel operations
    ares_channel_t* channel = nullptr;
    ares_channel_t* dup_channel = nullptr;
    
    // Initialize a channel
    if (ares_init(&channel) == ARES_SUCCESS && channel) {
        // Duplicate the channel
        int dup_result = ares_dup(&dup_channel, channel);
        
        if (dup_result == ARES_SUCCESS && dup_channel) {
            // Create dummy server list from input data
            struct ares_addr_node* servers = nullptr;
            if (size >= sizeof(struct in_addr)) {
                servers = static_cast<struct ares_addr_node*>(
                    malloc(sizeof(struct ares_addr_node)));
                if (servers) {
                    servers->next = nullptr;
                    servers->family = AF_INET;
                    
                    // Use first 4 bytes as IPv4 address
                    memcpy(&servers->addr.addr4, data, 
                           std::min(size, sizeof(struct in_addr)));
                    
                    // Set servers on duplicated channel
                    ares_set_servers(dup_channel, servers);
                    
                    // Get servers as CSV
                    char* servers_csv = ares_get_servers_csv(dup_channel);
                    if (servers_csv) {
                        // Try to parse as DNS record type
                        ares_dns_rec_type_t parsed_type;
                        ares_dns_rec_type_fromstr(&parsed_type, servers_csv);
                        
                        ares_free_string(servers_csv);
                    }
                    
                    free(servers);
                }
            }
            
            // Part 4: Create a DNS record and write it
            ares_dns_record_t* dnsrec = nullptr;
            ares_status_t create_status = ares_dns_record_create(&dnsrec, fuzz_uint16_t_1, 
                                                                ARES_FLAG_QR | ARES_FLAG_RD,
                                                                ARES_OPCODE_QUERY,
                                                                ARES_RCODE_NOERROR);
            
            if (create_status == ARES_SUCCESS && dnsrec) {
                // Add a query to the DNS record
                if (size > 0) {
                    // Use first byte as character for domain name
                    char domain_name[] = "example.com";
                    if (data[0] < 26) {
                        domain_name[0] = 'a' + (data[0] % 26);
                    }
                    
                    ares_dns_record_query_add(dnsrec, fuzz_str_2, 
                                             ARES_REC_TYPE_A, ARES_CLASS_IN);
                }
                
                // Write DNS record to buffer
                unsigned char* dns_buf = nullptr;
                size_t buf_len = 0;
                ares_status_t write_status = ares_dns_write(dnsrec, &dns_buf, &buf_len);
                
                if (write_status == ARES_SUCCESS && dns_buf) {
                    // Use the written buffer for getnameinfo
                    if (buf_len >= sizeof(struct sockaddr_in)) {
                        struct sockaddr_in sa;
                        memset(&sa, 0, sizeof(sa));
                        sa.sin_family = AF_INET;
                        sa.sin_port = htons(53);
                        
                        // Use bytes from DNS buffer for IP address
                        size_t copy_len = std::min(buf_len, sizeof(sa.sin_addr));
                        memcpy(&sa.sin_addr, dns_buf, copy_len);
                        
                        // Get name info (callback does nothing in fuzzing context)
                        ares_getnameinfo(dup_channel, 
                                        reinterpret_cast<struct sockaddr*>(&sa),
                                        sizeof(sa), 
                                        fuzz_int32_t_3, 
                                        [](void* arg, int status, int timeouts, 
                                           const char* node, const char* service) {
                                            // Empty callback for fuzzing
                                        }, 
                                        nullptr);
                    }
                    
                    free(dns_buf);
                }
                
                // Destroy DNS record
                ares_dns_record_destroy(dnsrec);
            }
            
            // Destroy duplicated channel
            ares_destroy(dup_channel);
        }
        
        // Destroy original channel
        ares_destroy(channel);
    }

    // Part 5: Additional string expansion tests
    if (size > 10) {
        // Try expanding from different offsets in the data
        for (size_t offset = 0; offset < std::min(size, static_cast<size_t>(5)); offset++) {
            unsigned char* expanded = nullptr;
            long enclen = 0;
            
            if (ares_expand_string(data + offset, data, static_cast<int>(size), 
                                  &expanded, &enclen) == ARES_SUCCESS && expanded) {
                ares_free_string(expanded);
            }
        }
    }

    // Clean up
    assert_file_closed(&in_file);;
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
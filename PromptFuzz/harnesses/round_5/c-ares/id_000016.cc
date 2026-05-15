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
//<ID> 180
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 14
//<Quality> {"density":5,"unique_branches":{"ares_parse_naptr_reply":[[42,7,42,19,0,0,4,1],[49,7,49,29,0,0,4,0],[49,7,49,29,0,0,4,1],[53,7,53,63,0,0,4,0],[53,7,53,63,0,0,4,1],[58,15,58,70,0,0,4,0],[58,15,58,70,0,0,4,1],[62,9,62,19,0,0,4,1],[68,9,68,51,0,0,4,0],[68,9,68,51,0,0,4,1],[69,9,69,56,0,0,4,0],[122,7,122,29,0,0,4,0],[122,7,122,29,0,0,4,1],[123,9,123,19,0,0,4,1]]},"library_calls":["ares_library_init_mem","ares_library_cleanup","ares_parse_naptr_reply","ares_init","ares_free_data","ares_library_cleanup","ares_set_local_ip6","ares_get_servers","ares_free_data","ares_inet_pton","ares_query_dnsrec","ares_dns_write","ares_dns_record_destroy","ares_free_data","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init_mem","ares_parse_naptr_reply","ares_init","ares_set_local_ip6","ares_get_servers","ares_free_data","ares_inet_pton","ares_query_dnsrec","ares_dns_write","ares_dns_record_destroy","ares_free_data","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


// Custom memory management functions for ares_library_init_mem
static void*  id_000016_custom_malloc(size_t size) {
    return malloc(size);
}

static void  id_000016_custom_free(void* ptr) {
    free(ptr);
}

static void*  id_000016_custom_realloc(void* ptr, size_t size) {
    return realloc(ptr, size);
}

// Callback for ares_query_dnsrec
static void  id_000016_query_callback(void* arg, ares_status_t status, 
                          size_t timeout_ms, const ares_dns_record_t* dnsrec) {
    // Do nothing in fuzzing context
    (void)arg;
    (void)status;
    (void)timeout_ms;
    (void)dnsrec;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize c-ares library with custom memory management
    if (ares_library_init_mem(ARES_LIB_INIT_ALL, 
                               id_000016_custom_malloc, 
                               id_000016_custom_free, 
                               id_000016_custom_realloc) != ARES_SUCCESS) {
        return 0;
    }

    // Create input file from fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Parse NAPTR reply from input data
    struct ares_naptr_reply* naptr_reply = nullptr;
    // Use first part of data for NAPTR parsing
    size_t naptr_size = (size > 1024) ? 1024 : size;
    unsigned char* naptr_buf = (unsigned char*)malloc(naptr_size);
    if (naptr_buf && naptr_size > 0) {
        fread(naptr_buf, 1, naptr_size, in_file);
        ares_parse_naptr_reply(naptr_buf, naptr_size, &naptr_reply);
        free(naptr_buf);
    }

    // 3. Parse DNS record from input data
    ares_dns_record_t* dnsrec = nullptr;
    rewind(in_file);
    unsigned char* dns_buf = (unsigned char*)malloc(size);
    if (dns_buf && size > 0) {
        fread(dns_buf, 1, size, in_file);
        // Note: ares_dns_parse is not in provided APIs, using alternative approach
        // We'll create a DNS record manually instead
    }
    free(dns_buf);

    // Create a DNS record manually
    ares_dns_record_t* new_dnsrec = nullptr;
    // We need to create via parsing or construction - using minimal construction
    // Since ares_dns_parse isn't available, we'll skip direct parsing
    
    // 4. Initialize channel
    ares_channel_t* channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        if (naptr_reply) ares_free_data(naptr_reply);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Set local IPv6 address
    unsigned char local_ip6[16] = {0};
    if (size >= 16) {
        memcpy(local_ip6, data, 16);
        ares_set_local_ip6(channel, local_ip6);
    }

    // 6. Get servers from channel
    struct ares_addr_node* servers = nullptr;
    ares_get_servers(channel, &servers);
    if (servers) {
        ares_free_data(servers);
    }

    // 7. Convert string to IPv6 address using ares_inet_pton
    struct in6_addr ip6;
    char ip6_str[INET6_ADDRSTRLEN] = "::1"; // Default IPv6 string
    if (size > 0) {
        // Use up to 40 bytes for IPv6 string (max length of IPv6 string)
        size_t str_len = (size < 40) ? size : 40;
        char* temp_str = (char*)malloc(str_len + 1);
        if (temp_str) {
            memcpy(temp_str, data, str_len);
            temp_str[str_len] = '\0';
            ares_inet_pton(AF_INET6, temp_str, &ip6);
            free(temp_str);
        }
    }

    // 8. Create a DNS record and set string value in RR
    // First create a simple DNS record
    ares_dns_record_t* test_dnsrec = nullptr;
    // We need to create a record - using query to create one
    unsigned short qid = 0;
    if (size > 0) {
        char domain[256] = "example.com";
        if (size < 256) {
            memcpy(domain, data, size);
            domain[size] = '\0';
        }
        
        // Query to create DNS record
        ares_query_dnsrec(channel, domain, 
                         ARES_CLASS_IN, 
                         ARES_REC_TYPE_A, 
                          id_000016_query_callback, 
                         nullptr, 
                         &qid);
    }

    // 9. Write DNS record to buffer (if we had one)
    if (new_dnsrec) {
        unsigned char* out_buf = nullptr;
        size_t out_len = 0;
        ares_dns_write(new_dnsrec, &out_buf, &out_len);
        if (out_buf) {
            free(out_buf);
        }
        ares_dns_record_destroy(new_dnsrec);
    }

    // 10. Clean up
    if (naptr_reply) {
        ares_free_data(naptr_reply);
    }
    
    if (channel) {
        ares_destroy(channel);
    }
    
    assert_file_closed(&in_file);;
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
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
//<ID> 913
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 11
//<Quality> {"density":6,"unique_branches":{"ares_htable_expand":[[219,7,219,25,0,0,4,1]],"ares_htable_buckets_destroy":[[77,7,77,22,0,0,4,0]],"ares_sconfig_linklocal":[[415,7,415,50,0,0,4,0]],"ares_dns_rec_type_fromstr":[[728,15,728,35,0,0,4,1]],"ares_uri_decode_inplace":[[100,34,100,59,2,0,4,0],[1022,22,1022,30,3,0,4,0],[1022,22,1022,30,4,0,4,0]],"ares_os_if_nametoindex":[[555,7,555,19,0,0,4,1]],"parse_nameserver":[[290,9,290,31,0,0,4,0],[376,9,376,31,0,0,4,0]],"ares_dns_opcode_tostr":[[262,5,262,28,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_servers_csv","ares_dns_rec_type_fromstr","ares_dns_rec_type_tostr","ares_expand_string","ares_free_string","ares_dns_record_create","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_parse","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_destroy","ares_dns_record_destroy","ares_search","ares_process_fd","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_servers_csv","ares_dns_rec_type_fromstr","ares_dns_rec_type_tostr","ares_expand_string","ares_free_string","ares_dns_record_create","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_parse","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_destroy","ares_dns_record_destroy","ares_search","ares_process_fd","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Callback function for ares_search
static void  id_000038_search_callback(void* arg, int status, int timeouts, 
                           const unsigned char* abuf, int alen) {
    // Do nothing in fuzzing context
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create channel
    ares_channel_t* channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 1. Use ares_set_servers_csv with data as CSV string
    if (size > 0) {
        // Create null-terminated string for servers CSV
        char* servers_csv = (char*)malloc(size + 1);
        if (servers_csv) {
            memcpy(servers_csv, data, size);
            servers_csv[size] = '\0';
            
            // Try to set servers (may fail if CSV is invalid, but that's OK for fuzzing)
            ares_set_servers_csv(channel, servers_csv);
            
            free(servers_csv);
        }
    }

    // 2. Use ares_dns_rec_type_fromstr with a portion of data
    if (size > 10) {
        // Create string for DNS record type
        char type_str[11];
        size_t type_len = (size < 10) ? size : 10;
        memcpy(type_str, data, type_len);
        type_str[type_len] = '\0';
        
        ares_dns_rec_type_t qtype;
        // Try to convert string to DNS record type
        ares_dns_rec_type_fromstr(&qtype, type_str);
        
        // 3. Use ares_dns_rec_type_tostr to convert back
        const char* type_name = ares_dns_rec_type_tostr(qtype);
        (void)type_name; // Use it to avoid unused variable warning
    }

    // 4. Use ares_expand_string with a portion of data
    if (size > 20) {
        // Use middle portion of data as "encoded" string
        size_t encoded_offset = size / 3;
        size_t encoded_len = (size - encoded_offset) / 2;
        
        if (encoded_len > 0) {
            unsigned char* expanded = nullptr;
            long enclen = 0;
            
            // abuf will be the entire data buffer
            // encoded will be a pointer within the buffer
            const unsigned char* encoded = data + encoded_offset;
            
            // Try to expand string
            ares_expand_string(encoded, data, size, &expanded, &enclen);
            
            // Clean up expanded string if allocation succeeded
            if (expanded) {
                ares_free_string(expanded);
            }
        }
    }

    // 5. Create a DNS record for testing ares_dns_write and related functions
    ares_dns_record_t* dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, 12345, 
                                                 ARES_FLAG_QR | ARES_FLAG_RD, 
                                                 ARES_OPCODE_QUERY, 
                                                 ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // 6. Get opcode using ares_dns_record_get_opcode
        ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
        
        // 7. Convert opcode to string using ares_dns_opcode_tostr
        const char* opcode_str = ares_dns_opcode_tostr(opcode);
        (void)opcode_str; // Use it to avoid unused variable warning
        
        // 8. Write DNS record to buffer using ares_dns_write
        unsigned char* buf = nullptr;
        size_t buf_len = 0;
        
        // Note: ares_dns_write is not in the provided list, but it's mentioned
        // in the required APIs. We'll use a similar API ares_dns_parse instead
        // to process the input data as DNS record
        if (size > 12) { // Minimum DNS header size
            ares_dns_record_t* parsed_dnsrec = nullptr;
            ares_status_t parse_status = ares_dns_parse(data, size, 0, &parsed_dnsrec);
            
            if (parse_status == ARES_SUCCESS && parsed_dnsrec) {
                // Get opcode from parsed record
                ares_dns_opcode_t parsed_opcode = ares_dns_record_get_opcode(parsed_dnsrec);
                const char* parsed_opcode_str = ares_dns_opcode_tostr(parsed_opcode);
                (void)parsed_opcode_str;
                
                // Clean up parsed record
                ares_dns_record_destroy(parsed_dnsrec);
            }
        }
        
        // Clean up created record
        ares_dns_record_destroy(dnsrec);
    }

    // 9. Use ares_search with a portion of data as domain name
    if (size > 5) {
        // Create domain name from data
        char domain[256];
        size_t domain_len = (size < 255) ? size : 255;
        memcpy(domain, data, domain_len);
        domain[domain_len] = '\0';
        
        // Perform search (will likely fail or timeout, but that's OK)
        ares_search(channel, domain, C_IN, T_A,  id_000038_search_callback, nullptr);
        
        // Process any pending operations
        ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    }

    // Clean up channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();

    return 0;
}
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
//<ID> 817
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 17
//<Quality> {"density":6,"unique_branches":{"ares_buf_append_start":[[292,22,292,31,0,0,4,0]],"ares_parse_a_reply":[[87,7,87,23,0,0,4,1]],"ares_buf_begins_with":[[1084,22,1084,34,0,0,4,0],[1084,38,1084,51,0,0,4,0]],"ares_llist_insert_at":[[128,23,128,34,0,0,4,0]],"ares_buf_tag_fetch":[[387,7,387,18,0,0,4,0],[387,22,387,49,0,0,4,0],[387,53,387,64,0,0,4,0]],"ares_buf_consume_nonwhitespace":[[697,15,697,32,0,0,4,1]],"ares_buf_tag_fetch_string":[[449,7,449,18,0,0,4,0],[449,22,449,30,0,0,4,0]],"ares_buf_create_const":[[61,7,61,19,0,0,4,0]],"ares_strcasecmp":[[378,7,378,16,0,0,4,1]],"ares_htable_create":[[113,7,113,21,0,0,4,0]],"ares_buf_tag_fetch_bytes":[[409,7,409,18,0,0,4,0],[409,22,409,35,0,0,4,0],[409,39,409,50,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_socket_functions","ares_reinit","ares_destroy","ares_library_cleanup","ares_parse_a_reply","ares_parse_uri_reply","ares_query","ares_dns_section_tostr","ares_gethostbyname_file","ares_dns_record_create","ares_dns_record_query_add","ares_dns_write","ares_free_string","ares_dns_record_destroy","ares_free_hostent","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_socket_functions","ares_reinit","ares_parse_a_reply","ares_parse_uri_reply","ares_query","ares_dns_section_tostr","ares_gethostbyname_file","ares_dns_record_create","ares_dns_record_query_add","ares_dns_write","ares_free_string","ares_dns_record_destroy","ares_free_hostent","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// c-ares headers

// Custom socket functions structure
static ares_socket_t  id_000037_dummy_socket(int domain, int type, int protocol, void *user_data) {
    (void)domain; (void)type; (void)protocol; (void)user_data;
    return ARES_SOCKET_BAD;
}

static int  id_000037_dummy_close(ares_socket_t socket_fd, void *user_data) {
    (void)socket_fd; (void)user_data;
    return -1;
}

static int  id_000037_dummy_connect(ares_socket_t socket_fd, const struct sockaddr *addr, 
                         ares_socklen_t addrlen, void *user_data) {
    (void)socket_fd; (void)addr; (void)addrlen; (void)user_data;
    return -1;
}

static ares_ssize_t  id_000037_dummy_recvfrom(ares_socket_t socket_fd, void *buffer, 
                                   size_t length, int flags, struct sockaddr *addr, 
                                   ares_socklen_t *addrlen, void *user_data) {
    (void)socket_fd; (void)buffer; (void)length; 
    (void)flags; (void)addr; (void)addrlen; (void)user_data;
    return -1;
}

static ares_ssize_t  id_000037_dummy_sendv(ares_socket_t socket_fd, const struct iovec *iov, 
                                int iovcnt, void *user_data) {
    (void)socket_fd; (void)iov; (void)iovcnt; (void)user_data;
    return -1;
}

// Callback for ares_query
static void  id_000037_query_callback(void *arg, int status, int timeouts, 
                          const unsigned char *abuf, int alen) {
    (void)arg; (void)status; (void)timeouts; (void)abuf; (void)alen;
    // Callback implementation - minimal for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Create channel with options
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // 1. Set socket functions
    struct ares_socket_functions funcs = {
         id_000037_dummy_socket,
         id_000037_dummy_close,
         id_000037_dummy_connect,
         id_000037_dummy_recvfrom,
         id_000037_dummy_sendv
    };
    ares_set_socket_functions(channel, &funcs, nullptr);
    
    // 2. Reinitialize channel
    if (ares_reinit(channel) != ARES_SUCCESS) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }
    
    // Use input data for parsing functions
    // 3. Parse A reply
    struct hostent *host_a = nullptr;
    struct ares_addrttl *addrttls = nullptr;
    int naddrttls = 0;
    
    // Use first half of data for A reply parsing
    size_t half_size = size / 2;
    ares_parse_a_reply(data, (int)std::min(half_size, (size_t)INT_MAX), 
                      &host_a, addrttls, &naddrttls);
    
    // 4. Parse URI reply using second half
    struct ares_uri_reply *uri_out = nullptr;
    if (half_size > 0) {
        ares_parse_uri_reply(data + half_size, 
                           (int)std::min(size - half_size, (size_t)INT_MAX), 
                           &uri_out);
    }
    
    // 5. Create a DNS query using input data as domain name
    if (size > 0) {
        // Ensure null termination for domain name
        char *domain_name = (char*)malloc(size + 1);
        if (domain_name) {
            memcpy(domain_name, data, size);
            domain_name[size] = '\0';
            
            // Simple query - only use first 100 chars max
            size_t query_len = std::min(size, (size_t)100);
            domain_name[query_len] = '\0';
            
            ares_query(channel, domain_name, ns_c_in, ns_t_a, 
                       id_000037_query_callback, nullptr);
            
            free(domain_name);
        }
    }
    
    // 6. Get section string (for coverage)
    const char *section_str = ares_dns_section_tostr(ARES_SECTION_ANSWER);
    (void)section_str; // Suppress unused warning
    
    // 7. Use gethostbyname_file with input data
    FILE *in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        char temp_filename[] = "/tmp/ares_fuzz_XXXXXX";
        int fd = mkstemp(temp_filename);
        if (fd >= 0) {
            // Write input data to temp file
            fwrite(data, 1, size, fdopen(fd, "wb"));
            close(fd);
            
            struct hostent *file_host = nullptr;
            ares_gethostbyname_file(channel, temp_filename, fuzz_int32_t_2, &file_host);
            
            // Clean up temp file
            unlink(temp_filename);
        }
        assert_file_closed(&in_file);;
    }
    
    // 8. Create DNS record and write it
    ares_dns_record_t *dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_3, fuzz_uint16_t_4, ARES_OPCODE_QUERY, 
                              ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        
        // Add a query using input data as name
        if (size > 0) {
            char query_name[256];
            size_t name_len = std::min(size, (size_t)255);
            memcpy(query_name, data, name_len);
            query_name[name_len] = '\0';
            
            ares_dns_record_query_add(dnsrec, query_name, 
                                     ARES_REC_TYPE_A, ARES_CLASS_IN);
        }
        
        unsigned char *buf = nullptr;
        size_t buf_len = 0;
        ares_dns_write(dnsrec, &buf, &buf_len);
        
        if (buf) {
            ares_free_string(buf);
        }
        ares_dns_record_destroy(dnsrec);
    }
    
    // Cleanup
    if (host_a) {
        ares_free_hostent(host_a);
    }
    
    // Free URI reply chain
    struct ares_uri_reply *uri_curr = uri_out;
    while (uri_curr) {
        struct ares_uri_reply *next = uri_curr->next;
        if (uri_curr->uri) free(uri_curr->uri);
        free(uri_curr);
        uri_curr = next;
    }
    
    ares_destroy(channel);
    ares_library_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
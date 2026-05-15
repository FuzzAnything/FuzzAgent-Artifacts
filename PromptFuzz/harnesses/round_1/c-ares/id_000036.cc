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
//<ID> 816
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_socket_functions","ares_destroy","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_parse_a_reply","ares_free_hostent","ares_parse_uri_reply","ares_query","ares_gethostbyname_file","ares_free_hostent","ares_dns_record_create","ares_dns_record_query_add","ares_dns_write","ares_parse_a_reply","ares_free_hostent","ares_dns_section_tostr","ares_dns_record_destroy","ares_fds","ares_process","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_socket_functions","ares_parse_a_reply","ares_free_hostent","ares_parse_uri_reply","ares_query","ares_gethostbyname_file","ares_free_hostent","ares_dns_record_create","ares_dns_record_query_add","ares_dns_write","ares_parse_a_reply","ares_free_hostent","ares_dns_section_tostr","ares_dns_record_destroy","ares_fds","ares_process","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" {
}

// Custom socket functions structure
static ares_socket_t  id_000036_my_asocket(int domain, int type, int protocol, void* user_data) {
    return (ares_socket_t)-1;  // Return invalid socket
}

static int  id_000036_my_aclose(ares_socket_t socket_fd, void* user_data) {
    return 0;
}

static int  id_000036_my_aconnect(ares_socket_t socket_fd, const struct sockaddr* addr,
                       ares_socklen_t addrlen, void* user_data) {
    return 0;
}

static ares_ssize_t  id_000036_my_arecvfrom(ares_socket_t socket_fd, void* buf, size_t len,
                                 int flags, struct sockaddr* from,
                                 ares_socklen_t* fromlen, void* user_data) {
    return -1;
}

static ares_ssize_t  id_000036_my_asendv(ares_socket_t socket_fd, const struct iovec* vec,
                              int len, void* user_data) {
    return -1;
}

// Callback function for ares_query
static void  id_000036_query_callback(void* arg, int status, int timeouts,
                           const unsigned char* abuf, int alen) {
    // Do nothing in fuzzing context
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

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

    // Set custom socket functions
    struct ares_socket_functions funcs = {
        .asocket =  id_000036_my_asocket,
        .aclose =  id_000036_my_aclose,
        .aconnect =  id_000036_my_aconnect,
        .arecvfrom =  id_000036_my_arecvfrom,
        .asendv =  id_000036_my_asendv,
    };
    ares_set_socket_functions(channel, &funcs, nullptr);

    // Create a FILE* from input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;  // Mark as used
    (void)out_fd; // Mark as used

    // Parse A reply from input data
    struct hostent* host = nullptr;
    struct ares_addrttl addrttls[10];
    int naddrttls = 10;
    ares_parse_a_reply(data, (int)size, &host, addrttls, &naddrttls);
    
    if (host) {
        ares_free_hostent(host);
    }

    // Parse URI reply from input data
    struct ares_uri_reply* uri_out = nullptr;
    ares_parse_uri_reply(data, (int)size, &uri_out);
    
    // Free URI reply chain
    while (uri_out) {
        struct ares_uri_reply* next = uri_out->next;
        free(uri_out->uri);
        free(uri_out);
        uri_out = next;
    }

    // Perform DNS query with part of input data as hostname
    // Extract null-terminated hostname from input
    size_t hostname_len = (size < 256) ? size : 255;
    char* hostname = (char*)malloc(hostname_len + 1);
    if (hostname) {
        memcpy(hostname, data, hostname_len);
        hostname[hostname_len] = '\0';
        
        // Use ares_query
        ares_query(channel, hostname, C_IN, T_A,  id_000036_query_callback, nullptr);
        
        free(hostname);
    }

    // Try to get host by name from file (using input file as mock)
    struct hostent* file_host = nullptr;
    char test_hostname[] = "localhost";
    ares_gethostbyname_file(channel, fuzz_str_1, fuzz_int32_t_2, &file_host);
    
    if (file_host) {
        ares_free_hostent(file_host);
    }

    // Create DNS record and write it to buffer
    ares_dns_record_t* dnsrec = nullptr;
    unsigned char* buf = nullptr;
    size_t buf_len = 0;
    
    if (ares_dns_record_create(&dnsrec, fuzz_uint16_t_3, fuzz_uint16_t_4, ARES_OPCODE_QUERY,
                              ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        // Add a query
        ares_dns_record_query_add(dnsrec, fuzz_str_5, ARES_REC_TYPE_A,
                                 ARES_CLASS_IN);
        
        // Write DNS record to buffer
        ares_dns_write(dnsrec, &buf, &buf_len);
        
        // Parse the written buffer as A reply
        struct hostent* written_host = nullptr;
        struct ares_addrttl written_addrttls[5];
        int written_naddrttls = 5;
        ares_parse_a_reply(buf, (int)buf_len, &written_host, written_addrttls,
                          &written_naddrttls);
        
        if (written_host) {
            ares_free_hostent(written_host);
        }
        
        // Convert DNS section to string (using ARES_SECTION_ANSWER)
        const char* section_str = ares_dns_section_tostr(ARES_SECTION_ANSWER);
        (void)section_str;  // Mark as used
        
        // Clean up
        if (buf) {
            free(buf);
        }
        ares_dns_record_destroy(dnsrec);
    }

    // Process any pending operations
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    if (nfds > 0) {
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        ares_process(channel, &read_fds, &write_fds);
    }

    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    ares_destroy(channel);
    ares_library_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
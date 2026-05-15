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
//<ID> 803
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_socket_callback","ares_destroy","ares_library_cleanup","ares_dns_parse","ares_dns_record_set_id","ares_dns_record_get_flags","ares_dns_record_rr_add","ares_dns_rr_get_name","ares_dns_rr_key_to_rec_type","ares_dns_opt_get_datatype","ares_dns_record_destroy","ares_destroy","ares_library_cleanup","ares_getaddrinfo","ares_fds","ares_timeout","ares_process","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_socket_callback","ares_dns_parse","ares_dns_record_set_id","ares_dns_record_get_flags","ares_dns_record_rr_add","ares_dns_rr_get_name","ares_dns_rr_key_to_rec_type","ares_dns_opt_get_datatype","ares_dns_record_destroy","ares_getaddrinfo","ares_fds","ares_timeout","ares_process","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Forward declaration for callback
static void  id_000034_getaddrinfo_callback(void* arg, int status, int timeouts,
                                 struct ares_addrinfo* result);

// Socket creation callback
static int  id_000034_socket_create_callback(ares_socket_t socket_fd, int type, void* data);

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 2) {
        return 0;
    }

    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t* channel = nullptr;
    if (ares_init_options(&channel, nullptr, 0) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Set socket callback
    ares_set_socket_callback(channel,  id_000034_socket_create_callback, nullptr);

    // Create FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Parse DNS record from input data
    ares_dns_record_t* dnsrec = nullptr;
    unsigned int parse_flags = 0;
    
    // Read the entire buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (file_size > 0) {
        unsigned char* buf = (unsigned char*)malloc(file_size);
        if (buf) {
            size_t bytes_read = fread(buf, 1, file_size, in_file);
            if (bytes_read > 0) {
                // Try to parse as DNS record
                ares_dns_parse(buf, bytes_read, parse_flags, &dnsrec);
            }
            free(buf);
        }
    }

    assert_file_closed(&in_file);;

    // If we parsed a DNS record, use it
    if (dnsrec) {
        // Set DNS record ID using first 2 bytes of input
        unsigned short id = 0;
        if (size >= 2) {
            id = (data[0] << 8) | data[1];
        }
        ares_dns_record_set_id(dnsrec, id);

        // Get flags from DNS record
        unsigned short flags = ares_dns_record_get_flags(dnsrec);
        (void)flags; // Use flags to avoid unused variable warning

        // Try to add a resource record
        ares_dns_rr_t* rr_out = nullptr;
        ares_status_t status = ares_dns_record_rr_add(
            &rr_out, dnsrec, ARES_SECTION_ANSWER,
            "test.example.com",  // Name
            ARES_REC_TYPE_A,     // Type
            ARES_CLASS_IN,       // Class
            300                  // TTL
        );

        if (status == ARES_SUCCESS && rr_out) {
            // Get the name from the resource record
            const char* rr_name = ares_dns_rr_get_name(rr_out);
            (void)rr_name; // Use to avoid unused variable warning

            // Get record type from RR key (for demonstration, use a known key)
            ares_dns_rec_type_t rec_type = ares_dns_rr_key_to_rec_type(ARES_RR_A_ADDR);
            (void)rec_type; // Use to avoid unused variable warning

            // Get datatype for an option
            ares_dns_opt_datatype_t datatype = ares_dns_opt_get_datatype(
                ARES_RR_OPT_OPTIONS, 1);
            (void)datatype; // Use to avoid unused variable warning
        }

        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // Create hints for getaddrinfo
    struct ares_addrinfo_hints hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;

    // Trigger getaddrinfo with callback
    const char* node = "localhost";
    const char* service = "http";
    
    // Create output file for callback context
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for output file
    int out_fd = fuzz_fileno(out_file);
    (void)out_fd; // Use to avoid unused variable warning

    // Call getaddrinfo
    ares_getaddrinfo(channel, node, service, &hints,
                      id_000034_getaddrinfo_callback, out_file);

    // Process for a short time to allow callback execution
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 10000; // 10ms timeout
    
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    if (nfds > 0) {
        struct timeval* timeout = ares_timeout(channel, nullptr, &tv);
        select(nfds, &read_fds, &write_fds, nullptr, timeout);
        ares_process(channel, &read_fds, &write_fds);
    }

    // Cancel any pending operations
    ares_cancel(channel);

    // Close output file
    assert_file_closed(&out_file);;

    // Cleanup
    ares_destroy(channel);
    ares_library_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
}

// Callback implementation for getaddrinfo
static void  id_000034_getaddrinfo_callback(void* arg, int status, int timeouts,
                                 struct ares_addrinfo* result) {
    FILE* out_file = (FILE*)arg;
    
    if (out_file && result) {
        // Write something to indicate callback was called
        fwrite("GETADDRINFO_CALLBACK\n", 1, 21, out_file);
        ares_freeaddrinfo(result);
    }
}

// Socket creation callback implementation
static int  id_000034_socket_create_callback(ares_socket_t socket_fd, int type, void* data) {
    // This is a simple callback that does nothing special
    (void)socket_fd;
    (void)type;
    (void)data;
    return 0; // Return 0 on success
}
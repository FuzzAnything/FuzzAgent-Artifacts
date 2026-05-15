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
#include <multi.h>
}
//<ID> 690
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 1
//<Quality> {"density":22,"unique_branches":{"Curl_getformdata":[[820,14,820,19,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_slist_append","curl_mime_headers","curl_mime_data","curl_formadd","curl_formget","curl_multi_add_handle","curl_multi_socket_action","curl_multi_socket_action","curl_multi_waitfds","curl_easy_header","curl_mprintf","curl_maprintf","curl_free","curl_mime_free","curl_formfree","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_slist_append","curl_mime_headers","curl_mime_data","curl_formadd","curl_formget","curl_multi_add_handle","curl_multi_socket_action","curl_multi_socket_action","curl_multi_waitfds","curl_easy_header","curl_mprintf","curl_maprintf","curl_free","curl_mime_free","curl_formfree","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<57) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Initialize CURL globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create file pointers for input/output
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    
    if (!in_file || !out_file) {
        if (in_file) assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Create mime structure
    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        // Add mime part
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use input data as filename (ensure null termination)
            std::string filename;
            if (size > 0) {
                size_t name_len = size > 100 ? 100 : size;
                filename.assign((const char*)data, name_len);
                // Ensure null termination
                if (filename.back() != '\0') {
                    filename.push_back('\0');
                }
            } else {
                filename = "test.bin";
            }
            
            // Use curl_mime_filename API
            curl_mime_filename(part, filename.c_str());
            
            // Create headers for mime part
            struct curl_slist *headers = curl_slist_append(nullptr, fuzz_str_1);
            if (headers) {
                // Use curl_mime_headers API
                curl_mime_headers(part, headers, 1); // take_ownership = 1
            }
            
            // Set mime data from input
            curl_mime_data(part, (const char*)data, size);
        }
    }
    
    // Create HTTP POST form (old style) for curl_formget
    struct curl_httppost *formpost = nullptr;
    struct curl_httppost *lastptr = nullptr;
    
    // Add form field with input data
    if (size > 0) {
        // Ensure null-terminated string for form name
        std::string field_name;
        size_t name_len = size > 50 ? 50 : size;
        field_name.assign((const char*)data, name_len);
        if (field_name.back() != '\0') {
            field_name.push_back('\0');
        }
        
        // Create form entry
        curl_formadd(&formpost, &lastptr,
                    CURLFORM_COPYNAME, field_name.c_str(),
                    CURLFORM_COPYCONTENTS, (const char*)data,
                    CURLFORM_CONTENTSLENGTH, (long)size,
                    CURLFORM_END);
    }
    
    // Use curl_formget API
    if (formpost) {
        // Callback function to append form data
        auto formget_callback = [](void *arg, const char *buf, size_t len) -> size_t {
            FILE *f = (FILE*)arg;
            if (f && buf && len > 0) {
                return fwrite(buf, 1, len, f);
            }
            return 0;
        };
        
        // Get form data
        curl_formget(formpost, out_file, formget_callback);
    }
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    // Setup socket for multi interface
    curl_socket_t sockfd = (curl_socket_t)in_fd;
    int running_handles = 0;
    
    // Use curl_multi_socket API
    curl_multi_socket(multi, sockfd, &running_handles);
    
    // Use curl_multi_socket_action API
    curl_multi_socket_action(multi, sockfd, fuzz_int32_t_2, &running_handles);
    
    // Setup waitfds structure
    struct curl_waitfd ufds[2];
    ufds[0].fd = in_fd;
    ufds[0].events = CURL_WAIT_POLLIN;
    ufds[0].revents = 0;
    
    ufds[1].fd = out_fd;
    ufds[1].events = CURL_WAIT_POLLOUT;
    ufds[1].revents = 0;
    
    unsigned int fd_count = 0;
    
    // Use curl_multi_waitfds API
    curl_multi_waitfds(multi, ufds, 2, &fd_count);
    
    // Try to get header using curl_easy_header
    struct curl_header *header = nullptr;
    if (size > 10) {
        // Use first 10 bytes as header name (null-terminated)
        std::string header_name;
        size_t name_len = size > 10 ? 10 : size;
        header_name.assign((const char*)data, name_len);
        if (header_name.back() != '\0') {
            header_name.push_back('\0');
        }
        
        // Try to get header
        curl_easy_header(easy, header_name.c_str(), fuzz_uint64_t_3, CURLH_HEADER, fuzz_int32_t_4, &header);
    }
    
    // Use curl_mprintf and curl_maprintf APIs
    if (size > 0) {
        // Format string with curl_mprintf (prints to stdout)
        curl_mprintf("Processed %zu bytes\n", size);
        
        // Format string with curl_maprintf (allocates string)
        char *formatted = curl_maprintf("Input size: %zu, FD count: %u", size, fd_count);
        if (formatted) {
            // Could write to file or use in some way
            fwrite(formatted, 1, strlen(formatted), out_file);
            curl_free(formatted);
        }
    }
    
    // Cleanup
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (formpost) {
        curl_formfree(formpost);
    }
    
    curl_multi_remove_handle(multi, easy);
    curl_multi_cleanup(multi);
    curl_easy_cleanup(easy);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
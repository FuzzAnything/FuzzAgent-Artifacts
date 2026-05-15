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
#include <mprintf.h>
}
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 210, nr_unique_branch: 16
//<Quality> {"density":21,"unique_branches":{"cf_ip_happy_connect":[[794,3,794,18,0,0,4,1],[802,3,802,21,0,0,4,0]],"async_thrdd_get_ai":[[766,14,766,16,0,0,4,1]],"cf_dns_connect":[[208,6,208,19,0,0,4,1]],"Curl_async_get_ai":[[790,8,790,23,0,0,4,0]],"Curl_thrdq_recv":[[312,6,312,7,0,0,4,0]],"async_thrdd_report_item":[[480,19,480,55,0,0,4,0],[319,4,319,10,2,0,4,0],[319,14,319,33,2,0,4,1]],"Curl_async_thrdd_multi_process":[[516,9,516,54,0,0,4,0],[522,8,522,12,0,0,4,0],[524,8,524,13,0,0,4,0],[532,10,532,44,0,0,4,0],[535,10,535,17,0,0,4,0]],"thrdq_item_destroy":[[87,6,87,17,0,0,4,1]],"curl_mime_encoder":[[1388,8,1388,42,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_data","curl_easy_setopt","curl_url","curl_url_set","curl_url_get","curl_free","curl_url_cleanup","curl_unescape","curl_free","curl_easy_unescape","curl_free","curl_formadd","curl_formfree","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_fdset","curl_multi_socket_action","curl_easy_pause","curl_easy_pause","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_data","curl_easy_setopt","curl_url","curl_url_set","curl_url_get","curl_free","curl_url_cleanup","curl_unescape","curl_free","curl_easy_unescape","curl_free","curl_formadd","curl_formfree","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_fdset","curl_multi_socket_action","curl_easy_pause","curl_easy_pause","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified curl APIs together:

*/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 1. Create multi handle and easy handle
    CURLM *multi_handle = curl_multi_init();
    CURL *easy_handle = curl_easy_init();
    
    if (multi_handle && easy_handle) {
        // 2. Create mime structure and set encoder
        curl_mime *mime = curl_mime_init(easy_handle);
        if (mime) {
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // Use fuzzer data as mime data
                char *mime_data = (char *)malloc(size + 1);
                if (mime_data) {
                    memcpy(mime_data, data, size);
                    mime_data[size] = 0;
                    
                    // Use curl_mime_encoder
                    curl_mime_encoder(part, "base64");
                    curl_mime_data(part, mime_data, size);
                    free(mime_data);
                }
            }
            
            // Set mime as POST data
            curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
        }
        
        // 3. Create URL handle and get URL parts
        CURLU *url_handle = curl_url();
        if (url_handle) {
            // Create a URL string from fuzzer data
            char *url_data = (char *)malloc(size + 20);
            if (url_data) {
                snprintf(url_data, size + 20, "http://%.*s.com", (int)(size > 100 ? 100 : size), data);
                
                // Set URL
                curl_url_set(url_handle, CURLUPART_URL, url_data, 0);
                
                // Get host part from URL
                char *host_part = nullptr;
                curl_url_get(url_handle, CURLUPART_HOST, &host_part, 0);
                
                // Clean up URL parts
                if (host_part) {
                    curl_free(host_part);
                }
                free(url_data);
            }
            curl_url_cleanup(url_handle);
        }
        
        // 4. Use unescape functions
        if (size > 0) {
            // Use curl_unescape
            char *unescaped1 = curl_unescape((const char *)data, (int)(size > 100 ? 100 : size));
            if (unescaped1) {
                curl_free(unescaped1);
            }
            
            // Use curl_easy_unescape
            int outlength = 0;
            char *unescaped2 = curl_easy_unescape(easy_handle, (const char *)data, 
                                                  (int)(size > 100 ? 100 : size), &outlength);
            if (unescaped2) {
                curl_free(unescaped2);
            }
        }
        
        // 5. Create and free form
        struct curl_httppost *formpost = nullptr;
        struct curl_httppost *lastptr = nullptr;
        
        // Try to add form data from fuzzer input
        if (size > 10) {
            char *field_name = (char *)malloc(10);
            char *field_value = (char *)malloc(size - 5);
            if (field_name && field_value) {
                memcpy(field_name, data, 9);
                field_name[9] = 0;
                memcpy(field_value, data + 10, size - 10);
                field_value[size - 10] = 0;
                
                curl_formadd(&formpost, &lastptr,
                            CURLFORM_COPYNAME, field_name,
                            CURLFORM_COPYCONTENTS, field_value,
                            CURLFORM_END);
                
                free(field_name);
                free(field_value);
            }
        }
        
        // Free the form (curl_formfree)
        if (formpost) {
            curl_formfree(formpost);
        }
        
        // 6. Set up easy handle options
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
        
        // 7. Add easy handle to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
        
        // 8. Use curl_multi_fdset
        fd_set read_fd_set, write_fd_set, exc_fd_set;
        FD_ZERO(&read_fd_set);
        FD_ZERO(&write_fd_set);
        FD_ZERO(&exc_fd_set);
        int max_fd = 0;
        
        // Add our file descriptors to the sets
        FD_SET(in_fd, &read_fd_set);
        if (in_fd > max_fd) max_fd = in_fd;
        FD_SET(out_fd, &write_fd_set);
        if (out_fd > max_fd) max_fd = out_fd;
        
        curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);
        
        // 9. Use curl_multi_socket
        int running_handles = 0;
        curl_multi_socket(multi_handle, in_fd, &running_handles);
        
        // 10. Use curl_easy_pause
        curl_easy_pause(easy_handle, CURLPAUSE_RECV);
        curl_easy_pause(easy_handle, CURLPAUSE_CONT);
        
        // 11. Perform a minimal multi operation
        int still_running = 0;
        curl_multi_perform(multi_handle, &still_running);
        
        // Clean up multi handle
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_multi_cleanup(multi_handle);
        
        // Clean up mime
        if (mime) {
            curl_mime_free(mime);
        }
    }
    
    // Clean up easy handle
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Cleanup global libcurl
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
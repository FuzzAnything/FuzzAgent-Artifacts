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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_getenv","curl_easy_init","curl_url","curl_url_set","curl_url_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_free","curl_formadd","curl_multi_init","curl_multi_add_handle","curl_easy_send","curl_easy_strerror","curl_easy_recv","curl_easy_strerror","curl_multi_socket_all","curl_multi_remove_handle","curl_multi_cleanup","curl_formfree","curl_easy_cleanup","curl_free","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_getenv","curl_easy_init","curl_url","curl_url_set","curl_url_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_free","curl_formadd","curl_multi_init","curl_multi_add_handle","curl_easy_send","curl_easy_strerror","curl_easy_recv","curl_easy_strerror","curl_multi_socket_all","curl_multi_remove_handle","curl_multi_cleanup","curl_formfree","curl_easy_cleanup","curl_free","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<401) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeCharArray(char, fuzz_char_array_1, fuzz_char_array_size_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeCharArray(char, fuzz_char_array_3, fuzz_char_array_size_3, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_4, fuzz_str_sz_4, fdp);
	
		const char * fuzzer_var_curl_getenv_2_0 = fuzz_char_array_1;
		const char * fuzzer_var_curl_mime_data_9_1 = fuzz_char_array_3;
		size_t fuzzer_size_curl_mime_data_9_1 = static_cast<size_t>(fuzz_char_array_size_3);//fuzzer shim end}




    // Initialize global curl environment
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a file handle for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a file handle for writing output
    FILE *out_file = fopen("output_file", "wb");
    
    // Get environment variable (using first part of data as variable name)
    char env_var[256] = {0};
    size_t env_var_len = size > 255 ? 255 : size;
    if (size > 0) {
        memcpy(env_var, data, env_var_len);
        env_var[env_var_len] = '\0';
        // Set environment variable first so curl_getenv can find it
        setenv(env_var, "fuzzer_test_value", 1);
    }
    char *env_value = curl_getenv(fuzzer_var_curl_getenv_2_0);
    
    // Create easy handle
    CURL *easy = curl_easy_init();
    if (easy) {
        // Create URL handle
        CURLU *url = curl_url();
        if (url) {
            // Set URL parts using data
            char url_part[256] = {0};
            size_t url_len = size > 128 ? 128 : size/2;
            if (size > 0) {
                memcpy(url_part, data, url_len);
                url_part[url_len] = '\0';
                curl_url_set(url, CURLUPART_URL, url_part, fuzz_uint32_t_2);
            }
            curl_url_cleanup(url);
        }
        
        // Create MIME structure and add parts
        curl_mime *mime = curl_mime_init(easy);
        if (mime) {
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // Use some data for mime
                if (size > 0) {
                    char mime_data[128] = {0};
                    size_t mime_len = size > 127 ? 127 : size;
                    memcpy(mime_data, data, mime_len);
                    curl_mime_data(part, fuzzer_var_curl_mime_data_9_1, fuzzer_size_curl_mime_data_9_1);
                }
            }
            curl_mime_free(mime);
        }
        
        // Create form data
        struct curl_httppost *formpost = NULL;
        struct curl_httppost *lastptr = NULL;
        if (size > 0) {
            char field_name[64] = {0};
            char field_value[64] = {0};
            size_t name_len = size > 63 ? 63 : size/2;
            size_t value_len = size > 63 ? 63 : size/2;
            
            memcpy(field_name, data, name_len);
            field_name[name_len] = '\0';
            
            // Use middle part of data for value
            size_t value_offset = size/4;
            if (value_offset + value_len > size) value_len = size - value_offset;
            memcpy(field_value, data + value_offset, value_len);
            field_value[value_len] = '\0';
            
            curl_formadd(&formpost, &lastptr,
                        CURLFORM_COPYNAME, field_name,
                        CURLFORM_COPYCONTENTS, field_value,
                        CURLFORM_END);
        }
        
        // Create multi handle
        CURLM *multi = curl_multi_init();
        if (multi) {
            // Add easy handle to multi
            curl_multi_add_handle(multi, easy);
            
            int running_handles = 0;
            // Try to send some data
            size_t sent = 0;
            char send_buf[128] = "test";
            if (size > 0) {
                size_t send_len = size > 127 ? 127 : size;
                memcpy(send_buf, data, send_len);
                send_buf[send_len] = '\0';
                CURLcode send_res = curl_easy_send(easy, fuzz_str_4, static_cast<size_t>(fuzz_str_sz_4), &sent);
                // Get error string for send result
                const char *send_err = curl_easy_strerror(send_res);
                (void)send_err; // Prevent unused variable warning
            }
            
            // Try to receive some data
            char recv_buf[128];
            size_t received = 0;
            CURLcode recv_res = curl_easy_recv(easy, recv_buf, sizeof(recv_buf), &received);
            // Get error string for receive result
            const char *recv_err = curl_easy_strerror(recv_res);
            (void)recv_err; // Prevent unused variable warning
            
            // Try multi socket operation
            curl_multi_socket_all(multi, &running_handles);
            
            // Clean up multi handle
            curl_multi_remove_handle(multi, easy);
            curl_multi_cleanup(multi);
        }
        
        // Free form data if created
        if (formpost) {
            curl_formfree(formpost);
        }
        
        // Clean up easy handle
        curl_easy_cleanup(easy);
    }
    
    // Free environment variable value
    if (env_value) {
        curl_free(env_value);
    }
    
    // Clean up file handles
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // Clean up global curl environment
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
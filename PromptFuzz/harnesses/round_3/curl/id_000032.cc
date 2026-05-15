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
//<ID> 469
//<Prompt> []
/*<Combination>: [
*/
//<score> 55, nr_unique_branch: 13
//<Quality> {"density":22,"unique_branches":{"FormAdd":[[417,5,417,30,0,0,4,0],[418,10,418,66,0,0,4,1],[422,12,422,18,0,0,4,0],[423,14,423,71,0,0,4,1],[311,26,311,31,14,0,4,1]],"Curl_getformdata":[[803,25,803,58,0,0,4,0]],"Curl_mime_prepare_headers":[[1706,5,1706,23,0,0,4,0],[1708,10,1708,22,0,0,4,0],[1710,10,1710,22,0,0,4,0],[1710,26,1710,40,0,0,4,1],[1713,5,1713,12,0,0,4,1]],"FormAddCheck":[[239,9,239,42,0,0,4,0],[240,9,240,45,0,0,4,1]]},"library_calls":["curl_global_sslset","curl_multi_init","curl_multi_setopt","curl_easy_init","curl_multi_cleanup","curl_multi_add_handle","curl_easy_pause","curl_easy_pause","curl_mime_init","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_mime_addpart","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_mime_name","curl_mime_filename","curl_mime_filedata","curl_slist_append","curl_mime_headers","curl_formadd","curl_formadd","curl_formget","curl_formfree","curl_mime_free","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup"],"critical_calls":["curl_global_sslset","curl_multi_init","curl_multi_setopt","curl_easy_init","curl_multi_add_handle","curl_easy_pause","curl_easy_pause","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_filename","curl_mime_filedata","curl_slist_append","curl_mime_headers","curl_formadd","curl_formadd","curl_formget","curl_formfree","curl_mime_free","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 10) {
	return 0;
	}
	
    
    // Initialize global SSL
    curl_global_sslset(CURLSSLBACKEND_NONE, NULL, NULL);
    
    // Create multi handle and set option
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
	return 0;
	}
	
    
    long max_connects = 5;
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_connects);
    
    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Pause and resume easy handle
    curl_easy_pause(easy_handle, CURLPAUSE_ALL);
    curl_easy_pause(easy_handle, CURLPAUSE_CONT);
    
    // Create MIME structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }
    
    // Create MIME part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }
    
    // Set MIME part properties using input data
    // Use first portion of data for name
    size_t name_len = size > 50 ? 50 : size / 2;
    char *name = (char *)malloc(name_len + 1);
    if (name) {
        memcpy(name, data, name_len);
        name[name_len] = '\0';
        curl_mime_name(part, name);
        free(name);
    }
    
    // Use next portion for filename
    size_t filename_start = name_len;
    size_t filename_len = size - filename_start > 50 ? 50 : size - filename_start;
    if (filename_len > 0) {
        char *filename = (char *)malloc(filename_len + 1);
        if (filename) {
            memcpy(filename, data + filename_start, filename_len);
            filename[filename_len] = '\0';
            curl_mime_filename(part, filename);
            
            // Create a temporary file with the input data
            FILE *temp_file = fopen("input_file", "wb");
            if (temp_file) {
                fwrite(data, 1, size, temp_file);
                assert_file_closed(&temp_file);;
                
                // Set filedata using the temporary file
                curl_mime_filedata(part, fuzz_str_1);
            }
            free(filename);
        }
    }
    
    // Create headers list
    struct curl_slist *headers = NULL;
    size_t header_start = filename_start + filename_len;
    if (header_start < size) {
        size_t header_len = size - header_start > 100 ? 100 : size - header_start;
        char *header_value = (char *)malloc(header_len + 1);
        if (header_value) {
            memcpy(header_value, data + header_start, header_len);
            header_value[header_len] = '\0';
            headers = curl_slist_append(headers, header_value);
            free(header_value);
        }
    }
    
    // Set MIME headers
    if (headers) {
        curl_mime_headers(part, headers, 1); // Take ownership
    }
    
    // Create form data using curl_formadd
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastpost = NULL;
    
    // Use a portion of data for form field name
    size_t form_name_len = size > 30 ? 30 : size / 3;
    char *form_name = (char *)malloc(form_name_len + 1);
    if (form_name) {
        memcpy(form_name, data, form_name_len);
        form_name[form_name_len] = '\0';
        
        // Use another portion for form field content
        size_t form_content_start = form_name_len;
        size_t form_content_len = size - form_content_start > 50 ? 50 : size - form_content_start;
        char *form_content = NULL;
        
        if (form_content_len > 0) {
            form_content = (char *)malloc(form_content_len + 1);
            if (form_content) {
                memcpy(form_content, data + form_content_start, form_content_len);
                form_content[form_content_len] = '\0';
                
                // Add form field
                curl_formadd(&formpost, &lastpost,
                            CURLFORM_COPYNAME, form_name,
                            CURLFORM_COPYCONTENTS, form_content,
                            CURLFORM_END);
                
                free(form_content);
            }
        }
        free(form_name);
    }
    
    // Add a file field to the form
    curl_formadd(&formpost, &lastpost,
                CURLFORM_COPYNAME, "filefield",
                CURLFORM_FILECONTENT, "input_file",
                CURLFORM_END);
    
    // Use curl_formget to process the form
    if (formpost) {
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            // Simple callback to write form data to file
            auto formget_callback = [](void *arg, const char *buf, size_t len) -> size_t {
                FILE *f = (FILE *)arg;
                return fwrite(buf, 1, len, f);
            };
            
            curl_formget(formpost, out_file, formget_callback);
            assert_file_closed(&out_file);;
        }
        
        // Free the form
        curl_formfree(formpost);
    }
    
    // Clean up MIME
    curl_mime_free(mime);
    
    // Clean up multi handle
    int still_running = 0;
    curl_multi_perform(multi_handle, &still_running);
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}
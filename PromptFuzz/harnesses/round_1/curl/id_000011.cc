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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_formadd","curl_formget","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_fdset","curl_multi_wait","curl_multi_perform","curl_formfree","curl_slist_free_all","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_formadd","curl_formget","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_fdset","curl_multi_wait","curl_multi_perform","curl_formfree","curl_slist_free_all","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<71) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeCharArray(char, fuzz_char_array_1, fuzz_char_array_size_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	
		const char * fuzzer_var_curl_mime_encoder_10_1 = fuzz_char_array_1;//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create FILE* from input data
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
    
    // Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
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
	return 0;
    }
    
    // Create MIME structure and set encoder
    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use part of input data as encoding type
            char encoding[64] = {0};
            size_t encoding_len = size < 63 ? size : 63;
            memcpy(encoding, data, encoding_len);
            encoding[encoding_len] = '\0';
            curl_mime_encoder(part, fuzzer_var_curl_mime_encoder_10_1);
        }
    }
    
    // Create form data
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Use formget callback structure
    struct FormGetData {
        FILE *fp;
        size_t total;
    } formget_data = {out_file, 0};
    
    // Formget callback function
    auto formget_callback = [](void *arg, const char *buffer, size_t len) -> size_t {
        FormGetData *data = (FormGetData *)arg;
        return fwrite(buffer, 1, len, data->fp);
    };
    
    // Add form fields using data from input
    const char *field_name = "fuzz_field";
    const char *field_value = (const char *)data;
    size_t field_len = size < 1024 ? size : 1024;
    
    // Create temporary string for form value
    char *form_value = (char *)malloc(field_len + 1);
    if (form_value) {
        memcpy(form_value, field_value, field_len);
        form_value[field_len] = '\0';
        
        curl_formadd(&formpost, &lastptr,
                    CURLFORM_COPYNAME, field_name,
                    CURLFORM_COPYCONTENTS, form_value,
                    CURLFORM_END);
        
        // Call curl_formget
        if (formpost) {
            curl_formget(formpost, &formget_data, formget_callback);
        }
        
        free(form_value);
    }
    
    // Create linked list and append data
    struct curl_slist *headers = NULL;
    char header_data[256];
    snprintf(header_data, sizeof(header_data), "X-Fuzz-Data: %zu bytes", size);
    headers = curl_slist_append(headers, header_data);
    
    // Set headers for easy handle
    if (headers) {
        curl_easy_setopt(easy, CURLOPT_HTTPHEADER, headers);
    }
    
    // Configure easy handle with FILE descriptor
    int in_fd = fuzz_fileno(in_file);
    curl_easy_setopt(easy, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy, CURLOPT_UPLOAD, 1L);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    // Setup fd_set for select
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    int max_fd = -1;
    
    // Initialize fd_sets
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);
    
    // Get file descriptors from multi handle
    curl_multi_fdset(multi, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);
    
    // Add our file descriptor to the sets if valid
    if (in_fd >= 0) {
        FD_SET(in_fd, &read_fd_set);
        if (in_fd > max_fd) max_fd = in_fd;
    }
    
    // Wait for activity with timeout
    int ret;
    int timeout_ms = 100; // Short timeout for fuzzing
    curl_multi_wait(multi, NULL, 0, fuzz_int32_t_2, &ret);
    
    // Perform multi transfers
    int still_running = 0;
    curl_multi_perform(multi, &still_running);
    
    // Cleanup
    if (formpost) {
        curl_formfree(formpost);
    }
    if (headers) {
        curl_slist_free_all(headers);
    }
    if (mime) {
        curl_mime_free(mime);
    }
    curl_multi_remove_handle(multi, easy);
    curl_multi_cleanup(multi);
    curl_easy_cleanup(easy);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Cleanup global curl
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}
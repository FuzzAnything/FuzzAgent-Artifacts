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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.5, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_formadd","curl_formget","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_fdset","curl_multi_wait","curl_multi_perform","curl_mime_free","curl_formfree","curl_slist_free_all","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_formadd","curl_formget","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_fdset","curl_multi_wait","curl_multi_perform","curl_mime_free","curl_formfree","curl_slist_free_all","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Define callback function for curl_formget
static size_t  id_000012_formget_callback(void *arg, const char *data, size_t size) {
    FILE *out_file = reinterpret_cast<FILE*>(arg);
    return fwrite(data, 1, size, out_file);
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<41) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // 1. Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 2. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create FILE* for output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Create mime structure and set encoder
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use part of input data as encoding type
            size_t encoding_len = size > 10 ? 10 : size;
            char encoding[11];
            memcpy(encoding, data, encoding_len);
            encoding[encoding_len] = '\0';
            
            // Call curl_mime_encoder with encoding from input
            curl_mime_encoder(part, encoding);
        }
    }
    
    // 7. Create form data using curl_formget
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Use input data for form field
    char *field_data = (char*)malloc(size + 1);
    if (field_data) {
        memcpy(field_data, data, size);
        field_data[size] = '\0';
        
        curl_formadd(&formpost, &lastptr,
                    CURLFORM_COPYNAME, "fuzzfield",
                    CURLFORM_COPYCONTENTS, field_data,
                    CURLFORM_END);
        
        // Call curl_formget with callback
        curl_formget(formpost, out_file,  id_000012_formget_callback);
        
        free(field_data);
    }
    
    // 8. Create slist and append data from input
    struct curl_slist *slist = NULL;
    size_t slist_data_len = size > 50 ? 50 : size;
    char *slist_data = (char*)malloc(slist_data_len + 1);
    if (slist_data) {
        memcpy(slist_data, data, slist_data_len);
        slist_data[slist_data_len] = '\0';
        
        // Call curl_slist_append multiple times
        slist = curl_slist_append(slist, slist_data);
        slist = curl_slist_append(slist, fuzz_str_1);
        
        free(slist_data);
    }
    
    // 9. Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, slist);
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    
    // Use input file for reading
    fseek(in_file, 0, SEEK_SET);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    
    // 10. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // 11. Call curl_multi_fdset
    fd_set read_fds, write_fds, exc_fds;
    int max_fd = -1;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&exc_fds);
    
    curl_multi_fdset(multi_handle, &read_fds, &write_fds, &exc_fds, &max_fd);
    
    // 12. Call curl_multi_wait with timeout from input
    int ret;
    int timeout_ms = (size > 0) ? (data[0] % 1000) : 100; // Use first byte for timeout
    curl_multi_wait(multi_handle, NULL, 0, timeout_ms, &ret);
    
    // 13. Perform a single multi read
    int still_running = 0;
    curl_multi_perform(multi_handle, &still_running);
    
    // 14. Cleanup
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (formpost) {
        curl_formfree(formpost);
    }
    
    if (slist) {
        curl_slist_free_all(slist);
    }
    
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
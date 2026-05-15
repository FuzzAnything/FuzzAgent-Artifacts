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
//<ID> 436
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_version","curl_easy_option_by_name","curl_escape","curl_free","curl_formadd","curl_easy_init","curl_easy_setopt","curl_easy_getinfo","curl_easy_cleanup","curl_multi_init","curl_multi_poll","curl_multi_cleanup","curl_formfree","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version","curl_easy_option_by_name","curl_escape","curl_free","curl_formadd","curl_easy_init","curl_easy_setopt","curl_easy_getinfo","curl_easy_cleanup","curl_multi_init","curl_multi_poll","curl_multi_cleanup","curl_formfree","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a file from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // 1. Get curl version
    char *version = curl_version();
    if (version) {
        // Use version to avoid optimization
        volatile char dummy = *version;
        (void)dummy;
    }
    
    // 2. Get easy option by name
    const struct curl_easyoption *opt = curl_easy_option_by_name("URL");
    if (opt) {
        // Use option to avoid optimization
        volatile CURLoption dummy_opt = opt->id;
        (void)dummy_opt;
    }
    
    // 3. Escape some data
    // Use first 100 bytes or entire data if smaller
    size_t escape_len = size > 100 ? 100 : size;
    char *escaped = curl_escape((const char *)data, (int)escape_len);
    if (escaped) {
        // Use escaped string to avoid optimization
        volatile char dummy = *escaped;
        (void)dummy;
        curl_free(escaped);
    }
    
    // 4. Create form data
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastpost = NULL;
    
    // Use middle section of data for form content (avoiding beginning/end)
    size_t form_data_len = size > 50 ? size - 50 : size;
    const char *form_content = NULL;
    char *temp_form_buf = NULL;
    
    if (form_data_len > 0) {
        // Ensure null-terminated string for form content
        temp_form_buf = (char *)malloc(form_data_len + 1);
        if (temp_form_buf) {
            size_t offset = size > 25 ? 25 : 0;
            size_t copy_len = form_data_len > (size - offset) ? (size - offset) : form_data_len;
            memcpy(temp_form_buf, data + offset, copy_len);
            temp_form_buf[copy_len] = '\0';
            form_content = temp_form_buf;
        }
    }
    
    if (form_content) {
        curl_formadd(&formpost, &lastpost,
                    CURLFORM_COPYNAME, "fuzzfield",
                    CURLFORM_COPYCONTENTS, form_content,
                    CURLFORM_END);
    }
    
    // 5. Create easy handle and get info
    CURL *easy = curl_easy_init();
    if (easy) {
        // Set some dummy options
        curl_easy_setopt(easy, CURLOPT_URL, "file:///dev/null");
        
        // Get info from easy handle
        long response_code = 0;
        curl_easy_getinfo(easy, CURLINFO_RESPONSE_CODE, &response_code);
        
        // Use response_code to avoid optimization
        volatile long dummy_rc = response_code;
        (void)dummy_rc;
        
        curl_easy_cleanup(easy);
    }
    
    // 6. Create multi handle and use poll
    CURLM *multi = curl_multi_init();
    if (multi) {
        // Create waitfd structure for our input file
        struct curl_waitfd wait_fd;
        wait_fd.fd = in_fd;
        wait_fd.events = CURL_WAIT_POLLIN;
        wait_fd.revents = 0;
        
        int ret = 0;
        // Poll with 0 timeout (non-blocking)
        curl_multi_poll(multi, &wait_fd, 1, 0, &ret);
        
        // Use ret to avoid optimization
        volatile int dummy_ret = ret;
        (void)dummy_ret;
        
        curl_multi_cleanup(multi);
    }
    
    // Cleanup form data
    if (formpost) {
        curl_formfree(formpost);
    }
    
    // Free temporary buffer
    if (temp_form_buf) {
        free(temp_form_buf);
    }
    
    // Close input file
    assert_file_closed(&in_file);;
    
    // Global cleanup
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}
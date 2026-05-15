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
//<ID> 947
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"curl_mvaprintf":[[1148,6,1148,27,0,0,4,1]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_formget","curl_maprintf","curl_free","curl_mprintf","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_ws_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_formget","curl_maprintf","curl_free","curl_mprintf","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_ws_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Form append callback to consume data
static size_t  id_000052_form_append_callback(void *arg, const char *data, size_t size) {
    // We'll just count bytes here for demonstration
    size_t *total = static_cast<size_t*>(arg);
    if (total) {
        *total += size;
    }
    return size;
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. Initialize curl global state
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
        // Even if init fails, try to continue with what we have
    }
    
    // 2. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // 3. Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 4. Create a form and use formget to process form data
    // We'll create a minimal form structure
    struct curl_httppost *form = NULL;
    struct curl_httppost *last = NULL;
    
    // Use a small part of input data for form name
    size_t name_len = size > 10 ? 10 : size;
    char *name_copy = static_cast<char*>(malloc(name_len + 1));
    if (name_copy) {
        memcpy(name_copy, data, name_len);
        name_copy[name_len] = 0;
        
        // Create a form entry using the API pattern (even though curl_formadd isn't in our list)
        // We'll simulate form data by manually setting up the structure
        form = static_cast<struct curl_httppost*>(calloc(1, sizeof(struct curl_httppost)));
        if (form) {
            form->name = name_copy;
            form->contents = name_copy; // Reuse for simplicity
            
            // Use curl_formget to process the form
            size_t form_output_size = 0;
            int formget_res = curl_formget(form, &form_output_size,  id_000052_form_append_callback);
            // Ignore result - just for API coverage
        }
    }
    
    // 5. Use maprintf with format string from input
    // Use a safe portion of input as format string
    size_t fmt_len = size > 20 ? 20 : size;
    char *format_str = static_cast<char*>(malloc(fmt_len + 1));
    if (format_str) {
        memcpy(format_str, data, fmt_len);
        format_str[fmt_len] = 0;
        
        char *maprintf_result = curl_maprintf("%s", format_str);
        if (maprintf_result) {
            curl_free(maprintf_result);
        }
        free(format_str);
    }
    
    // 6. Use mprintf to output something
    int mprintf_res = curl_mprintf("Fuzz test: %zu bytes processed\n", size);
    
    // 7. Create a FILE* from input data for potential use
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Could use the file descriptor if needed
        int fd = fuzz_fileno(in_file);
        (void)fd; // Mark as used
        
        assert_file_closed(&in_file);;
    }
    
    // 8. Call curl_easy_upkeep (requires a handle that's been used)
    // Set some options first
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost");
    curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);
    
    CURLcode upkeep_res = curl_easy_upkeep(easy_handle);
    // Ignore result - just for API coverage
    
    // 9. Try to receive WebSocket data (will likely fail but exercise the API)
    char ws_buffer[1024];
    size_t ws_recv_len = 0;
    const struct curl_ws_frame *meta = NULL;
    if (size > 0) {
        CURLcode ws_res = curl_ws_recv(easy_handle, ws_buffer, 
                                      size > 1024 ? 1024 : size, 
                                      &ws_recv_len, &meta);
        // Ignore result - just for API coverage
    }
    
    // 10. Add easy handle to multi handle first (required for removal)
    // Note: curl_multi_add_handle isn't in our list but we need it for proper flow
    // We'll still call remove_handle even if not added (should return error)
    CURLMcode remove_res = curl_multi_remove_handle(multi_handle, easy_handle);
    // Ignore result - just for API coverage
    
    // 11. Clean up form if created
    if (form) {
        if (form->name == name_copy) {
            form->name = NULL; // Don't double free
        }
        if (form->contents == name_copy) {
            form->contents = NULL; // Don't double free
        }
        free(form);
    }
    
    if (name_copy) {
        free(name_copy);
    }
    
    // 12. Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    // 13. Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // 14. Clean up global state
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
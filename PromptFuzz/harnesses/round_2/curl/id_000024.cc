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
//<ID> 318
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 45
//<Quality> {"density":13,"unique_branches":{"curl_easy_strerror":[[44,3,44,25,0,0,4,0],[54,3,54,35,0,0,4,0],[63,3,63,32,0,0,4,0],[66,3,66,34,0,0,4,0],[69,3,69,31,0,0,4,0],[75,3,75,29,0,0,4,0],[78,3,78,34,0,0,4,0],[84,3,84,34,0,0,4,0],[87,3,87,31,0,0,4,0],[90,3,90,19,0,0,4,0],[102,3,102,25,0,0,4,0],[105,3,105,33,0,0,4,0],[114,3,114,24,0,0,4,0],[117,3,117,27,0,0,4,0],[123,3,123,29,0,0,4,0],[126,3,126,34,0,0,4,0],[129,3,129,25,0,0,4,0],[132,3,132,31,0,0,4,0],[135,3,135,33,0,0,4,0],[147,3,147,33,0,0,4,0],[150,3,150,35,0,0,4,0],[153,3,153,30,0,0,4,0],[156,3,156,32,0,0,4,0],[159,3,159,28,0,0,4,0],[162,3,162,34,0,0,4,0],[168,3,168,33,0,0,4,0],[183,3,183,29,0,0,4,0],[192,3,192,32,0,0,4,0],[195,3,195,34,0,0,4,0],[201,3,201,28,0,0,4,0],[210,3,210,30,0,0,4,0],[216,3,216,26,0,0,4,0],[219,3,219,27,0,0,4,0],[225,3,225,30,0,0,4,0],[237,3,237,29,0,0,4,0],[252,3,252,32,0,0,4,0],[255,3,255,31,0,0,4,0],[258,3,258,26,0,0,4,0],[264,3,264,38,0,0,4,0],[267,3,267,35,0,0,4,0],[279,3,279,19,0,0,4,0],[282,3,282,32,0,0,4,0],[285,3,285,19,0,0,4,0],[291,3,291,32,0,0,4,0],[301,3,301,10,0,0,4,0]]},"library_calls":["curl_global_init_mem","curl_getenv","curl_free","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_unescape","curl_free","curl_easy_strerror","curl_msnprintf","curl_easy_setopt","curl_multi_add_handle","curl_multi_wakeup","curl_multi_perform","curl_multi_info_read","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_getenv","curl_free","curl_multi_init","curl_easy_init","curl_unescape","curl_free","curl_easy_strerror","curl_msnprintf","curl_easy_setopt","curl_multi_add_handle","curl_multi_wakeup","curl_multi_perform","curl_multi_info_read","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Custom memory callbacks for curl_global_init_mem
static void * id_000024_custom_malloc(size_t size) {
    return malloc(size);
}

static void  id_000024_custom_free(void *ptr) {
    free(ptr);
}

static void * id_000024_custom_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char * id_000024_custom_strdup(const char *str) {
    return strdup(str);
}

static void * id_000024_custom_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. Initialize curl with custom memory functions
    CURLcode global_res = curl_global_init_mem(CURL_GLOBAL_ALL,
                                               id_000024_custom_malloc,
                                               id_000024_custom_free,
                                               id_000024_custom_realloc,
                                               id_000024_custom_strdup,
                                               id_000024_custom_calloc);
    if (global_res != CURLE_OK) {
        return 0;
    }
    
    // 2. Get environment variable (using a fixed name since we can't control env in fuzzer)
    char *env_var = curl_getenv("PATH");
    if (env_var) {
        // Use it somehow - maybe in formatting
        curl_free(env_var);
    }
    
    // 3. Create a multi handle to use with curl_multi_info_read and curl_multi_wakeup
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // 4. Create an easy handle to add to multi handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 5. Use input data to create a string for unescaping
    // We'll use up to first 100 bytes or available size
    size_t unescape_len = size > 100 ? 100 : size;
    char *unescape_input = (char*)malloc(unescape_len + 1);
    if (!unescape_input) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    memcpy(unescape_input, data, unescape_len);
    unescape_input[unescape_len] = '\0';
    
    // 6. Unescape the string
    char *unescaped = curl_unescape(unescape_input, (int)unescape_len);
    if (unescaped) {
        // Use the unescaped string somehow
        curl_free(unescaped);
    }
    free(unescape_input);
    
    // 7. Use curl_msnprintf to format a string
    char buffer[256];
    // Get an error code from first byte of input
    CURLcode sample_error = (CURLcode)(data[0] % 100); // Limit to valid range
    const char *error_str = curl_easy_strerror(sample_error);
    
    // 8. Format string using curl_msnprintf
    int printed = curl_msnprintf(buffer, sizeof(buffer), 
                                "Error %d: %s", sample_error, error_str);
    
    // 9. Set up easy handle with some options
    // We'll use the input data as a URL or similar
    // Create a temporary string from input data
    size_t url_len = size > 200 ? 200 : size;
    char *url = (char*)malloc(url_len + 1);
    if (url) {
        memcpy(url, data, url_len);
        url[url_len] = '\0';
        
        // Set URL option (using file:// protocol to avoid network)
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file:///dev/null");
        
        // Add to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
        
        free(url);
    }
    
    // 10. Wake up multi handle
    curl_multi_wakeup(multi_handle);
    
    // 11. Perform some operations
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);
    
    // 12. Read multi info
    int msgs_in_queue = 0;
    CURLMsg *msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    
    // 13. Clean up
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    return 0;
}
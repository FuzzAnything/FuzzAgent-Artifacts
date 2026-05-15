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
//<ID> 995
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 1
//<Quality> {"density":22,"unique_branches":{"Curl_mime_duppart":[[1112,3,1112,21,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_unescape","curl_global_cleanup","curl_easy_init","curl_free","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_free","curl_global_cleanup","curl_strequal","curl_mime_addpart","curl_mime_data","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_duphandle","curl_multi_init","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_reset","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_free","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_unescape","curl_easy_init","curl_mime_init","curl_strequal","curl_mime_addpart","curl_mime_data","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_duphandle","curl_multi_init","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_reset","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_free","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. Initialize global curl
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
        return 0;
    }

    // 2. Create a null-terminated string from input for unescape
    char *input_str = static_cast<char*>(malloc(size + 1));
    if (!input_str) {
        curl_global_cleanup();
        return 0;
    }
    memcpy(input_str, data, size);
    input_str[size] = '\0';

    // 3. Unescape the string
    char *unescaped_str = curl_unescape(input_str, static_cast<int>(size));
    if (!unescaped_str) {
        free(input_str);
        curl_global_cleanup();
        return 0;
    }

    // 4. Initialize easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_free(unescaped_str);
        free(input_str);
        curl_global_cleanup();
        return 0;
    }

    // 5. Initialize mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_free(unescaped_str);
        free(input_str);
        curl_global_cleanup();
        return 0;
    }

    // 6. Compare strings (original vs unescaped)
    int strings_equal = curl_strequal(input_str, unescaped_str);

    // 7. Add a mime part
    curl_mimepart *mime_part = curl_mime_addpart(mime);
    if (mime_part) {
        // Use input data for mime part
        curl_mime_data(mime_part, reinterpret_cast<const char*>(data), size);
    }

    // 8. Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost/test");
    curl_easy_setopt(easy_handle, CURLOPT_CONNECT_ONLY, 1L);

    // 9. Create a duplicate handle
    CURL *dup_handle = curl_easy_duphandle(easy_handle);
    
    // 10. Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle) {
        // 11. Add both handles to multi
        if (easy_handle) {
            curl_multi_add_handle(multi_handle, easy_handle);
        }
        if (dup_handle) {
            curl_multi_add_handle(multi_handle, dup_handle);
        }

        // 12. Perform multi operations
        int running_handles = 0;
        CURLMcode multi_perform_res = curl_multi_perform(multi_handle, &running_handles);
        
        // 13. Remove handles from multi
        if (easy_handle) {
            curl_multi_remove_handle(multi_handle, easy_handle);
        }
        if (dup_handle) {
            curl_multi_remove_handle(multi_handle, dup_handle);
        }

        // 14. Cleanup multi handle
        curl_multi_cleanup(multi_handle);
    }

    // 15. Reset easy handle
    if (easy_handle) {
        curl_easy_reset(easy_handle);
    }

    // 16. Free mime structure
    curl_mime_free(mime);

    // 17. Cleanup easy handles
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    if (dup_handle) {
        curl_easy_cleanup(dup_handle);
    }

    // 18. Free allocated strings
    curl_free(unescaped_str);
    free(input_str);

    // 19. Global cleanup
    curl_global_cleanup();

    // Use the comparison result to avoid optimization
    if (strings_equal) {
        // This branch will never be taken for random inputs,
        // but prevents compiler from optimizing away the comparison
        volatile int dummy = strings_equal;
        (void)dummy;
    }

    return 0;
}
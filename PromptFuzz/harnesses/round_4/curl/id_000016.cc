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
//<ID> 85
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_multi_timeout","curl_url","curl_multi_cleanup","curl_global_cleanup","curl_url_set","curl_url_dup","curl_formadd","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_mime_name","curl_strequal","curl_ws_send","curl_mime_free","curl_easy_reset","curl_easy_cleanup","curl_formfree","curl_url_cleanup","curl_url_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_multi_timeout","curl_url","curl_url_set","curl_url_dup","curl_formadd","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_mime_name","curl_strequal","curl_ws_send","curl_mime_free","curl_easy_reset","curl_easy_cleanup","curl_formfree","curl_url_cleanup","curl_url_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include necessary curl headers

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<69) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeCharArray(char, fuzz_char_array_1, fuzz_char_array_size_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeCharArray(char, fuzz_char_array_3, fuzz_char_array_size_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
	
		const char * fuzzer_var_curl_url_set_7_2 = fuzz_char_array_1;
		const char * fuzzer_var_curl_mime_type_13_1 = fuzz_char_array_3;//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Initialize curl global
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
        return 0;
    }

    // 2. Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // 3. Get multi timeout
    long timeout_ms;
    CURLMcode timeout_res = curl_multi_timeout(multi_handle, &timeout_ms);

    // 4. Create URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // 5. Set URL part using input data
    // Use first portion of data as URL scheme
    size_t scheme_len = size > 10 ? 10 : size;
    char scheme_buf[11] = {0};
    memcpy(scheme_buf, data, scheme_len);
    CURLUcode url_set_res = curl_url_set(url_handle, CURLUPART_SCHEME, fuzzer_var_curl_url_set_7_2, fuzz_uint32_t_2);

    // 6. Duplicate URL handle
    CURLU *url_dup = curl_url_dup(url_handle);

    // 7. Create form
    struct curl_httppost *formpost = nullptr;
    struct curl_httppost *lastpost = nullptr;
    
    // Use portion of data as form field value
    size_t form_data_len = size > 20 ? 20 : size;
    char form_data_buf[21] = {0};
    if (size > 10) {
        size_t offset = scheme_len < size ? scheme_len : 0;
        size_t copy_len = size - offset;
        if (copy_len > 20) copy_len = 20;
        memcpy(form_data_buf, data + offset, copy_len);
    }
    
    CURLFORMcode form_res = curl_formadd(&formpost, &lastpost,
        CURLFORM_COPYNAME, "field1",
        CURLFORM_COPYCONTENTS, form_data_buf,
        CURLFORM_END);

    // 8. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // 9. Create mime structure
        curl_mime *mime = curl_mime_init(easy_handle);
        if (mime) {
            // 10. Add mime part
            curl_mimepart *mimepart = curl_mime_addpart(mime);
            if (mimepart) {
                // 11. Set mime type using input data
                size_t mime_type_len = size > 30 ? 10 : (size > 20 ? size - 20 : 0);
                if (mime_type_len > 0) {
                    char mime_type_buf[11] = {0};
                    size_t offset = 20 < size ? 20 : 0;
                    memcpy(mime_type_buf, data + offset, mime_type_len);
                    CURLcode mime_type_res = curl_mime_type(mimepart, fuzzer_var_curl_mime_type_13_1);
                }
                
                // 12. Set mime name
                CURLcode mime_name_res = curl_mime_name(mimepart, fuzz_str_4);
                
                // 13. String comparison
                const char *str1 = "test";
                const char *str2 = "test";
                int str_equal = curl_strequal(fuzz_str_5, fuzz_str_6);
                
                // 14. WebSocket send
                if (size > 30) {
                    size_t ws_offset = 30;
                    size_t ws_len = size - ws_offset;
                    if (ws_len > 100) ws_len = 100; // Limit size
                    
                    size_t sent = 0;
                    CURLcode ws_res = curl_ws_send(easy_handle, 
                        data + ws_offset, 
                        ws_len, 
                        &sent, 
                        fuzz_int64_t_7,  // fragsize
                        fuzz_uint32_t_8   // flags
                    );
                }
            }
            
            // 15. Free mime
            curl_mime_free(mime);
        }
        
        // 16. Reset easy handle
        curl_easy_reset(easy_handle);
        
        // 17. Cleanup easy handle
        curl_easy_cleanup(easy_handle);
    }

    // 18. Free form
    if (formpost) {
        curl_formfree(formpost);
    }

    // 19. Cleanup URL handles
    if (url_dup) {
        curl_url_cleanup(url_dup);
    }
    curl_url_cleanup(url_handle);

    // 20. Cleanup multi handle
    curl_multi_cleanup(multi_handle);

    // 21. Global cleanup
    curl_global_cleanup();

    return 0;
}
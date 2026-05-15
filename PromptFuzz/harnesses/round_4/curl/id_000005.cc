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
//<ID> 33
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 7
//<Quality> {"density":11,"unique_branches":{"getinfo_char":[[94,20,94,21,0,0,4,1]],"Curl_getformdata":[[820,14,820,19,0,0,4,1]],"FormAdd":[[407,5,407,33,0,0,4,0],[313,26,313,31,9,0,4,1]],"FormAddCheck":[[230,9,230,29,0,0,4,0],[231,9,231,42,0,0,4,1]],"FormInfoCopyField":[[127,8,127,12,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_share_init","curl_easy_escape","curl_easy_unescape","curl_free","curl_free","curl_easy_getinfo","curl_formadd","curl_formget","curl_formfree","curl_easy_ssls_export","curl_ws_start_frame","curl_easy_option_next","curl_share_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_share_init","curl_easy_escape","curl_easy_unescape","curl_free","curl_free","curl_easy_getinfo","curl_formadd","curl_formget","curl_formfree","curl_easy_ssls_export","curl_ws_start_frame","curl_easy_option_next","curl_share_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Helper function for curl_mvprintf
static int  id_000005_my_curl_mvprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = curl_mvprintf(format, args);
    va_end(args);
    return ret;
}

// SSL export callback for curl_easy_ssls_export
static CURLcode  id_000005_ssl_export_callback(CURL *easy, void *userptr,
                                    const char *session_key,
                                    const unsigned char *shmac, size_t shmac_len,
                                    const unsigned char *sdata, size_t sdata_len,
                                    curl_off_t content_len, int compressed,
                                    const char *filename, size_t filename_len) {
    // Do nothing with the data, just return OK
    (void)easy; (void)userptr; (void)session_key; (void)shmac; (void)shmac_len;
    (void)sdata; (void)sdata_len; (void)content_len; (void)compressed;
    (void)filename; (void)filename_len;
    return CURLE_OK;
}

// Formget callback for curl_formget
static size_t  id_000005_formget_callback(void *arg, const char *buffer, size_t len) {
    FILE *out_file = (FILE*)arg;
    if (out_file) {
        return fwrite(buffer, 1, len, out_file);
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Initialize CURL globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create output file for form data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
    }
    
    CURL *easy = curl_easy_init();
    CURLSH *share = curl_share_init();
    
    if (easy && share) {
        // 1. Use curl_easy_escape and curl_easy_unescape
        // Use first 100 bytes or size whichever is smaller for escape/unescape
        size_t escape_len = size > 100 ? 100 : size;
        char *escaped = curl_easy_escape(easy, (const char *)data, (int)escape_len);
        if (escaped) {
            int outlength = 0;
            char *unescaped = curl_easy_unescape(easy, escaped, -1, &outlength);
            if (unescaped) {
                curl_free(unescaped);
            }
            curl_free(escaped);
        }
        
        // 2. Use curl_easy_getinfo
        char *effective_url = nullptr;
        CURLcode res = curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &effective_url);
        (void)res; // Result intentionally unused
        
        // 3. Use curl_formget
        struct curl_httppost *formpost = nullptr;
        struct curl_httppost *lastptr = nullptr;
        
        // Add a simple form field using the input data
        curl_formadd(&formpost, &lastptr,
                     CURLFORM_COPYNAME, "fuzzfield",
                     CURLFORM_COPYCONTENTS, (const char *)data,
                     CURLFORM_CONTENTSLENGTH, (long)size,
                     CURLFORM_END);
        
        if (formpost) {
            curl_formget(formpost, out_file,  id_000005_formget_callback);
            curl_formfree(formpost);
        }
        
        // 4. Use curl_easy_ssls_export
        curl_easy_ssls_export(easy,  id_000005_ssl_export_callback, nullptr);
        
        // 5. Use curl_ws_start_frame
        // Note: This requires a WebSocket connection, but we'll call it anyway
        // The function will likely fail but that's acceptable for fuzzing
        curl_ws_start_frame(easy, CURLWS_BINARY, (curl_off_t)size);
        
        // 6. Use curl_easy_option_next
        const struct curl_easyoption *option = nullptr;
        int option_count = 0;
        while ((option = curl_easy_option_next(option)) && option_count < 5) {
            option_count++;
        }
        
        // 7. Use curl_mvprintf via helper function
         id_000005_my_curl_mvprintf("Fuzzing curl library with %zu bytes\n", size);
    }
    
    // 8. Use curl_share_cleanup
    if (share) {
        curl_share_cleanup(share);
    }
    
    // Cleanup
    if (easy) {
        curl_easy_cleanup(easy);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
}
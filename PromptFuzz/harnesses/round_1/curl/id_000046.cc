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
//<ID> 563
//<Prompt> []
/*<Combination>: [
*/
//<score> 4.5, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["curl_share_init","curl_multi_init","curl_share_cleanup","curl_easy_init","curl_multi_cleanup","curl_share_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_slist_append","curl_mime_headers","curl_strnequal","curl_share_setopt","curl_share_strerror","curl_multi_timeout","curl_multi_socket_action","curl_slist_free_all","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup"],"critical_calls":["curl_share_init","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_mime_headers","curl_strnequal","curl_share_setopt","curl_share_strerror","curl_multi_timeout","curl_multi_socket_action","curl_slist_free_all","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Initialize share object
    CURLSH *share = curl_share_init();
    if (!share) {
	return 0;
	}
	

    // 2. Create a multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_share_cleanup(share);
        return 0;
    }

    // 3. Create a mime part for headers
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        return 0;
    }

    curl_mime *mime = curl_mime_init(easy);
    curl_mimepart *part = curl_mime_addpart(mime);
    
    // 4. Prepare header string from input data
    // Ensure we don't exceed input bounds
    size_t header_len = size > 100 ? 100 : size;
    char *header_str = (char*)malloc(header_len + 1);
    if (!header_str) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        return 0;
    }
    memcpy(header_str, data, header_len);
    header_str[header_len] = '\0';
    
    // 5. Create header list and set mime headers
    struct curl_slist *headers = curl_slist_append(NULL, header_str);
    if (headers) {
        // Use the headers with take_ownership = 0 (we'll free manually)
        curl_mime_headers(part, headers, 0);
    }
    
    // 6. String comparison using input data
    // Use two parts of the input for comparison
    size_t compare_len = 10;
    if (size >= compare_len * 2) {
        const char *s1 = (const char*)data;
        const char *s2 = (const char*)data + compare_len;
        curl_strnequal(s1, s2, fuzz_uint64_t_1);
    }
    
    // 7. Get share error (deliberately cause an error for testing)
    CURLSHcode share_err = curl_share_setopt(share, CURLSHOPT_SHARE, (long)CURL_LOCK_DATA_CONNECT);
    const char *err_str = curl_share_strerror(share_err);
    (void)err_str; // Use to avoid unused variable warning
    
    // 8. Multi handle operations
    long timeout_ms;
    CURLMcode timeout_err = curl_multi_timeout(multi, &timeout_ms);
    (void)timeout_err;
    
    int running_handles = 0;
    // Use a dummy socket (-1) since we don't have real sockets in fuzzing
    curl_socket_t dummy_sock = -1;
    CURLMcode socket_err = curl_multi_socket(multi, dummy_sock, &running_handles);
    (void)socket_err;
    
    // 9. Cleanup
    if (headers) curl_slist_free_all(headers);
    free(header_str);
    curl_mime_free(mime);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_share_cleanup(share);
    
    return 0;
}
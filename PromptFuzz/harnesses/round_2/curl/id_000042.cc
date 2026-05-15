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
//<ID> 697
//<Prompt> []
/*<Combination>: [
*/
//<score> 37.333332, nr_unique_branch: 4
//<Quality> {"density":28,"unique_branches":{"setopt_long_bool":[[682,3,682,30,0,0,4,0],[716,3,716,30,0,0,4,0]],"Curl_ssl_conn_config_update":[[422,6,422,16,0,0,4,1]],"Curl_cookie_run":[[1641,6,1641,19,0,0,4,0]]},"library_calls":["curl_global_init","curl_share_init","curl_global_cleanup","curl_share_setopt","curl_share_setopt","curl_easy_init","curl_share_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_notify_disable","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_easy_setopt","curl_multi_assign","curl_multi_socket_action","curl_easy_unescape","curl_free","curl_multi_notify_enable","curl_easy_ssls_export","curl_easy_strerror","curl_share_strerror","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_share_setopt","curl_share_setopt","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_notify_disable","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_easy_setopt","curl_multi_assign","curl_multi_socket_action","curl_easy_unescape","curl_free","curl_multi_notify_enable","curl_easy_ssls_export","curl_easy_strerror","curl_share_strerror","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":2}
/**/


// Dummy callback for curl_easy_ssls_export
static CURLcode  id_000042_dummy_export_callback(CURL *handle, void *userptr,
                                     const char *session_key,
                                     const unsigned char *shmac,
                                     size_t shmac_len,
                                     const unsigned char *sdata,
                                     size_t sdata_len,
                                     curl_off_t lifetime,
                                     int flags,
                                     const char *cipher,
                                     size_t cipher_len) {
    return CURLE_OK;
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	

    // 1. Initialize global curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	

    // 2. Create share object for sharing data between easy handles
    CURLSH *share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }

    // 3. Set share options (share cookies and SSL session data)
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);

    // 4. Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // 5. Set easy handle options
    // Use input data for URL
    char *url_data = (char *)malloc(size + 1);
    if (!url_data) {
        curl_easy_cleanup(easy);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url_data, data, size);
    url_data[size] = '\0';
    
    curl_easy_setopt(easy, CURLOPT_URL, "https://example.com");
    curl_easy_setopt(easy, CURLOPT_SHARE, share);
    curl_easy_setopt(easy, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(easy, CURLOPT_SSL_VERIFYHOST, 0L);

    // 6. Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        free(url_data);
        curl_easy_cleanup(easy);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // 7. Add easy handle to multi handle
    CURLMcode add_res = curl_multi_add_handle(multi, easy);
    
    // 8. Disable notifications temporarily
    curl_multi_notify_disable(multi, CURL_WAIT_POLLIN);

    // 9. Create MIME structure for multipart form data
    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use first half of input data for MIME data
            size_t mime_size = size / 2;
            if (mime_size > 0) {
                char *mime_data = (char *)malloc(mime_size + 1);
                if (mime_data) {
                    memcpy(mime_data, data, mime_size);
                    mime_data[mime_size] = '\0';
                    curl_mime_data(part, mime_data, mime_size);
                    free(mime_data);
                }
            }
        }
        
        // Set MIME type for easy handle
        curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    }

    // 10. Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        
        // 11. Assign socket to multi handle (using file descriptor as dummy socket)
        void *sockp = NULL;
        curl_multi_assign(multi, (curl_socket_t)fd, sockp);
        
        // 12. Process socket action (dummy call)
        int running_handles = 0;
        curl_multi_socket(multi, (curl_socket_t)fd, &running_handles);
        
        assert_file_closed(&in_file);;
    }

    // 13. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // 14. Use second half of input data for unescaping
        size_t escape_offset = size / 2;
        size_t escape_size = size - escape_offset;
        if (escape_size > 0) {
            char *escaped_data = (char *)malloc(escape_size + 1);
            if (escaped_data) {
                memcpy(escaped_data, data + escape_offset, escape_size);
                escaped_data[escape_size] = '\0';
                
                int outlength = 0;
                char *unescaped = curl_easy_unescape(easy, escaped_data, escape_size, &outlength);
                if (unescaped) {
                    fwrite(unescaped, 1, outlength, out_file);
                    curl_free(unescaped);
                }
                free(escaped_data);
            }
        }
        assert_file_closed(&out_file);;
    }

    // 15. Enable notifications again
    curl_multi_notify_enable(multi, CURL_WAIT_POLLIN);

    // 16. Export SSL session data (dummy call)
    curl_easy_ssls_export(easy,  id_000042_dummy_export_callback, NULL);

    // 17. Get error strings (demonstration)
    const char *easy_error = curl_easy_strerror(CURLE_OK);
    const char *share_error = curl_share_strerror(CURLSHE_OK);
    (void)easy_error;
    (void)share_error;

    // 18. Cleanup sequence
    if (mime) {
        curl_mime_free(mime);
    }
    
    curl_multi_remove_handle(multi, easy);
    curl_multi_cleanup(multi);
    curl_easy_cleanup(easy);
    curl_share_cleanup(share);
    curl_global_cleanup();
    
    free(url_data);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
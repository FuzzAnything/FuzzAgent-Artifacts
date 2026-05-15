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
//<ID> 882
//<Prompt> []
/*<Combination>: [
*/
//<score> 33.5, nr_unique_branch: 0
//<Quality> {"density":67,"unique_branches":{},"library_calls":["curl_global_init","curl_share_init","curl_global_cleanup","curl_multi_init","curl_share_cleanup","curl_global_cleanup","curl_multi_setopt","curl_easy_init","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_slist_append","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_headers","curl_slist_free_all","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_name","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_data_cb","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_formadd","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_setopt","curl_multi_add_handle","curl_formfree","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_multi_perform","curl_multi_socket_action","curl_multi_info_read","curl_multi_remove_handle","curl_easy_perform","curl_formget","curl_unescape","curl_free","curl_share_strerror","curl_formfree","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_multi_init","curl_multi_setopt","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_mime_headers","curl_mime_name","curl_mime_data_cb","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_formadd","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_socket_action","curl_multi_info_read","curl_multi_remove_handle","curl_easy_perform","curl_formget","curl_unescape","curl_free","curl_share_strerror","curl_formfree","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":1}
/**/


// Include necessary curl headers

// Custom callback functions
static size_t  id_000049_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *fp = (FILE *)arg;
    size_t bytes_read = fread(buffer, 1, size * nitems, fp);
    if (bytes_read == 0 && feof(fp)) {
        return 0;
    }
    return bytes_read;
}

static int  id_000049_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *fp = (FILE *)arg;
    if (fseek(fp, (long)offset, origin) != 0) {
        return CURL_SEEKFUNC_CANTSEEK;
    }
    return CURL_SEEKFUNC_OK;
}

static void  id_000049_free_callback(void *arg) {
    // Nothing to free here since FILE* is managed elsewhere
}

static size_t  id_000049_formget_callback(void *arg, const char *buf, size_t len) {
    FILE *fp = (FILE *)arg;
    return fwrite(buf, 1, len, fp);
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<72) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    // Initialize global curl
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
        return 0;
    }

    // Create share object
    CURLSH *share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }

    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // Set multi handle option
    CURLMcode multi_setopt_res = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 10L);
    if (multi_setopt_res != CURLM_OK) {
        // Continue anyway
    }

    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // Create mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // Create mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // Create headers list for mime part
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, fuzz_str_1);
    if (!headers) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // Set mime headers (take ownership)
    CURLcode mime_headers_res = curl_mime_headers(part, headers, 1);
    if (mime_headers_res != CURLE_OK) {
        curl_slist_free_all(headers);
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // Set mime name
    CURLcode mime_name_res = curl_mime_name(part, fuzz_str_2);
    if (mime_name_res != CURLE_OK) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // Create FILE* from input data for mime callback
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Set mime data with callback
    CURLcode mime_data_cb_res = curl_mime_data_cb(part, (curl_off_t)size,
                                                   id_000049_read_callback,  id_000049_seek_callback,
                                                   id_000049_free_callback, in_file);
    if (mime_data_cb_res != CURLE_OK) {
        assert_file_closed(&in_file);;
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy_handle, CURLOPT_SHARE, share);
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file:///dev/null"); // Dummy URL

    // Create form data
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    CURLFORMcode formadd_res = curl_formadd(&formpost, &lastptr,
                                            CURLFORM_COPYNAME, "fuzzfield",
                                            CURLFORM_COPYCONTENTS, (const char *)data,
                                            CURLFORM_CONTENTSLENGTH, (long)size,
                                            CURLFORM_END);
    if (formadd_res != CURL_FORMADD_OK) {
        assert_file_closed(&in_file);;
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Set form data to easy handle
    curl_easy_setopt(easy_handle, CURLOPT_HTTPPOST, formpost);

    // Add easy handle to multi handle
    CURLMcode add_handle_res = curl_multi_add_handle(multi_handle, easy_handle);
    if (add_handle_res != CURLM_OK) {
        curl_formfree(formpost);
        assert_file_closed(&in_file);;
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Perform multi handle operations
    int running_handles = 0;
    int still_running = 1;
    while (still_running) {
        CURLMcode perform_res = curl_multi_perform(multi_handle, &running_handles);
        if (perform_res != CURLM_OK) {
            break;
        }
        
        // Use socket action with dummy socket
        curl_multi_socket_action(multi_handle, CURL_SOCKET_TIMEOUT, fuzz_int32_t_3, &still_running);
        
        int msgs_in_queue = 0;
        CURLMsg *msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
        if (msg && msg->msg == CURLMSG_DONE) {
            break;
        }
    }

    // Remove handle from multi
    curl_multi_remove_handle(multi_handle, easy_handle);

    // Perform easy handle operation
    CURLcode easy_perform_res = curl_easy_perform(easy_handle);
    (void)easy_perform_res; // Result intentionally unused

    // Create output file for formget
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write form data to file
        int formget_res = curl_formget(formpost, out_file,  id_000049_formget_callback);
        (void)formget_res; // Result intentionally unused
        assert_file_closed(&out_file);;
    }

    // Use unescape on a dummy string
    char *unescaped = curl_unescape(fuzz_str_4, static_cast<int>(fuzz_str_sz_4));
    if (unescaped) {
        curl_free(unescaped);
    }

    // Use share_strerror with dummy error code
    const char *error_str = curl_share_strerror(CURLSHE_OK);
    (void)error_str; // Intentionally unused

    // Cleanup
    assert_file_closed(&in_file);;
    curl_formfree(formpost);
    curl_mime_free(mime);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_share_cleanup(share);
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
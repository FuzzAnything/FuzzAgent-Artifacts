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
//<ID> 884
//<Prompt> []
/*<Combination>: [
*/
//<score> 94, nr_unique_branch: 1
//<Quality> {"density":47,"unique_branches":{"curl_mime_headers":[[1409,17,1409,31,0,0,4,1]]},"library_calls":["curl_global_init","curl_share_init","curl_global_cleanup","curl_multi_init","curl_share_cleanup","curl_global_cleanup","curl_multi_setopt","curl_easy_init","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_init","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_slist_append","curl_slist_append","curl_mime_headers","curl_mime_name","curl_mime_data_cb","curl_formadd","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_perform","curl_formget","curl_share_strerror","curl_unescape","curl_free","curl_multi_perform","curl_multi_socket_action","curl_multi_info_read","curl_multi_remove_handle","curl_formfree","curl_slist_free_all","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_multi_init","curl_multi_setopt","curl_easy_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_slist_append","curl_mime_headers","curl_mime_name","curl_mime_data_cb","curl_formadd","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_perform","curl_formget","curl_share_strerror","curl_unescape","curl_free","curl_multi_perform","curl_multi_socket_action","curl_multi_info_read","curl_multi_remove_handle","curl_formfree","curl_slist_free_all","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Callbacks for mime data
static size_t  id_000050_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *f = (FILE*)arg;
    return fread(buffer, size, nitems, f);
}

static int  id_000050_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *f = (FILE*)arg;
    return fseek(f, offset, origin);
}

static void  id_000050_free_callback(void *arg) {
    // File will be closed separately
}

// Callback for formget
static size_t  id_000050_formget_callback(void *arg, const char *buf, size_t len) {
    FILE *f = (FILE*)arg;
    return fwrite(buf, 1, len, f);
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<76) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    if (size < 10) {
        return 0;
    }

    // Initialize curl globally
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Initialize share handle
    CURLSH *share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }

    // Initialize multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // Set multi handle option
    curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, 10L);

    // Initialize easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // Initialize another easy handle for separate operations
    CURL *easy2 = curl_easy_init();
    if (!easy2) {
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // Create input file from fuzzing data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy2);
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy2);
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create mime structure
    curl_mime *mime = curl_mime_init(easy);
    if (!mime) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy2);
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Add mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy2);
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create headers for mime part
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, fuzz_str_1);
    headers = curl_slist_append(headers, fuzz_str_2);

    // Set mime headers (don't take ownership)
    curl_mime_headers(part, headers, 0);

    // Set mime name
    curl_mime_name(part, fuzz_str_3);

    // Set mime data with callback using input file
    // Use half of size for mime data
    curl_mime_data_cb(part, size/2,  id_000050_read_callback,  id_000050_seek_callback,  id_000050_free_callback, in_file);

    // Create form data
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;

    // Use part of the data for form field
    size_t form_data_len = size > 100 ? 100 : size;
    char *form_data = (char*)malloc(form_data_len + 1);
    if (form_data) {
        memcpy(form_data, data, form_data_len);
        form_data[form_data_len] = '\0';

        // Add form field
        curl_formadd(&formpost, &lastptr,
                     CURLFORM_COPYNAME, "fuzzed_field",
                     CURLFORM_COPYCONTENTS, form_data,
                     CURLFORM_CONTENTSLENGTH, form_data_len,
                     CURLFORM_END);

        free(form_data);
    }

    // Set easy handle options
    curl_easy_setopt(easy, CURLOPT_URL, "file:///dev/null");  // Dummy URL
    curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, out_file);
    curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);

    // Set easy2 handle options with form
    if (formpost) {
        curl_easy_setopt(easy2, CURLOPT_URL, "file:///dev/null");
        curl_easy_setopt(easy2, CURLOPT_HTTPPOST, formpost);
        curl_easy_setopt(easy2, CURLOPT_WRITEDATA, out_file);
        curl_easy_setopt(easy2, CURLOPT_VERBOSE, 0L);
    }

    // Add easy handle to multi handle
    CURLMcode multi_add_res = curl_multi_add_handle(multi, easy);
    if (multi_add_res != CURLM_OK) {
        // Cleanup and continue
    }

    // Perform with easy2
    if (formpost) {
        CURLcode perform_res = curl_easy_perform(easy2);
        (void)perform_res; // Result intentionally unused
    }

    // Get form data using formget
    if (formpost) {
        rewind(out_file);
        curl_formget(formpost, out_file,  id_000050_formget_callback);
    }

    // Test share strerror
    const char *share_err = curl_share_strerror(CURLSHE_OK);
    (void)share_err; // Result intentionally unused

    // Test unescape (use a portion of data)
    size_t unescape_len = size > 50 ? 50 : size;
    char *to_unescape = (char*)malloc(unescape_len + 1);
    if (to_unescape) {
        memcpy(to_unescape, data, unescape_len);
        to_unescape[unescape_len] = '\0';
        
        // Create an "escaped" string by doubling %
        std::string escaped;
        for (size_t i = 0; i < unescape_len; i++) {
            if (to_unescape[i] == '%') {
                escaped += "%%";
            } else {
                escaped += to_unescape[i];
            }
        }
        
        char *unescaped = curl_unescape(escaped.c_str(), escaped.length());
        if (unescaped) {
            curl_free(unescaped);
        }
        free(to_unescape);
    }

    // Multi interface operations
    int still_running = 0;
    int numfds = 0;
    
    // Perform initial multi perform
    curl_multi_perform(multi, &still_running);
    
    // Use socket action with dummy socket (simulate no activity)
    curl_multi_socket_action(multi, CURL_SOCKET_TIMEOUT, fuzz_int32_t_4, &still_running);
    
    // Check for messages
    int msgs_in_queue = 0;
    CURLMsg *msg = curl_multi_info_read(multi, &msgs_in_queue);
    (void)msg; // Result intentionally unused
    
    // Remove handle from multi
    if (multi_add_res == CURLM_OK) {
        curl_multi_remove_handle(multi, easy);
    }

    // Cleanup in reverse order
    if (formpost) {
        curl_formfree(formpost);
    }
    
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    curl_easy_cleanup(easy2);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_share_cleanup(share);
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
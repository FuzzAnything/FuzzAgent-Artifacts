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
//<ID> 880
//<Prompt> []
/*<Combination>: [
*/
//<score> 252, nr_unique_branch: 70
//<Quality> {"density":28,"unique_branches":{"multi_done":[[713,3,713,24,0,0,4,0],[721,3,721,10,0,0,4,1]],"Curl_multi_xfer_ulbuf_borrow":[[3921,6,3921,18,0,0,4,1],[3925,6,3925,35,0,0,4,1],[3929,6,3929,38,0,0,4,1],[3934,6,3934,29,0,0,4,1],[3942,6,3942,30,0,0,4,0],[3945,8,3945,32,0,0,4,1]],"Curl_client_read":[[1193,6,1193,29,0,0,4,0],[1195,8,1195,14,0,0,4,1],[1199,6,1199,31,0,0,4,0],[1204,6,1204,51,0,0,4,1],[319,4,319,10,10,0,4,0],[319,14,319,33,10,0,4,1]],"Curl_creader_set_fread":[[1133,6,1133,12,0,0,4,1],[1133,16,1133,18,0,0,4,1],[319,4,319,10,3,0,4,0],[319,14,319,33,3,0,4,1]],"file_upload":[[284,6,284,10,0,0,4,1],[287,6,287,13,0,0,4,1],[291,6,291,29,0,0,4,1],[305,6,305,12,0,0,4,1],[310,6,310,34,0,0,4,0],[315,6,315,33,0,0,4,1],[325,6,325,12,0,0,4,1],[328,9,328,16,0,0,4,0],[328,20,328,24,0,0,4,0],[334,8,334,14,0,0,4,0],[368,6,368,13,0,0,4,1]],"Curl_pgrsSetUploadSize":[[403,6,403,15,0,0,4,0]],"Curl_creader_read":[[521,6,521,13,0,0,4,1]],"cr_in_read":[[652,6,652,18,0,0,4,1],[657,6,657,19,0,0,4,1],[663,6,663,25,0,0,4,0],[667,6,667,18,0,0,4,0],[667,22,667,26,0,0,4,0],[675,3,675,9,0,0,4,0],[676,8,676,29,0,0,4,0],[676,33,676,65,0,0,4,0],[688,3,688,27,0,0,4,1],[697,3,697,27,0,0,4,1],[714,3,714,10,0,0,4,1],[319,4,319,10,29,0,4,0],[319,14,319,33,29,0,4,1]],"do_init_reader_stack":[[1113,6,1113,10,0,0,4,0],[1113,15,1113,29,0,0,4,1],[1115,9,1115,33,0,0,4,1]],"curlx_sotouz_range":[[288,6,288,15,0,0,4,1],[1290,24,1290,33,1,0,4,0],[1290,25,1290,26,1,0,4,0],[1290,37,1290,38,1,0,4,0]],"cl_reset_reader":[[62,9,62,15,0,0,4,0]],"setopt_offt":[[2704,3,2704,31,0,0,4,1],[2713,3,2713,35,0,0,4,1],[2729,3,2729,32,0,0,4,0],[2734,8,2734,17,0,0,4,1],[2738,3,2738,36,0,0,4,1],[2749,3,2749,36,0,0,4,1],[2760,3,2760,33,0,0,4,1],[2768,3,2768,33,0,0,4,1],[2777,3,2777,10,0,0,4,1]],"Curl_vsetopt":[[2887,11,2887,37,0,0,4,1],[2889,11,2889,36,0,0,4,0]],"Curl_rlimit_active":[[190,10,190,32,0,0,4,1],[190,36,190,46,0,0,4,1]],"file_connect":[[241,6,241,25,0,0,4,1]],"Curl_creader_create":[[933,6,933,8,0,0,4,1],[943,14,943,20,0,0,4,1],[944,6,944,12,0,0,4,1]],"file_do":[[407,6,407,24,0,0,4,0]]},"library_calls":["curl_global_init","curl_share_init","curl_global_cleanup","curl_multi_init","curl_share_cleanup","curl_global_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_multi_setopt","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_mime_headers","curl_mime_name","curl_mime_data_cb","curl_easy_setopt","curl_formadd","curl_formget","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_socket_action","curl_multi_info_read","curl_multi_remove_handle","curl_unescape","curl_share_strerror","curl_free","curl_easy_cleanup","curl_mime_free","curl_formfree","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_multi_init","curl_easy_init","curl_multi_setopt","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_mime_headers","curl_mime_name","curl_mime_data_cb","curl_easy_setopt","curl_formadd","curl_formget","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_socket_action","curl_multi_info_read","curl_multi_remove_handle","curl_unescape","curl_share_strerror","curl_free","curl_easy_cleanup","curl_mime_free","curl_formfree","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":1}
/**/


// Callback functions for curl_mime_data_cb
static size_t  id_000048_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *file = (FILE *)arg;
    return fread(buffer, size, nitems, file);
}

static int  id_000048_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *file = (FILE *)arg;
    return fseek(file, offset, origin);
}

static void  id_000048_free_callback(void *arg) {
    // File will be closed separately
}

// Callback for curl_formget
static size_t  id_000048_formget_callback(void *arg, const char *buf, size_t len) {
    FILE *file = (FILE *)arg;
    return fwrite(buf, 1, len, file);
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<56) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    if (size == 0) {
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
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    
    // Create form output file
    FILE *form_file = fopen("form_output", "wb");
    
    // Reuse the input data for multiple purposes
    const char *input_str = (const char *)data;
    size_t str_len = size > 1024 ? 1024 : size; // Limit string length
    
    // Create and setup easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        if (out_file) assert_file_closed(&out_file);;
        if (form_file) assert_file_closed(&form_file);;
        assert_file_closed(&in_file);;
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&form_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set share option on multi handle (example option)
    curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, 10L);
    
    // Create a mime structure
    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        // Add a mime part
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Create headers list for mime part
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, fuzz_str_1);
            
            // Set mime headers
            curl_mime_headers(part, headers, 1); // take_ownership = 1
            
            // Set mime name
            curl_mime_name(part, fuzz_str_2);
            
            // Set mime data callback that reads from input file
            rewind(in_file);
            curl_mime_data_cb(part, size,  id_000048_read_callback,  id_000048_seek_callback,  id_000048_free_callback, in_file);
        }
        
        // Set mime data to easy handle
        curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    }
    
    // Create and setup form data
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Add form field using the input data
    curl_formadd(&formpost, &lastptr,
                CURLFORM_COPYNAME, "data",
                CURLFORM_COPYCONTENTS, input_str,
                CURLFORM_CONTENTSLENGTH, (long)str_len,
                CURLFORM_END);
    
    // Get form data using callback
    if (formpost && form_file) {
        curl_formget(formpost, form_file,  id_000048_formget_callback);
    }
    
    // Set various easy handle options
    curl_easy_setopt(easy, CURLOPT_URL, "file:///dev/null"); // Safe local URL
    curl_easy_setopt(easy, CURLOPT_SHARE, share);
    curl_easy_setopt(easy, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy, CURLOPT_INFILESIZE_LARGE, (curl_off_t)size);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    // Try to perform the transfer
    int still_running = 1;
    int numfds = 0;
    
    // Use multi perform to start the transfer
    curl_multi_perform(multi, &still_running);
    
    // Try socket action with invalid socket (just to call the API)
    int dummy_running = 0;
    curl_multi_socket_action(multi, CURL_SOCKET_BAD, fuzz_int32_t_3, &dummy_running);
    
    // Check for messages
    int msgs_in_queue = 0;
    CURLMsg *msg = curl_multi_info_read(multi, &msgs_in_queue);
    
    // Remove handle from multi
    curl_multi_remove_handle(multi, easy);
    
    // Test unescape with input data (ensure null termination)
    char *unescaped = NULL;
    if (str_len > 0) {
        // Create a null-terminated copy for unescape
        char *temp_str = (char *)malloc(str_len + 1);
        if (temp_str) {
            memcpy(temp_str, input_str, str_len);
            temp_str[str_len] = '\0';
            unescaped = curl_unescape(temp_str, (int)str_len);
            free(temp_str);
        }
    }
    
    // Test share error (get error string for a known error code)
    const char *share_err = curl_share_strerror(CURLSHE_OK);
    
    // Cleanup
    if (unescaped) {
        curl_free(unescaped);
    }
    
    curl_easy_cleanup(easy);
    
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (formpost) {
        curl_formfree(formpost);
    }
    
    curl_multi_cleanup(multi);
    curl_share_cleanup(share);
    curl_global_cleanup();
    
    // Close files
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    if (form_file) assert_file_closed(&form_file);;
    
    assert_file_closed(&form_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
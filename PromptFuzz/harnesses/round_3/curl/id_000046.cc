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
//<ID> 817
//<Prompt> []
/*<Combination>: [
*/
//<score> 80, nr_unique_branch: 20
//<Quality> {"density":48,"unique_branches":{"Curl_getformdata":[[806,17,806,46,0,0,4,0],[808,35,808,53,0,0,4,0],[829,21,829,39,0,0,4,0],[830,12,830,22,0,0,4,1],[830,26,831,61,0,0,4,0]],"FormAddCheck":[[235,9,235,22,0,0,4,1],[245,9,245,40,0,0,4,0],[247,39,247,70,0,0,4,0]],"curl_formfree":[[680,8,681,71,0,0,4,1]],"FormAdd":[[469,5,469,28,0,0,4,0],[471,10,471,22,0,0,4,1],[475,12,475,18,0,0,4,0],[485,5,485,31,0,0,4,0],[486,10,486,28,0,0,4,1],[554,5,554,25,0,0,4,0],[556,10,556,46,0,0,4,1],[558,15,558,79,0,0,4,1],[311,26,311,31,22,0,4,1],[313,26,313,31,23,0,4,1],[311,26,311,31,30,0,4,1]]},"library_calls":["curl_global_init","curl_share_init","curl_global_cleanup","curl_multi_init","curl_share_cleanup","curl_global_cleanup","curl_multi_setopt","curl_easy_init","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_data_cb","curl_mime_name","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_formadd","curl_formadd","curl_formget","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_action","curl_multi_socket_action","curl_multi_perform","curl_multi_info_read","curl_easy_perform","curl_easy_duphandle","curl_ws_send","curl_easy_cleanup","curl_slist_free_all","curl_formfree","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_multi_init","curl_multi_setopt","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_name","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_formadd","curl_formadd","curl_formget","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_action","curl_multi_socket_action","curl_multi_perform","curl_multi_info_read","curl_easy_perform","curl_easy_duphandle","curl_ws_send","curl_easy_cleanup","curl_slist_free_all","curl_formfree","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":2}
/**/


extern "C" {
}

// Callback functions for curl_mime_data_cb
static size_t  id_000046_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *in_file = (FILE *)arg;
    return fread(buffer, size, nitems, in_file);
}

static int  id_000046_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *in_file = (FILE *)arg;
    return fseek(in_file, offset, origin);
}

static void  id_000046_free_callback(void *arg) {
    // Nothing to free since we use fmemopen which will be closed separately
}

// Callback for curl_formget
static size_t  id_000046_formget_callback(void *arg, const char *data, size_t size) {
    return size; // Just return size to indicate success
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<86) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Initialize curl globally
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	
    
    // Create a share handle
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
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 10L);
    
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
    
    // Add mime part
    curl_mimepart *mimepart = curl_mime_addpart(mime);
    if (!mimepart) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Create FILE* for input data
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
    
    // Set mime data using callback
    curl_mime_data_cb(mimepart, size,  id_000046_read_callback,  id_000046_seek_callback,  id_000046_free_callback, in_file);
    
    // Set mime name
    curl_mime_name(mimepart, fuzz_str_1);
    
    // Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy_handle, CURLOPT_SHARE, share);
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    
    // Create form data
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Add form field
    curl_formadd(&formpost, &lastptr,
                CURLFORM_COPYNAME, "field1",
                CURLFORM_COPYCONTENTS, "value1",
                CURLFORM_END);
    
    // Add form field with file data
    curl_formadd(&formpost, &lastptr,
                CURLFORM_COPYNAME, "file",
                CURLFORM_BUFFER, "fuzzfile",
                CURLFORM_BUFFERPTR, data,
                CURLFORM_BUFFERLENGTH, size,
                CURLFORM_END);
    
    // Get form data
    char *form_buffer = (char *)malloc(size * 2);
    if (form_buffer) {
        int form_len = curl_formget(formpost, form_buffer,  id_000046_formget_callback);
        free(form_buffer);
    }
    
    // Create and append to slist
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, fuzz_str_2);
    headers = curl_slist_append(headers, fuzz_str_3);
    
    // Set headers
    curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Socket operations (simulated since we don't have real sockets)
    int running_handles = 0;
    curl_multi_socket(multi_handle, CURL_SOCKET_BAD, &running_handles);
    curl_multi_socket_action(multi_handle, CURL_SOCKET_BAD, fuzz_int32_t_4, &running_handles);
    
    // Perform multi operations
    curl_multi_perform(multi_handle, &running_handles);
    
    // Read multi info
    int msgs_in_queue = 0;
    CURLMsg *msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    
    // Perform easy handle operation (will fail since no real connection)
    curl_easy_perform(easy_handle);
    
    // Duplicate easy handle
    CURL *duplicated_handle = curl_easy_duphandle(easy_handle);
    if (duplicated_handle) {
        // Try WebSocket send with duplicated handle (will fail but exercise the code)
        size_t sent = 0;
        unsigned int ws_flags = CURLWS_TEXT;
        curl_ws_send(duplicated_handle, data, size > 100 ? 100 : size, &sent, fuzz_int64_t_5, ws_flags);
        curl_easy_cleanup(duplicated_handle);
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    curl_slist_free_all(headers);
    curl_formfree(formpost);
    curl_mime_free(mime);
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_share_cleanup(share);
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
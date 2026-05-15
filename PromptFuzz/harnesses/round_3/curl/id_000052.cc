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
//<ID> 890
//<Prompt> []
/*<Combination>: [
*/
//<score> 361, nr_unique_branch: 51
//<Quality> {"density":38,"unique_branches":{"Curl_cpool_get_conn":[[784,6,784,12,0,0,4,1],[43,8,43,9,1,0,4,0],[54,8,54,9,2,0,4,0],[41,4,41,7,3,0,4,1],[41,4,41,7,8,0,4,1]],"sigpipe_restore":[[75,6,75,20,0,0,4,0]],"cpool_get_first":[[137,43,137,45,0,0,4,0],[141,8,141,17,0,0,4,0]],"cpool_find_conn":[[771,6,771,37,0,0,4,0]],"Curl_cpool_destroy":[[243,8,243,12,0,0,4,0],[245,11,245,15,0,0,4,0]],"close_connect_only":[[775,6,775,29,0,0,4,0]],"Curl_cpool_conn_now_idle":[[561,6,561,11,0,0,4,1],[561,15,561,27,0,0,4,1],[564,6,564,31,0,0,4,0],[564,6,564,31,0,0,4,1],[566,19,566,44,0,0,4,0],[573,6,573,11,0,0,4,0],[573,15,573,26,0,0,4,0],[576,8,576,15,0,0,4,1],[578,8,578,37,0,0,4,1],[588,8,588,15,0,0,4,1],[39,32,39,35,2,0,4,0],[39,39,39,50,2,0,4,0]],"sigpipe_ignore":[[53,6,53,26,0,0,4,0]],"Curl_cpool_do_by_id":[[818,6,818,12,0,0,4,1],[43,8,43,9,1,0,4,0],[54,8,54,9,2,0,4,0],[41,4,41,7,3,0,4,1],[41,4,41,7,8,0,4,1]],"multi_conn_should_close":[[631,6,631,15,0,0,4,1]],"Curl_conn_get_max_concurrent":[[1065,12,1065,14,0,0,4,1],[1070,11,1070,17,0,0,4,0],[406,36,406,46,1,0,4,0],[406,50,406,59,1,0,4,0]],"Curl_getconnectinfo":[[279,6,279,38,0,0,4,0],[283,8,283,13,0,0,4,1],[288,8,288,13,0,0,4,0]],"sigpipe_apply":[[83,6,83,10,0,0,4,0],[83,14,83,52,0,0,4,0]],"curl_multi_remove_handle":[[856,32,856,49,0,0,4,1],[873,6,873,38,0,0,4,0]],"cpool_do_conn":[[805,6,805,37,0,0,4,0]],"multi_done_locked":[[658,6,658,65,0,0,4,1],[667,11,667,65,0,0,4,1],[676,8,676,44,0,0,4,0],[319,4,319,10,17,0,4,0],[319,14,319,33,17,0,4,1]],"state_do":[[2173,6,2173,28,0,0,4,0],[2173,32,2173,58,0,0,4,0]],"cpool_foreach":[[539,10,539,38,0,0,4,0]]},"library_calls":["curl_global_init","curl_share_init","curl_multi_init","curl_multi_setopt","curl_easy_init","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_mime_headers","curl_mime_name","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_data_cb","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_formadd","curl_multi_add_handle","curl_multi_perform","curl_multi_socket_action","curl_multi_info_read","curl_multi_remove_handle","curl_easy_perform","curl_formget","curl_easy_escape","curl_unescape","curl_free","curl_free","curl_share_strerror","curl_formfree","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_free","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_multi_init","curl_multi_setopt","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_mime_headers","curl_mime_name","curl_mime_data_cb","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_formadd","curl_multi_add_handle","curl_multi_perform","curl_multi_socket_action","curl_multi_info_read","curl_multi_remove_handle","curl_easy_perform","curl_formget","curl_easy_escape","curl_unescape","curl_free","curl_free","curl_share_strerror","curl_formfree","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_free","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Callback functions for mime data
static size_t  id_000052_read_callback(char* buffer, size_t size, size_t nitems, void* arg) {
    FILE* f = (FILE*)arg;
    return fread(buffer, size, nitems, f);
}

static int  id_000052_seek_callback(void* arg, curl_off_t offset, int origin) {
    FILE* f = (FILE*)arg;
    return fseek(f, (long)offset, origin);
}

static void  id_000052_free_callback(void* arg) {
    // FILE will be closed by fmemopen automatically
}

// Formget callback
static size_t  id_000052_formget_callback(void* arg, const char* buf, size_t len) {
    return len; // Just consume the data
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<56) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize global state
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	
    
    // Create share object
    CURLSH* share = curl_share_init();
    
    // Create multi handle
    CURLM* multi = curl_multi_init();
    
    // Set multi options
    curl_multi_setopt(multi, CURLMOPT_MAX_TOTAL_CONNECTIONS, 5L);
    
    // Create easy handle
    CURL* easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Create mime structure
    curl_mime* mime = curl_mime_init(easy);
    
    // Add mime part
    curl_mimepart* part = curl_mime_addpart(mime);
    
    // Create header list
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, fuzz_str_1);
    
    // Set mime headers (take ownership)
    curl_mime_headers(part, headers, 1);
    
    // Set mime name
    curl_mime_name(part, fuzz_str_2);
    
    // Create FILE from input data for mime callback
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set mime data callback
    curl_mime_data_cb(part, (curl_off_t)size, 
                       id_000052_read_callback,  id_000052_seek_callback,  id_000052_free_callback, in_file);
    
    // Set easy options
    curl_easy_setopt(easy, CURLOPT_URL, "file:///dev/null"); // Dummy URL
    curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy, CURLOPT_SHARE, share);
    curl_easy_setopt(easy, CURLOPT_CONNECT_ONLY, 1L); // Connect only, no transfer
    
    // Create form
    struct curl_httppost* formpost = NULL;
    struct curl_httppost* lastptr = NULL;
    
    // Extract a string for form data (first 100 bytes or less)
    size_t form_data_len = size < 100 ? size : 100;
    char* form_data = (char*)malloc(form_data_len + 1);
    if (form_data) {
        memcpy(form_data, data, form_data_len);
        form_data[form_data_len] = '\0';
        
        // Add form field
        curl_formadd(&formpost, &lastptr,
                     CURLFORM_COPYNAME, "field1",
                     CURLFORM_COPYCONTENTS, form_data,
                     CURLFORM_END);
        
        free(form_data);
    }
    
    // Add easy handle to multi
    curl_multi_add_handle(multi, easy);
    
    // Perform multi operations
    int running_handles = 0;
    curl_multi_perform(multi, &running_handles);
    
    // Try socket action (won't have real socket, but API will handle it)
    curl_multi_socket_action(multi, CURL_SOCKET_TIMEOUT, fuzz_int32_t_3, &running_handles);
    
    // Check for messages
    int msgs_in_queue = 0;
    CURLMsg* msg = curl_multi_info_read(multi, &msgs_in_queue);
    
    // Remove handle from multi
    curl_multi_remove_handle(multi, easy);
    
    // Try easy perform (will fail due to CONNECT_ONLY, but exercise API)
    curl_easy_perform(easy);
    
    // Use formget
    if (formpost) {
        curl_formget(formpost, NULL,  id_000052_formget_callback);
    }
    
    // Test unescape with part of input data
    if (size > 10) {
        char* escaped = curl_easy_escape(easy, (const char*)data, 10);
        if (escaped) {
            char* unescaped = curl_unescape(escaped, -1);
            if (unescaped) {
                curl_free(unescaped);
            }
            curl_free(escaped);
        }
    }
    
    // Test share error string
    const char* share_err = curl_share_strerror(CURLSHE_OK);
    (void)share_err; // Use variable to avoid warning
    
    // Clean up
    if (formpost) {
        curl_formfree(formpost);
    }
    
    // Create another mime structure to use the API again
    curl_mime* mime2 = curl_mime_init(easy);
    curl_mimepart* part2 = curl_mime_addpart(mime2);
    
    // Use data callback again
    rewind(in_file);
    curl_mime_data_cb(part2, (curl_off_t)size,
                       id_000052_read_callback,  id_000052_seek_callback,  id_000052_free_callback, in_file);
    
    // Close file
    assert_file_closed(&in_file);;
    
    // Clean up mime
    curl_mime_free(mime2);
    curl_mime_free(mime);
    
    // Final cleanup
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_share_cleanup(share);
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
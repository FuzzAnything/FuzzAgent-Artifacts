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
//<ID> 1003
//<Prompt> []
/*<Combination>: [
*/
//<score> 56, nr_unique_branch: 3
//<Quality> {"density":28,"unique_branches":{"multi_runsingle":[[2499,8,2499,36,0,0,4,0],[319,4,319,10,57,0,4,0],[319,14,319,33,57,0,4,1]]},"library_calls":["curl_global_init","curl_share_init","curl_share_setopt","curl_multi_init","curl_multi_setopt","curl_easy_init","curl_easy_init","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_name","curl_easy_setopt","curl_formadd","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_easy_pause","curl_easy_pause","curl_easy_perform","curl_easy_duphandle","curl_ws_send","curl_formget","curl_easy_cleanup","curl_multi_remove_handle","curl_multi_remove_handle","curl_slist_free_all","curl_formfree","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_share_setopt","curl_multi_init","curl_multi_setopt","curl_easy_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_name","curl_easy_setopt","curl_formadd","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_easy_pause","curl_easy_pause","curl_easy_perform","curl_easy_duphandle","curl_ws_send","curl_formget","curl_easy_cleanup","curl_multi_remove_handle","curl_multi_remove_handle","curl_slist_free_all","curl_formfree","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Callbacks for mime data
static size_t  id_000058_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *in_file = (FILE *)arg;
    return fread(buffer, size, nitems, in_file);
}

static int  id_000058_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *in_file = (FILE *)arg;
    return fseek(in_file, (long)offset, origin);
}

static void  id_000058_free_callback(void *arg) {
    // Nothing to free since we use fmemopen
}

// Callback for formget
static size_t  id_000058_formget_callback(void *arg, const char *buf, size_t len) {
    FILE *out_file = (FILE *)arg;
    return fwrite(buf, 1, len, out_file);
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<39) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create share handle
    CURLSH *share = curl_share_init();
    if (share) {
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    }
    
    // Create multi handle
    CURLM *multi = curl_multi_init();
    if (multi) {
        curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, 1L);
    }
    
    // Create easy handles
    CURL *easy1 = curl_easy_init();
    CURL *easy2 = curl_easy_init();
    
    if (!easy1 || !easy2) {
        if (easy1) curl_easy_cleanup(easy1);
        if (easy2) curl_easy_cleanup(easy2);
        if (multi) curl_multi_cleanup(multi);
        if (share) curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Create mime structure
    curl_mime *mime = curl_mime_init(easy1);
    if (mime) {
        // Add mime part
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Create FILE* from input data
            FILE *in_file = fmemopen((void *)data, size, "rb");
            if (in_file) {
                // Set mime data callback
                curl_mime_data_cb(part, (curl_off_t)size,  id_000058_read_callback, 
                                   id_000058_seek_callback,  id_000058_free_callback, in_file);
                curl_mime_name(part, fuzz_str_1);
                
                // Set mime as POST fields
                curl_easy_setopt(easy1, CURLOPT_MIMEPOST, mime);
                
                assert_file_closed(&in_file);;
            }
        }
    }
    
    // Create form data
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Use first byte as form field name length
    size_t name_len = data[0] % 32 + 1;
    if (name_len > size - 1) name_len = size - 1;
    
    char *field_name = (char *)malloc(name_len + 1);
    if (field_name) {
        memcpy(field_name, data + 1, name_len);
        field_name[name_len] = '\0';
        
        // Add form field
        curl_formadd(&formpost, &lastptr,
                    CURLFORM_COPYNAME, field_name,
                    CURLFORM_COPYCONTENTS, "fuzz_content",
                    CURLFORM_END);
        
        free(field_name);
    }
    
    // Create header list
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, fuzz_str_2);
    
    // Set easy handle options
    curl_easy_setopt(easy1, CURLOPT_URL, "file:///dev/null");
    curl_easy_setopt(easy1, CURLOPT_SHARE, share);
    curl_easy_setopt(easy1, CURLOPT_HTTPHEADER, headers);
    if (formpost) {
        curl_easy_setopt(easy1, CURLOPT_HTTPPOST, formpost);
    }
    
    // Add easy handles to multi handle
    if (multi) {
        curl_multi_add_handle(multi, easy1);
        curl_multi_add_handle(multi, easy2);
    }
    
    // Perform operations
    int still_running = 0;
    if (multi) {
        curl_multi_perform(multi, &still_running);
    }
    
    // Pause and resume
    curl_easy_pause(easy1, CURLPAUSE_ALL);
    curl_easy_pause(easy1, CURLPAUSE_CONT);
    
    // Try to perform with easy handle
    curl_easy_perform(easy1);
    
    // Duplicate handle
    CURL *easy_dup = curl_easy_duphandle(easy1);
    
    // Try WebSocket send (will likely fail but exercise the API)
    size_t sent = 0;
    if (size > 0) {
        curl_ws_send(easy1, data, size > 100 ? 100 : size, &sent, fuzz_int64_t_3, CURLWS_BINARY);
    }
    
    // Write form data to output
    FILE *out_file = fopen("output_file", "wb");
    if (out_file && formpost) {
        curl_formget(formpost, out_file,  id_000058_formget_callback);
        assert_file_closed(&out_file);;
    }
    
    // Cleanup duplicated handle
    if (easy_dup) {
        curl_easy_cleanup(easy_dup);
    }
    
    // Remove handles from multi
    if (multi) {
        curl_multi_remove_handle(multi, easy1);
        curl_multi_remove_handle(multi, easy2);
    }
    
    // Cleanup
    if (headers) curl_slist_free_all(headers);
    if (formpost) curl_formfree(formpost);
    if (mime) curl_mime_free(mime);
    curl_easy_cleanup(easy1);
    curl_easy_cleanup(easy2);
    if (multi) curl_multi_cleanup(multi);
    if (share) curl_share_cleanup(share);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	return 0;
}
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
//<ID> 1001
//<Prompt> []
/*<Combination>: [
*/
//<score> 220, nr_unique_branch: 36
//<Quality> {"density":20,"unique_branches":{"Curl_xfer_pause_send":[[894,6,894,13,0,0,4,0],[894,17,894,45,0,0,4,1]],"Curl_splayremove":[[266,8,266,19,0,0,4,1]],"url_match_destination":[[1027,6,1027,46,0,0,4,1],[1029,9,1029,35,0,0,4,0],[1032,8,1032,67,0,0,4,1],[1049,9,1049,37,0,0,4,1],[1051,9,1051,37,0,0,4,1],[1056,8,1056,61,0,0,4,1],[1057,8,1057,51,0,0,4,1]],"Curl_cpool_find":[[614,6,614,12,0,0,4,0],[616,11,616,15,0,0,4,0],[616,11,616,15,0,0,4,1],[621,10,621,33,0,0,4,1]],"Curl_splay":[[69,10,69,20,0,0,4,1],[71,10,71,50,0,0,4,1]],"url_match_connect_config":[[733,6,733,29,0,0,4,1],[733,33,733,49,0,0,4,1],[733,53,733,72,0,0,4,1],[737,6,737,51,0,0,4,1],[741,6,741,25,0,0,4,1],[741,29,741,49,0,0,4,1],[760,6,760,61,0,0,4,1],[765,6,765,61,0,0,4,1],[772,6,772,35,0,0,4,1],[780,11,780,35,0,0,4,1]],"Curl_pgrsSendPause":[[237,6,237,13,0,0,4,0]],"url_match_result":[[1286,11,1286,35,0,0,4,0],[1286,39,1286,64,0,0,4,1]],"url_match_fully_connected":[[790,6,790,48,0,0,4,0],[794,8,794,24,0,0,4,0],[319,4,319,10,6,0,4,0],[319,14,319,33,6,0,4,1]],"url_match_conn":[[1214,6,1214,40,0,0,4,1],[1218,6,1218,37,0,0,4,1],[1221,6,1221,41,0,0,4,0]]},"library_calls":["curl_global_init","curl_share_init","curl_share_setopt","curl_multi_init","curl_multi_setopt","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_data_cb","curl_easy_setopt","curl_formadd","curl_formget","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_duphandle","curl_multi_add_handle","curl_multi_perform","curl_easy_pause","curl_easy_pause","curl_multi_perform","curl_easy_perform","curl_ws_send","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_remove_handle","curl_easy_cleanup","curl_slist_free_all","curl_formfree","curl_mime_free","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_share_setopt","curl_multi_init","curl_multi_setopt","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_data_cb","curl_easy_setopt","curl_formadd","curl_formget","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_duphandle","curl_multi_add_handle","curl_multi_perform","curl_easy_pause","curl_easy_pause","curl_multi_perform","curl_easy_perform","curl_ws_send","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_remove_handle","curl_easy_cleanup","curl_slist_free_all","curl_formfree","curl_mime_free","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a comprehensive C++ fuzz driver that uses the specified curl APIs to simulate a multipart form upload scenario:

*/


// Include necessary curl headers

// Callback functions for mime data
static size_t  id_000057_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *f = (FILE *)arg;
    return fread(buffer, size, nitems, f);
}

static int  id_000057_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *f = (FILE *)arg;
    return fseek(f, (long)offset, origin);
}

static void  id_000057_free_callback(void *arg) {
    // Nothing to free since we use fmemopen
}

// Callback for formget
static size_t  id_000057_formget_callback(void *arg, const char *buf, size_t len) {
    FILE *f = (FILE *)arg;
    return fwrite(buf, 1, len, f);
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<81) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimal data

    // Open input file from fuzzing data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Open output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Global initialization
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 2. Initialize share handle
    CURLSH *share = curl_share_init();
    if (share) {
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    }

    // 3. Initialize multi handle
    CURLM *multi = curl_multi_init();
    if (multi) {
        // Set multi handle option
        curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, (long)10);
    }

    // 4. Create first easy handle
    CURL *easy1 = curl_easy_init();
    CURL *easy2 = nullptr;
    
    if (easy1) {
        // 5. Create mime structure
        curl_mime *mime = curl_mime_init(easy1);
        if (mime) {
            // 6. Add mime part
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // 7. Set mime name
                curl_mime_name(part, fuzz_str_1);
                
                // 8. Set mime data with callback
                rewind(in_file);
                curl_mime_data_cb(part, (curl_off_t)size, 
                                  id_000057_read_callback,  id_000057_seek_callback, 
                                  id_000057_free_callback, in_file);
            }
            
            // 9. Set mime to easy handle
            curl_easy_setopt(easy1, CURLOPT_MIMEPOST, mime);
        }

        // 10. Create old-style form data
        struct curl_httppost *formpost = nullptr;
        struct curl_httppost *lastptr = nullptr;
        
        // Use part of fuzzing data for form
        size_t form_data_len = size > 100 ? 100 : size;
        char *form_data = (char *)malloc(form_data_len + 1);
        if (form_data) {
            memcpy(form_data, data, form_data_len);
            form_data[form_data_len] = '\0';
            
            curl_formadd(&formpost, &lastptr,
                        CURLFORM_COPYNAME, "fuzz_field",
                        CURLFORM_COPYCONTENTS, form_data,
                        CURLFORM_END);
            
            free(form_data);
        }

        // 11. Write form data to output file
        if (formpost) {
            rewind(out_file);
            curl_formget(formpost, out_file,  id_000057_formget_callback);
            fflush(out_file);
        }

        // 12. Create header list
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, fuzz_str_2);
        headers = curl_slist_append(headers, fuzz_str_3);
        
        // 13. Set various options
        curl_easy_setopt(easy1, CURLOPT_URL, "http://localhost/test");
        curl_easy_setopt(easy1, CURLOPT_HTTPHEADER, headers);
        if (share) {
            curl_easy_setopt(easy1, CURLOPT_SHARE, share);
        }
        curl_easy_setopt(easy1, CURLOPT_VERBOSE, 0L);

        // 14. Add to multi handle
        if (multi) {
            curl_multi_add_handle(multi, easy1);
        }

        // 15. Create second easy handle (duplicate)
        easy2 = curl_easy_duphandle(easy1);
        
        // 16. Add second handle to multi
        if (easy2 && multi) {
            curl_multi_add_handle(multi, easy2);
        }

        // 17. Perform multi operations
        int still_running = 0;
        if (multi) {
            curl_multi_perform(multi, &still_running);
            
            // 18. Pause and resume first handle
            curl_easy_pause(easy1, CURLPAUSE_ALL);
            curl_easy_pause(easy1, CURLPAUSE_CONT);
            
            // Perform again
            curl_multi_perform(multi, &still_running);
        }

        // 19. Try to perform single easy operation (will likely fail but exercise code)
        curl_easy_perform(easy1);

        // 20. Try WebSocket send if we have enough data
        if (size >= 20) {
            size_t sent = 0;
            curl_ws_send(easy1, data, size > 1000 ? 1000 : size, 
                        &sent, fuzz_int64_t_4, CURLWS_BINARY);
        }

        // 21. Cleanup in reverse order
        if (easy2) {
            if (multi) {
                curl_multi_remove_handle(multi, easy2);
            }
            curl_easy_cleanup(easy2);
        }
        
        if (easy1) {
            if (multi) {
                curl_multi_remove_handle(multi, easy1);
            }
            curl_easy_cleanup(easy1);
        }
        
        if (headers) {
            curl_slist_free_all(headers);
        }
        
        if (formpost) {
            curl_formfree(formpost);
        }
        
        if (mime) {
            curl_mime_free(mime);
        }
    }

    // 22. Cleanup multi handle
    if (multi) {
        curl_multi_cleanup(multi);
    }

    // 23. Cleanup share handle
    if (share) {
        curl_share_cleanup(share);
    }

    // 24. Global cleanup
    curl_global_cleanup();

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
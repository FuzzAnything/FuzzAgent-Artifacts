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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 48.333332, nr_unique_branch: 8
//<Quality> {"density":29,"unique_branches":{"setopt_long_net":[[885,3,885,30,0,0,4,0]],"multi_runsingle":[[2538,10,2538,34,0,0,4,0]],"Curl_timeleft_now_ms":[[115,30,115,60,0,0,4,0],[122,11,122,29,0,0,4,1],[122,33,122,55,0,0,4,1],[133,6,133,19,0,0,4,0]],"multi_addtimeout":[[3586,43,3586,44,0,0,4,1],[3589,10,3589,18,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_share_init","curl_share_cleanup","curl_multi_init","curl_global_cleanup","curl_multi_setopt","curl_multi_setopt","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_data_cb","curl_mime_name","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_formadd","curl_formget","curl_mfprintf","curl_mfprintf","curl_multi_perform","curl_multi_fdset","curl_multi_socket_action","curl_multi_info_read","curl_easy_perform","curl_formfree","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_share_cleanup","curl_multi_init","curl_multi_setopt","curl_multi_setopt","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_name","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_formadd","curl_formget","curl_mfprintf","curl_mfprintf","curl_multi_perform","curl_multi_fdset","curl_multi_socket_action","curl_multi_info_read","curl_easy_perform","curl_formfree","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":2}
/**/


extern "C" {
}

// Callback functions for curl_mime_data_cb
static size_t  id_000004_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *f = (FILE *)arg;
    return fread(buffer, size, nitems, f);
}

static int  id_000004_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *f = (FILE *)arg;
    return fseek(f, (long)offset, origin);
}

static void  id_000004_free_callback(void *arg) {
    // Nothing to free in this case
    (void)arg;
}

// Callback for curl_formget
static size_t  id_000004_formget_callback(void *arg, const char *buf, size_t len) {
    FILE *f = (FILE *)arg;
    return fwrite(buf, 1, len, f);
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Initialize curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	
    
    // Create input and output files
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create share handle and clean it up (to satisfy API requirement)
    CURLSH *share = curl_share_init();
    if (share) {
        curl_share_cleanup(share);
    }
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set multi handle options
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, 1L);
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 5L);
    
    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create mime part with data callback
    curl_mimepart *part = curl_mime_addpart(mime);
    if (part) {
        rewind(in_file);
        curl_mime_data_cb(part, (curl_off_t)size, 
                          id_000004_read_callback,  id_000004_seek_callback, 
                          id_000004_free_callback, in_file);
        curl_mime_name(part, fuzz_str_1);
    }
    
    // Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file:///dev/null");
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Create a simple form to use with curl_formget
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Use first 100 bytes or less for form data
    size_t form_data_size = size > 100 ? 100 : size;
    char *form_data = (char *)malloc(form_data_size + 1);
    if (form_data) {
        memcpy(form_data, data, form_data_size);
        form_data[form_data_size] = '\0';
        curl_formadd(&formpost, &lastptr,
                    CURLFORM_COPYNAME, "fuzzed_field",
                    CURLFORM_COPYCONTENTS, form_data,
                    CURLFORM_END);
    }
    
    // Use curl_formget to write form data to output file
    if (formpost) {
        rewind(out_file);
        curl_formget(formpost, out_file,  id_000004_formget_callback);
    }
    
    // Use curl_mfprintf to write to output file
    if (out_file) {
        curl_mfprintf(out_file, "Fuzzing with libcurl\n");
        curl_mfprintf(out_file, "Input size: %zu\n", size);
    }
    
    int still_running = 0;
    int max_fd = 0;
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    
    // Perform the transfer using multi interface
    curl_multi_perform(multi_handle, &still_running);
    
    // Use fdset and socket action for event handling
    while (still_running) {
        FD_ZERO(&read_fd_set);
        FD_ZERO(&write_fd_set);
        FD_ZERO(&exc_fd_set);
        max_fd = 0;
        
        curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, 
                        &exc_fd_set, &max_fd);
        
        // Get file descriptor from input file
        int in_fd = fuzz_fileno(in_file);
        if (in_fd >= 0 && in_fd < FD_SETSIZE) {
            FD_SET(in_fd, &read_fd_set);
            if (in_fd > max_fd) max_fd = in_fd;
        }
        
        // Use select with timeout
        struct timeval timeout = {0, 10000}; // 10ms timeout
        if (max_fd >= 0) {
            select(max_fd + 1, &read_fd_set, &write_fd_set, 
                  &exc_fd_set, &timeout);
        }
        
        // Use socket action with timeout socket
        curl_multi_socket_action(multi_handle, CURL_SOCKET_TIMEOUT, 
                                fuzz_int32_t_2, &still_running);
        
        // Check for messages
        int msgs_in_queue = 0;
        CURLMsg *msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
        if (msg && msg->msg == CURLMSG_DONE) {
            // Transfer completed
            break;
        }
    }
    
    // Also perform a simple easy perform (though it won't actually transfer)
    curl_easy_perform(easy_handle);
    
    // Cleanup
    if (formpost) {
        curl_formfree(formpost);
    }
    if (form_data) {
        free(form_data);
    }
    
    curl_mime_free(mime);
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
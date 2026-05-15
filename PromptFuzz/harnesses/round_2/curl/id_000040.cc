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
//<ID> 572
//<Prompt> []
/*<Combination>: [
*/
//<score> 78, nr_unique_branch: 7
//<Quality> {"density":26,"unique_branches":{"out_number":[[795,16,795,40,0,0,4,0]],"lookup":[[35,6,35,10,0,0,4,1],[35,14,35,16,0,0,4,0],[38,10,38,14,0,0,4,1],[43,12,43,25,0,0,4,0],[43,12,43,25,0,0,4,1],[43,29,43,60,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_url","curl_global_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_set","curl_url_set","curl_url_set","curl_url_get","curl_free","curl_easy_init","curl_url_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_data_cb","curl_mime_name","curl_mime_filename","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_option_by_id","curl_multi_init","curl_mime_free","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup","curl_share_init","curl_share_setopt","curl_share_strerror","curl_share_cleanup","curl_multi_waitfds","curl_multi_fdset","curl_multi_notify_disable","curl_multi_perform","curl_mfprintf","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_set","curl_url_set","curl_url_get","curl_free","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_name","curl_mime_filename","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_option_by_id","curl_multi_init","curl_multi_add_handle","curl_share_init","curl_share_setopt","curl_share_strerror","curl_share_cleanup","curl_multi_waitfds","curl_multi_fdset","curl_multi_notify_disable","curl_multi_perform","curl_mfprintf","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include curl headers

// Custom callback functions for mime data
static size_t  id_000040_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *f = (FILE*)arg;
    if (feof(f)) return 0;
    return fread(buffer, size, nitems, f);
}

static int  id_000040_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *f = (FILE*)arg;
    return fseek(f, (long)offset, origin) ? CURL_SEEKFUNC_CANTSEEK : CURL_SEEKFUNC_OK;
}

static void  id_000040_free_callback(void *arg) {
    // FILE will be closed elsewhere
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // 1. Initialize curl
    CURLcode global_ret = curl_global_init(CURL_GLOBAL_ALL);
    if (global_ret != CURLE_OK) {
	return 0;
	}
	

    // Create file for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 2. Create URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Parse first part of data as URL scheme (limit to safe length)
    size_t url_len = size > 100 ? 100 : size;
    char *url_scheme = (char*)malloc(url_len + 1);
    if (!url_scheme) {
        curl_url_cleanup(url_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    memcpy(url_scheme, data, url_len);
    url_scheme[url_len] = '\0';
    
    // Set URL parts
    curl_url_set(url_handle, CURLUPART_SCHEME, "http", 0);
    curl_url_set(url_handle, CURLUPART_HOST, "example.com", 0);
    curl_url_set(url_handle, CURLUPART_PATH, "/test", 0);
    
    // Get URL back
    char *url_part = NULL;
    curl_url_get(url_handle, CURLUPART_URL, &url_part, 0);
    if (url_part) curl_free(url_part);
    free(url_scheme);

    // 3. Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_url_cleanup(url_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 4. Create MIME structure
    curl_mime *mime = curl_mime_init(easy);
    if (!mime) {
        curl_easy_cleanup(easy);
        curl_url_cleanup(url_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 5. Add MIME part with data callback
    curl_mimepart *part = curl_mime_addpart(mime);
    if (part) {
        // Rewind input file
        fseek(in_file, 0, SEEK_SET);
        
        // Set up mime part with callback
        curl_mime_data_cb(part, (curl_off_t)size, 
                          id_000040_read_callback,  id_000040_seek_callback,  id_000040_free_callback, in_file);
        curl_mime_name(part, "fuzzdata");
        curl_mime_filename(part, "fuzz.bin");
    }

    // 6. Set easy handle options
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, out_file);
    
    // Check easy option by ID
    const struct curl_easyoption *opt = curl_easy_option_by_id(CURLOPT_URL);
    if (opt) {
        // Option exists, could use it
    }

    // 7. Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        curl_url_cleanup(url_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 8. Add easy handle to multi
    CURLMcode mret = curl_multi_add_handle(multi, easy);
    if (mret != CURLM_OK) {
        curl_multi_cleanup(multi);
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        curl_url_cleanup(url_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 9. Create share handle
    CURLSH *share = curl_share_init();
    if (share) {
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
        
        // Get share error string (for demonstration)
        const char *share_err = curl_share_strerror(CURLSHE_OK);
        (void)share_err; // Suppress unused warning
        
        curl_share_cleanup(share);
    }

    // 10. Multi handle operations
    int running_handles = 0;
    
    // Use multi_waitfds
    struct curl_waitfd ufds[1];
    unsigned int fd_count = 0;
    curl_multi_waitfds(multi, ufds, 1, &fd_count);
    
    // Also try fdset for compatibility
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    int max_fd = 0;
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);
    curl_multi_fdset(multi, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);
    
    // Disable notifications
    curl_multi_notify_disable(multi, 1);
    
    // Perform (won't actually transfer without network, but will process)
    curl_multi_perform(multi, &running_handles);
    
    // 11. Use curl_mfprintf
    curl_mfprintf(out_file, "Fuzzed %zu bytes\n", size);
    
    // 12. Clean up multi handle
    curl_multi_remove_handle(multi, easy);
    curl_multi_cleanup(multi);

    // 13. Clean up everything else
    curl_mime_free(mime);
    curl_easy_cleanup(easy);
    curl_url_cleanup(url_handle);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Final cleanup
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
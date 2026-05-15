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
#include <mprintf.h>
}
//<ID> 537
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 13
//<Quality> {"density":9,"unique_branches":{"curl_url_strerror":[[427,3,427,25,0,0,4,0],[430,3,430,30,0,0,4,0],[442,3,442,24,0,0,4,0],[445,3,445,28,0,0,4,0],[448,3,448,31,0,0,4,0],[451,3,451,27,0,0,4,0],[457,3,457,22,0,0,4,0],[460,3,460,26,0,0,4,0],[463,3,463,25,0,0,4,0],[469,3,469,22,0,0,4,0],[472,3,472,23,0,0,4,0],[475,3,475,26,0,0,4,0],[478,3,478,24,0,0,4,0]]},"library_calls":["curl_global_init_mem","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_easy_duphandle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_multi_assign","curl_url_strerror","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_easy_init","curl_multi_init","curl_easy_duphandle","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_multi_assign","curl_url_strerror","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Custom memory management functions for curl_global_init_mem
static void * id_000036_custom_malloc(size_t size) {
    return malloc(size);
}

static void  id_000036_custom_free(void *ptr) {
    free(ptr);
}

static void * id_000036_custom_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char * id_000036_custom_strdup(const char *str) {
    if (!str) return nullptr;
    return strdup(str);
}

static void * id_000036_custom_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

// Callbacks for curl_mime_data_cb
static size_t  id_000036_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *file = (FILE *)arg;
    return fread(buffer, size, nitems, file);
}

static int  id_000036_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *file = (FILE *)arg;
    return fseek(file, offset, origin);
}

static void  id_000036_free_callback(void *arg) {
    // Nothing to free since we're using fmemopen
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize curl with custom memory functions
    if (curl_global_init_mem(CURL_GLOBAL_DEFAULT,
                              id_000036_custom_malloc,
                              id_000036_custom_free,
                              id_000036_custom_realloc,
                              id_000036_custom_strdup,
                              id_000036_custom_calloc) != CURLE_OK) {
        return 0;
    }

    // Create a memory file for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Duplicate easy handle
    CURL *dup_handle = curl_easy_duphandle(easy_handle);
    if (!dup_handle) {
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create mime structure and add part
    curl_mime *mime = curl_mime_init(dup_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Set mime data callback that reads from our input file
            curl_mime_data_cb(part, 
                             (curl_off_t)size,
                              id_000036_read_callback,
                              id_000036_seek_callback,
                              id_000036_free_callback,
                             in_file);
        }
    }

    // Get file descriptor from input file
    int fd = fuzz_fileno(in_file);
    if (fd != -1) {
        // Assign socket to multi handle
        curl_multi_assign(multi_handle, (curl_socket_t)fd, nullptr);
    }

    // Create URL error for demonstration
    CURLUcode url_error = CURLUE_OK;
    if (size > 0) {
        // Use first byte to determine error code
        url_error = (CURLUcode)(data[0] % 20);  // CURLUcode has ~15 error codes
    }
    
    // Get URL error string
    const char *error_str = curl_url_strerror(url_error);
    (void)error_str;  // Use variable to avoid unused warning

    // Clean up in reverse order
    if (mime) {
        curl_mime_free(mime);
    }
    curl_easy_cleanup(dup_handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy_handle);
    assert_file_closed(&in_file);;
    curl_global_cleanup();

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
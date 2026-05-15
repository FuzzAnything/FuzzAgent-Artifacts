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
//<ID> 535
//<Prompt> []
/*<Combination>: [
*/
//<score> 63, nr_unique_branch: 25
//<Quality> {"density":21,"unique_branches":{"dupset":[[920,6,920,24,0,0,4,0],[923,8,923,27,0,0,4,1],[928,8,928,14,0,0,4,1]],"curl_mime_filename":[[1263,6,1263,14,0,0,4,1]],"Curl_mime_duppart":[[1110,3,1110,21,0,0,4,1],[1112,3,1112,21,0,0,4,1],[1115,3,1115,21,0,0,4,1],[1121,3,1121,25,0,0,4,0],[1121,3,1121,25,0,0,4,1],[1125,3,1125,26,0,0,4,0],[1125,3,1125,26,0,0,4,1],[1129,11,1129,15,0,0,4,0],[1132,49,1132,53,0,0,4,0],[1132,57,1132,58,0,0,4,0],[1132,57,1132,58,0,0,4,1],[1134,13,1134,14,0,0,4,0],[1137,3,1137,10,0,0,4,1],[1144,6,1144,10,0,0,4,0],[1144,14,1144,30,0,0,4,1],[1158,6,1158,10,0,0,4,0],[1163,6,1163,10,0,0,4,0],[1165,6,1165,10,0,0,4,0],[1169,6,1169,9,0,0,4,1]],"curl_mime_type":[[1355,6,1355,14,0,0,4,1]],"curl_mime_name":[[1246,6,1246,10,0,0,4,1]]},"library_calls":["curl_global_init_mem","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_data_cb","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_duphandle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_multi_assign","curl_url_strerror","curl_easy_cleanup","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_easy_setopt","curl_easy_duphandle","curl_multi_assign","curl_url_strerror","curl_easy_cleanup","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Custom memory callbacks for curl_global_init_mem
static void * id_000035_custom_malloc(size_t size) {
    return malloc(size);
}

static void  id_000035_custom_free(void *ptr) {
    free(ptr);
}

static void * id_000035_custom_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char * id_000035_custom_strdup(const char *str) {
    return strdup(str);
}

static void * id_000035_custom_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

// Callbacks for curl_mime_data_cb
static size_t  id_000035_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *f = (FILE*)arg;
    return fread(buffer, size, nitems, f);
}

static int  id_000035_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *f = (FILE*)arg;
    return fseek(f, (long)offset, origin);
}

static void  id_000035_free_callback(void *arg) {
    // File is closed elsewhere
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. Initialize libcurl with custom memory functions
    CURLcode global_rc = curl_global_init_mem(CURL_GLOBAL_ALL,
                                               id_000035_custom_malloc,
                                               id_000035_custom_free,
                                               id_000035_custom_realloc,
                                               id_000035_custom_strdup,
                                               id_000035_custom_calloc);
    if (global_rc != CURLE_OK) {
	return 0;
	}
	
    
    // Create a multi handle for curl_multi_assign
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // 2. Create easy handle and set up mime with data callback
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Add a mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Open input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Set up mime data callback using the input file
    CURLcode mime_rc = curl_mime_data_cb(part, (curl_off_t)size,
                                          id_000035_read_callback,
                                          id_000035_seek_callback,
                                          id_000035_free_callback,
                                         in_file);
    if (mime_rc != CURLE_OK) {
        assert_file_closed(&in_file);;
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set mime to easy handle (simulating form post)
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    
    // 4. Duplicate the easy handle
    CURL *duplicate_handle = curl_easy_duphandle(easy_handle);
    if (!duplicate_handle) {
        assert_file_closed(&in_file);;
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Create output file and get its descriptor for curl_multi_assign
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_easy_cleanup(duplicate_handle);
        assert_file_closed(&in_file);;
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for assignment
    curl_socket_t sockfd = (curl_socket_t)fuzz_fileno(out_file);
    
    // 6. Assign socket to multi handle (simulating socket management)
    // We use NULL as sockp since we don't have actual socket data
    CURLMcode assign_rc = curl_multi_assign(multi_handle, sockfd, NULL);
    
    // 7. Get URL error string (even if no error occurred)
    const char *error_str = curl_url_strerror(CURLUE_OK);
    
    // Clean up in reverse order
    assert_file_closed(&out_file);;
    remove("output_file");
    
    curl_easy_cleanup(duplicate_handle);
    assert_file_closed(&in_file);;  // Close after curl might have used it
    curl_mime_free(mime);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    // Use error_str to prevent optimization
    if (error_str) {
        // Do nothing, just reference to avoid unused variable warning
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(sockfd);
	return 0;
}
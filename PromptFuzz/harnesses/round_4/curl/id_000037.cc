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
//<ID> 539
//<Prompt> []
/*<Combination>: [
*/
//<score> 29, nr_unique_branch: 2
//<Quality> {"density":29,"unique_branches":{"curl_url_strerror":[[493,3,493,26,0,0,4,0],[496,3,496,25,0,0,4,0]]},"library_calls":["curl_global_init_mem","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_duphandle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_data_cb","curl_easy_setopt","curl_multi_add_handle","curl_multi_assign","curl_url","curl_url_set","curl_url_strerror","curl_url_get","curl_url_strerror","curl_free","curl_url_cleanup","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_multi_init","curl_easy_init","curl_easy_duphandle","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_easy_setopt","curl_multi_add_handle","curl_multi_assign","curl_url","curl_url_set","curl_url_strerror","curl_url_get","curl_url_strerror","curl_free","curl_url_cleanup","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":1}
/*To create a fuzz driver using the specified curl APIs that consumes the input data, I'll simulate a scenario where we:

1. Initialize curl with custom memory callbacks
2. Create a multi handle and assign a socket
3. Duplicate an easy handle
4. Set up a mime part with data callback using the input
5. Handle URL parsing errors

Here's the implementation:

*/


extern "C" {
}

// Custom memory callbacks
static void * id_000037_custom_malloc(size_t size) {
    return malloc(size);
}

static void  id_000037_custom_free(void *ptr) {
    free(ptr);
}

static void * id_000037_custom_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char * id_000037_custom_strdup(const char *str) {
    return strdup(str);
}

static void * id_000037_custom_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

// Callbacks for mime data
static size_t  id_000037_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *f = (FILE *)arg;
    return fread(buffer, size, nitems, f);
}

static int  id_000037_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *f = (FILE *)arg;
    return fseek(f, (long)offset, origin);
}

static void  id_000037_free_callback(void *arg) {
    // Nothing to free here since we'll close the file separately
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // 1. Initialize curl with custom memory callbacks
    curl_global_init_mem(CURL_GLOBAL_ALL,
                         id_000037_custom_malloc,
                         id_000037_custom_free,
                         id_000037_custom_realloc,
                         id_000037_custom_strdup,
                         id_000037_custom_calloc);

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // 3. Duplicate the easy handle
    CURL *dup_handle = curl_easy_duphandle(easy_handle);
    if (!dup_handle) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create a mime structure
    curl_mime *mime = curl_mime_init(dup_handle);
    if (!mime) {
        curl_easy_cleanup(dup_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create a mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(dup_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_mime_free(mime);
        curl_easy_cleanup(dup_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Set up mime data callback using input data
    // Use the first 4 bytes to determine data size for callback
    size_t data_size = size;
    if (size >= 4) {
        // Use a simple calculation: use 1/4 of the input data for callback
        data_size = size / 4;
        if (data_size < 1) data_size = 1;
    }

    curl_mime_data_cb(part, data_size,  id_000037_read_callback,  id_000037_seek_callback,  id_000037_free_callback, in_file);

    // Set the mime data to the easy handle
    curl_easy_setopt(dup_handle, CURLOPT_MIMEPOST, mime);

    // Add the duplicated handle to multi handle
    curl_multi_add_handle(multi_handle, dup_handle);

    // 2. Assign a socket to the multi handle
    // Create output file to get a file descriptor
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int sockfd = fuzz_fileno(out_file);
        if (sockfd >= 0) {
            // Use a dummy pointer for sockp (could be any pointer)
            void *sockp = &data;
            curl_multi_assign(multi_handle, sockfd, sockp);
        }
    }

    // 5. Use curl_url_strerror to demonstrate error handling
    // Create a URL handle and try to parse something
    CURLU *url_handle = curl_url();
    if (url_handle) {
        // Try to set a URL part from input data (first 100 bytes or less)
        size_t url_part_size = size > 100 ? 100 : size;
        char *url_part = (char *)malloc(url_part_size + 1);
        if (url_part) {
            memcpy(url_part, data, url_part_size);
            url_part[url_part_size] = '\0';
            
            CURLUcode url_error = curl_url_set(url_handle, CURLUPART_URL, url_part, fuzz_uint32_t_1);
            
            // Get error string for the URL error
            const char *error_str = curl_url_strerror(url_error);
            (void)error_str; // Prevent unused variable warning
            
            free(url_part);
        }
        
        // Also test getting a URL part
        char *scheme = NULL;
        CURLUcode get_error = curl_url_get(url_handle, CURLUPART_SCHEME, &scheme, fuzz_uint32_t_2);
        const char *get_error_str = curl_url_strerror(get_error);
        (void)get_error_str; // Prevent unused variable warning
        
        if (scheme) {
            curl_free(scheme);
        }
        
        curl_url_cleanup(url_handle);
    }

    // Perform a minimal multi perform
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);

    // Clean up
    curl_multi_remove_handle(multi_handle, dup_handle);
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    curl_mime_free(mime);
    curl_easy_cleanup(dup_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
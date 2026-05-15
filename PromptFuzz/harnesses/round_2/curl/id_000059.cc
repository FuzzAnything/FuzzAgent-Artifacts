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
//<ID> 1085
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 3
//<Quality> {"density":33,"unique_branches":{"setopt_func":[[2552,3,2552,29,0,0,4,1],[2568,3,2568,28,0,0,4,0],[2573,8,2573,26,0,0,4,1]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_data","curl_strnequal","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_slist_append","curl_easy_setopt","curl_multi_add_handle","curl_multi_timeout","curl_multi_perform","curl_multi_wakeup","curl_multi_perform","curl_multi_info_read","curl_easy_nextheader","curl_multi_remove_handle","curl_multi_remove_handle","curl_mime_free","curl_slist_free_all","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_data","curl_strnequal","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_slist_append","curl_easy_setopt","curl_multi_add_handle","curl_multi_timeout","curl_multi_perform","curl_multi_wakeup","curl_multi_perform","curl_multi_info_read","curl_easy_nextheader","curl_multi_remove_handle","curl_multi_remove_handle","curl_mime_free","curl_slist_free_all","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Callback functions that will be used in the fuzz test
static size_t  id_000059_read_callback(char *buffer, size_t size, size_t nitems, void *instream) {
    FILE *in_file = (FILE *)instream;
    return fread(buffer, size, nitems, in_file);
}

static size_t  id_000059_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;  // Just consume the data
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize global curl environment
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Initialize easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }

    // Initialize multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }

    // Create a file from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_multi_cleanup(multi);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_multi_cleanup(multi);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;  // Mark as used
    (void)out_fd; // Mark as used

    // Create mime structure
    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use random data from input for mime
            const char *mime_data = (const char *)data;
            size_t mime_size = size > 100 ? 100 : size;
            
            curl_mime_encoder(part, "base64");
            curl_mime_data(part, mime_data, mime_size);
            
            // Compare strings using curl_strnequal
            if (size > 200) {
                const char *s1 = (const char *)data;
                const char *s2 = (const char *)data + 100;
                curl_strnequal(s1, s2, 50);
            }
        }
    }

    // Set up easy handle options
    curl_easy_setopt(easy, CURLOPT_URL, "file://input_file");
    curl_easy_setopt(easy, CURLOPT_READFUNCTION,  id_000059_read_callback);
    curl_easy_setopt(easy, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION,  id_000059_write_callback);
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, out_file);
    curl_easy_setopt(easy, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);

    // Create and set headers
    struct curl_slist *headers = NULL;
    if (size > 10) {
        char header[100];
        snprintf(header, sizeof(header), "X-Fuzz: %.10s", data);
        headers = curl_slist_append(headers, header);
        curl_easy_setopt(easy, CURLOPT_HTTPHEADER, headers);
    }

    // Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);

    // Perform multi operations
    int running_handles = 0;
    long timeout = 0;
    
    // Get timeout
    curl_multi_timeout(multi, &timeout);
    
    // Perform initial transfer
    curl_multi_perform(multi, &running_handles);
    
    // Wake up multi handle
    curl_multi_wakeup(multi);
    
    // Perform again to process wakeup
    curl_multi_perform(multi, &running_handles);

    // Get info about completed transfers
    int msgs_in_queue = 0;
    CURLMsg *msg = curl_multi_info_read(multi, &msgs_in_queue);
    (void)msg;  // Mark as used

    // Try to read headers (even though we're using file protocol)
    struct curl_header *hdr = curl_easy_nextheader(easy, 0, 0, NULL);
    (void)hdr;  // Mark as used

    // Remove handle from multi (called twice as requested)
    curl_multi_remove_handle(multi, easy);
    curl_multi_remove_handle(multi, easy);  // Second call as per requirement

    // Clean up
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
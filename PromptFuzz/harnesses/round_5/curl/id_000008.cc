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
//<ID> 27
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 1
//<Quality> {"density":27,"unique_branches":{"multi_addmsg":[[223,6,223,40,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_easy_duphandle","curl_easy_cleanup","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_data","curl_slist_append","curl_slist_append","curl_mime_headers","curl_easy_setopt","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_ws_meta","curl_easy_option_by_id","curl_easy_ssls_export","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_easy_duphandle","curl_multi_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_slist_append","curl_slist_append","curl_mime_headers","curl_easy_setopt","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_ws_meta","curl_easy_option_by_id","curl_easy_ssls_export","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


// Include necessary curl headers
extern "C" {
}

// Helper function to print using curl_mvprintf
static void  id_000008_print_message(const char* format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

// SSL export callback function
static CURLcode  id_000008_ssl_export_callback(CURL *handle, void *userptr,
                                   const char *session_key,
                                   const unsigned char *shmac,
                                   size_t shmac_len,
                                   const unsigned char *sdata,
                                   size_t sdata_len,
                                   curl_off_t age, int flags,
                                   const char *cipher, size_t cipher_len) {
    // Print exported SSL data
     id_000008_print_message("SSL Export: session_key=%s, age=%ld\n", 
                  session_key ? session_key : "(null)", (long)age);
    return CURLE_OK;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize curl globally
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }

    // Create a file from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 1. Duplicate the easy handle
    CURL *duplicated_handle = curl_easy_duphandle(easy_handle);
    if (!duplicated_handle) {
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 2. Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(duplicated_handle);
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 3. Create a mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(duplicated_handle);
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 4. Add a mime part
    curl_mimepart *mime_part = curl_mime_addpart(mime);
    if (mime_part) {
        // Set mime data from input
        if (size > 0) {
            // Use first 256 bytes for mime data (or entire input if smaller)
            size_t mime_data_size = size > 256 ? 256 : size;
            curl_mime_data(mime_part, (const char*)data, mime_data_size);
        }
        
        // Create headers for mime part
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
        if (size > 0) {
            char header_buf[100];
            snprintf(header_buf, sizeof(header_buf), "Content-Length: %zu", size);
            headers = curl_slist_append(headers, header_buf);
        }
        
        // 5. Set mime headers
        curl_mime_headers(mime_part, headers, 1); // take ownership
        
        // Set mime to easy handle
        curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    }

    // 6. Add handles to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    curl_multi_add_handle(multi_handle, duplicated_handle);

    // 7. Perform multi transfer
    int running_handles = 0;
    CURLMcode multi_res = curl_multi_perform(multi_handle, &running_handles);
    
    // Print result
     id_000008_print_message("Multi perform result: %d, running handles: %d\n", 
                  multi_res, running_handles);

    // 8. Get WebSocket metadata (if any)
    const struct curl_ws_frame *ws_meta = curl_ws_meta(easy_handle);
    if (ws_meta) {
         id_000008_print_message("WebSocket metadata: age=%d, flags=%d, len=%zu\n",
                      ws_meta->age, ws_meta->flags, ws_meta->len);
    }

    // 9. Get easy option by ID
    const struct curl_easyoption *option = curl_easy_option_by_id(CURLOPT_URL);
    if (option) {
         id_000008_print_message("Easy option by ID: name=%s\n", option->name);
    }

    // 10. Export SSL data
    curl_easy_ssls_export(easy_handle,  id_000008_ssl_export_callback, NULL);

    // Clean up multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_remove_handle(multi_handle, duplicated_handle);
    curl_multi_cleanup(multi_handle);

    // Clean up mime
    curl_mime_free(mime);

    // Clean up easy handles
    curl_easy_cleanup(duplicated_handle);
    curl_easy_cleanup(easy_handle);

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Clean up global curl
    curl_global_cleanup();

    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
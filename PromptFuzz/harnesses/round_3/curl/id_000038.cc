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
//<ID> 506
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 2
//<Quality> {"density":44,"unique_branches":{"cf_ip_ballers_pollset":[[566,8,566,17,0,0,4,0]],"setopt_func":[[2573,8,2573,26,0,0,4,0]]},"library_calls":["curl_global_init","curl_share_init","curl_global_cleanup","curl_multi_init","curl_share_cleanup","curl_global_cleanup","curl_multi_setopt","curl_easy_init","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_data_cb","curl_mime_name","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_formadd","curl_formget","curl_easy_init","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_slist_free_all","curl_easy_cleanup","curl_multi_socket_action","curl_multi_info_read","curl_multi_remove_handle","curl_easy_cleanup","curl_mime_free","curl_formfree","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_multi_init","curl_multi_setopt","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_name","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_formadd","curl_formget","curl_easy_init","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_slist_free_all","curl_easy_cleanup","curl_multi_socket_action","curl_multi_info_read","curl_multi_remove_handle","curl_easy_cleanup","curl_mime_free","curl_formfree","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified curl APIs to simulate a multi-handle HTTP POST with multipart/form-data and legacy form data:

*/


extern "C" {
}

// Custom structure to hold our input data for callbacks
struct mime_data_context {
    const uint8_t* data;
    size_t size;
    size_t position;
};

// Callback function for mime data reading
static size_t  id_000038_mime_read_callback(char* buffer, size_t size, size_t nitems, void* arg) {
    mime_data_context* ctx = (mime_data_context*)arg;
    size_t total_size = size * nitems;
    size_t bytes_to_copy = 0;
    
    if (ctx->position < ctx->size) {
        bytes_to_copy = (ctx->size - ctx->position < total_size) ? 
                       ctx->size - ctx->position : total_size;
        memcpy(buffer, ctx->data + ctx->position, bytes_to_copy);
        ctx->position += bytes_to_copy;
    }
    
    return bytes_to_copy;
}

// Callback function for mime seeking
static int  id_000038_mime_seek_callback(void* arg, curl_off_t offset, int origin) {
    mime_data_context* ctx = (mime_data_context*)arg;
    
    switch (origin) {
        case SEEK_SET:
            if (offset < 0 || (size_t)offset > ctx->size) return CURL_SEEKFUNC_FAIL;
            ctx->position = (size_t)offset;
            break;
        case SEEK_CUR:
            if (ctx->position + offset < 0 || 
                ctx->position + offset > ctx->size) return CURL_SEEKFUNC_FAIL;
            ctx->position += offset;
            break;
        case SEEK_END:
            if (ctx->size + offset < 0 || 
                ctx->size + offset > ctx->size) return CURL_SEEKFUNC_FAIL;
            ctx->position = ctx->size + offset;
            break;
        default:
            return CURL_SEEKFUNC_FAIL;
    }
    
    return CURL_SEEKFUNC_OK;
}

// Callback function for freeing mime data context
static void  id_000038_mime_free_callback(void* arg) {
    mime_data_context* ctx = (mime_data_context*)arg;
    free(ctx);
}

// Callback for curl_formget
static size_t  id_000038_formget_callback(void* arg, const char* buf, size_t len) {
    FILE* out_file = (FILE*)arg;
    if (out_file) {
        return fwrite(buf, 1, len, out_file);
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	
    
    // Initialize curl
    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    if (res != CURLE_OK) {
	return 0;
	}
	
    
    // Create share handle for sharing data between easy handles
    CURLSH* share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create multi handle
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Set multi handle options
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
    
    // Create easy handle for multi interface
    CURL* easy_multi = curl_easy_init();
    if (!easy_multi) {
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Create mime structure for multipart data
    curl_mime* mime = curl_mime_init(easy_multi);
    if (!mime) {
        curl_easy_cleanup(easy_multi);
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Add mime part
    curl_mimepart* part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_multi);
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Create mime data context
    mime_data_context* ctx = (mime_data_context*)malloc(sizeof(mime_data_context));
    if (!ctx) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_multi);
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    ctx->data = data;
    ctx->size = size;
    ctx->position = 0;
    
    // Set up mime part with data callback
    curl_mime_data_cb(part, size,  id_000038_mime_read_callback, 
                       id_000038_mime_seek_callback,  id_000038_mime_free_callback, ctx);
    
    // Set mime part name
    curl_mime_name(part, fuzz_str_1);
    
    // Set easy handle options for multi interface
    curl_easy_setopt(easy_multi, CURLOPT_SHARE, share);
    curl_easy_setopt(easy_multi, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy_multi, CURLOPT_URL, "http://localhost/test");
    curl_easy_setopt(easy_multi, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy_multi, CURLOPT_READFUNCTION, NULL);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_multi);
    
    // Create legacy form data
    struct curl_httppost* formpost = NULL;
    struct curl_httppost* lastptr = NULL;
    
    // Use part of input data for legacy form
    size_t form_data_size = size > 100 ? 100 : size;
    char* form_data = (char*)malloc(form_data_size + 1);
    if (form_data) {
        memcpy(form_data, data, form_data_size);
        form_data[form_data_size] = '\0';
        
        curl_formadd(&formpost, &lastptr,
                    CURLFORM_COPYNAME, "legacy_field",
                    CURLFORM_COPYCONTENTS, form_data,
                    CURLFORM_END);
        
        free(form_data);
    }
    
    // Create output file for formget
    FILE* out_file = fopen("output_file", "wb");
    if (out_file && formpost) {
        curl_formget(formpost, out_file,  id_000038_formget_callback);
        assert_file_closed(&out_file);;
    } else {
        if (out_file) assert_file_closed(&out_file);;
    }
    
    // Create another easy handle for simple perform
    CURL* easy_simple = curl_easy_init();
    if (easy_simple) {
        // Create header list
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, fuzz_str_2);
        
        // Set options for simple easy handle
        curl_easy_setopt(easy_simple, CURLOPT_SHARE, share);
        curl_easy_setopt(easy_simple, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(easy_simple, CURLOPT_URL, "http://localhost/simple");
        curl_easy_setopt(easy_simple, CURLOPT_NOBODY, 1L);
        
        // Perform the request
        curl_easy_perform(easy_simple);
        
        // Clean up headers
        curl_slist_free_all(headers);
        curl_easy_cleanup(easy_simple);
    }
    
    // Perform multi handle operations
    int still_running = 1;
    int numfds = 0;
    
    // Create input file from fuzz data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        
        // Simulate socket activity (this would normally come from actual network events)
        curl_multi_socket_action(multi_handle, fd, fuzz_int32_t_3, &still_running);
        
        assert_file_closed(&in_file);;
    }
    
    // Check for multi handle messages
    int msgs_in_queue = 0;
    CURLMsg* msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    
    // Clean up multi handle resources
    curl_multi_remove_handle(multi_handle, easy_multi);
    curl_easy_cleanup(easy_multi);
    curl_mime_free(mime);
    
    // Clean up form data
    if (formpost) {
        curl_formfree(formpost);
    }
    
    // Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    // Clean up share handle
    curl_share_cleanup(share);
    
    // Final cleanup
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
}
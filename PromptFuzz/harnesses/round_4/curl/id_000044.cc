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
//<ID> 686
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 1
//<Quality> {"density":25,"unique_branches":{"setopt_func":[[2573,8,2573,26,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_trace","curl_version","curl_mprintf","curl_multi_init","curl_global_cleanup","curl_multi_notify_disable","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_data_cb","curl_share_strerror","curl_url_strerror","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_fdset","curl_easy_upkeep","curl_multi_socket_all","curl_multi_perform","curl_multi_remove_handle","curl_easy_strerror","curl_easy_cleanup","curl_mime_free","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_version","curl_mprintf","curl_multi_init","curl_multi_notify_disable","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_share_strerror","curl_url_strerror","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_fdset","curl_easy_upkeep","curl_multi_socket_all","curl_multi_perform","curl_multi_remove_handle","curl_easy_strerror","curl_easy_cleanup","curl_mime_free","curl_multi_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Custom structure for mime data callback
struct mime_callback_data {
    const uint8_t* data;
    size_t size;
    size_t pos;
    FILE* file;
};

// Read callback for mime data
static size_t  id_000044_read_callback(char* buffer, size_t size, size_t nitems, void* arg) {
    mime_callback_data* cb_data = (mime_callback_data*)arg;
    size_t total_size = size * nitems;
    size_t to_read = total_size;
    
    if (cb_data->pos + to_read > cb_data->size) {
        to_read = cb_data->size - cb_data->pos;
    }
    
    if (to_read > 0) {
        memcpy(buffer, cb_data->data + cb_data->pos, to_read);
        cb_data->pos += to_read;
    }
    
    return to_read;
}

// Seek callback for mime data
static int  id_000044_seek_callback(void* arg, curl_off_t offset, int origin) {
    mime_callback_data* cb_data = (mime_callback_data*)arg;
    
    switch (origin) {
        case SEEK_SET:
            if (offset < 0 || (size_t)offset > cb_data->size) {
                return CURL_SEEKFUNC_FAIL;
            }
            cb_data->pos = offset;
            break;
        case SEEK_CUR:
            if (cb_data->pos + offset < 0 || cb_data->pos + offset > cb_data->size) {
                return CURL_SEEKFUNC_FAIL;
            }
            cb_data->pos += offset;
            break;
        case SEEK_END:
            if (cb_data->size + offset < 0 || cb_data->size + offset > cb_data->size) {
                return CURL_SEEKFUNC_FAIL;
            }
            cb_data->pos = cb_data->size + offset;
            break;
        default:
            return CURL_SEEKFUNC_FAIL;
    }
    return CURL_SEEKFUNC_OK;
}

// Free callback for mime data
static void  id_000044_free_callback(void* arg) {
    mime_callback_data* cb_data = (mime_callback_data*)arg;
    if (cb_data->file) {
        fclose(cb_data->file);
    }
    free(cb_data);
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Set up trace configuration using input data
    if (size > 0) {
        char* trace_config = (char*)malloc(size + 1);
        if (trace_config) {
            memcpy(trace_config, data, size);
            trace_config[size] = '\0';
            curl_global_trace(trace_config);
            free(trace_config);
        }
    }
    
    // Get curl version
    char* version = curl_version();
    if (version) {
        curl_mprintf("Curl version: %s\n", version);
    }
    
    // Create multi handle
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Disable notifications (using first byte of data as notification ID)
    unsigned int notification_id = 0;
    if (size > 0) {
        notification_id = data[0] % 256;
    }
    curl_multi_notify_disable(multi_handle, fuzz_uint32_t_1);
    
    // Create easy handle
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create mime structure
    curl_mime* mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Add mime part
    curl_mimepart* part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Set up mime data callback using input data
    mime_callback_data* cb_data = (mime_callback_data*)malloc(sizeof(mime_callback_data));
    if (cb_data) {
        cb_data->data = data;
        cb_data->size = size;
        cb_data->pos = 0;
        cb_data->file = NULL;
        
        // Create input file from data
        if (size > 0) {
            cb_data->file = fmemopen((void*)data, size, "rb");
        }
        
        curl_mime_data_cb(part, size,  id_000044_read_callback,  id_000044_seek_callback,  id_000044_free_callback, cb_data);
    }
    
    // Get error strings (dummy calls to satisfy API requirement)
    const char* share_err = curl_share_strerror(CURLSHE_OK);
    const char* url_err = curl_url_strerror(CURLUE_OK);
    
    // Set mime filedata
    if (size > 0) {
        // Write data to a file first
        FILE* temp_file = fopen("input_file", "wb");
        if (temp_file) {
            fwrite(data, 1, size, temp_file);
            assert_file_closed(&temp_file);;
            curl_mime_filedata(part, "input_file");
        }
    }
    
    // Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost/");
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, NULL);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Set up fdset for multi handle
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);
    int max_fd = -1;
    
    curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);
    
    // Perform upkeep on easy handle
    curl_easy_upkeep(easy_handle);
    
    // Drive multi socket operations
    int running_handles = 0;
    curl_multi_socket_all(multi_handle, &running_handles);
    
    // Perform a single iteration
    int still_running = 0;
    curl_multi_perform(multi_handle, &still_running);
    
    // Remove easy handle from multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Get error string (dummy call)
    const char* easy_err = curl_easy_strerror(CURLE_OK);
    
    // Clean up
    curl_easy_cleanup(easy_handle);
    
    // Note: mime is cleaned up by curl_easy_cleanup
    // but we need to explicitly free if not attached
    if (mime) {
        curl_mime_free(mime);
    }
    
    // Free allocated memory for curl_mprintf output
    // curl_mprintf uses internal allocation, so we don't need to free
    
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    // Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}
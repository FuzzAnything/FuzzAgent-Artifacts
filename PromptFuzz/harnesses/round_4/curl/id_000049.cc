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
//<ID> 898
//<Prompt> []
/*<Combination>: [
*/
//<score> 217, nr_unique_branch: 10
//<Quality> {"density":31,"unique_branches":{"Curl_init_do":[[3524,6,3524,23,0,0,4,0]],"curl_multi_remove_handle":[[793,6,793,23,0,0,4,0],[134,5,134,8,2,0,4,1]],"Curl_pretransfer":[[510,11,510,48,0,0,4,1]],"setopt_long_net":[[881,3,881,23,0,0,4,0],[885,3,885,30,0,0,4,0]],"setopt_set_timeout_sec":[[55,6,55,14,0,0,4,1],[58,6,58,36,0,0,4,1]],"setopt_long_bool":[[465,3,465,22,0,0,4,0],[471,8,471,22,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_trace","curl_version","curl_mprintf","curl_multi_init","curl_global_cleanup","curl_multi_notify_disable","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_data_cb","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_fdset","curl_easy_upkeep","curl_multi_socket_all","curl_multi_perform","curl_multi_remove_handle","curl_easy_strerror","curl_share_strerror","curl_url_strerror","curl_mime_free","curl_easy_cleanup","curl_multi_remove_handle","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_version","curl_mprintf","curl_multi_init","curl_multi_notify_disable","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_fdset","curl_easy_upkeep","curl_multi_socket_all","curl_multi_perform","curl_multi_remove_handle","curl_easy_strerror","curl_share_strerror","curl_url_strerror","curl_mime_free","curl_easy_cleanup","curl_multi_remove_handle","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Define callback functions for curl_mime_data_cb
static size_t  id_000049_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *f = (FILE *)arg;
    return fread(buffer, size, nitems, f);
}

static int  id_000049_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *f = (FILE *)arg;
    return fseek(f, offset, origin);
}

static void  id_000049_free_callback(void *arg) {
    // Nothing to free since we're using fmemopen buffer
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
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





    if (size == 0) {
        return 0;
    }

    // Initialize global curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Set global trace configuration using input data
    // Ensure the config string is null-terminated
    char *trace_config = (char *)malloc(size + 1);
    if (!trace_config) {
        curl_global_cleanup();
        return 0;
    }
    memcpy(trace_config, data, size);
    trace_config[size] = '\0';
    
    // Use a portion of data for trace config
    size_t config_len = size > 100 ? 100 : size;
    char config_buf[101];
    memcpy(config_buf, data, config_len);
    config_buf[config_len] = '\0';
    curl_global_trace(config_buf);

    // Get curl version and print it
    char *version = curl_version();
    if (version) {
        curl_mprintf("CURL Version: %s\n", version);
    }

    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        free(trace_config);
        curl_global_cleanup();
        return 0;
    }

    // Disable a notification
    curl_multi_notify_disable(multi_handle, fuzz_uint32_t_1); // Disable first notification

    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        free(trace_config);
        curl_global_cleanup();
        return 0;
    }

    // Create a MIME structure
    struct curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        free(trace_config);
        curl_global_cleanup();
        return 0;
    }

    // Create MIME part
    struct curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        free(trace_config);
        curl_global_cleanup();
        return 0;
    }

    // Set up FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        free(trace_config);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Set MIME data callback using input data
    CURLcode mime_res = curl_mime_data_cb(part, (curl_off_t)size,
                                           id_000049_read_callback,
                                           id_000049_seek_callback,
                                           id_000049_free_callback,
                                          in_file);
    
    // Also set filedata (though we use data_cb above)
    curl_mime_filedata(part, "input_file");

    // Set MIME to easy handle
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    
    // Set other options
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost/test");
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_NOBODY, 1L); // HEAD request

    // Add easy handle to multi handle
    CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
    
    // Set up fd_set for select
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);
    int max_fd = -1;
    
    // Get file descriptors
    curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);
    
    // Perform upkeep on easy handle
    curl_easy_upkeep(easy_handle);
    
    // Perform socket actions
    int running_handles = 0;
    curl_multi_socket_all(multi_handle, &running_handles);
    
    // Perform multi handle operations
    curl_multi_perform(multi_handle, &running_handles);
    
    // Remove easy handle from multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Get error strings (even if not used for errors)
    const char *easy_error = curl_easy_strerror(CURLE_OK);
    const char *share_error = curl_share_strerror(CURLSHE_OK);
    const char *url_error = curl_url_strerror(CURLUE_OK);
    
    // Clean up resources
    assert_file_closed(&in_file);;
    curl_mime_free(mime);
    curl_easy_cleanup(easy_handle);
    
    // Remove handle again (to satisfy API call requirement)
    curl_multi_remove_handle(multi_handle, NULL);
    
    curl_multi_cleanup(multi_handle);
    free(trace_config);
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}
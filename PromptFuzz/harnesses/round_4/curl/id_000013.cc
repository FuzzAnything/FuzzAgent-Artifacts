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
//<ID> 75
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_version","curl_mprintf","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_data_cb","curl_mime_filedata","curl_multi_add_handle","curl_multi_socket_all","curl_easy_upkeep","curl_url_strerror","curl_share_strerror","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version","curl_mprintf","curl_easy_init","curl_multi_init","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_filedata","curl_multi_add_handle","curl_multi_socket_all","curl_easy_upkeep","curl_url_strerror","curl_share_strerror","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Custom callback functions for curl_mime_data_cb
static size_t  id_000013_read_callback(char *buffer, size_t size, size_t nitems, void *instream) {
    static size_t offset = 0;
    FILE *in_file = (FILE *)instream;
    if (!in_file) return CURL_READFUNC_ABORT;
    
    size_t bytes_to_read = size * nitems;
    size_t bytes_read = fread(buffer, 1, bytes_to_read, in_file);
    offset += bytes_read;
    return bytes_read;
}

static int  id_000013_seek_callback(void *instream, curl_off_t offset, int origin) {
    FILE *in_file = (FILE *)instream;
    if (!in_file) return CURL_SEEKFUNC_FAIL;
    
    int fseek_origin;
    switch (origin) {
        case SEEK_SET: fseek_origin = SEEK_SET; break;
        case SEEK_CUR: fseek_origin = SEEK_CUR; break;
        case SEEK_END: fseek_origin = SEEK_END; break;
        default: return CURL_SEEKFUNC_FAIL;
    }
    
    if (fseek(in_file, (long)offset, fseek_origin) != 0) {
        return CURL_SEEKFUNC_FAIL;
    }
    return CURL_SEEKFUNC_OK;
}

static void  id_000013_free_callback(void *ptr) {
    // Nothing to free in our case
}

// Helper function for curl_mvsprintf
static void  id_000013_format_string(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1) {
	return 0;
	}
	
    
    // Initialize curl
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_init_res != CURLE_OK) {
        return 0;
    }
    
    // Create temporary files for input/output
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
    
    // Get curl version
    char *version = curl_version();
    if (version) {
        // Use curl_mprintf to print version info
        char version_buffer[256];
        curl_mprintf("Libcurl version: %s\n", version);
         id_000013_format_string(version_buffer, "Version: %s", version);
    }
    
    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (part) {
        // Set up data callback using input file
        rewind(in_file);
        CURLcode res = curl_mime_data_cb(part, (curl_off_t)size,
                                          id_000013_read_callback,
                                          id_000013_seek_callback,
                                          id_000013_free_callback,
                                         in_file);
        
        // Also set file data
        curl_mime_filedata(part, "input_file");
    }
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Perform multi socket operations
    int running_handles = 0;
    CURLMcode multi_res = curl_multi_socket_all(multi_handle, &running_handles);
    
    // Perform easy handle upkeep
    curl_easy_upkeep(easy_handle);
    
    // Get error strings (even if no error occurred)
    const char *url_error = curl_url_strerror(CURLUE_OK);
    const char *share_error = curl_share_strerror(CURLSHE_OK);
    
    // Clean up
    curl_mime_free(mime);
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy_handle);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up global curl
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
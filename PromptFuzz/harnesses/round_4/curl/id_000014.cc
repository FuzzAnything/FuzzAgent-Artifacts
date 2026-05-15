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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["curl_version","curl_global_init","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_share_init","curl_url","curl_url_cleanup","curl_share_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_mime_addpart","curl_mime_data_cb","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_easy_upkeep","curl_mprintf","curl_url_strerror","curl_share_strerror","curl_share_setopt","curl_url_set","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_version","curl_global_init","curl_easy_init","curl_multi_init","curl_share_init","curl_url","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_mime_addpart","curl_mime_data_cb","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_easy_upkeep","curl_mprintf","curl_url_strerror","curl_share_strerror","curl_share_setopt","curl_url_set","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Callback function for curl_mime_data_cb
static size_t  id_000014_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *f = (FILE*)arg;
    return fread(buffer, size, nitems, f);
}

static int  id_000014_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *f = (FILE*)arg;
    return fseek(f, (long)offset, origin);
}

static void  id_000014_free_callback(void *arg) {
    FILE *f = (FILE*)arg;
    if (f) {
        assert_file_closed(&f);;
    }
}

// Custom function to use curl_mvsprintf
static void  id_000014_test_mvsprintf(const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
        return 0;
    }

    // 1. Get curl version
    char *version = curl_version();
    
    // 2. Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 3. Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // 4. Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    
    // 5. Create share handle
    CURLSH *share = curl_share_init();
    
    // 6. Create URL handle
    CURLU *url = curl_url();
    
    // 7. Create input file from fuzzing data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_url_cleanup(url);
        curl_share_cleanup(share);
        curl_multi_cleanup(multi);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 8. Create output file
    FILE *out_file = fopen("output_file", "wb");
    
    // 9. Create mime structure
    curl_mime *mime = curl_mime_init(curl);
    if (mime) {
        // 10. Add first mime part with file data
        curl_mimepart *part1 = curl_mime_addpart(mime);
        if (part1) {
            // Rewind the file for use in multiple parts
            rewind(in_file);
            curl_mime_filedata(part1, "input_file");
        }
        
        // 11. Add second mime part with callback
        curl_mimepart *part2 = curl_mime_addpart(mime);
        if (part2) {
            // Create another FILE* for the callback (curl will close it via  id_000014_free_callback)
            FILE *in_file2 = fmemopen((void *)data, size, "rb");
            if (in_file2) {
                curl_mime_data_cb(part2, (curl_off_t)size, 
                                  id_000014_read_callback, 
                                  id_000014_seek_callback, 
                                  id_000014_free_callback, 
                                 in_file2);
            }
        }
        
        // Set mime data to easy handle
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    }
    
    // 12. Set up easy handle options
    curl_easy_setopt(curl, CURLOPT_URL, "file://output_file");
    if (out_file) {
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, out_file);
    }
    
    // 13. Add easy handle to multi handle
    curl_multi_add_handle(multi, curl);
    
    // 14. Perform socket operations on all sockets
    int running_handles = 0;
    curl_multi_socket_all(multi, &running_handles);
    
    // 15. Perform upkeep on easy handle
    curl_easy_upkeep(curl);
    
    // 16. Test mprintf functions
    curl_mprintf("Curl version: %s\n", version);
    
    // 17. Test mvsprintf
     id_000014_test_mvsprintf("Fuzzing data size: %zu\n", size);
    
    // 18. Get error strings (even if no error occurred)
    const char *url_error = curl_url_strerror(CURLUE_OK);
    const char *share_error = curl_share_strerror(CURLSHE_OK);
    
    // 19. Set share option
    if (share) {
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    }
    
    // 20. Set URL parts
    if (url) {
        curl_url_set(url, CURLUPART_URL, fuzz_str_1, fuzz_uint32_t_2);
    }
    
    // Cleanup
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    curl_multi_remove_handle(multi, curl);
    curl_multi_cleanup(multi);
    curl_easy_cleanup(curl);
    
    if (share) {
        curl_share_cleanup(share);
    }
    
    if (url) {
        curl_url_cleanup(url);
    }
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
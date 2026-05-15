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
//<ID> 171
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 5
//<Quality> {"density":9,"unique_branches":{"parsefmt":[[252,14,252,25,0,0,4,1],[272,16,272,22,0,0,4,1],[274,16,274,53,0,0,4,1],[277,16,277,22,0,0,4,1]],"formatf":[[1003,13,1003,33,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_escape","curl_easy_unescape","curl_free","curl_free","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_mime_name","curl_mime_free","curl_easy_cleanup","curl_slist_append","curl_slist_append","curl_slist_free_all","curl_msnprintf","curl_mfprintf","curl_msnprintf","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_wait","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_escape","curl_easy_unescape","curl_free","curl_free","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_mime_name","curl_mime_free","curl_easy_cleanup","curl_slist_append","curl_slist_append","curl_slist_free_all","curl_msnprintf","curl_mfprintf","curl_msnprintf","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_wait","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Helper function to create a temporary file with fuzzer input
static FILE*  id_000012_create_temp_file(const uint8_t* data, size_t size) {
    FILE* file = fmemopen((void*)data, size, "rb");
    if (!file) {
        // Fallback to a regular file if fmemopen fails
        file = fopen("temp_input", "wb");
        if (file) {
            fwrite(data, 1, size, file);
            assert_file_closed(&file);;
            file = fopen("temp_input", "rb");
        }
    }
    return file;
}

// Callback for mime data (unused in this fuzzer)
static size_t  id_000012_read_callback(char* buffer, size_t size, size_t nitems, void* arg) {
    return 0;
}

static int  id_000012_seek_callback(void* arg, curl_off_t offset, int origin) {
    return CURL_SEEKFUNC_OK;
}

static void  id_000012_free_callback(void* arg) {}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<50) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	
    
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a temporary file for input data
    FILE* in_file =  id_000012_create_temp_file(data, size);
    if (!in_file) {
        curl_global_cleanup();
        return 0;
    }
    
    // Get file descriptor for the input file
    int fd_in = fuzz_fileno(in_file);
    (void)fd_in; // May be used in curl_multi_wait
    
    // Create an output file
    FILE* out_file = fopen("output_file", "wb");
    
    // 1. Use curl_escape and curl_easy_unescape
    // Extract a string from input data (ensuring null termination)
    size_t str_len = size > 100 ? 100 : size;
    char* input_str = (char*)malloc(str_len + 1);
    if (!input_str) {
        assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    memcpy(input_str, data, str_len);
    input_str[str_len] = '\0';
    
    // Escape the string
    char* escaped = curl_escape(input_str, (int)str_len);
    if (escaped) {
        // Unescape the escaped string
        int outlength = 0;
        char* unescaped = curl_easy_unescape(nullptr, escaped, -1, &outlength);
        if (unescaped) {
            curl_free(unescaped);
        }
        curl_free(escaped);
    }
    
    // 2. Use curl_mime_filedata with a mime part
    CURL* easy = curl_easy_init();
    if (easy) {
        curl_mime* mime = curl_mime_init(easy);
        if (mime) {
            curl_mimepart* part = curl_mime_addpart(mime);
            if (part) {
                // Use the temporary file for mime data
                curl_mime_filedata(part, fuzz_str_1);
                curl_mime_name(part, fuzz_str_2);
            }
            curl_mime_free(mime);
        }
        curl_easy_cleanup(easy);
    }
    
    // 3. Use curl_slist functions
    struct curl_slist* slist = nullptr;
    slist = curl_slist_append(slist, fuzz_str_3);
    slist = curl_slist_append(slist, input_str); // Use input data in header
    curl_slist_free_all(slist);
    
    // 4. Use formatted printing functions
    char buffer[256];
    
    // Use curl_msnprintf
    int printed = curl_msnprintf(buffer, sizeof(buffer), 
                                "Input size: %zu, First bytes: %.10s", 
                                size, input_str);
    
    // Use curl_mvprintf with variable arguments
    if (out_file) {
        // Create va_list for curl_mvprintf (simulate with fixed args)
        const char* format = "Test output: %s\n";
        int fd_out = fuzz_fileno(out_file);
        (void)fd_out;
        
        // Note: We can't easily create proper va_list here, so we'll use
        // the non-varargs version for demonstration
        curl_mfprintf(out_file, "Fuzzed data processed\n");
    }
    
    // Use curl_mvsnprintf (similarly, we'll use the non-varargs version)
    char buffer2[256];
    curl_msnprintf(buffer2, sizeof(buffer2), "Data: %.*s", 20, input_str);
    
    // 5. Use curl_multi_wait
    CURLM* multi = curl_multi_init();
    if (multi) {
        CURL* easy2 = curl_easy_init();
        if (easy2) {
            // Set some options (but don't actually perform transfer)
            curl_easy_setopt(easy2, CURLOPT_URL, "file://temp_input");
            curl_multi_add_handle(multi, easy2);
            
            // Create waitfd structure
            struct curl_waitfd waitfd;
            waitfd.fd = fd_in;
            waitfd.events = CURL_WAIT_POLLIN;
            waitfd.revents = 0;
            
            int running_handles = 0;
            int ret = 0;
            
            // Wait with 0 timeout (non-blocking)
            curl_multi_wait(multi, &waitfd, 1, fuzz_int32_t_4, &ret);
            
            // Perform (won't actually transfer due to file protocol)
            curl_multi_perform(multi, &running_handles);
            
            curl_multi_remove_handle(multi, easy2);
            curl_easy_cleanup(easy2);
        }
        curl_multi_cleanup(multi);
    }
    
    // Cleanup
    free(input_str);
    assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("temp_input");
    remove("output_file");
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_in);
	return 0;
}
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
//<ID> 166
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["curl_global_sslset","curl_global_init","curl_easy_init","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_global_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup","curl_mime_filename","curl_mime_filedata","curl_multi_init","curl_multi_add_handle","curl_multi_timeout","curl_multi_wakeup","curl_multi_remove_handle","curl_multi_cleanup","curl_strnequal","curl_free","curl_escape","curl_free","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_sslset","curl_global_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_mime_filedata","curl_multi_init","curl_multi_add_handle","curl_multi_timeout","curl_multi_wakeup","curl_multi_remove_handle","curl_multi_cleanup","curl_strnequal","curl_free","curl_escape","curl_free","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Helper function to create formatted string using curl_mvaprintf
static char*  id_000027_format_string(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

// Fuzzer entry point
extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 2) {
	return 0;
	}
	
    
    // Initialize global curl with SSL support
    CURLsslset sslset_result = curl_global_sslset(CURLSSLBACKEND_NONE, NULL, NULL);
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a MIME structure and part
    CURL* easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }
    
    curl_mime* mime = curl_mime_init(easy);
    if (!mime) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }
    
    curl_mimepart* part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }
    
    // Create input file from fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Use a portion of input data as filename
    size_t filename_len = size < 256 ? size : 256;
    char* filename = new char[filename_len + 1];
    memcpy(filename, data, filename_len);
    filename[filename_len] = '\0';
    
    // Set MIME filename using fuzzer data
    curl_mime_filename(part, filename);
    
    // Set MIME filedata using the input file
    rewind(in_file);
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    rewind(in_file);
    
    if (file_size > 0) {
        char* file_data = new char[file_size];
        fread(file_data, 1, file_size, in_file);
        curl_mime_filedata(part, "input_file");
        delete[] file_data;
    }
    
    // Create multi handle for concurrent operations
    CURLM* multi = curl_multi_init();
    if (multi) {
        // Add easy handle to multi handle
        curl_multi_add_handle(multi, easy);
        
        // Get timeout value from multi handle
        long timeout_ms;
        curl_multi_timeout(multi, &timeout_ms);
        
        // Wake up multi handle
        curl_multi_wakeup(multi);
        
        // Remove handle and cleanup multi
        curl_multi_remove_handle(multi, easy);
        curl_multi_cleanup(multi);
    }
    
    // Use curl_strnequal to compare strings
    const char* str1 = "test1";
    const char* str2 = "test2";
    int compare_result = curl_strnequal(fuzz_str_1, fuzz_str_2, fuzz_uint64_t_3);
    
    // Use curl_mvaprintf through helper function
    char* formatted =  id_000027_format_string("Compare result: %d, Size: %zu", compare_result, size);
    if (formatted) {
        curl_free(formatted);
    }
    
    // Use curl_escape on a portion of input data
    size_t escape_len = size < 128 ? size : 128;
    char* escape_input = new char[escape_len + 1];
    memcpy(escape_input, data, escape_len);
    escape_input[escape_len] = '\0';
    
    char* escaped = curl_escape(escape_input, (int)escape_len);
    if (escaped) {
        curl_free(escaped);
    }
    
    // Cleanup
    delete[] filename;
    delete[] escape_input;
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    curl_mime_free(mime);
    curl_easy_cleanup(easy);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}
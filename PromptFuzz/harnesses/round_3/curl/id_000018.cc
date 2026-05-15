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
//<ID> 193
//<Prompt> []
/*<Combination>: [
*/
//<score> 4.5, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["curl_global_init","curl_version_info","curl_mprintf","curl_mprintf","curl_easy_option_by_id","curl_mprintf","curl_formadd","curl_easy_init","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_easy_nextheader","curl_mprintf","curl_easy_nextheader","curl_slist_free_all","curl_easy_cleanup","curl_free","curl_mprintf","curl_formfree","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version_info","curl_mprintf","curl_mprintf","curl_easy_option_by_id","curl_mprintf","curl_formadd","curl_easy_init","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_easy_nextheader","curl_mprintf","curl_easy_nextheader","curl_slist_free_all","curl_easy_cleanup","curl_free","curl_mprintf","curl_formfree","curl_global_cleanup"],"visited":1}
/**/


// Helper function to use curl_mvaprintf
static char*  id_000018_va_print_helper(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<71) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 1. Use curl_version_info to get version data
    CURLversion stamp = CURLVERSION_NOW;
    curl_version_info_data* version_info = curl_version_info(stamp);
    if (version_info) {
        // Use curl_mprintf to print version info
        curl_mprintf("Version: %s\n", version_info->version);
        curl_mprintf("Features: %d\n", version_info->features);
    }
    
    // 2. Use curl_easy_option_by_id to get an option
    const struct curl_easyoption* option = curl_easy_option_by_id(CURLOPT_URL);
    if (option) {
        curl_mprintf("Option name: %s\n", option->name);
    }
    
    // 3. Use curl_formadd to create form data from input
    struct curl_httppost* formpost = NULL;
    struct curl_httppost* last_post = NULL;
    
    // Read first part of input for form name
    char name_buf[256] = {0};
    size_t name_len = size > 255 ? 255 : size;
    fread(name_buf, 1, name_len, in_file);
    
    // Read second part for form content (if available)
    char content_buf[256] = {0};
    size_t content_len = 0;
    if (size > name_len) {
        content_len = (size - name_len) > 255 ? 255 : (size - name_len);
        fread(content_buf, 1, content_len, in_file);
    }
    
    curl_formadd(&formpost, &last_post,
                CURLFORM_COPYNAME, name_buf,
                CURLFORM_COPYCONTENTS, content_buf,
                CURLFORM_END);
    
    // 4. Use curl_easy_nextheader - create a simple easy handle with headers
    CURL* easy = curl_easy_init();
    if (easy) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, fuzz_str_1);
        headers = curl_slist_append(headers, fuzz_str_2);
        
        curl_easy_setopt(easy, CURLOPT_HTTPHEADER, headers);
        
        // Try to get headers using curl_easy_nextheader
        struct curl_header* header = NULL;
        unsigned int origin = 0;
        int request = 0;
        
        header = curl_easy_nextheader(easy, fuzz_uint32_t_3, fuzz_int32_t_4, header);
        while (header) {
            curl_mprintf("Header: %s: %s\n", header->name, header->value);
            header = curl_easy_nextheader(easy, fuzz_uint32_t_5, fuzz_int32_t_6, header);
        }
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(easy);
    }
    
    // 5. Use curl_mvaprintf with input data
    char format_buf[256] = {0};
    size_t format_len = size > 255 ? 255 : size;
    fseek(in_file, 0, SEEK_SET);
    fread(format_buf, 1, format_len, in_file);
    
    char* formatted =  id_000018_va_print_helper("Input data: %.*s", (int)format_len, format_buf);
    if (formatted) {
        // Do something with formatted string
        curl_free(formatted);
    }
    
    // 6. Use curl_mprintf again with option info
    if (option) {
        curl_mprintf("Option ID: %d, Type: %d\n", option->id, option->type);
    }
    
    // Clean up
    curl_formfree(formpost);
    assert_file_closed(&in_file);;
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
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
//<ID> 1262
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"curl_multi_remove_handle":[[856,32,856,49,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_global_cleanup","curl_mime_filedata","curl_mime_encoder","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_url_strerror","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_mime_encoder","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_url_strerror","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Custom callback to simulate va_list usage for curl_mvfprintf
static int  id_000063_custom_write_callback(const char* format, ...) {
    va_list args;
    va_start(args, format);
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        va_end(args);
        return -1;
    }
    int result = curl_mvfprintf(out_file, format, args);
    assert_file_closed(&out_file);;
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<9) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // 2. Create input file using the fuzzing data
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create a CURL easy handle
    CURL* curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create a mime structure
    curl_mime* mime = curl_mime_init(curl);
    if (!mime) {
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Create a mime part
    curl_mimepart* part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Write input data to a temporary file for curl_mime_filedata
    // First, rewind the in_file to read from beginning
    rewind(in_file);
    FILE* tmp_file = fopen("input_file", "wb");
    if (tmp_file) {
        uint8_t buffer[4096];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
            fwrite(buffer, 1, bytes_read, tmp_file);
        }
        assert_file_closed(&tmp_file);;
    }
    assert_file_closed(&in_file);;
    
    // 7. Use curl_mime_filedata with the temporary file
    CURLcode res = curl_mime_filedata(part, "input_file");
    if (res != CURLE_OK) {
        // Handle error but continue with other API calls
    }
    
    // 8. Use curl_mime_encoder with a sample encoding
    res = curl_mime_encoder(part, fuzz_str_1);
    if (res != CURLE_OK) {
        // Handle error but continue
    }
    
    // 9. Set the mime data as POST fields
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    
    // 10. Set a dummy URL (required for curl_easy_perform)
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/test");
    
    // 11. Disable actual network communication for fuzzing safety
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    
    // 12. Perform the curl operation (will fail due to CONNECT_ONLY but still exercise the code)
    res = curl_easy_perform(curl);
    
    // 13. Use curl_url_strerror for a sample error code
    const char* error_str = curl_url_strerror(CURLUE_OK);
    
    // 14. Use curl_mvfprintf to write the error string
     id_000063_custom_write_callback("URL Error: %s\n", error_str ? error_str : "Unknown");
    
    // 15. Cleanup in proper order
    curl_mime_free(mime);
    curl_easy_cleanup(curl);
    
    // 16. Remove temporary files
    remove("input_file");
    remove("output_file");
    
    curl_global_cleanup();
    
    assert_file_closed(&tmp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}
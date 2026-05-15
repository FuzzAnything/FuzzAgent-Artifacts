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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 100, nr_unique_branch: 21
//<Quality> {"density":20,"unique_branches":{"multi_posix_poll":[[1491,6,1491,14,0,0,4,1],[1514,11,1514,20,0,0,4,1]],"getinfo_char":[[92,3,92,30,0,0,4,0],[94,20,94,21,0,0,4,0],[97,3,97,33,0,0,4,1],[127,3,127,29,0,0,4,1],[130,3,130,24,0,0,4,1],[133,3,133,31,0,0,4,1],[141,3,141,29,0,0,4,1],[146,3,146,24,0,0,4,1],[150,3,150,27,0,0,4,1],[154,3,154,25,0,0,4,1],[159,3,159,32,0,0,4,1],[166,3,166,23,0,0,4,1],[169,3,169,23,0,0,4,1],[176,3,176,23,0,0,4,1],[183,3,183,10,0,0,4,1]],"Curl_getinfo":[[642,3,642,23,0,0,4,0],[644,8,644,19,0,0,4,0]],"multi_wait":[[1592,19,1592,26,0,0,4,1],[1592,30,1592,40,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_easy_init","curl_multi_init","curl_easy_escape","curl_mfprintf","curl_free","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_wait","curl_multi_wakeup","curl_easy_getinfo","curl_mfprintf","curl_easy_header","curl_mfprintf","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_easy_escape","curl_mfprintf","curl_free","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_wait","curl_multi_wakeup","curl_easy_getinfo","curl_mfprintf","curl_easy_header","curl_mfprintf","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include necessary curl headers

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early return if data is too small for basic operations
    if (size < 10) {
	return 0;
	}
	
    
    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create a FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
    
    // Create easy and multi handles
    CURL *easy = curl_easy_init();
    CURLM *multi = curl_multi_init();
    
    if (easy && multi) {
        // 1. Use curl_easy_escape - escape a portion of input data
        const char *string_to_escape = reinterpret_cast<const char*>(data);
        int length_to_escape = size > 100 ? 100 : static_cast<int>(size);
        char *escaped = curl_easy_escape(easy, string_to_escape, length_to_escape);
        
        if (escaped) {
            // 2. Use curl_mprintf - print formatted string to output file
            curl_mfprintf(out_file, "Escaped string: %s\n", escaped);
            curl_free(escaped);
        }
        
        // 3. Use curl_mime_filedata - create mime part with input file
        curl_mime *mime = curl_mime_init(easy);
        if (mime) {
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // Write first 50 bytes to a temporary file for curl_mime_filedata
                FILE *tmp_file = fopen("input_file", "wb");
                if (tmp_file) {
                    size_t write_size = size > 50 ? 50 : size;
                    fwrite(data, 1, write_size, tmp_file);
                    assert_file_closed(&tmp_file);;
                    
                    curl_mime_filedata(part, "input_file");
                }
            }
            
            // Set mime data to easy handle
            curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
        }
        
        // Set URL to a file URL (using the temporary file)
        curl_easy_setopt(easy, CURLOPT_URL, "file://input_file");
        
        // Add easy handle to multi handle
        curl_multi_add_handle(multi, easy);
        
        // 4. Use curl_multi_wait - wait for activity
        int running_handles = 0;
        int ret = 0;
        curl_multi_perform(multi, &running_handles);
        
        // Wait with timeout of 1ms
        curl_multi_wait(multi, nullptr, 0, 1, &ret);
        
        // 5. Use curl_multi_wakeup - wake up multi handle
        curl_multi_wakeup(multi);
        
        // 6. Use curl_easy_getinfo - get effective URL
        char *effective_url = nullptr;
        curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &effective_url);
        if (effective_url && out_file) {
            curl_mfprintf(out_file, "Effective URL: %s\n", effective_url);
        }
        
        // 7. Use curl_easy_header - try to get a header (though file protocol won't have headers)
        struct curl_header *header = nullptr;
        CURLHcode hcode = curl_easy_header(easy, "Content-Type", 0, CURLH_HEADER, 0, &header);
        if (hcode == CURLHE_OK && header && out_file) {
            curl_mfprintf(out_file, "Got header: %s: %s\n", header->name, header->value);
        }
        
        // Clean up handles
        if (mime) {
            curl_mime_free(mime);
        }
        curl_multi_remove_handle(multi, easy);
        curl_easy_cleanup(easy);
    }
    
    // Clean up multi handle
    if (multi) {
        curl_multi_cleanup(multi);
    }
    
    // Clean up file handles
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    // Cleanup global curl state
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}
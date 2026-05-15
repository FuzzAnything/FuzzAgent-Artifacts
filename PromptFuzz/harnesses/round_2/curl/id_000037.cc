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
//<ID> 471
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 3
//<Quality> {"density":6,"unique_branches":{"setname":[[696,15,696,19,0,0,4,1],[699,6,699,12,0,0,4,1]],"Curl_getformdata":[[780,12,780,45,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_version","curl_msnprintf","curl_formadd","curl_formget","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_mime_free","curl_easy_cleanup","curl_formfree","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version","curl_msnprintf","curl_formadd","curl_formget","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_mime_free","curl_easy_cleanup","curl_formfree","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Helper function to create va_list for curl_mvfprintf
static void  id_000037_call_curl_mvfprintf(FILE* fd, const char* format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvfprintf(fd, format, args);
    va_end(args);
}

// Callback function for curl_formget
static size_t  id_000037_formget_callback(void *arg, const char *data, size_t length) {
    // Simulate processing form data - could write to file or buffer
    (void)arg; // Unused in this example
    (void)data; // Unused in this example
    return length; // Return the length we "processed"
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Initialize curl globally (required for some operations)
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create output file for curl_mvfprintf
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }
    
    // 1. Use curl_version() - get curl version string
    char *version_str = curl_version();
    
    // 2. Use curl_msnprintf() - format version string into buffer
    char buffer[512];
    int printed = curl_msnprintf(buffer, sizeof(buffer), 
                                 "Curl Version: %s\nInput size: %zu", 
                                 version_str ? version_str : "unknown", 
                                 size);
    
    // 3. Create a simple form structure for curl_formget()
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Add a form field using input data (truncated to safe length)
    size_t field_len = size > 100 ? 100 : size;
    char *field_data = (char*)malloc(field_len + 1);
    if (field_data) {
        memcpy(field_data, data, field_len);
        field_data[field_len] = '\0';
        
        // Create form with curl_formadd (needed for curl_formget)
        curl_formadd(&formpost, &lastptr,
                     CURLFORM_COPYNAME, "fuzz_field",
                     CURLFORM_COPYCONTENTS, field_data,
                     CURLFORM_END);
        
        free(field_data);
    }
    
    // 4. Use curl_formget() - process form data
    if (formpost) {
        curl_formget(formpost, NULL,  id_000037_formget_callback);
    }
    
    // 5. Create a mime part and set its type using input data
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_mime *mime = curl_mime_init(easy);
        if (mime) {
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // Use first 50 bytes (or less) as mime type if it looks like text
                size_t mime_len = size > 50 ? 50 : size;
                char *mime_type = (char*)malloc(mime_len + 1);
                if (mime_type) {
                    memcpy(mime_type, data, mime_len);
                    mime_type[mime_len] = '\0';
                    
                    // Replace non-printable characters with safe ones
                    for (size_t i = 0; i < mime_len; i++) {
                        if (mime_type[i] < 32 || mime_type[i] > 126) {
                            mime_type[i] = '_';
                        }
                    }
                    
                    // 6. Use curl_mime_type() - set mime type
                    curl_mime_type(part, mime_type);
                    free(mime_type);
                }
            }
            curl_mime_free(mime);
        }
        curl_easy_cleanup(easy);
    }
    
    // 7. Use curl_mvfprintf() - write formatted output to file
    if (printed > 0) {
         id_000037_call_curl_mvfprintf(out_file, "Formatted output: %s\n", buffer);
    }
    
    // 8. Simulate curl_pushheader_byname() usage
    // Note: We can't easily create push headers without actual HTTP/2 push,
    // but we can demonstrate the API usage pattern
    struct curl_pushheaders *push_headers = NULL; // Would be provided in real push callback
    (void)push_headers; // Mark as used to avoid warning
    
    // This would normally be called in a push callback context:
    // char *header_value = curl_pushheader_byname(push_headers, "example-header");
    
    // Cleanup
    if (formpost) {
        curl_formfree(formpost);
    }
    
    assert_file_closed(&out_file);;
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	return 0;
}
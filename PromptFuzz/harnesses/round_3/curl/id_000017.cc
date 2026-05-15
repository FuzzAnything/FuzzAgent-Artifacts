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
//<ID> 191
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["curl_version_info","curl_mprintf","curl_mprintf","curl_mprintf","curl_mprintf","curl_easy_option_by_id","curl_mprintf","curl_formadd","curl_easy_init","curl_easy_setopt","curl_easy_nextheader","curl_mprintf","curl_easy_cleanup","curl_free","curl_msnprintf","curl_mfprintf","curl_msprintf","curl_formfree"],"critical_calls":["curl_version_info","curl_mprintf","curl_mprintf","curl_mprintf","curl_mprintf","curl_easy_option_by_id","curl_mprintf","curl_formadd","curl_easy_init","curl_easy_setopt","curl_easy_nextheader","curl_mprintf","curl_easy_cleanup","curl_free","curl_msnprintf","curl_mfprintf","curl_msprintf","curl_formfree"],"visited":0}
/**/


// Include necessary curl headers
extern "C" {
}

// Custom wrapper for curl_mvaprintf that handles va_list
static char*  id_000017_call_curl_mvaprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Get curl version information
    curl_version_info_data* version_info = curl_version_info(CURLVERSION_NOW);
    if (!version_info) {
        return 0;
    }

    // 2. Use curl_mprintf to print version info
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        curl_mprintf("Version: %s\n", version_info->version);
        curl_mprintf("Protocols: ");
        for (int i = 0; version_info->protocols && version_info->protocols[i]; i++) {
            curl_mprintf("%s ", version_info->protocols[i]);
        }
        curl_mprintf("\n");
        fflush(out_file);
    }

    // 3. Get easy option by ID
    const struct curl_easyoption* easy_option = curl_easy_option_by_id(CURLOPT_URL);
    if (easy_option) {
        if (out_file) {
            curl_mprintf("Easy option name: %s\n", easy_option->name);
        }
    }

    // 4. Create form data using input bytes
    struct curl_httppost* formpost = nullptr;
    struct curl_httppost* lastptr = nullptr;
    
    // Create a null-terminated string from input data for form
    std::vector<char> input_str(data, data + size);
    input_str.push_back('\0');
    
    // Add form field using input data
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "fuzz_field",
                 CURLFORM_COPYCONTENTS, input_str.data(),
                 CURLFORM_END);

    // 5. Process headers (simulate header iteration)
    // Create a dummy easy handle for header operations
    CURL* easy = curl_easy_init();
    if (easy) {
        // Set a dummy URL to make handle valid
        curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
        
        // Try to get next header (will likely return NULL since no headers exist)
        struct curl_header* header = curl_easy_nextheader(easy, fuzz_uint32_t_1, fuzz_int32_t_2, nullptr);
        if (header) {
            // This branch won't execute in normal fuzzing, but API is called
            if (out_file) {
                curl_mprintf("Header name: %s\n", header->name);
            }
        }
        
        curl_easy_cleanup(easy);
    }

    // 6. Use curl_mvaprintf with variable arguments
    char* formatted =  id_000017_call_curl_mvaprintf("Input size: %zu, Version: %s", 
                                          size, version_info->version);
    if (formatted && out_file) {
        fwrite(formatted, strlen(formatted), 1, out_file);
        curl_free(formatted);
    }

    // 7. Additional curl_mprintf variations
    if (out_file) {
        char buffer[256];
        curl_msnprintf(buffer, sizeof(buffer), "Buffer test with size: %zu", size);
        
        // Use mfprintf to write to file
        curl_mfprintf(out_file, "Form fields: %d\n", formpost ? 1 : 0);
        
        // Use msprintf
        char buffer2[100];
        curl_msprintf(buffer2, "Test %s", "msprintf");
    }

    // Cleanup
    if (formpost) {
        curl_formfree(formpost);
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }

    assert_file_closed(&out_file);
	return 0;
}
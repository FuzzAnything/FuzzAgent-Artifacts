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
//<ID> 945
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 6
//<Quality> {"density":9,"unique_branches":{"FormAdd":[[407,5,407,33,0,0,4,0],[313,26,313,31,9,0,4,1]],"FormInfoCopyField":[[127,8,127,12,0,0,4,1]],"FormAddCheck":[[230,9,230,29,0,0,4,0],[231,9,231,42,0,0,4,1]],"Curl_getformdata":[[820,14,820,19,0,0,4,1]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_formadd","curl_maprintf","curl_mprintf","curl_formget","curl_easy_upkeep","curl_multi_add_handle","curl_ws_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_formfree","curl_free","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_formadd","curl_maprintf","curl_mprintf","curl_formget","curl_easy_upkeep","curl_multi_add_handle","curl_ws_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_formfree","curl_free","curl_global_cleanup"],"visited":0}
/**/


// System headers for curl

// Custom callback for curl_formget
static size_t  id_000051_formget_callback(void *ptr, const char *string, size_t size) {
    // Write to file if provided, otherwise just count
    FILE *out_file = static_cast<FILE*>(ptr);
    if (out_file) {
        return fwrite(string, 1, size, out_file);
    }
    return size; // Just pretend we wrote everything
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize global curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // 2. Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }

    // 3. Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }

    // 4. Create form data from input
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Create a copy of the input data for form (curl_formadd needs non-const)
    char *form_data = static_cast<char*>(malloc(size + 1));
    if (form_data) {
        memcpy(form_data, data, size);
        form_data[size] = '\0'; // Null-terminate
        
        // Create a simple form with one field
        // Note: We use deprecated API because curl_formget requires it
        curl_formadd(&formpost, &lastptr,
                    CURLFORM_COPYNAME, "fuzzfield",
                    CURLFORM_COPYCONTENTS, form_data,
                    CURLFORM_CONTENTSLENGTH, static_cast<long>(size),
                    CURLFORM_END);
    }

    // 5. Use curl_maprintf
    char *maprintf_str = curl_maprintf("Input size: %zu", size);
    
    // 6. Use curl_mprintf (prints to stdout)
    curl_mprintf("Fuzzing with %zu bytes\n", size);

    // 7. Process form data to output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file && formpost) {
        curl_formget(formpost, out_file,  id_000051_formget_callback);
    }

    // 8. Create input FILE* for possible use
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Could use the file descriptor if needed
        int fd = fuzz_fileno(in_file);
        (void)fd; // Avoid unused variable warning
        assert_file_closed(&in_file);;
    }

    // 9. Perform easy upkeep
    curl_easy_upkeep(easy);

    // 10. Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);

    // 11. Try to receive WebSocket data (will likely fail, but we call it)
    char ws_buffer[1024];
    size_t ws_recv = 0;
    const struct curl_ws_frame *ws_meta = NULL;
    if (size > 0) {
        // Use a small buffer to avoid overflows
        size_t buffer_size = size > sizeof(ws_buffer) ? sizeof(ws_buffer) : size;
        curl_ws_recv(easy, ws_buffer, buffer_size, &ws_recv, &ws_meta);
    }

    // 12. Remove handle from multi
    curl_multi_remove_handle(multi, easy);

    // 13. Cleanup multi
    curl_multi_cleanup(multi);

    // 14. Cleanup easy
    curl_easy_cleanup(easy);

    // 15. Free form data
    if (formpost) {
        curl_formfree(formpost);
    }
    
    // 16. Free allocated strings
    if (maprintf_str) {
        curl_free(maprintf_str);
    }
    
    if (form_data) {
        free(form_data);
    }
    
    // 17. Close output file
    if (out_file) {
        assert_file_closed(&out_file);;
    }

    // 18. Global cleanup
    curl_global_cleanup();

    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
}
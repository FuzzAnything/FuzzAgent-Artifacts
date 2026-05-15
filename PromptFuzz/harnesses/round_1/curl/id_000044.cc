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
//<ID> 499
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"curl_global_cleanup":[[264,6,264,19,0,0,4,0]],"global_init":[[126,6,126,19,0,0,4,0]]},"library_calls":["curl_global_init","curl_getenv","curl_url","curl_url_set","curl_url_set","curl_url_set","curl_formadd","curl_easy_init","curl_formfree","curl_url_cleanup","curl_free","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_multi_init","curl_multi_add_handle","curl_multi_socket_all","curl_easy_send","curl_easy_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_setopt","curl_easy_perform","curl_easy_strerror","curl_mime_free","curl_formfree","curl_easy_cleanup","curl_url_cleanup","curl_free","curl_global_init","curl_multi_init","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_getenv","curl_url","curl_url_set","curl_url_set","curl_url_set","curl_formadd","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_multi_init","curl_multi_add_handle","curl_multi_socket_all","curl_easy_send","curl_easy_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_setopt","curl_easy_perform","curl_easy_strerror","curl_mime_free","curl_formfree","curl_easy_cleanup","curl_url_cleanup","curl_free","curl_global_init","curl_multi_init","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified curl APIs to simulate a complex multi-part form upload scenario:

*/


extern "C" {
// Include necessary curl headers (omitted for brevity)
}

// Custom types for curl callbacks
struct FuzzData {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static size_t  id_000044_read_callback(char* buffer, size_t size, size_t nitems, void* userdata) {
    FuzzData* fuzz = static_cast<FuzzData*>(userdata);
    size_t total_size = size * nitems;
    size_t bytes_to_copy = fuzz->size - fuzz->offset;
    
    if (bytes_to_copy > total_size) {
        bytes_to_copy = total_size;
    }
    
    if (bytes_to_copy > 0) {
        memcpy(buffer, fuzz->data + fuzz->offset, bytes_to_copy);
        fuzz->offset += bytes_to_copy;
        return bytes_to_copy;
    }
    
    return 0;
}

static int  id_000044_seek_callback(void* userdata, curl_off_t offset, int origin) {
    FuzzData* fuzz = static_cast<FuzzData*>(userdata);
    
    switch (origin) {
        case SEEK_SET:
            if (offset >= 0 && offset < static_cast<curl_off_t>(fuzz->size)) {
                fuzz->offset = static_cast<size_t>(offset);
                return CURL_SEEKFUNC_OK;
            }
            break;
        case SEEK_CUR:
            if (fuzz->offset + offset >= 0 && 
                fuzz->offset + offset < fuzz->size) {
                fuzz->offset += static_cast<size_t>(offset);
                return CURL_SEEKFUNC_OK;
            }
            break;
        case SEEK_END:
            if (fuzz->size + offset >= 0 && 
                fuzz->size + offset < fuzz->size) {
                fuzz->offset = fuzz->size + static_cast<size_t>(offset);
                return CURL_SEEKFUNC_OK;
            }
            break;
    }
    
    return CURL_SEEKFUNC_FAIL;
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<78) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	
    
    // Initialize global curl environment
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    
    // Get environment variable
    char* env_var = curl_getenv(fuzz_str_1);
    
    // Create URL handle and set URL parts
    CURLU* url_handle = curl_url();
    if (url_handle) {
        curl_url_set(url_handle, CURLUPART_URL, fuzz_str_2, fuzz_uint32_t_3);
        curl_url_set(url_handle, CURLUPART_PATH, fuzz_str_4, fuzz_uint32_t_5);
        curl_url_set(url_handle, CURLUPART_QUERY, fuzz_str_6, fuzz_uint32_t_7);
    }
    
    // Create and populate curl form
    struct curl_httppost* formpost = nullptr;
    struct curl_httppost* lastptr = nullptr;
    
    // Use first 100 bytes for form data if available
    size_t form_data_size = size > 100 ? 100 : size;
    char* form_data = static_cast<char*>(malloc(form_data_size + 1));
    if (form_data) {
        memcpy(form_data, data, form_data_size);
        form_data[form_data_size] = '\0';
        
        curl_formadd(&formpost, &lastptr,
                     CURLFORM_COPYNAME, "fuzzfield",
                     CURLFORM_COPYCONTENTS, form_data,
                     CURLFORM_END);
        
        free(form_data);
    }
    
    // Initialize easy handle
    CURL* easy = curl_easy_init();
    if (!easy) {
        if (formpost) curl_formfree(formpost);
        if (url_handle) curl_url_cleanup(url_handle);
        if (env_var) curl_free(env_var);
        curl_global_cleanup();
        return 0;
    }
    
    // Create and populate mime structure
    curl_mime* mime = curl_mime_init(easy);
    if (mime) {
        curl_mimepart* part = curl_mime_addpart(mime);
        if (part) {
            // Use next 100 bytes for mime data if available
            size_t mime_offset = form_data_size;
            size_t mime_data_size = (size - mime_offset) > 100 ? 100 : (size - mime_offset);
            
            if (mime_data_size > 0) {
                char* mime_data = static_cast<char*>(malloc(mime_data_size));
                if (mime_data) {
                    memcpy(mime_data, data + mime_offset, mime_data_size);
                    curl_mime_data(part, mime_data, mime_data_size);
                    free(mime_data);
                }
                curl_mime_name(part, fuzz_str_8);
            }
        }
    }
    
    // Initialize multi handle
    CURLM* multi = curl_multi_init();
    if (multi && easy) {
        curl_multi_add_handle(multi, easy);
        
        // Try to drive the multi handle
        int running_handles = 0;
        curl_multi_socket_all(multi, &running_handles);
        
        // Try to send some data
        size_t send_offset = form_data_size + 100;
        if (send_offset < size) {
            size_t send_size = size - send_offset > 256 ? 256 : size - send_offset;
            size_t sent = 0;
            curl_easy_send(easy, data + send_offset, send_size, &sent);
        }
        
        // Try to receive some data
        char recv_buffer[256];
        size_t received = 0;
        curl_easy_recv(easy, recv_buffer, sizeof(recv_buffer), &received);
        
        // Clean up multi handle
        curl_multi_remove_handle(multi, easy);
        curl_multi_cleanup(multi);
    }
    
    // Try to perform the easy handle operation
    if (easy) {
        // Set some options
        curl_easy_setopt(easy, CURLOPT_URL, "https://example.com");
        
        // Try to perform (will likely fail without network, but that's OK for fuzzing)
        CURLcode res = curl_easy_perform(easy);
        
        // Get error string
        const char* error_str = curl_easy_strerror(res);
        (void)error_str; // Use error_str to avoid unused variable warning
    }
    
    // Clean up all resources
    if (mime) curl_mime_free(mime);
    if (formpost) curl_formfree(formpost);
    if (easy) curl_easy_cleanup(easy);
    if (url_handle) curl_url_cleanup(url_handle);
    if (env_var) curl_free(env_var);
    
    // Call curl_global_init again (as specified in API list)
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Call curl_multi_init again (as specified in API list)
    CURLM* multi2 = curl_multi_init();
    if (multi2) {
        curl_multi_cleanup(multi2);
    }
    
    curl_global_cleanup();
    
    return 0;
}
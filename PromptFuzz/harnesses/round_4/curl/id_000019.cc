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
//<ID> 180
//<Prompt> []
/*<Combination>: [
*/
//<score> 62, nr_unique_branch: 8
//<Quality> {"density":31,"unique_branches":{"progress_calc":[[462,6,462,64,0,0,4,0],[483,14,483,49,0,0,4,0],[491,6,491,22,0,0,4,1],[494,6,494,41,0,0,4,1],[504,6,504,35,0,0,4,1]],"Curl_vsetopt":[[2874,5,2874,26,0,0,4,0]],"setopt_pointers":[[1460,3,1460,24,0,0,4,0],[1476,3,1476,24,0,0,4,1]]},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_set","curl_url_get","curl_free","curl_mime_init","curl_url_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_data","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_mime_free","curl_easy_init","curl_mime_free","curl_url_cleanup","curl_global_cleanup","curl_url_get","curl_easy_setopt","curl_free","curl_easy_setopt","curl_multi_init","curl_easy_cleanup","curl_mime_free","curl_url_cleanup","curl_global_cleanup","curl_multi_notify_enable","curl_multi_add_handle","curl_formadd","curl_easy_setopt","curl_multi_socket_action","curl_multi_wait","curl_multi_remove_handle","curl_formfree","curl_multi_cleanup","curl_easy_cleanup","curl_mime_free","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_get","curl_free","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_mime_free","curl_easy_init","curl_url_get","curl_easy_setopt","curl_free","curl_easy_setopt","curl_multi_init","curl_multi_notify_enable","curl_multi_add_handle","curl_formadd","curl_easy_setopt","curl_multi_socket_action","curl_multi_wait","curl_multi_remove_handle","curl_formfree","curl_multi_cleanup","curl_easy_cleanup","curl_mime_free","curl_url_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Helper function to split data into chunks for different purposes
static size_t  id_000019_min(size_t a, size_t b) { return a < b ? a : b; }

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Initialize curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Create URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Split input data for different purposes
    size_t offset = 0;
    
    // Use first part for URL (null-terminated string)
    size_t url_len = 0;
    while (offset + url_len < size && data[offset + url_len] != 0 && url_len < 256) {
        url_len++;
    }
    char *url_str = (char*)malloc(url_len + 1);
    if (!url_str) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url_str, data + offset,  id_000019_min(url_len, size - offset));
    url_str[ id_000019_min(url_len, size - offset)] = 0;
    offset +=  id_000019_min(url_len + 1, size - offset);
    
    // Set URL
    curl_url_set(url_handle, CURLUPART_URL, url_str, fuzz_uint32_t_1);
    free(url_str);
    
    // Get URL back (just for fuzzing)
    char *retrieved_url = nullptr;
    curl_url_get(url_handle, CURLUPART_URL, &retrieved_url, fuzz_uint32_t_2);
    if (retrieved_url) {
        curl_free(retrieved_url);
    }

    // Create MIME structure
    curl_mime *mime = curl_mime_init(nullptr);
    if (!mime) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // Add first MIME part
    curl_mimepart *part1 = curl_mime_addpart(mime);
    if (part1) {
        // Use next chunk for MIME data
        size_t mime_data_len =  id_000019_min(1024, size - offset);
        if (mime_data_len > 0) {
            char *mime_data = (char*)malloc(mime_data_len);
            if (mime_data) {
                memcpy(mime_data, data + offset, mime_data_len);
                curl_mime_data(part1, mime_data, mime_data_len);
                free(mime_data);
                offset += mime_data_len;
            }
        }
    }

    // Create subparts MIME structure
    curl_mime *submime = curl_mime_init(nullptr);
    if (submime) {
        curl_mimepart *subpart = curl_mime_addpart(submime);
        if (subpart) {
            // Use next chunk for subpart data
            size_t subpart_data_len =  id_000019_min(512, size - offset);
            if (subpart_data_len > 0) {
                char *subpart_data = (char*)malloc(subpart_data_len);
                if (subpart_data) {
                    memcpy(subpart_data, data + offset, subpart_data_len);
                    curl_mime_data(subpart, subpart_data, subpart_data_len);
                    free(subpart_data);
                    offset += subpart_data_len;
                }
            }
        }
        
        // Attach subparts to main part
        if (part1) {
            curl_mime_subparts(part1, submime);
        }
        curl_mime_free(submime);
    }

    // Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_mime_free(mime);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // Set URL option
    char *final_url = nullptr;
    if (curl_url_get(url_handle, CURLUPART_URL, &final_url, fuzz_uint32_t_3) == CURLUE_OK && final_url) {
        curl_easy_setopt(easy, CURLOPT_URL, final_url);
        curl_free(final_url);
    }

    // Set MIME option
    curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);

    // Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        curl_mime_free(mime);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // Enable notifications
    curl_multi_notify_enable(multi, CURLMOPT_SOCKETFUNCTION);

    // Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);

    // Create form data
    struct curl_httppost *formpost = nullptr;
    struct curl_httppost *lastptr = nullptr;
    
    // Use remaining data for form fields
    if (offset < size) {
        size_t form_data_len =  id_000019_min(256, size - offset);
        if (form_data_len > 0) {
            char *form_field = (char*)malloc(form_data_len + 1);
            if (form_field) {
                memcpy(form_field, data + offset, form_data_len);
                form_field[form_data_len] = 0;
                curl_formadd(&formpost, &lastptr,
                           CURLFORM_COPYNAME, "fuzzfield",
                           CURLFORM_COPYCONTENTS, form_field,
                           CURLFORM_END);
                free(form_field);
                offset += form_data_len;
            }
        }
    }

    // Set form option (alternative to MIME)
    if (formpost) {
        curl_easy_setopt(easy, CURLOPT_HTTPPOST, formpost);
    }

    // Simulate socket activity with a dummy file
    FILE *in_file = fmemopen((void*)data,  id_000019_min(size, 1024), "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        
        // Simulate socket action
        int running_handles = 0;
        curl_multi_socket_action(multi, fd, fuzz_int32_t_4, &running_handles);
        
        // Wait with minimal timeout
        struct curl_waitfd wait_fd;
        wait_fd.fd = fd;
        wait_fd.events = CURL_WAIT_POLLIN;
        wait_fd.revents = 0;
        
        int ret = 0;
        curl_multi_wait(multi, &wait_fd, 1, fuzz_int32_t_5, &ret);
        
        assert_file_closed(&in_file);;
    }

    // Remove handle from multi
    curl_multi_remove_handle(multi, easy);

    // Cleanup
    if (formpost) {
        curl_formfree(formpost);
    }
    
    curl_multi_cleanup(multi);
    curl_easy_cleanup(easy);
    curl_mime_free(mime);
    curl_url_cleanup(url_handle);
    curl_global_cleanup();

    assert_file_closed(&in_file);
	return 0;
}
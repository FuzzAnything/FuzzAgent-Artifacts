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
//<ID> 943
//<Prompt> []
/*<Combination>: [
*/
//<score> 168, nr_unique_branch: 3
//<Quality> {"density":42,"unique_branches":{"curl_multi_wakeup":[[83,12,83,43,1,0,4,1]],"thrdpool_signal":[[258,9,258,61,0,0,4,1]],"thrdslot_run":[[132,8,132,62,0,0,4,1]]},"library_calls":["curl_global_init","curl_share_init","curl_global_cleanup","curl_multi_init","curl_share_cleanup","curl_global_cleanup","curl_multi_setopt","curl_easy_init","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_slist_append","curl_slist_append","curl_mime_name","curl_mime_data_cb","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_formadd","curl_multi_add_handle","curl_multi_perform","curl_multi_socket_action","curl_multi_info_read","curl_multi_remove_handle","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_cleanup","curl_formget","curl_unescape","curl_free","curl_mime_encoder","curl_share_strerror","curl_strequal","curl_free","curl_formfree","curl_slist_free_all","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_multi_init","curl_multi_setopt","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_slist_append","curl_mime_name","curl_mime_data_cb","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_formadd","curl_multi_add_handle","curl_multi_perform","curl_multi_socket_action","curl_multi_info_read","curl_multi_remove_handle","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_cleanup","curl_formget","curl_unescape","curl_free","curl_mime_encoder","curl_share_strerror","curl_strequal","curl_free","curl_formfree","curl_slist_free_all","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Helper function to create a va_list for curl_mvaprintf
static char*  id_000053_create_formatted_string(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

// Callback functions for curl_mime_data_cb
static size_t  id_000053_read_callback(char* buffer, size_t size, size_t nitems, void* arg) {
    FILE* file = (FILE*)arg;
    return fread(buffer, size, nitems, file);
}

static int  id_000053_seek_callback(void* arg, curl_off_t offset, int origin) {
    FILE* file = (FILE*)arg;
    return fseek(file, (long)offset, origin);
}

static void  id_000053_free_callback(void* arg) {
    // Nothing to free here since we'll close the FILE separately
}

// Callback for curl_formget
static size_t  id_000053_formget_callback(void* arg, const char* buf, size_t len) {
    FILE* out_file = (FILE*)arg;
    return fwrite(buf, 1, len, out_file);
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<89) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Initialize curl global
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // 2. Create a share handle
    CURLSH* share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }

    // 3. Create a multi handle
    CURLM* multi = curl_multi_init();
    if (!multi) {
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // 4. Set multi handle option
    curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, 10L);

    // 5. Create an easy handle
    CURL* easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // Create input file from data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 6. Create mime structure
    curl_mime* mime = curl_mime_init(easy);
    if (!mime) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 7. Add mime part
    curl_mimepart* part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 8. Create header list
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, fuzz_str_1);
    headers = curl_slist_append(headers, fuzz_str_2);

    // 9. Set mime name
    curl_mime_name(part, fuzz_str_3);

    // 10. Set mime data callback
    rewind(in_file);
    curl_mime_data_cb(part, (curl_off_t)size, 
                       id_000053_read_callback,  id_000053_seek_callback,  id_000053_free_callback, in_file);

    // 11. Set easy handle options
    curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(easy, CURLOPT_SHARE, share);
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com/upload");
    curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);

    // 12. Create form data
    struct curl_httppost* formpost = nullptr;
    struct curl_httppost* lastptr = nullptr;
    
    // Use first 100 bytes or less for form data
    size_t form_data_size = size > 100 ? 100 : size;
    char* form_data = (char*)malloc(form_data_size + 1);
    if (form_data) {
        memcpy(form_data, data, form_data_size);
        form_data[form_data_size] = '\0';
        
        curl_formadd(&formpost, &lastptr,
                     CURLFORM_COPYNAME, "field1",
                     CURLFORM_COPYCONTENTS, form_data,
                     CURLFORM_END);
        
        free(form_data);
    }

    // 13. Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);

    // 14. Perform multi transfers
    int still_running = 0;
    curl_multi_perform(multi, &still_running);

    // 15. Socket action (simulate socket activity)
    curl_socket_t sockfd = fuzz_fileno(in_file);
    curl_multi_socket_action(multi, sockfd, fuzz_int32_t_4, &still_running);

    // 16. Read multi info
    int msgs_in_queue = 0;
    CURLMsg* msg = curl_multi_info_read(multi, &msgs_in_queue);

    // 17. Remove easy handle from multi
    curl_multi_remove_handle(multi, easy);

    // 18. Perform easy transfer separately
    if (formpost) {
        CURL* easy2 = curl_easy_init();
        if (easy2) {
            curl_easy_setopt(easy2, CURLOPT_HTTPPOST, formpost);
            curl_easy_setopt(easy2, CURLOPT_URL, "http://example.com/form");
            curl_easy_perform(easy2);
            curl_easy_cleanup(easy2);
        }
    }

    // 19. Get form data
    if (formpost) {
        rewind(out_file);
        curl_formget(formpost, out_file,  id_000053_formget_callback);
    }

    // 20. Unescape some data
    if (size > 50) {
        char* to_unescape = (char*)malloc(51);
        if (to_unescape) {
            memcpy(to_unescape, data + size - 50, 50);
            to_unescape[50] = '\0';
            char* unescaped = curl_unescape(to_unescape, 50);
            if (unescaped) {
                curl_free(unescaped);
            }
            free(to_unescape);
        }
    }

    // 21. Set mime encoder
    curl_mime_encoder(part, fuzz_str_5);

    // 22. Get share error string (just to use the function)
    const char* share_error = curl_share_strerror(CURLSHE_OK);

    // Use curl_strequal
    if (curl_strequal(fuzz_str_6, fuzz_str_7)) {
        // Do nothing, just using the function
    }

    // Use curl_mvaprintf
    char* formatted =  id_000053_create_formatted_string("Size: %zu", size);
    if (formatted) {
        curl_free(formatted);
    }

    // Cleanup
    if (formpost) {
        curl_formfree(formpost);
    }
    
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    curl_mime_free(mime);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_share_cleanup(share);
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(sockfd);
	return 0;
}
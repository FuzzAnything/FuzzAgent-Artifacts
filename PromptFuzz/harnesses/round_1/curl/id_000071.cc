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
//<ID> 1411
//<Prompt> []
/*<Combination>: [
*/
//<score> 100, nr_unique_branch: 20
//<Quality> {"density":25,"unique_branches":{"FormAdd":[[434,5,434,23,0,0,4,0],[436,10,436,39,0,0,4,1],[458,12,458,18,0,0,4,0],[459,14,459,71,0,0,4,1],[311,26,311,31,16,0,4,1]],"FormAddCheck":[[232,9,232,42,0,0,4,0],[233,9,233,45,0,0,4,1],[244,9,244,42,0,0,4,0],[246,8,246,44,0,0,4,0],[247,39,247,70,0,0,4,1],[250,10,250,15,0,0,4,0],[252,10,252,15,0,0,4,0],[256,10,256,69,0,0,4,1],[269,8,271,44,0,0,4,1],[280,8,280,43,0,0,4,0]],"Curl_mime_contenttype":[[1643,6,1643,14,0,0,4,0],[1648,16,1648,34,0,0,4,0],[1648,16,1648,34,0,0,4,1],[1651,10,1651,22,0,0,4,0],[1651,26,1651,74,0,0,4,1]]},"library_calls":["curl_global_init","curl_getenv","curl_free","curl_url","curl_global_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_set","curl_url_set","curl_url_get","curl_free","curl_multi_init","curl_url_cleanup","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_easy_setopt","curl_formadd","curl_multi_add_handle","curl_multi_timeout","curl_multi_waitfds","curl_easy_perform","curl_easy_strerror","curl_easy_send","curl_easy_recv","curl_multi_socket_action","curl_multi_socket_all","curl_multi_remove_handle","curl_multi_cleanup","curl_formfree","curl_mime_free","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_getenv","curl_free","curl_url","curl_url_set","curl_url_set","curl_url_get","curl_free","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_easy_setopt","curl_formadd","curl_multi_add_handle","curl_multi_timeout","curl_multi_waitfds","curl_easy_perform","curl_easy_strerror","curl_easy_send","curl_easy_recv","curl_multi_socket_action","curl_multi_socket_all","curl_multi_remove_handle","curl_multi_cleanup","curl_formfree","curl_mime_free","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* f_data, size_t f_size) {
	if(f_size<39) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1) {
	return 0;
	}
	
    
    // 1. Initialize curl global environment
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	
    
    // 2. Get environment variable
    char *env_var = curl_getenv(fuzz_str_1);
    if (env_var) {
        curl_free(env_var);
    }
    
    // 3. Create URL handle and set URL components
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Use part of input data as URL components
    size_t url_part_size = size > 100 ? 100 : size;
    char *url_part = (char*)malloc(url_part_size + 1);
    if (!url_part) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    memcpy(url_part, data, url_part_size);
    url_part[url_part_size] = '\0';
    
    curl_url_set(url_handle, CURLUPART_SCHEME, fuzz_str_2, fuzz_uint32_t_3);
    curl_url_set(url_handle, CURLUPART_HOST, url_part, fuzz_uint32_t_4);
    
    // Get URL back
    char *full_url = NULL;
    curl_url_get(url_handle, CURLUPART_URL, &full_url, fuzz_uint32_t_5);
    if (full_url) {
        curl_free(full_url);
    }
    
    free(url_part);
    
    // 4. Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 5. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 6. Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost:8080");
    
    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Create and setup mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use part of input data for mime
            size_t mime_data_size = size > 50 ? 50 : size;
            char *mime_data = (char*)malloc(mime_data_size + 1);
            if (mime_data) {
                memcpy(mime_data, data, mime_data_size);
                mime_data[mime_data_size] = '\0';
                curl_mime_data(part, mime_data, mime_data_size);
                curl_mime_name(part, fuzz_str_6);
                free(mime_data);
            }
        }
        
        curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    }
    
    // 8. Create form data
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    curl_formadd(&formpost, &lastptr,
                CURLFORM_COPYNAME, "fuzzfile",
                CURLFORM_FILE, "input_file",
                CURLFORM_END);
    
    // 9. Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 10. Setup socket for multi handle
    curl_socket_t sockfd = (curl_socket_t)in_fd;
    int running_handles = 0;
    
    // 11. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // 12. Get multi timeout
    long timeout = 0;
    curl_multi_timeout(multi_handle, &timeout);
    
    // 13. Setup wait fds
    struct curl_waitfd ufds[2];
    unsigned int fd_count = 0;
    ufds[0].fd = sockfd;
    ufds[0].events = CURL_WAIT_POLLIN;
    ufds[1].fd = out_fd;
    ufds[1].events = CURL_WAIT_POLLOUT;
    
    curl_multi_waitfds(multi_handle, ufds, 2, &fd_count);
    
    // 14. Try to perform easy handle
    CURLcode perform_res = curl_easy_perform(easy_handle);
    const char *error_str = curl_easy_strerror(perform_res);
    
    // 15. Try send/recv operations
    char buffer[1024];
    size_t bytes_sent = 0, bytes_recv = 0;
    
    if (size > 0) {
        // Try to send data
        curl_easy_send(easy_handle, data, size > 1024 ? 1024 : size, &bytes_sent);
        
        // Try to receive data
        curl_easy_recv(easy_handle, buffer, sizeof(buffer), &bytes_recv);
    }
    
    // 16. Socket operations with multi handle
    curl_multi_socket(multi_handle, sockfd, &running_handles);
    curl_multi_socket_all(multi_handle, &running_handles);
    
    // 17. Cleanup multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);
    
    // 18. Cleanup form
    curl_formfree(formpost);
    
    // 19. Cleanup mime
    if (mime) {
        curl_mime_free(mime);
    }
    
    // 20. Cleanup easy handle
    curl_easy_cleanup(easy_handle);
    
    // 21. Cleanup URL handle
    curl_url_cleanup(url_handle);
    
    // 22. Cleanup files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 23. Cleanup global environment
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
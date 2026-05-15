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
//<ID> 582
//<Prompt> []
/*<Combination>: [
*/
//<score> 49.5, nr_unique_branch: 4
//<Quality> {"density":33,"unique_branches":{"curl_url_set":[[1833,8,1833,14,0,0,4,0]],"set_url_scheme":[[1642,33,1642,43,0,0,4,0],[1647,46,1647,48,0,0,4,0],[1647,52,1647,59,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_trace","curl_global_sslset","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_easy_escape","curl_free","curl_easy_setopt","curl_slist_append","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_addpart","curl_mime_subparts","curl_mime_name","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_socket_all","curl_easy_perform","curl_easy_header","curl_url","curl_url_set","curl_url_strerror","curl_share_init","curl_share_cleanup","curl_slist_free_all","curl_mime_free","curl_url_cleanup","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_global_sslset","curl_easy_init","curl_multi_init","curl_multi_add_handle","curl_easy_escape","curl_free","curl_easy_setopt","curl_slist_append","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_addpart","curl_mime_subparts","curl_mime_name","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_socket_all","curl_easy_perform","curl_easy_header","curl_url","curl_url_set","curl_url_strerror","curl_share_init","curl_share_cleanup","curl_slist_free_all","curl_mime_free","curl_url_cleanup","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


// Include curl headers

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // 1. Initialize curl global state
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
	return 0;
	}
	

    // 2. Set trace configuration using input data
    // Use first 100 bytes or less as trace config string
    size_t trace_len = size > 100 ? 100 : size;
    char* trace_config = (char*)malloc(trace_len + 1);
    if (!trace_config) {
        curl_global_cleanup();
        return 0;
    }
    memcpy(trace_config, data, trace_len);
    trace_config[trace_len] = '\0';
    CURLcode trace_res = curl_global_trace(trace_config);
    free(trace_config);

    // 3. Try to set SSL backend (just call it, ignore result)
    curl_sslbackend dummy_backend = CURLSSLBACKEND_NONE;
    const curl_ssl_backend*** avail = nullptr;
    curl_global_sslset(dummy_backend, nullptr, avail);

    // 4. Create easy handle
    CURL* easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }

    // 5. Create multi handle
    CURLM* multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }

    // 6. Add easy handle to multi handle
    CURLMcode add_res = curl_multi_add_handle(multi, easy);
    if (add_res != CURLM_OK) {
        curl_multi_cleanup(multi);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }

    // 7. Create FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        curl_multi_remove_handle(multi, easy);
        curl_multi_cleanup(multi);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // 8. Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);

    // 9. Create escaped string from input data
    // Use middle portion for escaping (avoiding boundaries)
    size_t escape_len = size > 50 ? 50 : size;
    size_t escape_offset = size > 100 ? 50 : 0;
    char* string_to_escape = (char*)malloc(escape_len + 1);
    if (string_to_escape) {
        memcpy(string_to_escape, data + escape_offset, escape_len);
        string_to_escape[escape_len] = '\0';
        char* escaped = curl_easy_escape(easy, string_to_escape, escape_len);
        free(string_to_escape);
        if (escaped) {
            curl_free(escaped);
        }
    }

    // 10. Set various options on easy handle
    // Use parts of input data for URL, headers, etc.
    size_t url_len = size > 30 ? 30 : size;
    char* url_buf = (char*)malloc(url_len + 1);
    if (url_buf) {
        memcpy(url_buf, data, url_len);
        url_buf[url_len] = '\0';
        curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
        free(url_buf);
    }

    // 11. Create and populate header list using input data
    struct curl_slist* headers = nullptr;
    for (size_t i = 0; i < size && i < 10; i += 3) {
        char header_buf[50];
        snprintf(header_buf, sizeof(header_buf), "X-Custom-%zu: %u", 
                 i, static_cast<unsigned int>(data[i]));
        headers = curl_slist_append(headers, header_buf);
    }

    // 12. Create MIME structure
    curl_mime* mime = curl_mime_init(easy);
    if (mime) {
        // Add first MIME part with input data
        curl_mimepart* part1 = curl_mime_addpart(mime);
        if (part1) {
            // Use first half of data for MIME data
            size_t mime_data_len = size / 2;
            if (mime_data_len > 0) {
                char* mime_data = (char*)malloc(mime_data_len);
                if (mime_data) {
                    memcpy(mime_data, data, mime_data_len);
                    curl_mime_data(part1, mime_data, mime_data_len);
                    free(mime_data);
                }
            }
            curl_mime_name(part1, "data_part");
        }

        // Add second MIME part as subpart
        curl_mime* sub_mime = curl_mime_init(easy);
        if (sub_mime) {
            curl_mimepart* subpart = curl_mime_addpart(sub_mime);
            if (subpart) {
                // Use second half of data for subpart
                size_t sub_data_len = size - size/2;
                if (sub_data_len > 0 && size/2 < size) {
                    char* sub_data = (char*)malloc(sub_data_len);
                    if (sub_data) {
                        memcpy(sub_data, data + size/2, sub_data_len);
                        curl_mime_data(subpart, sub_data, sub_data_len);
                        free(sub_data);
                    }
                }
                curl_mime_name(subpart, "subpart");
            }

            // Add subparts to first part
            curl_mimepart* part2 = curl_mime_addpart(mime);
            if (part2) {
                curl_mime_subparts(part2, sub_mime);
                curl_mime_name(part2, "nested");
            }
        }

        // Set MIME data for POST
        curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    }

    // 13. Set headers if any were created
    if (headers) {
        curl_easy_setopt(easy, CURLOPT_HTTPHEADER, headers);
    }

    // 14. Set CONNECT_ONLY to avoid actual network transfer
    curl_easy_setopt(easy, CURLOPT_CONNECT_ONLY, 1L);

    // 15. Try to perform with multi handle
    int running_handles = 0;
    CURLMcode multi_perform_res = curl_multi_socket_all(multi, &running_handles);

    // 16. Also try easy perform (will fail due to CONNECT_ONLY but we call it)
    CURLcode easy_perform_res = curl_easy_perform(easy);

    // 17. Try to get headers (will likely fail but we call the API)
    struct curl_header* header_out = nullptr;
    CURLHcode header_res = curl_easy_header(easy, "Content-Type", 0, 
                                           CURLH_HEADER, 1, &header_out);

    // 18. Create URL handle and use some input data for it
    CURLU* url_handle = curl_url();
    if (url_handle) {
        size_t scheme_len = size > 10 ? 10 : size;
        char* scheme_buf = (char*)malloc(scheme_len + 1);
        if (scheme_buf) {
            memcpy(scheme_buf, data, scheme_len);
            scheme_buf[scheme_len] = '\0';
            curl_url_set(url_handle, CURLUPART_SCHEME, scheme_buf, 0);
            free(scheme_buf);
        }
        
        // Get error string for testing (won't have error but we call it)
        const char* url_error = curl_url_strerror(CURLUE_OK);
    }

    // 19. Create and cleanup share handle (for completeness)
    CURLSH* share = curl_share_init();
    if (share) {
        curl_share_cleanup(share);
    }

    // 20. Cleanup everything
    assert_file_closed(&in_file);;
    
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }
    
    curl_multi_remove_handle(multi, easy);
    curl_multi_cleanup(multi);
    curl_easy_cleanup(easy);
    curl_global_cleanup();

    assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}
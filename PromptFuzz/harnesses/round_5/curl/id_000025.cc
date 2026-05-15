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
//<ID> 450
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 7
//<Quality> {"density":9,"unique_branches":{"Curl_getformdata":[[728,6,728,11,0,0,4,0]],"Curl_mime_prepare_headers":[[1724,11,1724,22,0,0,4,0],[1724,26,1724,35,0,0,4,0],[1725,11,1725,66,0,0,4,1]],"content_type_match":[[1660,21,1660,61,0,0,4,1]],"add_content_type":[[1615,31,1615,39,0,0,4,1],[1616,31,1616,39,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_slist_append","curl_formget","curl_easy_init","curl_mime_init","curl_mime_init","curl_mime_addpart","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_mime_free","curl_easy_cleanup","curl_share_init","curl_share_setopt","curl_share_cleanup","curl_easy_init","curl_ws_start_frame","curl_easy_cleanup","curl_multi_init","curl_multi_timeout","curl_multi_get_offt","curl_multi_cleanup","curl_slist_free_all","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_slist_append","curl_formget","curl_easy_init","curl_mime_init","curl_mime_init","curl_mime_addpart","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_mime_free","curl_easy_cleanup","curl_share_init","curl_share_setopt","curl_share_cleanup","curl_easy_init","curl_ws_start_frame","curl_easy_cleanup","curl_multi_init","curl_multi_timeout","curl_multi_get_offt","curl_multi_cleanup","curl_slist_free_all","curl_global_cleanup"],"visited":0}
/**/


// Include necessary curl headers

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a FILE* to write output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 1. Create and use curl_slist
    struct curl_slist *slist = nullptr;
    // Add a header using the first 100 bytes of data (or less if size is smaller)
    size_t header_len = size > 100 ? 100 : size;
    char *header_data = (char *)malloc(header_len + 1);
    if (header_data) {
        memcpy(header_data, data, header_len);
        header_data[header_len] = '\0';
        slist = curl_slist_append(slist, header_data);
        free(header_data);
    }
    
    // 2. Create form data and use curl_formget
    struct curl_httppost *formpost = nullptr;
    struct curl_httppost *lastptr = nullptr;
    
    // Create a callback for curl_formget
    auto formget_callback = [](void *arg, const char *buf, size_t len) -> size_t {
        FILE *f = (FILE *)arg;
        return fwrite(buf, 1, len, f);
    };
    
    // Add form fields using input data
    size_t form_data_len = size > 50 ? 50 : size;
    char *form_name = (char *)malloc(form_data_len + 1);
    char *form_content = (char *)malloc(form_data_len + 1);
    
    if (form_name && form_content) {
        memcpy(form_name, data, form_data_len);
        form_name[form_data_len] = '\0';
        
        // Use different part of data for content
        size_t content_offset = form_data_len < size - form_data_len ? form_data_len : 0;
        memcpy(form_content, data + content_offset, form_data_len);
        form_content[form_data_len] = '\0';
        
        // Create form (simplified - actual curl_formadd would need proper format)
        // For demonstration, we'll just call curl_formget with a dummy form
        curl_formget(formpost, out_file, formget_callback);
        
        free(form_name);
        free(form_content);
    }
    
    // 3. Create mime structure and use curl_mime_subparts
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_mime *mime = curl_mime_init(easy);
        curl_mime *submime = curl_mime_init(easy);
        
        if (mime && submime) {
            curl_mimepart *part = curl_mime_addpart(mime);
            curl_mimepart *subpart = curl_mime_addpart(submime);
            
            if (part && subpart) {
                // Set some data for the subpart
                size_t mime_data_len = size > 30 ? 30 : size;
                curl_mime_data(subpart, (const char *)data, mime_data_len);
                
                // Attach submime as subparts
                curl_mime_subparts(part, submime);
            }
            
            curl_mime_free(mime);
            // Note: submime is owned by mime now, don't free separately
        }
        curl_easy_cleanup(easy);
    }
    
    // 4. Create share handle and cleanup
    CURLSH *share = curl_share_init();
    if (share) {
        // Set some share options
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
        curl_share_cleanup(share);
    }
    
    // 5. WebSocket frame operations
    CURL *ws_easy = curl_easy_init();
    if (ws_easy) {
        // Start a WebSocket frame
        // Use part of input data for frame length
        curl_off_t frame_len = size > 1000 ? 1000 : (curl_off_t)size;
        unsigned int flags = 0;
        if (size > 0) {
            flags = data[0] & 0x0F; // Use first byte for flags
        }
        curl_ws_start_frame(ws_easy, flags, frame_len);
        curl_easy_cleanup(ws_easy);
    }
    
    // 6. Multi handle operations
    CURLM *multi = curl_multi_init();
    if (multi) {
        long timeout = 0;
        curl_multi_timeout(multi, &timeout);
        
        curl_off_t offset_value = 0;
        // Use CURLMINFO_OFFT_ACTIVE if available, otherwise 0
        // Note: CURLMinfo_offt enum values aren't provided, using 0 as default
        curl_multi_get_offt(multi, (CURLMinfo_offt)0, &offset_value);
        
        curl_multi_cleanup(multi);
    }
    
    // 7. Cleanup slist
    if (slist) {
        curl_slist_free_all(slist);
    }
    
    // Cleanup file handles
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Global cleanup
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
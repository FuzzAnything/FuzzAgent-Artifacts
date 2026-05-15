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
//<ID> 907
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 2
//<Quality> {"density":15,"unique_branches":{"getinfo_double":[[512,3,512,10,0,0,4,0],[538,3,538,30,0,0,4,0]]},"library_calls":["curl_global_init","curl_share_init","curl_share_setopt","curl_share_strerror","curl_easy_init","curl_easy_setopt","curl_easy_strerror","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_mime_data","curl_mime_filedata","curl_easy_setopt","curl_multi_init","curl_multi_notify_disable","curl_multi_strerror","curl_ws_meta","curl_msnprintf","curl_easy_header","curl_easy_unescape","curl_free","curl_easy_ssls_export","curl_easy_getinfo","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_share_setopt","curl_share_strerror","curl_easy_init","curl_easy_setopt","curl_easy_strerror","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_mime_data","curl_mime_filedata","curl_easy_setopt","curl_multi_init","curl_multi_notify_disable","curl_multi_strerror","curl_ws_meta","curl_msnprintf","curl_easy_header","curl_easy_unescape","curl_free","curl_easy_ssls_export","curl_easy_getinfo","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Initialize share handle
    CURLSH *share = curl_share_init();
    if (share) {
        // Set share options (using error as example option)
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
        // Get share error string (should be CURLSHE_OK if successful)
        const char *share_err = curl_share_strerror(CURLSHE_OK);
        (void)share_err; // Use variable to avoid unused warning
    }
    
    // Initialize easy handle
    CURL *easy = curl_easy_init();
    if (easy) {
        // Set some easy options using input data
        if (size > 0) {
            // Create a null-terminated string from input data for URL
            char *url = (char *)malloc(size + 1);
            if (url) {
                memcpy(url, data, size);
                url[size] = '\0';
                curl_easy_setopt(easy, CURLOPT_URL, url);
                free(url);
            }
        }
        
        // Get error string for a known error code
        const char *easy_err = curl_easy_strerror(CURLE_OK);
        (void)easy_err; // Use variable
        
        // Initialize mime structure
        curl_mime *mime = curl_mime_init(easy);
        if (mime) {
            // Add a mime part
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // Set filename from input data if we have enough
                if (size > 10) {
                    char filename[11];
                    memcpy(filename, data, 10);
                    filename[10] = '\0';
                    curl_mime_filename(part, filename);
                }
                
                // Set mime data from input
                if (size > 0) {
                    curl_mime_data(part, (const char *)data, size);
                }
                
                // Also set filedata option
                curl_mime_filedata(part, "input_file");
            }
            
            // Set mime as POSTFIELDS
            curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
        }
        
        // Initialize multi handle
        CURLM *multi = curl_multi_init();
        if (multi) {
            // Disable notifications
            curl_multi_notify_disable(multi, 0);
            
            // Get multi error string
            const char *multi_err = curl_multi_strerror(CURLM_OK);
            (void)multi_err;
            
            // Get WebSocket metadata (will be NULL unless WS is active)
            const struct curl_ws_frame *ws_meta = curl_ws_meta(easy);
            (void)ws_meta;
            
            // Format a string using input data
            char buffer[256];
            if (size > 0) {
                size_t copy_len = size > 255 ? 255 : size;
                char format_str[256];
                memcpy(format_str, data, copy_len);
                format_str[copy_len] = '\0';
                curl_msnprintf(buffer, sizeof(buffer), "Data: %s", format_str);
            }
            
            // Get header information
            struct curl_header *header = NULL;
            CURLHcode hcode = curl_easy_header(easy, "Content-Type", 0, 
                                              CURLH_HEADER, 0, &header);
            (void)hcode;
            (void)header;
            
            // Unescape some data if we have enough input
            if (size > 20) {
                int outlength = 0;
                char *unescaped = curl_easy_unescape(easy, (const char *)data, 
                                                    20, &outlength);
                if (unescaped) {
                    curl_free(unescaped);
                }
            }
            
            // Try to export SSL session data (will likely fail without SSL)
            CURLcode export_res = curl_easy_ssls_export(easy, NULL, NULL);
            (void)export_res;
            
            // Get some easy handle info
            double dsize;
            CURLcode info_res = curl_easy_getinfo(easy, CURLINFO_SIZE_DOWNLOAD, &dsize);
            (void)info_res;
            (void)dsize;
            
            // Clean up multi handle
            curl_multi_cleanup(multi);
        }
        
        // Free mime structure
        if (mime) {
            curl_mime_free(mime);
        }
        
        // Clean up easy handle
        curl_easy_cleanup(easy);
    }
    
    // Clean up share handle
    if (share) {
        curl_share_cleanup(share);
    }
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}
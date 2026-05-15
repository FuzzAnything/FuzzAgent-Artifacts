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
//<ID> 1141
//<Prompt> []
/*<Combination>: [
*/
//<score> 171, nr_unique_branch: 31
//<Quality> {"density":19,"unique_branches":{"Curl_multi_pollset":[[327,4,327,40,12,0,4,1],[319,14,319,33,18,0,4,0],[320,5,320,24,18,0,4,0]],"trc_write":[[51,6,51,23,0,0,4,0],[52,8,52,24,0,0,4,1],[63,7,63,25,0,0,4,0],[64,7,64,31,0,0,4,1],[65,7,65,30,0,0,4,1],[69,7,69,14,0,0,4,1]],"trc_infof":[[251,6,251,10,0,0,4,1],[253,6,253,12,0,0,4,1],[90,3,90,51,1,0,4,1],[319,4,319,10,9,0,4,0],[319,14,319,33,9,0,4,0],[320,5,320,24,9,0,4,0]],"trc_end_buf":[[113,6,113,39,0,0,4,1],[113,24,113,29,0,0,4,0],[120,11,120,16,0,0,4,0]],"multi_warn_debug":[[319,14,319,33,4,0,4,0],[320,5,320,24,4,0,4,0],[319,14,319,33,5,0,4,0],[320,5,320,24,5,0,4,0]],"Curl_infof":[[319,4,319,10,2,0,4,0],[319,14,319,33,2,0,4,0],[320,5,320,24,2,0,4,0]],"curl_multi_remove_handle":[[327,4,327,40,21,0,4,1],[319,14,319,33,22,0,4,0],[320,5,320,24,22,0,4,0]],"curl_multi_add_handle":[[327,4,327,40,16,0,4,1],[319,14,319,33,17,0,4,0],[320,5,320,24,17,0,4,0]]},"library_calls":["curl_global_init","curl_share_init","curl_global_cleanup","curl_multi_init","curl_share_cleanup","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_filename","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_ssls_export","curl_multi_add_handle","curl_multi_waitfds","curl_easy_getinfo","curl_easy_strerror","curl_share_strerror","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_ssls_export","curl_multi_add_handle","curl_multi_waitfds","curl_easy_getinfo","curl_easy_strerror","curl_share_strerror","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Curl headers would be included here
extern "C" {
}

// Custom callback function for curl_easy_ssls_export
static CURLcode  id_000060_ssls_export_callback(CURL *handle, void *userptr,
                                     const char *session_key,
                                     const unsigned char *shmac,
                                     size_t shmac_len,
                                     const unsigned char *sdata,
                                     size_t sdata_len,
                                     curl_off_t ssl_version,
                                     int cipher,
                                     const char *hostname,
                                     size_t hostname_len) {
    // Simple callback that does nothing
    (void)handle; (void)userptr; (void)session_key; (void)shmac;
    (void)shmac_len; (void)sdata; (void)sdata_len; (void)ssl_version;
    (void)cipher; (void)hostname; (void)hostname_len;
    return CURLE_OK;
}

// Variadic wrapper for curl_mvfprintf to use with fuzzer input
static int  id_000060_custom_mvfprintf(FILE *fd, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvfprintf(fd, format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize global curl environment
    CURLcode global_init_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_result != CURLE_OK) {
        return 0;
    }
    
    // Create share handle for sharing data between easy handles
    CURLSH *share_handle = curl_share_init();
    if (!share_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create a multi handle for managing multiple transfers
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_share_cleanup(share_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create an easy handle for a single transfer
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create a mime structure for multipart form data
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create a mime part and set filename from fuzzer input
    curl_mimepart *mimepart = curl_mime_addpart(mime);
    if (mimepart && size > 0) {
        // Use fuzzer data as filename (ensure null termination for safety)
        char *filename = (char*)malloc(size + 1);
        if (filename) {
            memcpy(filename, data, size);
            filename[size] = '\0';
            curl_mime_filename(mimepart, filename);
            free(filename);
        }
    }
    
    // Set up waitfd structures for curl_multi_waitfds
    struct curl_waitfd *ufds = (struct curl_waitfd*)malloc(sizeof(struct curl_waitfd) * 2);
    unsigned int fd_count = 0;
    
    // Create FILE* for reading fuzzer input
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Set up first waitfd for input file
        ufds[0].fd = fuzz_fileno(in_file);
        ufds[0].events = CURL_WAIT_POLLIN;
        ufds[0].revents = 0;
        
        // Create output file for curl_mvfprintf
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            // Set up second waitfd for output file
            ufds[1].fd = fuzz_fileno(out_file);
            ufds[1].events = CURL_WAIT_POLLOUT;
            ufds[1].revents = 0;
            
            // Set easy handle options
            curl_easy_setopt(easy_handle, CURLOPT_SHARE, share_handle);
            curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
            curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 1L);
            
            // Try to export SSL session data
            curl_easy_ssls_export(easy_handle,  id_000060_ssls_export_callback, NULL);
            
            // Add easy handle to multi handle
            curl_multi_add_handle(multi_handle, easy_handle);
            
            // Perform multi wait on file descriptors
            curl_multi_waitfds(multi_handle, ufds, 2, &fd_count);
            
            // Try to get information from easy handle
            char *effective_url = NULL;
            curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &effective_url);
            
            // Use curl_mvfprintf to write to output file
            if (size > 0) {
                 id_000060_custom_mvfprintf(out_file, "Fuzzer input size: %zu\n", size);
                 id_000060_custom_mvfprintf(out_file, "FD count: %u\n", fd_count);
                
                // Get and print error strings (even if no error occurred)
                const char *easy_error = curl_easy_strerror(CURLE_OK);
                const char *share_error = curl_share_strerror(CURLSHE_OK);
                
                 id_000060_custom_mvfprintf(out_file, "Easy error: %s\n", easy_error);
                 id_000060_custom_mvfprintf(out_file, "Share error: %s\n", share_error);
            }
            
            // Clean up output file
            assert_file_closed(&out_file);;
        }
        
        // Clean up input file
        assert_file_closed(&in_file);;
    }
    
    // Clean up allocated memory
    if (ufds) {
        free(ufds);
    }
    
    // Clean up mime structure
    if (mime) {
        curl_mime_free(mime);
    }
    
    // Remove easy handle from multi handle
    if (easy_handle && multi_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    
    // Clean up handles in reverse order
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    if (share_handle) {
        curl_share_cleanup(share_handle);
    }
    
    // Clean up global curl environment
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
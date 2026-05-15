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
//<ID> 34
//<Prompt> []
/*<Combination>: [
*/
//<score> 105, nr_unique_branch: 7
//<Quality> {"density":35,"unique_branches":{"Curl_share_lock":[[378,6,378,50,0,0,4,1]],"curl_share_setopt":[[204,6,204,25,0,0,4,0],[245,5,245,36,0,0,4,0],[277,8,277,12,0,0,4,1]],"dnscache_entry_is_stale":[[117,8,117,30,0,0,4,0]],"Curl_share_easy_link":[[447,8,447,22,0,0,4,1]],"Curl_share_unlock":[[394,6,394,50,0,0,4,1]]},"library_calls":["curl_global_init","curl_share_init","curl_global_cleanup","curl_share_setopt","curl_easy_init","curl_share_cleanup","curl_global_cleanup","curl_easy_setopt","curl_mime_init","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_type","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_type","curl_mime_subparts","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_mime_free","curl_mime_free","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_easy_ssls_import","curl_multi_poll","curl_share_setopt","curl_share_strerror","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_mime_free","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_share_setopt","curl_easy_init","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_type","curl_mime_subparts","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_easy_ssls_import","curl_multi_poll","curl_share_setopt","curl_share_strerror","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_mime_free","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":1}
/**/


// Include necessary headers for curl

// Custom types that might be needed
struct mime_data_context {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// Callback functions for curl_mime_data_cb
static size_t  id_000009_read_callback(char* buffer, size_t size, size_t nitems, void* arg) {
    mime_data_context* ctx = static_cast<mime_data_context*>(arg);
    size_t total_size = size * nitems;
    size_t available = ctx->size - ctx->pos;
    
    if (available == 0) {
        return 0; // EOF
    }
    
    if (total_size > available) {
        total_size = available;
    }
    
    memcpy(buffer, ctx->data + ctx->pos, total_size);
    ctx->pos += total_size;
    return total_size;
}

static int  id_000009_seek_callback(void* arg, curl_off_t offset, int origin) {
    mime_data_context* ctx = static_cast<mime_data_context*>(arg);
    
    switch (origin) {
        case SEEK_SET:
            if (offset < 0 || static_cast<size_t>(offset) > ctx->size) {
                return CURL_SEEKFUNC_FAIL;
            }
            ctx->pos = offset;
            break;
        case SEEK_CUR:
            if (ctx->pos + offset < 0 || 
                static_cast<size_t>(ctx->pos + offset) > ctx->size) {
                return CURL_SEEKFUNC_FAIL;
            }
            ctx->pos += offset;
            break;
        case SEEK_END:
            if (offset > 0 || static_cast<size_t>(-offset) > ctx->size) {
                return CURL_SEEKFUNC_FAIL;
            }
            ctx->pos = ctx->size + offset;
            break;
        default:
            return CURL_SEEKFUNC_FAIL;
    }
    
    return CURL_SEEKFUNC_OK;
}

static void  id_000009_free_callback(void* arg) {
    mime_data_context* ctx = static_cast<mime_data_context*>(arg);
    free(ctx);
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create share object for SSL session sharing
    CURLSH* share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set share options - using SSL session sharing
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
    
    // Create easy handle
    CURL* easy = curl_easy_init();
    if (!easy) {
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Set share object for the easy handle
    curl_easy_setopt(easy, CURLOPT_SHARE, share);
    
    // Create MIME structure
    curl_mime* mime = curl_mime_init(easy);
    if (!mime) {
        curl_easy_cleanup(easy);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Create main MIME part
    curl_mimepart* part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Set MIME type for the part
    curl_mime_type(part, "multipart/mixed");
    
    // Create subparts MIME structure
    curl_mime* subparts = curl_mime_init(easy);
    if (subparts) {
        // Create a subpart
        curl_mimepart* subpart = curl_mime_addpart(subparts);
        if (subpart) {
            // Allocate context for data callback
            mime_data_context* ctx = static_cast<mime_data_context*>(
                malloc(sizeof(mime_data_context)));
            if (ctx) {
                ctx->data = data;
                ctx->size = size;
                ctx->pos = 0;
                
                // Set data callback for the subpart
                curl_mime_data_cb(subpart, size,
                                  id_000009_read_callback,
                                  id_000009_seek_callback,
                                  id_000009_free_callback,
                                 ctx);
            }
            
            // Set MIME type for subpart
            curl_mime_type(subpart, "application/octet-stream");
        }
        
        // Attach subparts to main part
        curl_mime_subparts(part, subparts);
    }
    
    // Set MIME data for easy handle
    curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    
    // Set a dummy URL (required for multi interface)
    curl_easy_setopt(easy, CURLOPT_URL, "http://localhost/test");
    
    // Create multi handle
    CURLM* multi = curl_multi_init();
    if (!multi) {
        if (subparts) curl_mime_free(subparts);
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    // Try to import SSL session data from input
    if (size >= 64) { // Need enough data for session import
        const char* session_key = "test_session";
        const unsigned char* shmac = data;
        size_t shmac_len = size > 32 ? 32 : size;
        const unsigned char* sdata = data + shmac_len;
        size_t sdata_len = size - shmac_len > 100 ? 100 : size - shmac_len;
        
        curl_easy_ssls_import(easy, session_key, shmac, shmac_len, sdata, sdata_len);
    }
    
    // Setup poll structure
    struct curl_waitfd extra_fds[1];
    extra_fds[0].fd = 0; // STDIN
    extra_fds[0].events = CURL_WAIT_POLLIN;
    extra_fds[0].revents = 0;
    
    int ret = 0;
    
    // Poll for activity (with minimal timeout since we're not actually connecting)
    curl_multi_poll(multi, extra_fds, 1, 1, &ret);
    
    // Get and use share error string (for demonstration)
    CURLSHcode share_err = curl_share_setopt(share, CURLSHOPT_UNSHARE, CURL_LOCK_DATA_SSL_SESSION);
    const char* err_str = curl_share_strerror(share_err);
    
    // Clean up multi handle
    int running_handles = 0;
    curl_multi_perform(multi, &running_handles);
    
    // Remove easy handle from multi
    curl_multi_remove_handle(multi, easy);
    
    // Clean up multi
    curl_multi_cleanup(multi);
    
    // Clean up MIME structures
    if (subparts) {
        curl_mime_free(subparts);
    }
    curl_mime_free(mime);
    
    // Clean up easy handle
    curl_easy_cleanup(easy);
    
    // Clean up share
    curl_share_cleanup(share);
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}
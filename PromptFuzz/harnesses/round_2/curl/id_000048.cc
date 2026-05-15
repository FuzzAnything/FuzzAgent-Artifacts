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
//<ID> 869
//<Prompt> []
/*<Combination>: [
*/
//<score> 473, nr_unique_branch: 12
//<Quality> {"density":43,"unique_branches":{"multi_addmsg":[[223,6,223,40,0,0,4,1]],"curl_share_setopt":[[204,6,204,25,0,0,4,0]],"dnscache_get":[[146,21,146,71,0,0,4,0]],"multi_runsingle":[[2499,8,2499,36,0,0,4,0],[319,4,319,10,57,0,4,0],[319,14,319,33,57,0,4,1]],"dnscache_unlock":[[163,21,163,55,0,0,4,0]],"dnscache_lock":[[156,21,156,55,0,0,4,0]],"handle_completed":[[2451,6,2451,25,0,0,4,1]],"multi_run_dirty":[[3140,13,3140,60,0,0,4,0]],"curl_share_strerror":[[395,3,395,22,0,0,4,0]],"curl_multi_remove_handle":[[890,8,890,39,0,0,4,1]]},"library_calls":["curl_global_init","curl_easy_strerror","curl_global_cleanup","curl_global_cleanup","curl_share_init","curl_share_setopt","curl_share_setopt","curl_easy_init","curl_easy_init","curl_easy_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_type","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_add_handle","curl_multi_notify_disable","curl_multi_assign","curl_multi_socket_action","curl_multi_wait","curl_easy_recv","curl_multi_remove_handle","curl_multi_remove_handle","curl_easy_escape","curl_easy_unescape","curl_free","curl_free","curl_easy_ssls_export","curl_easy_strerror","curl_share_setopt","curl_share_strerror","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_share_setopt","curl_share_setopt","curl_easy_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_type","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_add_handle","curl_multi_notify_disable","curl_multi_assign","curl_multi_socket_action","curl_multi_wait","curl_easy_recv","curl_multi_remove_handle","curl_multi_remove_handle","curl_easy_escape","curl_easy_unescape","curl_free","curl_free","curl_easy_ssls_export","curl_easy_strerror","curl_share_setopt","curl_share_strerror","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified curl APIs to simulate a complex multi-handle HTTP POST scenario with MIME data, SSL session sharing, and error handling:

*/


// Callback for SSL session export (dummy implementation)
static CURLcode  id_000048_ssl_export_callback(CURL *handle, void *arg, const char *session_key,
                                    const unsigned char *shmac, size_t shmac_len,
                                    const unsigned char *sdata, size_t sdata_len,
                                    curl_off_t lifetime, int flags,
                                    const char *peer_name, size_t peer_name_len) {
  // Dummy implementation - just return OK
  return CURLE_OK;
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





  if (size == 0) {
	return 0;
	}
	

  // 1. Initialize curl globally
  CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
  if (global_res != CURLE_OK) {
    const char *err = curl_easy_strerror(global_res);
    return 0;
  }

  // Create input file from fuzzer data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    curl_global_cleanup();
    assert_file_closed(&in_file);
	return 0;
  }
  
  // Create output file
  FILE *out_file = fopen("output_file", "wb");
  if (!out_file) {
    assert_file_closed(&in_file);;
    curl_global_cleanup();
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // 2. Create and configure share object for SSL session sharing
  CURLSH *share = curl_share_init();
  if (share) {
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
  }

  // 3. Create easy handles
  CURL *easy1 = curl_easy_init();
  CURL *easy2 = curl_easy_init();
  
  if (!easy1 || !easy2) {
    if (easy1) curl_easy_cleanup(easy1);
    if (easy2) curl_easy_cleanup(easy2);
    if (share) curl_share_cleanup(share);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    curl_global_cleanup();
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // 4. Configure easy handles with fuzzer data
  // Use part of input data as URL string
  size_t url_len = size > 100 ? 100 : size;
  char *url = (char *)malloc(url_len + 1);
  if (url) {
    memcpy(url, data, url_len);
    url[url_len] = '\0';
    
    // Set options for first easy handle
    curl_easy_setopt(easy1, CURLOPT_URL, url);
    curl_easy_setopt(easy1, CURLOPT_SHARE, share);
    curl_easy_setopt(easy1, CURLOPT_UPLOAD, 1L);
    
    // Set file descriptor for reading
    curl_easy_setopt(easy1, CURLOPT_READDATA, (void *)in_file);
    curl_easy_setopt(easy1, CURLOPT_INFILESIZE_LARGE, (curl_off_t)size);
    
    free(url);
  }

  // 5. Create MIME structure and add part with fuzzer data
  curl_mime *mime = curl_mime_init(easy1);
  if (mime) {
    curl_mimepart *part = curl_mime_addpart(mime);
    if (part) {
      // Use fuzzer data as MIME data
      curl_mime_data(part, (const char *)data, size);
      curl_mime_name(part, "fuzzed_data");
      curl_mime_type(part, "application/octet-stream");
    }
    curl_easy_setopt(easy1, CURLOPT_MIMEPOST, mime);
  }

  // 6. Configure second easy handle
  curl_easy_setopt(easy2, CURLOPT_URL, "file://output_file");
  curl_easy_setopt(easy2, CURLOPT_SHARE, share);
  curl_easy_setopt(easy2, CURLOPT_WRITEDATA, (void *)out_file);

  // 7. Create multi handle
  CURLM *multi = curl_multi_init();
  if (!multi) {
    if (mime) curl_mime_free(mime);
    curl_easy_cleanup(easy1);
    curl_easy_cleanup(easy2);
    if (share) curl_share_cleanup(share);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    curl_global_cleanup();
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // 8. Add easy handles to multi handle
  curl_multi_add_handle(multi, easy1);
  curl_multi_add_handle(multi, easy2);

  // 9. Disable a notification
  curl_multi_notify_disable(multi, CURLMOPT_SOCKETFUNCTION);

  // 10. Get file descriptor and assign it
  int fd = fuzz_fileno(in_file);
  if (fd >= 0) {
    curl_multi_assign(multi, (curl_socket_t)fd, (void *)"input_file");
  }

  // 11. Perform socket action (simulate I/O)
  int running_handles = 0;
  curl_multi_socket_action(multi, CURL_SOCKET_TIMEOUT, 0, &running_handles);

  // 12. Wait for activity (short timeout)
  struct curl_waitfd extra_fds;
  extra_fds.fd = fd >= 0 ? (curl_socket_t)fd : CURL_SOCKET_BAD;
  extra_fds.events = CURL_WAIT_POLLIN;
  extra_fds.revents = 0;
  
  int ret = 0;
  curl_multi_wait(multi, &extra_fds, 1, 10, &ret);

  // 13. Try to receive data (though connection isn't active)
  char buffer[1024];
  size_t n = 0;
  if (easy1) {
    curl_easy_recv(easy1, buffer, sizeof(buffer), &n);
  }

  // 14. Remove handles from multi
  curl_multi_remove_handle(multi, easy1);
  curl_multi_remove_handle(multi, easy2);

  // 15. Demo escape/unescape with fuzzer data
  if (size > 0) {
    // Take first portion for string operations
    size_t str_len = size > 50 ? 50 : size;
    char *str = (char *)malloc(str_len + 1);
    if (str) {
      memcpy(str, data, str_len);
      str[str_len] = '\0';
      
      // Escape the string
      char *escaped = curl_easy_escape(easy1, str, (int)str_len);
      if (escaped) {
        // Unescape it back
        int outlen = 0;
        char *unescaped = curl_easy_unescape(easy1, escaped, -1, &outlen);
        if (unescaped) {
          curl_free(unescaped);
        }
        curl_free(escaped);
      }
      free(str);
    }
  }

  // 16. Try SSL session export (will likely fail but we call it)
  CURLcode export_res = curl_easy_ssls_export(easy1,  id_000048_ssl_export_callback, NULL);
  if (export_res != CURLE_OK) {
    const char *err = curl_easy_strerror(export_res);
  }

  // 17. Get share error (if any)
  if (share) {
    CURLSHcode share_res = curl_share_setopt(share, CURLSHOPT_UNSHARE, CURL_LOCK_DATA_SSL_SESSION);
    if (share_res != CURLSHE_OK) {
      const char *err = curl_share_strerror(share_res);
    }
  }

  // 18. Clean up in reverse order
  if (mime) curl_mime_free(mime);
  curl_multi_cleanup(multi);
  curl_easy_cleanup(easy1);
  curl_easy_cleanup(easy2);
  if (share) curl_share_cleanup(share);
  assert_file_closed(&out_file);;
  assert_file_closed(&in_file);;
  curl_global_cleanup();

  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
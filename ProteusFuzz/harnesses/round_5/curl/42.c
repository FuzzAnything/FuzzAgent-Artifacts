// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_ws_meta at ws.c:1851:29 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/websockets.h>
#include <curl/typecheck-gcc.h>

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
  CURLcode res;
  CURL *curl = NULL;
  const struct curl_ws_frame *meta = NULL;
  size_t sent = 0;
  unsigned int flags = 0;
  curl_off_t frame_len = 0;
  curl_off_t fragsize = 0;
  const void *buffer = NULL;
  size_t buflen = 0;

  if (Size < 1) {
    return 0;
  }

  // Initialize libcurl globally
  res = curl_global_init(CURL_GLOBAL_DEFAULT);
  if (res != CURLE_OK) {
    return 0;
  }

  // Create a CURL handle
  curl = curl_easy_init();
  if (!curl) {
    curl_global_cleanup();
    return 0;
  }

  // Extract fuzzing parameters if available, with strict bounds checking
  if (Size >= 5) {
    flags = (unsigned int)Data[0];
    frame_len = (curl_off_t)((Data[1] << 24) | (Data[2] << 16) | (Data[3] << 8) | Data[4]);
    if (Size >= 6) {
      fragsize = (curl_off_t)Data[5];
    }
    if (Size >= 7) {
      buflen = (size_t)Data[6];
    }
  }

  // Set a dummy URL to simulate a connection context
  res = curl_easy_setopt(curl, CURLOPT_URL, "ws://example.com");
  if (res != CURLE_OK) {
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
  }

  // Set CONNECT_ONLY to prepare for WebSocket operations
  res = curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
  if (res != CURLE_OK) {
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
  }

  // Perform the connection
  res = curl_easy_perform(curl);
  // Note: We don't check res here to allow fuzzing of error paths

  // Fuzz curl_ws_start_frame
  res = curl_ws_start_frame(curl, flags, frame_len);
  // Ignore return value for fuzzing purposes

  // Fuzz curl_ws_send
  // Ensure we have enough data for the buffer offset (10 bytes) and the requested length
  if (buflen > 0 && Size >= 10 + buflen) {
    buffer = (const void *)(Data + 10);
    res = curl_ws_send(curl, buffer, buflen, &sent, fragsize, flags);
  }

  // Fuzz curl_ws_meta
  meta = curl_ws_meta(curl);
  if (meta) {
    (void)meta->age;
    (void)meta->flags;
    (void)meta->offset;
    (void)meta->bytesleft;
    (void)meta->len;
  }

  // Cleanup
  curl_easy_cleanup(curl);
  curl_global_cleanup();

  return 0;
}
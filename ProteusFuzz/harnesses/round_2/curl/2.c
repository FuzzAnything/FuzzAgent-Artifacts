// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_pushheader_bynum at http2.c:657:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <curl/mprintf.h>

/* Fuzzing entry point */
int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  /* Initialize libcurl global environment */
  curl_global_init(CURL_GLOBAL_ALL);

  /* Create an easy handle */
  CURL *easy = curl_easy_init();
  if (!easy) {
    curl_global_cleanup();
    return 0;
  }

  /* Create a multi handle */
  CURLM *multi = curl_multi_init();
  if (!multi) {
    curl_easy_cleanup(easy);
    curl_global_cleanup();
    return 0;
  }

  /* Prepare dummy data from input */
  char *dummy_str = NULL;
  if (Size > 0) {
    /* Ensure null termination for string operations */
    dummy_str = (char *)malloc(Size + 1);
    if (dummy_str) {
      memcpy(dummy_str, Data, Size);
      dummy_str[Size] = '\0';
    }
  }

  /* 1. Test curl_version */
  /* No arguments needed, just call it to exercise the path */
  (void)curl_version();

  /* 2. Test curl_maprintf */
  char *formatted = NULL;
  if (dummy_str) {
    /* Use a safe format string to avoid crashes, but fuzz the content */
    formatted = curl_maprintf("Fuzzed: %s", dummy_str);
    if (formatted) {
      /* Free the allocated string as per documentation */
      curl_free(formatted);
    }
  }

  /* 3. Test curl_easy_setopt with various options to trigger type checks */
  /* The Wcurl_easy_setopt_err_hstswrite_cb is a compile-time check,
     but we can exercise the runtime path of curl_easy_setopt with valid/invalid types. */
  
  /* Set a simple string option */
  if (dummy_str) {
    curl_easy_setopt(easy, CURLOPT_URL, dummy_str);
  }

  /* Set a long option */
  curl_easy_setopt(easy, CURLOPT_TIMEOUT, (long)Size);

  /* 4. Test curl_easy_getinfo with long type to trigger Wcurl_easy_getinfo_err_long checks */
  long response_code = 0;
  /* This should compile and run without warning if types match */
  curl_easy_getinfo(easy, CURLINFO_RESPONSE_CODE, &response_code);

  /* 5. Test curl_pushheader_bynum and curl_pushheader_byname */
  /* These require a valid struct curl_pushheaders. Since the struct is opaque
     and forward-declared in the public headers, we cannot instantiate it directly.
     We pass NULL to test error handling paths. */
  
  /* Test with NULL struct */
  char *header_by_num = curl_pushheader_bynum(NULL, 0);
  (void)header_by_num; /* Suppress unused variable warning */

  /* Test by name with NULL struct */
  char *header_by_name = curl_pushheader_byname(NULL, "Content-Type");
  (void)header_by_name;

  /* Test with NULL name */
  header_by_name = curl_pushheader_byname(NULL, NULL);
  (void)header_by_name;

  /* Cleanup */
  curl_multi_cleanup(multi);
  curl_easy_cleanup(easy);
  curl_global_cleanup();

  if (dummy_str) {
    free(dummy_str);
  }

  return 0;
}
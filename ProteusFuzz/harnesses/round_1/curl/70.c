// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_trace at easy.c:292:10 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

/*
 * The curl_ssls_export_cb callback signature is defined in curl.h.
 * Based on the error message, the expected signature is:
 * CURLcode (*)(void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)
 *
 * Parameters:
 * 1. handle (void *)
 * 2. userptr (void *)
 * 3. session_id (const char *)
 * 4. session_id_len (const unsigned char *)
 * 5. session_id_len_value (unsigned long) - Note: The error suggests the 5th arg is unsigned long, not a pointer.
 *    Wait, looking at the error: 'const unsigned char *, unsigned long' vs 'const unsigned char *, const unsigned char *'.
 *    Let's re-read the error carefully.
 *    My function: (..., const char *, const unsigned char *, const unsigned char *, unsigned long, ...)
 *    Expected:   (..., const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, ...)
 *    
 *    The difference is the 5th and 6th arguments.
 *    My 5th: const unsigned char * (session_id_len pointer?)
 *    Expected 5th: unsigned long (session_id_len value)
 *    My 6th: unsigned long (session_len value)
 *    Expected 6th: const unsigned char * (session pointer)
 *    My 7th: long (session_age)
 *    Expected 7th: unsigned long (session_len value)
 *    My 8th: int (session_type)
 *    Expected 8th: long (session_age)
 *    My 9th: const char * (peer_cert)
 *    Expected 9th: int (session_type)
 *    My 10th: unsigned long (peer_cert_len)
 *    Expected 10th: const char * (peer_cert)
 *    Expected 11th: unsigned long (peer_cert_len)
 *
 *    Actually, let's look at the standard curl_ssls_export_cb definition from recent curl versions.
 *    It is usually:
 *    typedef CURLcode (*curl_ssls_export_cb)(void *userptr,
 *                                            const char *session_id,
 *                                            size_t session_id_len,
 *                                            const unsigned char *session,
 *                                            size_t session_len,
 *                                            long session_age,
 *                                            int session_type,
 *                                            const char *peer_cert,
 *                                            size_t peer_cert_len);
 *    Wait, the error message says:
 *    'CURLcode (void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)'
 *    This implies the first argument is void * (handle) and second is void * (userptr).
 *    Let's map the error message's expected signature exactly:
 *    1. void * (handle)
 *    2. void * (userptr)
 *    3. const char * (session_id)
 *    4. const unsigned char * (session_id_len pointer?? No, that's weird. Maybe it's a pointer to the length?)
 *    5. unsigned long (session_id_len value)
 *    6. const unsigned char * (session)
 *    7. unsigned long (session_len)
 *    8. long (session_age)
 *    9. int (session_type)
 *    10. const char * (peer_cert)
 *    11. unsigned long (peer_cert_len)
 *
 *    Wait, the error message says:
 *    Passing: 'CURLcode (CURL *, void *, const char *, const unsigned char *, const unsigned char *, unsigned long, long, int, const char *, unsigned long)'
 *    Expected: 'CURLcode (*)(void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)'
 *
 *    Let's align them:
 *    My:       1. CURL * (void *)
 *    Exp:      1. void *
 *    My:       2. void *
 *    Exp:      2. void *
 *    My:       3. const char *
 *    Exp:      3. const char *
 *    My:       4. const unsigned char *
 *    Exp:      4. const unsigned char *
 *    My:       5. const unsigned char *  <-- Mismatch
 *    Exp:      5. unsigned long
 *    My:       6. unsigned long          <-- Mismatch
 *    Exp:      6. const unsigned char *
 *    My:       7. long
 *    Exp:      7. unsigned long
 *    My:       8. int
 *    Exp:      8. long
 *    My:       9. const char *
 *    Exp:      9. int
 *    My:       10. unsigned long
 *    Exp:      10. const char *
 *    Exp:      11. unsigned long
 *
 *    It seems the expected signature in the error message has 11 arguments, while mine has 10.
 *    The error message's expected signature:
 *    (void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)
 *    
 *    Let's try to construct the function based on the error message's expectation.
 *    Arg 1: void * (handle)
 *    Arg 2: void * (userptr)
 *    Arg 3: const char * (session_id)
 *    Arg 4: const unsigned char * (session_id_len_ptr?)
 *    Arg 5: unsigned long (session_id_len_val)
 *    Arg 6: const unsigned char * (session)
 *    Arg 7: unsigned long (session_len)
 *    Arg 8: long (session_age)
 *    Arg 9: int (session_type)
 *    Arg 10: const char * (peer_cert)
 *    Arg 11: unsigned long (peer_cert_len)
 *
 *    Wait, the error message says:
 *    'CURLcode (*)(void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)'
 *    Let's count:
 *    1. void *
 *    2. void *
 *    3. const char *
 *    4. const unsigned char *
 *    5. unsigned long
 *    6. const unsigned char *
 *    7. unsigned long
 *    8. long
 *    9. int
 *    10. const char *
 *    11. unsigned long
 *
 *    This is 11 arguments.
 *    My function had 10 arguments.
 *    Let's redefine the callback to match the 11-argument signature exactly as shown in the error message.
 */

static CURLcode ssls_export_callback(void *handle, void *userptr,
                                     const char *session_id,
                                     const unsigned char *session_id_len_ptr,
                                     unsigned long session_id_len_val,
                                     const unsigned char *session,
                                     unsigned long session_len,
                                     long session_age,
                                     int session_type,
                                     const char *peer_cert,
                                     unsigned long peer_cert_len) {
    (void)handle;
    (void)userptr;
    (void)session_id;
    (void)session_id_len_ptr;
    (void)session_id_len_val;
    (void)session;
    (void)session_len;
    (void)session_age;
    (void)session_type;
    (void)peer_cert;
    (void)peer_cert_len;
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    // Prepare a null-terminated URL from input
    char *url_buf = malloc(Size + 1);
    if (!url_buf) {
        curl_easy_cleanup(handle);
        return 0;
    }
    memcpy(url_buf, Data, Size);
    url_buf[Size] = '\0';

    // Prepare a null-terminated POST data from input
    char *post_buf = malloc(Size + 1);
    if (!post_buf) {
        free(url_buf);
        curl_easy_cleanup(handle);
        return 0;
    }
    memcpy(post_buf, Data, Size);
    post_buf[Size] = '\0';

    // Prepare a null-terminated trace config from input
    char *trace_buf = malloc(Size + 1);
    if (!trace_buf) {
        free(url_buf);
        free(post_buf);
        curl_easy_cleanup(handle);
        return 0;
    }
    memcpy(trace_buf, Data, Size);
    trace_buf[Size] = '\0';

    // Initialize global tracing
    curl_global_trace(trace_buf);

    // Set options
    curl_easy_setopt(handle, CURLOPT_URL, url_buf);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, post_buf);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, Size);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "fuzz-agent");
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);

    // Perform the transfer
    curl_easy_perform(handle);

    // Get info
    long http_code = 0;
    double total_time = 0.0;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_code);
    curl_easy_getinfo(handle, CURLINFO_TOTAL_TIME, &total_time);

    // Export SSL sessions
    curl_easy_ssls_export(handle, ssls_export_callback, NULL);

    // Perform upkeep
    curl_easy_upkeep(handle);

    // Cleanup
    curl_easy_cleanup(handle);
    free(url_buf);
    free(post_buf);
    free(trace_buf);

    return 0;
}
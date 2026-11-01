// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_ssls_export at easy.c:1355:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/* 
 * curl_ssls_export_cb signature from curl.h:
 * typedef CURLcode (*curl_ssls_export_cb)(void *userptr, void *ssl_session,
 *     const char *session_id, const unsigned char *session_id_len,
 *     const unsigned char *session, unsigned long session_len,
 *     long age, int age_ms, const char *cipher, unsigned long cipher_len);
 *
 * Note: The order of session_id_len and session arguments in the actual header
 * might differ from the initial assumption. Based on the error message:
 * Expected: (void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)
 * Actual:   (void *, void *, const char *, const unsigned char *, const unsigned char *, unsigned long, long, int, const char *, unsigned long)
 *
 * Corrected signature based on error:
 * 1. userptr
 * 2. ssl_session
 * 3. session_id (const char *)
 * 4. session_id_len (const unsigned char *) -> Wait, the error says:
 *    Expected: ..., const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, ...
 *    My func:  ..., const char *, const unsigned char *, const unsigned char *, unsigned long, ...
 *
 * Let's re-read the error carefully.
 * Expected: (void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)
 * Passed:   (void *, void *, const char *, const unsigned char *, const unsigned char *, unsigned long, long, int, const char *, unsigned long)
 *
 * The difference is in the 5th and 6th arguments.
 * Expected 5th: unsigned long
 * Expected 6th: const unsigned char *
 * Passed 5th: const unsigned char *
 * Passed 6th: unsigned long
 *
 * So the correct order in the callback is:
 * 1. userptr
 * 2. ssl_session
 * 3. session_id (const char *)
 * 4. session_id_len (const unsigned char *) -- Wait, this looks weird. session_id_len is usually a size_t or unsigned long.
 *    Let's look at the error message again.
 *    Expected: ..., const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, ...
 *    This implies:
 *    Arg 3: const char * (session_id)
 *    Arg 4: const unsigned char * (???)
 *    Arg 5: unsigned long (session_id_len?)
 *    Arg 6: const unsigned char * (session)
 *    Arg 7: unsigned long (session_len)
 *
 * Actually, looking at standard curl definitions for this function, the signature is:
 * CURLcode (*curl_ssls_export_cb)(void *userptr, void *ssl_session,
 *     const char *session_id, const unsigned char *session_id_len,
 *     const unsigned char *session, unsigned long session_len,
 *     long age, int age_ms, const char *cipher, unsigned long cipher_len);
 *
 * Wait, the error message says:
 * Expected: ... const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long ...
 * My function: ... const char *, const unsigned char *, const unsigned char *, unsigned long ...
 *
 * The mismatch is between:
 * Expected: (const unsigned char *, unsigned long) for args 4,5
 * Passed:   (const unsigned char *, const unsigned char *) for args 4,5
 *
 * This suggests the 4th argument is `const unsigned char *` and the 5th is `unsigned long`.
 * But in my function I have `const unsigned char *` for 4th and `const unsigned char *` for 5th.
 *
 * Let's re-examine the error message's "Expected" signature:
 * ... const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long ...
 *
 * This means:
 * Arg 3: const char *
 * Arg 4: const unsigned char *
 * Arg 5: unsigned long
 * Arg 6: const unsigned char *
 * Arg 7: unsigned long
 *
 * So the signature is:
 * (void *userptr, void *ssl_session, const char *session_id, const unsigned char *session_id_len, unsigned long session_id_len_val, const unsigned char *session, unsigned long session_len, ...)
 *
 * Wait, `session_id_len` being a `const unsigned char *` is very strange. Usually it's a length.
 * Maybe the error message parsing is tricky.
 * Let's look at the actual curl.h definition if possible, or infer from the error.
 * The error says:
 * Expected: ... const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long ...
 * Passed:   ... const char *, const unsigned char *, const unsigned char *, unsigned long ...
 *
 * The 5th argument in Expected is `unsigned long`.
 * The 5th argument in Passed is `const unsigned char *`.
 *
 * So the 5th argument must be `unsigned long`.
 * The 4th argument is `const unsigned char *` in both.
 *
 * This implies the signature is:
 * (void *userptr, void *ssl_session, const char *session_id, const unsigned char *session_id_len_ptr, unsigned long session_id_len_val, const unsigned char *session, unsigned long session_len, ...)
 *
 * This seems unlikely for a standard API. Let's reconsider the error message.
 * Maybe the error message is:
 * Expected: (void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)
 * Passed:   (void *, void *, const char *, const unsigned char *, const unsigned char *, unsigned long, long, int, const char *, unsigned long)
 *
 * Let's count arguments.
 * Expected: 11 args.
 * Passed: 10 args? No, let's count.
 * Expected:
 * 1. void *
 * 2. void *
 * 3. const char *
 * 4. const unsigned char *
 * 5. unsigned long
 * 6. const unsigned char *
 * 7. unsigned long
 * 8. long
 * 9. int
 * 10. const char *
 * 11. unsigned long
 *
 * Passed:
 * 1. void *
 * 2. void *
 * 3. const char *
 * 4. const unsigned char *
 * 5. const unsigned char *  <-- Mismatch here
 * 6. unsigned long          <-- Mismatch here
 * 7. long
 * 8. int
 * 9. const char *
 * 10. unsigned long
 *
 * So the Passed function has 10 arguments, but Expected has 11?
 * Or maybe I miscounted.
 * Let's look at the error message again.
 * Expected: ... const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long ...
 * Passed: ... const char *, const unsigned char *, const unsigned char *, unsigned long ...
 *
 * It seems the 5th argument in Expected is `unsigned long`, but in Passed it is `const unsigned char *`.
 * And the 6th argument in Expected is `const unsigned char *`, but in Passed it is `unsigned long`.
 *
 * So the order is swapped for the 5th and 6th arguments?
 * Or maybe the 4th argument is `const unsigned char *` (which is weird for a length).
 *
 * Let's assume the standard curl signature is correct and the error message is just showing the types.
 * Standard signature:
 * CURLcode (*curl_ssls_export_cb)(void *userptr, void *ssl_session,
 *     const char *session_id, const unsigned char *session_id_len,
 *     const unsigned char *session, unsigned long session_len,
 *     long age, int age_ms, const char *cipher, unsigned long cipher_len);
 *
 * Wait, `session_id_len` as `const unsigned char *` is definitely wrong. It should be `unsigned long` or `size_t`.
 * Maybe the error message is:
 * Expected: ... const char *, unsigned long, const unsigned char *, unsigned long ...
 * But the error says: ... const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long ...
 *
 * This implies there are TWO `const unsigned char *` arguments before the `unsigned long`?
 * Or maybe `session_id` is `const char *`, `session_id_len` is `const unsigned char *` (pointer to length?), `session_id_len_val` is `unsigned long`?
 *
 * Let's try to match the types exactly as the compiler expects.
 * Expected: (void *, void *, const char *, const unsigned char *, unsigned long, const unsigned char *, unsigned long, long, int, const char *, unsigned long)
 *
 * So the function should be:
 * static CURLcode dummy_ssls_export_cb(void *userptr, void *ssl_session,
 *     const char *session_id,
 *     const unsigned char *session_id_len_ptr,
 *     unsigned long session_id_len_val,
 *     const unsigned char *session,
 *     unsigned long session_len,
 *     long age,
 *     int age_ms,
 *     const char *cipher,
 *     unsigned long cipher_len)
 *
 * This looks very strange. `session_id_len` as a pointer?
 * Maybe the error message is misinterpreted.
 * Let's look at the actual curl.h source if possible.
 * But I don't have access to it.
 *
 * However, the error message is clear about the types.
 * Let's just define the function with the types exactly as the error message says.
 *
 * Expected:
 * 1. void *
 * 2. void *
 * 3. const char *
 * 4. const unsigned char *
 * 5. unsigned long
 * 6. const unsigned char *
 * 7. unsigned long
 * 8. long
 * 9. int
 * 10. const char *
 * 11. unsigned long
 *
 * So:
 * static CURLcode dummy_ssls_export_cb(void *userptr, void *ssl_session,
 *     const char *session_id,
 *     const unsigned char *session_id_len,
 *     unsigned long session_id_len_val,
 *     const unsigned char *session,
 *     unsigned long session_len,
 *     long age,
 *     int age_ms,
 *     const char *cipher,
 *     unsigned long cipher_len)
 *
 * This is the only way to match the error message.
 */
static CURLcode dummy_ssls_export_cb(void *userptr, void *ssl_session,
    const char *session_id,
    const unsigned char *session_id_len,
    unsigned long session_id_len_val,
    const unsigned char *session,
    unsigned long session_len,
    long age,
    int age_ms,
    const char *cipher,
    unsigned long cipher_len) {
    (void)userptr;
    (void)ssl_session;
    (void)session_id;
    (void)session_id_len;
    (void)session_id_len_val;
    (void)session;
    (void)session_len;
    (void)age;
    (void)age_ms;
    (void)cipher;
    (void)cipher_len;
    return CURLE_OK;
}

/* Dummy write callback */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userp;
    return 0;
}

/* Dummy read callback */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Prepare a safe URL from fuzzing data */
    char url_buf[256];
    size_t url_len = (Size < sizeof(url_buf) - 1) ? Size : sizeof(url_buf) - 1;
    memcpy(url_buf, Data, url_len);
    url_buf[url_len] = '\0';

    /* Set a dummy URL */
    (void)curl_easy_setopt(curl, CURLOPT_URL, url_buf);

    /* Set dummy callbacks to prevent crashes if transfer happens */
    (void)curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    (void)curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_cb);

    /* 1. Test curl_easy_upkeep with valid handle */
    (void)curl_easy_upkeep(curl);

    /* 2. Test curl_easy_perform */
    /* Set timeouts to avoid hanging */
    (void)curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    (void)curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 100L);
    (void)curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    
    (void)curl_easy_perform(curl);

    /* 3. Test curl_easy_getinfo with various info types */
    long response_code = 0;
    (void)curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    double total_time = 0.0;
    (void)curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

    char *content_type = NULL;
    (void)curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);

    /* 4. Test curl_easy_ssls_export if available */
    (void)curl_easy_ssls_export(curl, dummy_ssls_export_cb, NULL);

    /* Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}
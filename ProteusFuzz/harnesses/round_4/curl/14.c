// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl API functions:
 * - curl_strequal
 * - curl_multi_info_read
 * - curl_mprintf
 * - curl_msprintf
 *
 * Note: Wcurl_easy_setopt_err_ssh_key_cb and Wcurl_easy_setopt_err_curl_slist
 * are compile-time warning macros and cannot be invoked at runtime.
 */

static void write_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy data for testing\n");
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure dummy file exists if needed */
    write_dummy_file();

    /* Initialize curl global environment */
    CURLcode init_result = curl_global_init(CURL_GLOBAL_ALL);
    if (init_result != CURLE_OK) {
        return 0;
    }

    /*
     * Test 1: curl_strequal
     * Fuzz with various string combinations derived from input data.
     * Handle NULL cases explicitly.
     */
    const char *s1 = NULL;
    const char *s2 = NULL;
    char buf1[256];
    char buf2[256];

    /* Case A: Both NULL */
    int eq1 = curl_strequal(NULL, NULL);
    (void)eq1; /* Use result to avoid unused warning */

    /* Case B: One NULL, one non-NULL */
    if (Size > 0) {
        buf1[0] = Data[0];
        buf1[1] = '\0';
        eq1 = curl_strequal(NULL, buf1);
        (void)eq1;

        eq1 = curl_strequal(buf1, NULL);
        (void)eq1;
    }

    /* Case C: Both non-NULL, fuzz content */
    size_t len1 = Size > 255 ? 255 : Size;
    size_t len2 = Size > 255 ? 255 : Size;
    if (Size > 0) {
        memcpy(buf1, Data, len1);
        buf1[len1] = '\0';
        memcpy(buf2, Data, len2);
        buf2[len2] = '\0';
        eq1 = curl_strequal(buf1, buf2);
        (void)eq1;
    }

    /*
     * Test 2: curl_multi_info_read
     * Requires a valid CURLM handle. Create one, add a dummy easy handle,
     * and attempt to read info.
     */
    CURLM *multi = curl_multi_init();
    if (multi) {
        CURL *easy = curl_easy_init();
        if (easy) {
            /* Set a dummy URL to trigger some state */
            if (Size > 0) {
                char url[512];
                size_t url_len = Size < 511 ? Size : 511;
                memcpy(url, Data, url_len);
                url[url_len] = '\0';
                curl_easy_setopt(easy, CURLOPT_URL, url);
                /* CURLOPT_RETURNTRANSFER is not available in all builds or versions,
                 * so we use a simpler option like CURLOPT_FOLLOWLOCATION or just skip it.
                 * Here we use CURLOPT_TIMEOUT to ensure a valid long option is set. */
                curl_easy_setopt(easy, CURLOPT_TIMEOUT, 1L);
            }

            curl_multi_add_handle(multi, easy);

            /* Perform a non-blocking operation to potentially generate messages */
            int running = 0;
            curl_multi_perform(multi, &running);

            int msgs_in_queue = 0;
            CURLMsg *msg = NULL;
            while ((msg = curl_multi_info_read(multi, &msgs_in_queue)) != NULL) {
                /* Process message if any */
                (void)msg;
            }

            curl_multi_remove_handle(multi, easy);
            curl_easy_cleanup(easy);
        }
        curl_multi_cleanup(multi);
    }

    /*
     * Test 3: curl_mprintf
     * Fuzz format string and arguments.
     */
    char fmt_buf[256];
    size_t fmt_len = Size < 255 ? Size : 255;
    if (Size > 0) {
        memcpy(fmt_buf, Data, fmt_len);
        fmt_buf[fmt_len] = '\0';
        /* Ensure format string is safe by limiting % specifiers if needed,
         * but for fuzzing we allow raw input to test robustness. */
        int ret = curl_mprintf(fmt_buf);
        (void)ret;
    }

    /*
     * Test 4: curl_msprintf
     * Fuzz format string and buffer.
     */
    char out_buf[1024];
    if (Size > 0) {
        size_t fmt_len2 = Size < 255 ? Size : 255;
        memcpy(fmt_buf, Data, fmt_len2);
        fmt_buf[fmt_len2] = '\0';
        int ret = curl_msprintf(out_buf, fmt_buf);
        (void)ret;
    }

    /* Cleanup global environment */
    curl_global_cleanup();

    return 0;
}
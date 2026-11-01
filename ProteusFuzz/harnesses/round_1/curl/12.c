// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_duphandle at easy.c:952:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void setup_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    // Ensure URL is null-terminated
    char url_buf[256];
    size_t url_len = (Size < sizeof(url_buf) - 1) ? Size : sizeof(url_buf) - 1;
    memcpy(url_buf, Data, url_len);
    url_buf[url_len] = '\0';

    // Set URL
    curl_easy_setopt(handle, CURLOPT_URL, url_buf);

    // Set other options to trigger more code paths
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "FuzzerAgent");
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, 5L);

    // Prepare POST data if needed, ensuring null termination or size setting
    if (Size > 0) {
        char post_buf[256];
        size_t post_len = (Size < sizeof(post_buf) - 1) ? Size : sizeof(post_buf) - 1;
        memcpy(post_buf, Data, post_len);
        post_buf[post_len] = '\0';
        curl_easy_setopt(handle, CURLOPT_POSTFIELDS, post_buf);
        curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, post_len);
    }

    // Test curl_easy_perform
    curl_easy_perform(handle);

    // Test curl_easy_getinfo after perform
    long http_code = 0;
    curl_easy_getinfo(handle, CURLINFO_HTTP_CODE, &http_code);

    double total_time = 0.0;
    curl_easy_getinfo(handle, CURLINFO_TOTAL_TIME, &total_time);

    // Test curl_easy_duphandle
    CURL *dup_handle = curl_easy_duphandle(handle);
    if (dup_handle) {
        curl_easy_perform(dup_handle);
        curl_easy_cleanup(dup_handle);
    }

    // Test curl_easy_pause
    curl_easy_pause(handle, CURLPAUSE_RECV);
    curl_easy_pause(handle, CURLPAUSE_SEND);
    curl_easy_pause(handle, CURLPAUSE_ALL);
    curl_easy_pause(handle, CURLPAUSE_CONT);

    // Test curl_easy_upkeep
    curl_easy_upkeep(handle);

    // Cleanup
    curl_easy_cleanup(handle);

    return 0;
}
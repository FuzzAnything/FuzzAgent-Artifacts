// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Static helper to ensure we don't pollute global namespace */
static void setup_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy content for fuzzing\n");
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl global state if not already done */
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }

    /* Create a dummy file if needed for file-based options */
    setup_dummy_file();

    /* Create a CURL handle */
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Use input data to drive fuzzing logic */
    const uint8_t *ptr = Data;
    size_t remaining = Size;

    /* Extract a URL from the input if possible, otherwise use a safe default */
    char url[256] = "http://127.0.0.1/";
    if (remaining > 0) {
        size_t url_len = remaining < sizeof(url) - 1 ? remaining : sizeof(url) - 1;
        memcpy(url, ptr, url_len);
        url[url_len] = '\0';
        ptr += url_len;
        remaining -= url_len;
    }

    /* Set the URL */
    curl_easy_setopt(curl, CURLOPT_URL, url);

    /* Fuzz various options based on input data to explore different states */
    if (remaining > 0) {
        long long_val = (long)*ptr;
        ptr++;
        remaining--;
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, long_val);
    }

    if (remaining > 0) {
        long long_val = (long)*ptr;
        ptr++;
        remaining--;
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, long_val);
    }

    /* Set a dummy write callback to avoid errors during perform */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL); /* Default is stdout */

    /* Perform the transfer */
    CURLcode res = curl_easy_perform(curl);

    /* Call curl_easy_upkeep to test connection pool maintenance */
    (void)curl_easy_upkeep(curl);

    /* Test curl_easy_getinfo with various info types */
    long response_code = 0;
    double total_time = 0.0;
    curl_off_t size_download = 0;
    char *effective_url = NULL;
    struct curl_slist *slist = NULL;

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &size_download);
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist);

    /* Cleanup slist if allocated */
    if (slist) {
        curl_slist_free_all(slist);
    }

    /* Cleanup the CURL handle */
    curl_easy_cleanup(curl);

    return 0;
}
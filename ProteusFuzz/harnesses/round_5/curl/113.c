// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

/* Static helper to create a dummy file if needed */
static void create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    if (size == 0) return;
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl global environment */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    /* Create a CURL handle */
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Ensure we have a null-terminated string for URL if possible */
    char url_buf[1024];
    size_t url_len = (Size < sizeof(url_buf) - 1) ? Size : sizeof(url_buf) - 1;
    memcpy(url_buf, Data, url_len);
    url_buf[url_len] = '\0';

    /* Set a basic URL to ensure a valid session context */
    curl_easy_setopt(curl, CURLOPT_URL, url_buf);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    /* 
     * Fuzzing curl_easy_getinfo and related type-checking macros.
     * We test various info types with correctly typed variables.
     */
    
    /* Test CURLINFO_LONG */
    long long_val = 0;
    curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &long_val);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_val);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME_T, &long_val);

    /* Test CURLINFO_DOUBLE */
    double double_val = 0.0;
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &double_val);
    curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &double_val);
    curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &double_val);
    curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &double_val);
    curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD, &double_val);
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &double_val);
    curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &double_val);
    curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &double_val);

    /* Test CURLINFO_OFF_T */
    curl_off_t off_t_val = 0;
    curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &off_t_val);
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_t_val);

    /* Test CURLINFO_SOCKET */
    curl_socket_t socket_val = CURL_SOCKET_BAD;
    curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_val);

    /* Test CURLINFO_STRING */
    char *string_val = NULL;
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &string_val);
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &string_val);
    curl_easy_getinfo(curl, CURLINFO_FTP_ENTRY_PATH, &string_val);
    curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &string_val);
    curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &string_val);
    curl_easy_getinfo(curl, CURLINFO_LOCAL_IP, &string_val);

    /* Test CURLINFO_CERTINFO (requires struct allocation) */
    struct curl_certinfo certinfo;
    memset(&certinfo, 0, sizeof(certinfo));
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);

    /* Test CURLINFO_TLS_SESSION (requires struct allocation) */
    struct curl_tlssessioninfo tlsinfo;
    memset(&tlsinfo, 0, sizeof(tlsinfo));
    curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tlsinfo);

    /* Test CURLINFO_COOKIELIST (requires slist pointer) */
    struct curl_slist *slist_val = NULL;
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_val);
    if (slist_val) {
        curl_slist_free_all(slist_val);
    }

    /* 
     * Fuzzing curl_easy_upkeep
     * This function performs connection upkeep.
     */
    curl_easy_upkeep(curl);

    /* 
     * Fuzzing curl_easy_setopt with various types to trigger type-checking macros
     * The macros themselves are compile-time, but passing diverse data exercises
     * the runtime handling of these options.
     */

    /* Long options - Safe access with bounds checking */
    if (Size > 0) {
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, (long)Data[0]);
    }
    if (Size > 1) {
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, (long)Data[1]);
    }
    if (Size > 2) {
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, (long)Data[2]);
    }

    /* String options (using the fuzzing data as a buffer, ensuring null termination) */
    char *str_ptr = (char *)Data;
    /* Ensure null termination for safety if the fuzzer doesn't provide it */
    if (Size > 0 && Data[Size-1] != 0) {
        /* We can't easily modify the input buffer, so we use a copy or a safe subset */
        char safe_str[256];
        size_t safe_len = (Size < 255) ? Size : 255;
        memcpy(safe_str, Data, safe_len);
        safe_str[safe_len] = '\0';
        str_ptr = safe_str;
    }

    curl_easy_setopt(curl, CURLOPT_USERAGENT, str_ptr);
    curl_easy_setopt(curl, CURLOPT_REFERER, str_ptr);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, str_ptr);

    /* Callback data options (void *) */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_PRIVATE, (void *)Data);

    /* Create a dummy file for file-based options if needed */
    if (Size > 0) {
        create_dummy_file("./dummy_file", Data, Size);
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_CAINFO, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_SSLCERT, "./dummy_file");
        curl_easy_setopt(curl, CURLOPT_SSLKEY, "./dummy_file");
    }

    /* Perform a dummy transfer to populate getinfo data (optional but good for state) */
    /* We use a short timeout to avoid hanging */
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    (void)curl_easy_perform(curl);

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}
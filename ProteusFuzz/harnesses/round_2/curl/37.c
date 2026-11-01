// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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

/* Static helper to safely create a null-terminated string from fuzz input */
static char *safe_strdup_fuzz(const uint8_t *data, size_t size) {
    if (size == 0) {
        return strdup("");
    }
    char *str = malloc(size + 1);
    if (!str) return NULL;
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

/* Static dummy callbacks to satisfy type requirements */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return size * nmemb;
}

static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static int dummy_progress_cb(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_debug_cb(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle; (void)type; (void)data; (void)size; (void)userptr;
    return 0;
}

static int dummy_trailer_cb(CURL *curl, char *key, char *value, void *userptr) {
    (void)curl; (void)key; (void)value; (void)userptr;
    return 0;
}

static int dummy_chunk_bgn_cb(CURL *curl, void *userdata, int *first, void *item, void *prev) {
    (void)curl; (void)userdata; (void)first; (void)item; (void)prev;
    return 0;
}

static int dummy_chunk_end_cb(CURL *curl, void *userdata, int *first, void *item) {
    (void)curl; (void)userdata; (void)first; (void)item;
    return 0;
}

static int dummy_seek_cb(void *instream, curl_off_t offset, int origin) {
    (void)instream; (void)offset; (void)origin;
    return 0;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    /* Initialize dummy file if needed */
    FILE *dummy_file = fopen("./dummy_file", "w");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    /* Prepare safe string from input */
    char *safe_str = safe_strdup_fuzz(Data, Size);
    if (!safe_str) {
        curl_easy_cleanup(curl);
        return 0;
    }

    /* Fuzz various options with safe data */
    
    /* CURLOPT_URL: must be null-terminated */
    curl_easy_setopt(curl, CURLOPT_URL, safe_str);

    /* CURLOPT_POSTFIELDS: must be null-terminated */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, safe_str);

    /* CURLOPT_WRITEFUNCTION */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);

    /* CURLOPT_READFUNCTION */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_cb);

    /* CURLOPT_PROGRESSFUNCTION */
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_cb);

    /* CURLOPT_DEBUGFUNCTION */
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);

    /* CURLOPT_TRAILERFUNCTION */
    curl_easy_setopt(curl, CURLOPT_TRAILERFUNCTION, dummy_trailer_cb);

    /* CURLOPT_CHUNK_BGN_FUNCTION */
    curl_easy_setopt(curl, CURLOPT_CHUNK_BGN_FUNCTION, dummy_chunk_bgn_cb);

    /* CURLOPT_CHUNK_END_FUNCTION */
    curl_easy_setopt(curl, CURLOPT_CHUNK_END_FUNCTION, dummy_chunk_end_cb);

    /* CURLOPT_SEEKFUNCTION */
    curl_easy_setopt(curl, CURLOPT_SEEKFUNCTION, dummy_seek_cb);

    /* CURLOPT_WRITEDATA: pass fuzz data as userdata */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)Data);

    /* CURLOPT_READDATA */
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)Data);

    /* CURLOPT_PROGRESSDATA */
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, (void *)Data);

    /* CURLOPT_DEBUGDATA */
    curl_easy_setopt(curl, CURLOPT_DEBUGDATA, (void *)Data);

    /* CURLOPT_TRAILERDATA */
    curl_easy_setopt(curl, CURLOPT_TRAILERDATA, (void *)Data);

    /* CURLOPT_CHUNK_DATA */
    curl_easy_setopt(curl, CURLOPT_CHUNK_DATA, (void *)Data);

    /* CURLOPT_SEEKDATA */
    curl_easy_setopt(curl, CURLOPT_SEEKDATA, (void *)Data);

    /* Test curl_easy_getinfo with double pointer (Wcurl_easy_getinfo_err_double) */
    double dummy_double;
    curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &dummy_double);
    curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_UPLOAD, &dummy_double);
    curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &dummy_double);
    curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &dummy_double);

    /* Cleanup */
    free(safe_str);
    curl_easy_cleanup(curl);

    return 0;
}
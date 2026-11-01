// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>
#include <stdarg.h>

/* Dummy write callback for curl_easy_setopt */
static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userptr) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userptr;
    return 0;
}

/* Dummy read callback for curl_easy_setopt */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userptr) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userptr;
    return 0;
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize CURL handle */
    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    /* Create a dummy file for file-based operations */
    const char *dummy_file_path = "./dummy_file";
    FILE *fd = fopen(dummy_file_path, "w+");
    if (!fd) {
        curl_easy_cleanup(handle);
        return 0;
    }

    /* Write fuzzing data to the dummy file */
    fwrite(Data, 1, Size, fd);
    fclose(fd);

    /* Reopen for reading if needed */
    fd = fopen(dummy_file_path, "r");
    if (!fd) {
        curl_easy_cleanup(handle);
        return 0;
    }

    /* Prepare a null-terminated string from input for safe formatting */
    char *safe_data = malloc(Size + 1);
    if (!safe_data) {
        fclose(fd);
        curl_easy_cleanup(handle);
        return 0;
    }
    memcpy(safe_data, Data, Size);
    safe_data[Size] = '\0';

    /* Fuzz curl_mfprintf */
    rewind(fd);
    curl_mfprintf(fd, "%s", safe_data);

    /* Fuzz curl_msprintf */
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    curl_msprintf(buffer, "%s", safe_data);

    /* Fuzz curl_easy_unescape */
    int outlength = 0;
    char *unescaped = curl_easy_unescape(handle, (const char *)Data, Size, &outlength);
    if (unescaped) {
        curl_free(unescaped);
    }

    /* Fuzz curl_easy_setopt with various options to trigger type checks */
    /* String option: Use safe_data which is guaranteed to be null-terminated */
    curl_easy_setopt(handle, CURLOPT_URL, safe_data);
    
    /* Long option */
    long long_val = (Size > 0) ? (long)Data[0] : 0;
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, long_val);

    /* Callback options */
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(handle, CURLOPT_READFUNCTION, dummy_read_callback);

    /* slist option: Use safe_data which is guaranteed to be null-terminated */
    struct curl_slist *slist = NULL;
    slist = curl_slist_append(slist, safe_data);
    if (slist) {
        curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist);
        curl_slist_free_all(slist);
    }

    /* Fuzz curl_easy_getinfo with various info types */
    char *string_info = NULL;
    long long_info = 0;
    double double_info = 0.0;
    struct curl_slist *slist_info = NULL;
    curl_off_t off_t_info = 0;
    curl_socket_t socket_info = 0;

    curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &string_info);
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &long_info);
    curl_easy_getinfo(handle, CURLINFO_SIZE_DOWNLOAD, &double_info);
    curl_easy_getinfo(handle, CURLINFO_COOKIELIST, &slist_info);
    curl_easy_getinfo(handle, CURLINFO_SIZE_UPLOAD_T, &off_t_info);
    curl_easy_getinfo(handle, CURLINFO_ACTIVESOCKET, &socket_info);

    /* Cleanup */
    if (slist_info) {
        curl_slist_free_all(slist_info);
    }

    fclose(fd);
    free(safe_data);
    curl_easy_cleanup(handle);
    remove(dummy_file_path);

    return 0;
}
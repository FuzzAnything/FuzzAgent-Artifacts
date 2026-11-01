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
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>
#include <stdarg.h>

/* Dummy callback implementations to satisfy type requirements */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return size * nmemb;
}

static int dummy_debug_cb(CURL *handle, curl_infotype type, char *data, size_t size, void *userp) {
    (void)handle; (void)type; (void)data; (void)size; (void)userp;
    return 0;
}

static int dummy_progress_cb(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_slist_cb(CURL *handle, int socket, curlsocktype purpose, void *userp) {
    (void)handle; (void)socket; (void)purpose; (void)userp;
    return 0;
}

/* Helper function to use va_start correctly with variadic arguments */
static void test_mvsprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Prepare a buffer for string operations */
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));

    /* Extract data from fuzz input */
    const char *input_str = (const char *)Data;
    size_t input_len = Size;

    /* Ensure null termination for string operations */
    if (input_len < sizeof(buffer)) {
        memcpy(buffer, input_str, input_len);
        buffer[input_len] = '\0';
    } else {
        memcpy(buffer, input_str, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
    }

    /* Test curl_maprintf with various formats derived from input */
    char *mapped_str = NULL;
    if (input_len >= 2) {
        /* Use first byte as format specifier hint, second as data */
        char fmt[32];
        snprintf(fmt, sizeof(fmt), "%%c%%s", buffer[0]);
        mapped_str = curl_maprintf(fmt, buffer[1]);
    } else if (input_len >= 1) {
        mapped_str = curl_maprintf("%%s", buffer);
    } else {
        mapped_str = curl_maprintf("default");
    }

    if (mapped_str) {
        /* Use the mapped string in setopt to exercise more paths */
        curl_easy_setopt(curl, CURLOPT_URL, mapped_str);
        curl_free(mapped_str);
    }

    /* Test curl_mvsprintf with va_list via helper function */
    test_mvsprintf(buffer, "test: %s", buffer);

    /* Exercise curl_easy_setopt with various option types to trigger type checking macros */
    /* String options */
    curl_easy_setopt(curl, CURLOPT_URL, buffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, buffer);
    curl_easy_setopt(curl, CURLOPT_REFERER, buffer);

    /* Long options */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)input_len);

    /* Callback options */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_cb);
    curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, dummy_slist_cb);

    /* Data pointers for callbacks */
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)buffer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)buffer);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, (void *)buffer);

    /* Exercise curl_easy_getinfo to trigger type checking macros */
    char *info_str = NULL;
    long info_long = 0;
    double info_double = 0.0;
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &info_str);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &info_long);
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &info_double);

    /* Perform a dummy perform to exercise state */
    (void)curl_easy_perform(curl);

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}
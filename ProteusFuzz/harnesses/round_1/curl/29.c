// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formget at formdata.c:627:5 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* Callback for curl_formget matching the expected signature:
   size_t (*curl_formget_callback)(void *userp, const char *ptr, size_t size);
*/
static size_t formget_callback(void *userp, const char *ptr, size_t size) {
    (void)userp;
    (void)ptr;
    (void)size;
    /* Simple callback that returns a fixed size to simulate data */
    return 1;
}

/* Callback for CURLOPT_CLOSESOCKETFUNCTION to satisfy type checking */
static int closesocket_callback(void *handle, curl_socket_t fd) {
    (void)handle;
    (void)fd;
    return 0;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment if not already done */
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Prepare a buffer for curl_msprintf */
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    /* Fuzz curl_mprintf with various format strings derived from input */
    /* Ensure null termination for safety */
    char format_str[128];
    size_t fmt_len = (Size < sizeof(format_str) - 1) ? Size : sizeof(format_str) - 1;
    memcpy(format_str, Data, fmt_len);
    format_str[fmt_len] = '\0';

    /* Avoid dangerous format specifiers that might cause crashes in mprintf */
    /* We rely on curl_mprintf's internal safety, but limit complexity */
    curl_mprintf("%s", format_str);

    /* Fuzz curl_msprintf */
    int msprintf_ret = curl_msprintf(buffer, "%s", format_str);
    (void)msprintf_ret; /* Suppress unused return value warning */

    /* Fuzz curl_formadd and curl_formfree */
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *last = NULL;

    /* Create a simple form part using input data */
    if (Size > 0) {
        char *name = malloc(Size + 1);
        char *contents = malloc(Size + 1);
        if (name && contents) {
            memcpy(name, Data, Size);
            name[Size] = '\0';
            memcpy(contents, Data, Size);
            contents[Size] = '\0';

            CURLFORMcode code = curl_formadd(&formpost, &last,
                                             CURLFORM_COPYNAME, name,
                                             CURLFORM_COPYCONTENTS, contents,
                                             CURLFORM_END);
            (void)code; /* Ignore return code for fuzzing stability */

            /* Free the temporary allocations if formadd failed or to prevent leaks */
            /* Note: curl_formadd copies the strings if CURLFORM_COPYNAME/CONTENTS is used,
               so we can free our copies. */
            free(name);
            free(contents);
        }
    }

    /* Fuzz curl_formget if formpost is not NULL */
    if (formpost) {
        int formget_ret = curl_formget(formpost, NULL, formget_callback);
        (void)formget_ret;
    }

    /* Cleanup formpost */
    curl_formfree(formpost);

    /* Fuzz curl_easy_setopt with CURLOPT_CLOSESOCKETFUNCTION */
    /* This exercises the type checking macro Wcurl_easy_setopt_err_close_socket_cb */
    curl_easy_setopt(curl, CURLOPT_CLOSESOCKETFUNCTION, closesocket_callback);
    curl_easy_setopt(curl, CURLOPT_CLOSESOCKETDATA, (void*)Data);

    /* Cleanup curl handle */
    curl_easy_cleanup(curl);

    return 0;
}